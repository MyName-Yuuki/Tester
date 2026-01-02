// Filename	: AUITextArea.cpp
// Creator	: Tom Zhou
// Date		: December 17, 2004
// Desc		: AUITextArea is the class of text area control.

#include "AUI.h"

#include "AM.h"
#include "A3DMacros.h"
#include "AScriptFile.h"
#include "A3DEngine.h"
#include "A3DDevice.h"
#include "A3DViewport.h"
#include "A3DFTFontMan.h"
#include "A3DFTFont.h"
#include "A2DSprite.h"
#include "A2DSpriteItem.h"

#include "CSplit.h"

#include "AUIDef.h"
#include "AUICommon.h"
#include "AUIDialog.h"
#include "AUITextArea.h"
#include "AUIManager.h"
#include "AUICTranslate.h"

#define AUITEXTAREA_LINESPACE		2
#define AUITEXTAREA_SCROLL_DELAY	50

AUITextArea::AUITextArea() : AUIObject()
{
	m_nType = AUIOBJECT_TYPE_TEXTAREA;
	m_nFrameMode = 0;
	m_pAUIFrame = NULL;
	for( int i = 0; i < AUITEXTAREA_RECT_NUM; i++ )
		m_pA2DSprite[i] = NULL;
	m_pA2DSpriteEmotion = NULL;
	m_vecEmotion = NULL;
	m_pvecImageList = NULL;
	m_pvecOtherFonts = NULL;
	m_nState = AUITEXTAREA_STATE_NORMAL;
	m_bDragging = false;
	m_bOnlyShowText = false;
	m_bLeftScrollBar = false;
	m_bScrollToBottom = false;
	m_nScrollDelay = GetTickCount();
	m_ptBar.x = m_ptBar.y = m_nDragDist = 0;
	m_nFirstLine = m_nLines = m_nLinesPP = m_nLinesThisPage = 0;
	m_bTextAppended = false;
	m_clLast = A3DCOLORRGB(255, 255, 255);
	m_UnderLineInfoLast.bUnderLine = false;
	m_UnderLineInfoLast.bSameColor = true;
	m_UnderLineInfoLast.color = A3DCOLORRGB(0, 0, 0);
	m_UnderLineInfoLast.fScale = 1.0f;
	m_nFontShadowLast.pFont = NULL;
	m_nFontShadowLast.nShadow = 0;
	m_ItemUnderLineMode = UM_NO_UNDERLINE;
	m_clItemUnderLine = A3DCOLORRGB(0, 0, 0);
	m_nMaxLines = -1;
	m_bTextTransparent = false;
	m_bTransScrollWheel = true;
	m_nItemMask = 0xffffffff;
	m_nLineSpace = AUITEXTAREA_LINESPACE;
	m_bForceRenderScroll = true;
}

AUITextArea::~AUITextArea()
{
	ClearLinesInfo();
}

bool AUITextArea::Init(A3DEngine *pA3DEngine, A3DDevice *pA3DDevice, AScriptFile *pASF)
{
	/*	ListBox property line format:
	 *
	 *		TEXT					Keyword
	 *		Server					Name
	 *		""						Command.
	 *		10 10 200 25			x y Width Height
	 *		1						String ID of font name
	 *		20						Font height
	 *		0 0 0 255				Text color, R, G, B, A
	 *		"ListFrame.bmp"			ImageFileName
	 *		"ListUp.bmp"			ImageFileName
	 *		"ListDown.bmp"			ImageFileName
	 *		"ListScroll.bmp"		ImageFileName
	 *		"ListBar.bmp"			ImageFileName
	 */
	if (!AUIObject::Init(pA3DEngine, pA3DDevice, pASF))
		return AUI_ReportError(__LINE__, 1, "AUITextArea::Init(), failed to call base AUIObject::Init()");

	if( !pASF ) return true;

	BEGIN_FAKE_WHILE2;

	// Font
	CHECK_BREAK2(pASF->GetNextToken(true));
	int idString = atoi(pASF->m_szToken);

	ACString strFontName = m_pParent->GetStringFromTable(idString);
	if( strFontName.GetLength() == 0 ) strFontName = _AL("方正细黑一简体");
	CSplit s(strFontName);
	CSPLIT_STRING_VECTOR vec = s.Split(_AL("###"));
	m_szFontName = vec[0];
	if( vec.size() > 1 ) m_nShadow = a_atoi(vec[1]);
	if( vec.size() > 2 ) m_nOutline = a_atoi(vec[2]);
	if( vec.size() > 3 ) m_bBold = a_atoi(vec[3]) ? true : false;
	if( vec.size() > 4 ) m_bItalic = a_atoi(vec[4]) ? true : false;

	CHECK_BREAK2(pASF->GetNextToken(true));
	m_nFontHeight = atoi(pASF->m_szToken);

	if (!SetFontAndColor(m_szFontName, m_nFontHeight,
		m_color, m_nShadow, m_nOutline, m_bBold, m_bItalic))
		return AUI_ReportError(__LINE__, 1, "AUITextArea::Init(), failed to call SetFontAndColor()");

	// Text color
	CHECK_BREAK2(pASF->GetNextToken(true));
	int r = atoi(pASF->m_szToken);

	CHECK_BREAK2(pASF->GetNextToken(true));
	int g = atoi(pASF->m_szToken);

	CHECK_BREAK2(pASF->GetNextToken(true));
	int b = atoi(pASF->m_szToken);

	CHECK_BREAK2(pASF->GetNextToken(true));
	int a = atoi(pASF->m_szToken);

	m_color = A3DCOLORRGBA(r, g, b, a);

	for( int i = AUITEXTAREA_RECT_MIN; i < AUITEXTAREA_RECT_MAX; i++ )
	{
		if (!pASF->GetNextToken(true))
			return AUI_ReportError(__LINE__, 1, "AUITextArea::Init(), failed to read from file");

		if (!InitIndirect(pASF->m_szToken, i))
			return AUI_ReportError(__LINE__, 1, "AUITextArea::Init(), failed to call InitIndirect()");
	}
	
	if( m_pParent->GetFileVersion() > 0 )
	{
		CHECK_BREAK2(pASF->GetNextToken(true));
		AUIOBJECT_SETPROPERTY p;
		p.i = atoi(pASF->m_szToken);
		SetProperty("Frame Mode", &p);
	}

	END_FAKE_WHILE2;

	BEGIN_ON_FAIL_FAKE_WHILE2;

	AUI_ReportError(DEFAULT_2_PARAM, "AUITextArea::Init(), failed to read from file");

	END_ON_FAIL_FAKE_WHILE2;

	RETURN_FAKE_WHILE_RESULT2;
}

bool AUITextArea::InitXML(A3DEngine *pA3DEngine, A3DDevice *pA3DDevice, AXMLItem *pItem)
{
	ASSERT(pA3DEngine && pA3DDevice);
	m_pA3DEngine = pA3DEngine;
	m_pA3DDevice = pA3DDevice;
	m_szFontName = m_pParent->GetAUIManager()->GetDefaultFontName();
	m_nFontHeight = m_pParent->GetAUIManager()->GetDefaultFontSize();
	
	XMLGET_STRING_VALUE(pItem, _AL("Name"), m_szName)
	XMLGET_INT_VALUE(pItem, _AL("x"), m_x)
	XMLGET_INT_VALUE(pItem, _AL("y"), m_y)
	XMLGET_INT_VALUE(pItem, _AL("Width"), m_nWidth)
	XMLGET_INT_VALUE(pItem, _AL("Height"), m_nHeight)
	XMLGET_BOOL_VALUE(pItem, _AL("TextTransparent"), m_bTextTransparent)
	XMLGET_BOOL_VALUE(pItem, _AL("TransScrollWheel"), m_bTransScrollWheel)
	XMLGET_BOOL_VALUE(pItem, _AL("LeftScrollBar"), m_bLeftScrollBar)
	XMLGET_INT_VALUE(pItem, _AL("LineSpace"), m_nLineSpace)
	XMLGET_INT_VALUE(pItem, _AL("LinkUnderLineMode"), m_ItemUnderLineMode)
	XMLGET_COLOR_VALUE(pItem, _AL("LinkUnderLineColor"), m_clItemUnderLine)
	XMLGET_STRING_VALUE(pItem, _AL("Template"), m_strTemplate)

	m_nDefaultX = m_x;
	m_nDefaultY = m_y;
	m_nDefaultWidth = m_nWidth;
	m_nDefaultHeight = m_nHeight;

	AXMLItem *pChildItem = pItem->GetFirstChildItem();
	while( pChildItem )
	{
		if( a_stricmp(pChildItem->GetName(), _AL("Resource")) == 0 )
		{
			AXMLItem *pResouceChildItem = pChildItem->GetFirstChildItem();
			while( pResouceChildItem )
			{
				if( a_stricmp(pResouceChildItem->GetName(), _AL("FrameImage")) == 0 )
				{
					XMLGET_INT_VALUE(pResouceChildItem, _AL("FrameMode"), m_nFrameMode)
					AString strFileName;
					XMLGET_STRING_VALUE(pResouceChildItem, _AL("FileName"), strFileName)
					InitIndirect(strFileName, AUITEXTAREA_RECT_FRAME);
				}
				else if( a_stricmp(pResouceChildItem->GetName(), _AL("UpImage")) == 0 )
				{
					AString strFileName;
					XMLGET_STRING_VALUE(pResouceChildItem, _AL("FileName"), strFileName)
					InitIndirect(strFileName, AUITEXTAREA_RECT_UP);
				}
				else if( a_stricmp(pResouceChildItem->GetName(), _AL("DownImage")) == 0 )
				{
					AString strFileName;
					XMLGET_STRING_VALUE(pResouceChildItem, _AL("FileName"), strFileName)
					InitIndirect(strFileName, AUITEXTAREA_RECT_DOWN);
				}
				else if( a_stricmp(pResouceChildItem->GetName(), _AL("ScrollImage")) == 0 )
				{
					AString strFileName;
					XMLGET_STRING_VALUE(pResouceChildItem, _AL("FileName"), strFileName)
					InitIndirect(strFileName, AUITEXTAREA_RECT_SCROLL);
				}
				else if( a_stricmp(pResouceChildItem->GetName(), _AL("BarImage")) == 0 )
				{
					AString strFileName;
					XMLGET_STRING_VALUE(pResouceChildItem, _AL("FileName"), strFileName)
					InitIndirect(strFileName, AUITEXTAREA_RECT_BAR);
				}
				pResouceChildItem = pResouceChildItem->GetNextItem();
			}
		}
		else if( a_stricmp(pChildItem->GetName(), _AL("Text")) == 0 )
		{
			XMLGET_WSTRING_VALUE(pChildItem, _AL("String"), m_szText);
			XMLGET_WSTRING_VALUE(pChildItem, _AL("FontName"), m_szFontName)
			XMLGET_INT_VALUE(pChildItem, _AL("FontSize"), m_nFontHeight)
			XMLGET_INT_VALUE(pChildItem, _AL("Shadow"), m_nShadow)
			XMLGET_BOOL_VALUE(pChildItem, _AL("Bold"), m_bBold)
			XMLGET_BOOL_VALUE(pChildItem, _AL("Italic"), m_bItalic)
			XMLGET_BOOL_VALUE(pChildItem, _AL("FreeType"), m_bFreeType)
			XMLGET_COLOR_VALUE(pChildItem, _AL("Color"), m_color)
			XMLGET_INT_VALUE(pChildItem, _AL("Outline"), m_nOutline)
			XMLGET_COLOR_VALUE(pChildItem, _AL("OutlineColor"), m_clOutline)
			XMLGET_COLOR_VALUE(pChildItem, _AL("InnerTextColor"), m_clInnerText)
			XMLGET_BOOL_VALUE(pChildItem, _AL("FadeColor"), m_bInterpColor)
			XMLGET_COLOR_VALUE(pChildItem, _AL("TextUpperColor"), m_clUpper)
			XMLGET_COLOR_VALUE(pChildItem, _AL("TextLowerColor"), m_clLower)
		}
		else if( a_stricmp(pChildItem->GetName(), _AL("Hint")) == 0)
		{
			ACString strHint;
			XMLGET_WSTRING_VALUE(pChildItem, _AL("String"), strHint);
			AUICTranslate trans;
			m_szHint = trans.Translate(strHint);
		}
		pChildItem = pChildItem->GetNextItem();
	}

	if (!SetFontAndColor(m_szFontName, m_nFontHeight, m_color, m_nShadow, m_nOutline, m_bBold, m_bItalic, 
			m_bFreeType, m_clOutline, m_clInnerText, m_bInterpColor, m_clUpper, m_clLower))
		return AUI_ReportError(__LINE__, 1, "AUITextArea::InitXML(), failed to call SetFontAndColor()");
	
	if( m_pParent )
		SetEmotionList(m_pParent->GetAUIManager()->GetDefaultSpriteEmotion(), m_pParent->GetAUIManager()->GetDefaultEmotion());

	AUICTranslate translate;
	SetText(translate.Translate(m_szText));
	return true;
}

