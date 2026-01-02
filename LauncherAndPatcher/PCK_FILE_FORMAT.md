# Format File .PCK dan Sistem Verifikasi

## 📋 Executive Summary

**✅ TIDAK ADA signature/key khusus untuk membaca file .pck!**

File .pck adalah **file archive terkompresi** yang bisa dibaca langsung tanpa authentication. Sistem menggunakan library **Angelica File Package (AFilePackage)** untuk handle file .pck.

---

## 🔓 Apa yang Dibutuhkan untuk Membaca .PCK?

### Jawaban: **TIDAK ADA!**

File .pck bisa dibaca langsung dengan memanggil fungsi:
```cpp
g_AFilePackMan.OpenFilePackageInGame("configs.pck");
g_AFilePackMan.OpenFilePackageInGame("models.pck");
```

**Tidak perlu:**
- ❌ Tidak ada encryption key
- ❌ Tidak ada signature verification
- ❌ Tidak ada password
- ❌ Tidak ada authentication token

---

## 📦 Struktur File .PCK

### Daftar File .PCK yang Digunakan

Berdasarkan kode [FileAnalyse.cpp:188-200](ClientTools/FileExtract/FileAnalyse.cpp#L188-L200):

```cpp
const char* szPckFiles[] =
{
    "configs.pck",     "configs",        // Konfigurasi game
    "facedata.pck",    "facedata",       // Data wajah karakter
    "models.pck",      "models",         // Model 3D (.ecm)
    "gfx.pck",         "gfx",            // Efek visual
    "interfaces.pck",  "interfaces",     // UI files
    "surfaces.pck",    "surfaces",       // Texture surfaces
    "grasses.pck",     "grasses",        // Grass data
    "sfx.pck",         "sfx",            // Sound effects
    "shaders.pck",     "shaders",        // Shader files
    "textures.pck",    "textures",       // Texture files
};
```

### Cara Membuka File .PCK

Contoh dari [ElementHintTool.cpp:487](CElementHintTool/ElementHintTool.cpp#L487):
```cpp
// Set working directory
GetCurrentDirectoryA(MAX_PATH, g_szWorkDir);
af_SetBaseDir(g_szWorkDir);

// Buka file .pck - TIDAK PERLU KEY/SIGNATURE!
g_AFilePackMan.OpenFilePackage("configs.pck");
```

Contoh dari [FileAnalyse.cpp:211-219](ClientTools/FileExtract/FileAnalyse.cpp#L211-L219):
```cpp
for( i=0; i<sizeof(szPckFiles)/sizeof(const char*)/2; i++ )
{
    // Coba buka dari .pck
    if( !g_AFilePackMan.OpenFilePackageInGame(szPckFiles[i*2])
        && _access(szPckFiles[i*2+1], 0) == -1 )  // Cek folder juga
    {
        // Error jika tidak bisa buka .pck DAN folder tidak ada
        SHOW_ERRMSG(ERR_OPENPCKFAIL);
        return false;
    }
}
```

---

## 🔧 Cara Membaca File dari Dalam .PCK

### Method 1: Otomatis via AFilePackage Manager

File bisa dibaca seperti file biasa. Jika file ada di .pck, sistem otomatis membacanya dari archive:

```cpp
// Membaca file dari .pck atau folder (otomatis)
AFileImage file;
if( file.Open("", "models\\character\\player.ecm",
              AFILE_OPENEXIST | AFILE_BINARY | AFILE_TEMPMEMORY) )
{
    // File berhasil dibuka!
    DWORD dwLen = file.GetFileLength();
    BYTE* pBuffer = new BYTE[dwLen];
    file.Read(pBuffer, dwLen, &dwReadLen);
    file.Close();
}
```

### Method 2: Manual Extract

Contoh dari [FileAnalyse.cpp:1589-1616](ClientTools/FileExtract/FileAnalyse.cpp#L1589-L1616):

```cpp
void FileAnalyse::ExportFile(const char* szFile, const char* szDestDir)
{
    // Cek apakah file ada di folder biasa
    if( _access(szFile, 0) == -1 )
    {
        // Tidak ada di folder, coba buka dari .pck
        AFileImage file;
        if( file.Open("", szFile,
                      AFILE_OPENEXIST|AFILE_BINARY|AFILE_TEMPMEMORY) )
        {
            DWORD dwReadLen;
            DWORD dwLen = file.GetFileLength();
            BYTE* pBuffer = (BYTE*)a_malloctemp(dwLen ? dwLen : 1);

            // Baca data dari .pck
            file.ResetPointer();
            file.Read(pBuffer, dwLen, &dwReadLen);
            file.Close();

            // Tulis ke file baru
            FILE* pFile = fopen(szNewFile, "wb");
            fwrite(pBuffer, dwLen, 1, pFile);
            fclose(pFile);

            a_freetemp(pBuffer);
        }
    }
    else
    {
        // File ada di folder, copy biasa
        CopyFileA(szFile, szNewFile, FALSE);
    }
}
```

---

## 🗜️ Sistem Kompresi dan Dekompresi

### Library yang Digunakan: **ZLIB**

Dari konfigurasi build di [ElementHintTool.dsp:55](CElementHintTool/ElementHintTool.dsp#L55):
```cpp
// Libraries yang dilink:
zliblib.lib      // Static library zlib
zlibwapi.lib     // Windows DLL version
```

### Kompresi Data

Contoh dari [EC_GameRun.cpp:2046](CElementClient/EC_GameRun.cpp#L2046):
```cpp
// Kompresi data menggunakan AFilePackage::Compress
BYTE* pSrc = ...;      // Source data
DWORD iTotalSize = ...; // Ukuran original
BYTE* pDst = ...;      // Destination buffer
DWORD dwCompLen;       // Ukuran hasil kompresi

int iRes = AFilePackage::Compress(pSrc, iTotalSize - iVerLen,
                                  pDst, &dwCompLen);
```

### Dekompresi Data

Contoh dari [EC_GameRun.cpp:2101](CElementClient/EC_GameRun.cpp#L2101):
```cpp
// Dekompresi data
BYTE* pData = ...;        // Compressed data
DWORD iDataSize = ...;    // Ukuran compressed data
char* pUncompBuf = ...;   // Output buffer
DWORD dwRealLen;          // Ukuran hasil dekompresi

int iRes = AFilePackage::Uncompress(pData, iDataSize-sizeof(DWORD),
                                    (BYTE*)pUncompBuf, &dwRealLen);
```

Contoh lain dari [EC_Scene.cpp:1186](CElementClient/EC_Scene.cpp#L1186):
```cpp
// Dekompresi bitmap data dari .pck
AFilePackage::Uncompress(pTempBuf, Chunk.dwCompSize, pBitsMap, &dwOriSize);
```

---

## 🔐 Sistem Verifikasi

### Verifikasi File .PCK: **TIDAK ADA**

File .pck **TIDAK diverifikasi** dengan signature/key. Sistem hanya mengecek:

1. **Apakah file bisa dibuka?**
   ```cpp
   if( !g_AFilePackMan.OpenFilePackageInGame(szPckFile) )
   {
       // Error: tidak bisa buka file
   }
   ```

2. **Apakah file ada di folder fallback?**
   ```cpp
   if( !g_AFilePackMan.OpenFilePackageInGame(szPckFiles[i*2])
       && _access(szPckFiles[i*2+1], 0) == -1 )
   {
       // Error: .pck tidak bisa dibuka DAN folder tidak ada
   }
   ```

### Verifikasi File Lain: **MD5 Hash**

Untuk file tertentu (seperti item data), sistem menggunakan MD5 hash untuk verifikasi:

Dari [ElementHintTool.cpp:181-189](CElementHintTool/ElementHintTool.cpp#L181-L189):
```cpp
// Hitung MD5 hash untuk verifikasi
GNET::Octets md5 = GNET::MD5Hash::Digest(decryptedItem);
data.checksum.clear();
data.checksum.reserve(md5.size()*2);

const byte *p = NULL;
for (p = (const byte *)md5.begin(); p != (const byte *)md5.end(); p++)
{
    sprintf(buf, "%02x", *p);
    data.checksum.insert(data.checksum.end(), buf, 2);
}
```

---

## 🔍 Struktur Internal File .PCK

### Informasi Terbatas

Struktur internal file .pck **TIDAK di-documented** dalam kode yang tersedia. Yang kita tahu:

1. **Dibaca oleh library Angelica:**
   - `AFilePackage` class (dari AFilePackage.h)
   - `AFilePackMan` manager (dari AFilePackMan.h)
   - `AFileImage` untuk membaca file

2. **Menggunakan kompresi ZLIB:**
   - Setiap file di dalam .pck dikompres dengan zlib
   - Bisa didekompres dengan `AFilePackage::Uncompress()`

3. **Index/File Table:**
   - File .pck memiliki internal index/file table
   - Memetakan filename → offset di archive → compressed size → uncompressed size

### Reverse Engineering (Jika Perlu)

Jika ingin membaca struktur .pck tanpa library Angelica, Anda perlu:

1. **Analisis header file .pck**
   - Buka file .pck dengan hex editor
   - Cari magic number/signature
   - Identifikasi offset ke file table

2. **Parse file table**
   - Baca list filename dan offset
   - Decompress data dengan zlib

3. **Atau gunakan library Angelica**
   - Lebih mudah: gunakan `AFilePackage` dan `AFilePackMan`
   - Library ini tersedia di game SDK

---

## 📝 Contoh Penggunaan Lengkap

### Contoh 1: Membuka Semua .PCK Files

```cpp
#include <AFilePackMan.h>
#include <AF.h>

// Inisialisasi Angelica Framework
af_Initialize();

// Set base directory
char szWorkDir[MAX_PATH];
GetCurrentDirectoryA(MAX_PATH, szWorkDir);
af_SetBaseDir(szWorkDir);

// Buka semua file .pck (TANPA KEY/SIGNATURE!)
const char* pckFiles[] = {
    "configs.pck",
    "facedata.pck",
    "models.pck",
    "gfx.pck",
    "interfaces.pck",
    "surfaces.pck",
    "grasses.pck",
    "sfx.pck",
    "shaders.pck",
    "textures.pck",
};

for(int i = 0; i < 10; i++)
{
    g_AFilePackMan.OpenFilePackageInGame(pckFiles[i]);
}

// Sekarang semua file bisa dibaca seperti file biasa!
AFileImage file;
file.Open("", "configs\\script\\login.lua",
          AFILE_OPENEXIST | AFILE_BINARY | AFILE_TEMPMEMORY);
// ...
```

### Contoh 2: Extract File dari .PCK

```cpp
#include <AFilePackMan.h>
#include <AFileImage.h>

void ExtractFromPck(const char* szFileInPck, const char* szDestPath)
{
    AFileImage file;

    // Buka file dari .pck (otomatis!)
    if( file.Open("", szFileInPck,
                  AFILE_OPENEXIST | AFILE_BINARY | AFILE_TEMPMEMORY) )
    {
        DWORD dwLen = file.GetFileLength();
        BYTE* pBuffer = new BYTE[dwLen];

        // Baca data
        DWORD dwReadLen;
        file.Read(pBuffer, dwLen, &dwReadLen);
        file.Close();

        // Tulis ke file
        FILE* pOut = fopen(szDestPath, "wb");
        if(pOut)
        {
            fwrite(pBuffer, 1, dwReadLen, pOut);
            fclose(pOut);
        }

        delete[] pBuffer;
    }
}

// Penggunaan:
ExtractFromPck("models\\character\\player.ecm", "player.ecm");
```

### Contoh 3: Cek File Existence

```cpp
#include <AFilePackMan.h>

bool FileExists(const char* szFile)
{
    // AFilePackMan otomatis mencari:
    // 1. Di file .pck
    // 2. Di folder fisik

    AFileImage file;
    return file.Open("", szFile,
                     AFILE_OPENEXIST | AFILE_BINARY | AFILE_TEMPMEMORY);
}

// Penggunaan:
if(FileExists("configs\\game.conf"))
{
    // File ada (di .pck atau folder)
}
```

---

## 🔑 Kesimpulan

### Untuk Membaca File .PCK, Anda Butuh:

1. ✅ **Angelica Framework Library** (`AFilePackage.lib`, `AFilePackMan.lib`)
2. ✅ **ZLIB Library** (`zliblib.lib`, `zlibwapi.lib`)
3. ✅ **Inisialisasi Framework:**
   ```cpp
   af_Initialize();
   af_SetBaseDir(szWorkDir);
   g_AFilePackMan.OpenFilePackageInGame("namafile.pck");
   ```

### TIDAK Butuh:

- ❌ Encryption key
- ❌ Signature verification
- ❌ Password
- ❌ Authentication token
- ❌ License check

---

## 📚 Referensi dalam Kode

- [FileAnalyse.cpp:186-219](ClientTools/FileExtract/FileAnalyse.cpp#L186-L219) - Inisialisasi .pck files
- [FileAnalyse.cpp:1577-1621](ClientTools/FileExtract/FileAnalyse.cpp#L1577-L1621) - Extract file dari .pck
- [ElementHintTool.cpp:487](CElementHintTool/ElementHintTool.cpp#L487) - Buka configs.pck
- [ElementClient.cpp:1213](CElementClient/ElementClient.cpp#L1213) - Load .pck files
- [EC_GameRun.cpp:2046](CElementClient/EC_GameRun.cpp#L2046) - Kompresi data
- [EC_GameRun.cpp:2101](CElementClient/EC_GameRun.cpp#L2101) - Dekompresi data
- [EC_Scene.cpp:1186](CElementClient/EC_Scene.cpp#L1186) - Uncompress bitmap

---

**Last Updated:** 2026-01-01
**Status:** ✅ TIDAK ADA ENKRIPSI/SIGNATURE
**Library:** Angelica File Package + ZLIB
