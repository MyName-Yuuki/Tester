# Flexible Key Format Documentation

## 📋 Summary

Sistem key verification sekarang **FLEXIBLE** - bisa menerima key dalam format:
1. **Plain Text** (text biasa)
2. **Base64** (otomatis terdeteksi dan didecode)
3. **Mixed** (sistem otomatis mendeteksi formatnya)

---

## 🔑 CARA KERJA SISTEM KEY

### Flow Deteksi Key:
```
Input Key
   ↓
Cek: Apakah format Base64?
   ↓
   ├─ YA → Decode Base64
   │         ↓
   │      Cek: Hasil decode valid text?
   │         ↓
   │      ├─ YA → Gunakan hasil decode
   │      └─ TIDAK → Gunakan input asli
   │
   └─ TIDAK → Gunakan input asli (plain text)
```

### Contoh:

#### Contoh 1: Plain Text
```cpp
PCK_SetKey("RAHASIA123");
// Output: "RAHASIA123" (digunakan apa adanya)
```

#### Contoh 2: Base64
```cpp
PCK_SetKey("UkFIQVNJQTEyMw==");
// Output: "RAHASIA123" (decoded dari base64)
```

#### Contoh 3: Base64 dengan Spasi
```cpp
PCK_SetKey("UkFIQVNJQTEyMw==   ");
// Output: "RAHASIA123" (whitespace diabaikan, tetap terdeteksi base64)
```

---

## 🔍 ALGORITMA DETEKSI BASE64

### Rule Deteksi:

**String dianggap Base64 jika:**
1. Panjang ≥ 4 karakter
2. Minimal 90% karakter valid base64
3. Hanya berisi karakter:
   - `A-Z` (huruf besar)
   - `a-z` (huruf kecil)
   - `0-9` (angka)
   - `+` (plus)
   - `/` (slash)
   - `=` (sama dengan untuk padding)
   - Whitespace (diabaikan)

### Contoh Valid Base64:
```
✅ "UkFIQVNJQTEyMw=="         // Valid: hasil decode "RAHASIA123"
✅ "SGVsbG8gV29ybGQ=="       // Valid: hasil decode "Hello World"
✅ "YWJjZGVmZ2hpams="        // Valid: hasil decode "abcdefghijk"
✅ "VGVzdDEyMw=="             // Valid: hasil decode "Test123"
```

### Contoh TIDAK Valid Base64:
```
❌ "RAHASIA123"               // Tidak valid: plain text
❌ "Hello @World"             // Tidak valid: ada karakter '@'
❌ "Test#Key"                 // Tidak valid: ada karakter '#'
❌ "Abc!123"                  // Tidak valid: ada karakter '!'
```

---

## 💻 CONTOH PENGGUNAAN

### Basic Usage:

```cpp
#include "PckKeyVerifier.h"

// ===== SET KEY (FLEXIBLE) =====

// Cara 1: Plain text
PCK_SetKey("RAHASIA123");

// Cara 2: Base64
PCK_SetKey("UkFIQVNJQTEyMw==");

// Cara 3: Dari file/config
char szKey[256];
ReadKeyFromFile("key.txt", szKey);  // Bisa plain atau base64
PCK_SetKey(szKey);

// ===== CEK KEY =====

// Get processed key (setelah decode)
const char* szProcessed = PCK_GetKey();      // "RAHASIA123"

// Get original key (input asli)
const char* szOriginal = PCK_GetOriginalKey();  // "UkFIQVNJQTEyMw==" atau "RAHASIA123"

// ===== VERIFIKASI PCK =====

if (PCK_VerifyKey("configs.pck"))
{
    // Key benar, buka file
    g_AFilePackMan.OpenFilePackageInGame("configs.pck");
}
else
{
    // Key salah!
    MessageBox(NULL, "Key salah!", "Error", MB_OK);
}

// ===== HELPER FUNCTIONS =====

// Cek apakah string adalah base64
if (PCK_IsBase64("UkFIQVNJQTEyMw=="))
{
    // Ini adalah base64
}

// Decode base64 manual
char szDecoded[256];
int iLen = PCK_DecodeBase64("UkFIQVNJQTEyMw==", szDecoded, 256);
// szDecoded = "RAHASIA123", iLen = 10
```

---

## 📊 TABLE CONVERSION

### Plain Text → Base64:

| Plain Text | Base64 Encoded |
|------------|----------------|
| `RAHASIA123` | `UkFIQVNJQTEyMw==` |
| `Test123` | `VGVzdDEyMw==` |
| `Hello` | `SGVsbG8=` |
| `password` | `cGFzc3dvcmQ=` |
| `MyKey2024` | `TXlLZXkyMDI0` |
| `Secret!` | `U2VjcmV0IQ==` |

### Base64 → Plain Text (Auto Decode):

| Base64 Input | Decoded Output | Used by System |
|--------------|----------------|----------------|
| `UkFIQVNJQTEyMw==` | `RAHASIA123` | ✅ "RAHASIA123" |
| `VGVzdDEyMw==` | `Test123` | ✅ "Test123" |
| `SGVsbG8=` | `Hello` | ✅ "Hello" |
| `RAHASIA123` | *(not base64)* | ✅ "RAHASIA123" (as-is) |

---

## 🧪 TESTING SCENARIO

