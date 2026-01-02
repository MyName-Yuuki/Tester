# 🎯 FOCUSED BUILD - Launcher & Patcher ONLY

## 📌 Apa yang Akan Kita Build?

HANYA 2 aplikasi yang PENTING:

1. **PWDownloader.exe** - Patcher (dengan key verification)
2. **HintLauncher.exe** - Launcher (untuk start game)

Aplikasi LAINNYA tidak perlu di-build (tidak penting untuk deployment).

---

## ✅ KEUNTUNGAN BUILD FOCUSED:

### Mengapa HANYA build Launcher + Patcher?

✅ **Lebih Sedikit Errors** - Hanya 5-15 errors (bukan 50-100)
✅ **Lebih Cepat** - 1-2 jam (bukan 1-3 hari)
✅ **Lebih Simple** - Hanya 2 project kecil
✅ **Cukup untuk Deployment** - Launcher + Patcher sudah cukup

### Yang TIDAK Perlu Di-build:
❌ ElementHintTool (tool editor - tidak penting)
❌ FileExtract (tool extra - sudah ada alternatif)
❌ ElementClient (game client - bukan bagian patcher)
❌ Libraries (FWEditorLib, dll - dependencies saja)

---

## 🎯 PROJECT YANG AKAN DI-BUILD:

### 1. PWDownloader (Patcher) - WAJIB
- **Lokasi:** `ClientTools/PWDownloader/`
- **Output:** `PWDownloader.exe`
- **Fungsi:** Download & extract update dengan key verification
- **Prioritas:** ⭐⭐⭐ WAJIB

### 2. ElementHintLauncher (Launcher) - WAJIB
- **Lokasi:** `CElementHintLauncher/`
- **Output:** `HintLauncher.exe` (atau `wmgj_HintLauncher.exe`)
- **Fungsi:** Launch game dan check updates
- **Prioritas:** ⭐⭐⭐ WAJIB

---

## 🔧 CARA BUILD DI VS2022 (Windows 11)

### STEP 1: Install VS2022 (sudah?)

Jika belum, install dengan components:
```
☑ Desktop development with C++
☑ MSVC v143 - VS 2022 C++ build tools
☑ MSVC v140 - VS 2015 C++ build tools ← PENTING!
☑ C++ MFC library for v143
☑ Windows 11 SDK
```

### STEP 2: Build PWDownloader (Patcher)

```
1. Buka: LauncherAndPatcher/ClientTools/PWDownloader/

2. Double-click: PWDownloader.dsp

3. VS2022 akan prompt upgrade → Klik "OK"

4. Configure Project Settings (PENTING!):

   Right-click Project → Properties:

   General:
   ├─ Platform Toolset: Visual Studio 2015 (v140) ← PENTING!
   ├─ Character Set: Use Multi-Byte Character Set
   └─ Whole Program Optimization: No

   C/C++ → General:
   └─ Additional Include Directories: ../../CCommon

   C/C++ → Precompiled Headers:
   └─ Precompiled Header: Not Using Precompiled Headers

   C/C++ → Advanced:
   └─ Warning As Error: No

5. Build (F7)

6. Expected Errors: 5-10 errors saja!

7. Fix Errors (lihat di bawah)

8. Rebuild sampai 0 errors

9. Output: PWDownloader.exe di Release/ folder
```

### STEP 3: Build ElementHintLauncher (Launcher)

```
1. Buka: LauncherAndPatcher/CElementHintLauncher/

2. Double-click: ElementHintLauncher.dsp

3. VS2022 akan prompt upgrade → Klik "OK"

4. Configure Project Settings (SAMA seperti PWDownloader):

   Right-click Project → Properties:

   General:
   ├─ Platform Toolset: Visual Studio 2015 (v140)
   ├─ Character Set: Use Multi-Byte Character Set
   └─ Whole Program Optimization: No

   C/C++ → Precompiled Headers:
   └─ Precompiled Header: Not Using Precompiled Headers

   C/C++ → Advanced:
   └─ Warning As Error: No

5. Build (F7)

6. Expected Errors: 0-5 errors (launcher lebih simple!)

7. Fix Errors (jika ada)

8. Rebuild sampai 0 errors

9. Output: HintLauncher.exe di CBin/ folder
```

