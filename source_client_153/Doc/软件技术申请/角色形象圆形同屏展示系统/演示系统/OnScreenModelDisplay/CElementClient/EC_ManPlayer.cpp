/*
 * FILE: EC_ManPlayer.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/8/30
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma warning (disable: 4284)

#include "EC_Global.h"
#include "EC_ManPlayer.h"
#include "EC_LoginPlayer.h"
#include "EC_Viewport.h"
#include "EC_GPDataType.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_UIManager.h"
#include "EC_World.h"
#include "EC_CDS.h"
#include "EC_Utility.h"
#include "EC_Configs.h"
#include "EC_UIManager.h"
#include "EC_FixedMsg.h"
#include "EC_Resource.h"
#include "EC_SceneLoader.h"
#include "EC_Model.h"
#include "EC_InputCtrl.h"

#include "A3DCamera.h"
#include "A2DSprite.h"
#include "A3DCollision.h"
#include "A3DTerrain2.h"
#include "A2DSpriteBuffer.h"
#include "A3DFont.h"

#define new A_DEBUG_NEW

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

#define MAX_CACHEDPLAYER	1024	//	Maximum cached player number

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


///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECPlayerMan
//	
///////////////////////////////////////////////////////////////////////////

CECPlayerMan::CECPlayerMan(CECGameRun* pGameRun) :
CECManager(pGameRun)
{
	m_iManagerID	= MAN_PLAYER;

	m_QueryCnt.SetPeriod(3000);
	m_CacheCnt.SetPeriod(300000);
	m_TLTimeCnt.SetPeriod(2000);

	InitializeCriticalSection(&m_csLoad);
	InitializeCriticalSection(&m_csPlayerTab);
}

CECPlayerMan::~CECPlayerMan()
{
	DeleteCriticalSection(&m_csLoad);
	DeleteCriticalSection(&m_csPlayerTab);
}

//	Release manager
void CECPlayerMan::Release()
{
	OnLeaveGameWorld();
}

//	On entering game world
bool CECPlayerMan::OnEnterGameWorld()
{
	return true;
}

//	On leaving game world
bool CECPlayerMan::OnLeaveGameWorld()
{	
	//	Release all loaded models
	ACSWrapper csa(&m_csLoad);

	int i;
	for (i=0; i < m_aLoadedModels.GetSize(); i++)
	{
		PLAYERMODEL* pInfo = m_aLoadedModels[i];
		CECPlayer::ReleasePlayerModel(pInfo->Ret);
		delete pInfo;
	}

	m_aLoadedModels.RemoveAll();

	return true;
}

//	Get host player
// CECHostPlayer* CECPlayerMan::GetHostPlayer()
// {
// 	return m_pGameRun->GetHostPlayer();
// }

//	Process message
bool CECPlayerMan::ProcessMessage(const ECMSG& Msg)
{
	return true;
}

//	Get else player by id
// CECElsePlayer* CECPlayerMan::GetElsePlayer(int cid, DWORD dwBornStamp/* 0 */)
// {
// 	ACSWrapper cspt(&m_csPlayerTab);		// 这里加个锁，因为此函数会被后台加载线程调用到
// 	PlayerTable::pair_type Pair = m_PlayerTab.get(cid);
// 	if (!Pair.second)
// 		return NULL;
// 
// 	if (dwBornStamp)
// 	{
// 		CECPlayer* pPlayer = *Pair.first;
// 		if (pPlayer->GetBornStamp() != dwBornStamp)
// 			return NULL;
// 	}
// 
// 	return *Pair.first;
// }

//	Get a player (may be host or else player) by id
// CECPlayer* CECPlayerMan::GetPlayer(int cid, DWORD dwBornStamp/* 0 */)
// {
// 	CECHostPlayer* pHost = GetHostPlayer();
// 	if (pHost && pHost->GetCharacterID() == cid)
// 		return pHost;
// 	else
// 		return GetElsePlayer(cid, dwBornStamp);
// }

