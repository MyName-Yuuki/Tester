# PCK Verification Injector - BUILD GUIDE

## 🎯 Apa Ini?

Project **standalone** untuk inject PCK key verification ke PWDownloader.exe yang **sudah ada** tanpa perlu recompile!

**Tidak butuh VS2005!** Bisa build dengan VS2019/VS2022.

---

## 📦 Project Structure

```
PCKInjector/
├── PCKVerifierDLL/           # DLL untuk verification logic
│   ├── PCKVerifier.cpp        # Pure C, no MFC needed!
│   ├── PCKVerifier.h          # Public API
│   ├── PCKVerifier.def        # DLL exports
│   └── PCKVerifier.vcxproj    # VS2019 project
│
├── LauncherWrapper/           # Injector & launcher
│   ├── LauncherWrapper.cpp    # Injection logic
│   └── LauncherWrapper.vcxproj
│
├── PCKInjector.sln           # VS2019 solution
├── README.md                 # This file
└── BUILD_GUIDE.md            # This file
```

---

## 🚀 Cara Build (VS2019/VS2022)

### Di Windows:

1. **Buka Visual Studio 2019/2022**
   ```
   Double click: PCKInjector.sln
   ```

2. **Pilih Configuration**
   ```
   - Configuration: Release
   - Platform: Win32
   ```

3. **Build Solution**
   ```
   - Menu: Build → Build Solution (F7)
   - Atau: Right-click Solution → Build
   ```

4. **Hasil Build**
   ```
   Bin/
   ├── PCKVerifier.dll         # Verification DLL
   ├── LauncherWrapper.exe     # Injector launcher
   └── launcher_log.txt        # Runtime log (akan dibuat saat run)
   ```

### Di GitHub Actions (Automated)

Coming soon! Add `.github/workflows/build-injector.yml`

---

## 📋 Cara Pakai

### Setup:

1. **Copy files ke folder PWDownloader.exe:**
   ```
   Copy semua file dari Bin/ ke folder PWDownloader.exe:

   PWDownloader/
   ├── PWDownloader.exe        # Original exe (JANGAN DIMODIFIKASI!)
   ├── PCKVerifier.dll         # Tambah ini
   └── LauncherWrapper.exe     # Tambah ini
   ```

2. **Run LauncherWrapper.exe:**
   ```bash
   # Jalankan launcher
   LauncherWrapper.exe

   # Launcher akan:
   # 1. Load PCKVerifier.dll
   # 2. Set key: "RAHASIA123"
   # 3. Launch PWDownloader.exe
   # 4. Inject verification ke PWDownloader
   ```

3. **Check Log:**
   ```
   Buka: launcher_log.txt
   Lihat: Key verification status
   ```

### Ganti Key:

Edit file `LauncherWrapper.cpp` line 20:
```cpp
const char* DEFAULT_KEY = "RAHASIA123";  // Ganti key di sini
```

Rebuild LauncherWrapper project.

---

## 🔧 Cara Kerja

### Architecture:

```
┌─────────────────────────────────────────┐
│  LauncherWrapper.exe                    │
│  ┌───────────────────────────────────┐  │
│  │ 1. Load PCKVerifier.dll           │  │
│  │ 2. Call PCK_SetKey("RAHASIA123")   │  │
│  │ 3. CreateProcess(PWDownloader.exe) │  │
│  │ 4. Inject DLL ke PWDownloader      │  │
│  │ 5. Resume main thread              │  │
│  └───────────────────────────────────┘  │
└─────────────────────────────────────────┘
              │
              ▼
┌─────────────────────────────────────────┐
│  PWDownloader.exe (Original)            │
│  ┌───────────────────────────────────┐  │
│  │ PCKVerifier.dll (INJECTED)        │  │
│  │ - Hooks file operations           │  │
│  │ - Intercepts .pck file access     │  │
│  │ - Calls PCK_VerifyKey()            │  │
│  │ - Block jika key tidak match      │  │
│  └───────────────────────────────────┘  │
└─────────────────────────────────────────┘
```

### Verification Flow:

1. PWDownloader tries to extract `.pck` file
2. PCKVerifier.dll intercepts file open
3. Check if file has magic number "PCKP"
4. If yes, extract hash from file header
5. Compute hash from current key
6. Compare hashes
7. Allow/Deny extraction

---

## ✅ Keuntungan Approach Ini

✅ **Tidak perlu VS2005** - Build dengan VS2019/VS2022!
✅ **Tidak perlu recompile PWDownloader** - Gunakan exe yang sudah ada
✅ **Non-destructive** - Original exe tidak dimodifikasi
✅ **Flexible** - Bisa ganti key tanpa recompile
✅ **Modern code** - Pure C, standard C++, no legacy dependencies
✅ **Testable** - Bisa test verification independently
✅ **Portable** - DLL dan wrapper standalone

---

## ⚠️ Keterbatasan

⚠️ **DLL Injection required** - Butuh run LauncherWrapper.exe, bukan langsung PWDownloader.exe
⚠️ **Antivirus might flag** - DLL injection terkadang terdeteksi sebagai suspicious
⚠️ **No compile-time verification** - Verification hanya runtime, bukan compile-time

---

## 🐛 Troubleshooting

### Build Errors:

**Error: "Cannot open include file: windows.h"**
- Install Windows SDK via Visual Studio Installer
- Components: "Windows 10 SDK" atau "Windows 11 SDK"

**Error: LNK2019: unresolved external symbol**
- Check PCKVerifier.def file exists
- Verify ModuleDefinitionFile setting in .vcxproj

### Runtime Errors:

**"PCKVerifier.dll not found!"**
- Pastikan PCKVerifier.dll di folder yang sama dengan LauncherWrapper.exe
- Check working directory saat run

**"PWDownloader.exe not found!"**
- Copy PWDownloader.exe ke folder Bin/ atau LauncherWrapper.exe ke folder PWDownloader

**"DLL injection failed"**
- Run as Administrator
- Disable antivirus sementara
- Check Windows Event Log untuk detail error

---

## 📊 Build Output Comparison

| Method | Build Time | Difficulty | Need VS2005? | Modifies Exe? |
|--------|------------|------------|--------------|---------------|
| Recompile PWDownloader | 2-4 jam fix | ⭐⭐⭐⭐⭐ Hard | Ya | Ya |
| PCKInjector (This) | 5 menit | ⭐ Easy | Tidak | Tidak |

---

## 🎯 Next Steps

1. Build project dengan VS2019/VS2022
2. Test dengan sample .pck files
3. Integrate ke deployment
4. Distribute dengan PWDownloader.exe

---

## 📝 Summary

**Project ini memungkinkan:**
- ✅ Add PCK verification TANPA recompile PWDownloader
- ✅ Build dengan VS2019/VS2022 (modern tools)
- ✅ Gunakan PWDownloader.exe yang sudah ada
- ✅ Flexible key management
- ✅ No legacy dependencies

**Result:** Anda bisa deploy PCK verification system **sekarang juga** tanpa perlu setup VS2005 VM!
