/*
 * FILE: EC_PWDownload.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Shizhenhua, 2013/7/18
 *
 * HISTORY: 
 *
 * Copyright (c) 2011 ACE Studio, All Rights Reserved.
 */


#include "StdAfx.h"
#include "../../CCommon/EC_HttpGet.h" // Using stub header
#include "EC_PWDownload.h"
#include "EC_Pipe.h"

#include "LogFile.h"
#include <io.h>


///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
	#define P2SP_DLL_PATH "p2sp_4th_libD.dll"
#else
	#define P2SP_DLL_PATH "p2sp_4th_lib.dll"
#endif

// 60���ڲ���P2SP�ɴﵽ������ٶ�
#define MAX_TIME_TESTSPEED 60000

#define FULLCLIENT_PACK_MD5 "patches/w2i_client_full.rar.MD5"


///////////////////////////////////////////////////////////////////////////
//	
//	Local Types and Variables and Global variables
//	
///////////////////////////////////////////////////////////////////////////

const int GAME_ID = 3;		// ������Ϊ3

const char g_szServerList[] = "server/updateserver.txt";
const char g_szCurServerCfg[] = "../config/patcher/update.cfg";
const char g_szTempFile2[] = "../config/element/tempfile2.dat";
char g_szLastClientMD5[] = "../config/element/lastclientmd5.dat";
char g_szDownloadDir[] = "../config/element/";
char g_szClientCompleted[MAX_PATH];


///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////

void UDeleteFile(const char* filename)
{
	SetFileAttributes(filename, FILE_ATTRIBUTE_NORMAL);
	DeleteFile(filename);
}

// HTTP�����ļ�
static void DownloadFileByHttp(const char* srcfile, const char* destfile, PATCH::callback_t callback,
							   unsigned int start, int trytimes = 5)
{
	int cur_try = 0;
	FILE* pFile = NULL;

	do
	{
		PATCH::DownloadManager::GetSingleton().GetByUrl(srcfile, destfile, callback, start);

		int i = 0;
		while( i <= 10 && !(pFile = fopen(destfile, "rb")) )
		{
			Sleep(30);
			i++;
		}

		++cur_try;
	}
	while( (pFile != NULL) && cur_try < trytimes );

	if( pFile )
		fclose( pFile );
}

// ��ȡ�ļ���һ���ı�
static bool GetLine(FILE* pFile, wchar_t* szLine)
{
	if( !fgetws(szLine, 512, pFile) )
		return false;
	wchar_t* pEnd = wcsstr(szLine, L"\r\n");
	if( pEnd ) *pEnd = L'\0';
	return true;
}

// �ָ��ı�
static void SplitString(const std::wstring& str, const wchar_t* split, abase::vector<std::wstring>& strList)
{
	strList.clear();
	wchar_t* temp = new wchar_t[str.length()+1];
	wcscpy(temp, str.c_str());

	wchar_t *pCur = temp, *p = NULL;
	while(1)
	{
		p = wcsstr(pCur, split);
		if(p) *p = L'\0';
		if(wcslen(pCur) > 0) strList.push_back(pCur);
		if(!p) break;
		pCur = p + wcslen(split);
	}

	delete[] temp;
}

// ANSIתUNICODE
static std::wstring AS2WS(const char* str)
{
	std::wstring ret;
	if( str )
	{
		int sLen = (int)strlen(str);
		int iLen = MultiByteToWideChar(CP_ACP, 0, str, sLen, NULL, 0);
		wchar_t* buffer = new wchar_t[iLen+1];
		MultiByteToWideChar(CP_ACP, 0, str, sLen, buffer, iLen);
		buffer[iLen] = L'\0';
		ret = buffer;
		delete[] buffer;
	}

	return ret;
}

// UNICODEתANSI
static std::string WS2AS(const wchar_t* str)
{
	std::string ret;
	if( str )
	{
		int sLen = (int)wcslen(str);
		int iLen = WideCharToMultiByte(CP_ACP, 0, str, sLen, NULL, 0, NULL, NULL);
		char* buffer = new char[iLen+1];
		WideCharToMultiByte(CP_ACP, 0, str, sLen, buffer, iLen, NULL, NULL);
		buffer[iLen] = '\0';
		ret = buffer;
		delete[] buffer;
	}
	
	return ret;
}

