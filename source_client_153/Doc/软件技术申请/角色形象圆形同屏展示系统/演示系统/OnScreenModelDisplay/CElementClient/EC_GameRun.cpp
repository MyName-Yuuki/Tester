/*
 * FILE: EC_GameRun.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/8/27
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

//	For WM_MOUSEWHEEL message
#pragma warning (disable: 4284)

#define _WIN32_WINDOWS	0x0500

#include "EC_Global.h"
#include "EC_GPDataType.h"
#include "EC_GameRun.h"
#include "EC_UIManager.h"
#include "EC_LoginUIMan.h"
#include "EC_Game.h"
#include "EC_InputCtrl.h"
#include "EC_RTDebug.h"
#include "EC_GFXCaster.h"
#include "EC_Resource.h"
#include "EC_Viewport.h"
#include "EC_World.h"
#include "EC_Counter.h"
#include "EC_Utility.h"
#include "EC_Configs.h"
#include "EC_UIConfigs.h"
#include "EC_ManPlayer.h"
#include "EC_ManOrnament.h"
#include "EC_ManMessage.h"
#include "EC_ManDecal.h"
#include "EC_FullGlowRender.h"
#include "EL_BackMusic.h"
#include "EL_Forest.h"
#include "EL_Grassland.h"
#include "EC_SceneLoader.h"
#include "EC_SceneBlock.h"
#include "EC_FixedMsg.h"
#include "EC_SoundCache.h"
#include "EL_CloudManager.h"
#include "EC_Instance.h"
#include "EC_SunMoon.h"
#include "EC_ImageRes.h"
#include "Render.h"
#include "EC_TimeSafeChecker.h"
#include "EC_RandomMapPreProcessor.h"

#include "gnetdef.h"

#include "AFI.h"
#include "A3DEngine.h"
#include "A3DCamera.h"
#include "A3DConfig.h"
#include "A3DSkinRender.h"
#include "A3DSkinMan.h"
#include "A3DWireCollector.h"
#include "A3DFlatCollector.h"
#include "A3DGFXExMan.h"
#include "A3DSkinMeshMan.h"
#include "A3DTrackMan.h"
#include "A3DTerrain2.h"
#include "A3DFont.h"
#include "A3DViewport.h"
#include "AWString.h"
#include "AWScriptFile.h"
#include <AScriptFile.h>
#include <A2DSprite.h>
#include <AIniFile.h>
#include <A3DSurface.h>

#include "AM3DSoundDevice.h"

#include "AutoScene.h"

#include "globaldataman.h"

#include "ExpTypes.h"
#include "elementdataman.h"


#define new A_DEBUG_NEW

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

#define TIME_TICKANIMATION	30
#define TIME_UPDATEEAR		100

//	User configs data version
#define USERCFG_VERSION		3

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

//	Game statistic
struct EC_STATISTIC
{
	//	Data size counter
	int		iMeshSize;			//	Skin mesh size
	int		iAllTexSize;		//	Total texture size
	int		iStrmDSize;			//	Dynamic stream size
	int		iStrmSSize;			//	Static stream size
	int		iStrmBSize;			//	Backup stream size
	int		iTrackSize;			//	Track data size
	int		iTrnDataSize;		//	Terrain data size
	int		iSndCacheSize;		//	Current cached sound size

	//	Time counter
	int		iTickTime;			//	Total tick time
	int		iRenderTime;		//	Total render time

	//	Object counter
	int		iNumPlayer;			//	Else player number
	int		iNumNPC;			//	NPC number
	int		iNumMatter;			//	Matter number
	int		iNumScnGFX;			//	Scene GFX number
	int		iNumOnmt;			//	Ornament number
	int		iNumScnModel;		//	Scene ECModel
	float	fSndCacheHit;		//	Sound cache hit percent
	int		iNumTrnBlock;		//	Visible terrain block counter

	//	Memory statistics
	int		iMemPeak;			//	Peak memory size
	DWORD	dwMemAllocCnt;		//	Memory allocate counter
	int		iMemCurSize;		//	Current allocated memory size
	int		iMemRawSize;		//	Current allocated raw memory size
	int		aSMemBlkSize[16];	//	Small memory block size
	int		aSMemSize[16];		//	Small memory allocate size
	int		aSMemBlkCnt[16];	//	Small memory total block counter
	int		aSMemFreeCnt[16];	//	Small memory free block 
	int		iSMemBlkCnt;		//	Small memory block counter
	int		iSMemAllocSize;		//	Allocated small memory size
	int		iLMemBlkCnt;		//	Large memory block counter
	int		iLMemAllocSize;		//	Allocated large memory size
};

//	Frame controller
struct FRAMECTRL
{
	int		iTickCnt;		//	Tick counter
	int		iRenderCnt;		//	Render counter
	int		iTickTime;		//	Accumulate tick time
	int		iAvgRdTime;		//	Average render time
	int		iRenderTime;	//	Accumulate render time
};

static CECCounter		l_SaveCfgCnt;	//	Time counter used to save UI configs
static EC_STATISTIC		l_Statistic;	//	Game statistic
static CECCounter		l_StatCnt;		//	Time counter used by statistic
static FRAMECTRL		l_fc;			//	Frame controller

//	网络延迟
static bool			l_bFirstQuery;
static int			l_iDelayTimeStamp;	//	上一次发送网络延迟查询协议时的时间戳
static CECCounter	l_DelayQueryCounter;//	查询网络延迟协议的计时器

//	网络对时
static CECCounter	l_QueryServerTime;	//	查询服务器当前时间

///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECGameRun
//	
///////////////////////////////////////////////////////////////////////////

CECGameRun::CECGameRun() : 
m_ID2NameTab(1024),
m_Name2IDTab(1024),
m_InstTab(64),
m_CommonDataTab(1024),
m_bFirstShow(true)
{
	m_pLogo			= NULL;
	m_pClassification = NULL;
	m_pUIManager	= NULL;
	m_pWorld		= NULL;
	m_pInputCtrl	= NULL;
	m_iGameState	= GS_NONE;
	m_bPVPMode		= false;
	m_dwRenderTime	= 0;
	m_bUIHasCursor	= false;
	m_iCfgDataSize	= 0;
	m_pCfgDataBuf	= NULL;
	m_iLogoutFlag	= -1;
	m_pMessageMan	= NULL;
	m_iDblExpMode	= 0;
	m_iDExpEndTime	= 0;
	m_iAvaDExpTime	= 0;
	m_bLockGamePt	= false;
	m_nGShopTimeStamp = 0;
	m_nGShopTimeStamp2 = 0;

	m_pFullGlowRender = NULL;

	//	Set time counters
	l_SaveCfgCnt.SetPeriod(1200000);
	l_StatCnt.SetPeriod(1000);

	memset(&l_Statistic, 0, sizeof (l_Statistic));
	memset(&l_fc, 0, sizeof (l_fc));
	memset(&m_WallowInfo, 0, sizeof(m_WallowInfo));
	m_bAccountLoginInfoShown = true;

	m_accountInfoFlag = 0;
	m_bAccountInfoShown = true;
	
	l_bFirstQuery = true;
	m_iInGameDelay = 0;
	l_DelayQueryCounter.SetPeriod(30*1000);

	l_QueryServerTime.SetPeriod(10*60*1000);

	m_SellingRoleID = 0;
}

CECGameRun::~CECGameRun()
{
}

//	Initialize object
bool CECGameRun::Init()
{
	//	Create full glow render
	m_pFullGlowRender = new CECFullGlowRender();
	if( !m_pFullGlowRender->Init(g_pGame->GetA3DDevice()) )
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECGameRun::Init", __LINE__);
		m_pFullGlowRender->Release();
		return false;
	}

	if( m_pFullGlowRender->CanDoFullGlow() )
	{
		m_pFullGlowRender->SetGlowOn(g_pGame->GetConfigs()->GetSystemSettings().bFullGlow);
		m_pFullGlowRender->SetWarpOn(g_pGame->GetConfigs()->GetSystemSettings().bSpaceWarp);
		m_pFullGlowRender->SetFlareOn(g_pGame->GetConfigs()->GetSystemSettings().bSunFlare);
	}
	
	//	Load instance information
	if (!LoadInstanceInfo("Configs\\instance.txt"))
	{
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECGameRun::Init", __LINE__);
		return false;
	}

	if (!ImportSoundStringTable("Configs\\sound.txt"))
	{
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECGameRun::Init", __LINE__);
		return false;
	}

	//	Create UI manager
	if (!(m_pUIManager = new CECUIManager))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECGameRun::Init", __LINE__);
		return false;
	}

	if (!m_pUIManager->Init(g_pGame->GetA3DEngine(), -1))
	{
		a_LogOutput(1, "CECGameRun::Init, Failed to initialize UI manager.");
		return false;
	}

	//	Create input controller
	if (!(m_pInputCtrl = new CECInputCtrl))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECGameRun::Init", __LINE__);
		return false;
	}

	//	Create host input filter
// 	if (!(m_pAutoHomeFilter = new CECAutoHomeInputFilter(m_pInputCtrl)))
// 	{
// 		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECGameRun::Init", __LINE__);
// 		return false;
// 	}


// 	g_Render.Init();

	g_pGame->ChangeCursor(RES_CUR_NORMAL);
	g_pGame->ShowCursor(true);

	m_pRandomMapProc = NULL;

	return true;
}

//	Release object
void CECGameRun::Release()
{
	//  通知子进程退出
// 	if( g_pGame->GetConfigs()->IsMiniClient() )
// 		CECMCDownload::GetInstance().SendQuit();

	//	Release resources
	EndGameState(false);

	if (m_pCfgDataBuf)
	{
		a_free(m_pCfgDataBuf);
		m_pCfgDataBuf = NULL;
	}

	if (m_pUIManager)
	{
		m_pUIManager->Release();
		delete m_pUIManager;
		m_pUIManager = NULL;
	}

// 	delete m_pAutoHomeFilter;
// 	m_pAutoHomeFilter = NULL;

	if (m_pInputCtrl)
	{
		m_pInputCtrl->Release();
		delete m_pInputCtrl;
		m_pInputCtrl = NULL;
	}

	if (m_pFullGlowRender)
	{
		m_pFullGlowRender->Release();
		delete m_pFullGlowRender;
		m_pFullGlowRender = NULL;
	}

	//	Release all instance information
	InstTable::iterator it = m_InstTab.begin();
	for (; it != m_InstTab.end(); ++it)
	{
		CECInstance* pInst = *it.value();
		delete pInst;
	}	
	m_InstTab.clear();

	ReleaseSoundTable();

	//	Release all player name and ID cache
	ClearNameIDPairs();

	UniqueDataMap::iterator unique_itr = m_UniqueDataMap.begin();
	for (;unique_itr!=m_UniqueDataMap.end();++unique_itr)
	{
		unique_data* p = unique_itr->second;
		delete p;
	}
	m_UniqueDataMap.clear();

	if (m_pRandomMapProc)
	{
		m_pRandomMapProc->Release();
		delete m_pRandomMapProc;
		m_pRandomMapProc = NULL;
	}
}

bool CECGameRun::IsFirstShow()const
{
	return m_bFirstShow;
}

void CECGameRun::OnFirstShowEnd()
{
	m_bFirstShow = false;
}

//	Start game
bool CECGameRun::StartGame(int idInst, const A3DVECTOR3& vHostPos)
{
	return true;
}

//	End game state
void CECGameRun::OnEndGameState()
{
	ReleasePendingActions();

	//	Release UI module
	m_pUIManager->ChangeCurUIManager(-1);

	//	Release shortcuts
	ReleaseShortcuts();

	//	Release host player before world released
	ReleaseHostPlayer();

	//	Release world
	ReleaseWorld();

	//	Release message manager
	A3DRELEASE(m_pMessageMan);

	g_pGame->ReleaseInGameRes();
	
}

void CECGameRun::OnWindowSizeChange(A3DRECT rcOld, A3DRECT rcNew)
{
	//	窗口大小缩放
	AUIManager *pAUIManager = GetUIManager()->GetCurrentUIManPtr();
	if (pAUIManager)
		pAUIManager->RearrangeWindows(rcOld, rcNew);
}

//	Start login interface
bool CECGameRun::StartLogin()
{
	//	End current game state
	EndGameState();

	m_iGameState = GS_LOGIN;

	if( !CreateLoginWorld() )
	{
		a_LogOutput(1, "CECGameRun::StartLogin, Failed to create login world.");
		return false;
	}

	//	Change UI manager
	if (!m_pUIManager->ChangeCurUIManager(CECUIManager::UIMAN_LOGIN))
	{
		a_LogOutput(1, "CECGameRun::StartLogin, Failed to change UI manager.");
		return false;
	}

	m_pUIManager->GetLoginUIMan()->LaunchPreface();
		
	//	Change cursor to default icon
	g_pGame->ChangeCursor(RES_CUR_NORMAL);
	//	Discard current frame
	g_pGame->DiscardFrame();

	return true;
}

//	End login state
void CECGameRun::OnEndLoginState()
{
	//	Release UI module
	m_pUIManager->ChangeCurUIManager(-1);

	//  Release World
	ReleaseLoginWorld();

	A3DRELEASE(m_pLogo);
	A3DRELEASE(m_pClassification);
}

//	End current game state
void CECGameRun::EndGameState(bool bReset/* true */)
{
	if (m_iGameState == GS_NONE)
		return;

	int iCurState = m_iGameState;
	m_iGameState = GS_NONE;

	if (iCurState == GS_LOGIN)
		OnEndLoginState();
	else if (iCurState == GS_GAME)
		OnEndGameState();

	//	Stop background sound and music
	CELBackMusic* pBackMusic = g_pGame->GetBackMusic();
	if (pBackMusic)
	{
		pBackMusic->StopMusic(true, true);
		pBackMusic->StopBackSFX();
	}

	if (bReset)
		g_pGame->Reset();
}

