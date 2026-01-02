// AUITag.h: interface for the AUITag class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUITAG_H__F5D79DDA_CEE1_4AFA_9BD0_DC83840670E1__INCLUDED_)
#define AFX_AUITAG_H__F5D79DDA_CEE1_4AFA_9BD0_DC83840670E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AUIObject.h"
#include "A3DFTFontMan.h"
#include "AUICommon.h"
#include "A2DSprite.h"
#include "A2DSpriteItem.h"
#include "A2DSpriteTexMan.h"


struct TAG_OP_IN_PARAMS
{
	//////////////////////////////////////////////////////////////////////////
	// in params
	int			_x;
	int			_y;
	const ACHAR* _pszText;
	A3DRECT*	_pRect;
	int			_nStartLine;
	int			_nWordWidth;
	int			_nShadow;
	int			_nAlpha;
	int			_nSpace;
	float		_fWindowScale;

	// flags
	bool		_bAutoIndent;
	bool		_bAutoReturn;
	bool		_bSmallEmotion;
	bool		_bDrawToScreen;

	// resources
	A3DFTFont*	_pFont;
	abase::vector<FONT_WITH_SHADOW>* _pVecOtherFonts;
	A2DSprite*	_pA2DSpriteEmotion;
	abase::vector<AUITEXTAREA_EMOTION>* _pVecEmotion;
	abase::vector<A2DSprite*>* _pVecImage;
	EditBoxItemsSet* _pItemsSet;

	TAG_OP_IN_PARAMS(A3DFTFont *pFont = NULL, 
		int x = 0, 
		int y = 0, 
		const ACHAR *pszText = NULL,
		A3DRECT *pRect = NULL, 
		int nStartLine = 0, 
		int nSpace = 0, 
		bool bAutoIndent = false,

		A2DSprite *pA2DSpriteEmotion = NULL,
		abase::vector<AUITEXTAREA_EMOTION> *pvecEmotion = NULL,

		bool bDrawToScreen = true,	
		int nShadow = 0, 
		int nAlpha = 255,
		bool bSmallEmotion = false,
		EditBoxItemsSet* pItemsSet = NULL,
		abase::vector<A2DSprite*> *pvecImage = NULL,
		float fWindowScale = 1.0f,
		bool bAutoReturn = true,
		abase::vector<FONT_WITH_SHADOW>* pvecOtherFonts = NULL,
		int nWordWidth = 0)
		:	_pFont(pFont),
		_x(x),
		_y(y),
		_pszText(pszText),
		_pRect(pRect),
		_nStartLine(nStartLine),
		_nSpace(nSpace),
		_bAutoIndent(bAutoIndent),
		_pA2DSpriteEmotion(pA2DSpriteEmotion),
		_pVecEmotion(pvecEmotion),
		_bDrawToScreen(bDrawToScreen),
		_nShadow(nShadow),
		_nAlpha(nAlpha),
		_bSmallEmotion(bSmallEmotion),
		_pItemsSet(pItemsSet),
		_pVecImage(pvecImage),
		_fWindowScale(fWindowScale),
		_bAutoReturn(bAutoReturn),
		_pVecOtherFonts(pvecOtherFonts),
		_nWordWidth(nWordWidth)
	{}
};


struct TAG_OP_OUT_PARAMS
{
	//////////////////////////////////////////////////////////////////////////
	// out params
	int* _pnLines;
	int* _pnHeight;
	int* _pnLinesThisPage;
	TEXTITEM_RCINFO* _pItemRcInfo;

	TAG_OP_OUT_PARAMS(int *pnLines = NULL,
		int *pnHeight = NULL,
		int *pnLinesThisPage = NULL,
		TEXTITEM_RCINFO* pRcInfo = NULL)
		:	_pnLines(pnLines),
		_pnHeight(pnHeight),
		_pnLinesThisPage(pnLinesThisPage),
		_pItemRcInfo(pRcInfo)
	{}
};

