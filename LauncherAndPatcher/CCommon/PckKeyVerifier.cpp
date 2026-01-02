#include "PckKeyVerifier.h"
#include <string.h>
#include <ctype.h>

// Global key storage
static char g_szPckKey[512] = {0};  // Increased buffer size
static char g_szProcessedKey[256] = {0};  // Final processed key
static BOOL g_bKeySet = FALSE;

// ===== BASE64 DECODING =====

static BOOL IsBase64(const char* szStr)
{
    if (!szStr || strlen(szStr) == 0)
        return FALSE;

    // Check if string contains only valid base64 characters
    int iValidCount = 0;
    int iTotalCount = 0;

    for (int i = 0; szStr[i]; i++)
    {
        char c = szStr[i];

        // Skip whitespace
        if (isspace(c))
            continue;

        iTotalCount++;

        // Check for base64 charset
        if ((c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z') ||
            (c >= '0' && c <= '9') ||
            c == '+' || c == '/' || c == '=')
        {
            iValidCount++;
        }
        else
        {
            // Invalid character found
            return FALSE;
        }
    }

    // Must be at least 4 characters and 90% valid
    return (iTotalCount >= 4 && iValidCount >= (iTotalCount * 90 / 100));
}

static char Base64DecodeChar(char c)
{
    if (c >= 'A' && c <= 'Z')
        return c - 'A';
    if (c >= 'a' && c <= 'z')
        return c - 'a' + 26;
    if (c >= '0' && c <= '9')
        return c - '0' + 52;
    if (c == '+')
        return 62;
    if (c == '/')
        return 63;
    return 0;
}

static int DecodeBase64(const char* szInput, char* szOutput, int iOutSize)
{
    if (!szInput || !szOutput || iOutSize <= 0)
        return 0;

    int iLen = strlen(szInput);
    int iOutLen = 0;

    for (int i = 0; i < iLen; i += 4)
    {
        // Get 4 characters
        char c1 = (i < iLen) ? Base64DecodeChar(szInput[i]) : 0;
        char c2 = (i + 1 < iLen) ? Base64DecodeChar(szInput[i + 1]) : 0;
        char c3 = (i + 2 < iLen) ? Base64DecodeChar(szInput[i + 2]) : 0;
        char c4 = (i + 3 < iLen) ? Base64DecodeChar(szInput[i + 3]) : 0;

        // Check for padding
        BOOL bPad2 = (szInput[i + 2] == '=');
        BOOL bPad3 = (szInput[i + 3] == '=');

        // Decode to 3 bytes
        if (iOutLen + 3 < iOutSize)
        {
            szOutput[iOutLen++] = (c1 << 2) | (c2 >> 4);
            if (!bPad2)
                szOutput[iOutLen++] = ((c2 & 0x0F) << 4) | (c3 >> 2);
            if (!bPad2 && !bPad3)
                szOutput[iOutLen++] = ((c3 & 0x03) << 6) | c4;
        }
    }

    szOutput[iOutLen] = '\0';
    return iOutLen;
}

// ===== KEY DETECTION & PROCESSING =====

static void ProcessKey(const char* szInputKey)
{
    if (!szInputKey || strlen(szInputKey) == 0)
    {
        g_szProcessedKey[0] = '\0';
        return;
    }

    // Copy original key
    strncpy(g_szPckKey, szInputKey, 511);
    g_szPckKey[511] = '\0';

    // Try to detect and decode base64
    if (IsBase64(szInputKey))
    {
        // Decode base64
        int iDecodedLen = DecodeBase64(szInputKey, g_szProcessedKey, 255);

        // Check if decoded result is valid text (printable)
        BOOL bValidText = TRUE;
        for (int i = 0; i < iDecodedLen; i++)
        {
            if (!isprint(g_szProcessedKey[i]) && !isspace(g_szProcessedKey[i]))
            {
                bValidText = FALSE;
                break;
            }
        }

        // If decoded to valid text, use decoded version
        if (bValidText && iDecodedLen > 0)
        {
            g_szProcessedKey[iDecodedLen] = '\0';
            // Keep decoded key
        }
        else
        {
            // Not valid text after decode, use original key
            strncpy(g_szProcessedKey, szInputKey, 255);
            g_szProcessedKey[255] = '\0';
        }
    }
    else
    {
        // Not base64, use as-is
        strncpy(g_szProcessedKey, szInputKey, 255);
        g_szProcessedKey[255] = '\0';
    }

    g_bKeySet = TRUE;
}

// ===== KEY MANAGEMENT =====

void PCK_SetKey(const char* szKey)
{
    if (szKey && strlen(szKey) > 0)
    {
        ProcessKey(szKey);
    }
}

const char* PCK_GetKey()
{
    return g_bKeySet ? g_szProcessedKey : NULL;
}

const char* PCK_GetOriginalKey()
{
    return g_bKeySet ? g_szPckKey : NULL;
}

// ===== VERIFY: Check key before opening PCK =====

BOOL PCK_VerifyKey(const char* szPckFile)
{
    if (!szPckFile)
        return FALSE;

    // If no key set, allow opening (backward compatible)
    if (!g_bKeySet || strlen(g_szProcessedKey) == 0)
        return TRUE;

    // Check if file exists
    FILE* pFile = fopen(szPckFile, "rb");
    if (!pFile)
        return FALSE;

    // Read first 4 bytes to check for protection marker
    DWORD dwMagic;
    fread(&dwMagic, 1, sizeof(DWORD), pFile);
    fclose(pFile);

    // If not protected, allow opening
    if (dwMagic != 0x50434B50)  // "PCKP" magic
        return TRUE;

    // File is protected! Verify key
    // Generate hash from PROCESSED key (decoded if base64)
    DWORD dwKeyHash = 0;
    for (int i = 0; g_szProcessedKey[i]; i++)
    {
        dwKeyHash ^= (DWORD)(g_szProcessedKey[i] << (i % 24));
    }

    // Read key hash from file
    pFile = fopen(szPckFile, "rb");
    fseek(pFile, 4, SEEK_SET);  // Skip magic
    DWORD dwStoredHash;
    fread(&dwStoredHash, 1, sizeof(DWORD), pFile);
    fclose(pFile);

    // Compare hashes
    return (dwKeyHash == dwStoredHash);
}

// ===== DECOMPRESS: With key verification =====

BOOL PCK_DecompressWithKey(
    const BYTE* pCompressedData,
    DWORD dwCompSize,
    const char* szPckFile,
    BYTE* pOutput,
    DWORD* pdwOutputSize)
{
    // Verify key first
    if (!PCK_VerifyKey(szPckFile))
        return FALSE;

    // If protected, decrypt first
    if (g_bKeySet && strlen(g_szProcessedKey) > 0)
    {
        // Check if data is encrypted
        // (Check protection flag in data)
        DWORD* pdwFlags = (DWORD*)(pCompressedData + 8);  // Offset to flags

        if (*pdwFlags & 0x01)  // Encrypted flag
        {
            // Decrypt with PROCESSED key (decoded if base64)
            DWORD dwKeyLen = strlen(g_szProcessedKey);
            for (DWORD i = 0; i < dwCompSize; i++)
            {
                pOutput[i] = pCompressedData[i] ^ g_szProcessedKey[i % dwKeyLen];
            }

            // Now decompress
            // Use existing decompression (ZLIB or AFilePackage)
            // ...
            *pdwOutputSize = dwCompSize;
            return TRUE;
        }
    }

    // Not encrypted, decompress normally
    // Call existing decompression function here
    // AFilePackage::Uncompress(...);

    *pdwOutputSize = dwCompSize;
    return TRUE;
}

// ===== COMPRESS: With key =====

BOOL PCK_CompressWithKey(
    const BYTE* pData,
    DWORD dwDataSize,
    const char* szPckFile,
    BYTE* pOutput,
    DWORD* pdwOutSize)
{
    // Compress first
    // Call existing compression function
    // AFilePackage::Compress(...);

    DWORD dwCompSize = dwDataSize;  // Placeholder

    // If key set, encrypt compressed data with PROCESSED key
    if (g_bKeySet && strlen(g_szProcessedKey) > 0)
    {
        DWORD dwKeyLen = strlen(g_szProcessedKey);
        for (DWORD i = 0; i < dwCompSize; i++)
        {
            pOutput[i] = pOutput[i] ^ g_szProcessedKey[i % dwKeyLen];
        }
    }

    *pdwOutSize = dwCompSize;
    return TRUE;
}

// ===== PATCH/UPDATE: With key =====

BOOL PCK_PatchWithKey(
    const char* szPckFilePath,
    const char* szInternalPath,
    const BYTE* pNewData,
    DWORD dwNewSize)
{
    // Verify key before patching
    if (!PCK_VerifyKey(szPckFilePath))
    {
        // Wrong key! Cannot patch
        return FALSE;
    }

    // Key is correct, proceed with patch
    // TODO: Call existing patch function
    // This is where your existing patch code goes

    return TRUE;
}

// ===== HELPER: Check if PCK is protected =====

BOOL PCK_HasKeyProtection(const char* szPckFile)
{
    if (!szPckFile)
        return FALSE;

    FILE* pFile = fopen(szPckFile, "rb");
    if (!pFile)
        return FALSE;

    DWORD dwMagic;
    fread(&dwMagic, 1, sizeof(DWORD), pFile);
    fclose(pFile);

    return (dwMagic == 0x50434B50);  // "PCKP"
}

// ===== FLEXIBLE: Process multiple PCK files =====

DWORD PCK_ProcessMultiple(
    const char** pPckList,
    DWORD dwCount)
{
    if (!pPckList || dwCount == 0)
        return 0;

    DWORD dwSuccess = 0;

    for (DWORD i = 0; i < dwCount; i++)
    {
        const char* szPckFile = pPckList[i];

        // Check if this PCK file exists
        if (!PCK_VerifyKey(szPckFile))
        {
            // Skip this file (wrong key or not found)
            continue;
        }

        // Process this PCK file
        // Your existing PCK processing code here
        dwSuccess++;
    }

    return dwSuccess;
}

// ===== PUBLIC HELPERS FOR BASE64 =====

// Check if string is valid base64
BOOL PCK_IsBase64(const char* szStr)
{
    return IsBase64(szStr);
}

// Decode base64 string
int PCK_DecodeBase64(const char* szInput, char* szOutput, int iOutSize)
{
    if (!szInput || !szOutput || iOutSize <= 0)
        return 0;

    if (!IsBase64(szInput))
        return 0;

    return DecodeBase64(szInput, szOutput, iOutSize);
}