bool AUITextArea::InitIndirect(const char *pszFileName, int idType)
{
	bool bval;

	ASSERT(m_pA3DEngine && m_pA3DDevice);
	ASSERT(idType >= AUITEXTAREA_RECT_MIN && idType < AUITEXTAREA_RECT_MAX);

	UpdateRenderTarget();
	if( AUIDIALOG_FRAME_FRAME == idType )
	{
		A3DRELEASE(m_pAUIFrame);
		if( strlen(pszFileName) <= 0 ) return true;

		m_pAUIFrame = new AUIFrame;
		if( !m_pAUIFrame ) return false;

		bval = m_pAUIFrame->Init(m_pA3DEngine, m_pA3DDevice, pszFileName, m_nFrameMode);
		if( !bval )
		{
			A3DRELEASE(m_pAUIFrame)
			return AUI_ReportError(__LINE__, 1, "AUITextArea::InitIndirect(), failed to call m_pAUIFrame->Init()");
		}

		if( m_pParent && m_pParent->GetAUIManager() )
			m_pAUIFrame->SetScale(m_pParent->GetAUIManager()->GetWindowScale() * m_pParent->GetScale());
	}
	else
	{
		A3DRELEASE(m_pA2DSprite[idType]);
		if( strlen(pszFileName) <= 0 ) return true;
	
		m_pA2DSprite[idType] = new A2DSprite;
		if( !m_pA2DSprite[idType] ) return false;
		
		bval = m_pA2DSprite[idType]->Init(m_pA3DDevice, pszFileName, AUI_COLORKEY);
		if( !bval )
		{
			A3DRELEASE(m_pA2DSprite[idType])
			return AUI_ReportError(__LINE__, 1, "AUITextArea::InitIndirect(), failed to call m_pA2DSprite[%d]->Init()", idType);
		}

		if( AUITEXTAREA_RECT_UP == idType || AUITEXTAREA_RECT_DOWN == idType )
		{
			A3DRECT rc[2];
			int W = m_pA2DSprite[idType]->GetWidth();
			int H = m_pA2DSprite[idType]->GetHeight();
			rc[0].SetRect(0, 0, W, H / 2);
			rc[1].SetRect(0, H / 2, W, H);
			bval = m_pA2DSprite[idType]->ResetItems(2, rc);
			if( !bval ) return AUI_ReportError(__LINE__, 1, "AUITextArea::InitIndirect(), failed to call m_pA2DSprite[%d]->ResetItems()", idType);
		}
	}

	return true;
}

bool AUITextArea::Release(void)
{
	OptimizeSpriteResource(true);

	A3DRELEASE(m_pAUIFrame);
	for( int i = 0; i < AUITEXTAREA_RECT_NUM; i++ )
		A3DRELEASE(m_pA2DSprite[i]);

	ClearLinesInfo();
	return AUIObject::Release();
}

bool AUITextArea::Save(FILE *file)
{
	fprintf(file, "TEXT");

	if( !AUIObject::Save(file) ) return false;

	ACHAR szFontName[256];
	a_sprintf(szFontName, _AL("%s###%d###%d###%d###%d"),
		m_szFontName, m_nShadow, m_nOutline, m_bBold, m_bItalic);
	int idString = m_pParent->AddStringToTable(szFontName);
	if( idString < 0 ) return false;

	fprintf(file, " %d %d %d %d %d %d", idString, m_nFontHeight,
		A3DCOLOR_GETRED(m_color), A3DCOLOR_GETGREEN(m_color),
		A3DCOLOR_GETBLUE(m_color), A3DCOLOR_GETALPHA(m_color));

	for( int i = 0; i < AUITEXTAREA_RECT_NUM; i++ )
	{
		if( AUITEXTAREA_RECT_FRAME == i )
		{
			if( m_pAUIFrame )
				m_pAUIFrame->Save(file);
			else
				fprintf(file, " \"\"");
		}
		else
			fprintf(file, " \"%s\"", m_pA2DSprite[i] ? m_pA2DSprite[i]->GetName() : "");
	}

	fprintf(file, " %d\n", m_nFrameMode);

	return true;
}

bool AUITextArea::SaveXML(AXMLItem *pXMLItemDialog)
{
	AXMLItem *pXMLItemTextArea = new AXMLItem;
	pXMLItemTextArea->InsertItem(pXMLItemDialog);
	pXMLItemTextArea->SetName(_AL("TEXT"));
	XMLSET_STRING_VALUE(pXMLItemTextArea, _AL("Name"), AS2AC(m_szName), _AL(""))
	XMLSET_INT_VALUE(pXMLItemTextArea, _AL("x"), m_x, 0)
	XMLSET_INT_VALUE(pXMLItemTextArea, _AL("y"), m_y, 0)
	XMLSET_INT_VALUE(pXMLItemTextArea, _AL("Width"), m_nWidth, 0)
	XMLSET_INT_VALUE(pXMLItemTextArea, _AL("Height"), m_nHeight, 0)
	XMLSET_BOOL_VALUE(pXMLItemTextArea, _AL("LeftScrollBar"), m_bLeftScrollBar, false)
	XMLSET_BOOL_VALUE(pXMLItemTextArea, _AL("TextTransparent"), m_bTextTransparent, false)
	XMLSET_BOOL_VALUE(pXMLItemTextArea, _AL("TransScrollWheel"), m_bTransScrollWheel, true)
	XMLSET_INT_VALUE(pXMLItemTextArea, _AL("LineSpace"), m_nLineSpace, AUITEXTAREA_LINESPACE)
	XMLSET_INT_VALUE(pXMLItemTextArea, _AL("LinkUnderLineMode"), m_ItemUnderLineMode, UM_NO_UNDERLINE)
	XMLSET_COLOR_VALUE(pXMLItemTextArea, _AL("LinkUnderLineColor"), m_clItemUnderLine, _AL("0,0,0,255"))
	XMLSET_STRING_VALUE(pXMLItemTextArea, _AL("Template"), AS2AC(m_strTemplate), _AL(""))

	AXMLItem *pXMLItemHint = new AXMLItem;
	pXMLItemHint->InsertItem(pXMLItemTextArea);
	pXMLItemHint->SetName(_AL("Hint"));
	AUICTranslate trans;
	XMLSET_STRING_VALUE(pXMLItemHint, _AL("String"), trans.ReverseTranslate(m_szHint), _AL(""))

	AXMLItem *pXMLItemText = new AXMLItem;
	pXMLItemText->InsertItem(pXMLItemTextArea);
	pXMLItemText->SetName(_AL("Text"));
	AUICTranslate translate;
	XMLSET_STRING_VALUE(pXMLItemText, _AL("String"), translate.ReverseTranslate(m_szText), _AL(""))
	XMLSET_STRING_VALUE(pXMLItemText, _AL("FontName"), m_szFontName, _AL(""))
	XMLSET_INT_VALUE(pXMLItemText, _AL("FontSize"), m_nFontHeight, 0)
	XMLSET_INT_VALUE(pXMLItemText, _AL("Shadow"), m_nShadow, 0)
	XMLSET_BOOL_VALUE(pXMLItemText, _AL("Bold"), m_bBold, false)
	XMLSET_BOOL_VALUE(pXMLItemText, _AL("Italic"), m_bItalic, false)
	XMLSET_BOOL_VALUE(pXMLItemText, _AL("FreeType"), m_bFreeType, true)
	XMLSET_COLOR_VALUE(pXMLItemText, _AL("Color"), m_color, _AL("255,255,255,255"));
	XMLSET_INT_VALUE(pXMLItemText, _AL("Outline"), m_nOutline, 0)
	XMLSET_COLOR_VALUE(pXMLItemText, _AL("OutlineColor"), m_clOutline, _AL("0,0,0,255"))
	XMLSET_COLOR_VALUE(pXMLItemText, _AL("InnerTextColor"), m_clInnerText, _AL("255,255,255,255"))
	XMLSET_BOOL_VALUE(pXMLItemText, _AL("FadeColor"), m_bInterpColor, 0)
	XMLSET_COLOR_VALUE(pXMLItemText, _AL("TextUpperColor"), m_clUpper, _AL("255,255,255,255"))
	XMLSET_COLOR_VALUE(pXMLItemText, _AL("TextLowerColor"), m_clLower, _AL("255,255,255,255"))
	
	AXMLItem *pXMLItemResource = new AXMLItem;
	pXMLItemResource->InsertItem(pXMLItemTextArea);
	pXMLItemResource->SetName(_AL("Resource"));
	
	if( m_pAUIFrame )
	{
		AXMLItem *pXMLItemFrameImage = new AXMLItem;
		pXMLItemFrameImage->InsertItem(pXMLItemResource);
		pXMLItemFrameImage->SetName(_AL("FrameImage"));
		XMLSET_STRING_VALUE(pXMLItemFrameImage, _AL("FileName"), AS2AC(m_pAUIFrame->GetA2DSprite()->GetName()), _AL(""))
		XMLSET_INT_VALUE(pXMLItemFrameImage, _AL("FrameMode"), m_nFrameMode, 0)
	}
	if( m_pA2DSprite[AUITEXTAREA_RECT_UP] )
	{
		AXMLItem *pXMLItemUpImage = new AXMLItem;
		pXMLItemUpImage->InsertItem(pXMLItemResource);
		pXMLItemUpImage->SetName(_AL("UpImage"));
		XMLSET_STRING_VALUE(pXMLItemUpImage, _AL("FileName"), AS2AC(m_pA2DSprite[AUITEXTAREA_RECT_UP]->GetName()), _AL(""))
	}
	if( m_pA2DSprite[AUITEXTAREA_RECT_DOWN] )
	{
		AXMLItem *pXMLItemDownImage = new AXMLItem;
		pXMLItemDownImage->InsertItem(pXMLItemResource);
		pXMLItemDownImage->SetName(_AL("DownImage"));
		XMLSET_STRING_VALUE(pXMLItemDownImage, _AL("FileName"), AS2AC(m_pA2DSprite[AUITEXTAREA_RECT_DOWN]->GetName()), _AL(""))
	}
	if( m_pA2DSprite[AUITEXTAREA_RECT_SCROLL] )
	{
		AXMLItem *pXMLItemScrollImage = new AXMLItem;
		pXMLItemScrollImage->InsertItem(pXMLItemResource);
		pXMLItemScrollImage->SetName(_AL("ScrollImage"));
		XMLSET_STRING_VALUE(pXMLItemScrollImage, _AL("FileName"), AS2AC(m_pA2DSprite[AUITEXTAREA_RECT_SCROLL]->GetName()), _AL(""))
	}
	if( m_pA2DSprite[AUITEXTAREA_RECT_BAR] )
	{
		AXMLItem *pXMLItemBarImage = new AXMLItem;
		pXMLItemBarImage->InsertItem(pXMLItemResource);
		pXMLItemBarImage->SetName(_AL("BarImage"));
		XMLSET_STRING_VALUE(pXMLItemBarImage, _AL("FileName"), AS2AC(m_pA2DSprite[AUITEXTAREA_RECT_BAR]->GetName()), _AL(""))
	}

	return true;
}

