/*
 * FILE: EC_LoginUIMan.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/8/27
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "AFI.h"
#include "AIniFile.h"

#include "EC_CDS.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_IvtrTypes.h"
#include "EC_LoginUIMan.h"
#include "EC_ManPlayer.h"
#include "EC_ShadowRender.h"
#include "EC_World.h"
#include "EC_Viewport.h"
#include "EC_LoginPlayer.h"
#include "EC_Configs.h"
#include "EL_BackMusic.h"
#include "EC_Resource.h"
#include "EC_CommandLine.h"
#include "CSplit.h"
#include "EC_Utility.h"
#include "EC_GFXCaster.h"
#include "EC_UIConfigs.h"
#include "EC_TimeSafeChecker.h"

#include "gnetdef.h"

#include "A3DEngine.h"
#include "A2DSprite.h"
#include "A3DCamera.h"
#include "A3DViewport.h"
#include "A3DFont.h"
#include "A3DSkinRender.h"
#include "A3DSkinMan.h"
#include "A3DCollision.h"
#include "A3DFuncs.h"
#include "A3DTerrainWater.h"
#include <AFilePackMan.h>

#include "A3DGFXEx.h"
#include "A3DGFXExMan.h"

#include "DlgCreate.h"
#include "EC_ProfConfigs.h"
#include "EC_ProfDisplay.h"

#include "elementdataman.h"

#include "roleinfo"

#define new A_DEBUG_NEW

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	
//	Reference to External variables and functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Local Types and Variables and Global variables
//	
///////////////////////////////////////////////////////////////////////////

static const A3DVECTOR3 s_camPosDelta[NUM_PROFESSION][2] = 
{
	A3DVECTOR3(0.0f, 0.2f, 0.0f), A3DVECTOR3(0.0f),
	A3DVECTOR3(0.0f, 0.2f, 0.0f), A3DVECTOR3(0.0f),
	A3DVECTOR3(0.0f, 0.2f, 0.0f), A3DVECTOR3(0.0f),
	A3DVECTOR3(0.0f), A3DVECTOR3(0.0f),
	A3DVECTOR3(0.0f), A3DVECTOR3(0.0f),
	A3DVECTOR3(0.0f, 0.2f, 0.0f), A3DVECTOR3(0.0f),
	A3DVECTOR3(0.0f, 0.2f, 0.0f), A3DVECTOR3(0.0f),
	A3DVECTOR3(0.0f, 0.2f, 0.0f), A3DVECTOR3(0.0f),
	A3DVECTOR3(0.0f, 0.2f, 0.0f), A3DVECTOR3(0.0f),
	A3DVECTOR3(0.0f, 0.2f, 0.0f), A3DVECTOR3(0.0f),
	A3DVECTOR3(0.0f, 0.2f, 0.0f), A3DVECTOR3(0.0f),
	A3DVECTOR3(0.0f, 0.2f, 0.0f), A3DVECTOR3(0.0f),
};

static bool s_bNeedTestRoleAutoLogin = true;

///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECLoginUIMan
//	
///////////////////////////////////////////////////////////////////////////

CECLoginUIMan::CECLoginUIMan()
{
	m_curScene = LOGIN_SCENE_NULL;

	m_idCurRole = -1;
	m_roleDisplayActionTrigger = new ActionTrigger;

	m_nCurProfession = m_nCurGender = -1;
	memset(m_aModel, 0, sizeof(m_aModel));
	m_bNewCharEquipShow = true;

	m_bReferrerBound = false;

	m_bSelectCharDragRole = false; // 登陆选择角色界面旋转人物标志

	m_roleListReady = false;
	m_bSelectRoleSent = false;
}

CECLoginUIMan::~CECLoginUIMan()
{
}

//	Initialize manager
bool CECLoginUIMan::Init(A3DEngine* pA3DEngine, A3DDevice* pA3DDevice, const char* szDCFile/* NULL */)
{
	int i;
	bool bval;
	float x, y, z;
	AIniFile theIni;
	char szKey[40], szFile[MAX_PATH];

	sprintf(szFile, "%s\\Configs\\SceneCtrl.ini", af_GetBaseDir());
	theIni.Open(szFile);

	for( i = 0; i < LOGIN_SCENE_MAX; i++ )
	{
		sprintf(szKey, "PosX%d", i);
		x = theIni.GetValueAsFloat("Camera", szKey, 0.0f);
		sprintf(szKey, "PosY%d", i);
		y = theIni.GetValueAsFloat("Camera", szKey, 0.0f);
		sprintf(szKey, "PosZ%d", i);
		z = theIni.GetValueAsFloat("Camera", szKey, 0.0f);
		m_aCamPos[i] = A3DVECTOR3(x, y, z);

		sprintf(szKey, "DirX%d", i);
		x = theIni.GetValueAsFloat("Camera", szKey, 0.0f);
		sprintf(szKey, "DirY%d", i);
		y = theIni.GetValueAsFloat("Camera", szKey, 0.0f);
		sprintf(szKey, "DirZ%d", i);
		z = theIni.GetValueAsFloat("Camera", szKey, 0.0f);
		m_aCamDir[i] = A3DVECTOR3(x, y, z);

		sprintf(szKey, "UpX%d", i);
		x = theIni.GetValueAsFloat("Camera", szKey, 0.0f);
		sprintf(szKey, "UpY%d", i);
		y = theIni.GetValueAsFloat("Camera", szKey, 0.0f);
		sprintf(szKey, "UpZ%d", i);
		z = theIni.GetValueAsFloat("Camera", szKey, 0.0f);
		m_aCamUp[i] = A3DVECTOR3(x, y, z);
	}

	for( i = 0; i < LOGIN_SCENE_CREATE_END - LOGIN_SCENE_CREATE_BEGIN + 1; i++ )
	{
		sprintf(szKey, "PosX%d", i);
		x = theIni.GetValueAsFloat("NewChar", szKey, 0.0f);
		sprintf(szKey, "PosY%d", i);
		y = theIni.GetValueAsFloat("NewChar", szKey, 0.0f);
		sprintf(szKey, "PosZ%d", i);
		z = theIni.GetValueAsFloat("NewChar", szKey, 0.0f);
		m_aCreatePos[i] = A3DVECTOR3(x, y, z);
	}
	{
		sprintf(szKey, "PosX%d", 0);
		x = theIni.GetValueAsFloat("NewCharCenter", szKey, 0.0f);
		sprintf(szKey, "PosY%d", 0);
		y = theIni.GetValueAsFloat("NewCharCenter", szKey, 0.0f);
		sprintf(szKey, "PosZ%d", 0);
		z = theIni.GetValueAsFloat("NewCharCenter", szKey, 0.0f);
		m_aCreateDisplayCenter = A3DVECTOR3(x, y, z);
		m_aCreateDisplayRadius = theIni.GetValueAsFloat("NewCharCenter", "Radius", RotateAnimator::RA_DEFAULT_RADIUS);
		m_aCreateDeltaDist	= theIni.GetValueAsFloat("NewCharCenter", "DeltaDist", RotateAnimator::RA_DEFAULT_DELTA_DIST);
		m_aCreateSelectMove = theIni.GetValueAsFloat("NewCharCenter", "SelectMove", RotateAnimator::RA_DEFAULT_SELECT_MOVE);
		RotateAnimator::Instance().SetCenter(m_aCreateDisplayCenter);
		RotateAnimator::Instance().SetRadius(m_aCreateDisplayRadius);
		RotateAnimator::Instance().SetDeltaDist(m_aCreateDeltaDist);
		RotateAnimator::Instance().SetSelectMove(m_aCreateSelectMove);
	}
	for (i = 0; i < NUM_PROFESSION; ++ i)
	{
		sprintf(szKey, "Weapon%d", i);
		m_aCreateEquips[i][NEWCHAR_WEAPON] = theIni.GetValueAsInt("NewCharEquip", szKey, 0);
		sprintf(szKey, "UpperBody%d", i);
		m_aCreateEquips[i][NEWCHAR_UPPERBODY] = theIni.GetValueAsInt("NewCharEquip", szKey, 0);
		sprintf(szKey, "LowerBody%d", i);
		m_aCreateEquips[i][NEWCHAR_LOWERBODY] = theIni.GetValueAsInt("NewCharEquip", szKey, 0);
		sprintf(szKey, "Wrist%d", i);
		m_aCreateEquips[i][NEWCHAR_WRIST] = theIni.GetValueAsInt("NewCharEquip", szKey, 0);
		sprintf(szKey, "Foot%d", i);
		m_aCreateEquips[i][NEWCHAR_FOOT] = theIni.GetValueAsInt("NewCharEquip", szKey, 0);
		sprintf(szKey, "Wing%d", i);
		m_aCreateEquips[i][NEWCHAR_WING] = theIni.GetValueAsInt("NewCharEquip", szKey, 0);
	}
	
	theIni.Close();

	// force to load the default setting
	ImportStringTable("loginui.stf");
	ImportUIParam("loginui.xml");

	bval = CECBaseUIMan::Init(pA3DEngine, pA3DDevice, szDCFile);
	if( !bval ) return AUI_ReportError(__LINE__, __FILE__);	

	LoadRaceDisplayModels();
			
	PAUIDIALOG pDlgCreateRace = GetDialog("Win_CreateRace");
	for (i = 0; i < NUM_RACE; ++ i)
	{
		if (PAUISTILLIMAGEBUTTON pBtn_Race = dynamic_cast<PAUISTILLIMAGEBUTTON>(pDlgCreateRace->GetDlgItem(AString().Format("Btn_Race%d", i)))){
			pBtn_Race->SetText(GetStringFromTable(311+i));
			pBtn_Race->SetPushLike(true);
		}
	}
	pDlgCreateRace->GetDlgItem("Img_Gfx1")->SetTransparent(true);	//	遮挡住 Win_CreateProfession 时仍能透过
	pDlgCreateRace->GetDlgItem("Img_Gfx2")->SetTransparent(true);
	SetLinearFilter("Win_CreateRace");

	return true;
}