typedef struct _TAG_RESOLVER_CONTEXT
{
	A3DFTFont*	pCurFont;
	int			index;
	int			nCurX;
	int			nCurY;
	bool		bExceedBottom;

	bool		bInQuote;
	int			nCurShadow;

	bool		bFinish;

	A3DCOLOR	color;

	A3DCOLOR	color_underline;
	A3DCOLOR	color_quote;
	bool		bUnderLine;
	bool		bUnderLineSameColor;

	int			nLines;
	int			nLineHeightThis;
	int			nLinesThisPage;

	AUITEXTAREA_NAME_LINK nl;

	//////////////////////////////////////////////////////////////////////////
	// calculate params (from default font, shouldn't be changed in render function)
	int			_nCharW;
	int			_nCharH;
	int			_nEmotionW;
	int			_nEmotionH;
	bool		_bCalcLines;
	bool		_bCalcRcInfo;
	int			_nTabW;
	int			_nWidthSpace;
	A3DCOLOR	_clDefault;
	int			_len;

	_TAG_RESOLVER_CONTEXT() :
	index(0),
		nCurX(0),
		nCurY(0),
		bExceedBottom(false),
		color(0xffffff),
		color_underline(0xffffff),
		color_quote(A3DCOLORRGB(255, 220, 138)),
		nLines(0),
		nLineHeightThis(0),
		nLinesThisPage(0),
		pCurFont(NULL),
		bFinish(false),
		bUnderLine(false),
		bUnderLineSameColor(false),
		bInQuote(false),
		_nCharW(0), 
		_nCharH(0),
		_nEmotionW(0), 
		_nEmotionH(0),
		_bCalcLines(false), 
		_nTabW(0),
		_nWidthSpace(0),
		_clDefault(A3DCOLORRGB(255,255,255)),
		_len(0),
		_bCalcRcInfo(false)
	{
		nl.rc.Clear();
	}	
}TAG_RESOLVER_CONTEXT;

struct TAG_PARAMS
{
	TAG_OP_IN_PARAMS inParams;
	TAG_OP_OUT_PARAMS outParams;
	TAG_RESOLVER_CONTEXT context;
};


#define DECLARE_AUITEXT_TAG(class_name, base_class) \
	static class_name ms_class;\
	class_name(int ilevel):base_class(ilevel){Init(ilevel);}

#define IMPLEMENT_AUITEXT_TAG(class_name, iLevel) \
	class_name class_name::ms_class(iLevel);

bool AUI_ConvertColor(const ACHAR* pszText, A3DCOLOR& color);

class AUITextTag
{
	friend class AUIRichTextPrinter;
public:
	AUITextTag(int iLevel);
	virtual bool IsMyType(const ACHAR* pszText, int index, int len) const = 0;
	virtual bool Output(const TAG_OP_IN_PARAMS& inParams, TAG_RESOLVER_CONTEXT& context, TAG_OP_OUT_PARAMS& outParams) const = 0;

protected:
	virtual void Init(int iLevel){}
	static abase::vector<const AUITextTag*> ms_Tags;
};

class AUIRichTextPrinter
{
public:
	static AUIRichTextPrinter& GetSingleton() {return ms_TextPrinter;}
	
	bool Output(TAG_OP_IN_PARAMS& inParams, TAG_OP_OUT_PARAMS& outParams) const;
	bool CalcSize(TAG_OP_IN_PARAMS& inParams, TAG_OP_OUT_PARAMS& outParams) const;

protected:
	AUIRichTextPrinter(){}
	AUIRichTextPrinter(const AUIRichTextPrinter&) {}
	AUIRichTextPrinter& operator = (const AUIRichTextPrinter&) {}

	static bool InitContext(TAG_OP_IN_PARAMS& inParams, const TAG_OP_OUT_PARAMS& outParams, TAG_RESOLVER_CONTEXT& context);
	static const AUITextTag* GetTag(const ACHAR* pszText, int index, int len);
	static const AUITextTag* RenderPlainText(TAG_OP_IN_PARAMS& inParams, TAG_RESOLVER_CONTEXT& context);

protected:
	static AUIRichTextPrinter ms_TextPrinter;
};

