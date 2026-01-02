# Patcher Key Verification Integration

## 📋 Summary

**Patcher.exe sekarang sudah dilengkapi dengan key verification untuk file .pck!**

Sistem akan otomatis mendeteksi dan memverifikasi key saat proses update/extract file .pck.

---

## 🔄 ALUR PROSES UPDATE (DENGAN KEY VERIFICATION)

### Flow Chart:
```
1. Download .rar file (P2SP)
   ↓
2. Extract .rar file
   ↓
3. Untuk SETIAP file yang diextract:
   ├─ File biasa → Langsung extract (DEFAULT)
   └─ File .pck → VERIFIKASI KEY dulu!
       ├─ Jika .pck lama tidak ada → Langsung extract
       ├─ Jika .pck lama ada TANPA key → Langsung extract
       └─ Jika .pck lama ada DENGAN key → Verifikasi key
           ├─ Key BENAR → Lanjut extract/overwrite
           └─ Key SALAH → Error, batalkan proses!
```

---

## 🔧 MODIFIKASI YANG DILAKUKAN

### 1. **EC_Archive7Z.cpp** ([ClientTools/PWDownloader/EC_Archive7Z.cpp](ClientTools/PWDownloader/EC_Archive7Z.cpp))

**Tambah include:**
```cpp
#include "..\..\CCommon\PckKeyVerifier.h"
#include <string>
#include <cstring>
```

**Tambah logika verifikasi di `GetStream()` ([line 348-373](ClientTools/PWDownloader/EC_Archive7Z.cpp#L348-L373)):**
```cpp
// Cek apakah file yang akan diextract adalah .pck
std::string strFileName = GetAnsiString(m_sPathInArchive);
if( strFileName.length() > 4 &&
    stricmp(strFileName.c_str() + strFileName.length() - 4, ".pck") == 0 )
{
    // File .pck terdeteksi!
    // Cek apakah file .pck yang sudah ada punya key protection
    NFile::NFind::CFileInfoW fi;
    if( fi.Find(fullProcessedPath) )
    {
        // File .pck lama ada, cek apakah punya key protection
        if( PCK_HasKeyProtection(strFileName.c_str()) )
        {
            // Verifikasi key sebelum overwrite
            if( !PCK_VerifyKey(strFileName.c_str()) )
            {
                // ERROR: Key salah!
                MessageBoxA(NULL, "Key verification failed! Cannot update .pck file without correct key.",
                           "PCK Protection Error", MB_OK|MB_ICONERROR);
                return E_ABORT;
            }
        }
    }
}
// Lanjut extract...
```

### 2. **PWDownloader.dsp** ([ClientTools/PWDownloader/PWDownloader.dsp](ClientTools/PWDownloader/PWDownloader.dsp))

**Tambah PckKeyVerifier.cpp ke build ([line 470](ClientTools/PWDownloader/PWDownloader.dsp#L470)):**
```
SOURCE=..\..\CCommon\PckKeyVerifier.cpp
# SUBTRACT CPP /YX /Yc /Yu
```

**Tambah include path untuk CCommon:**
- Release config ([line 46](ClientTools/PWDownloader/PWDownloader.dsp#L46)):
  ```
  /I "../../CCommon"
  ```
- Debug config ([line 72](ClientTools/PWDownloader/PWDownloader.dsp#L72)):
  ```
  /I "../../CCommon"
  ```

---

## 📝 CARA MENGGUNAKAN

### Sebelum Build/Run Patcher:

```cpp
// Di awal aplikasi (misal di PWDownloaderDlg::OnInitDialog)
PCK_SetKey("RAHASIA123"); // Set key verification

// Sekarang semua proses extract .pck akan otomatis verifikasi key!
```

### Saat Proses Update Berjalan:

**Jika .pck TIDAK punya key protection:**
```
✅ Extract lancar tanpa verifikasi
```

**Jika .pck punya key protection:**
```
✅ Key benar → Extract berhasil
❌ Key salah → MessageBox error: "Key verification failed!"
```

---

## 🎯 SCENARIO CONTOH

### Scenario 1: Update Normal (.pck TANPA key)
```
1. Download w2i_clientV1246.rar
2. Extract → configs.pck (TANPA key)
3. ✅ configs.pck langsung dioverwrite
4. Selesai!
```

### Scenario 2: Update .pck DENGAN Key Protection
```
1. Download w2i_clientV1247.rar
2. Extract → configs.pck (DENGAN key)
3. Cek: configs.pck lama punya key protection?
   - Ya → Cek key dengan PCK_VerifyKey()
     - Key benar → ✅ Overwrite configs.pck
     - Key salah → ❌ Error, batalkan update
   - Tidak → ✅ Langsung overwrite
```

### Scenario 3: Fresh Install (.pck lama TIDAK ada)
```
1. Download w2i_client_full.rar
2. Extract → configs.pck
3. Cek: configs.pck lama ada?
   - Tidak → ✅ Langsung extract
4. Selesai!
```

---

## 🔍 LOKASI FILE

| File | Path | Deskripsi |
|------|------|-----------|
| PckKeyVerifier.h | [CCommon/PckKeyVerifier.h](CCommon/PckKeyVerifier.h) | Header untuk key verification |
| PckKeyVerifier.cpp | [CCommon/PckKeyVerifier.cpp](CCommon/PckKeyVerifier.cpp) | Implementasi key verification |
| EC_Archive7Z.cpp | [ClientTools/PWDownloader/EC_Archive7Z.cpp](ClientTools/PWDownloader/EC_Archive7Z.cpp) | Extract dengan key verification |
| PWDownloader.dsp | [ClientTools/PWDownloader/PWDownloader.dsp](ClientTools/PWDownloader/PWDownloader.dsp) | Build configuration |

---

## ⚠️ PENTING!

### Yang BERUBA:
- ✅ Proses extract file .pck sekarang otomatis verifikasi key
- ✅ Jika key salah, proses update DIBATALKAN
- ✅ Tidak mengganggu proses file selain .pck

### yang TIDAK BERUBA:
- ✅ Proses download (P2SP) tetap sama
- ✅ Proses extract file biasa tetap sama
- ✅ Hanya menambahkan verifikasi saat extract .pck

---

## 🛠️ BUILD INSTRUCTIONS

### Build dengan Visual Studio 6.0/2003:

1. Buka workspace: `LauncherAndPatcher/ClientTools/PWDownloader/PWDownloader.dsw`
2. Pilih configuration: **Win32 Release** atau **Win32 Debug**
3. Build → **Rebuild All**
4. Output: `Release/PWDownloader.exe` atau `Debug/PWDownloader_d.exe`

### Dependencies:
- `CCommon/PckKeyVerifier.cpp` (sudah ditambahkan ke .dsp)
- `CCommon/PckKeyVerifier.h` (sudah di-include)
- Angelica Framework (AFilePackage, dll)
- 7-Zip Library

---

## 📚 REFERENSI KODE

- **Download Process**: [EC_PWDownload.cpp:296-326](ClientTools/PWDownloader/EC_PWDownload.cpp#L296-L326)
- **Install Process**: [PWInstallDlg.cpp:94-113](ClientTools/PWDownloader/PWInstallDlg.cpp#L94-L113)
- **Extract Process**: [EC_Archive7Z.cpp:348-395](ClientTools/PWDownloader/EC_Archive7Z.cpp#L348-L395)
- **Key Verification**: [CCommon/PckKeyVerifier.cpp](CCommon/PckKeyVerifier.cpp)

---

**Last Updated:** 2026-01-01
**Status:** ✅ INTEGRATION COMPLETE
**Build:** Ready to compile