//	Release manager
bool CECLoginUIMan::Release(void)
{
	int i, j;

	for (i = 0; i < int(m_vecRoleModel.size()); i++)
		A3DRELEASE(m_vecRoleModel[i]);
	m_vecRoleModel.clear();

	delete m_roleDisplayActionTrigger;
	m_roleDisplayActionTrigger = NULL;
	
	for (i = 0; i < NUM_PROFESSION; i++)
	{
		for (j = 0; j < NUM_GENDER; j++)
			A3DRELEASE(m_aModel[i][j]);
	}
	m_bNewCharEquipShow = true;

	return CECBaseUIMan::Release();
}

//	On command
bool CECLoginUIMan::OnCommand(const char* szCommand, AUIDialog* pDlg)
{
	if (CECBaseUIMan::OnCommand(szCommand, pDlg))
		return true;

	if (!stricmp(pDlg->GetName(), "Win_CreateRace") ||
		strstr(pDlg->GetName(), "Win_RaceInfo") == pDlg->GetName()){
		return CECBaseUIMan::OnCommand(szCommand, GetDialog("Win_CreateProfession"));
	}

	return false;
}

//	Handle event
bool CECLoginUIMan::OnEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, AUIDialog* pDlg, AUIObject* pObj)
{
	if (CECBaseUIMan::OnEvent(uMsg, wParam, lParam, pDlg, pObj))
		return true;
	return true;
}

