// Filename	: AUISubDialog.h
// Creator	: Xiao Zhou
// Date		: 2009/4/2
// Desc		: AUISubDialog is the class of sub dialog control.

#ifndef _AUISUBDIALOG_H_
#define _AUISUBDIALOG_H_

#include "vector.h"

#include "A3DMacros.h"
#include "AAssist.h"
#include "AUIObject.h"
#include "AUIFrame.h"
#include "AUIDialog.h"

enum
{
	AUISUBDIALOG_RECT_MIN = 0,
	AUISUBDIALOG_RECT_UP,
	AUISUBDIALOG_RECT_DOWN,
	AUISUBDIALOG_RECT_VSCROLL,
	AUISUBDIALOG_RECT_VBAR,
	AUISUBDIALOG_RECT_LEFT,
	AUISUBDIALOG_RECT_RIGHT,
	AUISUBDIALOG_RECT_HSCROLL,
	AUISUBDIALOG_RECT_HBAR,
	AUISUBDIALOG_RECT_MAX
};
#define AUISUBDIALOG_RECT_NUM			AUISUBDIALOG_RECT_MAX

enum
{
	AUISUBDIALOG_STATE_NORMAL = 0,
	AUISUBDIALOG_STATE_PRESSUP,
	AUISUBDIALOG_STATE_PRESSDOWN,
	AUISUBDIALOG_STATE_PRESSVBAR,
	AUISUBDIALOG_STATE_PRESSLEFT,
	AUISUBDIALOG_STATE_PRESSRIGHT,
	AUISUBDIALOG_STATE_PRESSHBAR
};

class AUISubDialog : public AUIObject
{
public:
	AUISubDialog();
	virtual ~AUISubDialog();

	virtual bool InitXML(A3DEngine *pA3DEngine, A3DDevice *pA3DDevice, AXMLItem *pItem);
	virtual bool InitIndirect(const char *pszFileName, int idType);
	virtual bool Release(void);
	virtual bool SaveXML(AXMLItem *pXMLItemDialog);
	virtual bool Tick(void);
	virtual bool Render(void);
	virtual void Show(bool bShow);
	virtual bool OnDlgItemMessage(UINT msg, WPARAM wParam, LPARAM lParam);
	virtual bool GetProperty(const	char *pszPropertyName, PAUIOBJECT_SETPROPERTY Property);
	virtual bool SetProperty(const	char *pszPropertyName, PAUIOBJECT_SETPROPERTY Property);
	virtual bool IsInHitArea(int x, int y, UINT msg = WM_LBUTTONDOWN);
	virtual bool Resize();
	virtual void SetSize(int W, int H);
	virtual void SetDefaultSize(int W, int H);
	virtual void SetPos(int x, int y);
	virtual void SetDefaultPos(int x, int y);
	virtual void OptimizeSpriteResource(bool bLoad);
	virtual void PrintOptimizeResource()const;

	void SetDialog(PAUIDIALOG pDlg);
	bool LoadDialog(const char* pszTempName);
	PAUIDIALOG GetSubDialog()	{ return m_pSubDialog; }
	void SetSubDialogOrgSize(int nWidth, int nHeight, bool bReset = true);
	void RemoveDialog();
	void UpdateViewport();
	A3DViewport* GetSubDialogViewport()			{ return m_pViewport; }
	void SetRenderVScroll(bool bRenderVScroll)	{ m_bRenderVScroll = bRenderVScroll; }
	void SetRenderHScroll(bool bRenderHScroll)	{ m_bRenderHScroll = bRenderHScroll; }
	int GetVBarPos() const { return m_nVBarPos; }
	int GetHBarPos() const { return m_nHBarPos; }
	int GetVBarMax() const { return m_nVBarMax; }
	int GetHBarMax() const { return m_nHBarMax; }
	void SetVBarPos(int nPos);
	void SetHBarPos(int nPos);
	void ForceRenderHVScroll(bool bForce = true);

protected:
	bool m_bRenderVScroll;
	bool m_bRenderHScroll;
	int m_nState;
	POINT m_ptVBar;
	POINT m_ptHBar;
	bool m_bDraggingVScroll;
	bool m_bDraggingHScroll;
	int m_nDragDist;
	int m_nHBarPos;
	int m_nHBarMax;
	int m_nVBarPos;
	int m_nVBarMax;
	int m_nVBarHeight;
	int m_nHBarWidth;
	int m_nScrollDelay;
	A3DViewport *m_pViewport;

	int m_nFrameMode;
	PAUIFRAME m_pVBarFrame;
	PAUIFRAME m_pHBarFrame;
	A2DSprite *m_pA2DSprite[AUISUBDIALOG_RECT_NUM];
	A3DRECT m_rcImage[AUISUBDIALOG_RECT_NUM];
	AString m_szDialogFileName;
	PAUIDIALOG m_pSubDialog;
	int m_nSubDialogOrgWidth;
	int m_nSubDialogOrgHeight;
	int m_nSubDialogOrgDefaultWidth;
	int m_nSubDialogOrgDefaultHeight;
	bool m_bIsLoad;
	bool m_bForceRenderScroll;

	bool RenderVScroll();
	bool RenderHScroll();
	void UpdateSubDialog(bool bNewDialog = true);
};

typedef AUISubDialog * PAUISUBDIALOG;

#endif	//	_AUISUBDIALOG_H_
