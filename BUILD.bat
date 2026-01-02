@echo off
REM ======================================================================
REM BUILD SCRIPT - PWDownloader with Key Verification
REM ======================================================================

echo.
echo ============================================================
echo   PWDownloader Build Script with Key Verification
echo ============================================================
echo.

REM Check if Visual Studio is installed
where msdev >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    set FOUND_MSDEV=1
) else (
    set FOUND_MSDEV=0
)

where devenv >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    set FOUND_DEVENV=1
) else (
    set FOUND_DEVENV=0
)

if %FOUND_MSDEV%==0 if %FOUND_DEVENV%==0 (
    echo ERROR: Visual Studio not found!
    echo Please install Visual Studio 6.0 or .NET 2003
    pause
    exit /b 1
)

echo [OK] Visual Studio detected
echo.

REM Set project paths
set PROJECT_DIR=%~dp0ClientTools\PWDownloader
set PROJECT_FILE=%PROJECT_DIR%\PWDownloader.dsw
set OUTPUT_DIR=%PROJECT_DIR%\Release

echo Project Directory: %PROJECT_DIR%
echo Project File: %PROJECT_FILE%
echo Output Directory: %OUTPUT_DIR%
echo.

REM Check if project file exists
if not exist "%PROJECT_FILE%" (
    echo ERROR: Project file not found!
    echo Expected location: %PROJECT_FILE%
    pause
    exit /b 1
)

echo [OK] Project file found
echo.

echo ============================================================
echo   READY TO BUILD!
echo ============================================================
echo.
echo To build manually:
echo   1. Open: %PROJECT_FILE%
echo   2. Select: Win32 Release
echo   3. Build: Rebuild All
echo.
pause
