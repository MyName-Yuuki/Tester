// Filename	: AUITextArea.h
// Creator	: Tom Zhou
// Date		: December 17, 2004
// Desc		: AUITextArea is the class of text area control.

#ifndef _AUITEXTAREA_H_
#define _AUITEXTAREA_H_

#include "vector.h"

#include "AAssist.h"
#include "AUIObject.h"
#include "AUIFrame.h"
#include "AUICommon.h"
#include "AUIEditBox.h"

class EditBoxItemBase;
class EditBoxItemsSet;

enum
{
	AUITEXTAREA_RECT_MIN = 0,
	AUITEXTAREA_RECT_FRAME = AUITEXTAREA_RECT_MIN,
	AUITEXTAREA_RECT_UP,
	AUITEXTAREA_RECT_DOWN,
	AUITEXTAREA_RECT_SCROLL,
	AUITEXTAREA_RECT_BAR,
	AUITEXTAREA_RECT_MAX
};
#define AUITEXTAREA_RECT_NUM			AUITEXTAREA_RECT_MAX

#define AUITEXTAREA_STATE_NORMAL		0
#define AUITEXTAREA_STATE_PRESSUP		1
#define AUITEXTAREA_STATE_PRESSDOWN		2
#define AUITEXTAREA_STATE_PRESSBAR		3
#define AUITEXTAREA_EMOTHION_MAXFRAME	20

typedef struct _AUITEXTAREA_NAME_LINK
{
	A3DRECT rc;
	ACString strName;
} AUITEXTAREA_NAME_LINK, * P_AUITEXTAREA_NAME_LINK;

struct AUITEXTAREA_EDITBOX_ITEM
{
	A3DRECT rc;
	EditBoxItemBase* m_pItem;
};

typedef struct _AUITEXTAREA_IMAGE
{
	A2DSprite *pSprite;
	bool bSelfRelease;
} AUITEXTAREA_IMAGE, * P_AUITEXTAREA_IMAGE;

typedef struct _AUITEXTAREA_EMOTION
{
	int nStartPos;
	int nNumFrames;
	int	nTotalTick;
	int	nFrameTick[AUITEXTAREA_EMOTHION_MAXFRAME];
	ACString strHint;
} AUITEXTAREA_EMOTION, * P_AUITEXTAREA_EMOTION;

class AUITextArea : public AUIObject
{
public:
	AUITextArea();
	virtual ~AUITextArea();

	virtual bool Init(A3DEngine *pA3DEngine, A3DDevice *pA3DDevice, AScriptFile *pASF);
	virtual bool InitXML(A3DEngine *pA3DEngine, A3DDevice *pA3DDevice, AXMLItem *pItem);
	virtual bool InitIndirect(const char *pszFileName, int idType);
	virtual bool Release(void);
	virtual bool Save(FILE *file);
	virtual bool SaveXML(AXMLItem *pXMLItemDialog);
	virtual bool Tick(void);
	virtual bool Render(void);
	virtual void SetText(const ACHAR *pszText);
	virtual bool OnDlgItemMessage(UINT msg, WPARAM wParam, LPARAM lParam);
	virtual bool GetProperty(const	char *pszPropertyName, PAUIOBJECT_SETPROPERTY Property);
	virtual bool SetProperty(const	char *pszPropertyName, PAUIOBJECT_SETPROPERTY Property);
	static bool RegisterPropertyType();
	virtual bool IsInHitArea(int x, int y, UINT msg = WM_LBUTTONDOWN);
	virtual int GetRequiredHeight();
	virtual bool SetFontAndColor(const ACHAR *pszFontName, int nFontHeight,
		A3DCOLOR dwColor, int nShadow = 0, int nOutline = 0,
		bool bBold = false, bool bItalic = false, bool bFreeType = true,
		A3DCOLOR clOutline = 0,
		A3DCOLOR clInnerText = A3DCOLORRGB(255, 255, 255),
		bool bInterpColor = false,
		A3DCOLOR clUpper = 0,
		A3DCOLOR clLower = 0);
	virtual bool Resize();
	virtual void Show(bool bShow);
	virtual void OptimizeSpriteResource(bool bLoad);
	virtual void PrintOptimizeResource()const;

