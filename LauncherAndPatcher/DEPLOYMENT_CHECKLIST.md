# Deployment Checklist - Patcher with Key Verification

## ✅ STATUS: READY TO BUILD

---

## 📋 PRE-BUILD CHECKLIST

### ✅ Source Code - COMPLETE
- [x] **CCommon/PckKeyVerifier.h** - Header file with API declarations
- [x] **CCommon/PckKeyVerifier.cpp** - Implementation (380 lines)
- [x] **ClientTools/PWDownloader/EC_Archive7Z.cpp** - Modified with key verification
- [x] **ClientTools/PWDownloader/PWDownloader.dsp** - Build configuration updated

### ✅ Build Configuration - COMPLETE
- [x] Include path added: `/I "../../CCommon"`
- [x] Source file added: `SOURCE=..\..\CCommon\PckKeyVerifier.cpp`
- [x] Both Release and Debug configurations updated
- [x] Precompiled headers disabled for PckKeyVerifier.cpp (`#SUBTRACT CPP /YX /Yc /Yu`)

### ✅ Integration Points - COMPLETE
- [x] EC_Archive7Z.cpp includes PckKeyVerifier.h
- [x] Key verification called in GetStream() method
- [x] Error handling on verification failure
- [x] MessageBox notification on key mismatch

---

## 🔧 BUILD INSTRUCTIONS

### Step 1: Open Workspace
```
File: LauncherAndPatcher/ClientTools/PWDownloader/PWDownloader.dsw
Platform: Visual Studio 6.0 or Visual Studio .NET 2003
```

### Step 2: Select Configuration
```
- Win32 Release (for production)
- Win32 Debug (for development)
```

### Step 3: Build
```
1. Build → Clean Solution
2. Build → Rebuild All
3. Check for errors in Output window
```

### Step 4: Expected Output
```
Release Build:
- Output: ClientTools/PWDownloader/Release/PWDownloader.exe
- Size: ~500-800 KB

Debug Build:
- Output: ClientTools/PWDownloader/Debug/PWDownloader_d.exe
- Size: ~1-2 MB (with debug symbols)
```

---

## 📦 FILES TO DEPLOY

### Required Files:
```
LauncherAndPatcher/
├── ClientTools/PWDownloader/
│   ├── Release/
│   │   └── PWDownloader.exe          ← Deploy this
│   └── Debug/
│       └── PWDownloader_d.exe         ← Or this for debugging
│
├── CCommon/
│   ├── PckKeyVerifier.h               ← Documentation/Reference
│   └── PckKeyVerifier.cpp
│
├── Documentation:
│   ├── PATCHER_KEY_INTEGRATION.md     ← How integration works
│   ├── KEY_FORMAT_GUIDE.md            ← Key format documentation
│   └── DEPLOYMENT_CHECKLIST.md        ← This file
│
└── README.md                          ← Overall project info
```

---

## 🚨 POTENTIAL BUILD ERRORS & SOLUTIONS

### Error 1: Cannot find PckKeyVerifier.h
```
Error: fatal error C1083: Cannot open include file: '..\..\CCommon\PckKeyVerifier.h'
Solution:
  1. Check file exists: LauncherAndPatcher/CCommon/PckKeyVerifier.h
  2. Check include path in .dsp: /I "../../CCommon"
  3. Verify relative path is correct (from PWDownloader/ to CCommon/)
```

### Error 2: Unresolved external symbol
```
Error: LNK2001: unresolved external symbol _PCK_SetKey
Solution:
  1. Check PckKeyVerifier.cpp is added to .dsp
  2. Check file path: SOURCE=..\..\CCommon\PckKeyVerifier.cpp
  3. Verify precompiled headers disabled: #SUBTRACT CPP /YX /Yc /Yu
```

### Error 3: Windows.h not found
```
Error: fatal error C1083: Cannot open include file: 'windows.h'
Solution:
  1. Verify Windows SDK is installed
  2. Check SDK path in Visual Studio: Tools → Options → Directories
  3. For VS6.0: Should point to: Platform SDK/Include
```

### Error 4: Missing runtime dependencies
```
Error: PWDownloader.exe won't run (missing DLLs)
Required DLLs:
  - MFC42.DLL (or MFC70.DLL for VS.NET)
  - MSVCRT.DLL (or MSVCR70.DLL)
  - gdiplus.dll
  - p2sp_4th_lib.dll

Solution:
  1. Copy required DLLs to output directory
  2. Or install Visual C++ Redistributable
```

---

## 🧪 TESTING CHECKLIST

### Test 1: Basic Build
```bash
1. Open PWDownloader.dsw
2. Select "Win32 Release"
3. Build → Rebuild All
4. Verify: 0 errors, 0 warnings
```