class AUITextTagReturn : public AUITextTag
{
protected:
	DECLARE_AUITEXT_TAG(AUITextTagReturn, AUITextTag)
public:
	virtual bool IsMyType(const ACHAR* pszText, int index, int len) const;
	virtual bool Output(const TAG_OP_IN_PARAMS& inParams, TAG_RESOLVER_CONTEXT& context, TAG_OP_OUT_PARAMS& outParams) const;
};

class AUITextTagCtrl : public AUITextTag
{
protected:
	DECLARE_AUITEXT_TAG(AUITextTagCtrl, AUITextTag)
public:
	virtual bool IsMyType(const ACHAR* pszText, int index, int len) const;
	virtual bool Output(const TAG_OP_IN_PARAMS& inParams, TAG_RESOLVER_CONTEXT& context, TAG_OP_OUT_PARAMS& outParams) const;

protected:
	virtual void Init(int iLevel);

	static abase::vector<AUITextTagCtrl*> ms_vTags;
};

class AUITextTagCtrlFont : public AUITextTagCtrl
{
protected:
	DECLARE_AUITEXT_TAG(AUITextTagCtrlFont, AUITextTagCtrl)
public:
	virtual bool IsMyType(const ACHAR* pszText, int index, int len) const;
	virtual bool Output(const TAG_OP_IN_PARAMS& inParams, TAG_RESOLVER_CONTEXT& context, TAG_OP_OUT_PARAMS& outParams) const;
};

class AUITextTagCtrlUnderline : public AUITextTagCtrl
{
protected:
	DECLARE_AUITEXT_TAG(AUITextTagCtrlUnderline, AUITextTagCtrl)
public:
	virtual bool IsMyType(const ACHAR* pszText, int index, int len) const;
	virtual bool Output(const TAG_OP_IN_PARAMS& inParams, TAG_RESOLVER_CONTEXT& context, TAG_OP_OUT_PARAMS& outParams) const;
};

class AUITextTagCtrlUnderlineX : public AUITextTagCtrl
{
protected:
	DECLARE_AUITEXT_TAG(AUITextTagCtrlUnderlineX, AUITextTagCtrl)
public:
	virtual bool IsMyType(const ACHAR* pszText, int index, int len) const;
	virtual bool Output(const TAG_OP_IN_PARAMS& inParams, TAG_RESOLVER_CONTEXT& context, TAG_OP_OUT_PARAMS& outParams) const;
};

class AUITextTagCtrlWordspace : public AUITextTagCtrl
{
protected:
	DECLARE_AUITEXT_TAG(AUITextTagCtrlWordspace, AUITextTagCtrl)
public:
	virtual bool IsMyType(const ACHAR* pszText, int index, int len) const;
	virtual bool Output(const TAG_OP_IN_PARAMS& inParams, TAG_RESOLVER_CONTEXT& context, TAG_OP_OUT_PARAMS& outParams) const;
};

class AUITextTagCtrlDefaultColor : public AUITextTagCtrl
{
protected:
	DECLARE_AUITEXT_TAG(AUITextTagCtrlDefaultColor, AUITextTagCtrl)
public:
	virtual bool IsMyType(const ACHAR* pszText, int index, int len) const;
	virtual bool Output(const TAG_OP_IN_PARAMS& inParams, TAG_RESOLVER_CONTEXT& context, TAG_OP_OUT_PARAMS& outParams) const;
};

class AUITextTagCtrlColor : public AUITextTagCtrl
{
protected:
	DECLARE_AUITEXT_TAG(AUITextTagCtrlColor, AUITextTagCtrl)
public:
	virtual bool IsMyType(const ACHAR* pszText, int index, int len) const;
	virtual bool Output(const TAG_OP_IN_PARAMS& inParams, TAG_RESOLVER_CONTEXT& context, TAG_OP_OUT_PARAMS& outParams) const;
};