//	Seek out else player with specified id, if he doesn't exist, try to get from server
// CECElsePlayer* CECPlayerMan::SeekOutElsePlayer(int cid)
// {
// 	PlayerTable::pair_type Pair = m_PlayerTab.get(cid);
// 	if (!Pair.second)
// 	{
// 		//	Counldn't find this else player, put it into unkonwn player table
// 		m_UkPlayerTab.put(cid, cid);
// 		return NULL;
// 	}
// 
// 	return *Pair.first;
// }
// 
// //	Player information messages handler
// bool CECPlayerMan::OnMsgPlayerInfo(const ECMSG& Msg)
// {
// 	using namespace S2C;
// 
// 	return true;
// }
// 
// //	Handler of MSG_PM_PLAYERMOVE
// bool CECPlayerMan::OnMsgPlayerMove(const ECMSG& Msg)
// {
// 	using namespace S2C;
// 	return true;
// }
// 
// //	Handler of MSG_PM_PLAYERRUNOUT
// bool CECPlayerMan::OnMsgPlayerRunOut(const ECMSG& Msg)
// {
// 	using namespace S2C;
// 
// 	CECGameSession* pSession = g_pGame->GetGameSession();
// 
// 	cmd_leave_slice* pCmd = (cmd_leave_slice*)Msg.dwParam1;
// 	ASSERT(pCmd && pCmd->id != pSession->GetCharacterID());
// 
// 	ElsePlayerLeave(pCmd->id, false);
// 
// 	return true;
// }
// 
// //	Handler of MSG_PM_PLAYEREXIT
// bool CECPlayerMan::OnMsgPlayerExit(const ECMSG& Msg)
// {
// 	using namespace S2C;
// 
// 	return true;
// }
// 
// //	Handler of MSG_PM_PLAYERDIED
// bool CECPlayerMan::OnMsgPlayerDied(const ECMSG& Msg)
// {
// 	using namespace S2C;
// 	
// 	return true;
// }
// 
// //	Handler of MSG_PM_PLAYERREVIVE
// bool CECPlayerMan::OnMsgPlayerRevive(const ECMSG& Msg)
// {
// 	using namespace S2C;
// 	return true;
// }
// 
// bool CECPlayerMan::OnMsgPlayerOutOfView(const ECMSG& Msg)
// {
// 	using namespace S2C;
// 	return true;
// }
// 
// bool CECPlayerMan::OnMsgPlayerDisappear(const ECMSG& Msg)
// {
// 	using namespace S2C;
// 	return true;
// }
// 
// bool CECPlayerMan::OnMsgPlayerStopMove(const ECMSG& Msg)
// {
// 	using namespace S2C;
// 
// 	return true;
// }
// 
// bool CECPlayerMan::OnMsgPlayerLevelUp(const ECMSG& Msg)
// {
// 	using namespace S2C;
// 
// 	return true;
// }
// 
// bool CECPlayerMan::OnMsgGoblinLevelUp(const ECMSG& Msg)
// {
// 	using namespace S2C;
// 	return true;
// }
// 
// bool CECPlayerMan::OnMsgPlayerInvisible(const ECMSG& Msg)
// {
// 	using namespace S2C;
// 	return true;
// }
// 
// bool CECPlayerMan::OnMsgForbidBeSelected(const ECMSG& Msg)
// {
// 	using namespace S2C;
// 	return true;
// }
// 
// 
// bool CECPlayerMan::OnMsgPlayerExtProp(const ECMSG& Msg)
// {
// 	using namespace S2C;
// 	return true;
// }
// 
// bool CECPlayerMan::OnMsgPlayerLeaveTeam(const ECMSG& Msg)
// {
// 	using namespace S2C;
// 	return true;
// }
// 
// bool CECPlayerMan::OnMsgDoConEmote(const ECMSG& Msg)
// {
// 	using namespace S2C;
// 	cmd_do_concurrent_emote* pCmd = (cmd_do_concurrent_emote*)Msg.dwParam1;
// 	ASSERT(pCmd && Msg.dwParam2 == DO_CONCURRENT_EMOTE);
// 
// 	CECHostPlayer* pHost = GetHostPlayer();
// 
// 	if (pCmd->id1)
// 	{
// 		if (pCmd->id1 == pHost->GetCharacterID())
// 			pHost->ProcessMessage(Msg);
// 		else
// 		{
// 			CECElsePlayer* pPlayer = SeekOutElsePlayer(pCmd->id1);
// 			if (pPlayer)
// 				pPlayer->ProcessMessage(Msg);
// 		}
// 	}
// 	
// 	if (pCmd->id2)
// 	{
// 		if (pCmd->id2 == pHost->GetCharacterID())
// 			pHost->ProcessMessage(Msg);
// 		else
// 		{
// 			CECElsePlayer* pPlayer = SeekOutElsePlayer(pCmd->id2);
// 			if (pPlayer)
// 				pPlayer->ProcessMessage(Msg);
// 		}
// 	}
// 	
// 	return true;
// }
// 
// bool CECPlayerMan::OnMsgInvalidObject(const ECMSG& Msg)
// {
// 	using namespace S2C;
// 	cmd_invalid_object* pCmd = (cmd_invalid_object*)Msg.dwParam1;
// 	ASSERT(pCmd);
// 
// 	//	Remove the player if it exists
// 	CECElsePlayer* pPlayer = GetElsePlayer(pCmd->id);
// 	if (pPlayer)
// 	{
// 		ElsePlayerLeave(pCmd->id, false);
// 	}
// 	
// 	return true;
// }
// 
// bool CECPlayerMan::OnMsgPlayerExtState(const ECMSG& Msg)
// {
// 	using namespace S2C;
// 
// 	int cid = 0;
// 
// 	cmd_icon_state_notify cmd;
// 
// 	if (Msg.dwParam2 == UPDATE_EXT_STATE)
// 		cid = ((cmd_update_ext_state*)Msg.dwParam1)->id;
// 	else if (Msg.dwParam2 == ICON_STATE_NOTIFY)
// 	{
// 		if (!cmd.Initialize((const void *)Msg.dwParam1, Msg.dwParam3))
// 		{
// 			ASSERT(0);
// 			return false;
// 		}
// 		cid = cmd.id;
// 	}
// 	else
// 	{
// 		ASSERT(0);
// 		return false;
// 	}
// 
// 	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
// 
// 	if (cid == pHost->GetCharacterID())
// 		pHost->ProcessMessage(Msg);
// 	else
// 	{
// 		CECElsePlayer* pPlayer = SeekOutElsePlayer(cid);
// 		if (pPlayer)
// 			pPlayer->ProcessMessage(Msg);
// 
// 		if (Msg.dwParam2 == ICON_STATE_NOTIFY && pHost->GetTeam())
// 		{
// 			//	Update host's team member's icon state
// 			CECTeam* pTeam = pHost->GetTeam();
// 			CECTeamMember* pMember = pTeam->GetMemberByID(cid);
// 			if (pMember)
// 				pMember->ResetIconStates(cmd.states);
// 		}
// 	}
// 
// 	return true;
// }
// 
// void CECPlayerMan::OnMsgPickUpMatter(const ECMSG& Msg)
// {
// 	using namespace S2C;
// 	cmd_matter_pickup* pCmd = (cmd_matter_pickup*)Msg.dwParam1;
// 	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
// 
// 	if (pCmd->who == pHost->GetCharacterID())
// 		pHost->ProcessMessage(Msg);
// 	else
// 	{
// 		CECElsePlayer* pPlayer = SeekOutElsePlayer(pCmd->who);
// 		if (pPlayer)
// 			pPlayer->ProcessMessage(Msg);
// 	}
// 
// 	//	Remove matter from world
// 	CECMatterMan* pMatterMan = g_pGame->GetGameRun()->GetWorld()->GetMatterMan();
// 	pMatterMan->RemoveMatter(pCmd->matter_id);
// }
// 
// void CECPlayerMan::OnMsgPlayerBindStart(const ECMSG& Msg)
// {
// 	using namespace S2C;
// 	const cmd_player_bind_start* pCmd = (const cmd_player_bind_start*)Msg.dwParam1;
// 
// 	CECPlayer* pPlayer = GetPlayer(pCmd->mule);
// 	if (pPlayer)
// 		pPlayer->OnStartBinding(pCmd->mule, pCmd->rider);
// 
// 	pPlayer = GetPlayer(pCmd->rider);
// 	if (pPlayer)
// 		pPlayer->OnStartBinding(pCmd->mule, pCmd->rider);
// }
// 
// void CECPlayerMan::OnMsgPlayerDuelResult(const ECMSG& Msg)
// {
// 	using namespace S2C;
// 	const cmd_duel_result* pCmd = (const cmd_duel_result*)Msg.dwParam1;
// 	CECGameRun* pGameRun = g_pGame->GetGameRun();
// 
// 	const ACHAR* szName1 = pGameRun->GetPlayerName(pCmd->id1, false);
// 	const ACHAR* szName2 = pGameRun->GetPlayerName(pCmd->id2, false);
// 	if (!szName1 || !szName2)
// 		return;
// 
// 	int r1=0, r2=0;
// 
// 	if (pCmd->result == 0)
// 	{
// 		r1 = r2 = 3;
// 		pGameRun->AddFixedMessage(FIXMSG_DUEL_DRAW, szName1, szName2);
// 	}
// 	else if (pCmd->result == 1)
// 	{
// 		r1 = 1;
// 		r2 = 2;
// 		pGameRun->AddFixedMessage(FIXMSG_DUEL_WIN, szName1, szName2);
// 	}
// 
// 	CECPlayer* pPlayer = GetPlayer(pCmd->id1);
// 	if (pPlayer)
// 		pPlayer->SetDuelResult(r1);
// 
// 	pPlayer = GetPlayer(pCmd->id2);
// 	if (pPlayer)
// 		pPlayer->SetDuelResult(r2);
// }
// 
// void CECPlayerMan::OnMsgTankLeader(const ECMSG& Msg)
// {
// 	using namespace S2C;
// 	const cmd_tank_leader_notify* pCmd = (const cmd_tank_leader_notify*)Msg.dwParam1;
// 
// 	if (pCmd->idLeader)
// 	{
// 		CECPlayer* pPlayer = GetPlayer(pCmd->idLeader);
// 		if (pPlayer)
// 			pPlayer->ChangeTankLeader(pCmd->idTank, true);
// 	}
// 	else
// 	{
// 		CECHostPlayer* pHost = GetHostPlayer();
// 		if (pHost->GetBattleTankNum())
// 			pHost->ChangeTankLeader(pCmd->idTank, false);
// 
// 		//	Handle all else players
// 		PlayerTable::iterator it = m_PlayerTab.begin();
// 		for (; it != m_PlayerTab.end(); ++it)
// 		{
// 			CECElsePlayer* pPlayer = *it.value();
// 			if (pPlayer->GetBattleTankNum())
// 				pPlayer->ChangeTankLeader(pCmd->idTank, false);
// 		}
// 	}
// }
// 
// void CECPlayerMan::OnMsgPlayerChangeSpouse(const ECMSG& Msg)
// {
// 	using namespace S2C;
// 	const player_change_spouse* pCmd = (const player_change_spouse*)Msg.dwParam1;
// 
// 	CECPlayer* pPlayer = GetPlayer(pCmd->who);
// 	if (pPlayer)
// 	{
// 		// See if need add a fixed message here.
// 		if( pPlayer == g_pGame->GetGameRun()->GetHostPlayer() && pPlayer->GetSpouse() && pCmd->spouse == 0 )
// 			g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_GOTDIVORCED, g_pGame->GetGameRun()->GetPlayerName(pPlayer->GetSpouse(), true));
// 		else if( pPlayer == g_pGame->GetGameRun()->GetHostPlayer() && pPlayer->GetSpouse() == 0 && pCmd->spouse )
// 			g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_GOTMARRIED, g_pGame->GetGameRun()->GetPlayerName(pCmd->spouse, true));
// 
// 		pPlayer->SetSpouse(pCmd->spouse);
// 	}
// }

