# 🔑 Cara Set Key untuk Test Deploy

## ✅ SUDAH SELESAI DICONFIGURASI!

Key sudah ditambahkan di `PWDownloaderDlg.cpp` baris 152:

```cpp
PCK_SetKey("RAHASIA123");
```

---

## 🎯 CARA GANTI KEY

### Option 1: Edit Langsung di Code

**File:** `ClientTools/PWDownloader/PWDownloaderDlg.cpp`
**Line:** 152

```cpp
// Ganti "RAHASIA123" dengan key Anda:
PCK_SetKey("KEY_ANDA_DISINI");
```

### Option 2: Gunakan Base64

```cpp
// Ganti dengan key dalam format base64:
PCK_SetKey("UkHIQVNJQTEyMw==");  // Base64 version
```

---

## 🔧 CARA BUILD & TEST

### Step 1: Build Executable

```bash
1. Buka: LauncherAndPatcher/ClientTools/PWDownloader/PWDownloader.dsw
2. Pilih: Win32 Release
3. Build → Rebuild All
4. Output: Release/PWDownloader.exe
```

### Step 2: Test

```bash
1. Copy PWDownloader.exe ke folder test
2. Copy required DLLs:
   - gdiplus.dll
   - p2sp_4th_lib.dll
3. Jalankan PWDownloader.exe
4. Cek: Key otomatis ter-load saat startup
```

---

## 📋 LOKASI KEY YANG SUDAH DICONFIG

**File:** `PWDownloaderDlg.cpp`
```cpp
Line 11:  #include "..\..\CCommon\PckKeyVerifier.h"
Line 152: PCK_SetKey("RAHASIA123");
```

**Fungsi:**
- Key di-set otomatis saat patcher berjalan
- Terjadi di `OnInitDialog()` saat aplikasi mulai
- Berlaku untuk SELURUH proses update/extract

---

## 🧪 TESTING SCENARIO

### Test 1: Update Tanpa Key Verification

```cpp
// Comment out baris 152:
// PCK_SetKey("RAHASIA123");

// Result: Semua file .pck bisa di-overwrite (tanpa verifikasi)
```

### Test 2: Update Dengan Key Verification

```cpp
// Uncomment baris 152:
PCK_SetKey("RAHASIA123");

// Result:
// - File .pck tanpa proteksi → Bisa di-overwrite
// - File .pck dengan proteksi + Key benar → Bisa di-overwrite
// - File .pck dengan proteksi + Key salah → ERROR, tidak bisa di-overwrite
```

### Test 3: Base64 Key

```cpp
// Ganti baris 152:
PCK_SetKey("UkFIQVNJQTEyMw==");

// Result: Sama seperti Test 2 (auto-decode)
```

---

## ⚠️ PENTING

### Untuk PRODUKSI (Real Deployment):

❌ **JANGAN** hardcode key di code
✅ **GUNAKAN** salah satu cara ini:

1. **Config File**
   ```ini
   [PCKProtection]
   Key=RAHASIA123
   ```
   Lalu baca dari file:
   ```cpp
   char szKey[256];
   GetPrivateProfileString("PCKProtection", "Key", "", szKey, 256, "config.ini");
   PCK_SetKey(szKey);
   ```

2. **Registry**
   ```cpp
   char szKey[256];
   DWORD dwSize = 256;
   RegGetValue(HKEY_CURRENT_USER, "Software\\MyGame", "PCKKey",
               RRF_RT_REG_SZ, NULL, szKey, &dwSize);
   PCK_SetKey(szKey);
   ```

3. **Command Line**
   ```cpp
   // Di InitInstance():
   char szKey[256];
   strcpy(szKey, __argv[1]);  // Dari command line
   PCK_SetKey(szKey);
   ```

---

## 📝 SUMMARY

### ✅ SEKARANG:

- Key sudah di-set di `PWDownloaderDlg.cpp` line 152
- Default key: `"RAHASIA123"`
- Lokasi: `OnInitDialog()` → Saat patcher mulai

### 🔧 UNTUK GANTI KEY:

1. Buka `PWDownloaderDlg.cpp`
2. Cari baris 152
3. Ganti `"RAHASIA123"` dengan key Anda
4. Rebuild

### 🎯 UNTUK TEST:

1. Build executable
2. Jalankan
3. Coba download update
4. Sistem akan otomatis verifikasi key saat extract .pck

---

**Current Key:** `RAHASIA123`
**Location:** `PWDownloaderDlg.cpp:152`
**Status:** ✅ READY TO TEST

---

## 🚀 NEXT STEPS

1. ✅ Key sudah diset
2. ⏳ Build executable
3. ⏳ Test dengan file .pck
4. ⏳ Deploy ke user

---

**Created:** 2026-01-01
**For:** Testing purposes only
**Production:** Use config file or other secure method