struct _hash_wstring
{
	_hash_wstring() {}
	_hash_wstring(const _hash_wstring & hf) {}
		
	inline unsigned long operator() (const std::wstring& str) const
	{
		unsigned long h = 0;
		for(size_t i=0; i<str.length(); i++)
		{
			h = h * 31 + str[i];
		}
		return h;
	}
};


///////////////////////////////////////////////////////////////////////////
//  
//  Implement class CECPWDownload
//  
///////////////////////////////////////////////////////////////////////////

// ��ȡʵ��
CECPWDownload& CECPWDownload::GetInstance()
{
	static CECPWDownload obj;
	return obj;
}

// ���캯��
CECPWDownload::CECPWDownload()
{
	m_hP2SPDLL = NULL;
	m_hDownloadThread = NULL;
	m_bP2SPInitOK = false;
	memset(&m_DownloadInfo, 0, sizeof(TaskInfomationT));
	m_bDownloadOK = false;
	m_bStopThread = false;

	InitializeCriticalSection(&m_csInfoLock);
}

// ��������
CECPWDownload::~CECPWDownload()
{
	DeleteCriticalSection(&m_csInfoLock);
}

// ��ʼ��������
bool CECPWDownload::Init(HWND hWndClient)
{
	// ����P2SP��̬��
	m_hP2SPDLL = LoadLibrary(P2SP_DLL_PATH);
	if( !m_hP2SPDLL )
	{
		assert(0);
		LogOutput("CECPWDownload::Init, Can't load the %s!", P2SP_DLL_PATH);
		return false;
	}

	// �����ܵ�
	m_pPipe = new CECPipe();
	if( !m_pPipe->Init(hWndClient) )
	{
		LogOutput("CECPWDownload::Init, Create the pipe failed!");
		return false;
	}

	// ֪ͨ�ͻ��ˣ��ҵĴ��ھ��
	::PostMessage(hWndClient, WM_SENDPIPEDATA, 1, (LPARAM)AfxGetMainWnd()->GetSafeHwnd());

	// ��ȡ�ϴ�������Ϣ������ϴ����ع�����ν�����ʾ���ֱ������
	GetLastClientDL();
	return true;
}

// �ͷ�������
void CECPWDownload::Release()
{
	// �ȴ��߳̽���
	m_bStopThread = true;
	WaitForSingleObject(m_hDownloadThread, INFINITE);

	if( m_pPipe )
	{
		delete m_pPipe;
		m_pPipe = NULL;
	}

	if( m_hDownloadThread )
	{
		CloseHandle(m_hDownloadThread);
		m_hDownloadThread = NULL;
	}

	if( m_bP2SPInitOK )
	{
		p2sp_Stop();
		FreeLibrary(m_hP2SPDLL);
	}
}

// ��ʼ����
bool CECPWDownload::StartDownload()
{
	// ��ȡ�ϴ����ؿͻ��˵�MD5���ļ����������ϴ��ٶ�
	GetLastClientDL();

	// ����Ƿ��Ѿ��������
	if( !m_LastDownInfo.filename.empty() )
	{
		std::string strClientFile = std::string(g_szDownloadDir) + m_LastDownInfo.filename;
		if( _access(strClientFile.c_str(), 0) != -1 )
		{
			// ֱ�ӽ��밲װ�׶�
			m_bDownloadOK = true;
			strcpy(g_szClientCompleted, g_szDownloadDir);
			strcat(g_szClientCompleted, m_LastDownInfo.filename.c_str());
			return true;
		}
	}

	// ��ȡ���ط�������ַ
	if( !GetUpdateServer() )
	{
		AfxMessageBox("�������ط�������ַʧ�ܣ�");
		LogOutput("CECPWDownload::StartDownload, Can't get the update server!");
		return false;
	}

	// ��ȡ�ͻ���MD5�ļ�
//	std::string strMD5 = "674d7d0ad24470414241dbf10058a620";
//	std::string strFile = "w2i_clientV1246.rar";
	std::string strMD5, strFile;
	GetFullClientMD5(strMD5, strFile);
	if( strMD5.empty() )
	{
		AfxMessageBox("���ؿͻ���MD5�ļ�ʧ�ܣ�");
		LogOutput("CECPWDownload::StartDownload, Get the md5 file of the full client failed!");
		return false;
	}

	if( !m_LastDownInfo.md5.empty() && m_LastDownInfo.md5 != strMD5 )
	{
		// ɾ��δ��������Ͽͻ����ļ�
		std::string strFileNotCompleted = std::string(g_szDownloadDir) + m_LastDownInfo.filename + ".pd";
		UDeleteFile(strFileNotCompleted.c_str());
	}

	m_LastDownInfo.md5 = strMD5;
	m_LastDownInfo.filename = strFile;
	strcpy(g_szClientCompleted, g_szDownloadDir);
	strcat(g_szClientCompleted, strFile.c_str());

	// ��ʼ��P2SP����
	if( !m_bP2SPInitOK && StartP2SPEngine() )
		m_bP2SPInitOK = true;

	// ���������߳�
	if( m_bP2SPInitOK )
	{
		m_hDownloadThread = CreateThread(NULL, 0, P2SPDownloadThread, this, 0, NULL);
	}

	return true;
}