bool CECLoginUIMan::LaunchPreface()
{
	SwitchToCreate(NULL);	
	return true;
}

bool CECLoginUIMan::Tick(DWORD dwDeltaTime)
{
	TickRaceModels(dwDeltaTime);
	return CECBaseUIMan::Tick(dwDeltaTime);
}

bool CECLoginUIMan::UIControlCursor()
{
	return false;
}

A3DCOLOR CECLoginUIMan::GetMsgBoxColor()
{
	return A3DCOLORRGBA(188, 255, 255, 160);
}
void CECLoginUIMan::ShowErrorMsg2(const ACHAR *pszMsg, const char *pszName)
{	
	MessageBox(pszName, pszMsg, MB_OK, GetMsgBoxColor());
}

void CECLoginUIMan::ChangeCameraByScene(LOGIN_SCENE scene)
{
	if (!IsSceneValid(scene)){
		ASSERT(false);
		return;
	}
	A3DCamera * pCamera = g_pGame->GetViewport()->GetA3DCamera();
	
	if( scene >= LOGIN_SCENE_BODYCUSTOMIZE_BEGIN &&
		scene <= LOGIN_SCENE_BODYCUSTOMIZE_END )
	{
		int nIndex = LOGIN_SCENE_CREATE_BEGIN + scene - LOGIN_SCENE_BODYCUSTOMIZE_BEGIN;
		pCamera->SetPos(m_aCamPos[nIndex]);
		pCamera->SetDirAndUp(m_aCamDir[nIndex], m_aCamUp[nIndex]);
	}else{
		pCamera->SetPos(m_aCamPos[scene]);
		pCamera->SetDirAndUp(m_aCamDir[scene], m_aCamUp[scene]);
	}
	
	if( scene >= LOGIN_SCENE_FACECUSTOMIZE_BEGIN && scene <= LOGIN_SCENE_FACECUSTOMIZE_END )
	{
		pCamera->SetPos(s_camPosDelta[m_pCurPlayer->GetProfession()][m_pCurPlayer->GetGender()] + pCamera->GetPos());
		
		pCamera->SetFOVSet(DEG2RAD(10.0f));
		pCamera->SetFOV(DEG2RAD(10.0f));
	}
	else
	{
		pCamera->SetFOVSet(DEG2RAD(DEFCAMERA_FOV));
		pCamera->SetFOV(DEG2RAD(DEFCAMERA_FOV));
	}
}

