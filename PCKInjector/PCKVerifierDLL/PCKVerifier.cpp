// PCKVerifier.dll - Standalone PCK Key Verification System
// Can be injected into any process to verify .pck files

#include <windows.h>
#include <stdio.h>
#include <string.h>

// ===== EXPORTS =====
extern "C" {
    __declspec(dllexport) void PCK_SetKey(const char* szKey);
    __declspec(dllexport) BOOL PCK_VerifyKey(const char* szPckFile);
    __declspec(dllexport) BOOL PCK_HasKeyProtection(const char* szPckFile);
    __declspec(dllexport) const char* PCK_GetVersion();
}

// ===== CONSTANTS =====
#define PCK_MAGIC_NUMBER 0x50434B50  // "PCKP"
#define PCK_HEADER_SIZE 8
#define HASH_SIZE 8

// ===== GLOBAL STATE =====
static char g_szPckKey[512] = {0};
static char g_szProcessedKey[256] = {0};
static BOOL g_bKeySet = FALSE;

// ===== BASE64 DECODING =====

static char Base64DecodeChar(char c)
{
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '/') return 63;
    return -1;
}

static int Base64Decode(const char* szInput, char* szOutput, int iOutSize)
{
    int iLen = strlen(szInput);
    int iOutLen = 0;

    for (int i = 0; i < iLen; i += 4)
    {
        if (iOutLen + 3 >= iOutSize) break;

        char c0 = Base64DecodeChar(szInput[i]);
        char c1 = Base64DecodeChar(szInput[i+1]);
        char c2 = (i+2 < iLen) ? Base64DecodeChar(szInput[i+2]) : 0;
        char c3 = (i+3 < iLen) ? Base64DecodeChar(szInput[i+3]) : 0;

        if (c0 == -1 || c1 == -1) break;

        szOutput[iOutLen++] = (c0 << 2) | (c1 >> 4);

        if (c2 != -1)
            szOutput[iOutLen++] = ((c1 & 0x0F) << 4) | (c2 >> 2);

        if (c3 != -1)
            szOutput[iOutLen++] = ((c2 & 0x03) << 6) | c3;
    }

    szOutput[iOutLen] = '\0';
    return iOutLen;
}

// ===== HASH COMPUTATION =====

static void ComputeHash(const char* szKey, unsigned char* hash)
{
    memset(hash, 0, HASH_SIZE);

    int iKeyLen = strlen(szKey);
    if (iKeyLen == 0) return;

    unsigned char acc = 0;
    for (int i = 0; i < iKeyLen && i < HASH_SIZE; i++)
    {
        acc ^= szKey[i];
        hash[i % HASH_SIZE] = szKey[i] ^ (i * 17) ^ acc;
    }

    // Second pass
    for (int i = 0; i < HASH_SIZE; i++)
    {
        hash[i] ^= (hash[i] << 3) | (hash[i] >> 5);
        hash[i] ^= iKeyLen;
    }
}

// ===== KEY SETTING =====

void PCK_SetKey(const char* szKey)
{
    if (!szKey || strlen(szKey) == 0)
    {
        strcpy(g_szPckKey, "");
        g_szProcessedKey[0] = '\0';
        g_bKeySet = FALSE;
        return;
    }

    strncpy(g_szPckKey, szKey, sizeof(g_szPckKey) - 1);
    g_szPckKey[sizeof(g_szPckKey) - 1] = '\0';

    // Try base64 decode first
    int iDecodeLen = Base64Decode(szKey, g_szProcessedKey, sizeof(g_szProcessedKey) - 1);

    // Check if decode was successful (valid ASCII text result)
    BOOL bIsBase64 = TRUE;
    if (iDecodeLen > 0)
    {
        for (int i = 0; i < iDecodeLen; i++)
        {
            if (g_szProcessedKey[i] < 32 || g_szProcessedKey[i] > 126)
            {
                bIsBase64 = FALSE;
                break;
            }
        }
    }
    else
    {
        bIsBase64 = FALSE;
    }

    // If not valid base64 or decode failed, use raw key
    if (!bIsBase64 || iDecodeLen == 0)
    {
        strncpy(g_szProcessedKey, szKey, sizeof(g_szProcessedKey) - 1);
        g_szProcessedKey[sizeof(g_szProcessedKey) - 1] = '\0';
    }

    g_bKeySet = TRUE;
}

// ===== PROTECTION DETECTION =====

BOOL PCK_HasKeyProtection(const char* szPckFile)
{
    if (!szPckFile) return FALSE;

    FILE* pFile = fopen(szPckFile, "rb");
    if (!pFile) return FALSE;

    unsigned char header[PCK_HEADER_SIZE];
    size_t iRead = fread(header, 1, PCK_HEADER_SIZE, pFile);
    fclose(pFile);

    if (iRead != PCK_HEADER_SIZE) return FALSE;

    // Check magic number (little-endian)
    unsigned int magic = *(unsigned int*)header;
    if (magic != PCK_MAGIC_NUMBER) return FALSE;

    // Check if hash is non-zero (indicates protection)
    unsigned char* hash = header + 4;
    BOOL bHasHash = FALSE;
    for (int i = 0; i < HASH_SIZE; i++)
    {
        if (hash[i] != 0)
        {
            bHasHash = TRUE;
            break;
        }
    }

    return bHasHash;
}

// ===== KEY VERIFICATION =====

BOOL PCK_VerifyKey(const char* szPckFile)
{
    if (!szPckFile) return FALSE;
    if (!g_bKeySet || g_szProcessedKey[0] == '\0') return TRUE; // No key = accept all

    FILE* pFile = fopen(szPckFile, "rb");
    if (!pFile) return FALSE;

    unsigned char header[PCK_HEADER_SIZE];
    size_t iRead = fread(header, 1, PCK_HEADER_SIZE, pFile);
    fclose(pFile);

    if (iRead != PCK_HEADER_SIZE) return FALSE;

    // Check magic number
    unsigned int magic = *(unsigned int*)header;
    if (magic != PCK_MAGIC_NUMBER)
    {
        // Not a protected .pck file, allow
        return TRUE;
    }

    // Extract hash from file
    unsigned char fileHash[HASH_SIZE];
    memcpy(fileHash, header + 4, HASH_SIZE);

    // Check if hash is all zeros (unprotected file)
    BOOL bIsProtected = FALSE;
    for (int i = 0; i < HASH_SIZE; i++)
    {
        if (fileHash[i] != 0)
        {
            bIsProtected = TRUE;
            break;
        }
    }

    if (!bIsProtected)
    {
        // Unprotected file, allow
        return TRUE;
    }

    // Compute hash from our key
    unsigned char keyHash[HASH_SIZE];
    ComputeHash(g_szProcessedKey, keyHash);

    // Compare hashes
    BOOL bMatch = TRUE;
    for (int i = 0; i < HASH_SIZE; i++)
    {
        if (fileHash[i] != keyHash[i])
        {
            bMatch = FALSE;
            break;
        }
    }

    return bMatch;
}

// ===== VERSION INFO =====

const char* PCK_GetVersion()
{
    return "PCK Verifier DLL v1.0 - Standalone";
}

// ===== DLL ENTRY POINT =====

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            // Initialize when loaded
            break;

        case DLL_PROCESS_DETACH:
            // Cleanup when unloaded
            break;
    }
    return TRUE;
}
