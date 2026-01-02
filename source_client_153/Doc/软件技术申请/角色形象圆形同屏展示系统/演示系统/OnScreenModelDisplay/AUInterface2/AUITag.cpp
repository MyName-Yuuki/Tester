// AUITag.cpp: implementation of the AUITag class.
//
//////////////////////////////////////////////////////////////////////

#include "AUI.h"
#include "AUITag.h"

extern int _tab_char;
// false-return. true-go on
static bool UpdateNewline( const TAG_OP_IN_PARAMS &inParams, TAG_RESOLVER_CONTEXT &context, bool bManualReturn = false)
{
	// 更新inParams._x，y。并判断是否需要绘制或者跳出循环
	context.nLines++;
	context.nCurX = ((bManualReturn && inParams._bAutoIndent) ? (inParams._x + 2 * context._nWidthSpace) : inParams._x);
		
	if( context.nLines > inParams._nStartLine )
	{
		context.nCurY += context.nLineHeightThis + inParams._nSpace;
		if( inParams._pRect && context.nCurY + context.pCurFont->GetFontHeight() > inParams._pRect->bottom )
		{
			context.bExceedBottom = true;
			if( !context._bCalcLines )
			{
				context.bFinish = true;
				return false;
			}
		}
		context.nLinesThisPage++;
	}
	context.nLineHeightThis = context.pCurFont->GetFontHeight();

	return true;
}

static inline bool IsNewLineNeed( const TAG_OP_IN_PARAMS &inParams, const TAG_RESOLVER_CONTEXT &context, int nCurTagWidth)
{
	return ( inParams._bAutoReturn && inParams._pRect && context.nCurX + nCurTagWidth > inParams._pRect->right );
}


static inline bool CanDraw( const TAG_OP_IN_PARAMS &inParams, const TAG_RESOLVER_CONTEXT &context )
{
	return inParams._bDrawToScreen && !context.bExceedBottom && context.nLines >= inParams._nStartLine;
}


#define INC_INDEX(context, nTagChar) context.index += nTagChar;

void InitTagParams(TAG_OP_IN_PARAMS& inParams, TAG_OP_OUT_PARAMS& outParams,
				   A3DFTFont *pFont, int x, int y, const ACHAR *pszText,
				   A3DRECT *pRect, int nStartLine, int *pnLines, int nSpace, bool bAutoIndent,
				   int *pnHeight, A2DSprite *pA2DSpriteEmotion,
				   abase::vector<AUITEXTAREA_EMOTION> *pvecEmotion,
				   int *pnLinesThisPage,
				   bool bDrawToScreen, int nShadow, int nAlpha, bool bSmallEmotion,
				   EditBoxItemsSet* pItemsSet,
				   abase::vector<A2DSprite*> *pvecImage,
				   float fWindowScale,
				   bool bAutoReturn,
				   abase::vector<FONT_WITH_SHADOW>* pvecOtherFonts,
				   int nWordWidth,
				   TEXTITEM_RCINFO* pItemRcInfo)
{
	// In Params
	inParams._pFont = pFont;
	inParams._pszText = pszText;
	inParams._x = x;
	inParams._y = y;
	inParams._bAutoIndent = bAutoIndent;
	inParams._bAutoReturn = bAutoReturn;
	inParams._bDrawToScreen = bDrawToScreen;
	inParams._bSmallEmotion = bSmallEmotion;
	inParams._fWindowScale = fWindowScale;
	inParams._nAlpha = nAlpha;
	inParams._nShadow = nShadow;
	inParams._nSpace = nSpace;
	inParams._nStartLine = nStartLine;
	inParams._nWordWidth = nWordWidth;
	inParams._pA2DSpriteEmotion = pA2DSpriteEmotion;
	inParams._pItemsSet = pItemsSet;
	inParams._pRect = pRect;
	inParams._pVecEmotion = pvecEmotion;
	inParams._pVecImage = pvecImage;
	inParams._pVecOtherFonts = pvecOtherFonts;
	
	if (inParams._pA2DSpriteEmotion)
	{
		A3DRECT rc = inParams._pA2DSpriteEmotion->GetItem(0)->GetRect();
		float fScale = 1.0f;
		if( inParams._bSmallEmotion )
			fScale = ((float)inParams._pFont->GetFontHeight()) / rc.Height();
		else
			fScale = inParams._fWindowScale;
		inParams._pA2DSpriteEmotion->SetScaleX(fScale);
		inParams._pA2DSpriteEmotion->SetScaleY(fScale);
	}
	
	// Out params
	outParams._pItemRcInfo = pItemRcInfo;
	outParams._pnHeight = pnHeight;
	outParams._pnLines = pnLines;
	outParams._pnLinesThisPage = pnLinesThisPage;
}


