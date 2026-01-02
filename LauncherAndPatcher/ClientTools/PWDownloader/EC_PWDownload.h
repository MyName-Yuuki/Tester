/*
 * FILE: EC_PWDownload.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Shizhenhua, 2013/7/18
 *
 * HISTORY: 
 *
 * Copyright (c) 2011 ACE Studio, All Rights Reserved.
 */


#pragma once

#include <vector>
#include <string>
#include "i_p2sp_download.h"


///////////////////////////////////////////////////////////////////////////
//  
//  Define and Macro
//  
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////

class CECPipe;


///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//  
//  class CECPWDownload
//  
///////////////////////////////////////////////////////////////////////////

class CECPWDownload
{
	// �����߳�
	friend DWORD WINAPI P2SPDownloadThread(LPVOID lpParameter);

	class CScopedLock
	{
	public:
		CScopedLock(CRITICAL_SECTION* cs) : m_cs(cs) { EnterCriticalSection(cs); }
		~CScopedLock() { LeaveCriticalSection(m_cs); }
	private:
		CRITICAL_SECTION* m_cs;
	};

	struct DOWNLOAD_INFO
	{
		std::string md5;
		std::string filename;
		int downspeed;
		int upspeed;
	};

public:
	virtual ~CECPWDownload();

	// ��ʼ��/����������
	bool Init(HWND hWndClient);
	void Release();

	// ��ʼ����
	bool StartDownload();

	// ֹͣ����
	void StopDownload();

	// ��Ӧ������Ϸ
	void OnEnterOrLeaveGame(bool bEnter);

	// ��ʼ��װ
	void Install();

	// �˳�����
	void Quit();

	// �����߼�
	void Tick();

	// ��ȡ��ǰ����״̬
	bool GetTaskInfo(float& fProgress, float& fDownSpeed, float& fUpSpeed);

	// ���������ٶ�
	bool SetDownSpeed(int downspeed, int upspeed);

	// ��ȡ��ǰ���õ������ٶ�
	bool GetMaxSpeed(int& downspeed, int& upspeed);

	// ��ȡ�ܵ�ָ��
	CECPipe* GetPipe() { return m_pPipe; }

	// �Ƿ���������
	bool IsDownloading() const { return m_hDownloadThread != NULL; }

	// �Ƿ��������
	bool IsDownloadOK() const { return m_bDownloadOK; }

	// ��ȡ������
	int GetErrorCode();

	// ��ȡʵ��
	static CECPWDownload& GetInstance();

protected:
	HMODULE m_hP2SPDLL;
	std::string m_sServerAddr;			// MD5�ļ����ط�����
	HANDLE m_hDownloadThread;			// �����߳�
	bool m_bP2SPInitOK;					// P2SP���ʼ�����
	CRITICAL_SECTION m_csInfoLock;		// ������Ϣ����
	TaskInfomationT m_DownloadInfo;		// ��ǰ������Ϣ
	DOWNLOAD_INFO m_LastDownInfo;		// �ϴ����ص���Ϣ
	CECPipe* m_pPipe;					// ��ͻ���ͨ�ŵĹܵ�

	volatile bool m_bDownloadOK;		// �������
	volatile bool m_bStopThread;		// ֹͣ�����߳�

protected:
	CECPWDownload();

	// ��ʼ��p2sp����
	bool p2sp_Start(int gameID);
	
	// ���û�ȡ����
	bool p2sp_Setting(const SettingItemT* setting);
	bool p2sp_GetSetting(SettingItemT* setting);
	bool p2sp_SetLsp(LSP_SETTING_FLAG flag);

	// ����һ����������
	bool p2sp_AddDownload(const char* filemd5);
	
	// ��ʼһ����������
	bool p2sp_StartDownload(const char* filemd5);
	
	// ֹͣ��������
	bool p2sp_StopDownload(const char* filemd5);
	
	// ɾ����������
	bool p2sp_DeleteDownload(const char* filemd5);
	
	// ��ȡ������Ϣ
	bool p2sp_GetTaskInformation(TaskInfomationT* info);
	
	// ֹͣp2sp����
	bool p2sp_Stop();

	// ��ȡ��ǰ���·������ĵ�ַ
	bool GetUpdateServer();

	// ��ȡ�����ؿͻ��˵�MD5
	bool GetFullClientMD5(std::string& strMD5, std::string& strFile);

	// ��ȡ�ϴ����ص���Ϣ
	bool GetLastClientDL();

	// д��δ��ɵĿͻ���������Ϣ
	void WriteLastClientDL();

	// ����P2SP����
	bool StartP2SPEngine();
};

///////////////////////////////////////////////////////////////////////////
//  
//  Inline Functions
//  
///////////////////////////////////////////////////////////////////////////