//	Transmit message
bool CECPlayerMan::TransmitMessage(const ECMSG& Msg)
{

	return true;
}

//	Tick routine
bool CECPlayerMan::Tick(DWORD dwDeltaTime)
{
	DWORD dwRealTime = g_pGame->GetRealTickTime();

	//	Deliver loaded player models
	DeliverLoadedPlayerModels();

	//	Tick all players
// 	PlayerTable::iterator it = m_PlayerTab.begin();
// 	for (; it != m_PlayerTab.end(); ++it)
// 	{
// 		CECElsePlayer* pPlayer = *it.value();
// 		pPlayer->Tick(dwDeltaTime);
// 	}

	//	Tick Login Players here, if there are
	int nNumLoginPlayers = m_LoginPlayers.size();
	for(int i=0; i < nNumLoginPlayers; i++)
	{
		m_LoginPlayers[i]->Tick(dwDeltaTime);
	}

	// UpdateUnknownElsePlayers();

	//	Update player's tank leader state if host is in battle
// 	CECHostPlayer* pHost = GetHostPlayer();
// 	if (pHost && pHost->IsInBattle() && !pHost->IsInFortress() && m_TLTimeCnt.IncCounter(dwRealTime))
// 	{
// 		m_TLTimeCnt.Reset();
// 
// 		if (pHost->GetBattleTankNum())
// 			pHost->UpdateTankLeader();
// 
// 		it = m_PlayerTab.begin();
// 		for (; it != m_PlayerTab.end(); ++it)
// 		{
// 			CECElsePlayer* pPlayer = *it.value();
// 			if (pPlayer->GetBattleTankNum())
// 				pPlayer->UpdateTankLeader();
// 		}
// 	}

	// test code
	/*
	if (GetHostPlayer())
	{
		PlayerTable::iterator it = m_PlayerTab.begin();
		for (; it != m_PlayerTab.end(); ++it)
		{
			CECElsePlayer* pPlayer = *it.value();
			if ((pPlayer->GetPos() - GetHostPlayer()->GetPos()).Normalize() < 2.0f)
			{
				if (pPlayer->GetAttachMode() == CECPlayer::enumAttachNone)
				{
					CECPlayer* p1 = GetHostPlayer();
					CECPlayer* p2 = pPlayer;

					if (p1->GetGender() != p2->GetGender())
					{
						if (p1->GetGender() == GENDER_MALE)
							p1->AttachBuddy(p2->GetPlayerInfo().cid);
						else
							p2->AttachBuddy(p1->GetPlayerInfo().cid);
						break;
					}
				}
			}
		}
	}
	*/

	return true; 
}

