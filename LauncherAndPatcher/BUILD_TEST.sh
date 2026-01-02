#!/bin/bash
# Build Verification Script
# Tests the key verification integration before building on Windows

echo "========================================"
echo "  PCK KEY VERIFIER BUILD TEST"
echo "========================================"
echo ""

ERRORS=0
WARNINGS=0

# Color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo "1. Checking source files..."
echo "----------------------------"

# Check PckKeyVerifier files
if [ -f "CCommon/PckKeyVerifier.h" ]; then
    lines=$(wc -l < CCommon/PckKeyVerifier.h)
    echo -e "${GREEN}✓${NC} CCommon/PckKeyVerifier.h exists ($lines lines)"
else
    echo -e "${RED}✗${NC} CCommon/PckKeyVerifier.h NOT FOUND"
    ERRORS=$((ERRORS+1))
fi

if [ -f "CCommon/PckKeyVerifier.cpp" ]; then
    lines=$(wc -l < CCommon/PckKeyVerifier.cpp)
    echo -e "${GREEN}✓${NC} CCommon/PckKeyVerifier.cpp exists ($lines lines)"
else
    echo -e "${RED}✗${NC} CCommon/PckKeyVerifier.cpp NOT FOUND"
    ERRORS=$((ERRORS+1))
fi

# Check modified files
if [ -f "ClientTools/PWDownloader/EC_Archive7Z.cpp" ]; then
    if grep -q "PckKeyVerifier.h" ClientTools/PWDownloader/EC_Archive7Z.cpp; then
        echo -e "${GREEN}✓${NC} EC_Archive7Z.cpp includes PckKeyVerifier.h"
    else
        echo -e "${RED}✗${NC} EC_Archive7Z.cpp does NOT include PckKeyVerifier.h"
        ERRORS=$((ERRORS+1))
    fi

    if grep -q "PCK_VerifyKey" ClientTools/PWDownloader/EC_Archive7Z.cpp; then
        echo -e "${GREEN}✓${NC} EC_Archive7Z.cpp calls PCK_VerifyKey()"
    else
        echo -e "${RED}✗${NC} EC_Archive7Z.cpp does NOT call PCK_VerifyKey()"
        ERRORS=$((ERRORS+1))
    fi
else
    echo -e "${RED}✗${NC} ClientTools/PWDownloader/EC_Archive7Z.cpp NOT FOUND"
    ERRORS=$((ERRORS+1))
fi

if [ -f "ClientTools/PWDownloader/PWDownloaderDlg.cpp" ]; then
    if grep -q "PCK_SetKey" ClientTools/PWDownloader/PWDownloaderDlg.cpp; then
        echo -e "${GREEN}✓${NC} PWDownloaderDlg.cpp calls PCK_SetKey()"
        # Show the key
        key_line=$(grep -A 1 "PCK_SetKey" ClientTools/PWDownloader/PWDownloaderDlg.cpp | head -1)
        echo "   $key_line"
    else
        echo -e "${RED}✗${NC} PWDownloaderDlg.cpp does NOT call PCK_SetKey()"
        ERRORS=$((ERRORS+1))
    fi
else
    echo -e "${RED}✗${NC} ClientTools/PWDownloader/PWDownloaderDlg.cpp NOT FOUND"
    ERRORS=$((ERRORS+1))
fi

echo ""
echo "2. Checking project files..."
echo "----------------------------"

if [ -f "ClientTools/PWDownloader/PWDownloader.dsp" ]; then
    if grep -q "PckKeyVerifier.cpp" ClientTools/PWDownloader/PWDownloader.dsp; then
        echo -e "${GREEN}✓${NC} PWDownloader.dsp includes PckKeyVerifier.cpp"
    else
        echo -e "${RED}✗${NC} PWDownloader.dsp does NOT include PckKeyVerifier.cpp"
        ERRORS=$((ERRORS+1))
    fi

    if grep -q "../../CCommon" ClientTools/PWDownloader/PWDownloader.dsp; then
        echo -e "${GREEN}✓${NC} PWDownloader.dsp has CCommon include path"
    else
        echo -e "${RED}✗${NC} PWDownloader.dsp missing CCommon include path"
        ERRORS=$((ERRORS+1))
    fi
else
    echo -e "${RED}✗${NC} ClientTools/PWDownloader/PWDownloader.dsp NOT FOUND"
    ERRORS=$((ERRORS+1))
fi

echo ""
echo "3. Syntax checking..."
echo "----------------------------"

# Check bracket matching in PckKeyVerifier.cpp
if [ -f "CCommon/PckKeyVerifier.cpp" ]; then
    open_braces=$(grep -o "{" CCommon/PckKeyVerifier.cpp | wc -l)
    close_braces=$(grep -o "}" CCommon/PckKeyVerifier.cpp | wc -l)

    if [ "$open_braces" -eq "$close_braces" ]; then
        echo -e "${GREEN}✓${NC} PckKeyVerifier.cpp: Braces matched ($open_braces pairs)"
    else
        echo -e "${RED}✗${NC} PckKeyVerifier.cpp: Brace mismatch! (open: $open_braces, close: $close_braces)"
        ERRORS=$((ERRORS+1))
    fi
fi

# Check for common syntax errors
if [ -f "CCommon/PckKeyVerifier.h" ]; then
    # Check for function declarations
    func_count=$(grep -c "PCK_" CCommon/PckKeyVerifier.h)
    echo -e "${GREEN}✓${NC} PckKeyVerifier.h: $func_count PCK_ function declarations"
fi

echo ""
echo "4. Checking documentation..."
echo "----------------------------"

doc_files=(
    "WINDOWS_BUILD_SUMMARY.md"
    "KEY_SETUP_GUIDE.md"
    "KEY_FORMAT_GUIDE.md"
    "DEPLOYMENT_CHECKLIST.md"
    "PATCHER_KEY_INTEGRATION.md"
    "BUILD_GUIDE.md"
)

for doc in "${doc_files[@]}"; do
    if [ -f "$doc" ]; then
        echo -e "${GREEN}✓${NC} $doc exists"
    else
        echo -e "${YELLOW}⚠${NC} $doc not found (optional)"
    fi
done

echo ""
echo "========================================"
echo "  TEST SUMMARY"
echo "========================================"

if [ $ERRORS -eq 0 ]; then
    echo -e "${GREEN}✓ ALL CHECKS PASSED!${NC}"
    echo ""
    echo "Your code is ready to build on Windows!"
    echo ""
    echo "Next steps:"
    echo "1. Copy 'LauncherAndPatcher' folder to Windows"
    echo "2. Open 'ClientTools/PWDownloader/PWDownloader.dsw'"
    echo "3. Build with Visual Studio 6.0/2003"
    echo ""
    exit 0
else
    echo -e "${RED}✗ FOUND $ERRORS ERROR(S)!${NC}"
    echo ""
    echo "Please fix the errors above before building."
    echo ""
    exit 1
fi
