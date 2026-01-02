// File		: DlgCreate.cpp
// Creator	: Feng Ning
// Date		: 2010/10/20

#include "DlgCreate.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_LoginUIMan.h"
#include "EC_ProfConfigs.h"
#include "EC_Configs.h"

#include <AUIStillImageButton.h>
#include <AUIImagePicture.h>

#define new A_DEBUG_NEW

//	CDlgCreateProfession
AUI_BEGIN_COMMAND_MAP(CDlgCreateProfession, CDlgBase)
AUI_ON_COMMAND("Btn_Prof*", OnCommand_Prof)
AUI_ON_COMMAND("Btn_Race*", OnCommand_Race)
AUI_ON_COMMAND("Btn_Confirm", OnCommand_Confirm)
AUI_ON_COMMAND("IDCANCEL", OnCommand_Cancel)
AUI_ON_COMMAND("Btn_PrevRace", OnCommand_PrevRace)
AUI_ON_COMMAND("Btn_NextRace", OnCommand_NextRace)
AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgCreateProfession, CDlgBase)
AUI_ON_EVENT("Btn_Prof*", WM_LBUTTONDBLCLK, OnEventLButtonDblClick_Prof)
AUI_END_EVENT_MAP()

CDlgCreateProfession::CDlgCreateProfession()
{
}

void CDlgCreateProfession::OnShowDialog()
{
	CECLoginUIMan* pUIMan = GetLoginUIMan();
	pUIMan->ChangeScene(CECLoginUIMan::LOGIN_SCENE_CREATE_CHOOSE_PROFESSION);

	PAUIDIALOG pDlgCreateRace = pUIMan->GetDialog("Win_CreateRace");
	if (!pDlgCreateRace->IsShow()){
		pDlgCreateRace->Show(true);
	}

	//	选中上次选中种族或默认种族
	int lastProf = pUIMan->GetCurProfession();
	int race(-1), prof(lastProf);
	if (lastProf < 0){
		//	找出优先显示的种族
		int order(INT_MAX);
		for (int r(0); r < NUM_RACE; ++ r)
		{
			int o = CECProfConfig::Instance().GetRaceShowOrder(r);
			if (o < order){
				order = o;
				race = r;
			}
		}
	}else{
		race = CECProfConfig::Instance().GetRaceByProfession(lastProf);
	}
	AString strCmd;
	strCmd.Format("Btn_Race%d", race);
	pDlgCreateRace->OnCommand(strCmd);

	CDlgBase::OnShowDialog();
}

void CDlgCreateProfession::SendCmdChooseProfession(int prof)
{
	if (!CECProfConfig::Instance().IsProfession(prof)){
		assert(false);
		return;
	}
	AString strName;
	for (int i(0); i < 2; ++ i)
	{
		strName.Format("Btn_Prof%d", i);
		PAUIOBJECT pBtn_Prof = GetDlgItem(strName);
		if (!pBtn_Prof){
			break;
		}
		if ((int)pBtn_Prof->GetData() == prof){
			this->OnCommand(strName);
			break;
		}
	}
}

void CDlgCreateProfession::OnHideDialog()
{
	PAUIMANAGER pUIMan = GetAUIManager();
	PAUIDIALOG pDlgCreateRace = pUIMan->GetDialog("Win_CreateRace");
	if (pDlgCreateRace->IsShow()){
		pDlgCreateRace->Show(false);
	}
	
	PAUIDIALOG pDlgCreateButton1 = pUIMan->GetDialog("Win_CreateButton1");
	if (pDlgCreateButton1->IsShow()){
		pDlgCreateButton1->Show(false);
	}
	
	AString strName;
	for (int r(0); r < NUM_RACE; ++ r)
	{
		strName.Format("Win_RaceInfo%d", r);
		PAUIDIALOG pDlgRaceInfo = pUIMan->GetDialog(strName);
		if (pDlgRaceInfo && pDlgRaceInfo->IsShow()){
			pDlgRaceInfo->Show(false);
		}
	}
	CDlgBase::OnHideDialog();
}