//	Render routine
bool CECPlayerMan::Render(CECViewport* pViewport)
{
	//	Render Login Players here, if there are
	int nNumLoginPlayers = m_LoginPlayers.size();
	for(int i=0; i<nNumLoginPlayers; i++)
	{
		m_LoginPlayers[i]->Render(pViewport);
	}

	return true;
}

//	RenderForReflected routine
bool CECPlayerMan::RenderForReflect(CECViewport * pViewport)
{
	//	Render Login Players here, if there are
	int nNumLoginPlayers = m_LoginPlayers.size();
	for(int i=0; i<nNumLoginPlayers; i++)
	{
		m_LoginPlayers[i]->Render(pViewport, CECObject::RD_REFLECT);
	}

	return true;
}

//	RenderForRefract routine
bool CECPlayerMan::RenderForRefract(CECViewport * pViewport)
{
	//	Render Login Players here, if there are
	int nNumLoginPlayers = m_LoginPlayers.size();
	for(int i=0; i<nNumLoginPlayers; i++)
	{
		m_LoginPlayers[i]->Render(pViewport, CECObject::RD_REFRACT);
	}

	return true;
}

//	Host information arrived
// bool CECPlayerMan::HostPlayerInfo1(const S2C::cmd_self_info_1& Info)
// {
// 	//	Set world load center
// 	ASSERT(m_pGameRun);
// 	const A3DVECTOR3& vInitLoadPos = m_pGameRun->GetWorld()->GetInitLoadPos();
// 	if (vInitLoadPos.x != Info.pos.x || vInitLoadPos.z != Info.pos.z)
// 		m_pGameRun->GetWorld()->SetLoadCenter(Info.pos);
// 
// 	CECHostPlayer* pHost = GetHostPlayer();
// 
// 	if (!pHost->Init(Info))
// 	{
// 		a_LogOutput(1, "CECPlayerMan::HostPlayerInfo1, Failed to initialize host player");
// 		return false;
// 	}
// 
// 	int nFaction = pHost->GetFactionID();
// 	if (nFaction)
// 	{
// 		g_pGame->GetGameSession()->GetFactionInfo(1, &nFaction);
// 		// g_pGame->GetGameSession()->faction_player_info();
// 	}
// 
// 	//	Update the camera which is associated with host player
// 	//	TODO: Directly get viewport here isn't a good way. Maybe we should
// 	//	find a way to know which camera is associated with host player.
// 	TickHostPlayer(0, g_pGame->GetViewport());
// 
// 	return true;
// }
// 
// //	One else player enter view area
// CECElsePlayer* CECPlayerMan::ElsePlayerEnter(const S2C::info_player_1& Info, int iCmd)
// {
// 	//	If this player's id is in unknown table, remove it because this player
// 	//	won't be unknown anymore
// 	UkPlayerTable::pair_type Pair = m_UkPlayerTab.get(Info.cid);
// 	if (Pair.second)
// 		m_UkPlayerTab.erase(Info.cid);
// 
// 	int iAppearFlag = (iCmd == S2C::PLAYER_ENTER_WORLD) ? CECElsePlayer::APPEAR_ENTERWORLD : CECElsePlayer::APPEAR_RUNINTOVIEW;
// 
// 	//	Has player been in active player table ?
// 	CECElsePlayer* pPlayer = GetElsePlayer(Info.cid);
// 	if (pPlayer)
// 	{
// 		//	This player has existed in player table, call special initial function
// 		pPlayer->Init(Info, iAppearFlag, true);
// 		return pPlayer;
// 	}
// 
// 	//	Create a new player
// 	if (!(pPlayer = CreateElsePlayer(Info, iAppearFlag)))
// 	{
// 		a_LogOutput(1, "CECPlayerMan::ElsePlayerEnter, Failed to create player (%d)", Info.cid);
// 		return NULL;
// 	}
// 
// 	//	Add player to table
// 	ACSWrapper cspt(&m_csPlayerTab);
// 	m_PlayerTab.put(Info.cid, pPlayer);
// 	g_pGame->GetGameRun()->GetMemSimplify()->OnPlayerEnter(pPlayer);
// 
// 	return pPlayer;
// }
// 
// //	One else player leave view area or exit game
// void CECPlayerMan::ElsePlayerLeave(int cid, bool bExit, bool bUpdateMMArray/* true */)
// {
// 	//	Player has been in active player table ?
// 	CECElsePlayer* pPlayer = GetElsePlayer(cid);
// 	CECHostPlayer* pHost = GetHostPlayer();
// 
// 	//	Remove player from m_aMMPlayers array
// 	if (pPlayer)
// 	{
// 		if (bUpdateMMArray)
// 			RemovePlayerFromMiniMap(pPlayer);
// 
// 		pPlayer->m_iMMIndex = -1;
// 
// 		//	If this player is selected by host, cancel the selection
// 		if (pPlayer->GetCharacterID() == pHost->GetSelectedTarget())
// 			pHost->SelectTarget(0);
// 		
// 		pHost->RemoveObjectFromTabSels(pPlayer);
// 	}
// 
// 	// 删除正在进行中的PlayerBaseInfo请求
// 	CECC2SCmdCache* pCache = g_pGame->GetGameSession()->GetC2SCmdCache();
// 	pCache->RemovePlayerBaseInfo(cid);
// 	if (bExit)	//	Player exit game
// 	{
// 		if (!pPlayer)
// 		{
// 			//	Release player data in cache
// 			CECEPCacheData* pData = PopPlayerFromCache(cid);
// 			if (pData)
// 				delete pData;
// 		}
// 		else
// 		{
// 			//	Remove from active player table
// 			ACSWrapper cspt(&m_csPlayerTab);
// 			m_PlayerTab.erase(cid);
// 
// 			QueuePlayerUndoLoad(cid);
// 		}
// 	}
// 	else	//	Player leave view area
// 	{
// 		if (!pPlayer)
// 			return;		//	Counldn't find this player in view area
// 
// 		//	If this player is a member of host player's team, remain it
// 		//	in player list !
// 		//	Note: there are no players in ghost mode anymore !
// 	/*	if (pHost && pHost->IsTeamMember(cid))
// 		{
// 			pPlayer->TurnToGhost();
// 			return;
// 		}	*/
// 
// 		//	This player shouldn't be in cache
// 		ASSERT(!IsPlayerInCache(cid));
// 
// 		//	Push player data into cache
// 		CECEPCacheData* pCacheData = pPlayer->CreateCacheData();
// 		if (pCacheData)
// 			PushPlayerToCache(pCacheData);
// 
// 		//	Remove from active player table
// 		{
// 			ACSWrapper cspt(&m_csPlayerTab);
// 			m_PlayerTab.erase(cid);
// 		}
// 
// 		QueuePlayerUndoLoad(cid);
// 	}
// 
// 	//	Release player object
// 	ReleaseElsePlayer(pPlayer);
// }

