// File		: DlgCreate.h
// Creator	: Feng Ning
// Date		: 2010/10/20

#pragma once

#include "DlgBase.h"

//	—°‘Ò÷∞“µ
class CDlgCreateProfession : public CDlgBase 
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP();
	
public:
	CDlgCreateProfession();

	void OnCommand_Race(const char *szCommand);
	void OnCommand_Prof(const char *szCommand);
	void OnCommand_Confirm(const char *szCommand);
	void OnCommand_Cancel(const char *szCommand);
	void OnCommand_PrevRace(const char *szCommand);
	void OnCommand_NextRace(const char *szCommand);
	
protected:
	virtual void OnShowDialog();
	virtual void OnHideDialog();
	virtual void OnTick();
	
	void OnEventLButtonDblClick_Prof(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	void SendCmdChooseProfession(int prof);
};