void CDlgCreateProfession::OnTick()
{
	PAUIMANAGER pUIMan = GetAUIManager();
	PAUIDIALOG pDlgMouseOn(NULL);
	PAUIOBJECT pObjMouseOn(NULL);
	pUIMan->GetMouseOn(&pDlgMouseOn, &pObjMouseOn);
	int profHover = -1;
	bool atLeft(false);
	AString strName;
	if (pDlgMouseOn == this){
		for (int i(0); i < 2; ++ i)
		{
			PAUIOBJECT pObj = GetDlgItem(strName.Format("Btn_Prof%d", i));
			if (pObj != pObjMouseOn){
				continue;
			}
			profHover = (int)pObj->GetData();
			atLeft = (i == 0);
			break;
		}
	}
}

void CDlgCreateProfession::OnCommand_Race(const char* szCommand)
{
	int race = atoi(szCommand + strlen("Btn_Race"));
	if (race < 0 || race >= NUM_RACE){
		assert(false);
		return;
	}
	CECLoginUIMan * pUIMan = GetLoginUIMan();
	AString strName;

	PAUIDIALOG pDlgCreateRace = pUIMan->GetDialog("Win_CreateRace");
	for (int i = 0; i < NUM_RACE; ++ i)
	{
		if (PAUISTILLIMAGEBUTTON pBtn = dynamic_cast<PAUISTILLIMAGEBUTTON>(pDlgCreateRace->GetDlgItem(strName.Format("Btn_Race%d", i)))){
			pBtn->SetPushed(i == race);
		}
	}

	//	显示种族介绍
	for (int r(0); r < NUM_RACE; ++ r)
	{
		PAUIDIALOG pDlgRaceInfo = pUIMan->GetDialog(strName.Format("Win_RaceInfo%d", r));
		bool bShouldShow = (r == race);
		if (pDlgRaceInfo && pDlgRaceInfo->IsShow() != bShouldShow){
			pDlgRaceInfo->Show(bShouldShow);
		}
	}

	int lastProf = pUIMan->GetCurProfession();
	if (CECProfConfig::Instance().IsProfession(lastProf) &&
		CECProfConfig::Instance().GetRaceByProfession(lastProf) == race){
		return;
	}

	//	显示此种族可选择的职业
	int prof(-1), order(INT_MAX);
	for (int p(0); p < NUM_PROFESSION; ++ p)
	{
		if (CECProfConfig::Instance().GetRaceByProfession(p) != race){
			continue;
		}
		const ACHAR *szProfName = GetGameRun()->GetProfName(p);
		int btnIndex = CECProfConfig::Instance().GetProfessionShowOrderInRace(p);
		for (int g(0); g < NUM_GENDER; ++ g)
		{
			if (CECProfConfig::Instance().CanShowOnCreate(p, g)){
				PAUIOBJECT pBtnProf = GetDlgItem(strName.Format("Btn_Prof%d", btnIndex));
				if (!pBtnProf){
					break;
				}
				pBtnProf->SetData(p);
				pBtnProf->SetHint(szProfName);
				
				PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName.Format("Img_Prof%d", btnIndex));
				pImg->FixFrame(p);
				pImg->SetHint(szProfName);
				PAUIIMAGEPICTURE pImgDisable = (PAUIIMAGEPICTURE)GetDlgItem(strName.Format("Img_Prof_Disable%d", btnIndex));
				pImgDisable->FixFrame(p);
				pImgDisable->SetHint(szProfName);
				
				PAUIOBJECT pTxtProf = GetDlgItem(strName.Format("Txt_Prof%d", btnIndex));
				pTxtProf->SetText(szProfName);
				
				PAUIOBJECT pTxtMsg = GetDlgItem(strName.Format("Txt_Msg%d", btnIndex));
				pTxtMsg->SetText(GetStringFromTable(2100+p));
				
				int o = CECProfConfig::Instance().GetProfessionShowOrderInRace(p);
				if (o < order){
					order = o;
					prof = p;
				}
			}
		}
	}
	
	//	默认选中某职业
	SendCmdChooseProfession(prof);
}