	void SetMaxLines(int nLines) { m_nMaxLines = nLines; }
	void ScrollToTop();
	void ScrollToBottom();
	void AppendText(const ACHAR *pszText, int nMsgIndex = 0, const ACHAR* szIvtrItem = L"", A3DCOLOR clIvtrItem = 0xffffffff, 
		const EditboxScriptItem** ppScriptItems = NULL, int nScriptItemCount = 0,
		UnderLineMode ulMode = UM_USE_AUIOBJECT_OPTION, A3DCOLOR clUnderLine = 0xff000000);
	void SetOnlyShowText(bool bShow);
	void SetForceRenderScroll(bool bForce);
	void SetLeftScrollBar(bool bLeft);
	void SetFirstLine(int nFirstLine);
	int GetFirstLine() { return m_nFirstLine; }
	int GetLines() { return m_nLines; }
	int GetLinesPP() { return m_nLinesPP;  }
	int GetHitArea(int x, int y);
	abase::vector<AUITEXTAREA_NAME_LINK> & GetNameLink();
	abase::vector<AUITEXTAREA_EDITBOX_ITEM>& GetEditBoxItems() { return m_EditBoxItems; }
	void SetEmotionList(A2DSprite **pA2DSpriteEmotion,
		abase::vector<AUITEXTAREA_EMOTION> **pvecEmotion);
	void SetImageList(abase::vector<A2DSprite*> *pvecImageList);
	void SetOtherFonts(abase::vector<FONT_WITH_SHADOW>* pvecFonts);
	void SetItemMask(int nMask)	{ m_nItemMask = nMask; }
	void ReserveBuffer(const unsigned int iSize);
	void UpdateAppend();

	const EditBoxItemsSet& AUITextArea::GetItemSet() {return m_ItemsSet;}

protected:
	bool m_bForceRenderScroll;
	bool m_bTextTransparent;
	bool m_bTransScrollWheel;
	POINT m_ptBar;
	int m_nDragDist;
	bool m_bDragging;
	int m_nScrollDelay;
	bool m_bOnlyShowText;
	bool m_bLeftScrollBar;
	bool m_bScrollToBottom;
	int	m_nItemMask;
	abase::vector<AUITEXTAREA_NAME_LINK> m_vecNameLink;
	int m_nFirstLine, m_nLines, m_nLinesPP, m_nLinesThisPage;

	A2DSprite **m_pA2DSpriteEmotion;
	abase::vector<AUITEXTAREA_EMOTION> **m_vecEmotion;

	abase::vector<AUITEXTAREA_EDITBOX_ITEM> m_EditBoxItems;
	EditBoxItemsSet m_ItemsSet;

	abase::vector<A2DSprite*> *m_pvecImageList;
	abase::vector<FONT_WITH_SHADOW> *m_pvecOtherFonts;

	int m_nFrameMode;
	PAUIFRAME m_pAUIFrame;
	A2DSprite *m_pA2DSprite[AUITEXTAREA_RECT_NUM];

	bool RenderScroll();

protected:

	struct UnderLineInfo
	{
		bool bUnderLine;
		bool bSameColor;
		A3DCOLOR color;
		float fScale;
	};

	struct LineInfo;
	struct LineTag
	{
		virtual ~LineTag() {}

		A3DPOINT2 pos;
		A3DPOINT2 size;
		int line_start;
		int line_num;
		LineInfo* line_info;

		void SetLineInfo(LineInfo* pInfo) { line_info = pInfo; }
		bool HasSubLine(int nSubLine) { return nSubLine >= line_start && nSubLine < line_start + line_num; }
		virtual int GetSubLineOff(int nSubLine) { return pos.y; }
		void SetPos(int x, int y) { pos.x = x; pos.y = y; }
		void SetSize(int w, int h) { size.x = w; size.y = h; }
		virtual int Render(A3DPOINT2 ptOff, int nStartSubLine, A3DRECT* pClip, int nAlpha) = 0;
		virtual int CalcPosAndSize(int cur_line, A3DPOINT2& off, int max_width, int& height, float scale);
		virtual ACHAR GetItemCode() { return 0; }
	};

	struct TextSubTag
	{
		int sub_line;
		const ACHAR* str;
		int count;
		A3DPOINT2 pos;
	};

	typedef abase::vector<TextSubTag*> SubTextTags;

	struct TextTag : public LineTag
	{
		virtual ~TextTag() { ClearSubTags(); }

		ACString content;
		A3DCOLOR cl;
		UnderLineInfo underlineinfo;
		FONT_WITH_SHADOW fontshadow;
		SubTextTags sub_tags;

		void ClearSubTags()
		{
			for (size_t i = 0; i < sub_tags.size(); i++)
				delete sub_tags[i];

			sub_tags.clear();
		}

		virtual int GetSubLineOff(int nSubLine)
		{
			for (size_t i = 0; i < sub_tags.size(); i++)
			{
				TextSubTag* pSub = sub_tags[i];

				if (pSub->sub_line == nSubLine)
					return pSub->pos.y;
			}

			return 0;
		}

		void SetText(const ACHAR* szText, A3DCOLOR _cl, UnderLineInfo _underlineinfo, FONT_WITH_SHADOW _fontshadow) { content = szText; cl = _cl; underlineinfo = _underlineinfo; fontshadow = _fontshadow; }
		virtual int Render(A3DPOINT2 ptOff, int nStartSubLine, A3DRECT* pClip, int nAlpha);
		virtual int CalcPosAndSize(int cur_line, A3DPOINT2& off, int max_width, int& height, float scale);
	};