bool AUITextArea::OnDlgItemMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if( !m_pAUIFrame ||
		!m_pA2DSprite[AUITEXTAREA_RECT_UP] ||
		!m_pA2DSprite[AUITEXTAREA_RECT_DOWN] ||
		!m_pA2DSprite[AUITEXTAREA_RECT_SCROLL] )
	{
		return AUIObject::OnDlgItemMessage(msg, wParam, lParam);
	}

	int nLines = m_nLines;
	int nLinesPP = m_nLinesPP;
	SIZE sLimit = {0, 0};
	if( m_pAUIFrame )
		sLimit = m_pAUIFrame->GetSizeLimit();
	int cx = sLimit.cx / 2;
	int cy = sLimit.cy / 2;
	A3DRECT rcUp = m_pA2DSprite[AUITEXTAREA_RECT_UP]->GetItem(0)->GetRect();
	A3DRECT rcDown = m_pA2DSprite[AUITEXTAREA_RECT_DOWN]->GetItem(0)->GetRect();
	A3DRECT rcScroll = m_pA2DSprite[AUITEXTAREA_RECT_SCROLL]->GetItem(0)->GetRect();
	float fWindowScale = m_pParent->GetAUIManager()->GetWindowScale() * m_pParent->GetScale();
	rcUp.SetRect(rcUp.left * fWindowScale, rcUp.top * fWindowScale,
		rcUp.right * fWindowScale, rcUp.bottom * fWindowScale);
	rcDown.SetRect(rcDown.left * fWindowScale, rcDown.top * fWindowScale,
		rcDown.right * fWindowScale, rcDown.bottom * fWindowScale);
	rcScroll.SetRect(rcScroll.left * fWindowScale, rcScroll.top * fWindowScale,
		rcScroll.right * fWindowScale, rcScroll.bottom * fWindowScale);
	A3DRECT rcText(cx, cy, m_nWidth	- cx - rcScroll.Width(), m_nHeight - cy);
	A3DRECT rcScrollArea(m_nWidth - rcScroll.Width(), rcUp.Height(), m_nWidth, m_nHeight - rcDown.Height());

	if( m_bLeftScrollBar )
	{
		rcText.Offset(rcScroll.Width(), 0);
		rcScrollArea.Offset(rcScroll.Width() - m_nWidth, 0);
	}

	if( msg == WM_LBUTTONDOWN ||
		msg == WM_RBUTTONDOWN ||
		msg == WM_LBUTTONDBLCLK ||
		msg == WM_MOUSEMOVE ||
		msg == WM_LBUTTONUP )
	{
		POINT ptPos = GetPos();
		int x = GET_X_LPARAM(lParam) - m_nOffX - ptPos.x;
		int y = GET_Y_LPARAM(lParam) - m_nOffY - ptPos.y;

		if( WM_LBUTTONUP == msg )
		{
			m_bDragging = false;
			m_pParent->SetCaptureObject(NULL);
			m_nState = AUITEXTAREA_STATE_NORMAL;
		}
		// Click on scroll bar.
		else if( m_bDragging || rcScrollArea.PtInRect(x, y) )
		{
			int nBarH = m_pA2DSprite[AUITEXTAREA_RECT_BAR]->GetItem(0)->GetRect().Height() * fWindowScale;
			if( msg == WM_MOUSEMOVE )
			{
				if( wParam & MK_LBUTTON )
				{
					if( AUITEXTAREA_STATE_PRESSBAR == m_nState && nLines > nLinesPP )
					{
						int L = nLines - nLinesPP;
						int H = rcScrollArea.Height() - nBarH;

						if( H > 0 )
						{
							m_nFirstLine = (y - rcScrollArea.top - m_nDragDist) * (L + 1) / H;
							a_Clamp(m_nFirstLine, 0, nLines - nLinesPP);
						}
					}
				}
				else
				{
					m_bDragging = false;
					m_pParent->SetCaptureObject(NULL);
				}
			}
			else
			{
				// Page up.
				if( y < m_ptBar.y )
				{
					if( msg == WM_LBUTTONDOWN || WM_LBUTTONDBLCLK == msg )
						return OnDlgItemMessage(WM_KEYDOWN, VK_PRIOR, 0);
				}
				// Page down.
				else if( y > m_ptBar.y + nBarH )
				{
					if( msg == WM_LBUTTONDOWN || WM_LBUTTONDBLCLK == msg )
						return OnDlgItemMessage(WM_KEYDOWN, VK_NEXT, 0);
				}
				// Scroll bar.
				else if( nLines > nLinesPP )
				{
					if( msg == WM_LBUTTONDOWN )
					{
						m_bDragging = true;
						m_nDragDist = y - m_ptBar.y;
						m_pParent->SetCaptureObject(this);
						m_nState = AUITEXTAREA_STATE_PRESSBAR;
					}
				}
			}
		}
		else if( x >= rcScrollArea.left && x <= rcScrollArea.right )
		{
			// Click on up button.
			if( y < rcScrollArea.top )
			{
				if( msg == WM_LBUTTONDOWN || WM_LBUTTONDBLCLK == msg )
				{
					m_nState = AUITEXTAREA_STATE_PRESSUP;
					m_nScrollDelay = -1;
				}
			}
			// Click on down button.
			else if( y > rcScrollArea.bottom )
			{
				if( msg == WM_LBUTTONDOWN || WM_LBUTTONDBLCLK == msg )
				{
					m_nState = AUITEXTAREA_STATE_PRESSDOWN;
					m_nScrollDelay = -1;
				}
			}
		}

		return true;
	}
	else if( msg == WM_KEYDOWN )
	{
		int nVKey = int(wParam);
		if( VK_UP == nVKey )
		{
			if( m_nFirstLine > 0 )
				m_nFirstLine--;
			return true;
		}
		else if( VK_DOWN == nVKey )
		{
			if( m_nFirstLine < nLines - nLinesPP )
				m_nFirstLine++;
			return true;
		}
		else if( VK_PRIOR == nVKey )
		{
			m_nFirstLine -= nLinesPP;
			if( m_nFirstLine < 0 )
				m_nFirstLine = 0;
			return true;
		}
		else if( VK_NEXT == nVKey )
		{
			if( nLines - m_nFirstLine >= 2 * nLinesPP )
				m_nFirstLine += nLinesPP;
			else if( nLines - m_nFirstLine >= nLinesPP )
				m_nFirstLine = nLines - nLinesPP;
			return true;
		}
	}
	else if( WM_KILLFOCUS == msg )
	{
		m_bDragging = false;
		m_nState = AUITEXTAREA_STATE_NORMAL;
	}
	else if( WM_MOUSEWHEEL == msg )
	{
		int zDelta = (short)HIWORD(wParam);
		if( zDelta > 0 && m_nFirstLine > 0 )
			m_nFirstLine--;
		else if( zDelta < 0 && m_nFirstLine + nLinesPP < nLines )
			m_nFirstLine++;

		return true;
	}

	return AUIObject::OnDlgItemMessage(msg, wParam, lParam);
}

bool AUITextArea::Tick(void)
{
	if( !m_pAUIFrame || !m_pA2DSprite[AUITEXTAREA_RECT_SCROLL] )
		return true;

	int nLines = m_nLines;
	int nLinesPP = m_nLinesPP;

	DWORD dwTick = GetTickCount();
	if( (int)dwTick - m_nScrollDelay >= AUITEXTAREA_SCROLL_DELAY )
	{
		if( AUITEXTAREA_STATE_PRESSUP == m_nState )
		{
			if( m_nFirstLine > 0 )
				m_nFirstLine--;
			if( m_nScrollDelay < 0 )
				m_nScrollDelay = dwTick + 500;
			else
				m_nScrollDelay = dwTick;
		}
		else if( AUITEXTAREA_STATE_PRESSDOWN == m_nState )
		{
			if( m_nFirstLine + nLinesPP < nLines )
				m_nFirstLine++;
			if( m_nScrollDelay < 0 )
				m_nScrollDelay = dwTick + 500;
			else
				m_nScrollDelay = dwTick;
		}
	}

	return AUIObject::Tick();
}

bool AUITextArea::Render(void)
{
	if( m_pParent->GetRenderState() == AUIDIALOG_RENDERSTATE_NORMAL ||
		m_pParent->GetRenderState() == AUIDIALOG_RENDERSTATE_STATIC && !NeedDynamicRender() ||
		m_pParent->GetRenderState() == AUIDIALOG_RENDERSTATE_DYNAMIC && NeedDynamicRender() )
	{
		float fWindowScale = m_pParent->GetAUIManager()->GetWindowScale() * m_pParent->GetScale();
		int nAlpha = int(m_pParent->GetWholeAlpha() * m_nAlpha / 255);

		bool bval = true;
		POINT ptPos = GetPos();
		int cx = 0, cy = 0;
		if( m_pAUIFrame )
		{
			SIZE sLimit = m_pAUIFrame->GetSizeLimit();
			cx = sLimit.cx / 2;
			cy = sLimit.cy / 2;
		}

		ptPos.x += m_nOffX;
		ptPos.y += m_nOffY;

		A3DRECT rcText(0, 0, 0, 0);
		A3DRECT rcScroll(0, 0, 0, 0);
		if( m_pA2DSprite[AUITEXTAREA_RECT_SCROLL] )
		{
			rcScroll = m_pA2DSprite[AUITEXTAREA_RECT_SCROLL]->GetItem(0)->GetRect();
			rcText.SetRect(ptPos.x + cx, ptPos.y + cy, ptPos.x + m_nWidth
				- cx - rcScroll.Width(), ptPos.y + m_nHeight - cy);
			if( m_bLeftScrollBar ) rcText.Offset(rcScroll.Width(), 0);
		}
		else
			rcText.SetRect(ptPos.x + cx, ptPos.y + cy, ptPos.x + m_nWidth - cx, ptPos.y + m_nHeight - cy);

		if( m_pvecImageList )
		{
			bval = m_pA3DEngine->FlushInternalSpriteBuffer();
			if( !bval ) return AUI_ReportError(__LINE__, 1, "AUITextArea::Render(), failed to call m_pA3DEngine->FlushInternalSpriteBuffer()");

			m_pParent->GetAUIManager()->GetA3DFTFontMan()->Flush();
		}

		// Only render when frames is OK.
		if( !m_bOnlyShowText )
		{
			if( m_pAUIFrame )
			{
				int x = ptPos.x;
				if( m_bLeftScrollBar )
					x = ptPos.x + rcScroll.Width();

				bval = m_pAUIFrame->Render(x, ptPos.y, m_nWidth - rcScroll.Width(), m_nHeight,
					NULL, 0, _AL(""), AUIFRAME_ALIGN_LEFT, 0, 0, nAlpha, m_pParent->IsNoFlush(), m_nShadow);
			}
			else
				bval = AUIObject::Render();
			if( !bval ) return AUI_ReportError(__LINE__, 1, "AUITextArea::Render(), failed to call m_pAUIFrame->Render() or base AUIObject::Render()");
		}

		UpdateAppend();
		CalcScrollLine(rcText);

		A3DFTFont *pFont = m_pFont ? m_pFont : m_pParent->GetAUIManager()->GetDefaultFont();

		int height_off = 0;
		int nCurLine = m_nFirstLine;
		size_t i;

		for (i = 0; i < m_LinesInfo.size(); i++)
		{
			LineInfo* pLine = m_LinesInfo[i];

			if (nCurLine < pLine->lines)
			{
				height_off = -pLine->lines_off[nCurLine];
				break;
			}

			nCurLine -= pLine->lines;
		}

		m_vecNameLink.clear();
		m_EditBoxItems.clear();
		m_nLinesThisPage = 0;

		for (; i < m_LinesInfo.size(); i++)
		{
			A3DPOINT2 ptOff(rcText.left, rcText.top);
			ptOff.y += height_off;

			if (ptOff.y > rcText.bottom)
				break;

			LineInfo* pLine = m_LinesInfo[i];
			m_nLinesThisPage += pLine->Render(ptOff, nCurLine, &rcText, nAlpha);
			height_off += pLine->height;
			nCurLine = 0;
		}
		/*
		while( m_bScrollToBottom )
		{
			bval = AUI_TextOutFormat(pFont, rcText.left, rcText.top, m_szText, &rcText,
				m_nFirstLine, &m_nLines, m_nLineSpace, false, &m_vecNameLink,
				m_pA2DSpriteEmotion, &m_vecEmotion, &m_nLinesThisPage, false, m_nShadow, nAlpha);
			if( !bval ) return AUI_ReportError(__LINE__, 1, "AUITextArea::Render(), failed to call AUI_TextOutFormat()");
			
			if( m_nFirstLine + m_nLinesThisPage < m_nLines )
				m_nFirstLine += max(1, (m_nLines - m_nFirstLine - m_nLinesThisPage) / 2);
			else
				m_bScrollToBottom = false;
		}

		bval = AUI_TextOutFormat(pFont, rcText.left, rcText.top, m_szText, &rcText,
			m_nFirstLine, &m_nLines, m_nLineSpace, false, &m_vecNameLink,
			m_pA2DSpriteEmotion, &m_vecEmotion, &m_nLinesThisPage, true, m_nShadow, nAlpha);
		if( !bval ) return AUI_ReportError(__LINE__, 1, "AUITextArea::Render(), failed to call AUI_TextOutFormat()");
		*/

		if( m_bForceRenderScroll || m_nLines > m_nLinesPP )
			bval = RenderScroll();

		if( !bval ) return AUI_ReportError(__LINE__, 1, "AUITextArea::Render(), failed to call RenderScroll()");
	}

	return true;
}