//	Create else player
// CECElsePlayer* CECPlayerMan::CreateElsePlayer(const S2C::info_player_1& Info, int iAppearFlag)
// {
// 	CECElsePlayer* pPlayer = new CECElsePlayer(this);
// 	if (!pPlayer)
// 	{
// 		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECPlayerMan::CreateElsePlayer", __LINE__);
// 		return false;
// 	}
// 
// 	pPlayer->SetBornStamp(g_pGame->GetGameRun()->GetWorld()->GetBornStamp());
// 
// 	//	Try to load player data from cache at first
// 	CECEPCacheData* pCacheData = PopPlayerFromCache(Info.cid);
// 	if (pCacheData)
// 	{
// 		if (!pPlayer->InitFromCache(Info, pCacheData, iAppearFlag))
// 		{
// 			delete pCacheData;
// 			a_LogOutput(1, "CECPlayerMan::CreateElsePlayer, Failed to init player from cache data.");
// 			return NULL;
// 		}
// 
// 		delete pCacheData;
// 	}
// 	else
// 	{
// 		if (!pPlayer->Init(Info, iAppearFlag, false))
// 		{
// 			glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECPlayerMan::CreateElsePlayer", __LINE__);
// 			return false;
// 		}
// 	}
// 
// 	return pPlayer;
// }

//	Release else player
// void CECPlayerMan::ReleaseElsePlayer(CECElsePlayer* pPlayer)
// {
// 	if (pPlayer)
// 	{
// 		//	Remove tab-selected array
// 		CECHostPlayer* pHost = GetHostPlayer();
// 		if (pHost)
// 		pHost->RemoveObjectFromTabSels(pPlayer);
// 		g_pGame->GetGameRun()->GetMemSimplify()->OnPlayerLeave(pPlayer);
// 
// 		pPlayer->Release();
// 		delete pPlayer;
// 		pPlayer = NULL;
// 	}
// }

//	Get else player cache data
// CECEPCacheData* CECPlayerMan::GetPlayerCacheData(int cid)
// {
// 	CacheTable::pair_type Pair = m_PlayerCache.get(cid);
// 	return Pair.second ? *Pair.first : NULL;
// }
// 
// //	Pop player data from cache
// CECEPCacheData* CECPlayerMan::PopPlayerFromCache(int cid)
// {
// 	CacheTable::pair_type Pair = m_PlayerCache.get(cid);
// 	if (!Pair.second)
// 		return NULL;
// 
// 	CECEPCacheData* pData = *Pair.first;
// 	ASSERT(pData);
// 
// 	//	Remove player from cache
// 	m_PlayerCache.erase(cid);
// 
// 	return pData;
// }
// 
// //	Push player to cache
// bool CECPlayerMan::PushPlayerToCache(CECEPCacheData* pData)
// {
// 	CacheTable::pair_type Pair = m_PlayerCache.get(pData->m_cid);
// 	if (Pair.second)
// 	{
// 		//	This case shouldn't happen, but we still handle it.
// 		CECEPCacheData* pTemp = *Pair.first;
// 		delete pTemp;
// 		m_PlayerCache.erase(pData->m_cid);
// 	}
// 
// 	pData->m_dwCacheTime = (DWORD)(a_GetTime() * 0.001f);
// 	m_PlayerCache.put(pData->m_cid, pData);
// 	return true;
// }
// 
// //	Clear up player data cache
// void CECPlayerMan::ClearUpPlayerCache()
// {
// 	if (m_PlayerCache.size() < MAX_CACHEDPLAYER)
// 		return;
// 
// 	DWORD dwCurTime = a_GetTime() / 1000;
// 
// 	//	Release some old enough cached data
// 	int aIDs[50], iCount=0;
// 	
// 	CacheTable::iterator it = m_PlayerCache.begin();
// 	for (; it != m_PlayerCache.end(); ++it)
// 	{
// 		CECEPCacheData* pData = *it.value();
// 		if (pData->m_dwCacheTime + 240 < dwCurTime)
// 		{
// 			aIDs[iCount++] = pData->m_cid;			
// 			delete pData;
// 			if (iCount >= 50)
// 				break;
// 		}
// 	}
// 
// 	for (int i=0; i < iCount; i++)
// 		m_PlayerCache.erase(aIDs[i]);
// }