void CDlgCreateProfession::OnCommand_Prof(const char* szCommand)
{
	//	设置职业和性别
	int btnIndex = atoi(szCommand + strlen("Btn_Prof"));
	PAUIOBJECT pBtn_Prof = GetDlgItem(szCommand);
	int prof = (int)pBtn_Prof->GetData();
	if (!CECProfConfig::Instance().IsProfession(prof)){
		assert(false);
		return;
	}
	int gender = GENDER_MALE;
	for (int g(0); g < NUM_GENDER; ++ g)
	{
		if (CECProfConfig::Instance().CanShowOnCreate(prof, g)){
			gender = g;
			break;
		}
	}
	GetLoginUIMan()->ChangeShowModel(prof, gender);

	//	更新界面	
	AString strName;
	for (int i(0); i < 2; ++ i)
	{
		PAUISTILLIMAGEBUTTON pBtn_Prof = dynamic_cast<PAUISTILLIMAGEBUTTON>(GetDlgItem(strName.Format("Btn_Prof%d", i)));
		if (!pBtn_Prof){
			break;
		}
		bool bSelected = (int)pBtn_Prof->GetData() == prof;
		pBtn_Prof->SetPushed(bSelected);		
		PAUIOBJECT pImg = GetDlgItem(strName.Format("Img_Prof%d", i));
		pImg->Show(bSelected);
		PAUIOBJECT pImgDisable = GetDlgItem(strName.Format("Img_Prof_Disable%d", i));
		pImgDisable->Show(!bSelected);		
		PAUIOBJECT pTxtMsg = GetDlgItem(strName.Format("Txt_Msg%d", i));
		pTxtMsg->SetAlpha(bSelected ? 255 : 64);
	}
}

void CDlgCreateProfession::OnCommand_Confirm(const char* szCommand)
{
}

void CDlgCreateProfession::OnCommand_Cancel(const char* szCommand)
{
}

void CDlgCreateProfession::OnCommand_PrevRace(const char* szCommand)
{
	CECLoginUIMan *pUIMan = GetLoginUIMan();
	int nCurProfession = pUIMan->GetCurProfession();
	if (CECProfConfig::Instance().IsProfession(nCurProfession)){
		int race = CECProfConfig::Instance().GetRaceByProfession(nCurProfession);
		int raceShowOrder = CECProfConfig::Instance().GetRaceShowOrder(race);
		int nextShowOrder = (raceShowOrder + NUM_RACE-1)%NUM_RACE;
		for (int i(0); i < NUM_RACE; ++ i)
		{
			if (i != race){
				int order = CECProfConfig::Instance().GetRaceShowOrder(i);
				if (order == nextShowOrder){
					AString strCmd;
					strCmd.Format("Btn_Race%d", i);
					pUIMan->GetDialog("Win_CreateRace")->OnCommand(strCmd);
					break;
				}
			}
		}
	}
}

void CDlgCreateProfession::OnCommand_NextRace(const char* szCommand)
{
	CECLoginUIMan *pUIMan = GetLoginUIMan();
	int nCurProfession = pUIMan->GetCurProfession();
	if (CECProfConfig::Instance().IsProfession(nCurProfession)){
		int race = CECProfConfig::Instance().GetRaceByProfession(nCurProfession);
		int raceShowOrder = CECProfConfig::Instance().GetRaceShowOrder(race);
		int nextShowOrder = (raceShowOrder + 1)%NUM_RACE;
		for (int i(0); i < NUM_RACE; ++ i)
		{
			if (i != race){
				int order = CECProfConfig::Instance().GetRaceShowOrder(i);
				if (order == nextShowOrder){
					AString strCmd;
					strCmd.Format("Btn_Race%d", i);
					pUIMan->GetDialog("Win_CreateRace")->OnCommand(strCmd);
					break;
				}
			}
		}
	}
}

void CDlgCreateProfession::OnEventLButtonDblClick_Prof(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	OnCommand_Confirm(NULL);
}