bool AUITextArea::RenderScroll()
{
	if( !m_pA2DSprite[AUITEXTAREA_RECT_UP] ||
		!m_pA2DSprite[AUITEXTAREA_RECT_DOWN] ||
		!m_pA2DSprite[AUITEXTAREA_RECT_SCROLL] )
	{
		return true;
	}

	int nAlpha = int(m_pParent->GetWholeAlpha() * m_nAlpha / 255);
	bool bval;
	int nLines = m_nLines;
	POINT ptPos = GetPos();
	A2DSprite *pA2DSpriteFrame = NULL;
	int cx = 0, cy = 0;
	if( m_pAUIFrame )
	{
		SIZE sLimit = m_pAUIFrame->GetSizeLimit();
		cx = sLimit.cx / 2;
		cy = sLimit.cy / 2;
	}
	A3DRECT rcUp = m_pA2DSprite[AUITEXTAREA_RECT_UP]->GetItem(0)->GetRect();
	A3DRECT rcDown = m_pA2DSprite[AUITEXTAREA_RECT_DOWN]->GetItem(0)->GetRect();
	A3DRECT rcScroll = m_pA2DSprite[AUITEXTAREA_RECT_SCROLL]->GetItem(0)->GetRect();
	float fWindowScale = m_pParent->GetAUIManager()->GetWindowScale() * m_pParent->GetScale();
	rcUp.SetRect(rcUp.left * fWindowScale, rcUp.top * fWindowScale,
		rcUp.right * fWindowScale, rcUp.bottom * fWindowScale);
	rcDown.SetRect(rcDown.left * fWindowScale, rcDown.top * fWindowScale,
		rcDown.right * fWindowScale, rcDown.bottom * fWindowScale);
	rcScroll.SetRect(rcScroll.left * fWindowScale, rcScroll.top * fWindowScale,
		rcScroll.right * fWindowScale, rcScroll.bottom * fWindowScale);
	A3DRECT rcText(ptPos.x + cx, ptPos.y + cy, ptPos.x + m_nWidth
		- cx - rcScroll.Width(), ptPos.y + m_nHeight - cy);
	if( m_bLeftScrollBar ) rcText.Offset(rcScroll.Width(), 0);

	ptPos.x += m_nOffX;
	ptPos.y += m_nOffY;

	A3DPOINT2 ptSize;
	int nTextH;
	A3DFTFont *pFont = m_pFont ? m_pFont : m_pParent->GetAUIManager()->GetDefaultFont();
	ptSize = pFont->GetTextExtent(_AL("Wjg"));
	nTextH = ptSize.y;

	if( m_pA2DSpriteEmotion && m_pA2DSpriteEmotion[0] )
		nTextH = max(m_pA2DSpriteEmotion[0]->GetItem(0)->GetRect().Height() * fWindowScale, nTextH);
	nTextH += m_nLineSpace * fWindowScale;
	if( m_pA2DSpriteEmotion )
		m_nLinesPP = m_nLinesThisPage;
	else
		m_nLinesPP = int((rcText.Height() + m_nLineSpace * fWindowScale) / nTextH);

	if( m_bOnlyShowText ) return true;

	// Draw up button.
	int nItem = (AUITEXTAREA_STATE_PRESSUP == m_nState) ? 1 : 0;
	bval = m_pA2DSprite[AUITEXTAREA_RECT_UP]->SetCurrentItem(nItem);
	if( !bval ) return AUI_ReportError(__LINE__, 1, "AUITextArea::RenderScroll(), failed to call m_pA2DSprite[AUITEXTAREA_RECT_UP]->SetCurrentItem()");

	int W = rcScroll.Width();
	int x, y = ptPos.y;
	if( m_bLeftScrollBar )
		x = ptPos.x;
	else
		x = ptPos.x + m_nWidth - W;
	m_pA2DSprite[AUITEXTAREA_RECT_UP]->SetScaleX(fWindowScale);
	m_pA2DSprite[AUITEXTAREA_RECT_UP]->SetScaleY(fWindowScale);
	m_pA2DSprite[AUITEXTAREA_RECT_UP]->SetAlpha(nAlpha);
	if( m_pParent->IsNoFlush() )
		bval = m_pA2DSprite[AUITEXTAREA_RECT_UP]->DrawToBack(x, y);
	else
		bval = m_pA2DSprite[AUITEXTAREA_RECT_UP]->DrawToInternalBuffer(x, y);
	if( !bval ) return AUI_ReportError(__LINE__, 1, "AUITextArea::RenderScroll(), failed to call m_pA2DSprite[AUITEXTAREA_RECT_UP]->DrawToBack() or m_pA2DSprite[AUITEXTAREA_RECT_UP]->DrawToInternalBuffer()");

	// Draw scroll area.
	int H = rcUp.Height();
	y += H;

	float fScale = float(m_nHeight - rcUp.Height() - rcDown.Height())
		/ m_pA2DSprite[AUITEXTAREA_RECT_SCROLL]->GetItem(0)->GetRect().Height();
	m_pA2DSprite[AUITEXTAREA_RECT_SCROLL]->SetScaleX(fWindowScale);
	m_pA2DSprite[AUITEXTAREA_RECT_SCROLL]->SetScaleY(fScale);
	m_pA2DSprite[AUITEXTAREA_RECT_SCROLL]->SetAlpha(nAlpha);
	if( m_pParent->IsNoFlush() )
		bval = m_pA2DSprite[AUITEXTAREA_RECT_SCROLL]->DrawToBack(x, y);
	else
		bval = m_pA2DSprite[AUITEXTAREA_RECT_SCROLL]->DrawToInternalBuffer(x, y);
	if( !bval ) return AUI_ReportError(__LINE__, 1, "AUITextArea::RenderScroll(), failed to call m_pA2DSprite[AUITEXTAREA_RECT_SCROLL]->DrawToBack() or m_pA2DSprite[AUITEXTAREA_RECT_SCROLL]->DrawToInternalBuffer()");

	// Draw down button.
	nItem = (AUITEXTAREA_STATE_PRESSDOWN == m_nState) ? 1 : 0;
	bval = m_pA2DSprite[AUITEXTAREA_RECT_DOWN]->SetCurrentItem(nItem);
	if( !bval ) return AUI_ReportError(__LINE__, 1, "AUITextArea::RenderScroll(), failed to call m_pA2DSprite[AUITEXTAREA_RECT_DOWN]->SetCurrentItem()");

	y = ptPos.y + m_nHeight - rcDown.Height();

	m_pA2DSprite[AUITEXTAREA_RECT_DOWN]->SetScaleX(fWindowScale);
	m_pA2DSprite[AUITEXTAREA_RECT_DOWN]->SetScaleY(fWindowScale);
	m_pA2DSprite[AUITEXTAREA_RECT_DOWN]->SetAlpha(nAlpha);
	if( m_pParent->IsNoFlush() )
		bval = m_pA2DSprite[AUITEXTAREA_RECT_DOWN]->DrawToBack(x, y);
	else
		bval = m_pA2DSprite[AUITEXTAREA_RECT_DOWN]->DrawToInternalBuffer(x, y);
	if( !bval ) return AUI_ReportError(__LINE__, 1, "AUITextArea::RenderScroll(), failed to call m_pA2DSprite[AUITEXTAREA_RECT_DOWN]->DrawToBack() or m_pA2DSprite[AUITEXTAREA_RECT_DOWN]->DrawToInternalBuffer()");

	// Draw scrollbar button if necessary.
	if( nLines > m_nLinesPP )
	{
		int nBarH = m_pA2DSprite[AUITEXTAREA_RECT_BAR]->GetItem(0)->GetRect().Height() * fWindowScale;
		int H = m_nHeight - rcUp.Height() - rcDown.Height() - nBarH;
		if( H > 0 )
		{
			int L = nLines - m_nLinesPP;
			if( m_bLeftScrollBar )
				m_ptBar.x = 0;
			else
				m_ptBar.x = m_nWidth - rcScroll.Width();
			m_ptBar.y = rcUp.Height() + m_nFirstLine * H / L;
			m_pA2DSprite[AUITEXTAREA_RECT_BAR]->SetScaleX(fWindowScale);
			m_pA2DSprite[AUITEXTAREA_RECT_BAR]->SetScaleY(fWindowScale);
			m_pA2DSprite[AUITEXTAREA_RECT_BAR]->SetAlpha(nAlpha);
			if( m_pParent->IsNoFlush() )
				bval = m_pA2DSprite[AUITEXTAREA_RECT_BAR]->DrawToBack(
				ptPos.x + m_ptBar.x, ptPos.y + m_ptBar.y);
			else
				bval = m_pA2DSprite[AUITEXTAREA_RECT_BAR]->DrawToInternalBuffer(
				ptPos.x + m_ptBar.x, ptPos.y + m_ptBar.y);
			if( !bval ) return AUI_ReportError(__LINE__, 1, "AUITextArea::RenderScroll(), failed to call m_pA2DSprite[AUITEXTAREA_RECT_BAR]->DrawToBack() or m_pA2DSprite[AUITEXTAREA_RECT_BAR]->DrawToInternalBuffer()");
		}
	}

	return true;
}