bool CECLoginUIMan::ChangeScene(LOGIN_SCENE scene)
{
	ChangeCameraByScene(scene);
	m_curScene = scene;
	if (scene == LOGIN_SCENE_CREATE_CHOOSE_PROFESSION){
		SwitchShowModelEquip(true);
		DisplayActionTrigger::Instance().Rest();
		if (!CECProfConfig::Instance().IsProfession(m_nCurProfession)){
			//	第一次使用创建
			A3DVECTOR3 dir = m_aCamPos[LOGIN_SCENE_CREATE_CHOOSE_PROFESSION] - m_aCreateDisplayCenter;
			RotateAnimator::Instance().SetDir(dir);
			//	默认职业将由界面计算、并调用 ChangeShowModel
		}else{
			//	直接指到最近一次选中的职业
			int race = CECProfConfig::Instance().GetRaceByProfession(m_nCurProfession);
			int order = CECProfConfig::Instance().GetRaceShowOrder(race);
			RotateAnimator::Instance().SetFirstIndicator(order);
			RefreshPlayerList();
			TickRaceModels(0);
		}
	}
	if (scene != LOGIN_SCENE_SELCHAR){
		if( m_bSelectCharDragRole )
		{
			m_bSelectCharDragRole = false;
			g_pGame->ShowCursor(true);
		}
	}

	// for update terrain water's current water height data
	g_pGame->GetGameRun()->GetWorld()->GetTerrainWater()->UpdateWaterHeight();

	return RefreshPlayerList();
}