// ��ʼ��p2sp����
bool CECPWDownload::StartP2SPEngine()
{
	if( p2sp_Start(GAME_ID) )
	{
		wchar_t szWorkPath[MAX_PATH];
		GetCurrentDirectoryW(MAX_PATH, szWorkPath);
		std::wstring strPackDir(szWorkPath);
		size_t nPos = strPackDir.find_last_of(L'\\');
		strPackDir = strPackDir.substr(0, nPos) + L"\\config\\element";

		SettingItemT setting;
		setting.mask = P2P_SETTING_ITEM_FILE_SAVE_PATH | P2P_SETTING_ITEM_MAX_DOWNLOAD_SPEED | P2P_SETTING_ITEM_MAX_UPLOAD_SPEED;
		setting.maxDownloadSpeed = 0;
		setting.maxUploadSpeed = 0;
		lstrcpyW(setting.savePath, strPackDir.c_str());
		LogOutput(WS2AS(setting.savePath).c_str());
		p2sp_Setting(&setting);
		LogOutput("Start p2sp engine successfully!");

		// �����������ȵ����ط�ʽ
		p2sp_SetLsp(LSP_SETTING_PRIORITY_BROWSE);
		Sleep(5000);
		return true;
	}
	else
	{
		AfxMessageBox("��ʼ��p2sp��������ʧ�ܣ�");
		LogOutput("CECPWDownload::StartP2SPEngine, Initialize the p2sp failed!");
	}

	return false;
}

// �����߳�
DWORD WINAPI P2SPDownloadThread(LPVOID lpParameter)
{
	CECPWDownload* pDownload = (CECPWDownload*)lpParameter;

	std::string strMD5 = pDownload->m_LastDownInfo.md5;
	if( pDownload->p2sp_AddDownload(strMD5.c_str()) )
	{
		LogOutput("Add download task ok!");
		if( pDownload->p2sp_StartDownload(strMD5.c_str()) )
		{
			LogOutput("Start download task ok!");
			strcpy(pDownload->m_DownloadInfo.md5, strMD5.c_str());

			while(1)
			{
				if( pDownload->m_bStopThread )
				{
					pDownload->p2sp_StopDownload(strMD5.c_str());
					break;
				}

				{
					CECPWDownload::CScopedLock lock(&pDownload->m_csInfoLock);

					if( pDownload->p2sp_GetTaskInformation(&pDownload->m_DownloadInfo) )
					{
						if( P2P_TASK_STATE_COMPLETE == pDownload->m_DownloadInfo.state )
						{
							// ���سɹ�
							pDownload->m_bDownloadOK = true;
							LogOutput("P2SPDownloadThread, Download OK!!!!!!");
							break;
						}
						else if( P2P_TASK_STATE_ERROR == pDownload->m_DownloadInfo.state )
						{
							LogOutput("P2SPDownloadThread, Download the client failed! P2P_TASK_STATE_ERROR:%d", pDownload->m_DownloadInfo.err);
							break;
						}
					}
				}

				Sleep(1000);
			}
		}
		else
		{
			LogOutput("P2SPDownloadThread, Start the download task failed!");
		}
	}
	else
	{
		LogOutput("P2SPDownloadThread, Add the download task failed!");
	}

	pDownload->WriteLastClientDL();
	return 0;
}

// ֹͣ����
void CECPWDownload::StopDownload()
{
	m_bStopThread = true;
	memset(&m_DownloadInfo, 0, sizeof(TaskInfomationT));

	if( m_hDownloadThread )
	{
		CloseHandle(m_hDownloadThread);
		m_hDownloadThread = NULL;
	}
}