bool AUITextArea::GetProperty(const char *pszPropertyName, PAUIOBJECT_SETPROPERTY Property)
{
	memset(Property, 0, sizeof(AUIOBJECT_SETPROPERTY));

	if( 0 == strcmpi(pszPropertyName, "Frame mode") )
		Property->i = m_nFrameMode;
	else if( 0 == strcmpi(pszPropertyName, "Frame Image") )
	{
		if( m_pAUIFrame )
			strncpy(Property->fn, m_pAUIFrame->GetA2DSprite()->GetName(), AFILE_LINEMAXLEN - 1);
	}
	else if( 0 == strcmpi(pszPropertyName, "Up Button Image") )
	{
		if( m_pA2DSprite[AUITEXTAREA_RECT_UP] )
			strncpy(Property->fn, m_pA2DSprite[AUITEXTAREA_RECT_UP]->GetName(), AFILE_LINEMAXLEN - 1);
	}
	else if( 0 == strcmpi(pszPropertyName, "Down Button Image") )
	{
		if( m_pA2DSprite[AUITEXTAREA_RECT_DOWN] )
			strncpy(Property->fn, m_pA2DSprite[AUITEXTAREA_RECT_DOWN]->GetName(), AFILE_LINEMAXLEN - 1);
	}
	else if( 0 == strcmpi(pszPropertyName, "Scroll Area Image") )
	{
		if( m_pA2DSprite[AUITEXTAREA_RECT_SCROLL] )
			strncpy(Property->fn, m_pA2DSprite[AUITEXTAREA_RECT_SCROLL]->GetName(), AFILE_LINEMAXLEN - 1);
	}
	else if( 0 == strcmpi(pszPropertyName, "Scroll Bar Image") )
	{
		if( m_pA2DSprite[AUITEXTAREA_RECT_BAR] )
			strncpy(Property->fn, m_pA2DSprite[AUITEXTAREA_RECT_BAR]->GetName(), AFILE_LINEMAXLEN - 1);
	}
	else if( 0 == strcmpi(pszPropertyName, "Left Scroll Bar") )
		Property->b = m_bLeftScrollBar;
	else if( 0 == strcmpi(pszPropertyName, "Text Transparent") )
		Property->b = m_bTextTransparent;
	else if( 0 == strcmpi(pszPropertyName, "Trans Scroll Wheel") )
		Property->b = m_bTransScrollWheel;
	else if( 0 == strcmpi(pszPropertyName, "Link Underline Mode") )
		Property->i = m_ItemUnderLineMode;
	else if( 0 == strcmpi(pszPropertyName, "Link Underline Color") )
		Property->dw = m_clItemUnderLine;
	else
		return AUIObject::GetProperty(pszPropertyName, Property);

	return true;
}

bool AUITextArea::SetProperty(const char *pszPropertyName, PAUIOBJECT_SETPROPERTY Property)
{
	if( 0 == strcmpi(pszPropertyName, "Width") )
	{
		AUIObject::SetProperty(pszPropertyName, Property);
		if( m_pAUIFrame )
		{
			SIZE s = m_pAUIFrame->GetSizeLimit();
			m_nWidth = max(m_nWidth, s.cx);
		}
	}
	else if( 0 == strcmpi(pszPropertyName, "Height") )
	{
		AUIObject::SetProperty(pszPropertyName, Property);
		if( m_pAUIFrame )
		{
			SIZE s = m_pAUIFrame->GetSizeLimit();
			m_nHeight = max(m_nHeight, s.cy);
		}
	}
	else if( 0 == strcmpi(pszPropertyName, "Frame mode") )
	{
		m_nFrameMode = Property->i;
		if( m_pAUIFrame )
			m_pAUIFrame->SetMode(m_nFrameMode);
	}
	else if( 0 == strcmpi(pszPropertyName, "Frame Image") )
		return InitIndirect(Property->fn, AUITEXTAREA_RECT_FRAME);
	else if( 0 == strcmpi(pszPropertyName, "Up Button Image") )
		return InitIndirect(Property->fn, AUITEXTAREA_RECT_UP);
	else if( 0 == strcmpi(pszPropertyName, "Down Button Image") )
		return InitIndirect(Property->fn, AUITEXTAREA_RECT_DOWN);
	else if( 0 == strcmpi(pszPropertyName, "Scroll Area Image") )
		return InitIndirect(Property->fn, AUITEXTAREA_RECT_SCROLL);
	else if( 0 == strcmpi(pszPropertyName, "Scroll Bar Image") )
		return InitIndirect(Property->fn, AUITEXTAREA_RECT_BAR);
	else if( 0 == strcmpi(pszPropertyName, "Text Transparent") )
		m_bTextTransparent = Property->b;
	else if( 0 == strcmpi(pszPropertyName, "Trans Scroll Wheel") )
		m_bTransScrollWheel = Property->b;
	else if( 0 == strcmpi(pszPropertyName, "Link Underline Mode") )
		m_ItemUnderLineMode = Property->i;
	else if( 0 == strcmpi(pszPropertyName, "Link Underline Color") )
		m_clItemUnderLine = Property->dw;
	else if( 0 == strcmpi(pszPropertyName, "Left Scroll Bar") )
		SetLeftScrollBar(Property->b);
	else
		return AUIObject::SetProperty(pszPropertyName, Property);

	return true;
}

bool AUITextArea::RegisterPropertyType()
{
	return	SetAUIObjectPropertyType("Up Frame File", AUIOBJECT_PROPERTY_VALUE_CHAR) &&
			SetAUIObjectPropertyType("Down Frame File", AUIOBJECT_PROPERTY_VALUE_CHAR) &&
			SetAUIObjectPropertyType("Frame Image", AUIOBJECT_PROPERTY_VALUE_CHAR) &&
			SetAUIObjectPropertyType("Scroll Area Image", AUIOBJECT_PROPERTY_VALUE_CHAR) &&
			SetAUIObjectPropertyType("Scroll Bar Image", AUIOBJECT_PROPERTY_VALUE_CHAR) &&
			SetAUIObjectPropertyType("Frame mode", AUIOBJECT_PROPERTY_VALUE_INT);
}

void AUITextArea::ScrollToTop()
{
	if( m_nFirstLine != 0 )
		UpdateRenderTarget();

	m_nFirstLine = 0;
}

void AUITextArea::ScrollToBottom()
{
	if( !m_bScrollToBottom )
		UpdateRenderTarget();

	m_bScrollToBottom = true;
}

void AUITextArea::SetOnlyShowText(bool bShow)
{
	if( bShow != m_bOnlyShowText )
		UpdateRenderTarget();

	m_bOnlyShowText = bShow;
}

void AUITextArea::SetForceRenderScroll(bool bForce)
{
	if( bForce != m_bForceRenderScroll )
		UpdateRenderTarget();

	m_bForceRenderScroll = bForce;
}

void AUITextArea::SetLeftScrollBar(bool bLeft)
{
	if( bLeft != m_bLeftScrollBar )
		UpdateRenderTarget();

	m_bLeftScrollBar = bLeft;
}

void AUITextArea::SetFirstLine(int nFirstLine)
{
	if( nFirstLine != m_nFirstLine )
		UpdateRenderTarget();

	m_nFirstLine = nFirstLine;
	a_ClampRoof(m_nFirstLine, m_nLines - m_nLinesPP);
	a_ClampFloor(m_nFirstLine, 0);
}

abase::vector<AUITEXTAREA_NAME_LINK> & AUITextArea::GetNameLink()
{
	return m_vecNameLink;
}

void AUITextArea::SetEmotionList(A2DSprite **pA2DSpriteEmotion,
	abase::vector<AUITEXTAREA_EMOTION> **pvecEmotion)
{
	m_pA2DSpriteEmotion = pA2DSpriteEmotion;
	m_vecEmotion = pvecEmotion;
}

void AUITextArea::SetImageList(abase::vector<A2DSprite*> *pvecImageList)
{
	m_pvecImageList = pvecImageList;
}

void AUITextArea::SetOtherFonts(abase::vector<FONT_WITH_SHADOW>* pvecFonts)
{
	m_pvecOtherFonts = pvecFonts;
}

int AUITextArea::GetHitArea(int x, int y)
{
	if( !m_pAUIFrame ||
		!m_pA2DSprite[AUITEXTAREA_RECT_UP] ||
		!m_pA2DSprite[AUITEXTAREA_RECT_DOWN] ||
		!m_pA2DSprite[AUITEXTAREA_RECT_SCROLL] )
	{
		return AUITEXTAREA_RECT_FRAME;
	}

	int nLines = m_nLines;
	int nLinesPP = m_nLinesPP;
	SIZE sLimit = {0, 0};
	if( m_pAUIFrame )
		sLimit = m_pAUIFrame->GetSizeLimit();
	int cx = sLimit.cx / 2;
	int cy = sLimit.cy / 2;
	A3DRECT rcUp = m_pA2DSprite[AUITEXTAREA_RECT_UP]->GetItem(0)->GetRect();
	A3DRECT rcDown = m_pA2DSprite[AUITEXTAREA_RECT_DOWN]->GetItem(0)->GetRect();
	A3DRECT rcScroll = m_pA2DSprite[AUITEXTAREA_RECT_SCROLL]->GetItem(0)->GetRect();
	float fWindowScale = m_pParent->GetAUIManager()->GetWindowScale() * m_pParent->GetScale();
	rcUp.SetRect(rcUp.left * fWindowScale, rcUp.top * fWindowScale,
		rcUp.right * fWindowScale, rcUp.bottom * fWindowScale);
	rcDown.SetRect(rcDown.left * fWindowScale, rcDown.top * fWindowScale,
		rcDown.right * fWindowScale, rcDown.bottom * fWindowScale);
	rcScroll.SetRect(rcScroll.left * fWindowScale, rcScroll.top * fWindowScale,
		rcScroll.right * fWindowScale, rcScroll.bottom * fWindowScale);
	A3DRECT rcText(cx, cy, m_nWidth	- cx - rcScroll.Width(), m_nHeight - cy);
	A3DRECT rcScrollArea(m_nWidth - rcScroll.Width(), rcUp.Height(), m_nWidth, m_nHeight - rcDown.Height());

	if( m_bLeftScrollBar )
	{
		rcText.Offset(rcScroll.Width(), 0);
		rcScrollArea.Offset(rcScroll.Width() - m_nWidth, 0);
	}

	// Click on text item area.
	if( rcText.PtInRect(x, y) )
		return AUITEXTAREA_RECT_FRAME;
	// Click on scroll bar.
	else if( rcScrollArea.PtInRect(x, y) )
	{
		int nBarH = m_pA2DSprite[AUITEXTAREA_RECT_BAR]->GetItem(0)->GetRect().Height();
		// Page up and page down.
		if( y < m_ptBar.y || y > m_ptBar.y + nBarH || nLines > nLinesPP )
			return AUITEXTAREA_RECT_SCROLL;
		// Scroll bar.
		else
			return AUITEXTAREA_RECT_BAR;
	}
	// Click on up button.
	else if( y < rcScrollArea.top )
		return AUITEXTAREA_RECT_UP;
	// Click on down button.
	else if( y > rcScrollArea.bottom )
		return AUITEXTAREA_RECT_DOWN;

	return AUITEXTAREA_RECT_FRAME;
}

void AUITextArea::CalcScrollLine(const A3DRECT& rc)
{
	if (!m_bScrollToBottom)
		return;

	m_bScrollToBottom = false;

	int lines = 0;
	int height = rc.Height();
	int cur_hei = 0;

	for (int i = m_LinesInfo.size() - 1; i >= 0; i--)
	{
		LineInfo* pLine = m_LinesInfo[i];

		if (cur_hei + pLine->height <= height)
		{
			cur_hei += pLine->height;
			lines += pLine->lines;
			continue;
		}

		int dif = height - cur_hei;

		for (int j = pLine->lines - 1; j >= 0; j--)
		{
			if (pLine->height - pLine->lines_off[j] <= dif)
			{
				lines++;
				continue;
			}
			
			break;
		}

		break;
	}

	m_nFirstLine = m_nLines - lines;
}