//	Update players in various ranges (Active, visible, mini-map etc.)
// void CECPlayerMan::UpdatePlayerInRanges(DWORD dwDeltaTime)
// {
// 	CECHostPlayer* pHost = GetHostPlayer();
// 	if (!pHost || !pHost->IsAllResReady())
// 		return;
// 
// 	CECConfigs* pConfigs = g_pGame->GetConfigs();
// 	int idHoverObject = pHost->GetCursorHoverObject();
// 	int idSelected = pHost->GetSelectedTarget();
// 
// 	//	Get current player visible radius
// 	float fPVRadius = pConfigs->GetPlayerVisRadius();
// 	if (!pConfigs->GetVideoSettings().bModelLimit)
// 		fPVRadius = pConfigs->GetSevActiveRadius();
// 
// 	m_aMMPlayers.RemoveAll(false);
// 
// 	//	Check all else players
// 	PlayerTable::iterator it = m_PlayerTab.begin();
// 	while(it != m_PlayerTab.end())
// 	{
// 		CECElsePlayer* pPlayer = *it.value();
// 		float fDistToHostH = pPlayer->GetDistToHostH();
// 		float fDistToHost = pPlayer->GetDistToHost();
// 		float fDistToCamera = pPlayer->GetDistToCamera();
// 
// 		//	Check player's visiblity
// 		if( pConfigs->GetVideoSettings().bModelLimit )
// 			pPlayer->SetUseHintModelFlag(fDistToHost <= fPVRadius ? false : true);
// 		else
// 			pPlayer->SetUseHintModelFlag(false);
// 
// 		//	Check whether name is visible
// 		if (fDistToHost <= 20.0f || pPlayer->GetCharacterID() == idHoverObject ||
// 			pPlayer->GetCharacterID() == idSelected)
// 			pPlayer->SetRenderNameFlag(true);
// 		else
// 			pPlayer->SetRenderNameFlag(false);
// 
// 		//	Set bars visible flags
// 		pPlayer->SetRenderBarFlag(pPlayer->GetCharacterID() == idSelected);
// 
// 		//	Check whether player is in mini-map
// 		if (fDistToHostH > pConfigs->GetSevActiveRadius() && !pHost->IsDeferedUpdateSlice())
// 		{		 
// 			// backup next item before delete it.
// 			++it;
// 			
// 			ElsePlayerLeave(pPlayer->GetCharacterID(), false, false);
// 			continue;
// 		}
// 		else if (!pPlayer->InGhostState() && fDistToHostH <= pConfigs->GetSevActiveRadius())
// 			pPlayer->m_iMMIndex = m_aMMPlayers.Add(pPlayer);
// 
// 		 ++it;
// 	}
// }
// 
// //	Remove else player from m_aMMPlayers array
// void CECPlayerMan::RemovePlayerFromMiniMap(CECElsePlayer* pPlayer)
// {
// 	ASSERT(pPlayer);
// 	int iIndex = pPlayer->m_iMMIndex;
// 	if (iIndex < 0 || iIndex >= m_aMMPlayers.GetSize())
// 		return;
// 
// 	CECElsePlayer* pTempPlayer = m_aMMPlayers[iIndex];
// 	if (pTempPlayer != pPlayer)
// 	{
// 		ASSERT(pTempPlayer == pPlayer);
// 		return;
// 	}
// 
// 	//	Exchange with the last layer in array
// 	int iLastIdx = m_aMMPlayers.GetSize() - 1;
// 	if (m_aMMPlayers.GetSize() > 1 && iIndex != iLastIdx)
// 	{
// 		pTempPlayer = m_aMMPlayers[iLastIdx];
// 		m_aMMPlayers[iIndex] = pTempPlayer;
// 		pTempPlayer->m_iMMIndex = iIndex;
// 		m_aMMPlayers.RemoveAt(iLastIdx);
// 	}
// 	else
// 		m_aMMPlayers.RemoveAtQuickly(iIndex);
// }

//	When world's loading center changed
void CECPlayerMan::OnLoadCenterChanged()
{
}

//	Ray trace
bool CECPlayerMan::RayTrace(ECRAYTRACE* pTraceInfo)
{
	//	If shift is pressed, ignore all players
	if (pTraceInfo->dwKeyStates & EC_KSF_SHIFT)
		return false;

	bool bRet = false;

/*	Trace host player heself

	if (m_pHostPlayer &&
		(pTraceInfo->iTraceObj == TRACEOBJ_LBTNCLICK || 
		pTraceInfo->iTraceObj == TRACEOBJ_TESTRAY))
	{
		//	Trace host player
		const A3DAABB* paabb = &m_pHostPlayer->GetPlayerAABB();
		if (CLS_RayToAABB3(pTraceInfo->vStart, pTraceInfo->vDelta, paabb->Mins, paabb->Maxs,
					vHitPos, &fFracion, vNormal))
		{
			if (fFracion < pTraceInfo->pTraceRt->fFraction)
			{
				pTraceInfo->pTraceRt->fFraction		= fFracion;
				pTraceInfo->pTraceRt->vHitPos		= vHitPos;
				pTraceInfo->pTraceRt->vPoint		= vHitPos;
				pTraceInfo->pTraceRt->vNormal		= vNormal;

				pTraceInfo->pECTraceRt->fFraction	= fFracion;
				pTraceInfo->pECTraceRt->iEntity		= ECENT_PLAYER;
				pTraceInfo->pECTraceRt->iObjectID	= m_pHostPlayer->GetCharacterID();
				pTraceInfo->pECTraceRt->vNormal		= vNormal;

				bRet = true;
			}
		}
	}
*/
	
	//	Trace all else player
	

	return bRet;
}

