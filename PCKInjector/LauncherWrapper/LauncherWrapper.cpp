// LauncherWrapper.cpp - Inject PCK verification into PWDownloader.exe
// This wrapper:
// 1. Loads PCKVerifier.dll
// 2. Sets the verification key
// 3. Launches PWDownloader.exe
// 4. Hooks file operations to verify .pck files

#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>
#include <string>

// ===== FUNCTION TYPES =====

typedef void (*PFN_PCKSetKey)(const char* szKey);
typedef BOOL (*PFN_PCKVerifyKey)(const char* szPckFile);
typedef BOOL (*PFN_PCKHasProtection)(const char* szPckFile);

// ===== CONFIGURATION =====

const char* DEFAULT_KEY = "RAHASIA123";
const char* TARGET_EXE = "PWDownloader.exe";
const char* DLL_NAME = "PCKVerifier.dll";

// ===== LOGGING =====

void Log(const char* szMsg)
{
    FILE* pFile = fopen("launcher_log.txt", "a");
    if (pFile)
    {
        SYSTEMTIME st;
        GetLocalTime(&st);
        fprintf(pFile, "[%02d:%02d:%02d.%03d] %s\n",
                st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, szMsg);
        fclose(pFile);
    }
}

// ===== DLL INJECTION =====

bool InjectDLL(HANDLE hProcess, const char* szDllPath)
{
    size_t iDllPathLen = strlen(szDllPath) + 1;

    // Allocate memory in target process
    LPVOID pRemoteMem = VirtualAllocEx(hProcess, NULL, iDllPathLen,
                                       MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!pRemoteMem)
    {
        Log("ERROR: VirtualAllocEx failed");
        return false;
    }

    // Write DLL path to target process
    if (!WriteProcessMemory(hProcess, pRemoteMem, szDllPath, iDllPathLen, NULL))
    {
        Log("ERROR: WriteProcessMemory failed");
        VirtualFreeEx(hProcess, pRemoteMem, 0, MEM_RELEASE);
        return false;
    }

    // Get LoadLibraryA address
    HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");
    LPVOID pLoadLibrary = (LPVOID)GetProcAddress(hKernel32, "LoadLibraryA");

    // Create remote thread to load DLL
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0,
                                        (LPTHREAD_START_ROUTINE)pLoadLibrary,
                                        pRemoteMem, 0, NULL);
    if (!hThread)
    {
        Log("ERROR: CreateRemoteThread failed");
        VirtualFreeEx(hProcess, pRemoteMem, 0, MEM_RELEASE);
        return false;
    }

    // Wait for injection
    WaitForSingleObject(hThread, INFINITE);

    // Cleanup
    CloseHandle(hThread);
    VirtualFreeEx(hProcess, pRemoteMem, 0, MEM_RELEASE);

    Log("DLL injected successfully");
    return true;
}

// ===== FIND PROCESS BY NAME =====

DWORD FindProcessId(const char* szProcessName)
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) return 0;

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hSnapshot, &pe32))
    {
        CloseHandle(hSnapshot);
        return 0;
    }

    DWORD dwPid = 0;
    do
    {
        if (_stricmp(pe32.szExeFile, szProcessName) == 0)
        {
            dwPid = pe32.th32ProcessID;
            break;
        }
    } while (Process32Next(hSnapshot, &pe32));

    CloseHandle(hSnapshot);
    return dwPid;
}

// ===== MAIN WRAPPER LOGIC =====

int main(int argc, char* argv[])
{
    Log("=== Launcher Wrapper Started ===");

    // Get current directory
    char szDir[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, szDir);

    // Build DLL path
    char szDllPath[MAX_PATH];
    snprintf(szDllPath, MAX_PATH, "%s\\%s", szDir, DLL_NAME);

    // Check if DLL exists
    if (GetFileAttributesA(szDllPath) == INVALID_FILE_ATTRIBUTES)
    {
        Log("ERROR: PCKVerifier.dll not found!");
        MessageBoxA(NULL, "PCKVerifier.dll not found!\nPlease ensure the DLL is in the same directory.",
                     "Launcher Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    // Build target EXE path
    char szTargetPath[MAX_PATH];
    snprintf(szTargetPath, MAX_PATH, "%s\\%s", szDir, TARGET_EXE);

    // Check if target EXE exists
    if (GetFileAttributesA(szTargetPath) == INVALID_FILE_ATTRIBUTES)
    {
        Log("ERROR: PWDownloader.exe not found!");
        MessageBoxA(NULL, "PWDownloader.exe not found!\nPlease ensure it's in the same directory.",
                     "Launcher Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    // Load PCK Verifier DLL in our process first
    HMODULE hDll = LoadLibraryA(szDllPath);
    if (!hDll)
    {
        Log("ERROR: Failed to load PCKVerifier.dll");
        MessageBoxA(NULL, "Failed to load PCKVerifier.dll", "Launcher Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    // Get function pointers
    PFN_PCKSetKey pSetKey = (PFN_PCKSetKey)GetProcAddress(hDll, "PCK_SetKey");
    PFN_PCKVerifyKey pVerifyKey = (PFN_PCKVerifyKey)GetProcAddress(hDll, "PCK_VerifyKey");

    if (!pSetKey)
    {
        Log("ERROR: PCK_SetKey function not found");
        MessageBoxA(NULL, "PCK_SetKey function not found in DLL", "Launcher Error", MB_OK | MB_ICONERROR);
        FreeLibrary(hDll);
        return 1;
    }

    // Set the key
    char szKey[512];
    snprintf(szKey, sizeof(szKey), "%s", DEFAULT_KEY);
    pSetKey(szKey);

    Log("Key set successfully");

    // Launch target process as suspended
    STARTUPINFOA si = {0};
    PROCESS_INFORMATION pi = {0};
    si.cb = sizeof(si);

    char szCmdLine[MAX_PATH];
    snprintf(szCmdLine, MAX_PATH, "\"%s\"", szTargetPath);

    Log("Launching PWDownloader.exe...");

    if (!CreateProcessA(szTargetPath, szCmdLine, NULL, NULL, FALSE,
                       CREATE_SUSPENDED, NULL, szDir, &si, &pi))
    {
        Log("ERROR: Failed to create process");
        MessageBoxA(NULL, "Failed to launch PWDownloader.exe", "Launcher Error", MB_OK | MB_ICONERROR);
        FreeLibrary(hDll);
        return 1;
    }

    Log("Process created, injecting DLL...");

    // Inject DLL into target process
    if (!InjectDLL(pi.hProcess, szDllPath))
    {
        Log("DLL injection failed, terminating process");
        TerminateProcess(pi.hProcess, 1);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        FreeLibrary(hDll);
        return 1;
    }

    // Resume main thread
    Log("Resuming process...");
    ResumeThread(pi.hThread);

    // Wait for process to exit
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Get exit code
    DWORD dwExitCode;
    GetExitCodeProcess(pi.hProcess, &dwExitCode);

    // Cleanup
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    FreeLibrary(hDll);

    char szMsg[256];
    snprintf(szMsg, sizeof(szMsg), "PWDownloader.exe exited with code: %d", dwExitCode);
    Log(szMsg);

    Log("=== Launcher Wrapper Ended ===");
    return 0;
}