void AUITextArea::SetText(const ACHAR *pszText)
{
	AUIObject::SetText(pszText);

	SetDynamicRender(false);
	ClearLinesInfo();
	m_ItemsSet.Release();
	ACString strConverted = UnmarshalEditBoxTextEx(pszText, m_ItemsSet, 0, L"", 0xffffffff, m_nItemMask);
	pszText = strConverted;

	m_clLast = m_color;
	m_UnderLineInfoLast.bUnderLine = false;
	m_UnderLineInfoLast.bSameColor = true;
	m_UnderLineInfoLast.color = A3DCOLORRGB(0, 0, 0);
	m_UnderLineInfoLast.fScale = 1.0f;
	m_nFontShadowLast.pFont = m_pFont ? m_pFont : m_pParent->GetAUIManager()->GetDefaultFont();
	m_nFontShadowLast.nShadow = m_nShadow;

	const ACHAR* szRemain = CalcTexFormat(pszText, m_LinesInfo);
	if (szRemain)
		m_strRemain = szRemain;
	else
		m_strRemain.Empty();

	bool bTrim = false;
	if (m_nMaxLines >= 0)
	{
		while (static_cast<int>(m_LinesInfo.size()) > m_nMaxLines)
		{
			LineInfo* pLine = m_LinesInfo.back();
			for (size_t i = 0; i < pLine->tags.size(); i++)
			{
				LineTag* pTag = pLine->tags[i];
				ACHAR c = pTag->GetItemCode();
				if( c != 0 )
					m_ItemsSet.DelItemByChar(c);
			}

			delete pLine;
			m_LinesInfo.pop_back();
			bTrim = true;
		}
	}

	if (bTrim) m_strRemain.Empty();

	CalcTagPosAndSize(m_LinesInfo);
	CalcTotalLines();

	m_nFirstLine = 0;
	m_bTextAppended = false;
}

void AUITextArea::UpdateAppend()
{
	if (!m_bTextAppended)
		return;

	m_bTextAppended = false;

	if (m_LinesInfo.size())
	{
		LineInfo* pLast = m_LinesInfo.back();

		if (!pLast->complete)
		{
			delete pLast;
			m_LinesInfo.pop_back();
		}
	}

	LinesInfo appends;
	const ACHAR* szRemain = CalcTexFormat(m_strRemain, appends);

	if (szRemain)
		m_strRemain = szRemain;
	else
		m_strRemain.Empty();

	CalcTagPosAndSize(appends);

	for (size_t i = 0; i < appends.size(); i++)
		m_LinesInfo.push_back(appends[i]);

	if (m_nMaxLines >= 0)
	{
		while (static_cast<int>(m_LinesInfo.size()) > m_nMaxLines)
		{
			LineInfo* pLine = m_LinesInfo.front();

			if (m_nFirstLine >= pLine->lines)
				m_nFirstLine -= pLine->lines;
			else
				m_nFirstLine = 0;

			for (size_t i = 0; i < pLine->tags.size(); i++)
			{
				LineTag* pTag = pLine->tags[i];
				ACHAR c = pTag->GetItemCode();
				if( c != 0 )
					m_ItemsSet.DelItemByChar(c);
			}

			delete pLine;
			m_LinesInfo.erase(m_LinesInfo.begin());
		}

		if (!m_LinesInfo.size()) m_strRemain.Empty();
	}

	CalcTotalLines();
}

#ifdef UNICODE
	#define a_str_chr(s, c) wcschr(s, c)
	#define a_str_rchr(s, c) wcsrchr(s, c)
#else
	#define a_str_chr(s, c) strchr(s, c)
	#define a_str_rchr(s, c) strrchr(s, c)
#endif

#define ADD_ONE_TEXT_TAG(s, cl, underlineinfo, font) \
{\
	if (!s.IsEmpty())\
	{\
		TextTag* pTag = new TextTag;\
		pTag->SetLineInfo(pLine);\
		pTag->SetText(s, cl, underlineinfo, font);\
		pLine->tags.push_back(static_cast<LineTag*>(pTag));\
		s.Empty();\
	}\
}


void AUITextArea::AppendText(const ACHAR *pszText, int nMsgIndex, const ACHAR* szIvtrItem, A3DCOLOR clIvtrItem, 
							 const EditboxScriptItem** ppScriptItems, int nScriptItemCount,
							 UnderLineMode ulMode, A3DCOLOR clUnderLine)
{
	UpdateRenderTarget();

	if( m_szText.GetLength() == 0 )
	{
		m_clLast = m_color;
		m_UnderLineInfoLast.bUnderLine = false;
		m_UnderLineInfoLast.bSameColor = true;
		m_UnderLineInfoLast.color = A3DCOLORRGB(0, 0, 0);
		m_UnderLineInfoLast.fScale = 1.0f;
		m_nFontShadowLast.pFont = m_pFont ? m_pFont : m_pParent->GetAUIManager()->GetDefaultFont();
		m_nFontShadowLast.nShadow = m_nShadow;
	}

	m_szText += pszText;

	bool bUnderLine = false;
	bool bSameColor = true;
	if( ulMode == UM_USE_AUIOBJECT_OPTION )
	{
		if( m_ItemUnderLineMode == UM_NO_UNDERLINE )
		{
			bUnderLine = false;
			bSameColor = true;
		}
		else if( m_ItemUnderLineMode == UM_UNDERLINE_SAME_COLOR )
		{
			bUnderLine = true;
			bSameColor = true;
		}
		else
		{
			bUnderLine = true;
			bSameColor = false;
			clUnderLine = m_clItemUnderLine;
		}
	}
	else if( ulMode == UM_NO_UNDERLINE )
	{
		bUnderLine = false;
		bSameColor = true;
	}
	else if( ulMode == UM_UNDERLINE_SAME_COLOR )
	{
		bUnderLine = true;
		bSameColor = true;
	}
	else if( ulMode == UM_UNDERLINE_SPECIFY_COLOR )
	{
		bUnderLine = true;
		bSameColor = false;
	}

	ACString strConverted = UnmarshalEditBoxText(pszText, m_ItemsSet, nMsgIndex, szIvtrItem, clIvtrItem, m_nItemMask, ppScriptItems, nScriptItemCount, bUnderLine, bSameColor, clUnderLine);
	pszText = strConverted;

	if (*pszText == _AL('\r') && m_LinesInfo.size())
	{
		LineInfo* pLine = m_LinesInfo.back();

		if (!pLine->complete)
		{
			if( pLine->tags.size() == 0 )
			{
				ACString s;
				s = _AL(" ");
				ADD_ONE_TEXT_TAG(s, m_clLast, m_UnderLineInfoLast, m_nFontShadowLast)
			}

			pLine->complete = true;
			const ACHAR* p = pszText + 1;
			if (*p == _AL('\n')) p++;
			m_strRemain = p;
			m_bTextAppended = !m_strRemain.IsEmpty();
			return;
		}
	}

	m_strRemain += pszText;
	m_bTextAppended = true;
}

const ACHAR* AUITextArea::CalcTexFormat(const ACHAR* szText, LinesInfo& infos)
{
	const ACHAR* p = szText;
	const ACHAR* t = a_str_chr(szText, _AL('\r'));
	float fWindowScale = m_pParent->GetAUIManager()->GetWindowScale() * m_pParent->GetScale();

	while (t)
	{
		LineInfo* pLine = new LineInfo;
		pLine->complete = true;
		pLine->line_space = m_nLineSpace * fWindowScale;
		infos.push_back(pLine);

		CalcLineFormat(pLine, p, t-p, m_clLast, m_UnderLineInfoLast, m_nFontShadowLast);
		p = t + 1;
		if (*p == _AL('\n')) p++;
		t = a_str_chr(p, _AL('\r'));

		if( pLine->tags.size() == 0 )
		{
			ACString s;
			s = _AL(" ");
			ADD_ONE_TEXT_TAG(s, m_clLast, m_UnderLineInfoLast, m_nFontShadowLast)
		}
	}

	if (*p != 0)
	{
		LineInfo* pLine = new LineInfo;
		pLine->complete = false;
		pLine->line_space = m_nLineSpace * fWindowScale;
		infos.push_back(pLine);
		CalcLineFormat(pLine, p, -1, m_clLast, m_UnderLineInfoLast, m_nFontShadowLast);
		return p;
	}
	else
		return NULL;
}