	struct NameLinkTag : public TextTag
	{
		char cType;
		A3DCOLOR clCustom;
		virtual ~NameLinkTag() {}
		abase::vector<AUITEXTAREA_NAME_LINK>* links;
		virtual int Render(A3DPOINT2 ptOff, int nStartSubLine, A3DRECT* pClip, int nAlpha);
		virtual int CalcPosAndSize(int cur_line, A3DPOINT2& off, int max_width, int& height, float scale);
	};

	struct EditBoxItemTag : public TextTag
	{
		EditBoxItemBase* m_pItem;
		abase::vector<AUITEXTAREA_EDITBOX_ITEM>* items;
		ACHAR itemCode;
		virtual ~EditBoxItemTag() {}
		virtual int Render(A3DPOINT2 ptOff, int nStartSubLine, A3DRECT* pClip, int nAlpha);
		virtual int CalcPosAndSize(int cur_line, A3DPOINT2& off, int max_width, int& height, float scale);
	};

	struct EmotionTag : public LineTag
	{
		A2DSprite* sprite;
		AUITEXTAREA_EMOTION* emotion;
		float fScale;
		virtual ~EmotionTag() {}
		virtual int Render(A3DPOINT2 ptOff, int nStartSubLine, A3DRECT* pClip, int nAlpha);
		virtual int CalcPosAndSize(int cur_line, A3DPOINT2& off, int max_width, int& height, float scale);
	};

	struct ImageTag : public LineTag
	{
		EditBoxItemBase* m_pItem;
		abase::vector<AUITEXTAREA_EDITBOX_ITEM>* items;
		ACHAR itemCode;
		A2DSprite* sprite;
		int iItem;
		float fScale;
		virtual ~ImageTag() {}
		virtual int Render(A3DPOINT2 ptOff, int nStartSubLine, A3DRECT* pClip, int nAlpha);
		virtual int CalcPosAndSize(int cur_line, A3DPOINT2& off, int max_width, int& height, float scale);
		virtual ACHAR GetItemCode() { return itemCode; }
	};

	typedef abase::vector<LineTag*> LineTags;

	struct LineInfo
	{
		~LineInfo()
		{
			for (size_t i = 0; i < tags.size(); i++)
				delete tags[i];
		}

		int lines;
		int line_space;
		int height;
		abase::vector<int> lines_off;
		bool complete;
		LineTags tags;

		int Render(A3DPOINT2 ptOff, int nStartSubLine, A3DRECT* pClip, int nAlpha)
		{
			int rendered_lines = 0;
			
			for (size_t i = 0; i < tags.size(); i++)
			{
				LineTag* pTag = tags[i];
				int last_rendered_line = pTag->Render(ptOff, nStartSubLine, pClip, nAlpha) + 1;
				if( last_rendered_line > rendered_lines)
					rendered_lines = last_rendered_line;
			}

			rendered_lines -= nStartSubLine;
			return rendered_lines >= 0 ? rendered_lines : 0;
		}
	};

	typedef abase::vector<LineInfo*> LinesInfo;
	LinesInfo m_LinesInfo;
	int m_nMaxLines;

	ACString m_strRemain;
	bool m_bTextAppended;
	A3DCOLOR m_clLast;
	UnderLineInfo m_UnderLineInfoLast;
	FONT_WITH_SHADOW m_nFontShadowLast;

	int m_ItemUnderLineMode;
	A3DCOLOR m_clItemUnderLine;

	void ClearLinesInfo()
	{
		for (size_t i = 0; i < m_LinesInfo.size(); i++)
			delete m_LinesInfo[i];

		m_LinesInfo.clear();
		m_strRemain.Empty();
		m_ItemsSet.Release();
		m_EditBoxItems.clear();
	}

	const ACHAR* CalcTexFormat(const ACHAR* szText, LinesInfo& infos);
	void CalcLineFormat(LineInfo* pLine, const ACHAR* szLine, int count, A3DCOLOR& cl, UnderLineInfo& underlineinfo, FONT_WITH_SHADOW& fontshadow);
	void CalcTagPosAndSize(LinesInfo& infos);
	void CalcOneTagPosAndSize(LineInfo* pLine, int max_width, int def_height);
	void CalcTotalLines()
	{
		m_nLines = 0;

		for (size_t i = 0; i < m_LinesInfo.size(); i++)
			m_nLines += m_LinesInfo[i]->lines;
	}
	void CalcScrollLine(const A3DRECT& rc);
};

typedef AUITextArea * PAUITEXTAREA;

#endif	//	_AUITEXTAREA_H_