//	Update unknown else player table
// void CECPlayerMan::UpdateUnknownElsePlayers()
// {
// 	DWORD dwRealTime = g_pGame->GetRealTickTime();
// 	if (!m_QueryCnt.IncCounter(dwRealTime))
// 		return;
// 
// 	m_QueryCnt.Reset();
// 
// 	//	Query unknown player's information
// 	if (m_UkPlayerTab.size())
// 	{
// 		AArray<int, int> aIDs;
// 
// 		UkPlayerTable::iterator it = m_UkPlayerTab.begin();
// 		for (; it != m_UkPlayerTab.end(); ++it)
// 			aIDs.Add(*it.value());
// 
// 		g_pGame->GetGameSession()->c2s_CmdQueryPlayerInfo1(aIDs.GetSize(), aIDs.GetData());
// 
// 		m_UkPlayerTab.clear();
// 	}
// 
// 	//	Query booth name
// 	if (m_aBoothQuery.GetSize())
// 	{
// 		g_pGame->GetGameSession()->c2s_CmdQueryBoothName(m_aBoothQuery.GetSize(), m_aBoothQuery.GetData());
// 		m_aBoothQuery.RemoveAll(false);
// 	}
// }

//	Load player model in loading thread
bool CECPlayerMan::ThreadLoadPlayerModel(
	int cid,
	DWORD dwBornStamp,
	int iProfession,
	int iGender,
	int iCustom,
	const int* pEquips,
	const char* szPetPath,
	bool bClient)
{
	PLAYERMODEL* pInfo = new PLAYERMODEL;
	memset(pInfo, 0, sizeof(*pInfo));
	pInfo->cid = cid;
	pInfo->dwBornStamp = dwBornStamp;
	pInfo->bClientModel = bClient;    // 纯客户端模型

	bool bSimpleFace = false, bSimpleModel = false;

// 	CECPlayer* pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetPlayer(cid);
// 	if( g_pGame->GetGameRun()->GetGameState() == CECGameRun::GS_GAME && pPlayer )
// 	{
// 		bSimpleFace = !pPlayer->ShouldUseFaceModel();
// 		bSimpleModel = !pPlayer->ShouldUseClothedModel();
// 	}

	if (!CECPlayer::LoadPlayerModel(iProfession, iGender, iCustom, pEquips, szPetPath, pInfo->Ret, bSimpleFace, bSimpleModel))
	{
		delete pInfo;
		return false;
	}

	ACSWrapper csa(&m_csLoad);
	m_aLoadedModels.Add(pInfo);

	return true;
}

bool CECPlayerMan::ThreadLoadPlayerEquips(int cid, DWORD dwBornStamp, int iProfession, int iGender, const int* pEquips, __int64 EquipMask)
{
	PLAYERMODEL* pInfo = new PLAYERMODEL;
	memset(pInfo, 0, sizeof(*pInfo));
	pInfo->cid = cid;
	pInfo->dwBornStamp = dwBornStamp;
	pInfo->Ret.dwValidMask |= PLAYERLOADRESULT_EQUIPMODEL;

	bool bSimpleModel = false;
// 	CECPlayer* pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetPlayer(cid);
// 	if( pPlayer && !pPlayer->ShouldUseClothedModel() )
// 		bSimpleModel = true;

	if (!CECPlayer::LoadPlayerEquips(iProfession, iGender, EquipMask, pEquips, pInfo->Ret.EquipResult, bSimpleModel))
	{
		delete pInfo;
		return false;
	}

	ACSWrapper csa(&m_csLoad);
	m_aLoadedModels.Add(pInfo);	

	return true;
}

bool CECPlayerMan::ThreadLoadPetModel(int cid, DWORD dwBornStamp, const char* szPetPath)
{
	PLAYERMODEL* pInfo = new PLAYERMODEL;
	memset(pInfo, 0, sizeof(*pInfo));
	pInfo->cid = cid;
	pInfo->dwBornStamp = dwBornStamp;
	pInfo->Ret.dwValidMask |= PLAYERLOADRESULT_PETMODEL;

	if (!CECPlayer::LoadPetModel(szPetPath, &pInfo->Ret.pPetModel))
	{
		delete pInfo;
		return false;
	}

	ACSWrapper csa(&m_csLoad);
	m_aLoadedModels.Add(pInfo);

	return true;
}

bool CECPlayerMan::ThreadLoadDummyModel(int cid, DWORD dwBornStamp, int iShape,bool bClient)
{
	PLAYERMODEL* pInfo = new PLAYERMODEL;
	memset(pInfo, 0, sizeof(*pInfo));
	pInfo->cid = cid;
	pInfo->dwBornStamp = dwBornStamp;
	pInfo->bClientModel = bClient;
	pInfo->Ret.dwValidMask |= PLAYERLOADRESULT_DUMMYMODEL;
	pInfo->Ret.iShape = iShape;
	
	if (!CECPlayer::LoadDummyModel(iShape, &pInfo->Ret.pDummyModel))
	{
		delete pInfo;
		return false;
	}
	
	ACSWrapper csa(&m_csLoad);
	m_aLoadedModels.Add(pInfo);
	
	return true;
}

bool CECPlayerMan::ThreadLoadPlayerFace(int cid, DWORD dwBornStamp, int iProfession, int iGender, int iCustom)
{
	PLAYERMODEL* pInfo = new PLAYERMODEL;
	memset(pInfo, 0, sizeof(*pInfo));
	pInfo->cid = cid;
	pInfo->dwBornStamp = dwBornStamp;
	pInfo->Ret.dwValidMask |= PLAYERLOADRESULT_PLAYERFACE;

	if (!(pInfo->Ret.pFaceModel = CECPlayer::ThreadLoadFaceModel(iProfession, iGender, iCustom)))
	{
		delete pInfo;
		return false;
	}

	ACSWrapper csa(&m_csLoad);
	m_aLoadedModels.Add(pInfo);

	return true;
}