// ��Ӧ������Ϸ
void CECPWDownload::OnEnterOrLeaveGame(bool bEnter)
{
	if( bEnter )
	{
		if( !m_LastDownInfo.md5.empty() )
		{
			if( IsDownloading() )
			{
				LogOutput("CECPWDownload::OnEnterOrLeaveGame, The program is already downloading!");
				return;
			}

			StartDownload();
			SetDownSpeed(m_LastDownInfo.downspeed, m_LastDownInfo.upspeed);
		}
	}
	else
	{
		StopDownload();
	}
}

// ��ʼ��װ����
void CECPWDownload::Install()
{
	if( m_bDownloadOK )
	{
		m_bStopThread = true;
		AfxGetMainWnd()->PostMessage(WM_COMMAND, IDOK, 0);
	}
}

// �˳�����
void CECPWDownload::Quit()
{
	m_bStopThread = true;
	AfxGetMainWnd()->PostMessage(WM_COMMAND, IDCANCEL, 0);
}

// ���µ�ǰ״̬
void CECPWDownload::Tick()
{
	static DWORD dwLastTime = GetTickCount();

	if( m_pPipe )
		m_pPipe->Tick();
}

// ��ȡ��ǰ����״̬
bool CECPWDownload::GetTaskInfo(float& fProgress, float& fDownSpeed, float& fUpSpeed)
{
	CScopedLock lock(&m_csInfoLock);
	if( m_bDownloadOK )
		fProgress = 1.0f;
	else
	{
		if( m_DownloadInfo.size > 0 )
			fProgress = (float)(__int64)m_DownloadInfo.complateSize / (__int64)m_DownloadInfo.size;
		else
			fProgress = 0.0f;
	}
	fDownSpeed = (float)m_DownloadInfo.dspeed / 1024;
	fUpSpeed = (float)m_DownloadInfo.uspeed / 1024;
	return true;
}

// �������غ��ϴ��ٶ�
bool CECPWDownload::SetDownSpeed(int downspeed, int upspeed)
{
	if( m_bP2SPInitOK )
	{
		SettingItemT setting;
		memset(&setting, 0, sizeof(SettingItemT));
		setting.mask = P2P_SETTING_ITEM_MAX_DOWNLOAD_SPEED | P2P_SETTING_ITEM_MAX_UPLOAD_SPEED;
		setting.maxDownloadSpeed = downspeed;
		setting.maxUploadSpeed = upspeed;
		if( !p2sp_Setting(&setting) )
		{
			LogOutput("CECPWDownload::SetDownSpeed, Adjust the down and up speed failed!");
			return false;
		}

		if( IsDownloading() )
			WriteLastClientDL();
		return true;
	}

	return false;
}

// ��ȡ��ǰ���õ�����ٶ�
bool CECPWDownload::GetMaxSpeed(int& downspeed, int& upspeed)
{
	if( m_bP2SPInitOK )
	{
		SettingItemT setting;
		if( p2sp_GetSetting(&setting) )
		{
			downspeed = setting.maxDownloadSpeed;
			upspeed = setting.maxUploadSpeed;
			return true;
		}
	}

	return false;
}