//	Create world
bool CECGameRun::CreateWorld(int id, const A3DVECTOR3& vHostPos, int iParallelWorldID)
{
	CECInstance* pInst = GetInstance(id);
	if (!pInst)
	{
		a_LogOutput(1, "CECGameRun::CreateWorld, wrong instance id: %d", id);
		return false;
	}

	if (!(m_pWorld = new CECWorld(this)))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECGameRun::CreateWorld", __LINE__);
		return false;
	}

	if (!m_pWorld->Init(id, iParallelWorldID))
	{
		a_LogOutput(1, "CECGameRun::CreateWorld, Failed to initialize world.");
		return false;
	}

	//	Set scene resource loading distance
	CECSceneBlock::SetResLoadDists(g_pGame->GetConfigs()->GetSystemSettings().nSight + 1);

	//	Begin load progress
	BeginLoadProgress(0, 100);

	//	Load scene from file
	char szFile[MAX_PATH];
	sprintf(szFile, "maps\\%s\\%s.ecwld", pInst->GetPath(), pInst->GetPath());
//	sprintf(szFile, "maps\\132\\132.ecwld");
	if (!m_pWorld->LoadWorld(szFile, vHostPos))
	{
		a_LogOutput(1, "CECGameRun::CreateWorld, Failed to load world.");
		return true;
	}

	//	End load progress
	EndLoadProgress();

	// now check files md5 again;
	bool CheckMapMD5(const char *);
	if( !CheckMapMD5(pInst->GetPath()) )
	{
		a_LogOutput(1, "CECGameRun::CreateWorld, Failed to create %s because md5 error.", szFile);
		return false;
	}

	//	Set default fog parameters
	A3DDevice* pA3DDevice = g_pGame->GetA3DDevice();
	CECConfigs* pConfigs = g_pGame->GetConfigs();
	pA3DDevice->SetFogStart(pConfigs->GetDefFogStart());
	pA3DDevice->SetFogEnd(pConfigs->GetDefFogEnd());
	pA3DDevice->SetFogDensity(pConfigs->GetDefFogDensity());
	pA3DDevice->SetFogColor(pConfigs->GetDefFogColor());

	if( m_pWorld->GetForest() )
		m_pWorld->GetForest()->SetLODLevel(pConfigs->GetSystemSettings().nTreeDetail * 0.25f);
	if( m_pWorld->GetGrassLand() )
		m_pWorld->GetGrassLand()->SetLODLevel(pConfigs->GetSystemSettings().nGrassDetail * 0.25f);
	if( m_pWorld->GetCloudManager() )
		m_pWorld->GetCloudManager()->SetCloudLevel(pConfigs->GetSystemSettings().nCloudDetail * 25 / 10);

	// adjust time of the day;
	struct tm serverLocal = g_pGame->GetServerLocalTime();
	int nTimeInDay = serverLocal.tm_hour * 3600 + serverLocal.tm_min * 60 + serverLocal.tm_sec;
	m_pWorld->GetSunMoon()->SetTimeOfTheDay(nTimeInDay / (4.0f * 3600.0f));
	return true;
}

//	Release world
void CECGameRun::ReleaseWorld()
{
	m_pInputCtrl->ClearKBFilterStack();
	m_pInputCtrl->ClearMouFilterStack();

	g_pGame->GetViewport()->SwitchCamera(false);
	
	if (m_pWorld)
	{
		m_pWorld->Release();
		delete m_pWorld;
		m_pWorld = NULL;
	}
}

