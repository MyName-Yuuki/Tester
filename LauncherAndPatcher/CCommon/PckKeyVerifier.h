#ifndef _PCK_KEY_VERIFIER_H_
#define _PCK_KEY_VERIFIER_H_

#include <windows.h>

// ===== KEY VERIFICATION FOR EXISTING PCK PROCESS =====

// Set verification key before processing
// ** SUPPORTS FLEXIBLE KEY FORMATS: **
// 1. Plain text: "RAHASIA123"
// 2. Base64: "UkFIQVNJQTEyMw==" (auto-decoded to "RAHASIA123")
// 3. Mixed: System auto-detects format
//
// Call this ONCE before any PCK operation
void PCK_SetKey(const char* szKey);

// Get current verification key (returns processed/decoded key)
const char* PCK_GetKey();

// Get original key input (before processing)
const char* PCK_GetOriginalKey();

// ===== VERIFY: Check if PCK file can be opened with current key =====
// Usage: Call this BEFORE OpenFilePackage()
// Input: szPckFile - Path to PCK file (e.g., "configs.pck")
// Output: TRUE if key is correct, FALSE if wrong
BOOL PCK_VerifyKey(const char* szPckFile);

// ===== DECOMPRESS: Decompress PCK data with key verification =====
// Usage: Call instead of AFilePackage::Uncompress()
// Input:  pCompressedData - Data from PCK
//         dwCompSize      - Compressed size
//         szPckFile       - Which PCK file (for key check)
// Output: pOutput         - Decompressed data
//         pdwOutputSize   - Decompressed size
BOOL PCK_DecompressWithKey(
    const BYTE* pCompressedData,
    DWORD dwCompSize,
    const char* szPckFile,
    BYTE* pOutput,
    DWORD* pdwOutputSize
);

// ===== COMPRESS: Compress data with key for PCK =====
// Usage: Call instead of AFilePackage::Compress()
// Input:  pData       - Original data
//         dwDataSize  - Data size
//         szPckFile   - Which PCK file (uses key)
// Output: pOutput     - Compressed data
//         pdwOutSize  - Compressed size
BOOL PCK_CompressWithKey(
    const BYTE* pData,
    DWORD dwDataSize,
    const char* szPckFile,
    BYTE* pOutput,
    DWORD* pdwOutSize
);

// ===== PATCH/UPDATE: Patch PCK file with key =====
// Usage: Call when updating PCK file
// Input:  szPckFilePath  - Path to PCK file
//         szInternalPath  - File inside PCK to update
//         pNewData        - New data
//         dwNewSize       - Size of new data
// Output: TRUE if patch successful (key verified)
BOOL PCK_PatchWithKey(
    const char* szPckFilePath,
    const char* szInternalPath,
    const BYTE* pNewData,
    DWORD dwNewSize
);

// ===== HELPER: Check if PCK has key protection =====
// Input: szPckFile - PCK filename
// Output: TRUE if PCK is protected with key
BOOL PCK_HasKeyProtection(const char* szPckFile);

// ===== FLEXIBLE: Process multiple PCK files =====
// Usage: Process different PCK files based on what's available
// Input: pPckList      - Array of PCK filenames
//         dwCount       - Number of files
// Output: Number of successfully processed files
DWORD PCK_ProcessMultiple(
    const char** pPckList,
    DWORD dwCount
);

// ===== KEY FORMAT HELPERS =====

// Check if a string is valid base64 format
BOOL PCK_IsBase64(const char* szStr);

// Decode base64 string
// Input:  szInput    - Base64 encoded string
// Output: szOutput   - Decoded string
//         iOutSize   - Output buffer size
// Returns: Number of bytes decoded, or 0 on error
int PCK_DecodeBase64(const char* szInput, char* szOutput, int iOutSize);

#endif