bool CECLoginUIMan::RefreshPlayerList()
{
	CECPlayerMan::LoginPlayerTable& players = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetLoginPlayerTable();

	players.clear();

	while (true)
	{
		if (m_curScene == LOGIN_SCENE_SELCHAR)
		{
			if( m_idCurRole >= 0 )
				players.push_back(m_vecRoleModel[m_idCurRole]);
			break;
		}

		if (m_curScene >= LOGIN_SCENE_CREATE_BEGIN &&
			m_curScene <= LOGIN_SCENE_CREATE_END)
		{
			if( m_aModel[m_nCurProfession][m_nCurGender] )
				players.push_back(m_aModel[m_nCurProfession][m_nCurGender]);
			break;
		}
		
		if (m_curScene == LOGIN_SCENE_CREATE_CHOOSE_PROFESSION)
		{
			for (int i = 0; i < NUM_PROFESSION; ++ i)
			{
				for (int j = 0; j < NUM_GENDER; ++ j)
				{
					if (!CECProfConfig::Instance().CanShowOnCreate(i, j) ||
						m_aModel[i][j] == NULL){
						continue;
					}
					players.push_back(m_aModel[i][j]);
				}
			}
			break;
		}

		if (m_curScene >= LOGIN_SCENE_FACECUSTOMIZE_BEGIN &&
			m_curScene <= LOGIN_SCENE_FACECUSTOMIZE_END	||
			m_curScene >= LOGIN_SCENE_BODYCUSTOMIZE_BEGIN &&
			m_curScene <= LOGIN_SCENE_BODYCUSTOMIZE_END)
		{
			if( m_pCurPlayer )
				players.push_back((CECLoginPlayer *)m_pCurPlayer);
			break;
		}

		break;
	}

	return true;
}

bool CECLoginUIMan::IsSceneValid(LOGIN_SCENE scene)const{
	return scene >= 0 && scene < LOGIN_SCENE_MAX;
}

namespace
{
#define DEFINE_DIALOG_CREATOR(dialogclass) static PAUIDIALOG wmgj_new_##dialogclass(void){ return new dialogclass; }
#define MAP_FILE_TO_CREATOR(file, dialogclass) s_mDialogCreator[file] = &wmgj_new_##dialogclass; s_mDialogCreator["version01\\"##file] = &wmgj_new_##dialogclass;

	//	定义各对话框创建函数
	DEFINE_DIALOG_CREATOR(CDlgCreateProfession)

	//	声明从界面文件名创建对话框的函数类型
	typedef	PAUIDIALOG	(*DialogCreatorFunc)(void);
	
	//	声明从界面文件名到对话框创建函数的映射
	typedef abase::hash_map<ConstChar, DialogCreatorFunc, ConstCharHashFunc>	DialogCreatorMap;
	
	//	定义映射函数
	static DialogCreatorFunc FindDialogCreator(const AString &strFileName)
	{
		static DialogCreatorMap	s_mDialogCreator;
		
		static bool	s_init(false);
		if (!s_init)
		{
			s_init = true;
			
			//	初始化映射函数
			MAP_FILE_TO_CREATOR("createprofession.xml", CDlgCreateProfession)
		}
		
		AString strTemp = strFileName;
		strTemp.MakeLower();
		DialogCreatorMap::iterator it = s_mDialogCreator.find((const char *)strTemp);
		return (it != s_mDialogCreator.end()) ? it->second : NULL;
	}
}

PAUIDIALOG CECLoginUIMan::CreateDlgInstance(const AString strTemplName)
{
	DialogCreatorFunc f = FindDialogCreator(strTemplName);
	return (f != NULL) ? f() : new CDlgTheme;
}

void CECLoginUIMan::CalcWindowScale()
{
	CECConfigs *pConfig = g_pGame->GetConfigs();
	if (!pConfig || pConfig->GetSystemSettings().bScaleUI)
	{
		// 如果选项中允许缩放，则调用基类默认处理缩放的方法
		CECBaseUIMan::CalcWindowScale();
		a_Clamp(m_fWindowScale, 0.78125f, 1.0f);
	}
	else
	{
		// 否则，保持不缩放
		m_fWindowScale = 1.0f;
	}
}

void CECLoginUIMan::RearrangeWindows(A3DRECT rcOld, A3DRECT rcNew)
{
	CECBaseUIMan::RearrangeWindows(rcOld, rcNew);
	ScaleDialogToAllVisibleByHeight("Win_CreateRace");
	RepositionAnimation();
}