//	Create Login World
bool CECGameRun::CreateLoginWorld()
{
	A3DVECTOR3	vCameraPos(-874.891968, 54.800098, -280.675232);	//	2014年秋季资料片的登录场景位置

	//	获取登录界面初始位置配置文件，修改登录场景初始加载位置，使场景创建时即加载此处内容
	AIniFile theIni;
	char szFile[MAX_PATH] = {0};
	sprintf(szFile, "%s\\Configs\\SceneCtrl.ini", af_GetBaseDir());
	if (theIni.Open(szFile))
	{
		char szKey[40];
		CECLoginUIMan::LOGIN_SCENE	cameraIndex = CECLoginUIMan::LOGIN_SCENE_CREATE_CHOOSE_PROFESSION;
		sprintf(szKey, "PosX%d", cameraIndex);
		vCameraPos.x = theIni.GetValueAsFloat("Camera", szKey, 0.0f);
		sprintf(szKey, "PosY%d", cameraIndex);
		vCameraPos.y = theIni.GetValueAsFloat("Camera", szKey, 0.0f);
		sprintf(szKey, "PosZ%d", cameraIndex);
		vCameraPos.z = theIni.GetValueAsFloat("Camera", szKey, 0.0f);
		theIni.Close();
	}
	else
	{
		a_LogOutput(1, "CECGameRun::CreateLoginWorld, Failed to set initial login position.");
	}

	if (!g_pGame->LoadLoginRes())
	{
		a_LogOutput(1, "CECGameRun::CreateLoginWorld, Failed to call LoadLoginRes().");
		return false;
	}

	if (!(m_pWorld = new CECWorld(this)))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECGameRun::CreateLoginWorld", __LINE__);
		return false;
	}

	if (!m_pWorld->Init(0))
	{
		a_LogOutput(1, "CECGameRun::CreateLoginWorld, Failed to initialize world.");
		return false;
	}

	//	Set scene resource loading distance
	CECSceneBlock::SetResLoadDists(0, 300.0f);	//	2014年资料片：为减少资源占用（资源释放有BUG？），应尽量减小视野距离

	//	Begin load progress
	BeginLoadProgress(0, 100);

	//	Load login scene from file
	if (!m_pWorld->LoadWorld("Maps\\Login\\Login.ecwld", vCameraPos))
	{
		a_LogOutput(1, "CECGameRun::CreateLoginWorld, Failed to load world.");
		return false;
	}

	//	End load progress
	EndLoadProgress();

	g_pGame->GetViewport()->GetA3DCamera()->SetPos(vCameraPos);

	//	Set default fog parameters
	A3DDevice* pA3DDevice = g_pGame->GetA3DDevice();
	CECConfigs* pConfigs = g_pGame->GetConfigs();
	pA3DDevice->SetFogStart(pConfigs->GetDefFogStart());
	pA3DDevice->SetFogEnd(pConfigs->GetDefFogEnd());
	pA3DDevice->SetFogDensity(pConfigs->GetDefFogDensity());
	pA3DDevice->SetFogColor(pConfigs->GetDefFogColor());

	AString strTheme = pConfigs->GetRandomThemeFile();
	if (!strTheme.IsEmpty())
		g_pGame->GetBackMusic()->PlayMusic(strTheme, false, true);
	
	if( g_pGame->GetGameRun()->GetWorld()->GetForest() )
		g_pGame->GetGameRun()->GetWorld()->GetForest()->SetLODLevel(1.0f);
	if( g_pGame->GetGameRun()->GetWorld()->GetGrassLand() )
		g_pGame->GetGameRun()->GetWorld()->GetGrassLand()->SetLODLevel(1.0f);
	if( g_pGame->GetGameRun()->GetWorld()->GetCloudManager() )
		g_pGame->GetGameRun()->GetWorld()->GetCloudManager()->SetCloudLevel(10);	
	return true;
}

//	Release Login World
void CECGameRun::ReleaseLoginWorld()
{
	if (m_pWorld)
	{
		m_pWorld->Release();
		delete m_pWorld;
		m_pWorld = NULL;
	}

	g_pGame->ReleaseLoginRes();

	return;
}

//	Create host player
bool CECGameRun::CreateHostPlayer()
{
	ASSERT(m_pWorld);
	CECPlayerMan* pPlayerMan = m_pWorld->GetPlayerMan();
	return true;
}

//	Release host player
void CECGameRun::ReleaseHostPlayer()
{
}

//	Create shortcuts
bool CECGameRun::CreateShortcuts()
{
	return true;
}

//	Release shortcuts
void CECGameRun::ReleaseShortcuts()
{
}

void CECGameRun::ReleasePendingActions()
{
}

//	Release name/id pairs cache
void CECGameRun::ClearNameIDPairs()
{
	if (m_ID2NameTab.size())
	{
		ID2NameTable::iterator it = m_ID2NameTab.begin();
		for (; it != m_ID2NameTab.end(); ++it)
		{
			ACString* pstrName = *it.value();
			delete pstrName;
		}

		m_ID2NameTab.clear();
	}

	m_Name2IDTab.clear();
}

//	Get command shortcut (except pose command) by command ID
CECSCCommand* CECGameRun::GetCmdShortcut(int iCommand)
{
	return NULL;
}

//	Get pose command shortcut by pose
CECSCCommand* CECGameRun::GetPoseCmdShortcut(int iPose)
{
	return NULL;
}

//	Deal windows message
bool CECGameRun::DealWindowsMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (!m_pUIManager || !m_pInputCtrl)
		return false;

	if (m_pUIManager->DealWindowsMessage(message, wParam, lParam))
	{
	//	m_bUIHasCursor = m_pUIManager->UIControlCursor();
		return true;
	}
	else
	{
//		if (message == WM_MOUSEMOVE)
//			m_bUIHasCursor = false;

		switch (message)
		{
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
			if( message == WM_KEYUP && wParam == VK_SNAPSHOT)
			{
				if (!m_pWorld)
					break;

				int N = 1;

				CaptureScreen(N);
				break;
			}

			if (m_pInputCtrl->DealKeyMessage(message, wParam, lParam))
				return true;

			break;

		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONDBLCLK:
		case WM_RBUTTONUP:
	//	case WM_MBUTTONDOWN:
	//	case WM_MBUTTONUP:
		case WM_MOUSEMOVE:
		case WM_MOUSEWHEEL:

			if (m_pInputCtrl->DealMouseMessage(message, wParam, lParam))
				return true;

			break;
		}
	}

	return false;
}

void CECGameRun::TickDiscardedFrame(DWORD dwDeltaTime)
{
}

//	Game tick routine
bool CECGameRun::Tick(DWORD dwDeltaTime)
{
	DWORD dwTickTime = a_GetTime();
	DWORD dwRealTime = g_pGame->GetRealTickTime();

	if (m_iLogoutFlag >= 0)
	{
		Logout();
		m_iLogoutFlag = -1;
	}

	if (m_pUIManager)
		m_bUIHasCursor = m_pUIManager->UIControlCursor();
	else
		m_bUIHasCursor = false;

	//	Deal input first
	if (m_pInputCtrl)
		m_pInputCtrl->Tick();

	//	Tick world
	if (!TickGameWorld(dwDeltaTime, g_pGame->GetViewport()))
		return false;

	//	Tick UI
	if (m_pUIManager)
		m_pUIManager->Tick();

	//	Tick GFX caster
	g_pGame->GetGFXCaster()->Tick(dwDeltaTime);

	//	Tick GFX Manager
	g_pGame->GetA3DGFXExMan()->Tick(dwDeltaTime);

	//	A3DEngine::TickAnimation trigger animation of many objects.
	//	For example: A3DSky objects, GFX objects etc.
	static DWORD dwAnimTime = 0;
	dwAnimTime += dwDeltaTime;
	while (dwAnimTime >= TIME_TICKANIMATION)
	{
		dwAnimTime -= TIME_TICKANIMATION;
		g_pGame->GetA3DEngine()->TickAnimation();
	}

	//	Update ear position so that all 3D sounds' positions are correct
	static DWORD dwEarTime = 0;
	if ((dwEarTime += dwDeltaTime) >= TIME_UPDATEEAR)
	{
		dwEarTime -= TIME_UPDATEEAR;

		A3DCamera * pCamera = g_pGame->GetViewport()->GetA3DCamera();
			g_pGame->GetViewport()->GetA3DCamera()->UpdateEar();
	}

	//	Tick Run-Time debug information
	g_pGame->GetRTDebug()->Tick(dwDeltaTime);

	//	Save UI configs when time reached
	if (m_iGameState == GS_GAME && l_SaveCfgCnt.IncCounter(dwRealTime))
	{
		l_SaveCfgCnt.Reset();
		SaveConfigsToServer();
	}
	
	l_StatCnt.IncCounter(dwDeltaTime);

	DWORD dwCurrentTick = a_GetTime();
	dwTickTime = (dwCurrentTick > dwTickTime) ? (dwCurrentTick - dwTickTime) : 0;
	l_Statistic.iTickTime = (int)dwTickTime;

	if (GetGameState() == CECGameRun::GS_GAME && l_fc.iAvgRdTime)
	{
		//	Accumulate tick time
		l_fc.iTickCnt++;
		l_fc.iTickTime += (int)dwTickTime;
	}

	if(m_pRandomMapProc)
		m_pRandomMapProc->Tick(dwDeltaTime);

	return true;
}

