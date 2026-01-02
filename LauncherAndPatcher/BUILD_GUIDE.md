# Launcher and Patcher Build Guide

## 📁 Folder Structure

```
LauncherAndPatcher/
├── CBin/                          # Build output folder (Release/Debug EXEs)
├── CBin2.2CRT/                    # Build output for VS2002/2003
├── CCommon/                       # Common libraries and utilities
│   ├── AUTOCA/
│   ├── Face/
│   ├── Gfx/
│   └── Memory/
├── CEleDataMan/                   # Element data manager
├── CElementClient/                # Main client code (dependencies)
│   ├── Arc/
│   ├── CrossServer/
│   ├── Network/
│   ├── GT/
│   ├── Home/
│   ├── Task/
│   └── defence/
├── CElementData/                  # Data structures and types
├── CElementHintLauncher/          # ✅ LAUNCHER APPLICATION
│   ├── ElementHintLauncher.cpp    # Main entry point
│   ├── ElementHintLauncher.dsp    # VC6 project file (FIXED)
│   └── doc/
├── CElementHintTool/              # ✅ HINT TOOL APPLICATION
│   ├── ElementHintTool.cpp        # Main entry point
│   ├── ElementHintTool.dsp        # VC6 project file (FIXED)
│   ├── data/
│   ├── doc/
│   ├── GTransForm/
│   └── Network/
├── CFWEditorLib/                  # Framework editor library
│   ├── DbgRelease/
│   ├── DebugLAAMemProfile/
│   ├── lib/
│   └── Release/
└── ClientTools/                   # ✅ PATCHER TOOLS
    ├── FileExtract/               # File extraction tool
    └── PWDownloader/              # Patcher downloader tool
```

## 🔧 Build Requirements

### Software Requirements
- **Visual Studio 6.0** or **Visual Studio 2003** (.NET)
- **Visual Studio 2005** (for .vcproj files)
- Windows SDK
- Platform SDK (if using VS6)

### Required External Libraries (NOT INCLUDED)
⚠️ **The following libraries are referenced but NOT included in this package:**

1. **CNewSkin Library** (for PWDownloader)
   - Location: Referenced as `../../../CNewSkin/`
   - Contains: `patchlib/`, `p2sp_4th_lib/`, `include/`, `lib/`
   - Files needed:
     - `EC_HttpGet.cpp/h`
     - HTTP download libraries

2. **SDK Libraries** (in `../../../../SDK/`)
   - `A3DSDK/` - Angelica 3D SDK
   - `acsdk/` - Audio SDK
   - `afsdk/` - Framework SDK
   - `3rdSDK/` - Third-party libraries
   - `Amsdk/` - ?
   - `Apsdk/` - ?

3. **Include Folder** (`../../Include/`)
   - Common header files
   - `stlport/` - STLPort library
   - `LuaWrapper/` - Lua bindings

4. **Lib Folder** (`../../Lib/`)
   - Pre-compiled libraries:
     - `AUInterface.lib` / `AUInterface_d.lib`
     - `AngelicaCommon_u.lib` / `AngelicaCommon_ud.lib`
     - `AngelicaFile_u.lib` / `AngelicaFile_ud.lib`
     - `ElementSkill.lib` / `ElementSkill_d.lib`
     - `zliblib.lib`, `zlibwapi.lib`
     - `ws2_32.lib` (Windows Sockets)

## 🚀 Building the Applications

### Option 1: Using Visual Studio 6.0 (.dsp files)

1. **Open Project Files Individually:**
   ```
   # Launcher
   CElementHintLauncher\ElementHintLauncher.dsp

   # Hint Tool
   CElementHintTool\ElementHintTool.dsp

   # Patcher Downloader
   ClientTools\PWDownloader\PWDownloader.dsp

   # File Extractor
   ClientTools\FileExtract\FileExtract.dsp
   ```

2. **Build Configuration:**
   - **Debug:** For development with debugging symbols
   - **Release:** For production build (optimized)

3. **Build Order:**
   1. Build `CElementHintTool` first (required by Launcher)
   2. Build `CElementHintLauncher`
   3. Build `ClientTools\FileExtract` (optional)
   4. Build `ClientTools\PWDownloader` (optional - requires CNewSkin)

### Option 2: Using Solution File

⚠️ **Note:** The solution file `LauncherAndPatcher.sln` is a template. You may need to manually add projects and configure GUIDs.

## 📦 Output Files

