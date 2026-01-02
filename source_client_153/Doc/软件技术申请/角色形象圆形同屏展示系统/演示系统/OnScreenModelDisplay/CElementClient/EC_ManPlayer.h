/*
 * FILE: EC_ManPlayer.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/8/30
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "EC_Manager.h"
#include "EC_GPDataType.h"
#include "EC_Counter.h"
#include "AArray.h"
#include "hashtab.h"
#include "vector.h"
#include "EC_Player.h"

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////

class CECPlayer;
class CECLoginPlayer;
class CECModel;
class A3DSkinModel;

#define PLAYERLOADRESULT_PLAYERMODEL	0x1
#define PLAYERLOADRESULT_PETMODEL		0x2
#define PLAYERLOADRESULT_EQUIPMODEL		0x4
#define PLAYERLOADRESULT_DUMMYMODEL		0x8
#define PLAYERLOADRESULT_PLAYERFACE		0x10

//	Player load result structure used by CECPlayer::LoadPlayerModel
//	dwValidMask should be set to match the data in this structure
struct EC_PLAYERLOADRESULT
{
	DWORD		dwValidMask;
	CECModel*	pPlayerModel;
	int			iShape;
	CECModel*	pDummyModel;
	CECModel*	pPetModel;
	CECFace*	pFaceModel;
	A3DShader*	pBodyShaders[3];
	CECModel*	pFlyNviagteModel;
	CECPlayer::EquipsLoadResult	EquipResult;
};

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECPlayerMan
//	
///////////////////////////////////////////////////////////////////////////

class CECPlayerMan : public CECManager
{
public:		//	Types

	//	Loaded player model information
	struct PLAYERMODEL
	{
		int	cid;					//	Player's character ID
		DWORD dwBornStamp;
		bool bClientModel;
		EC_PLAYERLOADRESULT Ret;	//	Model loaded result
	};

	typedef abase::vector<CECLoginPlayer *>	LoginPlayerTable;

public:		//	Constructor and Destructor

	CECPlayerMan(CECGameRun* pGameRun);
	virtual ~CECPlayerMan();

public:		//	Attributes

public:		//	Operations

	//	Release manager
	virtual void Release();

	//	Tick routine
	virtual bool Tick(DWORD dwDeltaTime);
	//	Render routine
	virtual bool Render(CECViewport* pViewport);
	//	RenderForReflected routine
	virtual bool RenderForReflect(CECViewport * pViewport);
	//	RenderForRefract routine
	virtual bool RenderForRefract(CECViewport * pViewport);

	//	Process message
	virtual bool ProcessMessage(const ECMSG& Msg);

	//	On entering game world
	virtual bool OnEnterGameWorld();
	//	On leaving game world
	virtual bool OnLeaveGameWorld();

	//	Ray trace
	virtual bool RayTrace(ECRAYTRACE* pTraceInfo);
	
	inline LoginPlayerTable& GetLoginPlayerTable() { return m_LoginPlayers; }

	//	Get a player (may be host or else player) by id
	CECPlayer* GetPlayer(int cid, DWORD dwBornStamp=0);
	//	Query player's booth name
	void QueryPlayerBoothName(int idPlayer) { m_aBoothQuery.Add(idPlayer); }

	//	When world's loading center changed
	void OnLoadCenterChanged();

	//	Load player model in loading thread
	bool ThreadLoadPlayerModel(int cid, DWORD dwBornStamp, int iProfession, int iGender, int iCustom, const int* pEquips, const char* szPetPath,bool bClient);
	bool ThreadLoadPlayerEquips(int cid, DWORD dwBornStamp, int iProfession, int iGender, const int* pEquips, __int64 EquipMask);
	bool ThreadLoadPetModel(int cid, DWORD dwBornStamp, const char* szPetPath);
	bool ThreadLoadDummyModel(int cid, DWORD dwBornStamp, int iShape,bool bClient );
	bool ThreadLoadPlayerFace(int cid, DWORD dwBornStamp, int iProfession, int iGender, int iCustom);

	void OptimizeShowExtendStates();
	void OptimizeWeaponStoneGfx();
	void OptimizeArmorStoneGfx();
	void OptimizeSuiteGfx();

protected:	//	Attributes

	CECCounter			m_QueryCnt;		//	Query unknown player time counter
	CECCounter			m_CacheCnt;		//	Cache garbage collector time counter
	CECCounter			m_TLTimeCnt;	//	Tank leader update time counter

	LoginPlayerTable	m_LoginPlayers;	//	Players in login UI, just for render here

	APtrArray<PLAYERMODEL*>		m_aLoadedModels;	//	Loaded player models
	CRITICAL_SECTION			m_csLoad;			//	Loaded models lock
	CRITICAL_SECTION			m_csPlayerTab;		//  Player table lock
	AArray<int, int>			m_aBoothQuery;		//	Used to query player's booth name

protected:	//	Operations

	//	Deliver loaded player models
	void DeliverLoadedPlayerModels();

	//	Messages handlers
	bool OnMsgPlayerInfo(const ECMSG& Msg);
	bool OnMsgPlayerMove(const ECMSG& Msg);
	bool OnMsgPlayerRunOut(const ECMSG& Msg);
	bool OnMsgPlayerExit(const ECMSG& Msg);
	bool OnMsgPlayerDied(const ECMSG& Msg);
	bool OnMsgPlayerRevive(const ECMSG& Msg);
	bool OnMsgPlayerOutOfView(const ECMSG& Msg);
	bool OnMsgPlayerDisappear(const ECMSG& Msg);
	bool OnMsgPlayerStopMove(const ECMSG& Msg);
	bool OnMsgPlayerLevelUp(const ECMSG& Msg);
	bool OnMsgGoblinLevelUp(const ECMSG& Msg);
	bool OnMsgPlayerInvisible(const ECMSG& Msg);
	bool OnMsgPlayerExtProp(const ECMSG& Msg);
	bool OnMsgPlayerLeaveTeam(const ECMSG& Msg);
	bool OnMsgDoConEmote(const ECMSG& Msg);
	bool OnMsgInvalidObject(const ECMSG& Msg);
	bool OnMsgPlayerExtState(const ECMSG& Msg);
	void OnMsgPickUpMatter(const ECMSG& Msg);
	void OnMsgPlayerBindStart(const ECMSG& Msg);
	void OnMsgPlayerDuelResult(const ECMSG& Msg);
	void OnMsgTankLeader(const ECMSG& Msg);
	void OnMsgPlayerChangeSpouse(const ECMSG& Msg);
	bool OnMsgForbidBeSelected(const ECMSG& Msg);

	//	Transmit message
	bool TransmitMessage(const ECMSG& Msg);
};