#define DEFENCE_INTERV_MIN 30000
#define DEFENCE_INTERV_MAX 60000

static DWORD dwDefenceCount = 0;
static DWORD dwDefenceInterval = DEFENCE_INTERV_MIN + (DEFENCE_INTERV_MAX - DEFENCE_INTERV_MIN) * rand() / RAND_MAX;
static DWORD dwLastTime = 0;

extern HANDLE g_hExitGame;


// #define DEFENCE_TIME_LOG

//	Tick game world
bool CECGameRun::TickGameWorld(DWORD dwDeltaTime, CECViewport* pViewport)
{
	if (!m_pWorld)
		return true;

	//	Offset world center for skin model manager, this will fix skin model
	//	flash rendering problem caused by float calculation precision error !
	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetWorldCenter(pViewport->GetA3DCamera()->GetPos());

	bool bHostReady = false;
// 	bool bTickHost = bHostReady && (!m_pWorld->IsInAutoSceneState() || m_pWorld->IsAutoScentHostMode());
	bool bTickHost = bHostReady;

	//	Tick host player at first

	//	Dispatch game messages
	if (m_pMessageMan)
		m_pMessageMan->Tick(dwDeltaTime);

	//	Some messages also effect host's position and direction, so update
	//	camera position and direction after messages were dispatched
	//	tick host flag must update here
// 	bTickHost = bHostReady && (!m_pWorld->IsInAutoSceneState() || m_pWorld->IsAutoScentHostMode());
	bTickHost = bHostReady;

	if(m_pWorld->IsValid())
		m_pWorld->Tick(dwDeltaTime, g_pGame->GetViewport());

	return true;
}

//	Game render routine
bool CECGameRun::Render(bool bPresent)
{
	if (!m_pWorld)
		return true;

	A3DEngine* pA3DEngine = g_pGame->GetA3DEngine();
	A3DDevice* pA3DDevice = g_pGame->GetA3DDevice();
	CECViewport* pViewport = g_pGame->GetViewport();

	CECUIManager *pUIManger = g_pGame->GetGameRun()->GetUIManager();
	bool bLogin = (pUIManger->GetCurrentUIMan() == CECUIManager::UIMAN_LOGIN);

	if (g_pGame->GetConfigs()->GetSkipFrameFlag() &&
		GetGameState() == CECGameRun::GS_GAME && l_fc.iAvgRdTime)
	{
		if (l_fc.iTickCnt < 10 &&
			l_fc.iTickTime + l_fc.iAvgRdTime > l_fc.iTickCnt * 150)
			return true;

		l_fc.iTickTime	= 0;
		l_fc.iTickCnt	= 0;
	}

	DWORD dwRenderTime = a_GetTime();

	if (!BeginRender())
	{
		a_LogOutput(1, "CECGameRun::Render(), Failed to begin render!");
		return false;
	}

	if( g_pGame->GetConfigs()->GetSystemSettings().bWideScreen )
	{
		A3DDEVFMT devFmt = pA3DDevice->GetDevFormat();
		A3DVIEWPORTPARAM * pMainView = g_pGame->GetViewport()->GetA3DViewport()->GetParam();
		
		// we must clear top and bottom bar to be pure black, otherwise, there maybe some random images
		A3DVIEWPORTPARAM viewParam;
		viewParam.X = 0;
		viewParam.Width  = devFmt.nWidth;
		viewParam.MinZ = 0.0f;
		viewParam.MaxZ = 1.0f;
		
		viewParam.Y = 0;
		viewParam.Height = pMainView->Y;
		pA3DDevice->SetViewport(&viewParam);
		pA3DDevice->Clear(D3DCLEAR_TARGET, 0x00000000, 1.0f, 0);
		
		viewParam.Y = pMainView->Y + pMainView->Height;
		viewParam.Height = devFmt.nHeight - viewParam.Y;
		pA3DDevice->SetViewport(&viewParam);
		pA3DDevice->Clear(D3DCLEAR_TARGET, 0x00000000, 1.0f, 0);
	}

	bool bFullGlowRender = false;
	if (m_pFullGlowRender)
	{
		bFullGlowRender = true;
		m_pFullGlowRender->BeginGlow();		
		if (GetGameState() == CECGameRun::GS_LOGIN){
			m_pFullGlowRender->SetGlowType(FULLGLOW_TYPE_LOGIN, 0xffffffff);
		}
		m_pFullGlowRender->Update(g_pGame->GetTickTime());
	}

	// don't clear color buffer because we may do motion blur effects.
	pViewport->Activate(false);
	g_pGame->GetA3DDevice()->Clear(D3DCLEAR_ZBUFFER, 0xff000000, 1.0f, 0);
	
	//	渲染开始前，准备记录高亮显示模型，以便恢复非高亮状态
	g_pGame->PrepareHighlightModel();
	
	//	Render world
	if (m_pWorld && m_pWorld->IsValid())
		m_pWorld->Render(pViewport);
	
	//	Render GFX
	g_pGame->GetGFXCaster()->Render(pViewport);
	pA3DEngine->RenderGFX(pViewport->GetA3DViewport(), 0xffffffff);
	
	if (g_pA3DConfig->RT_GetShowBoundBoxFlag())
		g_pGame->GetA3DGFXExMan()->RenderAllGfxAABB();
	
	g_pGame->GetA3DGFXExMan()->RenderAllSkinModels(pViewport->GetA3DViewport());
	A3DSkinRender* psr1 = g_pGame->GetSkinRender1();
	
	//	Render non alpha skin models
	psr1->Render(pViewport->GetA3DViewport(), false);
	psr1->ResetRenderInfo(false);
	psr1->RenderAlpha(pViewport->GetA3DViewport());
	psr1->ResetRenderInfo(true);
	
	g_pGame->GetA3DGFXExMan()->RenderAllGfx(pViewport->GetA3DViewport());
	
	//	Flush wire and flat collectors
	pA3DEngine->GetA3DWireCollector()->Flush();
	pA3DEngine->GetA3DFlatCollector()->Flush();
	
	if( bFullGlowRender)
	{
		m_pFullGlowRender->EndGlow(pViewport->GetA3DViewport());
	}
	
	//	Render all registered pate text
	if (m_pWorld && m_pWorld->GetDecalMan())
	{
		pA3DDevice->SetAlphaBlendEnable(false);
		pA3DDevice->SetAlphaTestEnable(true);
		m_pWorld->GetDecalMan()->RenderPateTexts(pViewport);
		pA3DDevice->SetAlphaBlendEnable(true);
		pA3DDevice->SetAlphaTestEnable(false);
	}
	
	//	Flush images
	g_pGame->GetImageRes()->PresentDraws();
	
	//	Render 2D gfx
	
	//	Render UI
	if (m_pUIManager)
	{
		g_pGame->GetA3DEngine()->FlushInternalSpriteBuffer();
		m_pUIManager->Render();
	}
	
	{
#ifdef SHOW_AUTOMOVE_FOOTPRINTS
		extern abase::vector<A3DVECTOR3> g_AutoPFFollowPoints;
		extern abase::vector<A3DVECTOR3> g_AutoPFPathPoints;
		if (!g_AutoPFFollowPoints.empty()){
			A3DStream * pStream = new A3DStream();
			A3DDevice *pA3DDevice = g_pGame->GetA3DDevice();
			if(pStream->Init(pA3DDevice, A3DVT_LVERTEX, g_AutoPFFollowPoints.size(), 0, A3DSTRM_REFERENCEPTR, 0)){
				pA3DDevice->ClearTexture(0);
				
				pA3DDevice->SetZWriteEnable(false);
				pA3DDevice->SetAlphaBlendEnable(false);
				pA3DDevice->SetPointSize(5.0f);
				pA3DDevice->SetLightingEnable(false);
				
				pStream->Appear();
				A3DLVERTEX * pVerts = NULL;
				A3DVECTOR3 deltaY(0.0f, 0.1f, 0.0f);
				if (!g_AutoPFFollowPoints.empty() && pStream->LockVertexBuffer(0, 0, (BYTE **)&pVerts, NULL)){
					int i(0);
					A3DCOLOR clrRed = A3DCOLORRGB(255, 0, 0);
					for (i = 0; i < (int)g_AutoPFFollowPoints.size(); ++ i)
					{
						pVerts[i] = A3DLVERTEX(g_AutoPFFollowPoints[i]+deltaY, clrRed, clrRed, 0, 0);
					}
					pStream->UnlockVertexBuffer();
					pA3DDevice->DrawPrimitive(A3DPT_POINTLIST, 0, g_AutoPFFollowPoints.size());
				}
				if (!g_AutoPFPathPoints.empty() && pStream->LockVertexBuffer(0, 0, (BYTE **)&pVerts, NULL)){
					int i(0);
					A3DCOLOR clrGreen = A3DCOLORRGB(0, 255, 0);
					for (i = 0; i < (int)g_AutoPFPathPoints.size(); ++ i)
					{
						pVerts[i] = A3DLVERTEX(g_AutoPFPathPoints[i]+deltaY, clrGreen, clrGreen, 0, 0);
					}
					pStream->UnlockVertexBuffer();
					pA3DDevice->DrawPrimitive(A3DPT_POINTLIST, 0, g_AutoPFPathPoints.size());
				}			
				
				pA3DDevice->SetZWriteEnable(true);
				pA3DDevice->SetAlphaBlendEnable(true);
				pA3DDevice->SetLightingEnable(true);
			}
			A3DRELEASE(pStream);
		}
#endif
	}
	
	// 在此处显示顶层gfx,不会被界面挡住
	g_pGame->GetGFXCaster()->RenderTop(pViewport);

	//	Render Run-Time debug information
	g_pGame->GetRTDebug()->Render(pViewport);

	//	Render statistic
	if (g_pGame->GetConfigs()->GetShowGameStatFlag())
		DisplayGameStatistic();

	EndRender(bPresent);
	
	//	恢复模型的非高亮显示状态
	g_pGame->RestoreHightlightModel();

	dwRenderTime = a_GetTime() - dwRenderTime;

	if (GetGameState() == CECGameRun::GS_GAME)
	{
		//	Calculate average render time
		l_fc.iRenderTime += (int)dwRenderTime;
		l_fc.iRenderCnt++;

		int iTotal = l_fc.iAvgRdTime ? 30 : 10;
		if (l_fc.iRenderCnt >= iTotal)
		{
			l_fc.iAvgRdTime	 = (int)((float)l_fc.iRenderTime / l_fc.iRenderCnt + 0.5f);
			l_fc.iRenderCnt  = 0;
			l_fc.iRenderTime = 0;
		}
	}

	// clear dynamic light list.
	GfxLightParamList& lightList = g_pGame->GetA3DGFXExMan()->GetLightParamList();
	g_pGame->GetA3DGFXExMan()->LockLightParamList();
	lightList.clear();
	g_pGame->GetA3DGFXExMan()->UnlockLightParamList();
	return true;
}