bool CECLoginUIMan::ChangeLayout(const char *pszFilename)
{
	bool ret = CECBaseUIMan::ChangeLayout(pszFilename);

	if(ret)
	{
		SetWindowScale(GetWindowScale());

		// 界面可能还需要一些对齐调整（设置相同的 rect）	
		A3DVIEWPORTPARAM param = *g_pGame->GetViewport()->GetA3DViewport()->GetParam();
		A3DRECT rcNew(param.X, param.Y, param.X+param.Width, param.Y+param.Height);
		RearrangeWindows(GetRect(), rcNew);
	}

	return ret;
}

bool CECLoginUIMan::LoadShowModel(int prof, int gender)
{
	if (CECProfConfig::Instance().IsExist(prof, gender)){
		CECLoginPlayer * & pPlayer = m_aModel[prof][gender];
		if (!pPlayer){
			pPlayer = new CECLoginPlayer(NULL);			
			GNET::RoleInfo Info;
			Info.race = CECProfConfig::Instance().GetRaceByProfession(prof);
			Info.occupation = prof;
			Info.gender = gender;
			CECPlayer::PLAYER_CUSTOMIZEDATA custom_data;
			if (CECPlayer::LoadDefaultCustomizeData(prof, gender, custom_data)){
				Info.custom_data.swap(GNET::Octets((const void *)&custom_data, sizeof(custom_data)));
			}
			if (!pPlayer->Load(Info)){
				assert(false);
				A3DRELEASE(pPlayer);
				a_LogOutput(1, "CECLoginUIMan::LoadShowModel, Failed to Load roleinfo for prof=%d, gender=%d", prof, gender);
			}else{
				if (m_bNewCharEquipShow){
					SwitchShowModelEquip(prof, gender, true);
				}
			}
		}
		return pPlayer != NULL;
	}
	return false;
}


bool CECLoginUIMan::IsShownNewModelEquip()
{
	return m_bNewCharEquipShow;
}

void CECLoginUIMan::SwitchShowModelEquip(bool bShow)
{
	m_bNewCharEquipShow = bShow;
	for (int i = 0; i < NUM_PROFESSION; ++ i)
	{
		for (int j = 0; j < NUM_GENDER; ++ j)
		{
			if (CECProfConfig::Instance().IsExist(i, j)){
				SwitchShowModelEquip(i, j, bShow);
			}
		}
	}
}

void CECLoginUIMan::SwitchShowModelEquip(int prof, int gender, bool bShow)
{
	if (!CECProfConfig::Instance().IsExist(prof, gender)){
		assert(false);
		return;
	}
	CECLoginPlayer *pPlayer = m_aModel[prof][gender];
	if (!pPlayer){
		return;
	}
	int	aEquips[SIZE_ALL_EQUIPIVTR] = {0};
	if (bShow){
		int equipPos[NEWCHAR_NUM_EQUIP] = {
			EQUIPIVTR_WEAPON,
				EQUIPIVTR_BODY,
				EQUIPIVTR_LEG,
				EQUIPIVTR_WRIST,
				EQUIPIVTR_FOOT,
				EQUIPIVTR_FLYSWORD,
		};
		for (int i(0); i < NEWCHAR_NUM_EQUIP; ++ i)
		{
			int id = m_aCreateEquips[prof][i];
			if (id > 0){
				aEquips[equipPos[i]] = id;
			}
		}
	}
	pPlayer->ShowEquipments(aEquips, true);
	if (pPlayer->HasWingModel()){
		pPlayer->ShowWing(true);
	}
	if (GetCurScene() >= LOGIN_SCENE_FACECUSTOMIZE_BEGIN && GetCurScene() <= LOGIN_SCENE_FACECUSTOMIZE_END ||
		GetCurScene() >= LOGIN_SCENE_CREATE_BEGIN && GetCurScene() <= LOGIN_SCENE_CREATE_END ||
		GetCurScene() >= LOGIN_SCENE_BODYCUSTOMIZE_BEGIN && GetCurScene() <= LOGIN_SCENE_BODYCUSTOMIZE_END){
		pPlayer->StandForCustomize();
	}else{
		pPlayer->Stand(true);
	}
}

