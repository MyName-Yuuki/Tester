@echo off
REM ========================================
REM   Windows Deployment Package Launcher
REM   PCK Key Verification System
REM ========================================

title PCK Key Verification System - Build Launcher
color 0A

echo.
echo ========================================
echo   PCK KEY VERIFICATION SYSTEM
echo   Build Launcher for Windows
echo ========================================
echo.

REM Check if we're in the right directory
if not exist "CCommon\PckKeyVerifier.h" (
    echo [ERROR] CCommon\PckKeyVerifier.h not found!
    echo.
    echo Please run this script from the LauncherAndPatcher folder.
    echo.
    pause
    exit /b 1
)

echo [OK] All required files detected!
echo.
echo ========================================
echo   MAIN MENU
echo ========================================
echo.
echo   1. Read QUICK START Guide (Recommended)
echo   2. Read WINDOWS DEPLOYMENT Guide
echo   3. Read VS DOWNLOAD Guide
echo   4. View Build Summary
echo   5. Open Project in Visual Studio
echo   6. Verify Build Files
echo   7. View All Documentation
echo   8. Exit
echo.
echo ========================================
echo.

set /p choice="Select option (1-8): "

if "%choice%"=="1" goto quickstart
if "%choice%"=="2" goto deployment
if "%choice%"=="3" goto vsdownload
if "%choice%"=="4" goto summary
if "%choice%"=="5" goto openproject
if "%choice%"=="6" goto verify
if "%choice%"=="7" goto docs
if "%choice%"=="8" goto end

echo.
echo Invalid choice! Please select 1-8.
goto end

:quickstart
echo.
echo Opening QUICK_START.md...
echo.
start QUICK_START.md
goto end

:deployment
echo.
echo Opening WINDOWS_DEPLOYMENT_PACKAGE.md...
echo.
start WINDOWS_DEPLOYMENT_PACKAGE.md
goto end

:vsdownload
echo.
echo Opening VS_DOWNLOAD_GUIDE.md...
echo.
start VS_DOWNLOAD_GUIDE.md
goto end

:summary
echo.
echo Opening WINDOWS_BUILD_SUMMARY.md...
echo.
start WINDOWS_BUILD_SUMMARY.md
goto end

:openproject
echo.
echo ========================================
echo   OPENING PROJECT
echo ========================================
echo.

REM Check if Visual Studio is installed
where devenv.exe >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo [OK] Visual Studio detected!
    echo.
    echo Opening PWDownloader.dsw...
    start ClientTools\PWDownloader\PWDownloader.dsw
    goto end
)

REM Check for VS2008
where vcexpress.exe >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo [OK] Visual Studio 2008 detected!
    echo.
    echo Opening PWDownloader.dsw...
    start ClientTools\PWDownloader\PWDownloader.dsw
    goto end
)

echo [WARNING] Visual Studio not found in PATH!
echo.
echo Please install Visual Studio first.
echo See VS_DOWNLOAD_GUIDE.md for download links.
echo.
echo After installation, you can manually open:
echo   ClientTools\PWDownloader\PWDownloader.dsw
echo.
pause
goto end

:verify
echo.
echo ========================================
echo   BUILD FILE VERIFICATION
echo ========================================
echo.

echo Checking required files...
echo.

if exist "CCommon\PckKeyVerifier.h" (
    echo [OK] CCommon\PckKeyVerifier.h
) else (
    echo [MISSING] CCommon\PckKeyVerifier.h
)

if exist "CCommon\PckKeyVerifier.cpp" (
    echo [OK] CCommon\PckKeyVerifier.cpp
) else (
    echo [MISSING] CCommon\PckKeyVerifier.cpp
)

if exist "ClientTools\PWDownloader\PWDownloaderDlg.cpp" (
    echo [OK] PWDownloaderDlg.cpp
) else (
    echo [MISSING] PWDownloaderDlg.cpp
)

if exist "ClientTools\PWDownloader\EC_Archive7Z.cpp" (
    echo [OK] EC_Archive7Z.cpp
) else (
    echo [MISSING] EC_Archive7Z.cpp
)

if exist "ClientTools\PWDownloader\PWDownloader.dsw" (
    echo [OK] PWDownloader.dsw
) else (
    echo [MISSING] PWDownloader.dsw
)

if exist "ClientTools\PWDownloader\PWDownloader.dsp" (
    echo [OK] PWDownloader.dsp
) else (
    echo [MISSING] PWDownloader.dsp
)

echo.
echo ========================================
echo.

REM Check for key initialization
findstr /C:"PCK_SetKey" ClientTools\PWDownloader\PWDownloaderDlg.cpp >nul
if %ERRORLEVEL% EQU 0 (
    echo [OK] Key initialization found in PWDownloaderDlg.cpp
) else (
    echo [WARNING] Key initialization not found!
)

echo.
echo Verification complete!
echo.
pause
goto end

:docs
echo.
echo ========================================
echo   AVAILABLE DOCUMENTATION
echo ========================================
echo.
echo   QUICK_START.md                  - 5 minutes to build
echo   WINDOWS_DEPLOYMENT_PACKAGE.md   - Complete deployment guide
echo   WINDOWS_BUILD_SUMMARY.md        - Quick reference
echo   VS_DOWNLOAD_GUIDE.md            - Visual Studio downloads
echo   BUILD_GUIDE.md                  - Detailed build guide
echo   VS_UPGRADE_ANALYSIS.md          - VS version comparison
echo   KEY_SETUP_GUIDE.md              - How to change keys
echo   KEY_FORMAT_GUIDE.md             - Key format options
echo   DEPLOYMENT_CHECKLIST.md         - Build verification
echo   PATCHER_KEY_INTEGRATION.md      - Technical details
echo   BUILD_REPORT.txt                - Linux test results
echo   README.md                       - Project overview
echo.
echo ========================================
echo.
set /p doc="Open which file? (or press Enter to cancel): "

if "%doc%"=="" goto end

if exist "%doc%" (
    start %doc%
) else (
    echo File not found: %doc%
    pause
)

goto end

:end
echo.
echo Thank you for using PCK Key Verification System!
echo.