//	Begin render
bool CECGameRun::BeginRender()
{
	A3DEngine* pA3DEngine = g_pGame->GetA3DEngine();

	if (!pA3DEngine->BeginRender())
		return false;

	m_dwRenderTime = a_GetTime();

	return true;
}

//	End render
bool CECGameRun::EndRender(bool bPresent)
{
	A3DEngine* pA3DEngine = g_pGame->GetA3DEngine();

	A3DSkinRender* psr = g_pGame->GetSkinRender1();
	if (psr)
	{
		psr->ResetRenderInfo(false);
		psr->ResetRenderInfo(true);
	}

	if ((psr = g_pGame->GetSkinRender2()))
	{
		psr->ResetRenderInfo(false);
		psr->ResetRenderInfo(true);
	}

	if (!pA3DEngine->EndRender())
		return false;

	if( bPresent )
		pA3DEngine->Present();

	m_dwRenderTime = a_GetTime() - m_dwRenderTime;
	l_Statistic.iRenderTime = (int)m_dwRenderTime;

	return true;
}

//	Set logout flag
void CECGameRun::SetLogoutFlag(int iFlag)
{
	m_iLogoutFlag = iFlag;
}

//	Logout
void CECGameRun::Logout()
{
}

//	Save necessary user configs (UI, shortcut, accelerate keys, etc.) to server
//	返回值: 0(保存失败), 1(数据无需重复保存), 2(已向服务器发送协议)
DWORD CECGameRun::SaveConfigsToServer()
{
	return 0;
}

//	Load necessary user configs (UI, shortcut, accelerate keys) from server
bool CECGameRun::LoadConfigsFromServer(const void* pDataBuf, int iDataSize)
{
	return true;
}

//	Add fixed format message to chat window
void CECGameRun::AddFixedMessage(int iMsg, ...)
{
	CECStringTab* pStrTab = g_pGame->GetFixedMsgTab();
	const wchar_t* szFixMsg = pStrTab->GetWideString(iMsg);
	if (!szFixMsg)
		return;

	ACHAR szBuf[1024];

	va_list vaList;
	va_start(vaList, iMsg);
	glb_vsnprintf(szBuf, sizeof(szBuf)/sizeof(szBuf[0]), szFixMsg, vaList);
	va_end(vaList);
}

void CECGameRun::AddFixedChannelMsg(int iMsg, char cChannel, ...)
{
	CECStringTab* pStrTab = g_pGame->GetFixedMsgTab();
	const wchar_t* szFixMsg = pStrTab->GetWideString(iMsg);
	if (!szFixMsg)
		return;

	ACHAR szBuf[1024];

	va_list vaList;
	va_start(vaList, cChannel);
	glb_vsnprintf(szBuf, sizeof(szBuf)/sizeof(szBuf[0]), szFixMsg, vaList);
	va_end(vaList);
}

//	Add message to chat window
void CECGameRun::AddChatMessage(const ACHAR *pszMsg, char cChannel, int idPlayer/* -1 */,
								const ACHAR* szName/* NULL */, BYTE byFlag, char cEmotion,
								CECIvtrItem *pItem /* NULL */,const ACHAR *pszMsgOrigion/* NULL */)
{
}