### Test 2: File Existence
```bash
1. Check output: Release/PWDownloader.exe exists
2. Check file size: Should be > 100 KB
3. Run: Double-click PWDownloader.exe
4. Verify: No crash on startup
```

### Test 3: Key Verification (Manual)
```cpp
// Add test code to PWDownloaderDlg::OnInitDialog():
PCK_SetKey("TEST123");
BOOL bResult = PCK_VerifyKey("configs.pck");
// Expected: TRUE if configs.pck exists and is not protected
```

### Test 4: Base64 Key Test
```cpp
// Test base64 decoding
PCK_SetKey("UkFIQVNJQTEyMw==");  // Base64 for "RAHASIA123"
const char* szKey = PCK_GetKey();
// Expected: "RAHASIA123"
```

---

## 📋 RUNTIME DEPENDENCIES

### Required Libraries (Already linked):
```
✅ gdiplus.lib        - Graphics (already in .dsp)
✅ p2sp_4th_lib.lib   - P2SP download library
✅ 7z.lib             - 7-Zip compression
```

### Required DLLs at Runtime:
```
1. gdiplus.dll        - Windows XP+ (usually in system32)
2. p2sp_4th_lib.dll   - P2SP engine (in CNewSkin/lib)
3. mfc*.dll           - MFC runtime (or static linked)
```

---

## 🎯 POST-BUILD VERIFICATION

### Verification Steps:

1. **Check Binary Size**
   ```
   Release/PWDownloader.exe
   - Min: 400 KB
   - Max: 2 MB
   ```

2. **Check Dependencies**
   ```
   Use Dependency Walker (depends.exe) or:
   - objdump -p PWDownloader.exe | grep DLL
   ```

3. **Check Exports**
   ```
   Should contain symbols:
   - PCK_SetKey
   - PCK_GetKey
   - PCK_VerifyKey
   - PCK_HasKeyProtection
   - PCK_IsBase64
   - PCK_DecodeBase64
   ```

4. **Resource Check**
   ```
   Check embedded resources (if any):
   - IDR_MAINFRAME
   - IDR_INSTALLBG
   - Dialog templates
   ```

---

## 🔧 CONFIGURATION AFTER BUILD

### Configuration File Location (Optional):
```
Create: config/patcher.ini

[PCKProtection]
; Set verification key (plain text or base64)
Key=RAHASIA123

; Or use base64:
; Key=UkFIQVNJQTEyMw==
```

### Hardcoded Key Alternative:
```cpp
// In PWDownloaderDlg::OnInitDialog():
PCK_SetKey("RAHASIA123");  // Set once at startup
```

---

## 📝 DEPLOYMENT NOTES

### What Changed from Original:
1. ✅ Added PckKeyVerifier module (380 lines)
2. ✅ Modified EC_Archive7Z.cpp (added 30 lines for verification)
3. ✅ Updated PWDownloader.dsp (added source file and include path)
4. ✅ NO changes to download logic
5. ✅ NO changes to P2SP engine
6. ✅ NO changes to 7-Zip extraction (except verification before overwrite)

### Backward Compatibility:
- ✅ If no key set: PCK files open normally
- ✅ If PCK file not protected: Opens normally even with key set
- ✅ Only protected PCK files require key verification
- ✅ Non-.pck files: No changes at all

### Performance Impact:
- ✅ Negligible: Only 8 bytes read for magic check
- ✅ Hash calculation: Only if file is protected
- ✅ No impact on non-.pck files

---

## ✅ FINAL CHECKLIST BEFORE DEPLOY

- [ ] Build completed successfully (0 errors)
- [ ] PWDownloader.exe exists in Release/ folder
- [ ] File size is reasonable (> 400 KB)
- [ ] Dependencies (DLLs) are available
- [ ] Documentation files are included
- [ ] Source code is backed up
- [ ] Key format is documented for users
- [ ] Test plan is prepared

---

## 🎉 READY TO DEPLOY!

### Next Steps:

1. **Build the executable**
   ```bash
   Open: PWDownloader.dsw
   Build: Rebuild All (Win32 Release)
   Output: Release/PWDownloader.exe
   ```

2. **Package for deployment**
   ```
   Create folder: Patcher-Deploy/
   Copy:
     - Release/PWDownloader.exe
     - Required DLLs
     - Documentation (KEY_FORMAT_GUIDE.md)
   ```

3. **Test in clean environment**
   ```
   - Copy to test machine
   - Run without Visual Studio installed
   - Verify no missing DLL errors
   ```

4. **Distribute to users**
   ```
   - Include installation instructions
   - Document key format
   - Provide troubleshooting guide
   ```

---

**Build Date:** 2026-01-01
**Status:** ✅ READY FOR COMPILATION
**Platform:** Windows (Visual Studio 6.0/2003)
**Compiler:** CL.EXE (Microsoft C/C++)
**Target:** Win32 x86