void CECLoginUIMan::ChangeShowModel(int prof, int gender)
{
	//	改变当前显示到最前端的模型（对应 LOGIN_SCENE_CREATE_CHOOSE_PROFESSION）、或当前显示的模型
	//	参数 prof gender 必须合法
	//	m_nCurProfession m_nCurGender 可以非法
	if (!CECProfConfig::Instance().IsExist(prof, gender)){
		assert(false);
		return;
	}
	int lastProfession(m_nCurProfession), lastGender(m_nCurGender);
	m_nCurProfession = prof;
	m_nCurGender = gender;
	if (GetCurScene() == LOGIN_SCENE_CREATE_CHOOSE_PROFESSION){
		int race = CECProfConfig::Instance().GetRaceByProfession(prof);
		int order = CECProfConfig::Instance().GetRaceShowOrder(race);
		if (!CECProfConfig::Instance().IsProfession(lastProfession)){
			//	设置显示到最前方的职业
			RotateAnimator::Instance().SetFirstIndicator(order);	//	保证 RotateAnimator::GetIndicatorPos(order) 会获取到内部0的位置，即对应最前的位置
		}else{
			RotateAnimator::Instance().RotateToIndicator(order);
		}
		DisplayActionTrigger::Instance().Trigger(prof);
		RefreshPlayerList();
		TickRaceModels(0);
	}
}

void CECLoginUIMan::SwitchToCreate(CECPlayer* player)
{
	//	初入创建角色
	m_nCurProfession = -1;
	m_nCurGender = -1;
	DisplayActionTrigger::Instance().Reset();
	PAUIDIALOG pDlgCreateProfession = GetDialog("Win_CreateProfession");
	pDlgCreateProfession->Show(true);
}

void CECLoginUIMan::LoadRaceDisplayModels()
{
	//	加载所有需展示模型
	for (int i = 0; i < NUM_PROFESSION; ++ i)
	{
		for (int j = 0; j < NUM_GENDER; ++ j)
		{
			if (CECProfConfig::Instance().CanShowOnCreate(i, j)){
				LoadShowModel(i, j);
			}
		}
	}
}

void CECLoginUIMan::TickRaceModels(DWORD dwDeltaTime)
{
	if (GetCurScene() != LOGIN_SCENE_CREATE_CHOOSE_PROFESSION){
		return;
	}
	RotateAnimator::Instance().Tick(dwDeltaTime);
	DisplayActionTrigger::Instance().Tick(dwDeltaTime);
	for (int i = 0; i < NUM_PROFESSION; ++ i)
	{
		bool moveFrontTriggered = DisplayActionTrigger::Instance().MoveFrontActionTriggered(i);
		bool goBackTriggered = DisplayActionTrigger::Instance().GoBackActionTriggered(i);
		bool displayActionTriggered = DisplayActionTrigger::Instance().DisplayActionTriggered(i);
		bool standActionTriggered = DisplayActionTrigger::Instance().StandActionTriggered(i);
		A3DVECTOR3 pos(0.0f), dir(g_vAxisY);
		DisplayActionTrigger::Instance().GetPosDir(i, pos, dir);
		for (int j = 0; j < NUM_GENDER; ++ j)
		{
			if (!CECProfConfig::Instance().CanShowOnCreate(i, j) ||
				m_aModel[i][j] == NULL){
				continue;
			}
			CECLoginPlayer *pPlayer = m_aModel[i][j];
			pPlayer->SetPos(pos);
			pPlayer->ChangeModelMoveDirAndUp(dir, g_vAxisY);
			if (moveFrontTriggered){
				pPlayer->MoveFront();
			}else if (goBackTriggered){
				pPlayer->GoBack();
			}else if (displayActionTriggered){
				pPlayer->ShowDisplayAction();
			}else if (standActionTriggered){
				pPlayer->Stand(true);
			}
			pPlayer->Tick(0);
		}
	}
}

void CECLoginUIMan::ResetCursor()
{
	if( m_bSelectCharDragRole )
	{	// Restore cursor position to start position
		POINT pt = {m_ptDragStart.x, m_ptDragStart.y};
		::ClientToScreen(g_pGame->GetGameInit().hWnd, &pt);
		::SetCursorPos(pt.x, pt.y);
	}
}