//	Display game statistic
void CECGameRun::DisplayGameStatistic()
{
	if (!m_pWorld)
		return;

	if (l_StatCnt.IsFull())
	{
		//	Reset statistic...
		l_StatCnt.Reset();

		l_Statistic.iMeshSize		= g_pGame->GetA3DEngine()->GetA3DSkinMan()->GetSkinMeshMan()->CalcDataSize();
		l_Statistic.iAllTexSize		= g_pGame->GetA3DEngine()->GetA3DTextureMan()->GetTextureDataSize();
		l_Statistic.iStrmDSize		= A3DStream::m_iDynDataSize;
		l_Statistic.iStrmSSize		= A3DStream::m_iStaticDataSize;
		l_Statistic.iStrmBSize		= A3DStream::m_iBackupDataSize;
		l_Statistic.iTrackSize		= A3D::g_A3DTrackMan.GetTrackDataSize();
		l_Statistic.iTrnDataSize	= m_pWorld->GetTerrain() ? m_pWorld->GetTerrain()->GetBlockDataSize() : 0;
		l_Statistic.iSndCacheSize	= g_pGame->GetSoundCache()->GetCacheSize();

		l_Statistic.iNumPlayer		= 0;
		l_Statistic.iNumNPC			= 0;
		l_Statistic.iNumMatter		= 0;
		l_Statistic.iNumScnGFX		= m_pWorld->GetOrnamentMan()->GetGFXNum();
		l_Statistic.iNumOnmt		= m_pWorld->GetOrnamentMan()->GetOrnamentNum();
		l_Statistic.iNumScnModel	= m_pWorld->GetOrnamentMan()->GetECModelNum();
		l_Statistic.iNumTrnBlock	= m_pWorld->GetTerrain() ? m_pWorld->GetTerrain()->GetRenderedBlockNum() : 0;

		if (g_pGame->GetSoundCache()->GetLoadFileCnt())
			l_Statistic.fSndCacheHit = g_pGame->GetSoundCache()->GetFileCacheHitCnt() * 100.0f / g_pGame->GetSoundCache()->GetLoadFileCnt();
		else
			l_Statistic.fSndCacheHit = 0.0f;

		l_Statistic.iMemPeak		= a_GetMemPeakSize();
		l_Statistic.dwMemAllocCnt	= a_GetMemAllocCounter();
		l_Statistic.iMemCurSize		= a_GetMemCurSize();
		l_Statistic.iMemRawSize		= a_GetMemRawSize();

		l_Statistic.iSMemAllocSize	= 0;
		l_Statistic.iSMemBlkCnt		= 0;
		for (int i=0; i < 16; i++)
		{
			a_GetSmallMemInfo(i, &l_Statistic.aSMemBlkSize[i], &l_Statistic.aSMemBlkCnt[i], &l_Statistic.aSMemFreeCnt[i]);
			l_Statistic.aSMemSize[i] = l_Statistic.aSMemBlkSize[i] * l_Statistic.aSMemBlkCnt[i];
			l_Statistic.iSMemAllocSize += l_Statistic.aSMemSize[i];
			l_Statistic.iSMemBlkCnt += l_Statistic.aSMemBlkCnt[i];
		}

		a_GetLargeMemInfo(&l_Statistic.iLMemBlkCnt, &l_Statistic.iLMemAllocSize);
	}

	A3DFont* pA3DFont = g_pGame->GetA3DEngine()->GetSystemFont();
	ACHAR szMsg[256];
	int x, y, i;
	DWORD dwCol;

	//	Data size counter ...
	dwCol = 0xffff8040;
	x = 50;
	y = 100;
	a_sprintf(szMsg, _AL("Skin Mesh Size: %.2f K"), l_Statistic.iMeshSize / 1024.0f);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("All Texture Size: %.2f K"), l_Statistic.iAllTexSize / 1024.0f);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("Stream (D) Size: %.2f K"), l_Statistic.iStrmDSize / 1024.0f);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("Stream (S) Size: %.2f K"), l_Statistic.iStrmSSize / 1024.0f);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("Stream (B) Size: %.2f K"), l_Statistic.iStrmBSize / 1024.0f);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("Track Size: %.2f K"), l_Statistic.iTrackSize / 1024.0f);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("Terrain Size: %.2f K"), l_Statistic.iTrnDataSize / 1024.0f);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("Sound Cache: %.2f K"), l_Statistic.iSndCacheSize / 1024.0f);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	//	Object counter ...
	dwCol = 0xff00ffff;
	x = 270;
	y = 100;
	a_sprintf(szMsg, _AL("player num: %d"), l_Statistic.iNumPlayer);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("NPC num: %d"), l_Statistic.iNumNPC);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("matter num: %d"), l_Statistic.iNumMatter);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("scene GFX num: %d"), l_Statistic.iNumScnGFX);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("building num: %d"), l_Statistic.iNumOnmt);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("scene model num: %d"), l_Statistic.iNumScnModel);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("snd cache hit: %.1f%%"), l_Statistic.fSndCacheHit);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("vis trn block: %d"), l_Statistic.iNumTrnBlock);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	//	Time counter ...
	dwCol = 0xffff00ff;
	y += 100;
	a_sprintf(szMsg, _AL("tick time: %d"), l_Statistic.iTickTime);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("render time: %d"), l_Statistic.iRenderTime);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	//	Gfx Stat
	x = 50;
	y = 300;
	a_sprintf(szMsg, _AL("gfx tick time: %.2f"), AfxGetGFXExMan()->GetTotalTickTime());
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("gfx render time: %.2f"), AfxGetGFXExMan()->GetTotalRenderTime());
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("gfx total count: %d"), AfxGetGFXExMan()->GetTotalGfxCount());
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("gfx render count: %d"), AfxGetGFXExMan()->GetTotalGfxRenderCount());
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;
	
	a_sprintf(szMsg, _AL("gfx draw count: %d"), AfxGetGFXExMan()->GetTotalDrawCount());
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("gfx prim count: %d"), AfxGetGFXExMan()->GetTotalPrimCount());
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("par total count: %d"), AfxGetGFXExMan()->GetParticleTotalCount());
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("par render count: %d"), AfxGetGFXExMan()->GetParticleRenderCount());
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	//	Memory statistics ...
	dwCol = 0xffffff00;
	x = 440;
	y = 100;
	a_sprintf(szMsg, _AL("mem peak: %.2f K"), l_Statistic.iMemPeak / 1024.0f);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("mem alloc cnt: %u"), l_Statistic.dwMemAllocCnt);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("mem size: %.2f K"), l_Statistic.iMemCurSize / 1024.0f);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("mem raw size: %.2f K"), l_Statistic.iMemRawSize / 1024.0f);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("mem(S) blk cnt: %d"), l_Statistic.iSMemBlkCnt);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("mem(S) size: %.2f K"), l_Statistic.iSMemAllocSize / 1024.0f);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("mem(L) blk cnt: %d"), l_Statistic.iLMemBlkCnt);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("mem(L) size: %.2f K"), l_Statistic.iLMemAllocSize / 1024.0f);
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	y += 16;
	for (i=0; i < 16; i++)
	{
		a_sprintf(szMsg, _AL("mem(S) [%d]: %d, %d, %d, %.2f K"), i, l_Statistic.aSMemBlkSize[i], 
			l_Statistic.aSMemBlkCnt[i], l_Statistic.aSMemFreeCnt[i], l_Statistic.aSMemSize[i] / 1024.0f);
		pA3DFont->TextOut(x, y, szMsg, dwCol);
		y += 16;
	}

	// sound load count
	dwCol = 0xff00ff00;
	x = 640;
	y = 100;
	a_sprintf(szMsg, _AL("snd always: %d"), g_pGame->GetSoundCache()->GetLoadCount(enumSoundLoadAlways));
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("snd noloop: %d"), g_pGame->GetSoundCache()->GetLoadCount(enumSoundLoadNonLoop));
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;

	a_sprintf(szMsg, _AL("snd loop  : %d"), g_pGame->GetSoundCache()->GetLoadCount(enumSoundLoadLoop));
	pA3DFont->TextOut(x, y, szMsg, dwCol);
	y += 16;
}

//	Begin load progress
bool CECGameRun::BeginLoadProgress(float Min, float Max)
{
	return true;
}

//	End load progress
void CECGameRun::EndLoadProgress()
{
}

//	Step load progress
void CECGameRun::StepLoadProgress(float Step)
{
	g_pGame->DispatchWindowsMessage();
}

A3DVECTOR3 CECGameRun::GetSafeHostPos(CECViewport* pViewport)
{
	{
		//	Use camera's position to imitate host's position
		if (!pViewport)
			pViewport = g_pGame->GetViewport();
		
		return pViewport->GetA3DCamera()->GetPos();
	}
}

//	Get player name
const ACHAR* CECGameRun::GetPlayerName(int cid, bool bAutoGen)
{
	ID2NameTable::pair_type Pair = m_ID2NameTab.get(cid);
	if (Pair.second)
		return **Pair.first;

	if (bAutoGen)
	{
		m_strAutoName.Format(_AL("P-%d"), cid);
		return m_strAutoName;
	}

	return NULL;
}

//	Get player ID by name
int CECGameRun::GetPlayerID(const ACHAR* szName)
{
	Name2IDTable::pair_type Pair = m_Name2IDTab.get(szName);
	return Pair.second ? *Pair.first : 0;
}

//	Add player name to table
void CECGameRun::AddPlayerName(int cid, const ACHAR* szName, bool bOverwrite/* false */)
{
	if(!cid)
	{
		ASSERT(false);
		a_LogOutput(1, "CECGameRun::AddPlayerName, Invalid cid(=0)!");
		return;
	}

	ACString strName = szName;
	if (strName.IsEmpty())
	{
		ASSERT(false);
		a_LogOutput(1, "CECGameRun::AddPlayerName, Invalid name for cid=%d!", cid);
		return;
	}

	Name2IDTable::pair_type Pair2 = m_Name2IDTab.get(strName);
	if (!Pair2.second)
		m_Name2IDTab.put(strName, cid);
	else
		*Pair2.first = cid;
	
	ID2NameTable::pair_type Pair = m_ID2NameTab.get(cid);
	if (Pair.second)
	{		
		if (bOverwrite)
		{
			//	Name exists, replace it
			ACString* pstrName = *Pair.first;
			*pstrName = strName;
		}
		
		return;
	}
	
	//	Create new name
	ACString* pstrName = new ACString(strName);
	m_ID2NameTab.put(cid, pstrName);
}

//	Get instance by ID
CECInstance* CECGameRun::GetInstance(int id)
{
	InstTable::pair_type Pair = m_InstTab.get(id);
	return Pair.second ? *Pair.first : NULL;
}

CECInstance* CECGameRun::GetInstance(const ACHAR* szName)
{	//	Release all instance information
	InstTable::iterator it = m_InstTab.begin();
	for (; it != m_InstTab.end(); ++it)
	{
		CECInstance* pInst = *it.value();
		if(AS2AC(pInst->GetPath()).CompareNoCase(szName) == 0)
		{
			return pInst;
		}
	}

	return NULL;
}

//	Load instance information from file
bool CECGameRun::LoadInstanceInfo(const char* szFile)
{
	AWScriptFile sf;
	if (!sf.Open(szFile))
	{
		a_LogOutput(1, "CECGameRun::LoadInstanceInfo, Failed to load %s", szFile);
		return false;
	}

	while (sf.PeekNextToken(true))
	{
		CECInstance* pInst = new CECInstance;
		if (!pInst || !pInst->Load(&sf))
		{
			a_LogOutput(1, "CECGameRun::LoadInstanceInfo, Failed to read %s near line:%d", szFile, sf.GetCurLine());
			return false;
		}

		if (!m_InstTab.put(pInst->GetID(), pInst))
		{
			//	ID collsion ?
			ASSERT(0);
			delete pInst;
		}
	}

	sf.Close();

	return true;
}

//	Jump to specified position in a instance
bool CECGameRun::JumpToInstance(int idInst, const A3DVECTOR3& vPos, int iParallelWorldID)
{
	return true;
}

void CECGameRun::ActivateDefInput()
{
	m_pInputCtrl->ClearKBFilterStack();
	m_pInputCtrl->ClearMouFilterStack();
}