After successful build, the following executables will be created in `CBin/`:

### Launcher Applications
- `wmgj_HintLauncher.exe` - Release build
- `wmgj_HintLauncher_d.exe` - Debug build

### Tool Applications
- `wmgj_HintTool.exe` - Release build
- `wmgj_HintTool_d.exe` - Debug build

### Patcher Tools
- `FileExtract.exe` - File extraction tool
- `PWDownloader.exe` - Patcher downloader (⚠️ requires CNewSkin library)

## ⚙️ Configuration

### Path Updates Applied
All relative paths in project files have been updated to work with the new folder structure:

- `..\CElementHintTool\` → `..\..\CElementHintTool\`
- `..\CElementClient\` → `..\..\CElementClient\`
- `..\CCommon\` → `..\..\CCommon\`
- `..\CBin\` → `..\..\CBin\`

### Include Paths Updated
- `/I "..\CCommon"` → `/I "..\..\CCommon"`
- `/I "..\CElementClient\Network"` → `/I "..\..\CElementClient\Network"`

## ❌ Known Issues & Missing Dependencies

### 1. CNewSkin Library (CRITICAL for PWDownloader)
**Status:** ❌ NOT INCLUDED
**Impact:** PWDownloader will NOT build without this library
**Solution:**
- Obtain CNewSkin library from original source
- Place in: `LauncherAndPatcher/../../../CNewSkin/`
- OR remove PWDownloader from build if not needed

### 2. SDK Folders
**Status:** ❌ NOT INCLUDED
**Impact:** Most tools will NOT build without SDKs
**Solution:**
- Obtain required SDKs from original source
- Place in: `LauncherAndPatcher/../../../../SDK/`
- SDKs needed: A3DSDK, acsdk, afsdk, 3rdSDK, Amsdk, Apsdk

### 3. Include & Lib Folders
**Status:** ❌ NOT INCLUDED
**Impact:** Linker errors and missing headers
**Solution:**
- Obtain pre-compiled libraries and headers
- Place in: `LauncherAndPatcher/../../Include/` and `LauncherAndPatcher/../../Lib/`

### 4. AUInterface2
**Status:** ⚠️ REFERENCED BUT LOCATION UNKNOWN
**Impact:** May cause build errors
**Files:** `AUInterface.lib`, headers
**Solution:** Search for this folder in original codebase

## 📝 Build Checklist

Before building, ensure:

- [ ] Visual Studio 6.0 or 2003 installed
- [ ] All required SDKs available
- [ ] CNewSkin library available (for PWDownloader)
- [ ] Include and Lib folders present
- [ ] All project files have correct paths
- [ ] CBin folder exists and is writable

## 🔍 Troubleshooting

### Common Build Errors

1. **"Cannot open include file: 'MyLog.h'"**
   - **Cause:** Path not updated correctly
   - **Fix:** Check include path in `.dsp` file
   - **Should be:** `..\..\CElementHintTool\MyLog.h`

2. **"fatal error LNK1181: cannot open input file 'AUInterface.lib'"**
   - **Cause:** Missing library folder
   - **Fix:** Obtain `../../Lib/` folder with all required `.lib` files

3. **"rc.exe not found"**
   - **Cause:** Resource compiler missing
   - **Fix:** Install Platform SDK or copy `rc.exe` and `rcdll.dll` to VC/bin folder

4. **"Could not find ../../../CNewSkin/patchlib/EC_HttpGet.cpp"**
   - **Cause:** CNewSkin library missing
   - **Fix:** Obtain CNewSkin or remove PWDownloader from build

## 📄 Deployment

### For Development
1. Copy built EXEs from `CBin/` to test environment
2. Ensure all required DLLs are present:
   - `CommonFile_d.dll` (for Debug builds)
   - `p2sp_4th_lib.dll` (for PWDownloader)
   - `7z.dll` (for PWDownloader)

### For Production
1. Use Release builds (without `_d` suffix)
2. Test on clean Windows system
3. Create installer package with:
   - Main executables
   - Required DLLs
   - Configuration files
   - Data files from `CElementHintTool/data/`

## 📞 Support

For issues or questions:
1. Check this BUILD_GUIDE.md first
2. Review project file paths
3. Verify all dependencies are present
4. Check Visual Studio configuration

---

**Last Updated:** 2026-01-01
**Build Status:** ⚠️ Requires External Dependencies
**Tested On:** Windows with Visual Studio 6.0/2003
