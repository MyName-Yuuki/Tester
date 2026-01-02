// PCKVerifier.h - Header for PCK Verifier DLL

#ifndef PCK_VERIFIER_H
#define PCK_VERIFIER_H

#ifdef PCK_VERIFIER_EXPORTS
#define PCK_API __declspec(dllexport)
#else
#define PCK_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Set the verification key (plain text or base64)
PCK_API void PCK_SetKey(const char* szKey);

// Verify if a .pck file matches the current key
// Returns TRUE if verification passes or file is unprotected
PCK_API BOOL PCK_VerifyKey(const char* szPckFile);

// Check if a .pck file has key protection
PCK_API BOOL PCK_HasKeyProtection(const char* szPckFile);

// Get version string
PCK_API const char* PCK_GetVersion();

#ifdef __cplusplus
}
#endif

#endif // PCK_VERIFIER_H