// void CECGameRun::ActivateAutoHomeInput()
// {
// 	m_pInputCtrl->ClearKBFilterStack();
// 	m_pInputCtrl->ClearMouFilterStack();
// 	m_pInputCtrl->ActivateKBFilter(m_pAutoHomeFilter);
// 	m_pInputCtrl->ActivateMouseFilter(m_pAutoHomeFilter);
// }

// test code
A3DVECTOR3 _org_pos(0);

//	Process message
bool CECGameRun::ProcessMessage(const ECMSG& Msg)
{

	return true; 
}

//	Post message
bool CECGameRun::PostMessage(DWORD dwMsg, int iManager, int iSubID, DWORD p1/* 0 */, 
						DWORD p2/* 0 */, DWORD p3/* 0 */, DWORD p4/* 0 */, MsgDataBase* pData/* NULL */)
{
	ECMSG Msg;

	Msg.dwMsg		= dwMsg;
	Msg.iManager	= iManager;
	Msg.iSubID		= iSubID;
	Msg.dwParam1	= p1;
	Msg.dwParam2	= p2;
	Msg.dwParam3	= p3;
	Msg.dwParam4	= p4;
	Msg.pMsgData	= pData;

	return PostMessage(Msg);
}

//	Post message
bool CECGameRun::PostMessage(const ECMSG& Msg)
{
	ASSERT(m_pMessageMan);
	if (m_pMessageMan)
		m_pMessageMan->AddMessage(Msg);
	return true;
}

//	Get remain time of current double experence time
int CECGameRun::GetRemainDblExpTime()
{
	int iRemainTime = m_iDExpEndTime - g_pGame->GetServerAbsTime();
	a_ClampFloor(iRemainTime, 0);
	return iRemainTime;
}