//////////////////////////////////////////////////////////////////////////
// AUITextTag
//////////////////////////////////////////////////////////////////////////
abase::vector<const AUITextTag*> AUITextTag::ms_Tags;
AUITextTag::AUITextTag(int iLevel)
{
	if (iLevel == 1)
		ms_Tags.push_back(this);
}

//////////////////////////////////////////////////////////////////////////
// AUIRichTextPrinter
//////////////////////////////////////////////////////////////////////////
AUIRichTextPrinter AUIRichTextPrinter::ms_TextPrinter;
bool AUIRichTextPrinter::InitContext(TAG_OP_IN_PARAMS& inParams, const TAG_OP_OUT_PARAMS& outParams, TAG_RESOLVER_CONTEXT& context)
{
	context._len = a_strlen(inParams._pszText);
	context._nCharH = inParams._pFont->GetFontHeight();
	context._nCharW = inParams._pFont->GetTextExtent(_AL("W"), 1).x;	// not every character's width
	context._nWidthSpace = context._nCharW;								// auto indent
	context._nTabW = _tab_char * context._nCharW;						// /v
	AUI_ConvertColor(inParams._pszText, context._clDefault);			// ^N
	context._bCalcLines = (outParams._pnLines != NULL);
	
	if( inParams._pA2DSpriteEmotion && inParams._pVecEmotion )
	{
		A3DRECT rc = inParams._pA2DSpriteEmotion->GetItem(0)->GetRect();
		context._nEmotionW = (int)(rc.Width() * inParams._pA2DSpriteEmotion->GetScaleX());
		context._nEmotionH = (int)(rc.Height() * inParams._pA2DSpriteEmotion->GetScaleY());
	}

	context.nCurX = inParams._x;
	context.nCurY = inParams._y;
	context.pCurFont = inParams._pFont;
	context.nLineHeightThis = context._nCharH;	
	context.nCurShadow = inParams._nShadow;
	context.color = A3DCOLORRGB(255, 255, 255);
	context.color_quote = A3DCOLORRGBA(255, 220, 138, inParams._nAlpha);

	return true;
}

const AUITextTag* AUIRichTextPrinter::GetTag(const ACHAR* pszText, int index, int len)
{
	abase::vector<const AUITextTag*>& vTags = AUITextTag::ms_Tags;
	for (size_t i=0; i<vTags.size(); i++)
	{
		if (vTags[i]->IsMyType(pszText, index, len))
			return vTags[i];
	}
	return NULL;
}