void AUITextArea::CalcLineFormat(LineInfo* pLine, const ACHAR* szLine, int count, A3DCOLOR& cl, UnderLineInfo& underlineinfo, FONT_WITH_SHADOW& fontshadow)
{
	int i = 0;
	ACString s;
	ACHAR c;
	const ACHAR* start = szLine;
	int emotion_w, emotion_h;

	float fWindowScale = m_pParent->GetAUIManager()->GetWindowScale() * m_pParent->GetScale();
	if (m_pA2DSpriteEmotion && m_pA2DSpriteEmotion[0] )
	{
		A3DRECT& rc = m_pA2DSpriteEmotion[0]->GetItem(0)->GetRect();
		emotion_w = rc.Width() * fWindowScale;
		emotion_h = rc.Height() * fWindowScale;
	}
	else
	{
		emotion_w = 0;
		emotion_h = 0;
	}

	while (count == -1 || i < count)
	{
		c = szLine[i];

		if (c == 0) break;
		else if (IsEditboxItemCode(c))
		{
			s += ACString(start, szLine + i - start);
			ADD_ONE_TEXT_TAG(s, cl, underlineinfo, fontshadow)
			start = szLine + ++i;

			EditBoxItemBase* pItem = m_ItemsSet.GetItemByChar(c);

			if (pItem)
			{
				if (pItem->GetType() == enumEIEmotion)
				{
					SetDynamicRender(true);
					EmotionTag* pTag = new EmotionTag;
					pTag->fScale = fWindowScale;
					pTag->SetLineInfo(pLine);
					pTag->SetSize(emotion_w, emotion_h + m_nLineSpace * fWindowScale);

					int nEmotion = 0, index = 0;
					UnmarshalEmotionInfo(pItem->GetInfo(), nEmotion, index);

					if ( m_vecEmotion && nEmotion < AUIMANAGER_MAX_EMOTIONGROUPS &&
						m_vecEmotion[nEmotion] && index < m_vecEmotion[nEmotion]->size())
					{
						pTag->sprite = m_pA2DSpriteEmotion[nEmotion];
						pTag->emotion = &(*m_vecEmotion[nEmotion])[index];
					}
					else
					{
						pTag->sprite = NULL;
						pTag->emotion = NULL;
					}

					pLine->tags.push_back(pTag);
					m_ItemsSet.DelItemByChar(c);
				}
				else if (pItem->GetType() == enumEIImage)
				{
					bool b = false;
					if( m_pvecImageList && pItem->GetImageIndex() < m_pvecImageList->size() )
					{
						A2DSprite *pSprite = (*m_pvecImageList)[pItem->GetImageIndex()];
						if( pSprite && pItem->GetImageFrame() < pSprite->GetItemNum() )
						{
							A2DSpriteItem *pSpriteItem = pSprite->GetItem(pItem->GetImageFrame());
							if( pSpriteItem )
							{
								ImageTag* pTag = new ImageTag;
								pTag->fScale = fWindowScale;
								pTag->SetLineInfo(pLine);
								pTag->SetSize(pSpriteItem->GetRect().Width() * fWindowScale, 
									pSpriteItem->GetRect().Height() * fWindowScale + m_nLineSpace * fWindowScale);

								pTag->sprite = pSprite;
								pTag->iItem = pItem->GetImageFrame();
								pTag->m_pItem = pItem;
								pTag->items = &m_EditBoxItems;
								pTag->itemCode = c;

								pLine->tags.push_back(pTag);
								b = true;
							}
						}
					}
					if( !b )
						m_ItemsSet.DelItemByChar(c);
				}
				else
				{
					EditBoxItemTag* pTag = new EditBoxItemTag;
					pTag->SetLineInfo(pLine);
					UnderLineInfo itemunderlineinfo;
					itemunderlineinfo.bUnderLine = pItem->GetUnderLine();
					itemunderlineinfo.bSameColor = pItem->GetSameColor();
					itemunderlineinfo.color = pItem->GetUnderLineColor();
					itemunderlineinfo.fScale = fWindowScale;
					pTag->SetText(pItem->GetName(), pItem->GetColor(), itemunderlineinfo, fontshadow);
					pTag->m_pItem = pItem;
					pTag->items = &m_EditBoxItems;
					pTag->itemCode = c;
					pLine->tags.push_back(pTag);
				}
			}
		}
		else if (c == _AL('\n'))
		{
			s += ACString(start, szLine + i - start);
			start = szLine + ++i;
		}
		else if (c == _AL('^'))
		{
			s += ACString(start, szLine + i - start);
			c = szLine[++i];

			if (c == _AL('^') || c == _AL('&'))
			{
				s += c;
				start = szLine + ++i;
			}
			else if( c == 'O' )
			{
				ADD_ONE_TEXT_TAG(s, cl, underlineinfo, fontshadow)
				int fontindex = 0;
				i++;

				for (int j = 0; j < 3; j++)
				{
					c = szLine[i];

					if (c == 0)
						break;

					if (c >= _AL('0') && c <= _AL('9'))
						c -= _AL('0');
					else
						c = 0;

					fontindex = fontindex * 10 + c;
					i++;
				}

				if( fontindex > 0 && m_pvecOtherFonts && fontindex <= m_pvecOtherFonts->size() && (*m_pvecOtherFonts)[fontindex - 1].pFont )
				{
					fontshadow.pFont = (*m_pvecOtherFonts)[fontindex - 1].pFont;
					fontshadow.nShadow = (*m_pvecOtherFonts)[fontindex - 1].nShadow;
				}
				else
				{
					fontshadow.pFont = m_pFont ? m_pFont : m_pParent->GetAUIManager()->GetDefaultFont();
					fontshadow.nShadow = m_nShadow;
				}
				start = szLine + i;
				continue;
			}
			else if( c == 'U' )
			{
				ADD_ONE_TEXT_TAG(s, cl, underlineinfo, fontshadow)
				underlineinfo.bUnderLine = !underlineinfo.bUnderLine;
				underlineinfo.bSameColor = false;
				underlineinfo.color = 0;
				underlineinfo.fScale = fWindowScale;
				i++;

				for (int j = 0; j < 6; j++)
				{
					c = szLine[i];

					if (c == 0)
						break;

					if (c >= _AL('0') && c <= _AL('9'))
						c -= _AL('0');
					else if (c >= _AL('a') && c <= _AL('f'))
						c += 0xA - _AL('a');
					else if (c >= 'A' && c <= 'F')
						c += 0xA - _AL('A');
					else
						c = 0;

					underlineinfo.color = (underlineinfo.color << 4) + c;
					i++;
				}

				underlineinfo.color |= 0xFF000000;
				start = szLine + i;
				continue;
			}
			else if( c == 'u' )
			{
				ADD_ONE_TEXT_TAG(s, cl, underlineinfo, fontshadow)
				underlineinfo.bUnderLine = !underlineinfo.bUnderLine;
				underlineinfo.bSameColor = true;
				underlineinfo.fScale = fWindowScale;
				i++;
				start = szLine + i;
				continue;
			}
			else if( c == 'N')
			{
				cl = m_color;
				i++;
				start = szLine + i;
				continue;
			}
			else // color
			{
				ADD_ONE_TEXT_TAG(s, cl, underlineinfo, fontshadow)
				cl = 0;

				for (int j = 0; j < 6; j++)
				{
					c = szLine[i];

					if (c == 0)
						break;

					if (c >= _AL('0') && c <= _AL('9'))
						c -= _AL('0');
					else if (c >= _AL('a') && c <= _AL('f'))
						c += 0xA - _AL('a');
					else if (c >= 'A' && c <= 'F')
						c += 0xA - _AL('A');
					else
						c = 0;

					cl = (cl << 4) + c;
					i++;
				}

				cl |= 0xFF000000;
				start = szLine + i;
				continue;
			}
		}
		else if (c == _AL('&'))
		{
			s += ACString(start, szLine + i - start);
			ADD_ONE_TEXT_TAG(s, cl, underlineinfo, fontshadow)
			start = szLine + ++i;

			while (c = szLine[i])
			{
				if (c == _AL('&'))
				{
					NameLinkTag* pTag = new NameLinkTag;
					pTag->SetLineInfo(pLine);
					if( *start == '^' )
					{
						int offset = 2;
						if (*(start+1) == '^')
						{
							if (AUI_ConvertColor(start+1, pTag->clCustom))
							{
								offset += 6;
								pTag->cType = 3;
							}
							else
								pTag->cType = -1;							
						}
						else
						{
							pTag->cType = *(start + 1) - '0';
						}
						pTag->SetText(ACString(start + offset, szLine + i - start - offset), cl, underlineinfo, fontshadow);
					}
					else
					{
						pTag->cType = -1;
						pTag->SetText(ACString(start, szLine + i - start), cl, underlineinfo, fontshadow);
					}
					pTag->links = &m_vecNameLink;
					pLine->tags.push_back(pTag);

					s.Empty();
					start = szLine + ++i;
					break;
				}

				i++;
			}
		}
		else
			i++;
	}

	s += ACString(start, szLine + i - start);
	ADD_ONE_TEXT_TAG(s, cl, underlineinfo, fontshadow)
}

void AUITextArea::CalcTagPosAndSize(LinesInfo& infos)
{
	int max_width;

	if (m_pA2DSprite[AUITEXTAREA_RECT_SCROLL])
	{
		if( m_pAUIFrame )
			max_width = m_nWidth - m_pAUIFrame->GetSizeLimit().cx - m_pA2DSprite[AUITEXTAREA_RECT_SCROLL]->GetItem(0)->GetRect().Width();
		else
			max_width = m_nWidth - m_pA2DSprite[AUITEXTAREA_RECT_SCROLL]->GetItem(0)->GetRect().Width();
	}
	else
	{
		if( m_pAUIFrame )
			max_width = m_nWidth - m_pAUIFrame->GetSizeLimit().cx;
		else
			max_width = m_nWidth;
	}

	if( m_pFont )
	{
		float fWindowScale = m_pParent->GetAUIManager()->GetWindowScale() * m_pParent->GetScale();
		FitFont();
	}
	A3DFTFont* pFont = m_pFont ? m_pFont : m_pParent->GetAUIManager()->GetDefaultFont();
	int def_height = pFont->GetFontHeight();

	for (size_t i = 0; i < infos.size(); i++)
		CalcOneTagPosAndSize(infos[i], max_width, def_height);
}

void AUITextArea::CalcOneTagPosAndSize(LineInfo* pLine, int max_width, int def_height)
{
	LineTags& tags = pLine->tags;
	int lines = 0;
	A3DPOINT2 pt(0, 0);

	if( tags.size() == 0 )
	{
		pLine->lines = 0;
		pLine->height = 0;
	}
	else
	{
		size_t i;

		float fWindowScale = m_pParent->GetAUIManager()->GetWindowScale() * m_pParent->GetScale();
		for (i = 0; i < tags.size(); i++)
		{
			LineTag* pTag = tags[i];
			lines += pTag->CalcPosAndSize(lines, pt, max_width, def_height, fWindowScale);
		}

		pLine->lines = 1 + lines;
		pLine->height = pt.y + def_height;
		pLine->lines_off.push_back(0);

		if (pLine->lines > 1)
		{
			int cur_line = 1;
			for (i = 0; i < tags.size(); i++)
			{
				LineTag* pTag = tags[i];

				while (pTag->HasSubLine(cur_line))
				{
					pLine->lines_off.push_back(pTag->GetSubLineOff(cur_line));

					if (++cur_line >= pLine->lines)
						return;
				}
			}
		}
	}

	assert(pLine->lines == pLine->lines_off.size());
}

int AUITextArea::LineTag::CalcPosAndSize(int cur_line, A3DPOINT2& off, int max_width, int& height, float scale)
{
	if (size.y > height)
		height = size.y;

	line_num = 1;

	if (off.x + size.x > max_width)
	{
		pos.x = 0;
		pos.y = off.y + height;
		off.x = size.x;
		off.y = pos.y;
		line_start = cur_line + 1;
		return 1;
	}
	else
	{
		pos = off;
		off.x += size.x;
		line_start = cur_line;
		return 0;
	}
}

int AUITextArea::NameLinkTag::CalcPosAndSize(int cur_line, A3DPOINT2& off, int max_width, int& height, float scale)
{
	if( fontshadow.pFont )
		size = fontshadow.pFont->GetTextExtent(content);
	else
		size.Set(0, 0);
	size.y += line_info->line_space;
	return LineTag::CalcPosAndSize(cur_line, off, max_width, height, scale);
}

int AUITextArea::EditBoxItemTag::CalcPosAndSize(int cur_line, A3DPOINT2& off, int max_width, int& height, float scale)
{
	if( fontshadow.pFont )
		size = fontshadow.pFont->GetTextExtent(content);
	else
		size.Set(0, 0);

	size.y += line_info->line_space;
	return LineTag::CalcPosAndSize(cur_line, off, max_width, height, scale);
}

int AUITextArea::EmotionTag::CalcPosAndSize(int cur_line, A3DPOINT2& off, int max_width, int& height, float scale)
{
	fScale = scale;

	if (sprite)
	{
		A3DRECT& rc = sprite->GetItem(0)->GetRect();
		SetSize(rc.Width() * fScale, rc.Height() * fScale + line_info->line_space);
	}
	else
		SetSize(0, line_info->line_space);

	return LineTag::CalcPosAndSize(cur_line, off, max_width, height, scale);
}

int AUITextArea::ImageTag::CalcPosAndSize(int cur_line, A3DPOINT2& off, int max_width, int& height, float scale)
{
	fScale = scale;

	SetSize(0, line_info->line_space);
	if (sprite)
	{
		A2DSpriteItem *pSpriteItem = sprite->GetItem(iItem);
		if(pSpriteItem)
			SetSize(pSpriteItem->GetRect().Width() * fScale * m_pItem->GetImageScale(), 
			pSpriteItem->GetRect().Height() * fScale * m_pItem->GetImageScale() + line_info->line_space);
	}

	return LineTag::CalcPosAndSize(cur_line, off, max_width, height, scale);
}

int AUITextArea::TextTag::CalcPosAndSize(int cur_line, A3DPOINT2& off, int max_width, int& height, float scale)
{
	ClearSubTags();

	if( !fontshadow.pFont )
		return 0;

	int i = 0;
	ACHAR c;
	int lines = 0;
	int start = 0;
	A3DPOINT2 sub_off = off;

	while (c = content[i])
	{
		A3DPOINT2 pt = fontshadow.pFont->GetTextExtent(&c, 1);
		pt.y += line_info->line_space;
		bool new_line;

		if (off.x + pt.x > max_width)
		{
			off.x = 0;
			off.y += height;
			height = pt.y;
			lines++;
			new_line = true;
			if( IsEnglishCode(c) )
			{
				int j = i;
				while( i > start && IsEnglishCode(content[i - 1]) )
					i--;
				if( i == start && sub_off.x == 0 )
					i = j;
				else
				{
					c = content[i];
					pt = fontshadow.pFont->GetTextExtent(&c, 1);
					pt.y += line_info->line_space;
				}
			}
		}
		else
		{
			if (pt.y > height) height = pt.y;
			new_line = false;
		}

		if (i == 0)
		{
			pos = sub_off = off;
			line_start = cur_line + lines;
		}
		else if (new_line)
		{
			TextSubTag* pSub = new TextSubTag;
			pSub->sub_line = cur_line + lines - 1;
			pSub->str = &content[start];
			pSub->count = i - start;
			pSub->pos = sub_off;
			sub_tags.push_back(pSub);
			start = i;
			sub_off = off;
		}

		off.x += pt.x;
		i++;
	}

	if (i > start)
	{
		TextSubTag* pSub = new TextSubTag;
		pSub->sub_line = cur_line + lines;
		pSub->str = &content[start];
		pSub->count = i - start;
		pSub->pos = sub_off;
		sub_tags.push_back(pSub);
	}

	line_num = lines + 1;
	return lines;
}