bool CECGameRun::CaptureScreen(int N)
{
	//	ANGELICA_2_2 引擎下 且 N <= 1 时使用 jpg，其它情况使用 bmp
	A3DDEVFMT devFormat = g_pGame->GetA3DDevice()->GetDevFormat();

	if (devFormat.fmtTarget != A3DFMT_A8R8G8B8 &&
		devFormat.fmtTarget != A3DFMT_X8R8G8B8)
		N = 1;

	if (g_pGame->GetConfigs()->GetSystemSettings().bWideScreen)
		N = 1;

	//	N > 1 时使用的临时 Surface
	A3DSurface * pTempSurface = NULL;
	if (N > 1)
	{
		pTempSurface = new A3DSurface();
		if (!pTempSurface->Create(g_pGame->GetA3DDevice(), devFormat.nWidth, devFormat.nHeight, devFormat.fmtTarget))
		{
			a_LogOutput(0, "Capture screen failure because creating A3DSurface failure for N=%d and N will be set to 1", N);
			delete pTempSurface;
			pTempSurface = NULL;
			N = 1;
		}
	}

	SYSTEMTIME st;
	GetLocalTime(&st);

	char szTitle[MAX_PATH], szFile[MAX_PATH];
	bool bExportJPG = false;
#ifdef ANGELICA_2_2
	bExportJPG = (N <= 1);
#endif
	char *szFileFormat = bExportJPG ? "%04d-%02d-%02d %02d-%02d-%02d.jpg" : "%04d-%02d-%02d %02d-%02d-%02d.bmp";
	sprintf(szTitle, szFileFormat,	st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	sprintf(szFile, "%s\\Screenshots\\", af_GetBaseDir());
	glb_CreateDirectory(szFile);
	strcat(szFile, szTitle);

	// we turn off the fps shown and take a snap shot
	bool bShowFPS = g_pA3DConfig->RT_GetShowFPSFlag();
	int bRTDebug = g_pGame->GetConfigs()->GetRTDebugLevel();
	g_pA3DConfig->RT_SetShowFPSFlag(false);
	g_pGame->GetConfigs()->SetRTDebugLevel(0);

	if (N <= 1)
	{
		Render(false);
		Sleep(100);// wait a while to let the entire scene rendered to back.
#ifdef ANGELICA_2_2
		g_pGame->GetA3DDevice()->ExportColorToFile(szFile, bExportJPG);
#else
		g_pGame->GetA3DDevice()->ExportColorToFile(szFile);
#endif
	}
	else
	{
		int nWidth = devFormat.nWidth;
		int nHeight = devFormat.nHeight;

		FILE * fpFile = fopen(szFile, "wb");
		if (!fpFile)
		{
			a_LogOutput(0, "Can not create screen shot file %s", szFile);
		}
		else
		{
			int pitch = N * nWidth * 3;
			pitch += (4 - (pitch & 0x3)) & 0x3;

			BITMAPFILEHEADER bmfh;
			BITMAPINFOHEADER bmih;
			memset(&bmfh, 0, sizeof(bmfh));
			memset(&bmih, 0, sizeof(bmih));
			bmfh.bfType = 0x4D42;
			bmfh.bfSize = sizeof(bmfh) + sizeof(bmih) + pitch * nHeight;
			bmfh.bfOffBits = sizeof(bmfh) + sizeof(bmih);

			bmih.biSize = sizeof(bmih);
			bmih.biWidth = N * nWidth;
			bmih.biHeight = N * nHeight;
			bmih.biPlanes = 1;
			bmih.biBitCount = 24;
			bmih.biCompression = BI_RGB;

			fwrite(&bmfh, sizeof(bmfh), 1, fpFile);
			fwrite(&bmih, sizeof(bmih), 1, fpFile);

			BYTE * pTempLine = (BYTE*)a_malloctemp(pitch * nHeight);
			A3DCameraBase * pCamera = g_pGame->GetViewport()->GetA3DCamera();
			for(int h=0; h<N; h++)
			{
				for(int w=0; w<N; w++)
				{
					A3DMATRIX4 matPostOffset;
					matPostOffset = Scaling((float)N, (float)N, 1.0f) * Translate(N - 1.0f - w * 2, h * 2 - (N - 1.0f), 0.0f);
					pCamera->SetPostProjectTM(matPostOffset);

					Render(false);
					Sleep(100);// wait a while to let the entire scene rendered to back.

					HRESULT hval;
#ifdef ANGELICA_2_2
					hval = D3DXLoadSurfaceFromSurface(pTempSurface->GetDXSurface(), NULL, NULL, g_pGame->GetA3DDevice()->GetBackBuffer(), NULL, NULL, D3DX_DEFAULT, 0);
#else
					hval = g_pGame->GetA3DDevice()->GetD3DDevice()->CopyRects(g_pGame->GetA3DDevice()->GetBackBuffer(), NULL, 0, pTempSurface->GetDXSurface(), NULL);
#endif
					if( hval != D3D_OK )
					{
						a_LogOutput(0, "Capture screen copy surface error=%x for N=%d, h=%d, w=%d", hval, N, h, w);
						continue;
					}

					D3DLOCKED_RECT rectLocked;
					hval = pTempSurface->GetDXSurface()->LockRect(&rectLocked, NULL, D3DLOCK_READONLY);
					if( hval != D3D_OK )
					{
						a_LogOutput(0, "Capture screen Lock surface rect error=%x for N=%d, h=%d, w=%d", hval, N, h, w);
						continue;
					}

					int x, y;
					DWORD * pLines = NULL;
					for(y=0; y<nHeight; y++)
					{
						pLines = (DWORD *)((BYTE*)rectLocked.pBits + y * rectLocked.Pitch);
						for(x=0; x<nWidth; x++)
						{
							A3DCOLOR color = pLines[x];
							pTempLine[(nHeight - 1 - y) * pitch + (w * nWidth + x) * 3] = A3DCOLOR_GETBLUE(color);
							pTempLine[(nHeight - 1 - y) * pitch + (w * nWidth + x) * 3 + 1] = A3DCOLOR_GETGREEN(color);
							pTempLine[(nHeight - 1 - y) * pitch + (w * nWidth + x) * 3 + 2] = A3DCOLOR_GETRED(color);
						}
					}

					pTempSurface->GetDXSurface()->UnlockRect();
				}

				fseek(fpFile, bmfh.bfOffBits + (N * nHeight - h * nHeight - nHeight) * pitch, SEEK_SET);
				fwrite(pTempLine, pitch * nHeight, 1, fpFile);
			}

			fflush(fpFile);
			fclose(fpFile);

			pCamera->SetPostProjectTM(IdentityMatrix());
			a_freetemp(pTempLine);
		}
	}

	if( pTempSurface )
	{
		pTempSurface->Release();
		delete pTempSurface;
		pTempSurface = NULL;
	}

	g_pA3DConfig->RT_SetShowFPSFlag(bShowFPS);
	g_pGame->GetConfigs()->SetRTDebugLevel(bRTDebug);
	return true;
}

long CECGameRun::GetCommonData(long key)
{
	CommonDataTable::pair_type Pair = m_CommonDataTab.get(key);
	if (Pair.second)
		return *Pair.first;
	else
		return 0;
}
CECGameRun::unique_data* CECGameRun::GetUniqueData(int key)
{
	UniqueDataMap::iterator itr = m_UniqueDataMap.find(key);
	if(itr==m_UniqueDataMap.end()) return NULL;
	
	return itr->second;
}
const wchar_t * CECGameRun::GetProfName(int iProf)const
{
	const wchar_t *szRet = NULL;
	if (iProf >= 0 && iProf < NUM_PROFESSION)
	{
		static int s_ProfDesc[NUM_PROFESSION] = {			
			FIXMSG_PROF_WARRIOR,
			FIXMSG_PROF_MAGE,
			FIXMSG_PROF_MONK,
			FIXMSG_PROF_HAG,
			FIXMSG_PROF_ORC,
			FIXMSG_PROF_GHOST,
			FIXMSG_PROF_ARCHOR,
			FIXMSG_PROF_ANGEL,
			FIXMSG_PROF_JIANLING,
			FIXMSG_PROF_MEILING,
			FIXMSG_PROF_YEYING,
			FIXMSG_PROF_YUEXIAN,
		};
		CECStringTab* pStrTab = g_pGame->GetFixedMsgTab();
		szRet = pStrTab->GetWideString(s_ProfDesc[iProf]);
	}
	else
	{
		ASSERT(false);
	}
	return szRet;
}

ACString CECGameRun::GetLevel2Name(int iLevel2)const
{
	ACString strRet;
	
	int iIndex = -1;
	if (iLevel2 >= 0 && iLevel2 <= 8)
		iIndex = FIXMSG_LEVEL2_01 + iLevel2;
	else if (iLevel2 >= 20 && iLevel2 <= 22)
		iIndex = FIXMSG_LEVEL2_20 + iLevel2 - 20;
	else if (iLevel2 >= 30 && iLevel2 <= 32)
		iIndex = FIXMSG_LEVEL2_30 + iLevel2 - 30;

	if (iIndex >= 0)
	{
		CECStringTab* pStrTab = g_pGame->GetFixedMsgTab();
		strRet = pStrTab->GetWideString(iIndex);
	}
	else
	{
		ASSERT(false);
	}
	return strRet;
}

ACString CECGameRun::GetLevel2NameForCondition(int iMinLevel2)const
{
	ACString strRet;
	
	CECStringTab* pStrTab = g_pGame->GetFixedMsgTab();

	if (iMinLevel2 >= 0 && iMinLevel2 <= 8)
	{
		strRet = pStrTab->GetWideString(FIXMSG_LEVEL2_01 + iMinLevel2);
	}
	else if (iMinLevel2 >= 20 && iMinLevel2 <= 22)
	{
		strRet.Format(pStrTab->GetWideString(FIXMSG_OR_STRING)
			, pStrTab->GetWideString(FIXMSG_LEVEL2_20 + iMinLevel2 - 20)
			, pStrTab->GetWideString(FIXMSG_LEVEL2_30 + iMinLevel2 - 20));
	}
	else if (iMinLevel2 >= 30 && iMinLevel2 <= 32)
	{
		strRet.Format(pStrTab->GetWideString(FIXMSG_OR_STRING)
			, pStrTab->GetWideString(FIXMSG_LEVEL2_20 + iMinLevel2 - 30)
			, pStrTab->GetWideString(FIXMSG_LEVEL2_30 + iMinLevel2 - 30));
	}
	else
	{
		ASSERT(false);
	}
	return strRet;
}

void CECGameRun::SetSellingRoleID(int roleid)
{
	m_SellingRoleID = roleid;
}

void CECGameRun::SetAccountLoginInfo(const AccountLoginInfo &info)
{
	if (m_AccountLoginInfo != info)
	{
		m_AccountLoginInfo = info;
		m_bAccountLoginInfoShown = false;
	}
}

void CECGameRun::ResetAccountLoginInfo()
{
	m_AccountLoginInfo.Reset();
	m_bAccountLoginInfoShown = true;
}

void CECGameRun::ShowAccountLoginInfo()
{
}

void CECGameRun::SetAccountInfoFlag(char accountinfo_flag)
{
	m_accountInfoFlag = accountinfo_flag;
	m_bAccountInfoShown = false;
}

void CECGameRun::ShowAccountInfo()
{
}

void CECGameRun::ReleaseSoundTable()
{
	if (!m_SoundTable.empty())
	{
		AMSoundBufferMan* pSoundBufMan = g_pGame->GetAMSoundBufferMan();
		SoundTable::iterator it = m_SoundTable.begin();
		for (; it != m_SoundTable.end(); ++ it)
		{
			AMSoundBuffer *pSBuffer = it->second;
			if (pSBuffer)
				pSoundBufMan->Release2DSound(&pSBuffer);
		}
		m_SoundTable.clear();
	}
}

bool CECGameRun::ImportSoundStringTable(const char* szFile)
{	
	AScriptFile sf;
	if (!sf.Open(szFile))
	{
		a_LogOutput(1, "CECGameRun::ImportSoundStringTable, Failed to load %s", szFile);
		return false;
	}
	
	while( !sf.IsEnd() )
	{
		bool bval = sf.GetNextToken(true);
		if( !bval ) break;		// End of file.
		int idString = atoi(sf.m_szToken);
		
		bval = sf.GetNextToken(true);
		if( !bval )
		{
			a_LogOutput(1, "CECGameRun::ImportSoundStringTable, Failed to read from file at line=%d", sf.GetCurLine());
			return false;
		}
		
		if (m_SoundStringTable.find(idString) != m_SoundStringTable.end())
		{
			a_LogOutput(1, "CECGameRun::ImportSoundStringTable, repeated id=%d at line=%d", idString, sf.GetCurLine());
			return false;
		}
		
		m_SoundStringTable[idString] = sf.m_szToken;
	}
	
	sf.Close();
	return true;
}

const AString & CECGameRun::GetSoundStringFromTable(int idString)
{
	return m_SoundStringTable[idString];
}

AMSoundBuffer * CECGameRun::GetOrLoadSoundFromTable(int idString)
{
	AMSoundBuffer * pSBuffer = NULL;

	AString strFile = GetSoundStringFromTable(idString);
	strFile.TrimLeft();
	strFile.TrimRight();
	strFile.MakeLower();
	if (!strFile.IsEmpty())
	{
		SoundTable::iterator it = m_SoundTable.find(strFile);
		if (it != m_SoundTable.end())
		{
			pSBuffer = it->second;
		}
		else
		{
			AMSoundBufferMan* pSoundBufMan = g_pGame->GetAMSoundBufferMan();
			pSBuffer = pSoundBufMan->Load2DSound(strFile);
			m_SoundTable[strFile] = pSBuffer;
		}
	}

	return pSBuffer;
}

bool CECGameRun::GetStageInfo(int id,ACString& name,ACString& desc,float& progress)
{
	DATA_TYPE DataType;
	elementdataman *pDataMan = g_pGame->GetElementDataMan();
	HISTORY_STAGE_CONFIG* pStage = (HISTORY_STAGE_CONFIG*)pDataMan->get_data_ptr(id,ID_SPACE_CONFIG,DataType);
	if (pStage)
	{
		name = pStage->name;
		desc = pStage->desc;
		CECGameRun::unique_data* udata = GetUniqueData(pStage->progress_value_id);
		if (udata)
		{
			if(udata->type==1 && pStage->progress_value_goal!=0)
			{
				progress = udata->GetValueAsInt() / (float)pStage->progress_value_goal;	
			}
			else if (udata->type ==2)
			{
				progress = (float)udata->GetValueAsDouble() / pStage->progress_value_goal;				
			}
			else
				progress = 0;

			a_Clamp(progress,0.0f,1.0f);

			return true;
		}
	}
	return false;
}
int CECGameRun::GetStageVarKey(int id)
{
	DATA_TYPE DataType;
	elementdataman *pDataMan = g_pGame->GetElementDataMan();
	HISTORY_STAGE_CONFIG* pStage = (HISTORY_STAGE_CONFIG*)pDataMan->get_data_ptr(id,ID_SPACE_CONFIG,DataType);
	if (pStage)
	{
		return pStage->progress_value_id;
	}
	return -1;
}
int CECGameRun::GetCurStageIndex()
{
	CECGameRun::unique_data* data = GetUniqueData(0);
	if (data)
	{
		if(data->type ==1)
		{
			return data->GetValueAsInt();
		}
	}
	return -1;
}
void CECGameRun::OnUniqueDataBroadcast(GNET::Protocol* p)
{
}

void CECGameRun::OnRandomMap(int row,int col,int* pData)
{
	if(m_pRandomMapProc)
		A3DRELEASE(m_pRandomMapProc);

	m_pRandomMapProc = new CECRandomMapProcess();
	if(m_pRandomMapProc)	
		m_pRandomMapProc->Init(row,col,pData);	
}