//	Deliver loaded player models
void CECPlayerMan::DeliverLoadedPlayerModels()
{
	ACSWrapper csa(&m_csLoad);

// 	for (int i=0; i < m_aLoadedModels.GetSize(); i++)
// 	{
// 		PLAYERMODEL* pInfo = m_aLoadedModels[i];
// 
// 		CECPlayer* pPlayer = NULL;
// 		if (pInfo->bClientModel && GetHostPlayer())		
// 			pPlayer = GetHostPlayer()->GetNavigatePlayer();			
// 		else		
// 			pPlayer = GetPlayer(pInfo->cid, pInfo->dwBornStamp);		
// 
// 		if (!pPlayer || !pPlayer->SetPlayerLoadedResult(pInfo->Ret))
// 		{
// 			CECPlayer::ReleasePlayerModel(pInfo->Ret);
// 		}
// 
// 		delete pInfo;
// 	}

	m_aLoadedModels.RemoveAll(false);
}


//	Get player candidates whom can be auto-selected by 'TAB' key
// void CECPlayerMan::TabSelectCandidates(int idCurSel, APtrArray<CECElsePlayer*>& aCands)
// {
// 	CECHostPlayer* pHost = GetHostPlayer();
// 	if (!pHost->IsSkeletonReady()){
// 		//	只有 IsSkeletonReady() 为 true 时 GetDistToHost() 才有效
// 		return;
// 	}
// 
// 	//	Trace all Else Players
// 	PlayerTable::iterator it = m_PlayerTab.begin();
// 	for (; it != m_PlayerTab.end(); ++it)
// 	{
// 		CECElsePlayer* pPlayer = *it.value();
// 		if (!pPlayer->IsSelectable() ||
// 			pPlayer->IsDead() ||
// 			pPlayer->GetCharacterID() == idCurSel || 
// 			pHost->AttackableJudge(pPlayer->GetCharacterID(), false) != 1)
// 			continue;
// 
// 		float fDist = pPlayer->GetDistToHost();
// 		if (fDist > EC_TABSEL_DIST || !CECHostPlayer::CanSafelySelectWith(fDist))
// 			continue;	//	Target is too far
// 
// 		aCands.Add(pPlayer);
// 	}
// }

// void CECPlayerMan::OptimizeShowExtendStates()
// {
// 	//	根据当前优化设置，更新 NPC 上的 Extend States 光效显示
// 	PlayerTable::iterator it = m_PlayerTab.begin();
// 	for (; it != m_PlayerTab.end(); ++it)
// 	{
// 		CECElsePlayer* pPlayer = *it.value();
// 		pPlayer->OptimizeShowExtendStates();
// 	}
// 	CECHostPlayer *pHost = GetHostPlayer();
// 	pHost->OptimizeShowExtendStates();
// }
// 
// void CECPlayerMan::OptimizeWeaponStoneGfx()
// {
// 	//	根据当前优化设置，更新 NPC 上的 Extend States 光效显示
// 	PlayerTable::iterator it = m_PlayerTab.begin();
// 	for (; it != m_PlayerTab.end(); ++it)
// 	{
// 		CECElsePlayer* pPlayer = *it.value();
// 		pPlayer->OptimizeWeaponStoneGfx();
// 	}
// 	CECHostPlayer *pHost = GetHostPlayer();
// 	pHost->OptimizeWeaponStoneGfx();
// }
// 
// 
// void CECPlayerMan::OptimizeArmorStoneGfx()
// {
// 	//	根据当前优化设置，更新 NPC 上的 Extend States 光效显示
// 	PlayerTable::iterator it = m_PlayerTab.begin();
// 	for (; it != m_PlayerTab.end(); ++it)
// 	{
// 		CECElsePlayer* pPlayer = *it.value();
// 		pPlayer->OptimizeArmorStoneGfx();
// 	}
// 	CECHostPlayer *pHost = GetHostPlayer();
// 	pHost->OptimizeArmorStoneGfx();
// }
// 
// 
// void CECPlayerMan::OptimizeSuiteGfx()
// {
// 	//	根据当前优化设置，更新 NPC 上的 Extend States 光效显示
// 	PlayerTable::iterator it = m_PlayerTab.begin();
// 	for (; it != m_PlayerTab.end(); ++it)
// 	{
// 		CECElsePlayer* pPlayer = *it.value();
// 		pPlayer->OptimizeSuiteGfx();
// 	}
// 	CECHostPlayer *pHost = GetHostPlayer();
// 	pHost->OptimizeSuiteGfx();
// }
// 
// CECElsePlayer* CECPlayerMan::GetMouseOnPateTextPlayer( int x, int y )
// {
// 	CECElsePlayer* result = NULL;
// 	PlayerTable::iterator it = m_PlayerTab.begin();
// 	for (; it != m_PlayerTab.end(); ++it)
// 	{
// 		CECElsePlayer* pPlayer = *it.value();
// 		if (pPlayer->IsClickBoothBar(x, y)){
// 			if(result == NULL)
// 				result = pPlayer;
// 			if (result->GetDistToCamera() > pPlayer->GetDistToCamera()){
// 				result = pPlayer;
// 			}
// 		}
// 	}
// 	return result;
// }

// void CECPlayerMan::OnModelChange(const CECFactionMan *p, const CECObservableChange *q){
// 	const CECFactionManChange *pChange = dynamic_cast<const CECFactionManChange *>(q);
// 	if (!pChange){
// 		ASSERT(false);
// 	}else if (pChange->ChangeMask() != CECFactionManChange::FACTION_RENAMEFACTION){
// 		return;
// 	}
// 	//	HostPlayer
// 	const CECFactionManFactionChange *pFactionChange = dynamic_cast<const CECFactionManFactionChange *>(pChange);
// 	if (CECHostPlayer *pHost = GetHostPlayer()){
// 		if (pHost->GetFactionID() == pFactionChange->FactionID()){
// 			pHost->OnFactionNameChange();
// 		}
// 	}
// 	//	ElsePlayer
// 	for (PlayerTable::iterator it = m_PlayerTab.begin(); it != m_PlayerTab.end(); ++it){
// 		CECElsePlayer* pPlayer = *it.value();
// 		if (pPlayer->GetFactionID() != pFactionChange->FactionID()){
// 			continue;
// 		}
// 		pPlayer->OnFactionNameChange();
// 	}
// }