int AUITextArea::TextTag::Render(
	A3DPOINT2 ptOff,
	int nStartSubLine,
	A3DRECT* pClip,
	int nAlpha)
{
	if( !fontshadow.pFont )
		return 0;

	A3DCOLOR s = A3DCOLORRGBA(0, 0, 0, fontshadow.nShadow * nAlpha / 255);
	int last_rendered_line = -1;
	A3DCOLOR textcolor = (nAlpha << 24) | (0xFFFFFF & cl);

	if (sub_tags.size())
	{
		for (size_t i = 0; i < sub_tags.size(); i++)
		{
			TextSubTag* pSub = sub_tags[i];

			if (pSub->sub_line < nStartSubLine)
				continue;

			int next_line = pSub->sub_line + 1;
			int bottom = next_line < line_info->lines ? line_info->lines_off[next_line] : line_info->height;

			if (bottom + ptOff.y > pClip->bottom)
				break;

			A3DPOINT2 pt = pSub->pos + ptOff;
			fontshadow.pFont->DrawText(pt.x, pt.y, pSub->str, textcolor, pClip, fontshadow.nShadow != 0, s, 0, pSub->count, 1.0f,
				underlineinfo.bUnderLine, underlineinfo.bSameColor ? textcolor : underlineinfo.color, 0.08f * underlineinfo.fScale);
			last_rendered_line = pSub->sub_line;
		}
	}
	else if (line_start < nStartSubLine)
		return -1;
	else
	{
		int next_line = line_start + 1;
		int bottom = next_line < line_info->lines ? line_info->lines_off[next_line] : line_info->height;

		if (bottom + ptOff.y > pClip->bottom)
			return -1;

		ptOff += pos;
		fontshadow.pFont->DrawText(ptOff.x, ptOff.y, content, textcolor, pClip, fontshadow.nShadow != 0, s, 0, -1, 1.0f,
			underlineinfo.bUnderLine, underlineinfo.bSameColor ? textcolor : underlineinfo.color, 0.08f * underlineinfo.fScale);
		last_rendered_line = line_start;
	}

	return last_rendered_line;
}

int AUITextArea::EmotionTag::Render(
	A3DPOINT2 ptOff,
	int nStartSubLine,
	A3DRECT* pClip,
	int nAlpha)
{
	if (line_start < nStartSubLine)
		return -1;

	ptOff += pos;
	if (ptOff.y + size.y > pClip->bottom || ptOff.x + size.x > pClip->right)
		return -1;

	if (emotion && sprite)
	{
		int nTick = int(GetTickCount() * 0.06f);
		int nFrame = emotion->nNumFrames;
		int nThisTick = nTick % emotion->nFrameTick[nFrame - 1];
		int nOffset = 0;
		while( nOffset < nFrame - 1 && nThisTick > emotion->nFrameTick[nOffset] )
			nOffset++;

		sprite->SetScaleX(fScale);
		sprite->SetScaleY(fScale);
		sprite->SetAlpha(nAlpha);
		sprite->SetCurrentItem(emotion->nStartPos + nOffset);
		sprite->DrawToInternalBuffer(ptOff.x, ptOff.y);
		sprite->SetAlpha(255);
	}

	return line_start;
}

int AUITextArea::ImageTag::Render(
	A3DPOINT2 ptOff,
	int nStartSubLine,
	A3DRECT* pClip,
	int nAlpha)
{
	if (line_start < nStartSubLine)
		return -1;

	ptOff += pos;
	if (ptOff.y + size.y > pClip->bottom || ptOff.x + size.x > pClip->right)
		return -1;

	if (sprite)
	{
		sprite->SetScaleX(fScale * m_pItem->GetImageScale());
		sprite->SetScaleY(fScale * m_pItem->GetImageScale());
		sprite->SetAlpha(nAlpha);
		sprite->SetColor(m_pItem->GetColor());
		sprite->SetCurrentItem(iItem);
		sprite->DrawToInternalBuffer(ptOff.x, ptOff.y);
		sprite->SetAlpha(255);
		AUITEXTAREA_EDITBOX_ITEM l;
		l.rc.SetRect(ptOff.x, ptOff.y, ptOff.x + size.x, ptOff.y + size.y);
		l.m_pItem = m_pItem;
		items->push_back(l);
	}

	return line_start;
}

int AUITextArea::NameLinkTag::Render(
	A3DPOINT2 ptOff,
	int nStartSubLine,
	A3DRECT* pClip,
	int nAlpha)
{
	switch(cType)
	{
	case 0:
		cl = A3DCOLORRGB(102, 255, 204);
		break;
	case 1:
		cl = A3DCOLORRGB(255, 153, 255);
		break;
	case 2:
		cl = A3DCOLORRGB(255, 227, 161);
		break;
	case 3:
		cl = clCustom;
		break;
	default:
		cl = A3DCOLORRGB(255, 220, 138);
	}
	int last_rendered_line = TextTag::Render(ptOff, nStartSubLine, pClip, nAlpha);

	if (last_rendered_line != -1)
	{
		AUITEXTAREA_NAME_LINK l;
		ptOff += pos;
		l.strName = content;
		l.rc.SetRect(ptOff.x, ptOff.y, ptOff.x + size.x, ptOff.y + size.y);
		links->push_back(l);
	}

	return last_rendered_line;
}

int AUITextArea::EditBoxItemTag::Render(
	A3DPOINT2 ptOff,
	int nStartSubLine,
	A3DRECT* pClip,
	int nAlpha)
{
	int last_rendered_line = TextTag::Render(ptOff, nStartSubLine, pClip, nAlpha);

	if (last_rendered_line != -1)
	{
		AUITEXTAREA_EDITBOX_ITEM l;
		ptOff += pos;
		l.rc.SetRect(ptOff.x, ptOff.y, ptOff.x + size.x, ptOff.y + size.y);
		l.m_pItem = m_pItem;
		items->push_back(l);
	}

	return last_rendered_line;
}

bool AUITextArea::IsInHitArea(int x, int y, UINT msg)
{
	if( !AUIObject::IsInHitArea(x, y, msg) )
		return false;

	if( !m_bTextTransparent || !m_bTransScrollWheel && !m_bOnlyShowText && msg == WM_MOUSEWHEEL && m_nLines > m_nLinesPP )
		return true;

	if( !m_bOnlyShowText && m_nLines > m_nLinesPP )
	{
		int nHitArea = GetHitArea(x, y);
		if( nHitArea == AUITEXTAREA_RECT_SCROLL ||
			nHitArea == AUITEXTAREA_RECT_UP ||
			nHitArea == AUITEXTAREA_RECT_DOWN )
			return true;
	}

	for(int i = 0; i < (int)m_EditBoxItems.size(); i++ )
	{
		if( (m_EditBoxItems[i].m_pItem->GetType() == enumEIIvtrlItem ||
			m_EditBoxItems[i].m_pItem->GetType() == enumEICoord ||
			m_EditBoxItems[i].m_pItem->GetType() == enumEIImage) &&
			m_EditBoxItems[i].rc.PtInRect(x, y) )
			return true;
	}

	return false;
}

int AUITextArea::GetRequiredHeight()
{
	UpdateAppend();
	int height = 0;

	for (int i = m_LinesInfo.size() - 1; i >= 0; i--)
	{
		LineInfo* pLine = m_LinesInfo[i];
		height += pLine->height;
	}

	SIZE sLimit = {0, 0};
	if( m_pAUIFrame )
		sLimit = m_pAUIFrame->GetSizeLimit();
	return height + sLimit.cy + 1;
}

bool AUITextArea::SetFontAndColor(const ACHAR *pszFontName, int nFontHeight,
	A3DCOLOR dwColor, int nShadow, int nOutline, bool bBold, bool bItalic, bool bFreeType,
	A3DCOLOR clOutline, A3DCOLOR clInnerText, bool bInterpColor, A3DCOLOR clUpper, A3DCOLOR clLower)
{
	bool bval=AUIObject::SetFontAndColor(pszFontName, nFontHeight, dwColor, nShadow, nOutline, 
							bBold, bItalic, bFreeType, clOutline, clInnerText, bInterpColor, clUpper, clLower);
	SetText(GetText());

	return bval;
}

bool AUITextArea::Resize()
{
	A3DFTFont *pOldFont = m_pFont ? m_pFont : m_pParent->GetAUIManager()->GetDefaultFont();

	if( !AUIObject::Resize() )
		return false;
	
	// update tags' font
	A3DFTFont *pFont = m_pFont ? m_pFont : m_pParent->GetAUIManager()->GetDefaultFont();
	if (pFont != pOldFont)
	{
		for (size_t i=0; i < m_LinesInfo.size(); i++)
		{
			LineTags tags = m_LinesInfo[i]->tags;

			for (size_t j=0; j < tags.size(); j++)
			{
				TextTag* pTag = dynamic_cast<TextTag*> (tags[j]);
				if (!pTag) continue;

				if (pTag->fontshadow.pFont == pOldFont)
					pTag->fontshadow.pFont = pFont;
			}
		}

		m_nFontShadowLast.pFont = pFont;
	}

	float fWindowScale = m_pParent->GetAUIManager()->GetWindowScale() * m_pParent->GetScale();

	if( m_pAUIFrame )
		m_pAUIFrame->SetScale(fWindowScale);

	for (size_t i = 0; i < m_LinesInfo.size(); i++)
	{
		LineInfo* pLine = m_LinesInfo[i];
		pLine->line_space = m_nLineSpace * fWindowScale;
		pLine->lines_off.clear();
	}
	
	CalcTagPosAndSize(m_LinesInfo);
	CalcTotalLines();
	
	return true;
}

void AUITextArea::Show(bool bShow)
{
	AUIObject::Show(bShow);
}

void AUITextArea::OptimizeSpriteResource(bool bLoad)
{
	if (m_nOptimizeResourceState == 0 && !bLoad || m_nOptimizeResourceState == 1 && bLoad)
		return;

	m_nOptimizeResourceState = (bLoad ? 1 : 0);

	for (int i = 0; i < AUITEXTAREA_RECT_NUM; i++)
	{
		if (m_pA2DSprite[i])
			glb_OptimizeSpriteResource(m_pA3DEngine, m_pA2DSprite[i], bLoad);
	}

	if (m_pAUIFrame)
		glb_OptimizeSpriteResource(m_pA3DEngine, m_pAUIFrame->GetA2DSprite(), bLoad);
}

void AUITextArea::PrintOptimizeResource()const
{
	for (int i = 0; i < AUITEXTAREA_RECT_NUM; i++)
	{
		glb_PrintOptimizeResource(m_pA2DSprite[i]);
	}
	
	if (m_pAUIFrame)
		glb_PrintOptimizeResource(m_pAUIFrame->GetA2DSprite());
}

void AUITextArea::ReserveBuffer(const unsigned int iSize)
{
	m_szText.GetBuffer(iSize);
	m_strRemain.GetBuffer(iSize);
}