const AUITextTag* AUIRichTextPrinter::RenderPlainText(TAG_OP_IN_PARAMS& inParams, TAG_RESOLVER_CONTEXT& context) 
{
	const AUITextTag* pNextTagCtrl = NULL;
	bool bAutoWrap = inParams._bAutoReturn && inParams._pRect;

	const ACHAR* p = inParams._pszText + context.index;
	bool bEnglishWord = IsEnglishCode(*p);
	int iTextEOF = context.index;
	int nTextWidth = 0;

	while (*p != '\0')
	{
		// text. vs. ctrl tag
		pNextTagCtrl = GetTag(inParams._pszText, iTextEOF, context._len);
		if (pNextTagCtrl)
			break;			

		// English and Non-English are separately rendered. 
		// NameLink is always rendered in one time
		if (!context.bInQuote && bAutoWrap && (bEnglishWord ^ IsEnglishCode(*(p+(*p=='^')))))
			break;

		// ^^, ^&  ==  ^, &
		if (*p == '^') 
		{
			p++;
			iTextEOF++;
		}	
				
		int nCurWordWidth = context.pCurFont->GetTextExtent(p, 1).x;		
		if (IsNewLineNeed(inParams, context, nTextWidth + nCurWordWidth))
		{
			if (iTextEOF == context.index || bEnglishWord || context.bInQuote)
			{
				if (!UpdateNewline(inParams, context))
					return NULL;
			}
			else
				break;
		}
		
		nTextWidth += nCurWordWidth;	

		p++;
		iTextEOF++;
	}

	// render [index, iTextEOF)
	if (CanDraw(inParams, context) && iTextEOF > context.index)
	{
		A3DCOLOR text_color = context.bInQuote ? (inParams._nAlpha << 24) | (0xFFFFFF & context.color_quote) : (inParams._nAlpha << 24) | (0xFFFFFF & context.color);	
		if( inParams._pRect )
		{
			context.pCurFont->DrawText(context.nCurX, context.nCurY, inParams._pszText + context.index, text_color, inParams._pRect,
				context.nCurShadow ? true : false, A3DCOLORRGBA(0, 0, 0, context.nCurShadow * inParams._nAlpha / 255), 0, iTextEOF-context.index, 1.0f,
				context.bUnderLine, context.bUnderLineSameColor ? text_color : context.color_underline, 0.08f * inParams._fWindowScale);
		}
		else
		{
			context.pCurFont->TextOut(context.nCurX, context.nCurY, inParams._pszText + context.index, text_color,
				context.nCurShadow ? true : false, A3DCOLORRGBA(0, 0, 0, context.nCurShadow * inParams._nAlpha / 255), 0, iTextEOF-context.index, 1.0f,
				context.bUnderLine, context.bUnderLineSameColor ? text_color : context.color_underline, 0.08f * inParams._fWindowScale);
		}
	}

	if (context._bCalcRcInfo && context.bInQuote)
	{
		context.nl.strName += p;
		context.nl.rc.right = context.nCurX;
		if( context.nl.rc.Height() == 0 )
			context.nl.rc.bottom += context.nLineHeightThis;
	}
	
	context.nCurX += nTextWidth;	
	INC_INDEX(context, iTextEOF - context.index);

	return pNextTagCtrl;
}

bool AUIRichTextPrinter::Output(TAG_OP_IN_PARAMS& inParams, TAG_OP_OUT_PARAMS& outParams) const
{
	TAG_RESOLVER_CONTEXT context;
	if (!InitContext(inParams, outParams, context))
		return false;

	do 
	{
		// 控制字符
		const AUITextTag* pTag = RenderPlainText(inParams, context);
		if (pTag)
			pTag->Output(inParams, context, outParams);			

	} while (!context.bFinish && context.index < context._len);

	if( outParams._pnLines )
		*outParams._pnLines = context.nLines + 1;
	if( outParams._pnLinesThisPage )
		*outParams._pnLinesThisPage = context.nLinesThisPage + 1;
	if( outParams._pnHeight )
		*outParams._pnHeight = context.nCurY + context.nLineHeightThis - inParams._y;

	return true;
}

bool AUIRichTextPrinter::CalcSize(TAG_OP_IN_PARAMS& inParams, TAG_OP_OUT_PARAMS& outParams) const
{
	return true;
}

//////////////////////////////////////////////////////////////////////////
// AUITextTagReturn
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_AUITEXT_TAG(AUITextTagReturn, 1)

inline bool AUITextTagReturn::IsMyType(const ACHAR* pszText, int index, int len) const
{
	if (pszText[index] == '\r')
		return true;
	return false;
}