// ��ȡ��ǰ���ط������ĵ�ַ��ȡ��Patcher��ͬ�ķ�����
bool CECPWDownload::GetUpdateServer()
{
	FILE* pFile;
	wchar_t szLine[512];
	abase::vector<std::wstring> strList;
	abase::hash_map<std::wstring, std::wstring, _hash_wstring> updateServers;

	if( !(pFile = fopen(g_szServerList, "rb")) )
	{
		LogOutput("CECPWDownload::GetUpdateServer, Can't load the update server list file!");
		return false;
	}

	fgetwc(pFile);	// ����UNICODE���
	while(GetLine(pFile, szLine))
	{
		wchar_t szName[40], szAddr[200];
		int i, j, iLen = wcslen(szLine);

		i = 0;
		while( i < iLen && szLine[i] != L'\"' ) i++;
		if( i == iLen ) continue;
		i++;
		j = i;
		while( i < iLen && szLine[i] != L'\"' ) i++;
		if( i == iLen ) continue;
		wcsncpy(szName, szLine + j, i - j);
		szName[i-j] = L'\0';

		i++;
		while( i < iLen && szLine[i] != L'\"' ) i++;
		if( i == iLen ) continue;
		i++;
		j = i;
		while( i < iLen && szLine[i] != L'\"' ) i++;
		if( i == iLen ) continue;
		wcsncpy(szAddr, szLine + j, i - j);
		szAddr[i-j] = L'\0';

		updateServers[szName] = szAddr;
	}

	fclose(pFile);

	// ���ص�ǰ��������
	if( !(pFile = fopen(g_szCurServerCfg, "rb")) )
	{
		LogOutput("CECPWDownload::GetUpdateServer, Can't load the current patcher config file!");
		return false;
	}

	std::wstring strCurName;
	fgetwc(pFile);	// ����UNICODE���
	while(GetLine(pFile, szLine))
	{
		SplitString(szLine, L" ", strList);
		if( strList.size() != 2 ) break;
		if( wcsicmp(strList[0].c_str(), L"UpdateServerSelect") ) continue;
		strCurName = strList[1];
	}

	abase::hash_map<std::wstring, std::wstring, _hash_wstring>::iterator it;
	it = updateServers.find(strCurName);
	if( it == updateServers.end() )
	{
		ASSERT(0);
		LogOutput("CECPWDownload::GetUpdateServer, Can't find the current server's address! (%s)", strCurName.c_str());
		return false;
	}

	m_sServerAddr = WS2AS(it->second.c_str());
	return true;
}

// ��ȡ�����ͻ��˵�MD5ֵ
bool CECPWDownload::GetFullClientMD5(std::string& strMD5, std::string& strFile)
{
	char szMD5[33], szFile[MAX_PATH];
	std::string strURL = m_sServerAddr + FULLCLIENT_PACK_MD5;
	UDeleteFile(g_szTempFile2);
	LogOutput(strURL.c_str());
	DownloadFileByHttp(strURL.c_str(), g_szTempFile2, NULL, 0);
	FILE* pFile = fopen(g_szTempFile2, "r");
	if( !pFile )
	{
		LogOutput("CECPWDownload::GetFullClientMD5, Download the md5 file of the client failed!");
		return false;
	}
	fscanf(pFile, "%s", szMD5);
	fscanf(pFile, "%s", szFile);
	fclose(pFile);
	UDeleteFile(g_szTempFile2);
	strMD5 = szMD5;
	strFile = szFile;
	return true;
}

// ��ȡ�ϴ����ؿͻ��˵���Ϣ
bool CECPWDownload::GetLastClientDL()
{
	char strMD5[33], strFile[MAX_PATH];
	FILE* pFile = fopen(g_szLastClientMD5, "r");
	if( !pFile ) return false;
	fscanf(pFile, "%s", strMD5);
	fscanf(pFile, "%s", strFile);
	fscanf(pFile, "%d", &m_LastDownInfo.downspeed);
	fscanf(pFile, "%d", &m_LastDownInfo.upspeed);
	fclose(pFile);
	m_LastDownInfo.md5 = strMD5;
	m_LastDownInfo.filename = strFile;
	return true;
}

// д��δ��ɵĿͻ���������Ϣ
void CECPWDownload::WriteLastClientDL()
{
	int dspeed, uspeed;
	FILE* pFile = fopen(g_szLastClientMD5, "w");
	if( !pFile ) return;
	GetMaxSpeed(dspeed, uspeed);
	fprintf(pFile, "%s %s %d %d", m_LastDownInfo.md5.c_str(), m_LastDownInfo.filename.c_str(), dspeed, uspeed);
	fclose(pFile);
}

// ��ʼ��p2sp����
bool CECPWDownload::p2sp_Start(int gameID)
{
	typedef bool (*DLL_P2SPSTART) (int gameID);

	if( m_hP2SPDLL )
	{
		DLL_P2SPSTART pFunc = (DLL_P2SPSTART)GetProcAddress(m_hP2SPDLL, "p2spStart");
		if( pFunc )
			return pFunc(gameID);
	}

	return false;
}

// ��������
bool CECPWDownload::p2sp_Setting(const SettingItemT* setting)
{
	typedef bool (*DLL_P2SPSETTING) (const SettingItemT* setting);

	if( m_hP2SPDLL )
	{
		DLL_P2SPSETTING pFunc = (DLL_P2SPSETTING)GetProcAddress(m_hP2SPDLL, "p2spSetting");
		if( pFunc )
			return pFunc(setting);
	}

	return false;
}

