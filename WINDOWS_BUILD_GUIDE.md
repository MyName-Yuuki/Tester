# 🚀 WINDOWS BUILD & DEPLOYMENT GUIDE

## ✅ SIAP UNTUK WINDOWS

Semua file sudah siap! Sekarang Anda tinggal:

---

## 📋 STEP-BY-STEP (WINDOWS)

### Step 1: Pindahkan Folder ke Windows

Copy folder ini ke Windows:
```
D:\Dev\LauncherAndPatcher\
```

### Step 2: Install Visual Studio

Diperlukan:
- ✅ Visual Studio 6.0
- ✅ ATAU Visual Studio .NET 2003
- ✅ Windows SDK

### Step 3: Build

**Cara 1: Menggunakan Script (RECOMMENDED)**

1. Buka Command Prompt
2. CD ke folder project:
   ```
   cd D:\Dev\LauncherAndPatcher
   ```
3. Jalankan build script:
   ```
   BUILD.bat
   ```
4. Tunggu proses build selesai

**Cara 2: Manual**

1. Buka file: ClientTools\PWDownloader\PWDownloader.dsw
2. Pilih configuration: Win32 Release
3. Tekan: F7 (Build)
4. Atau: Build → Rebuild All

### Step 4: Cek Output

Output akan di:
```
ClientTools\PWDownloader\Release\PWDownloader.exe
```

---

## 🎯 FILE YANG SUDAH DIEDIT

### 1. Source Code:

- CCommon/PckKeyVerifier.h (100 lines)
- CCommon/PckKeyVerifier.cpp (380 lines)
- ClientTools/PWDownloader/EC_Archive7Z.cpp (Modified)
- ClientTools/PWDownloader/PWDownloaderDlg.cpp (Modified with key!)
- ClientTools/PWDownloader/PWDownloader.dsp (Updated)

---

## 📝 KEY YANG SUDAH DISET:

**Location:** PWDownloaderDlg.cpp line 152
```cpp
PCK_SetKey("RAHASIA123");
```

**To change key:**
1. Buka PWDownloaderDlg.cpp
2. Cari line 152
3. Ganti "RAHASIA123" dengan key Anda
4. Rebuild

---

🎉 **TINGGAL COPY KE WINDOWS DAN BUILD!** 🎉