inline bool AUITextTagReturn::Output(const TAG_OP_IN_PARAMS& inParams, TAG_RESOLVER_CONTEXT& context, TAG_OP_OUT_PARAMS& outParams) const
{
	UpdateNewline(inParams, context, true);
	INC_INDEX(context, 1);
	if (context.bExceedBottom)
		context.nLinesThisPage -= 1;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// AUITextTagCtrl
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_AUITEXT_TAG(AUITextTagCtrl, 1)
abase::vector<AUITextTagCtrl*> AUITextTagCtrl::ms_vTags;
void AUITextTagCtrl::Init(int iLevel)
{
	if (iLevel == 2 && (ms_vTags.empty() || ms_vTags.back() != this))
		ms_vTags.push_back(this);
}

inline bool AUITextTagCtrl::IsMyType(const ACHAR* pszText, int index, int len) const
{
	// ^*
	return pszText[index] == '^' && (index+1>=len || (pszText[index+1] != '&' && pszText[index+1] != '^'));
}

bool AUITextTagCtrl::Output(const TAG_OP_IN_PARAMS& inParams, TAG_RESOLVER_CONTEXT& context, TAG_OP_OUT_PARAMS& outParams) const
{
	INC_INDEX(context, 1);
	for (size_t i=0; i<ms_vTags.size(); i++) // skip self
	{
		if (ms_vTags[i]->IsMyType(inParams._pszText, context.index, context._len))
			return ms_vTags[i]->Output(inParams, context, outParams);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
// AUITextTagCtrlFont
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_AUITEXT_TAG(AUITextTagCtrlFont, 2)

inline bool AUITextTagCtrlFont::IsMyType(const ACHAR* pszText, int index, int len) const
{
	return index + 3 < len && pszText[index] == 'O';
}

bool AUITextTagCtrlFont::Output(const TAG_OP_IN_PARAMS& inParams, TAG_RESOLVER_CONTEXT& context, TAG_OP_OUT_PARAMS& outParams) const
{	
	int nIndex = ((inParams._pszText[context.index + 1] - '0') * 10 + inParams._pszText[context.index + 2] - '0') * 10 + inParams._pszText[context.index + 3] - '0';
	if( nIndex > 0 && inParams._pVecOtherFonts && nIndex <= (int)inParams._pVecOtherFonts->size() && (*inParams._pVecOtherFonts)[nIndex - 1].pFont )
	{
		context.pCurFont = (*inParams._pVecOtherFonts)[nIndex - 1].pFont;
		context.nCurShadow = (*inParams._pVecOtherFonts)[nIndex - 1].nShadow;
	}
	else
	{
		context.pCurFont = inParams._pFont;
		context.nCurShadow = inParams._nShadow;
	}
	INC_INDEX(context, 4);

	if (context.pCurFont->GetFontHeight() > context.nLineHeightThis)
		context.nLineHeightThis = context.pCurFont->GetFontHeight();

	return true;
}

//////////////////////////////////////////////////////////////////////////
// AUITextTagCtrlUnderline
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_AUITEXT_TAG(AUITextTagCtrlUnderline, 2)

inline bool AUITextTagCtrlUnderline::IsMyType(const ACHAR* pszText, int index, int len) const
{
	// ^UFFFFFF	
	return index + 7 <= len && pszText[index] == 'U';
}

bool AUITextTagCtrlUnderline::Output(const TAG_OP_IN_PARAMS& inParams, TAG_RESOLVER_CONTEXT& context, TAG_OP_OUT_PARAMS& outParams) const
{			
	context.bUnderLine = !context.bUnderLine;
	context.bUnderLineSameColor = false;
	
	if( !context.bExceedBottom && inParams._bDrawToScreen )
	{
		context.color_underline = 0;
		for( int j = 0; j < 6; j++ )
		{
			int nValue = inParams._pszText[context.index + 1 + j];
			if( nValue >= '0' && nValue <= '9' )
				nValue -= '0';
			else if( nValue >= 'a' && nValue <= 'z' )
				nValue = nValue - 'a' + 0xA;
			else if( nValue >= 'A' && nValue <= 'Z' )
				nValue = nValue - 'A' + 0xA;
			else
				nValue = 0;
			context.color_underline = context.color_underline * 0x10 + nValue;
		}
		context.color_underline |= inParams._nAlpha << 24;
	}
	
	INC_INDEX(context, 7);

	return true;
}


//////////////////////////////////////////////////////////////////////////
// AUITextTagCtrlUnderlineX
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_AUITEXT_TAG(AUITextTagCtrlUnderlineX, 2)

inline bool AUITextTagCtrlUnderlineX::IsMyType(const ACHAR* pszText, int index, int len) const
{
	// ^u
	return pszText[index] == 'u';
}

bool AUITextTagCtrlUnderlineX::Output(const TAG_OP_IN_PARAMS& inParams, TAG_RESOLVER_CONTEXT& context, TAG_OP_OUT_PARAMS& outParams) const
{				
	context.bUnderLine = !context.bUnderLine;
	context.bUnderLineSameColor = true;
	
	INC_INDEX(context, 1);

	return true;
}

//////////////////////////////////////////////////////////////////////////
// AUITextTagCtrlWordspace
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_AUITEXT_TAG(AUITextTagCtrlWordspace, 2)

inline bool AUITextTagCtrlWordspace::IsMyType(const ACHAR* pszText, int index, int len) const
{
	//^o
	return pszText[index] == 'o';
}

bool AUITextTagCtrlWordspace::Output(const TAG_OP_IN_PARAMS& inParams, TAG_RESOLVER_CONTEXT& context, TAG_OP_OUT_PARAMS& outParams) const
{
	context.nCurX += inParams._nWordWidth;	
	
	INC_INDEX(context, 1);

	return true;
}


//////////////////////////////////////////////////////////////////////////
// AUITextTagCtrlDefaultColor
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_AUITEXT_TAG(AUITextTagCtrlDefaultColor, 2)

inline bool AUITextTagCtrlDefaultColor::IsMyType(const ACHAR* pszText, int index, int len) const
{
	//^N
	return (pszText[index] == 'N');
}

bool AUITextTagCtrlDefaultColor::Output(const TAG_OP_IN_PARAMS& inParams, TAG_RESOLVER_CONTEXT& context, TAG_OP_OUT_PARAMS& outParams) const
{
	context.color = context._clDefault;		
	INC_INDEX(context, 1);
	return true;
}


//////////////////////////////////////////////////////////////////////////
// AUITextTagCtrlDefaultColor
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_AUITEXT_TAG(AUITextTagCtrlColor, 2)

inline bool AUITextTagCtrlColor::IsMyType(const ACHAR* pszText, int index, int len) const
{
	//^FFFFFF
	return index+6 <= len;
}

bool AUITextTagCtrlColor::Output(const TAG_OP_IN_PARAMS& inParams, TAG_RESOLVER_CONTEXT& context, TAG_OP_OUT_PARAMS& outParams) const
{
	if( !context.bExceedBottom && inParams._bDrawToScreen )
	{
		context.color = 0;
		for( int j = 0; j < 6; j++ )
		{
			int nValue = inParams._pszText[context.index + j];
			if( nValue >= '0' && nValue <= '9' )
				nValue -= '0';
			else if( nValue >= 'a' && nValue <= 'z' )
				nValue = nValue - 'a' + 0xA;
			else if( nValue >= 'A' && nValue <= 'Z' )
				nValue = nValue - 'A' + 0xA;
			else
				nValue = 0;
			context.color = context.color * 0x10 + nValue;
		}
		context.color |= inParams._nAlpha << 24;
	}

	INC_INDEX(context, 6);

	return true;
}


//////////////////////////////////////////////////////////////////////////
// AUITextTagItem
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_AUITEXT_TAG(AUITextTagItem, 1)
abase::vector<AUITextTagItem*> AUITextTagItem::ms_vTags;
void AUITextTagItem::Init(int iLevel)
{
	if (iLevel == 2 && (ms_vTags.empty() || ms_vTags.back() != this))
		ms_vTags.push_back(this);
}

inline bool AUITextTagItem::IsMyType(const ACHAR* pszText, int index, int len) const
{
	return IsEditboxItemCode(pszText[index]);
}

bool AUITextTagItem::Output(const TAG_OP_IN_PARAMS& inParams, TAG_RESOLVER_CONTEXT& context, TAG_OP_OUT_PARAMS& outParams) const
{	
	EditBoxItemBase* pItem = NULL;
	if (!inParams._pItemsSet || NULL == (pItem = inParams._pItemsSet->GetItemByChar(inParams._pszText[context.index])))
	{
		INC_INDEX(context, 1);
		return true;
	}

	INC_INDEX(context, 1);
	for (size_t i=0; i<ms_vTags.size(); i++)
	{
		if (!ms_vTags[i]->IsMyType(pItem))
			continue;
		
		int oldx = context.nCurX;
		int oldy = context.nCurY;
		bool bval = ms_vTags[i]->Output(inParams, context, outParams, pItem);
		
		if (pItem->GetType() != enumEIEmotion && outParams._pItemRcInfo && outParams._pItemRcInfo->pVecEBItems)
		{
			AUITEXTAREA_EDITBOX_ITEM l;
			l.rc.SetRect(oldx, oldy, context.nCurX, oldy + context.nLineHeightThis);
			l.m_pItem = pItem;
			outParams._pItemRcInfo->pVecEBItems->push_back(l);
		}
		
		return bval;		
	}	

	return true;
}

//////////////////////////////////////////////////////////////////////////
// AUITextTagItemImage
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_AUITEXT_TAG(AUITextTagItemImage, 2)
inline bool AUITextTagItemImage::IsMyType(const EditBoxItemBase* pItem) const
{
	return (pItem && pItem->GetType() == enumEIImage);
}

bool AUITextTagItemImage::Output(const TAG_OP_IN_PARAMS& inParams, TAG_RESOLVER_CONTEXT& context, TAG_OP_OUT_PARAMS& outParams, const EditBoxItemBase* pItem) const
{	
	if( !pItem || !inParams._pVecImage || pItem->GetImageIndex() >= (int)inParams._pVecImage->size() )
		return true;

	A2DSprite* pSprite = (*inParams._pVecImage)[pItem->GetImageIndex()];
	if( !pSprite )
		return true;
		
	A2DSpriteItem *pSpriteItem = pSprite->GetItem(pItem->GetImageFrame());
	if( !pSpriteItem )
		return true;

	SIZE szItem = {0, 0};	
	float fImgScale = inParams._fWindowScale * pItem->GetImageScale();
	szItem.cx = (int)(pSpriteItem->GetRect().Width() * fImgScale);
	szItem.cy = (int)(pSpriteItem->GetRect().Height() * fImgScale);
	pSprite->SetCurrentItem(pItem->GetImageFrame());
	pSprite->SetColor(pItem->GetColor());

	if( szItem.cy > context.nLineHeightThis )
		context.nLineHeightThis = szItem.cy;

	if( IsNewLineNeed(inParams, context, szItem.cx) && !UpdateNewline(inParams, context) )
		return true;
				
	if( !CanDraw(inParams, context) )
	{
		context.nCurX += szItem.cx;
		return true;
	}

	if( !inParams._pRect ||
		(context.nCurX < inParams._pRect->right && context.nCurX >= inParams._pRect->left))
	{
		pSprite->SetScaleX(fImgScale);
		pSprite->SetScaleY(fImgScale);
		bool bval = pSprite->DrawToInternalBuffer(context.nCurX, context.nCurY);
		if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
	}	
	context.nCurX += szItem.cx;

	return true;
}

//////////////////////////////////////////////////////////////////////////
// AUITextTagItemEmotion
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_AUITEXT_TAG(AUITextTagItemEmotion, 2)
inline bool AUITextTagItemEmotion::IsMyType(const EditBoxItemBase* pItem) const
{
	return (pItem && pItem->GetType() == enumEIEmotion);
}

bool AUITextTagItemEmotion::Output(const TAG_OP_IN_PARAMS& inParams, TAG_RESOLVER_CONTEXT& context, TAG_OP_OUT_PARAMS& outParams, const EditBoxItemBase* pItem) const
{	
	if (!pItem)
		return true;

	if( context._nEmotionW <= 0 )
		return true;
	
	if( context._nEmotionH > context.nLineHeightThis )
		context.nLineHeightThis = context._nEmotionH;

	if (IsNewLineNeed(inParams, context, context._nEmotionW) && !UpdateNewline(inParams, context))
		return true;

	if( !CanDraw(inParams, context) || !inParams._pA2DSpriteEmotion || !inParams._pVecEmotion)
	{
		context.nCurX += context._nEmotionW;
		return true;
	}
	
	if( !inParams._pRect || 
		(context.nCurX >= inParams._pRect->left && context.nCurX + context._nEmotionW < inParams._pRect->right) )
	{
		int nNumber = 0;
		int nSet = 0;
		
		UnmarshalEmotionInfo(pItem->GetInfo(), nSet, nNumber);
		if( nNumber < (int)inParams._pVecEmotion->size() )
		{
			AUITEXTAREA_EMOTION& e = (*inParams._pVecEmotion)[nNumber];
			int nBase = e.nStartPos;
			int nFrame = e.nNumFrames;
			int nThisTick = int(GetTickCount() * 0.06) % e.nFrameTick[nFrame - 1];
			int nOffset = 0;
			while( nOffset < nFrame - 1 && nThisTick > e.nFrameTick[nOffset] )
				nOffset++;
			inParams._pA2DSpriteEmotion->SetCurrentItem(nBase + nOffset);
			
			bool bval = inParams._pA2DSpriteEmotion->DrawToInternalBuffer(context.nCurX, context.nCurY);
			if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
		}
	}
	
	context.nCurX += context._nEmotionW;

	return true;
}


//////////////////////////////////////////////////////////////////////////
// AUITextTagItemText
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_AUITEXT_TAG(AUITextTagItemText, 2)
inline bool AUITextTagItemText::IsMyType(const EditBoxItemBase* pItem) const
{
	return (pItem && (pItem->GetType() != enumEIEmotion && pItem->GetType() != enumEIImage));
}

bool AUITextTagItemText::Output(const TAG_OP_IN_PARAMS& inParams, TAG_RESOLVER_CONTEXT& context, TAG_OP_OUT_PARAMS& outParams, const EditBoxItemBase* pItem) const
{	
	if (!pItem)
		return true;

	A3DPOINT2 ptSize = context.pCurFont->GetTextExtent(pItem->GetName());
	if( ptSize.y > context.nLineHeightThis )
		context.nLineHeightThis = ptSize.y;

	if (IsNewLineNeed(inParams, context, ptSize.x) && !UpdateNewline(inParams, context))
		return true;

	if( !CanDraw(inParams, context) )
	{
		context.nCurX += ptSize.x;
		return true;
	}

	A3DCOLOR text_color = (inParams._nAlpha << 24) | (0xFFFFFF & pItem->GetColor());
	if( inParams._pRect )
	{
		context.pCurFont->DrawText(context.nCurX, context.nCurY, pItem->GetName(), text_color, inParams._pRect,
			context.nCurShadow ? true : false, A3DCOLORRGBA(0, 0, 0, context.nCurShadow * inParams._nAlpha / 255), 0, -1, 1.0f,
			context.bUnderLine, context.bUnderLineSameColor ? text_color : context.color_underline, 0.08f * inParams._fWindowScale);
	}
	else
	{
		context.pCurFont->TextOut(context.nCurX, context.nCurY, pItem->GetName(), text_color,
			context.nCurShadow ? true : false, A3DCOLORRGBA(0, 0, 0, context.nCurShadow * inParams._nAlpha / 255), 0, -1, 1.0f,
			context.bUnderLine, context.bUnderLineSameColor ? text_color : context.color_underline, 0.08f * inParams._fWindowScale);
	}
	
	context.nCurX += ptSize.x;

	return true;
}


//////////////////////////////////////////////////////////////////////////
// AUITextTagQuote
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_AUITEXT_TAG(AUITextTagQuote, 1)
inline bool AUITextTagQuote::IsMyType(const ACHAR* pszText, int index, int len) const
{
	return pszText[index] == '&';
}

bool AUITextTagQuote::Output(const TAG_OP_IN_PARAMS& inParams, TAG_RESOLVER_CONTEXT& context, TAG_OP_OUT_PARAMS& outParams) const
{		
	context.bInQuote = !context.bInQuote;
	if (!context.bInQuote)
	{
		if (outParams._pItemRcInfo && outParams._pItemRcInfo->pVecNameLinks)
			outParams._pItemRcInfo->pVecNameLinks->push_back(context.nl);
	}
	else
	{
		context.color_quote = A3DCOLORRGBA(255, 220, 138, inParams._nAlpha);		
		if (AUI_ConvertColor(inParams._pszText + context.index + 1, context.color_quote))
			INC_INDEX(context, 7);

		context.nl.strName = _AL("");
		context.nl.rc.SetRect(context.nCurX, context.nCurY, context.nCurX, context.nCurY);
	}

	INC_INDEX(context, 1);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// AUITextTagFakReturn
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_AUITEXT_TAG(AUITextTagFakReturn, 1)
inline bool AUITextTagFakReturn::IsMyType(const ACHAR* pszText, int index, int len) const
{
	return pszText[index] == '\n';
}

bool AUITextTagFakReturn::Output(const TAG_OP_IN_PARAMS& inParams, TAG_RESOLVER_CONTEXT& context, TAG_OP_OUT_PARAMS& outParams) const
{	
	INC_INDEX(context, 1);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// AUITextTagTable
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_AUITEXT_TAG(AUITextTagTable, 1)
inline bool AUITextTagTable::IsMyType(const ACHAR* pszText, int index, int len) const
{
	return pszText[index] == '\v';
}

bool AUITextTagTable::Output(const TAG_OP_IN_PARAMS& inParams, TAG_RESOLVER_CONTEXT& context, TAG_OP_OUT_PARAMS& outParams) const
{	
	int nWidthThis = context.nCurX - inParams._x;
	nWidthThis = (nWidthThis / context._nTabW + 1) * context._nTabW;
	INC_INDEX(context, 1);

	context.nCurX = inParams._x + nWidthThis;
	return true;
}