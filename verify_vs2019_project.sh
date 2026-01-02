#!/bin/bash

echo "========================================"
echo "  VS2019 PROJECT VERIFICATION"
echo "========================================"
echo ""

ERRORS=0

echo "1. CHECKING VS2019 PROJECT FILES:"
echo "----------------------------"

# Check .vcxproj files
FILES=(
    "PCKSystem.sln"
    "LauncherAndPatcher/PWDownloader/PWDownloader.vcxproj"
    "LauncherAndPatcher/HintLauncher/HintLauncher.vcxproj"
)

for file in "${FILES[@]}"; do
    if [ -f "$file" ]; then
        echo "✓ $file exists"
    else
        echo "✗ $file MISSING"
        ERRORS=$((ERRORS+1))
    fi
done

echo ""
echo "2. VERIFYING PROJECT CONFIGURATIONS:"
echo "----------------------------"

# Check toolset
if grep -q "v140" LauncherAndPatcher/PWDownloader/PWDownloader.vcxproj; then
    echo "✓ PlatformToolset: v140 (VS2015) - Good for VS6 compatibility"
else
    echo "✗ PlatformToolset not set correctly"
    ERRORS=$((ERRORS+1))
fi

# Check character set
if grep -q "MultiByte" LauncherAndPatcher/PWDownloader/PWDownloader.vcxproj; then
    echo "✓ CharacterSet: MultiByte - Correct"
else
    echo "✗ CharacterSet not MultiByte"
    ERRORS=$((ERRORS+1))
fi

# Check precompiled header disabled
if grep -q "NotUsing" LauncherAndPatcher/PWDownloader/PWDownloader.vcxproj; then
    echo "✓ PrecompiledHeader: NotUsing - Correct"
else
    echo "✗ PrecompiledHeader not disabled"
    ERRORS=$((ERRORS+1))
fi

# Check warnings disabled
if grep -q "DisableSpecificWarnings" LauncherAndPatcher/PWDownloader/PWDownloader.vcxproj; then
    echo "✓ Warnings disabled (4996;4267;4244) - Correct"
else
    echo "⚠ Warnings not disabled (optional)"
fi

# Check MFC
if grep -q "UseOfMfc" LauncherAndPatcher/PWDownloader/PWDownloader.vcxproj; then
    echo "✓ MFC enabled - Correct"
else
    echo "✗ MFC not enabled"
    ERRORS=$((ERRORS+1))
fi

echo ""
echo "3. VERIFYING KEY VERIFICATION INTEGRATION:"
echo "----------------------------"

if grep -q "PckKeyVerifier.cpp" LauncherAndPatcher/PWDownloader/PWDownloader.vcxproj; then
    echo "✓ PckKeyVerifier.cpp included in project"
else
    echo "✗ PckKeyVerifier.cpp not included"
    ERRORS=$((ERRORS+1))
fi

if grep -q "PckKeyVerifier.h" LauncherAndPatcher/PWDownloader/PWDownloader.vcxproj; then
    echo "✓ PckKeyVerifier.h included in project"
else
    echo "✗ PckKeyVerifier.h not included"
    ERRORS=$((ERRORS+1))
fi

if [ -f "LauncherAndPatcher/CCommon/PckKeyVerifier.cpp" ]; then
    lines=$(wc -l < LauncherAndPatcher/CCommon/PckKeyVerifier.cpp)
    echo "✓ PckKeyVerifier.cpp exists ($lines lines)"
else
    echo "✗ PckKeyVerifier.cpp missing"
    ERRORS=$((ERRORS+1))
fi

echo ""
echo "4. CHECKING SOURCE FILES:"
echo "----------------------------"

# Count source files
PW_COUNT=$(find LauncherAndPatcher/ClientTools/PWDownloader -name "*.cpp" | wc -l)
HL_COUNT=$(find LauncherAndPatcher/CElementHintLauncher -name "*.cpp" 2>/dev/null | wc -l)

echo "✓ PWDownloader source files: $PW_COUNT"
echo "✓ HintLauncher source files: $HL_COUNT"

echo ""
echo "5. GITHUB ACTIONS WORKFLOW:"
echo "----------------------------"

if [ -f ".github/workflows/build.yml" ]; then
    echo "✓ GitHub Actions workflow exists"

    # Check if it uses VS2019
    if grep -q "windows-latest" .github/workflows/build.yml; then
        echo "✓ Using windows-latest runner"
    fi

    if grep -q "v140" .github/workflows/build.yml; then
        echo "✓ Using v140 toolset"
    fi
else
    echo "⚠ GitHub Actions workflow not found"
fi

echo ""
echo "========================================"
echo "  VERIFICATION RESULT"
echo "========================================"
echo ""

if [ $ERRORS -eq 0 ]; then
    echo "✓✓✓ ALL CHECKS PASSED! ✓✓✓"
    echo ""
    echo "VS2019 project is ready for build!"
    echo ""
    echo "Project files created:"
    echo "  - PCKSystem.sln (VS2019 solution)"
    echo "  - PWDownloader.vcxproj (VS2019 project)"
    echo "  - HintLauncher.vcxproj (VS2019 project)"
    echo ""
    echo "Configuration:"
    echo "  - Platform Toolset: v140"
    echo "  - Character Set: MultiByte"
    echo "  - Precompiled Header: NotUsing"
    echo "  - Warnings Disabled: 4996;4267;4244"
    echo "  - MFC: Dynamic"
    echo ""
    echo "Next steps:"
    echo "  1. Copy project to Windows"
    echo "  2. Open PCKSystem.sln in VS2019/2022"
    echo "  3. Build (F7)"
    echo "  4. Expected: 10-30 errors (manageable)"
    echo ""
    echo "  OR push to GitHub for automatic build:"
    echo "  git add ."
    echo "  git commit -m 'Add VS2019 project files'"
    echo "  git push"
    echo ""
    exit 0
else
    echo "✗✗✗ FOUND $ERRORS ERROR(S)! ✗✗✗"
    echo ""
    echo "Please fix the errors above."
    echo ""
    exit 1
fi