class AUITextTagItem : public AUITextTag
{
protected:
	DECLARE_AUITEXT_TAG(AUITextTagItem, AUITextTag)
public:
	virtual bool IsMyType(const ACHAR* pszText, int index, int len) const;
	virtual bool Output(const TAG_OP_IN_PARAMS& inParams, TAG_RESOLVER_CONTEXT& context, TAG_OP_OUT_PARAMS& outParams) const;
	virtual inline int GetSize() const {return (int)ms_vTags.size();}

	virtual bool IsMyType(const EditBoxItemBase* pItem) const {return false;}
	virtual bool Output(const TAG_OP_IN_PARAMS& inParams, TAG_RESOLVER_CONTEXT& context, 
		TAG_OP_OUT_PARAMS& outParams, const EditBoxItemBase* pItem) const 
	{return true;}

protected:
	virtual void Init(int iLevel);
	static abase::vector<AUITextTagItem*> ms_vTags;
};

class AUITextTagItemImage : public AUITextTagItem
{
protected:
	DECLARE_AUITEXT_TAG(AUITextTagItemImage, AUITextTagItem)
public:
	virtual bool IsMyType(const EditBoxItemBase* pItem) const;
	virtual bool Output(const TAG_OP_IN_PARAMS& inParams, TAG_RESOLVER_CONTEXT& context, TAG_OP_OUT_PARAMS& outParams, const EditBoxItemBase* pItem) const;
};

class AUITextTagItemEmotion : public AUITextTagItem
{
protected:
	DECLARE_AUITEXT_TAG(AUITextTagItemEmotion, AUITextTagItem)
public:
	virtual bool IsMyType(const EditBoxItemBase* pItem) const;
	virtual bool Output(const TAG_OP_IN_PARAMS& inParams, TAG_RESOLVER_CONTEXT& context, TAG_OP_OUT_PARAMS& outParams, const EditBoxItemBase* pItem) const;
};

class AUITextTagItemText : public AUITextTagItem
{
protected:
	DECLARE_AUITEXT_TAG(AUITextTagItemText, AUITextTagItem)
public:
	virtual bool IsMyType(const EditBoxItemBase* pItem) const;
	virtual bool Output(const TAG_OP_IN_PARAMS& inParams, TAG_RESOLVER_CONTEXT& context, TAG_OP_OUT_PARAMS& outParams, const EditBoxItemBase* pItem) const;
};

class AUITextTagQuote : public AUITextTag
{
protected:
	DECLARE_AUITEXT_TAG(AUITextTagQuote, AUITextTag)
public:
	virtual bool IsMyType(const ACHAR* pszText, int index, int len) const;
	virtual bool Output(const TAG_OP_IN_PARAMS& inParams, TAG_RESOLVER_CONTEXT& context, TAG_OP_OUT_PARAMS& outParams) const;
};

class AUITextTagFakReturn : public AUITextTag
{
protected:
	DECLARE_AUITEXT_TAG(AUITextTagFakReturn, AUITextTag)
public:
	virtual bool IsMyType(const ACHAR* pszText, int index, int len) const;
	virtual bool Output(const TAG_OP_IN_PARAMS& inParams, TAG_RESOLVER_CONTEXT& context, TAG_OP_OUT_PARAMS& outParams) const;
};

class AUITextTagTable : public AUITextTag
{
protected:
	DECLARE_AUITEXT_TAG(AUITextTagTable, AUITextTag)
public:
	virtual bool IsMyType(const ACHAR* pszText, int index, int len) const;
	virtual bool Output(const TAG_OP_IN_PARAMS& inParams, TAG_RESOLVER_CONTEXT& context, TAG_OP_OUT_PARAMS& outParams) const;
};

#endif // !defined(AFX_AUITAG_H__F5D79DDA_CEE1_4AFA_9BD0_DC83840670E1__INCLUDED_)