// ��ȡ����
bool CECPWDownload::p2sp_GetSetting(SettingItemT* setting)
{
	typedef bool (*DLL_P2SPGETSETTING) (SettingItemT* setting);

	if( m_hP2SPDLL )
	{
		DLL_P2SPGETSETTING pFunc = (DLL_P2SPGETSETTING)GetProcAddress(m_hP2SPDLL, "p2spGetSetting");
		if( pFunc )
			return pFunc(setting);
	}

	return false;
}

// �������ط�ʽ
bool CECPWDownload::p2sp_SetLsp(LSP_SETTING_FLAG flag)
{
	typedef bool (*DLL_P2SPSETLSP) (LSP_SETTING_FLAG flag);

	if( m_hP2SPDLL )
	{
		DLL_P2SPSETLSP pFunc = (DLL_P2SPSETLSP)GetProcAddress(m_hP2SPDLL, "p2spSetLsp");
		if( pFunc )
			return pFunc(flag);
	}

	return false;
}

// ����һ����������
bool CECPWDownload::p2sp_AddDownload(const char* filemd5)
{
	typedef bool (*DLL_P2SPADDDOWNLOAD) (const char* filemd5);

	if( m_hP2SPDLL )
	{
		DLL_P2SPADDDOWNLOAD pFunc = (DLL_P2SPADDDOWNLOAD)GetProcAddress(m_hP2SPDLL, "p2spAddDownload");
		if( pFunc )
			return pFunc(filemd5);
	}

	return false;
}

// ��ʼһ����������
bool CECPWDownload::p2sp_StartDownload(const char* filemd5)
{
	typedef bool (*DLL_P2SPSTARTDOWNLOAD) (const char* filemd5);

	if( m_hP2SPDLL )
	{
		DLL_P2SPSTARTDOWNLOAD pFunc = (DLL_P2SPSTARTDOWNLOAD)GetProcAddress(m_hP2SPDLL, "p2spStartDownload");
		if( pFunc )
			return pFunc(filemd5);
	}

	return false;
}

// ֹͣ��������
bool CECPWDownload::p2sp_StopDownload(const char* filemd5)
{
	typedef bool (*DLL_P2SPSTOPDOWNLOAD) (const char* filemd5);

	if( m_hP2SPDLL )
	{
		DLL_P2SPSTOPDOWNLOAD pFunc = (DLL_P2SPSTOPDOWNLOAD)GetProcAddress(m_hP2SPDLL, "p2spStopDownload");
		if( pFunc )
			return pFunc(filemd5);
	}

	return false;
}

// ɾ����������
bool CECPWDownload::p2sp_DeleteDownload(const char* filemd5)
{
	typedef bool (*DLL_P2SPDELETEDOWNLOAD) (const char* filemd5);

	if( m_hP2SPDLL )
	{
		DLL_P2SPDELETEDOWNLOAD pFunc = (DLL_P2SPDELETEDOWNLOAD)GetProcAddress(m_hP2SPDLL, "p2spDeleteDownload");
		if( pFunc )
			return pFunc(filemd5);
	}

	return false;
}

// ��ȡ������Ϣ
bool CECPWDownload::p2sp_GetTaskInformation(TaskInfomationT* info)
{
	typedef bool (*DLL_P2SPGETTASKINFORMATION) (TaskInfomationT* info);

	if( m_hP2SPDLL )
	{
		DLL_P2SPGETTASKINFORMATION pFunc = (DLL_P2SPGETTASKINFORMATION)GetProcAddress(m_hP2SPDLL, "p2spGetTaskInfomation");
		if( pFunc )
			return pFunc(info);
	}

	return false;
}

// ֹͣp2sp����
bool CECPWDownload::p2sp_Stop()
{
	typedef bool (*DLL_P2SPSTOP) ();

	if( m_hP2SPDLL )
	{
		DLL_P2SPSTOP pFunc = (DLL_P2SPSTOP)GetProcAddress(m_hP2SPDLL, "p2spStop");
		if( pFunc )
			return pFunc();
	}

	return false;
}

// ��ȡ���صĴ�����
int CECPWDownload::GetErrorCode()
{
	CScopedLock lock(&m_csInfoLock);
	return m_DownloadInfo.err;
}