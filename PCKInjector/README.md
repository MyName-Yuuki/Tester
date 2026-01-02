# PCK Verification Injector

## 🎯 Tujuan
Inject PCK key verification system ke .exe yang sudah ada (PWDownloader.exe) tanpa perlu recompile ulang.

## 📋 Metode yang Tersedia

### Metode 1: DLL Injection (RECOMMENDED)
- Buat DLL berisi PCK verification code
- Inject DLL ke PWDownloader.exe saat runtime
- Hook saat extract .pck files
- ✅ Non-destructive (original .exe tidak dimodifikasi)
- ✅ Bisa di-disable/remove kapan saja
- ⚠️ Butuh injector program

### Metode 2: Code Patching
- Cari offset/alamat di .exe untuk extract function
- Patch dengan jump ke PCK verification code
- ✅ Permanent (tidak butuh injector tiap run)
- ❌ Hard (perlu reverse engineering)
- ❌ Risiko corrupt .exe

### Metode 3: Launcher Wrapper
- Buat launcher baru yang:
  1. Check PCK verification
  2. Jika pass, launch PWDownloader.exe asli
  3. Monitor extract operations
- ✅ Paling mudah
- ✅ Tidak memodifikasi .exe asli
- ❌ PWDownloader tetap bisa extract unprotected .pck

### Metode 4: Resource Replacement
- Ganti resource section di .exe
- Inject PCK verification code sebagai resource
- Load dan execute saat runtime
- ✅ Clean
- ⚠️ Technical complexity tinggi

---

## 🚀 Rekomendasi: DLL Injection + Launcher Wrapper

Kombinasi kedua metode untuk maximum protection:

### Step 1: Buat PCKVerifier.dll
```cpp
// PCKVerifier.dll - berisi:
// - PCK_SetKey()
// - PCK_VerifyKey()
// - Patch EC_Archive7Z::GetStream() function
```

### Step 2: Buat LauncherWrapper.exe
```cpp
// LauncherWrapper.exe:
// 1. Inject PCKVerifier.dll ke PWDownloader.exe
// 2. Set key "RAHASIA123"
// 3. Launch PWDownloader.exe
// 4. Monitor extraction
```

---

## 📂 Project Structure

```
PCKInjector/
├── PCKVerifierDLL/          # DLL project
│   ├── PCKVerifier.cpp      # Verification code
│   ├── PCKVerifier.def      # Exports
│   └── PCKVerifier.vcxproj
│
├── LauncherWrapper/         # Launcher project
│   ├── Main.cpp             # Injection logic
│   ├── Injector.cpp         # DLL injection
│   └── LauncherWrapper.vcxproj
│
├── TestPCK/                # Test .pck files
│   ├── test_protected.pck   # With key
│   └── test_unprotected.pck # Without key
│
└── README.md
```

---

## 🔧 Apa yang Perlu Dibuat?

1. **PCKVerifier.dll** (VS2019 compatible)
   - Pure C code, tidak butuh MFC
   - Export: PCK_SetKey, PCK_VerifyKey, PCK_HasProtection
   - Deteksi dan verify protected .pck files

2. **LauncherWrapper.exe** (VS2019 compatible)
   - Load PWDownloader.exe
   - Inject PCKVerifier.dll
   - Monitor file operations
   - Block extraction jika verification fail

3. **Hook System**
   - Hook CreateFile/ReadFile operations
   - Intercept .pck file access
   - Verify sebelum allow extraction

---

## 💡 Keuntungan Approach Ini

✅ **Tidak perlu recompile PWDownloader** - Gunakan .exe yang sudah ada
✅ **VS2019 compatible** - DLL dan Wrapper fresh code, modern C++
✅ **Flexible** - Bisa ganti key tanpa recompile
✅ **Safe** - Tidak memodifikasi original .exe
✅ **Testable** - Bisa test verification independently
✅ **Maintainable** - Code terpisah, mudah update

---

## 🎯 Next Steps

Pilih metode yang diinginkan dan saya akan buat:
1. Project files (.vcxproj)
2. Source code
3. Build configuration
4. Test script