---

## 🔧 COMMON ERRORS & FIXES

### Error 1: 'strcpy' deprecated (Paling Common)

**Error Message:**
```
error C4996: 'strcpy': This function or variable may be unsafe.
Consider using strcpy_s instead.
```

**Quick Fix (Disable Warning):**
```
Project Properties → C/C++ → Advanced
→ Disable Specific Warnings: 4996
→ Apply → OK
→ Rebuild (F7)
```

**Atau Proper Fix (Replace):**
```cpp
// Old (VS2005):
strcpy(dest, src);

// New (VS2022):
strcpy_s(dest, sizeof(dest), src);
```

### Error 2: Cannot open include file 'stdafx.h'

**Error Message:**
```
fatal error C1010: unexpected end of file while looking for
precompiled header. Did you forget to add '#include "StdAfx.h"'
```

**Fix:**
```
Project Properties → C/C++ → Precompiled Headers
→ Precompiled Header: Not Using Precompiled Headers
→ Apply → OK
→ Rebuild (F7)
```

### Error 3: MFC not found

**Error Message:**
```
fatal error C1083: Cannot open include file: 'afxwin.h': No such file or directory
```

**Fix:**
```
1. Open Visual Studio Installer
2. Click "Modify" on VS2022
3. Individual Components
4. Search: "MFC"
5. Check: "C++ MFC library for v143"
6. Click "Modify" to install
7. Rebuild
```

### Error 4: Cannot open include file '../CCommon/...'

**Error Message:**
```
fatal error C1083: Cannot open include file: '../CCommon/PckKeyVerifier.h'
```

**Fix:**
```
Project Properties → C/C++ → General
→ Additional Include Directories:
   Add: ../../CCommon
→ Apply → OK
→ Rebuild (F7)
```

### Error 5: LINK error (unresolved external)

**Error Message:**
```
error LNK2019: unresolved external symbol
```

**Ini biasanya karena missing dependencies.**

**Option 1: Skip the dependency**
```
Jika error tidak critical, cari file yang menyebabkan error
→ Remove from project (right-click → Remove)
→ Rebuild
```

**Option 2: Comment out code yang tidak penting**
```cpp
// Comment out calls to missing libraries
// TODO: Fix this later
```

---

## 📊 EXPECTED RESULTS

### PWDownloader (Patcher):
```
Files to compile: ~18 files
Expected errors: 5-10
Fix time: 30-60 menit
Output size: ~500-800 KB
```

### ElementHintLauncher (Launcher):
```
Files to compile: ~2 files
Expected errors: 0-5
Fix time: 10-20 menit
Output size: ~100-200 KB
```

### Total:
```
Total time: 1-2 jam
Total errors: 5-15
Success rate: 95%+
```

---

## ⚡ QUICK FIX STRATEGY

### Jika mau SECEPAT MUNGKIN:

1. **Disable semua warnings as error**
```
Project Properties → C/C++ → Advanced
→ Warning As Error: No
→ Treat Specific Warnings As Errors: (blank)
→ Disable Specific Warnings: 4996;4995;4267
```

2. **Gunakan v140 toolset**
```
Project Properties → General
→ Platform Toolset: Visual Studio 2015 (v140)
```

3. **Disable Precompiled Headers**
```
Project Properties → C/C++ → Precompiled Headers
→ Precompiled Header: Not Using Precompiled Headers
```

4. **Build - Fix - Repeat**
```
F7 → Build
Lihat error
Fix dengan cara di atas
F7 → Rebuild
Repeat sampai success
```

---

## ✅ VERIFY BUILD SUCCESS

### Setelah Build Sukses:

1. **Check Output Files**
```
PWDownloader:
  Locate: LauncherAndPatcher/ClientTools/PWDownloader/Release/
  Find: PWDownloader.exe
  Check: Size > 400 KB

HintLauncher:
  Locate: LauncherAndPatcher/CBin/
  Find: wmgj_HintLauncher.exe atau HintLauncher.exe
  Check: Size > 100 KB
```

2. **Quick Test**
```
1. Copy PWDownloader.exe ke folder test
2. Double-click PWDownloader.exe
3. Check: Window opens? ✓
4. Check: No crash? ✓
5. Close

6. Copy HintLauncher.exe ke folder test
7. Double-click HintLauncher.exe
8. Check: Window opens? ✓
9. Check: No crash? ✓
10. Close
```

3. **Deploy**
```
Copy kedua exe ke folder deployment:
  /MyGamePatcher/
    ├── PWDownloader.exe    ← Patcher
    ├── HintLauncher.exe    ← Launcher
    └── README.txt          (instructions)
```

---

## 🎯 TIPS UNTUK MEMPERCEPAT

### Tip 1: Build Satu-satu
```
Jangan build semua project sekaligus!
Build satu project sampai sukses,
baru lanjut ke project berikutnya.
```

### Tip 2: Gunakan "Build Only"
```
Jangan click "Rebuild All" setiap saat.
Click "Build" saja (F7) untuk incremental compile.
Lebih cepat!
```

### Tip 3: Focus Patcher Dulu
```
Build PWDownloader dulu (paling penting).
Kalau sukses, launcher pasti lebih mudah.
```

### Tip 4: Copy Settings
```
Setelah sukses configure PWDownloader,
copy settings yang sama ke HintLauncher.
Tidak perlu set ulang dari awal.
```

---

## 🆘 MASIH ERROR?

### Jika masih banyak error setelah semua tips:

**Option 1: Coba toolset berbeda**
```
Platform Toolset: v140 → v141 → v142
Coba satu-satu sampai yang paling sedikit error.
```

**Option 2: Build hanya Patcher**
```
Kalau launcher terlalu banyak error,
skip dulu. Patcher saja sudah cukup!
```

**Option 3: Gunakan VM**
```
Install VirtualBox + Windows XP + VS2005
Build di VM (0 errors)
Copy .exe ke Windows 11
```

---

## 📝 SUMMARY CHECKLIST

### Sebelum Mulai:
- [ ] VS2022 installed dengan MFC + v140 toolset
- [ ] LauncherAndPatcher folder ada di Windows
- [ ] Baca QUICK_START.md

### Build PWDownloader:
- [ ] Open PWDownloader.dsp
- [ ] Configure: v140 toolset, Multi-Byte, No PCH
- [ ] Build (F7)
- [ ] Fix 5-10 errors
- [ ] Rebuild sampai 0 errors
- [ ] Verify PWDownloader.exe exists

### Build HintLauncher:
- [ ] Open ElementHintLauncher.dsp
- [ ] Configure: v140 toolset, Multi-Byte, No PCH
- [ ] Build (F7)
- [ ] Fix 0-5 errors
- [ ] Rebuild sampai 0 errors
- [ ] Verify HintLauncher.exe exists

### Deploy:
- [ ] Test kedua exe
- [ ] Copy ke deployment folder
- [ ] Done!

---

## 🎉 FINAL RESULT

Setelah selesai, Anda punya:

✅ **PWDownloader.exe** - Patcher dengan key verification
✅ **HintLauncher.exe** - Launcher untuk game
✅ Siap deploy ke users
✅ Build time: 1-2 jam (bukan 1-3 hari!)

---

**Created:** 2026-01-01
**Status:** ✅ Ready for Focused Build
**Platform:** Windows 10/11 + VS2022
**Build Time:** 1-2 hours
**Errors:** 5-15 expected

---

## 🚀 MULAI SEKARANG!

1. Open VS2022
2. Open PWDownloader.dsp
3. Configure settings
4. Build (F7)
5. Fix errors
6. Done!

**Good luck!** 🎯