### Scenario 1: Input Plain Text
```cpp
PCK_SetKey("RAHASIA123");

// Internal process:
// 1. IsBase64("RAHASIA123") → FALSE
// 2. Gunakan as-is
// 3. g_szProcessedKey = "RAHASIA123"
// 4. g_szPckKey = "RAHASIA123"

PCK_GetKey();          // "RAHASIA123"
PCK_GetOriginalKey();  // "RAHASIA123"
```

### Scenario 2: Input Base64
```cpp
PCK_SetKey("UkFIQVNJQTEyMw==");

// Internal process:
// 1. IsBase64("UkFIQVNJQTEyMw==") → TRUE
// 2. DecodeBase64() → "RAHASIA123"
// 3. Cek valid text → TRUE
// 4. g_szProcessedKey = "RAHASIA123"
// 5. g_szPckKey = "UkHIQVNJQTEyMw=="

PCK_GetKey();          // "RAHASIA123" (decoded)
PCK_GetOriginalKey();  // "UkFIQVNJQTEyMw==" (original)
```

### Scenario 3: Input Invalid Base64 (Binary Data)
```cpp
PCK_SetKey("YWJjZGVmZ2hpams=");

// Internal process:
// 1. IsBase64("YWJjZGVmZ2hpams=") → TRUE
// 2. DecodeBase64() → [0x61,0x62,0x63,...] (binary)
// 3. Cek valid text → FALSE (binary data)
// 4. Gunakan original key
// 5. g_szProcessedKey = "YWJjZGVmZ2hpams="
// 6. g_szPckKey = "YWJjZGVmZ2hpams="

PCK_GetKey();          // "YWJjZGVmZ2hpams=" (original, karena binary)
PCK_GetOriginalKey();  // "YWJjZGVmZ2hpams="
```

---

## 🎯 BEST PRACTICE

### Untuk Pengguna:

1. **Gunakan Plain Text** untuk key yang mudah diingat
   ```cpp
   PCK_SetKey("RahasiaKu2024");
   ```

2. **Gunakan Base64** untuk menyimpan key di config file (lebih aman visually)
   ```cpp
   // Di config file:
   // KEY=UmFoYXNpYUt1MjAyNA==
   PCK_SetKey("UmFoYXNpYUt1MjAyNA==");
   ```

3. **Jangan campur format** dalam satu aplikasi
   ```cpp
   // ❌ JANGAN:
   PCK_SetKey("RAHASIA123");          // Set key
   PCK_SetKey("UkFIQVNJQTEyMw==");   // Reset dengan base64 sama

   // ✅ GUNAKAN:
   PCK_SetKey("RAHASIA123");          // Pilih satu format saja
   ```

### Untuk Developer:

1. **Debug Mode**: Cek tipe key
   ```cpp
   PCK_SetKey(szInput);

   if (PCK_IsBase64(szInput))
   {
       printf("Input: Base64 format\n");
       printf("Original: %s\n", PCK_GetOriginalKey());
       printf("Decoded: %s\n", PCK_GetKey());
   }
   else
   {
       printf("Input: Plain text format\n");
       printf("Key: %s\n", PCK_GetKey());
   }
   ```

2. **Config File**: Simpan key dalam format yang konsisten
   ```ini
   [PCKProtection]
   KeyFormat=BASE64    // atau PLAIN
   Key=UkFIQVNJQTEyMw==
   ```

---

## 📝 IMPLEMENTATION DETAILS

### Functions:
- `ProcessKey()` - Proses key input dan auto-detect format
- `IsBase64()` - Cek apakah string valid base64
- `DecodeBase64()` - Decode base64 ke plain text
- `PCK_SetKey()` - Set key dengan auto-detection
- `PCK_GetKey()` - Get processed key (decoded)
- `PCK_GetOriginalKey()` - Get original input
- `PCK_IsBase64()` - Public helper untuk cek base64
- `PCK_DecodeBase64()` - Public helper untuk decode

### Storage:
```cpp
static char g_szPckKey[512];        // Original input (sebelum decode)
static char g_szProcessedKey[256];  // Final processed key (setelah decode)
static BOOL g_bKeySet;              // Flag apakah key sudah diset
```

---

## 🔐 SECURITY NOTES

1. **Base64 BUKAN Encryption**
   - Base64 hanya encoding, bukan encryption
   - Mudah di-decode oleh siapa saja
   - Gunakan hanya untuk obfuscation visual

2. **Key Storage**
   - Jangan hardcode key di source code
   - Simpan di config terpisay
   - Atau minta input dari user

3. **Key Strength**
   - Gunakan minimal 8 karakter
   - Campur huruf besar, kecil, angka
   - Contoh: "MySec2024Key!"

---

## 📚 REFERENSI

- **Base64 Standard**: RFC 4648
- **Header**: [CCommon/PckKeyVerifier.h](CCommon/PckKeyVerifier.h)
- **Implementation**: [CCommon/PckKeyVerifier.cpp](CCommon/PckKeyVerifier.cpp)
- **Integration**: [PATCHER_KEY_INTEGRATION.md](PATCHER_KEY_INTEGRATION.md)

---

**Last Updated:** 2026-01-01
**Status:** ✅ FLEXIBLE KEY SYSTEM IMPLEMENTED
**Supported Formats:** Plain Text, Base64
