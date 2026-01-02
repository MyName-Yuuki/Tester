// ElementHintLauncher.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../../CElementHintTool/MyLog.h"

#include <windows.h>
#include <tchar.h>
#include <time.h>
#include <mmsystem.h>

HANDLE  g_hMutex = NULL;
const char *g_szMutex = "WMGJ_ElementHintLauncher";
const char *g_szLogFileName = "wmgj_HintLauncher.log";
const char *g_szChildProcessName = "wmgj_HintTool.exe";

bool initInstance();
void clearInstance();

int main(int argc, char* argv[])
{
	if (!initInstance())
		return -1;

	//	��ȡ��ǰ��ִ�г��������ļ���
	char curPath[MAX_PATH] = {0};
	DWORD dwLength = ::GetModuleFileNameA(NULL, curPath, MAX_PATH);
	if (dwLength == 0)
	{
		printf("Unable to get exe file path!\n");
		return -1;
	}
	while (dwLength>0
		&& curPath[dwLength-1] != '\\'
		&& curPath[dwLength-1] != '/')
	{
		curPath[--dwLength] = '\0';
	}
	if (dwLength == 0)
	{
		printf("Unable to get exe file path!\n");
		return -2;
	}

	//	��ʼ����־�ļ�
	char szLogFile[MAX_PATH] = {0};
	strcat(szLogFile, curPath);
	strcat(szLogFile, g_szLogFileName);
	if (!MyLog::GetInstance().Init(szLogFile, "wmgj Element Hint Tool Launcher log file create(or open)"))
	{
		printf("Unable to init log file!\n");
		return -3;
	}

	//	����Ŀ�����·��
	char szChildPath[MAX_PATH] = {0};
	strcat(szChildPath, curPath);
	strcat(szChildPath, g_szChildProcessName);
	
	//	���ô�����ģʽΪ����ģʽ����ʹ�ӽ��̵Ĵ����������Ի���
	SetErrorMode(SEM_FAILCRITICALERRORS);
	
	//	���ϳ��Դ���Ŀ�����
	while (true)
	{
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		
		ZeroMemory( &si, sizeof(si) );
		si.cb = sizeof(si);
		ZeroMemory( &pi, sizeof(pi) );

		DWORD dwTime1 = timeGetTime();
		
		// �������ػ�����
		if (!CreateProcess(szChildPath,
			"",
			NULL,			// Process handle not inheritable. 
			NULL,			// Thread handle not inheritable. 
			FALSE,			// Set handle inheritance to FALSE. 
			0,				// No creation flags. 
			NULL,			// Use parent's environment block. 
			NULL,			// Use parent's starting directory. 
			&si,			// Pointer to STARTUPINFO structure.
			&pi)			// Pointer to PROCESS_INFORMATION structure.
			)
		{
			//	����ʧ�ܣ��˳�
			MyLog::GetInstance().Log("Create child process failed, launcher is now to exit!");
			break;
		}
		else
		{
			//	�����ɹ�
			MyLog::GetInstance().Log("Create child process success!");
			
			//	�ȴ��ӽ����˳�
			DWORD dwResult = WaitForSingleObject(pi.hProcess, INFINITE);
			if (dwResult == WAIT_FAILED)
			{
				//	�ȴ��ӽ����˳����󣬿����ӽ������ڵ�ǰ��������
				DWORD dwLastError = GetLastError();
				MyLog::GetInstance().Log("Wait for child process exit failed(LastError: %08x), launcher is now to exit!", dwLastError);
				break;
			}

			//	�ӽ����˳�

			//	��ѯ�����˳�����
			DWORD dwExitCode(0);
			if (GetExitCodeProcess(pi.hProcess, &dwExitCode))
			{
				//	��ѯ�ɹ�
				MyLog::GetInstance().Log("Child process Exited(ExitCode: %08x)!", dwExitCode);
			}
			else
			{
				//	��ѯ�˳�����ʧ��
				MyLog::GetInstance().Log("Child process Exited(Unable to get Exitcode)!");
			}
			
			//	�ر��ӽ��̾��
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);

			//	����Ƿ��Ǵ������ü��˳�
			DWORD dwTime2 = timeGetTime();
			DWORD dwTimeDelta = (dwTime2<dwTime1) ? 0 : (dwTime2-dwTime1);
			if (dwTimeDelta < 10000)
			{
				//	�ǣ����ػ����������⣨��ȱ��DLL���������ٽ��г���
				MyLog::GetInstance().Log("Child process exit too quickly(in %d ms), launcher is now to exit!", dwTimeDelta);
				break;
			}
		}

		//	˯�ߺ��Ժ������´���
		Sleep(1000);
	}

	MyLog::GetInstance().Log("Log is to be closed!");
	clearInstance();
	return 0;
}

bool initInstance()
{
	//	����Ƿ��ж��ʵ������
	g_hMutex = CreateMutexA(NULL, TRUE, g_szMutex);
	DWORD dwLastError = GetLastError();
	if (dwLastError == ERROR_ALREADY_EXISTS)
	{
		//	�Ѿ�����һ��ʵ��
		printf("An ElementHintLauncher instance already exists!");
		CloseHandle(g_hMutex);
		return false;
	}
	if (!g_hMutex)
	{
		printf("Create instance mutex failed!");
		return false;
	}
	return true;
}

void clearInstance()
{
	if (g_hMutex)
	{
		CloseHandle(g_hMutex);
		g_hMutex = NULL;
	}
}