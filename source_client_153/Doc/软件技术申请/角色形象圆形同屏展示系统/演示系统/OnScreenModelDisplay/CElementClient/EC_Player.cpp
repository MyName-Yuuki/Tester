/*
 * FILE: EC_Player.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/9/1
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_Player.h"
#include "EC_Model.h"
#include "EC_Face.h"
#include "EC_IvtrTypes.h"
#include "EC_RTDebug.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrScroll.h"
#include "EC_IvtrWeapon.h"
#include "EC_IvtrEquipMatter.h"
#include "EC_Resource.h"
#include "EC_Viewport.h"
#include "EC_Utility.h"
#include "EC_ManDecal.h"
#include "EC_Decal.h"
#include "EC_World.h"
#include "EC_GameRun.h"
#include "EC_ImageRes.h"
#include "EC_GFXCaster.h"
#include "EC_Resource.h"
#include "EC_FixedMsg.h"
#include "EC_ManPlayer.h"
#include "EC_PortraitRender.h"
#include "EC_Configs.h"
#include "EC_SceneLoader.h"
#include "EC_Sprite.h"
#include "EC_UIManager.h"
#include "EL_Precinct.h"
#include "EC_UIConfigs.h"
#include "EC_ProfConfigs.h"
#include "EC_PlayerActionController.h"
#include "EC_PlayerBodyController.h"
#include "EC_ElementDataHelper.h"

#include "elementdataman.h"
#include "ids.hxx"

#include "A2DSprite.h"
#include "A2DSpriteItem.h"
#include "A3DSkin.h"
#include "A3DSkinMan.h"
#include "A3DShaderMan.h"
#include "A3DSkinMesh.h"
#include "A3DShader.h"
#include "A3DSkinModel.h"
#include "A3DSkinModelAct.h"
#include "A3DSkinModelAux.h"
#include "A3DTerrainWater.h"
#include "A3DCamera.h"
#include "A3DSkeleton.h"
#include "A3DBone.h"
#include "A3DFlatCollector.h"
#include "AAssist.h"
#include "AFile.h"
#include "A3DLight.h"
#include <A3DFont.h>
#include <AFI.h>

#include "A3DCombinedAction.h"
#include "A3DGFXFuncs.h"
#include <A3DSKinRender.h>
#include <AIniFile.h>

#define new A_DEBUG_NEW

#define FASHION_POWER		30.0f
#define FASHION_SPECULAR	0xff202020

#define ARMOR_POWER		20.0f
#define ARMOR_SPECULAR	0xffffffff

#define WEAPON_POWER	15.0f
#define WEAPON_SPECULAR	0xff000000//0xffffffff

#define PLAYERMODEL_GETTYPE(iShape) ((iShape & 0xff) >> 6)
#define PLAYERMODEL_GETID(iShape) (iShape & 0x3f)

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
static const char* _left_hand_weapon	= "LeftHand";
static const char* _right_hand_weapon	= "RightHand";
static const char* _wing				= "Wing";
static const char* _wing2				= "Wing2";
const char* _cc_ride					= "CC_Ride";
static const char* _cc_ride2			= "CC_Ride2";
const char* _hh_ride					= "HH_Ride";
const char* _hanger_ride				= "Rider";
static const char* _hanger_ride2		= "Rider2";
static const char* _hanger_hug			= "Hug";
static const char* _hh_bind				= "HH_Bind";
static const char* _cc_bind				= "CC_Bind";

const char* _cc_fenghuolun				= "CC_feijian";
const char* _hh_left_foot				= "HH_左脚";
const char* _hh_right_foot				= "HH_右脚";

static const char* _hh_left_shoulder_weapon	= "HH_leftsword";	//	人背上的武器挂点（左肩）
static const char* _hh_right_shoulder_weapon= "HH_rightsword";	//	人背上的武器挂点（右肩）

static const char* _hh_left_hand_weapon		= "HH_lefthandweapon";	//	人手上的武器挂点（左手）
static const char* _hh_right_hand_weapon	= "HH_righthandweapon";	//	人手上的武器挂点（右手）

static const char* _hh_qiu				=	"HH_qiu";			//	人手上的球挂点（右手）

static const char* _hh_right_hand_nickle_weapon	=	"HH_weapon";		//	人手上胧族月镰挂点（右手）

static const char* _cc_weapon			=	"CC_weapon";		//	武器上的挂点名称
static const char* _cc_qiu				=	"CC_qiu";			//	球上的挂点名称
static const char* _hh_qiu_base			=	"HH_base";			//	球上的特效挂点（右手）

const char* _multiobject_effect[] = {"绿色连线.gfx","红色连线.gfx","黑色连线.gfx",};

static const char* _head_skin[NUM_PROFESSION*NUM_GENDER] = 
{
	"Models\\Players\\形象\\武侠男\\头\\武侠男头",
	"Models\\Players\\形象\\武侠女\\头\\武侠女头",

	"Models\\Players\\形象\\法师男\\头\\法师男头",
	"Models\\Players\\形象\\法师女\\头\\法师女头",

	"Models\\Players\\形象\\巫师男\\头\\巫师男头",
	"Models\\Players\\形象\\巫师女\\头\\巫师女头",

	"",
	"Models\\Players\\形象\\妖精\\头\\妖精头",
	"Models\\Players\\形象\\妖兽男\\头\\妖兽男头",
	"",
	"Models\\Players\\形象\\刺客男\\头\\刺客男头",
	"Models\\Players\\形象\\刺客女\\头\\刺客女头",

	"Models\\Players\\形象\\羽芒男\\头\\羽芒男头",
	"Models\\Players\\形象\\羽芒女\\头\\羽芒女头",

	"Models\\Players\\形象\\羽灵男\\头\\羽灵男头",
	"Models\\Players\\形象\\羽灵女\\头\\羽灵女头",
	
	"Models\\Players\\形象\\剑灵男\\头\\剑灵男头",
	"Models\\Players\\形象\\剑灵女\\头\\剑灵女头",
	
	"Models\\Players\\形象\\魅灵男\\头\\魅灵男头",
	"Models\\Players\\形象\\魅灵女\\头\\魅灵女头",

	"Models\\Players\\形象\\夜影男\\头\\夜影男头",
	"Models\\Players\\形象\\夜影女\\头\\夜影女头",
	
	"Models\\Players\\形象\\月仙男\\头\\月仙男头",
	"Models\\Players\\形象\\月仙女\\头\\月仙女头",
};

static const char* _body_skin[NUM_PROFESSION*NUM_GENDER] = 
{
	"Models\\Players\\形象\\武侠男\\躯干\\武侠男",
	"Models\\Players\\形象\\武侠女\\躯干\\武侠女",

	"Models\\Players\\形象\\法师男\\躯干\\法师男",
	"Models\\Players\\形象\\法师女\\躯干\\法师女",

	"Models\\Players\\形象\\巫师男\\躯干\\巫师男%d",
	"Models\\Players\\形象\\巫师女\\躯干\\巫师女%d",
	"",
	"Models\\Players\\形象\\妖精\\躯干\\妖精%d",
	"Models\\Players\\形象\\妖兽男\\躯干\\妖兽男%d",
	"",
	"Models\\Players\\形象\\刺客男\\躯干\\刺客男%d",
	"Models\\Players\\形象\\刺客女\\躯干\\刺客女%d",

	"Models\\Players\\形象\\羽芒男\\躯干\\羽芒男",
	"Models\\Players\\形象\\羽芒女\\躯干\\羽芒女",

	"Models\\Players\\形象\\羽灵男\\躯干\\羽灵男",
	"Models\\Players\\形象\\羽灵女\\躯干\\羽灵女",

	"Models\\Players\\形象\\剑灵男\\躯干\\剑灵男",
	"Models\\Players\\形象\\剑灵女\\躯干\\剑灵女",
	
	"Models\\Players\\形象\\魅灵男\\躯干\\魅灵男",
	"Models\\Players\\形象\\魅灵女\\躯干\\魅灵女",

	"Models\\Players\\形象\\夜影男\\躯干\\夜影男",
	"Models\\Players\\形象\\夜影女\\躯干\\夜影女",
	
	"Models\\Players\\形象\\月仙男\\躯干\\月仙男",
	"Models\\Players\\形象\\月仙女\\躯干\\月仙女",
};

static const char* _simple_body_skin[NUM_PROFESSION*NUM_GENDER] = 
{
	"Models\\Players\\形象\\武侠男\\躯干\\武侠男简化",
	"Models\\Players\\形象\\武侠女\\躯干\\武侠女简化",
	
	"Models\\Players\\形象\\法师男\\躯干\\法师男简化",
	"Models\\Players\\形象\\法师女\\躯干\\法师女简化",
	
	"Models\\Players\\形象\\巫师男\\躯干\\巫师男简化",
	"Models\\Players\\形象\\巫师女\\躯干\\巫师女简化",
	"",
	"Models\\Players\\形象\\妖精\\躯干\\妖精简化",
	"Models\\Players\\形象\\妖兽男\\躯干\\妖兽男简化%d",
	"",
	"Models\\Players\\形象\\刺客男\\躯干\\刺客男简化",
	"Models\\Players\\形象\\刺客女\\躯干\\刺客女简化",
	
	"Models\\Players\\形象\\羽芒男\\躯干\\羽芒男简化",
	"Models\\Players\\形象\\羽芒女\\躯干\\羽芒女简化",
	
	"Models\\Players\\形象\\羽灵男\\躯干\\羽灵男简化",
	"Models\\Players\\形象\\羽灵女\\躯干\\羽灵女简化",
	
	"Models\\Players\\形象\\剑灵男\\躯干\\剑灵男简化",
	"Models\\Players\\形象\\剑灵女\\躯干\\剑灵女简化",
	
	"Models\\Players\\形象\\魅灵男\\躯干\\魅灵男简化",
	"Models\\Players\\形象\\魅灵女\\躯干\\魅灵女简化",

	"Models\\Players\\形象\\夜影男\\躯干\\夜影男简化",
	"Models\\Players\\形象\\夜影女\\躯干\\夜影女简化",
	
	"Models\\Players\\形象\\月仙男\\躯干\\月仙男简化",
	"Models\\Players\\形象\\月仙女\\躯干\\月仙女简化",
};

static const char* _equipment_skin[NUM_PROFESSION*NUM_GENDER] = 
{
	"Models\\Players\\装备\\男\\%s\\男通用%s",
	"Models\\Players\\装备\\女\\%s\\女通用%s",
	"Models\\Players\\装备\\男\\%s\\男通用%s",
	"Models\\Players\\装备\\女\\%s\\女通用%s",

	"Models\\Players\\装备\\男\\%s\\男通用%s",
	"Models\\Players\\装备\\女\\%s\\女通用%s",
	"",
	"Models\\Players\\装备\\女\\%s\\妖精%s",
	"Models\\Players\\装备\\男\\%s\\妖兽%s",
	"",

	"Models\\Players\\装备\\男\\%s\\男通用%s",
	"Models\\Players\\装备\\女\\%s\\女通用%s",

	"Models\\Players\\装备\\男\\%s\\男通用%s",
	"Models\\Players\\装备\\女\\%s\\女通用%s",
	"Models\\Players\\装备\\男\\%s\\男通用%s",
	"Models\\Players\\装备\\女\\%s\\女通用%s",
	
	"Models\\Players\\装备\\男\\%s\\男通用%s",
	"Models\\Players\\装备\\女\\%s\\女通用%s",
	"Models\\Players\\装备\\男\\%s\\男通用%s",
	"Models\\Players\\装备\\女\\%s\\女通用%s",

	"Models\\Players\\装备\\男\\%s\\男通用%s",
	"Models\\Players\\装备\\女\\%s\\女通用%s",
	"Models\\Players\\装备\\男\\%s\\男通用%s",
	"Models\\Players\\装备\\女\\%s\\女通用%s",
};

static const char* _equipment_default_skin[NUM_PROFESSION*NUM_GENDER] = 
{
	"Models\\Players\\装备\\男\\%s\\武侠男%s",
	"Models\\Players\\装备\\女\\%s\\武侠女%s",
	"Models\\Players\\装备\\男\\%s\\法师男%s",
	"Models\\Players\\装备\\女\\%s\\法师女%s",

	"Models\\Players\\装备\\男\\%s\\巫师男%s",
	"Models\\Players\\装备\\女\\%s\\巫师女%s",
	"",
	"Models\\Players\\装备\\女\\%s\\妖精%s",
	"Models\\Players\\装备\\男\\%s\\妖兽%s",
	"",

	"Models\\Players\\装备\\男\\%s\\刺客男%s",
	"Models\\Players\\装备\\女\\%s\\刺客女%s",

	"Models\\Players\\装备\\男\\%s\\羽芒男%s",
	"Models\\Players\\装备\\女\\%s\\羽芒女%s",
	"Models\\Players\\装备\\男\\%s\\羽灵男%s",
	"Models\\Players\\装备\\女\\%s\\羽灵女%s",
	
	"Models\\Players\\装备\\男\\%s\\剑灵男%s",
	"Models\\Players\\装备\\女\\%s\\剑灵女%s",
	"Models\\Players\\装备\\男\\%s\\魅灵男%s",
	"Models\\Players\\装备\\女\\%s\\魅灵女%s",

	"Models\\Players\\装备\\男\\%s\\夜影男%s",
	"Models\\Players\\装备\\女\\%s\\夜影女%s",
	"Models\\Players\\装备\\男\\%s\\月仙男%s",
	"Models\\Players\\装备\\女\\%s\\月仙女%s",
};

static const int _skin_alpha_map[] =
{
	SKIN_SORT_DEFAULT,					// SKIN_BODY_INDEX
	SKIN_SORT_UPPER,					// SKIN_UPPER_BODY_INDEX
	SKIN_SORT_WRIST,					// SKIN_WRIST_INDEX
	SKIN_SORT_LOWER,					// SKIN_LOWER_INDEX
	SKIN_SORT_FOOT,						// SKIN_FOOT_INDEX
	SKIN_SORT_DEFAULT,					// SKIN_HEAD_INDEX
	SKIN_SORT_UPPER,					// SKIN_FASHION_UPPER_BODY_INDEX
	SKIN_SORT_WRIST,					// SKIN_FASHION_WRIST_INDEX
	SKIN_SORT_LOWER,					// SKIN_FASHION_LOWER_INDEX
	SKIN_SORT_FOOT,						// SKIN_FASHION_FOOT_INDEX
};

static char strWeaponActName[512];		// Weapon action name 

static CRITICAL_SECTION l_csLoadPlayerSkin;

static CECPlayer::PLAYER_ACTION*	_default_actions = NULL;	//	Static Action data table
static CECPlayer::PLAYER_ACTION*	_turning_actions = NULL;	//	Static Action data table

static abase::hash_map<unsigned int, const PLAYER_ACTION_INFO_CONFIG *> _default_skill_actions; // skill actions
static PLAYER_LEVELEXP_CONFIG	_player_levelup_exp;	// Level up exp needed
static const int	GAP_BETWEEN_NAME_TITLE = 7;
static const A3DVECTOR3 aExts[NUM_PROFESSION*NUM_GENDER] =
{
	A3DVECTOR3(0.4f, 0.9f, 0.4f),	//	武侠
	A3DVECTOR3(0.3f, 0.85f, 0.3f),
	A3DVECTOR3(0.3f, 0.9f, 0.3f),	//	法师
	A3DVECTOR3(0.3f, 0.85f, 0.3f),
	A3DVECTOR3(0.3f, 0.9f, 0.3f),	//	巫师
	A3DVECTOR3(0.3f, 0.85f, 0.3f),	
	A3DVECTOR3(0.3f, 0.9f, 0.3f),	//	妖精
	A3DVECTOR3(0.3f, 0.85f, 0.3f),	
	A3DVECTOR3(0.5f, 1.05f, 0.5f), 	//	妖兽
	A3DVECTOR3(0.3f, 0.9f, 0.3f),
	A3DVECTOR3(0.3f, 0.9f, 0.3f), 	//	刺客
	A3DVECTOR3(0.3f, 0.85f, 0.3f),
	A3DVECTOR3(0.3f, 0.9f, 0.3f), 	//	羽芒
	A3DVECTOR3(0.3f, 0.85f, 0.3f),
	A3DVECTOR3(0.3f, 0.9f, 0.3f), 	//	羽灵
	A3DVECTOR3(0.3f, 0.85f, 0.3f),
	A3DVECTOR3(0.3f, 0.9f, 0.3f),	//	剑灵
	A3DVECTOR3(0.3f, 0.85f, 0.3f),	
	A3DVECTOR3(0.3f, 0.9f, 0.3f), 	//	魅灵
	A3DVECTOR3(0.3f, 0.85f, 0.3f),
	A3DVECTOR3(0.3f, 0.9f, 0.3f),	//	夜影
	A3DVECTOR3(0.3f, 0.85f, 0.3f),	
	A3DVECTOR3(0.3f, 0.9f, 0.3f), 	//	月仙
	A3DVECTOR3(0.3f, 0.85f, 0.3f),
};

const int CECPlayer::m_sciStateIDForStateAction[1] = {117};
///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////

static void _GenSkinPath(char* szPath, int nProfession, int nGender, const char* szSkinName)
{
	sprintf(szPath, _equipment_skin[nProfession * NUM_GENDER + nGender], szSkinName, szSkinName);
}

static void _GenDefaultSkinPath(char* szPath, int nProfession, int nGender, const char* szSkinName)
{
	sprintf(szPath, _equipment_default_skin[nProfession * NUM_GENDER + nGender], szSkinName, szSkinName);
}

static const char* _GenWeaponActionName(char* szAct, int nGender)
{
	sprintf(strWeaponActName, "%s_%s", szAct, (nGender==GENDER_MALE) ? "男" : "女");
	return strWeaponActName;
}

static inline int _GetProfessionTransformModelID(int nChar, int nGender, int nModelID)
{
	int result(0);
	switch (nChar){
	case PROF_HAG:		//	妖精
		result = (2 == nModelID) ? RES_MOD_ORC_FOX2 : RES_MOD_ORC_FOX;
		break;
	case PROF_ORC:		//	妖兽
		result = (2 == nModelID) ? RES_MOD_ORC_PANDER : RES_MOD_ORC_TIGER;
		break;
	case PROF_MONK:		//	巫师
	case PROF_GHOST:	//	刺客
		result = (GENDER_MALE == nGender) ? RES_MOD_SHADOW_FISH_M : RES_MOD_SHADOW_FISH_F;
		break;
	case PROF_YEYING:	//	夜影
		result = (GENDER_MALE == nGender) ? RES_MOD_YEYING_RESHAPE_M : RES_MOD_YEYING_RESHAPE_F;
		break;
	case PROF_YUEXIAN:	//	月仙
		result = (GENDER_MALE == nGender) ? RES_MOD_YUEXIAN_RESHAPE_M : RES_MOD_YUEXIAN_RESHAPE_F;
		break;
	}
	return result;
}

static bool IsTransofrmModelLikeHuman(int nChar, int nGender, int nModelID){
	bool result(false);
	const int *likeHumanModelArray = NULL;
	int likeHumanModelCount = 0;
	switch (nChar)
	{
	case PROF_YEYING:
		if (GENDER_MALE == nGender){
			const int LIKE_HUMAN_MODEL_COUNT = 1;
			static int s_LikeHumanModel[LIKE_HUMAN_MODEL_COUNT] = {RES_MOD_YEYING_RESHAPE_M};
			likeHumanModelArray = s_LikeHumanModel;
			likeHumanModelCount = LIKE_HUMAN_MODEL_COUNT;
		}else{
			const int LIKE_HUMAN_MODEL_COUNT = 1;
			static int s_LikeHumanModel[LIKE_HUMAN_MODEL_COUNT] = {RES_MOD_YEYING_RESHAPE_F};
			likeHumanModelArray = s_LikeHumanModel;
			likeHumanModelCount = LIKE_HUMAN_MODEL_COUNT;
		}
		break;
	case PROF_YUEXIAN:
		if (GENDER_MALE == nGender){
			const int LIKE_HUMAN_MODEL_COUNT = 1;
			static int s_LikeHumanModel[LIKE_HUMAN_MODEL_COUNT] = {RES_MOD_YUEXIAN_RESHAPE_M};
			likeHumanModelArray = s_LikeHumanModel;
			likeHumanModelCount = LIKE_HUMAN_MODEL_COUNT;
		}else{
			const int LIKE_HUMAN_MODEL_COUNT = 1;
			static int s_LikeHumanModel[LIKE_HUMAN_MODEL_COUNT] = {RES_MOD_YUEXIAN_RESHAPE_F};
			likeHumanModelArray = s_LikeHumanModel;
			likeHumanModelCount = LIKE_HUMAN_MODEL_COUNT;
		}
		break;
	}
	if (likeHumanModelCount > 0){
		const int *begin = likeHumanModelArray;
		const int *end = likeHumanModelArray + likeHumanModelCount;
		if (std::find(begin, end, _GetProfessionTransformModelID(nChar, nGender, nModelID)) != end){
			result = true;
		}
	}
	return result;
}

inline void _ReleaseLoadModel(EC_PLAYERLOADRESULT& Ret)
{
	if (Ret.pPlayerModel)
	{
		Ret.pPlayerModel->Release();
		delete Ret.pPlayerModel;
	}

	if (Ret.pDummyModel)
	{
		Ret.pDummyModel->Release();
		delete Ret.pDummyModel;
	}

	if (Ret.pFaceModel)
	{
		Ret.pFaceModel->Release();
		delete Ret.pFaceModel;
	}

	if (Ret.pPetModel)
	{
		Ret.pPetModel->Release();
		delete Ret.pPetModel;
	}

	for (int i = 0; i < NUM_SKIN_INDEX; i++)
	{
		g_pGame->ReleaseA3DSkin(Ret.EquipResult.aSkins[i][0]);
		g_pGame->ReleaseA3DSkin(Ret.EquipResult.aSkins[i][1]);
		g_pGame->ReleaseA3DSkin(Ret.EquipResult.aSkins[i][2]);
	}

	if (Ret.EquipResult.pLeftHandWeapon)
	{
		Ret.EquipResult.pLeftHandWeapon->Release();
		delete Ret.EquipResult.pLeftHandWeapon;
	}
	if (Ret.EquipResult.pRightHandWeapon)
	{
		Ret.EquipResult.pRightHandWeapon->Release();
		delete Ret.EquipResult.pRightHandWeapon;
	}

	if (Ret.EquipResult.pWing)
	{
		Ret.EquipResult.pWing->Release();
		delete Ret.EquipResult.pWing;
	}
}

//	Build pate faction text
static void _BuildPateFactionText(int idFaction, int iRank, ACString& strText)
{
}
void GetFashionActionNameByID(int idEquipment, AString& strAction)
{
}
bool CECPlayer::LoadPlayerEquips(
	int iProf,
	int iGender,
	__int64 Mask,
	const int* pEquips,
	EquipsLoadResult& Ret,
	bool bSimpleModel)
{
	memset(&Ret, 0, sizeof(Ret));

	ShowEquipments(
		iProf,
		iGender,
		pEquips,
		Mask,
		&Ret,
		bSimpleModel
	);

	return true;
}

//	Load player model
bool CECPlayer::LoadPlayerModel(
	int iProfession,
	int iGender,
	int iCustom,
	const int* pEquips,
	const char* szPetPath,
	EC_PLAYERLOADRESULT& Ret,
	bool bSimpleFace, bool bSimpleModel)
{
	static const char* aModelFiles[NUM_PROFESSION*NUM_GENDER] =
	{
		res_ModelFile(RES_MOD_WARRIOR_M),	//	武侠
		res_ModelFile(RES_MOD_WARRIOR_F),
		res_ModelFile(RES_MOD_MAGE_M),		//	法师
		res_ModelFile(RES_MOD_MAGE_F),		
		res_ModelFile(RES_MOD_MONK_M),		//	巫师
		res_ModelFile(RES_MOD_MONK_F),		
		res_ModelFile(RES_MOD_HAG_M),		//	妖精
		res_ModelFile(RES_MOD_HAG_F),		
		res_ModelFile(RES_MOD_ORC_M), 		//	妖兽
		res_ModelFile(RES_MOD_ORC_F),
		res_ModelFile(RES_MOD_GHOST_M), 	//	刺客
		res_ModelFile(RES_MOD_GHOST_F),
		res_ModelFile(RES_MOD_ARCHOR_M), 	//	羽芒
		res_ModelFile(RES_MOD_ARCHOR_F),
		res_ModelFile(RES_MOD_ANGEL_M), 	//	羽灵
		res_ModelFile(RES_MOD_ANGEL_F),
		res_ModelFile(RES_MOD_JIANLING_M), 	//	剑灵
		res_ModelFile(RES_MOD_JIANLING_F),
		res_ModelFile(RES_MOD_MEILING_M), 	//	魅灵
		res_ModelFile(RES_MOD_MEILING_F),
		res_ModelFile(RES_MOD_YEYING_M), 	//	夜影
		res_ModelFile(RES_MOD_YEYING_F),
		res_ModelFile(RES_MOD_YUEXIAN_M), 	//	月仙
		res_ModelFile(RES_MOD_YUEXIAN_F),
	};

	int iChar = iProfession * NUM_GENDER + iGender;
	memset(&Ret, 0, sizeof(Ret));

	Ret.dwValidMask |= PLAYERLOADRESULT_PLAYERMODEL | PLAYERLOADRESULT_EQUIPMODEL | PLAYERLOADRESULT_PETMODEL;

	//	Load player model ----------------------------
	if (!(Ret.pPlayerModel = new CECModel))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECPlayer::LoadPlayerModel", __LINE__);
		_ReleaseLoadModel(Ret);
		return false;
	}

	//	Load skeleton without skins
	const char* szFile = aModelFiles[iChar];
	if (!Ret.pPlayerModel->Load(szFile, true, A3DSkinModel::LSF_NOSKIN, /*true*/false))
	{
		a_LogOutput(1, "CECPlayer::LoadPlayerModel, Failed to load player model %s", szFile);
		_ReleaseLoadModel(Ret);
		return false;
	}

	A3DSkinModel* pA3DModel = Ret.pPlayerModel->GetA3DSkinModel();

	if (!pA3DModel)
	{
		_ReleaseLoadModel(Ret);
		return false;
	}

	pA3DModel->AddSkin(NULL, false);	// body skin
	pA3DModel->AddSkin(NULL, false);	// upper body skin
	pA3DModel->AddSkin(NULL, false);	// wrist skin
	pA3DModel->AddSkin(NULL, false);	// lower body skin
	pA3DModel->AddSkin(NULL, false);	// foot skin
	pA3DModel->AddSkin(NULL, false);	// head skin
	pA3DModel->AddSkin(NULL, false);	// fashion upper body skin
	pA3DModel->AddSkin(NULL, false);	// fashion wrist skin
	pA3DModel->AddSkin(NULL, false);	// fashion lower body skin
	pA3DModel->AddSkin(NULL, false);	// fashion foot skin

	//	load equips
	__int64 EquipMask = 0;
	if (pEquips)
	{
		for (__int64 i = 0; i < SIZE_ALL_EQUIPIVTR; i++)
			EquipMask |= (pEquips[i] < 0) ? 0 : (1 << i);
	}

	if(EquipMask)
	{
		ShowEquipments(
			iProfession,
			iGender,
			pEquips,
			EquipMask,
			&Ret.EquipResult,
			bSimpleModel
		);
	}

	if (!LoadBodySkin(iCustom, iProfession, iGender, Ret.EquipResult.aSkins, Ret.pBodyShaders, bSimpleModel))
	{
		_ReleaseLoadModel(Ret);
		return false;
	}

	if( bSimpleFace )
	{
		Ret.pFaceModel = NULL;
	}
	else
	{
		Ret.pFaceModel = ThreadLoadFaceModel(iProfession, iGender, iCustom);
		if (!Ret.pFaceModel)
		{
			_ReleaseLoadModel(Ret);
			return false;
		}
	}

	if (szPetPath && szPetPath[0] && !LoadPetModel(szPetPath, &Ret.pPetModel))
	{
		_ReleaseLoadModel(Ret);
		return false;
	}

	return true;
}

bool CECPlayer::LoadPetModel(const char* szPetPath, CECModel** ppPetModel)
{
	CECModel*& pPetModel = *ppPetModel;
	pPetModel = new CECModel();

//	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("armor.sdr", A3DSkinMan::SHADERREPLACE_REFLECTPREFIX);
	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile((const char*)glb_ArmorReplaceShader_ReflectPrefix, A3DSkinMan::SHADERREPLACE_USERDEFINE);
	if (!pPetModel->Load(szPetPath, true, A3DSkinModel::LSF_NOSKIN, true))
	{
		delete pPetModel;
		pPetModel = NULL;
		g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
		return false;
	}

	pPetModel->BuildWoundActionChannel();

	char szSkin[MAX_PATH];
	strncpy(szSkin, szPetPath, MAX_PATH);
	glb_ChangeExtension(szSkin, "ski");
	A3DSkin * pPetSkin = g_pGame->LoadA3DSkin(szSkin, false);
	if( !pPetSkin )
	{
		pPetModel->Release();
		delete pPetModel;
		pPetModel = NULL;
		g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
		return false;
	}
	pPetModel->GetA3DSkinModel()->AddSkin(pPetSkin, true);
	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
	return true;
}

bool CECPlayer::LoadDummyModel(int iShape, CECModel** ppDummyModel)
{
	CECModel* pDummyModel = NULL;
	
	int resID = PLAYERMODEL_GETID(iShape);
	if(resID > 0) // == 0 means no model changed but logic transformed
	{
		pDummyModel = new CECModel;
		if (!pDummyModel)
		{
			glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECPlayer::LoadDummyModel", __LINE__);
			return false;
		}

		const char* szFile = NULL;
		if (PLAYERMODEL_GETTYPE(iShape) == PLAYERMODEL_DUMMYTYPE2) // 小动物变身
			szFile = res_ModelFileForChangeShape2(resID);
		else
			szFile = res_ModelFile(resID);

		if (!pDummyModel->Load(szFile, true, 0, false))
		{
			delete pDummyModel;
			a_LogOutput(1, "CECPlayer::LoadDummyModel, Failed to load dummy model %s", szFile);
			return false;
		}
	}
	
	*ppDummyModel = pDummyModel;
	return true;
}

//	Release player model
void CECPlayer::ReleasePlayerModel(EC_PLAYERLOADRESULT& Ret)
{
	if (Ret.pPlayerModel)
	{
		QueueECModelForRelease(Ret.pPlayerModel);
		Ret.pPlayerModel = NULL;
	}

	if (Ret.pDummyModel)
	{
		QueueECModelForRelease(Ret.pDummyModel);
		Ret.pDummyModel = NULL;
	}

	A3DRELEASE(Ret.pFaceModel);
	A3DRELEASE(Ret.pPetModel);
	A3DRELEASE(Ret.EquipResult.pLeftHandWeapon);
	A3DRELEASE(Ret.EquipResult.pRightHandWeapon);
	A3DRELEASE(Ret.EquipResult.pWing);
	A3DRELEASE(Ret.EquipResult.pWing2);

	for (int i = 0; i < 3; i++)
	{
		g_pGame->ReleaseA3DSkin(Ret.EquipResult.aSkins[SKIN_BODY_INDEX][i]);
		g_pGame->ReleaseA3DSkin(Ret.EquipResult.aSkins[SKIN_HEAD_INDEX][i]);
		g_pGame->ReleaseA3DSkin(Ret.EquipResult.aSkins[SKIN_UPPER_BODY_INDEX][i]);
		g_pGame->ReleaseA3DSkin(Ret.EquipResult.aSkins[SKIN_WRIST_INDEX][i]);
		g_pGame->ReleaseA3DSkin(Ret.EquipResult.aSkins[SKIN_LOWER_INDEX][i]);
		g_pGame->ReleaseA3DSkin(Ret.EquipResult.aSkins[SKIN_FOOT_INDEX][i]);
		g_pGame->ReleaseA3DSkin(Ret.EquipResult.aSkins[SKIN_FASHION_UPPER_BODY_INDEX][i]);
		g_pGame->ReleaseA3DSkin(Ret.EquipResult.aSkins[SKIN_FASHION_WRIST_INDEX][i]);
		g_pGame->ReleaseA3DSkin(Ret.EquipResult.aSkins[SKIN_FASHION_LOWER_INDEX][i]);
		g_pGame->ReleaseA3DSkin(Ret.EquipResult.aSkins[SKIN_FASHION_FOOT_INDEX][i]);
	}
}

//	Get exp of specified level
int CECPlayer::GetLevelUpExp(int iLevel)
{
	return _player_levelup_exp.exp[iLevel - 1];
	//return iLevel * iLevel * 500;
}

//	Build riding pet file name
const char* CECPlayer::GetRidingPetFileName(int idPet)
{
	static const char* szDef = "Models\\NPCs\\宠物\\骑宠\\骑宠马\\骑宠马白.ecm";

	DATA_TYPE DataType;
	const void* pDataPtr = g_pGame->GetElementDataMan()->get_data_ptr(idPet, ID_SPACE_ESSENCE, DataType);
	if (DataType != DT_PET_ESSENCE)
		return szDef;

	const PET_ESSENCE* pData = (const PET_ESSENCE*)pDataPtr;
	if (!pData->file_model[0])
		return szDef;

	return pData->file_model;
}

//	Initialize static resources
bool CECPlayer::InitStaticRes()
{
	//	Element data man must has been initialized
	elementdataman* pdb = g_pGame->GetElementDataMan();
	if (!pdb)
	{
		ASSERT(pdb);
		return false;
	}

	BuildActionList();

	//	Initialize level up exp table
	DATA_TYPE dt;
	_player_levelup_exp = *(PLAYER_LEVELEXP_CONFIG*)g_pGame->GetElementDataMan()->get_data_ptr(202, ID_SPACE_CONFIG, dt);

	//  Initialize a CRITICAL_SECTION for loading player skin
	InitializeCriticalSection(&l_csLoadPlayerSkin);

	return true;
}

//	Release static resources
void CECPlayer::ReleaseStaticRes()
{
	delete[] _default_actions;
	_default_actions = NULL;

	delete[] _turning_actions;
	_turning_actions = NULL;

	_default_skill_actions.clear();

	DeleteCriticalSection(&l_csLoadPlayerSkin);
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECPlayer
//	
///////////////////////////////////////////////////////////////////////////
CECPlayer::CECPlayer(CECPlayerMan* pPlayerMan)
{
	m_iCID				= OCID_PLAYER;
	m_pPlayerMan		= pPlayerMan;
	m_pPlayerModel		= NULL;
	m_pFaceModel		= NULL;
	memset(m_pModels, 0, sizeof(m_pModels));
	memset(m_aShapeID, 0, sizeof(m_aShapeID));
	m_pLeftHandWeapon	= NULL;
	m_pRightHandWeapon	= NULL;
	m_bWeaponAttached	= false;
	m_pPetModel			= NULL;
	m_pActionController	= NULL;
	m_pBodyController	= NULL;
	m_pSprite			= NULL;
	m_pGoblin			= NULL;
	memset(m_pBodyShader, 0, sizeof(A3DShader *) * 3);
	m_iMoveMode			= MOVE_STAND;
	m_iMoveEnv			= MOVEENV_GROUND;
	m_bCastShadow		= false;
	m_pLevelUpGFX		= NULL;
	m_pWaterWaveStill	= NULL;
	m_pWaterWaveMove	= NULL;
	m_pAirBubble		= NULL;
	m_pSwimBubble		= NULL;
	m_pTransformGfx		= NULL;
	m_pDuelStateGFX		= NULL;
	m_pPetCureGFX		= NULL;
	m_pPetCureGFXtate	= -1;
	memset(m_pMonsterSpiritGFX, 0, sizeof(m_pMonsterSpiritGFX));
	m_iMonsterSpiritMineID = 0;
	m_stateMonsterSpirit = BALL_STATE_NONE;
	m_fTouchRad			= 0.3f;
	m_iMoneyCnt			= 0;
	m_iMaxMoney			= 1000;
	m_bWalkRun			= 1;
	m_pTeam				= NULL;
	m_iProfession		= PROF_WARRIOR;
	m_iGender			= GENDER_MALE;
	m_bRenderName		= false;
	m_bRenderBar		= true;
	m_dwStates			= 0;
	m_dwStates2			= 0;
	m_dwResFlags		= 0;
	m_uAttackType		= DEFAULT_ACTION_TYPE;
	m_iFashionWeaponType= DEFAULT_ACTION_TYPE;
	m_bAboutToDie		= false;
	memset(m_aExtStates, 0, sizeof(m_aExtStates));
	memset(m_aExtStatesShown, 0, sizeof(m_aExtStatesShown));
	m_pCurSkill			= NULL;
	m_idCurSkillTarget	= 0;
	m_bFight			= false;
	m_iReputation		= 0;
	m_iShape			= 0;
	m_bInSanctuary		= false;
	m_bPetInSanctuary	= false;
	m_iBoothState		= 0;
	m_crcBooth			= 0;
	m_bFashionMode		= false;
	m_factionPVPMask	= 0;
	m_idFaction			= 0;
	m_idFRole			= GNET::_R_UNMEMBER;
	m_idSpouse			= 0;	
	m_byPariahLvl		= 0;
	m_bRushFly			= false;
	m_AttachMode		= enumAttachNone;
	m_bHangerOn			= false;
	m_iBuddyId			= 0;
	m_idCandBuddy		= 0;
	m_bCandHangerOn		= false;
	m_pFactionDecal		= NULL;
	m_iBattleCamp		= GP_BATTLE_CAMP_NONE;
	m_dwGMFlags			= 0;
	m_idCurPet			= 0;
	m_pBoothModel		= NULL;
	m_iBoothModelCertificateID = -1;	//	默认值为-1，以处理默认摆摊模型为0时、也需要加载默认摆摊模型的情况
	m_bBoothModelLoaded	= false;
	m_iBoothBarCertificateID = -1;		//	默认值为-1，以处理默认摆摊模型为0时、也需要加载默认摆摊模型的情况
	m_weaponHangerPos	= WEAPON_HANGER_HAND;
	m_vNamePos			= A3DVECTOR3(0.0f);
	m_i64EquipDisabled	= 0;
	m_idForce			= 0;
	m_idCountry			= 0;
	m_pCountryDecal = NULL;
	m_PlayerActions		= _default_actions;	// bind to default actions

	m_GoblinRenderCnt.SetPeriod(30000);
	m_GoblinRenderCnt.Reset(true);

	m_bRenderGoblin = true;

	m_TransCnt.SetPeriod(500);
	m_fDstTrans = 0.f;
	m_fCurTrans = 0.f;
	m_fTransDelta = 0.f;

	m_fDistToCamera		= 0.0f;
	
	m_pBubbleTexts = new CECBubbleDecalList;

	m_nLowerEquipMethod			= enumSkinShowNone;
	m_nLowerFashionEquipMethod	= enumSkinShowNone;
	m_wingType					= WINGTYPE_FLYSWORD;

	m_aabb.Center	= g_vOrigin;
	m_aabb.Extents.Set(0.3f, 0.9f, 0.3f);
	m_aabbServer = m_aabb;
	m_MoveConst.fStepHei		= 0.8f;
	m_MoveConst.fMinAirHei		= 1.6f;	
	m_MoveConst.fMinWaterHei	= 0.3f;
	m_MoveConst.fShoreDepth		= 1.6f;
	m_MoveConst.fWaterSurf		= 0.6f;	

	memset(&m_CustomizeData, 0, sizeof(m_CustomizeData));
	m_CustomizeData.bodyID = 0;
	m_CustomizeData.dwVersion = CUSTOMIZE_DATA_VERSION;
	m_CustomizeData.colorBody = 0xffffffff;
	m_CustomizeData.headScale = 128;
	m_CustomizeData.upScale = 128;
	m_CustomizeData.waistScale = 128;
	m_CustomizeData.armWidth = 128;
	m_CustomizeData.legWidth = 128;
	m_CustomizeData.breastScale = 128;

	m_OldCustomizeData = m_CustomizeData;
	m_ChgPllCustomizeData = m_CustomizeData;
	m_vPortraitCamScale	= 1.0f;
	m_bIsChangingFace = false;

	memset(&m_PlayerInfo, 0, sizeof (m_PlayerInfo));
	memset(&m_BasicProps, 0, sizeof (m_BasicProps));
	memset(&m_ExtProps, 0, sizeof (m_ExtProps));
	memset(m_aEquips, 0xff, sizeof (m_aEquips));
	memset(&m_TeamReq, 0, sizeof (m_TeamReq));
	memset(m_aSkins, 0, sizeof (m_aSkins));
	memset(m_aCurSkins, 0, sizeof (m_aCurSkins));
	memset(&m_pvp, 0, sizeof (m_pvp));
	memset(&m_meridiansProp, 0, sizeof (m_meridiansProp));

	m_PlayerInfo.crc_c = -1;
	m_PateContent.iVisible = 0;

	m_bShowWeapon = true;

	m_stoneUpperBody	= 0;
	m_stoneWrist		= 0;
	m_stoneLowerBody	= 0;
	m_stoneFoot			= 0;
	m_stoneWeapon		= 0;
	m_idFullSuite		= 0;

	m_stoneUpperBodyShown	= 0;
	m_stoneWristShown		= 0;
	m_stoneLowerBodyShown	= 0;
	m_stoneFootShown		= 0;
	m_stoneWeaponShown		= 0;
	m_idFullSuiteShown	= 0;
	m_TitleID = 0;
	m_ReincarnationCount = 0;
	m_RealmLevel = 0;

	m_strLastSayCnt.SetPeriod(20000);
	m_fScaleBySkill = 1.f;

	m_SkillIDForStateAction = 0;

	//	Initialize Customize Factor
	InitCustomizeFactor();

	m_idSelTarget = 0;

	//载入个性化限制
	//m_pCustomizeBound = g_CustomizeBoundMgr.GetCustomizeBound("Configs\\CustomizeBound.ini");
	//assert(m_pCustomizeBound);
}

CECPlayer::~CECPlayer()
{
}

//#define _DEBUG_OUTPUT_ACTIONS

void CECPlayer::BuildActionList()
{
#ifdef _DEBUG_OUTPUT_ACTIONS
	FILE * fpActionList = fopen("actions.txt", "wt");
#endif

	//	Load action names from file
	if (!_default_actions)
	{
		typedef abase::hashtab<PLAYER_ACTION_INFO_CONFIG *, AString, abase::_hash_function>	PlayerActionMap;
		typedef abase::hashtab<PLAYER_ACTION_INFO_CONFIG *, AWString, abase::_hash_function>	PlayerSkillActionMap;

		int i;
		elementdataman * dataman = g_pGame->GetElementDataMan();
		PLAYER_ACTION_INFO_CONFIG * data = NULL;
		
		//	1. 建立动作名称到 PLAYER_ACTION_INFO_CONFIG 数据的映射，供后续快速查询
		PlayerActionMap	actionMap(100);
		PlayerSkillActionMap	skillActionMap(100);
		{
			int count = dataman->get_data_num(ID_SPACE_CONFIG);

			DATA_TYPE dt;
			int id(0);

			for (i = 0; i < count; ++ i)
			{
				id = dataman->get_data_id(ID_SPACE_CONFIG, i, dt);
				if (dt != DT_PLAYER_ACTION_INFO_CONFIG)
					continue;

				data = (PLAYER_ACTION_INFO_CONFIG *) dataman->get_data_ptr(id, ID_SPACE_CONFIG, dt);

				//	建立通用动作名的映射表条目
				if (data->action_name[0] && data->action_name[0] != '0')
				{
					if (!actionMap.put(data->action_name, data))
					{
#ifdef _DEBUG
						AString strTemp;
						strTemp.Format("通用动作名重复: action_name = %s, 原id = %d, 新id = %d\n", data->action_name, (*(actionMap.find(data->action_name).value()))->id, data->id);
						::OutputDebugStringA(strTemp);
#endif
					}
				}

				//	建立技能动作名的映射表条目
				if (data->name[0] && data->name[0] != '0')
				{
					if (!skillActionMap.put(data->name, data))
					{
#ifdef _DEBUG
						ACString strTemp;
						strTemp.Format(_AL("技能动作名重复: name = %s, 原id = %d, 新id = %d\n"), data->name, (*(skillActionMap.find(data->name).value()))->id, data->id);
						::OutputDebugString(strTemp);
#endif
					}
				}

#ifdef _DEBUG
				if (!(data->action_name[0] && data->action_name[0] != '0') &&
					!(data->name[0] && data->name[0] != '0'))
				{
					AString strTemp;
					strTemp.Format("动作配置表错误，名称为空: id = %d\n", data->id);
					::OutputDebugStringA(strTemp);
				}
#endif
			}
		}

		//	2.处理通用动作
		CECStringTab actionNames;
		actionNames.Init("Configs\\actions_player.txt", false);

		_default_actions = new PLAYER_ACTION[ACT_MAX];
		memset(_default_actions, 0, sizeof(PLAYER_ACTION) * ACT_MAX);

		for(i=0; i<ACT_MAX; i++)
		{
			_default_actions[i].type = (PLAYER_ACTION_TYPE) i;
			
			const char *szName = actionNames.GetANSIString(i);
			if (szName && szName[0])
			{
				PlayerActionMap::iterator it = actionMap.find(szName);
				if (it != actionMap.end())
				{
					_default_actions[i].data = data = *it.value();
					
#ifdef _DEBUG_OUTPUT_ACTIONS
					if( i >= ACT_ATTACK_1 && i <= ACT_ATTACK_4 )
					{
						for(int n=0; n<NUM_WEAPON_TYPE; n++)
						{
							fprintf(fpActionList, "%s_%s起\n", data->action_prefix, data->action_weapon_suffix[n].suffix);
							fprintf(fpActionList, "%s_%s落\n", data->action_prefix, data->action_weapon_suffix[n].suffix);
						}
					}
					else
					{
						for(int n=0; n<NUM_WEAPON_TYPE; n++)
						{
							fprintf(fpActionList, "%s_%s\n", data->action_prefix, data->action_weapon_suffix[n].suffix);
						}
					}
#endif
					continue;
				}
			}

			// not found
			// a_LogOutput(1, "CECPlayer::CECPlayer(), Failed to find action [%s]'s data", actionNames.GetANSIString(i));
		}

		// 建立特殊动作映射
		if(_turning_actions) delete[] _turning_actions;
		_turning_actions = new PLAYER_ACTION[ACT_MAX];
		PlayerActionMap::iterator turning = actionMap.find("自身旋转"); // 查找特殊的动作
		for(i=0; i<ACT_MAX; i++)
		{
			if( (i < ACT_GROUNDDIE ||	// 陆地死亡
				 i > ACT_REVIVE) &&		// 复活
				turning != actionMap.end() )
			{
				// 用旋转动作替换通用动作
				_turning_actions[i].type = (PLAYER_ACTION_TYPE) i;
				_turning_actions[i].data = *turning.value();
			}
			else
			{
				// 使用通用动作
				_turning_actions[i] = _default_actions[i];	
			}
		}

		
	}

#ifdef _DEBUG_OUTPUT_ACTIONS
	fclose(fpActionList);
#endif
}

//	Release object
void CECPlayer::Release()
{
	DetachBuddy();

	//	Clear extend states before model is released
	ClearShowExtendStates();
	::memset(m_aExtStates, 0, sizeof(m_aExtStates));
	m_aIconStates.clear();

// 	if (m_pPateName)
// 	{
// 		delete m_pPateName;
// 		m_pPateName = NULL;
// 	}
// 
// 	if (m_pPateMarry)
// 	{
// 		delete m_pPateMarry;
// 		m_pPateMarry = NULL;
// 	}
// 
// 	if (m_pPateForce)
// 	{
// 		delete m_pPateForce;
// 		m_pPateForce = NULL;
// 	}
// 
// 	if (m_pPateTitle)
// 	{
// 		delete m_pPateTitle;
// 		m_pPateTitle = NULL;
// 	}
// 
// 	if (m_pPateLastWords1)
// 	{
// 		delete m_pPateLastWords1;
// 		m_pPateLastWords1 = NULL;
// 	}
// 
// 	if (m_pPateLastWords2)
// 	{
// 		delete m_pPateLastWords2;
// 		m_pPateLastWords2 = NULL;
// 	}
// 
// 	if (m_pPateTeamReq)
// 	{
// 		delete m_pPateTeamReq;
// 		m_pPateTeamReq = NULL;
// 	}
// 
// 	if (m_pPateBooth)
// 	{
// 		delete m_pPateBooth;
// 		m_pPateBooth = NULL;
// 	}
// 
// 	if (m_pPateFaction)
// 	{
// 		delete m_pPateFaction;
// 		m_pPateFaction = NULL;
// 	}

	if (m_pFactionDecal)
	{
		delete m_pFactionDecal;
		m_pFactionDecal = NULL;
	}

// 	if (m_pPateCountry)
// 	{
// 		delete m_pPateCountry;
// 		m_pPateCountry = NULL;
// 	}
	if (m_pCountryDecal)
	{
		delete m_pCountryDecal;
		m_pCountryDecal = NULL;
	}

	if (m_pBubbleTexts)
	{
		delete m_pBubbleTexts;
		m_pBubbleTexts = NULL;
	}

	if (m_pLevelUpGFX)
	{
		g_pGame->GetGFXCaster()->ReleaseGFXEx(m_pLevelUpGFX);
		m_pLevelUpGFX = NULL;
	}

	if (m_pPetCureGFX)
	{		
		A3DGFXExMan *pGFXExMan = g_pGame->GetA3DGFXExMan();
		if (pGFXExMan)
			pGFXExMan->CacheReleasedGfx(m_pPetCureGFX);
		m_pPetCureGFX = NULL;
		m_pPetCureGFXtate = -1;
	}
	for (int i = 0; i < sizeof(m_pMonsterSpiritGFX) / sizeof(m_pMonsterSpiritGFX[0]); ++i) {
		A3DGFXExMan *pGFXExMan = g_pGame->GetA3DGFXExMan();
		if (pGFXExMan)
			pGFXExMan->CacheReleasedGfx(m_pMonsterSpiritGFX[i]);
		m_pMonsterSpiritGFX[i] = NULL;
	}

	if (m_pWaterWaveStill)
	{
		g_pGame->GetGFXCaster()->ReleaseGFXEx(m_pWaterWaveStill);
		m_pWaterWaveStill = NULL;
	}
	
	if (m_pWaterWaveMove)
	{
		g_pGame->GetGFXCaster()->ReleaseGFXEx(m_pWaterWaveMove);
		m_pWaterWaveMove = NULL;
	}
	
	if (m_pAirBubble)
	{
		g_pGame->GetGFXCaster()->ReleaseGFXEx(m_pAirBubble);
		m_pAirBubble = NULL;
	}

	if (m_pSwimBubble)
	{
		g_pGame->GetGFXCaster()->ReleaseGFXEx(m_pSwimBubble);
		m_pSwimBubble = NULL;
	}

	if (m_pTransformGfx)
	{
		g_pGame->GetGFXCaster()->ReleaseGFXEx(m_pTransformGfx);
		m_pTransformGfx = NULL;
	}

	if (m_pDuelStateGFX)
	{
		g_pGame->GetGFXCaster()->ReleaseGFXEx(m_pDuelStateGFX);
		m_pDuelStateGFX = NULL;
	}

	if (m_pPetModel)
	{
		m_pPetModel->RemoveChildModel(_hanger_ride, false);
		A3DRELEASE(m_pPetModel);
	}

	ClearBoothModel();
	
	for (MOEffectMAP::iterator it = m_mapMOEffect.begin();it != m_mapMOEffect.end();++it)
	{
		A3DGFXExMan *pGFXExMan = g_pGame->GetA3DGFXExMan();
		pGFXExMan->CacheReleasedGfx(it->second);
	}

	m_mapMOEffect.clear();

	//	Release face model
	ReleaseFaceModel();

	//	Release player model
	ReleasePlayerModel();

	//	Clear resource ready flags
	SetResReadyFlag(RESFG_ALL, false);

	m_bAboutToDie = false;
}

bool CECPlayer::LoadBodySkin(
	int nBodyID,
	int iProfession,
	int iGender,
	A3DSkin* aSkins[NUM_SKIN_INDEX][3],
	A3DShader* pBodyShaders[3],
	bool bSimpleModel)
{
	//	body skin will be customized later, so they counld't be sharedly loaded by
	//	A3DSkinMan

	char szBodySkin[256];

	// ensure the gender is valid
	if(iGender != GENDER_MALE)
	{
		ASSERT(iGender == GENDER_FEMALE);
		iGender = GENDER_FEMALE;
	}

	const int nMaxBodyID = CECProfConfig::Instance().GetMaxBodyID(iProfession);
	if (nMaxBodyID > 0)
	{
		// use the max value instead of invalid value
		if(nBodyID < 0 || nBodyID > nMaxBodyID)
		{
#ifndef	_PROFILE_MEMORY
			ASSERT(false);
#endif
			nBodyID = nMaxBodyID;
		}

		if( !bSimpleModel )
			sprintf(szBodySkin, _body_skin[iProfession * NUM_GENDER + iGender], nBodyID + 1);
		else
			sprintf(szBodySkin, _simple_body_skin[iProfession * NUM_GENDER + iGender], nBodyID + 1);
	}
	else
	{
		if( !bSimpleModel )
			strncpy(szBodySkin, _body_skin[iProfession * NUM_GENDER + iGender], 256);
		else
			strncpy(szBodySkin, _simple_body_skin[iProfession * NUM_GENDER + iGender], 256);
	}

	if (!LoadPlayerSkin(aSkins[SKIN_BODY_INDEX], SKIN_BODY_INDEX, szBodySkin))
	{
		a_LogOutput(1, "CECPlayer::LoadBodySkin, Falied to replace body skin");
		return false;
	}

	int n;
	for(n=0; n<3; n++)
	{
		A3DSkin* pBodySkin = aSkins[SKIN_BODY_INDEX][n];
		int i;

		// adjust skin's material to make it a little speculable
		int nNumMaterial = pBodySkin->GetMaterialNum();
		for(i=0; i<nNumMaterial; i++)
		{
			A3DMATERIALPARAM param = pBodySkin->GetMaterial(i)->GetMaterialParam();
			param.Specular = 0xff3a3a3a;
			param.Power = 10.0f;
			pBodySkin->GetMaterial(i)->SetMaterialParam(param);
		}

		A3DSkinMesh* pMesh = NULL;
		for(i=0; i<pBodySkin->GetSkinMeshNum(); i++)
		{
			if( pBodySkin->GetSkinMesh(i)->GetTextureIndex() == 0 )
			{
				// found body mesh
				pMesh = pBodySkin->GetSkinMesh(i);
				break;
			}
		}
		if (!pMesh)
		{
			a_LogOutput(1, "CECPlayer::LoadBodySkin, Failed to set body shader");
			return false;
		}

		int iTexIndex = pMesh->GetTextureIndex();
		A3DTexture* pTex = pBodySkin->GetTexture(iTexIndex);
		char szTextureMap[MAX_PATH];
		strncpy(szTextureMap, pTex->GetMapFile(), MAX_PATH);

		// now load body skin shader
		pBodyShaders[n] = glb_LoadBodyShader(pBodySkin, szTextureMap);
		if (!pBodyShaders[n])
			a_LogOutput(1, "CECPlayer::LoadBodySkin, Failed to load body shader");

		if (!pBodySkin->ChangeSkinTexturePtr(iTexIndex, pBodyShaders[n]))
		{
			a_LogOutput(1, "CECPlayer::LoadBodySkin, Failed to change skin texture");
			return false;
		}
	}

	//	Record head skin file
	char szHeadFile[256];
	if( nMaxBodyID > 0 )
		sprintf(szHeadFile, "%s%d", _head_skin[iProfession * NUM_GENDER + iGender], nBodyID + 1);
	else
		strcpy(szHeadFile, _head_skin[iProfession * NUM_GENDER + iGender]);
	
	if (!LoadPlayerSkin(aSkins[SKIN_HEAD_INDEX], SKIN_HEAD_INDEX, szHeadFile))
	{
		a_LogOutput(1, "CECPlayer::LoadBodySkin, Falied to replace head skin");
		return false;
	}

	return true;
}

bool CECPlayer::LoadBodySkin(int nBodyID, bool bSimpleModel)
{
	A3DSkin* aSkins[NUM_SKIN_INDEX][3] = {0};
	A3DShader* pBodyShaders[3];

	if (!LoadBodySkin(nBodyID, m_iProfession, m_iGender, aSkins, pBodyShaders, bSimpleModel))
	{
		for (int i = 0; i < NUM_SKIN_INDEX; i++)
		{
			g_pGame->ReleaseA3DSkin(aSkins[i][0]);
			g_pGame->ReleaseA3DSkin(aSkins[i][1]);
			g_pGame->ReleaseA3DSkin(aSkins[i][2]);
		}
		return false;
	}

	// we must remove the skin from skin model before we can release it.
	ReplaceCurSkin(SKIN_BODY_INDEX, NULL);
	ReplaceCurSkin(SKIN_HEAD_INDEX, NULL);

	for (int i = 0; i < 3; i++)
	{
		g_pGame->ReleaseA3DSkin(m_aSkins[SKIN_BODY_INDEX][i]);
		g_pGame->ReleaseA3DSkin(m_aSkins[SKIN_HEAD_INDEX][i]);

		m_aSkins[SKIN_BODY_INDEX][i] = aSkins[SKIN_BODY_INDEX][i];
		m_aSkins[SKIN_HEAD_INDEX][i] = aSkins[SKIN_HEAD_INDEX][i];

		m_pBodyShader[i] = pBodyShaders[i];
	}

	if (GetMajorModel() && !bSimpleModel)
	{
		GetMajorModel()->ShowSkin(SKIN_HEAD_INDEX, false);
	}

	return true;
}

bool CECPlayer::LoadPlayerSkin(
	A3DSkin* aSkins[3],
	int iIndex,
	const char* szFile)
{
	ASSERT(iIndex >= 0 && iIndex < NUM_SKIN_INDEX);
	
// #ifdef _DEBUG
// 	static int _reentrant = 0;
// 	assert(_reentrant++ == 0);
// #endif

	ACSWrapper csa(&l_csLoadPlayerSkin);

	memset(aSkins, 0, sizeof(A3DSkin*) * 3);

	// now we make skinman replace skin's texture with a shader automatically
	if( iIndex >= SKIN_UPPER_BODY_INDEX && iIndex <= SKIN_FOOT_INDEX )
	{
	#ifdef SKIN_BUMP_ENABLE
		g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("armornormalmap.sdr", A3DSkinMan::SHADERREPLACE_NORMALMAPNEEDED);
	#else
	//	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("armor.sdr", 0);
		g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile((const char*)glb_ArmorReplaceShader, A3DSkinMan::SHADERREPLACE_USERDEFINE);
	#endif
	}
	else if( iIndex >= SKIN_FASHION_UPPER_BODY_INDEX && iIndex <= SKIN_FASHION_FOOT_INDEX )
	{
	//	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("fashion.sdr", 0);
		g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile((const char*)glb_FashionReplaceShader, A3DSkinMan::SHADERREPLACE_USERDEFINE);
	}

	if( szFile )
	{
		const char * suffix1[] = {"一级", "二级", "三级"};
		const char * suffix2[] = {"", "二级", "三级"};
		const char ** pSuffixes;

		if( iIndex == SKIN_BODY_INDEX || iIndex == SKIN_HEAD_INDEX )
			pSuffixes = suffix2;
		else
			pSuffixes = suffix1;

		char strSkinPath[MAX_PATH];
		for(int i=0; i<3; i++)
		{
			sprintf(strSkinPath, "%s%s.ski", szFile, pSuffixes[i]);

			A3DSkin * pSkin = g_pGame->LoadA3DSkin(strSkinPath, true);
			if (pSkin == NULL)
			{
				a_LogOutput(1, "CECPlayer::LoadPlayerSkin, Falied to load skin %s", strSkinPath);
				g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
// #ifdef _DEBUG
// 				assert(--_reentrant == 0);
// #endif
				return false;
			}

			// now set a specific material for armors
			if( iIndex >= SKIN_UPPER_BODY_INDEX && iIndex <= SKIN_FOOT_INDEX )
			{
				A3DCOLOR specular = ARMOR_SPECULAR;
				if( !g_pGame->GetA3DDevice()->TestPixelShaderVersion(1, 1) )
				{
					specular = 0xff606060;
				}
				
				int numMaterial = pSkin->GetMaterialNum();
				for(int j=0; j<numMaterial; j++)
				{
					A3DMATERIALPARAM param = pSkin->GetMaterial(j)->GetMaterialParam();
					param.Specular = specular;
					param.Power = ARMOR_POWER;
					pSkin->GetMaterial(j)->SetMaterialParam(param);
				}
			}
			else if( iIndex >= SKIN_FASHION_UPPER_BODY_INDEX && iIndex <= SKIN_FASHION_FOOT_INDEX )
			{
				A3DCOLOR specular = FASHION_SPECULAR;
				int numMaterial = pSkin->GetMaterialNum();
				for(int j=0; j<numMaterial; j++)
				{
					A3DMATERIALPARAM param = pSkin->GetMaterial(j)->GetMaterialParam();
					param.Specular = specular;
					param.Power = FASHION_POWER;
					pSkin->GetMaterial(j)->SetMaterialParam(param);
				}
			}

			aSkins[i] = pSkin;
		}
	}

	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);

// #ifdef _DEBUG
// 	assert(--_reentrant == 0);
// #endif

	return true;
}

//	Release player model
void CECPlayer::ReleasePlayerModel()
{
	if( m_pSprite )
	{
		m_pSprite->Release();
		delete m_pSprite;
		m_pSprite = NULL;
	}

	//	Release player skins
	int i(0);
	for (i=0; i < NUM_SKIN_INDEX; i++)
	{
		g_pGame->ReleaseA3DSkin(m_aSkins[i][0]);
		g_pGame->ReleaseA3DSkin(m_aSkins[i][1]);
		g_pGame->ReleaseA3DSkin(m_aSkins[i][2]);
		m_aSkins[i][0] = m_aSkins[i][1] = m_aSkins[i][2] = NULL;
		m_aCurSkins[i] = NULL;
	}

	memset(m_pBodyShader, 0, sizeof(m_pBodyShader));

	//memset(m_aEquips, 0xff, sizeof(m_aEquips));

	ReleaseWeapon();
	m_pPlayerModel = NULL;
	//memset(m_aShapeID, 0, sizeof(m_aShapeID));
	for(i = 0; i<PLAYERMODEL_MAX;i++)
	{
		if(!m_pModels[i]) continue;
		
		if (IsLoadThreadReady())
		{
			QueueECModelForRelease(m_pModels[i]);
		}
		else
		{
			m_pModels[i]->Release();
			delete m_pModels[i];
		}
	}
	memset(m_pModels, 0, sizeof(m_pModels));

	m_GfxRecords.clear();

	if (m_pFaceModel)
		m_pFaceModel->SetParent(NULL);

	SetResReadyFlag(RESFG_SKELETON | RESFG_SKIN, false);
	
	RecreateActionController();
	RecreateBodyController();
}

CECFace* CECPlayer::ThreadLoadFaceModel(int nCurCharacter, int nCurGender, int nFaceID)
{
	CECFace* pFaceModel;

	if (!(pFaceModel = new CECFace))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECPlayer::ThreadLoadFaceModel", __LINE__);
		return NULL;
	}

	// load different model's according to the character and gender
	bool bval;
	bval = pFaceModel->Init(nCurCharacter, nCurGender, nFaceID);
	if( !bval )
	{
		a_LogOutput(1, "CECPlayer::ThreadLoadFaceModel(), Failed to init face model.");
		delete pFaceModel;
		return NULL;
	}

	return pFaceModel;
}

//	Load player face model
bool CECPlayer::LoadFaceModel(int nCurCharacter, int nCurGender, int nFaceID)
{
	//	Release old face model
	ReleaseFaceModel();
	if (m_pFaceModel = ThreadLoadFaceModel(nCurCharacter, nCurGender, nFaceID)){
		AttachFaceModel();
	}
	return m_pFaceModel != NULL;
}

//	Release player face model
void CECPlayer::ReleaseFaceModel()
{
	if (m_pFaceModel)
	{
		if (GetMajorModel()){
			GetMajorModel()->RemoveComboModel(m_pFaceModel->GetECModel());
			GetMajorModel()->ShowSkin(SKIN_HEAD_INDEX, true);
		}

		m_pFaceModel->Release();
		delete m_pFaceModel;
		m_pFaceModel = NULL;
	}

//	SetResReadyFlag(RESFG_CUSTOM, false);
}

void CECPlayer::TransformShape(int iShape, bool bLoadAtOnce/* =false */)
{
	SetShape(iShape);	
	a_LogOutput(1, "CECPlayer::TransformShape(iShape=%d)(iShapeType=%d,iShapeID=%d)", iShape, PLAYERMODEL_GETTYPE(iShape), PLAYERMODEL_GETID(iShape));
	
	if (!GetMajorModel()) return;
	
	if (IsShapeChanged())
	{
		// change to a dummy model, may cause an asynchronous loading
		QueueLoadDummyModel(m_iShape, bLoadAtOnce);
	}
	else
	{
		// back to major model is a synchronous operation
		ApplyShapeModelChange(GetMajorModel());
	}
}

void CECPlayer::ApplyShapeModelChange(CECModel* pModel)
{
	// store and reset the attach state
	int iBuddyId = m_iBuddyId;
	bool bHangerOn = m_bHangerOn;
	if (m_AttachMode != enumAttachNone) DetachBuddy(NULL, false);
	
	// logic transform but no model changed
	if(!pModel) pModel = GetMajorModel();

	if(pModel && m_pPlayerModel && m_pPlayerModel != pModel)
	{		
		// sync the position
		pModel->SetPos(m_pPlayerModel->GetPos());
		pModel->SetDirAndUp(m_pPlayerModel->GetDir(), m_pPlayerModel->GetUp());
		
		// sync wing
		CECModel* pWing = m_pPlayerModel->GetChildModel(_wing);
		m_pPlayerModel->RemoveChildModel(_wing, false);

		CECModel* pWing2 = m_pPlayerModel->GetChildModel(_wing2);
		m_pPlayerModel->RemoveChildModel(_wing2, false);

		if (m_wingType == WINGTYPE_FLYSWORD || m_wingType == WINGTYPE_WING) {
			if (pWing) {
				pModel->AddChildModel(
					_wing,
					false,
					UsingWing() ? "HH_chibang" : "HH_feijian",
					pWing,
					UsingWing() ? "CC_chibang" : "CC_feijian");
			}
		} else if (m_wingType == WINGTYPE_DOUBLEWHEEL) {
			if (pWing) {
				pModel->AddChildModel(
					_wing,
					false,
					_hh_left_foot,
					pWing,
					_cc_fenghuolun);
			}
			if (pWing2) {
				pModel->AddChildModel(
					_wing2,
					false,
					_hh_right_foot,
					pWing2,
					_cc_fenghuolun);
			}
		}
		ShowWing(IsFlying());
		
		// sync the effect
		typedef abase::hash_map<AString, GFXRECORD>::iterator GFXIter;
		for(GFXIter i=m_GfxRecords.begin();i!=m_GfxRecords.end();++i)
		{
			const GFXRECORD& rec = i->second;
			m_pPlayerModel->RemoveGfx(rec.strPath, rec.strHook);
			if (!pModel->GetGfx(rec.strPath, rec.strHook)){
				pModel->PlayGfx(rec.strPath, rec.strHook, rec.fScale);
			}
		}
	}
	
	// change the target
	if (m_pPlayerModel != pModel){
		ClearShowExtendStates();
		if (IsMajorModel(m_pPlayerModel)){
			RemoveEquipGfx();
		}
		DetachWeapon();
		m_pPlayerModel = pModel;
		AttachWeapon();
		if (pModel != NULL){
			ShowExtendStates(0, m_aExtStates, OBJECT_EXT_STATE_COUNT);
		}
		if (IsMajorModel(m_pPlayerModel)){
			AddEquipGfx();
		}
		RecreateActionController();
		RecreateBodyController();
	}
	
	// different ground mode
	SetUseGroundNormal( ShouldUseGroundNormalForCurrentShapeModel() );
	PlayAction(GetMoveStandAction(false, IsFighting()), true);
	
	// restore the attach state
// 	if (iBuddyId)
// 	{
// 		if( !bHangerOn )
// 			AttachBuddy(iBuddyId);
// 		else
// 		{
// 			CECPlayer* pBuddy = m_pPlayerMan->GetPlayer(iBuddyId);
// 			if( pBuddy ) pBuddy->AttachBuddy(m_PlayerInfo.cid);
// 		}
// 	}
	ScaleBody(m_fScaleBySkill);
}

void CECPlayer::AttachBuddy(int iBuddy)
{
}

void CECPlayer::DetachBuddy(CECPlayer* pBuddy, bool bResetData)
{
}

//	Load player's skeleton
bool CECPlayer::LoadPlayerSkeleton(bool bAtOnce)
{
	// remove disabled equipments before loading
	int aEnabledEquips[SIZE_ALL_EQUIPIVTR];
	for (__int64 i=0; i < SIZE_ALL_EQUIPIVTR; i++)
	{
		aEnabledEquips[i] = 
			((1 << i) & m_i64EquipDisabled) == 1 ? 0 : m_aEquips[i];
	}

	const char* szPetPath = NULL;
	if (m_RidingPet.id)
		szPetPath = GetRidingPetFileName(m_RidingPet.id);

	EC_PLAYERLOADRESULT Ret;
	int aEquips[IVTRSIZE_EQUIPPACK];
	__int64 EquipMask = 0;
	if (aEnabledEquips[EQUIPIVTR_WEAPON] > 0)
		EquipMask |=  1 << EQUIPIVTR_WEAPON;
	if (aEnabledEquips[EQUIPIVTR_FASHION_WEAPON] > 0)
		EquipMask |=  1 << EQUIPIVTR_FASHION_WEAPON; 
	memcpy(aEquips, aEnabledEquips, sizeof(aEquips));
	DecideWeaponLoad(aEquips, EquipMask);
	if (bAtOnce || !IsLoadThreadReady())
	{
		// 正常情况下只有HostPlayer可以走到这里
		if (!LoadPlayerModel(m_iProfession, m_iGender, m_CustomizeData.bodyID, aEquips, szPetPath, Ret,false,false))
		{
			a_LogOutput(1, "CECPlayer::Init, failed to call LoadPlayerModel() !");
			return false;
		}

		SetPlayerLoadedResult(Ret);

		if(IsShapeChanged() && !QueueLoadDummyModel(m_iShape, true))
		{
			// ignore the dummy model loading failure
			a_LogOutput(1, "CECPlayer::Init, failed to call QueueLoadDummyModel() !");
		}
	}
	else
	{
		int iType = m_iCID == OCID_HOST_NAVIGATER ? MTL_ECM_HOST_NAVIGATER : MTL_ECM_PLAYER;
		QueueECModelForLoad(
			iType,
			m_PlayerInfo.cid,
			m_dwBornStamp,
			GetPos(),
			szPetPath,
			m_iProfession,
			m_iGender,
			m_CustomizeData.bodyID,
			aEquips);

		if(IsShapeChanged())
			QueueLoadDummyModel(m_iShape, false);
	}

	return true;
}

bool CECPlayer::QueueLoadEquips(const int* pEquips, __int64 Mask, bool bAtOnce)
{
	if (bAtOnce || !IsLoadThreadReady())
	{
		EquipsLoadResult Ret;

		if (!LoadPlayerEquips(m_iProfession, m_iGender, Mask, pEquips, Ret))
			return false;

		SetEquipsLoadedResult(Ret, true);
	}
	else
	{
		QueueECModelForLoad(
			MTL_ECM_PLAYER_EQUIP,
			m_PlayerInfo.cid,
			m_dwBornStamp,
			GetPos(),
			NULL,
			m_iProfession,
			m_iGender,
			Mask,
			const_cast<int*>(pEquips));
	}

	return true;
}

bool CECPlayer::QueueLoadPetModel(const char* szPetPath, bool bAtOnce)
{
	if (bAtOnce || !IsLoadThreadReady())
	{
		CECModel* pPetModel = NULL;
		
		if (!LoadPetModel(szPetPath, &pPetModel) || !pPetModel)
			return false;

		if (!SetPetLoadResult(pPetModel))
		{
			A3DRELEASE(pPetModel);
			return false;
		}
	}
	else
	{
		QueueECModelForLoad(MTL_ECM_PET, m_PlayerInfo.cid, m_dwBornStamp, GetPos(), szPetPath);
	}

	return true;
}

bool CECPlayer::QueueLoadFace(bool bAtOnce)
{
	if( bAtOnce || !IsLoadThreadReady() )
	{
		EC_PLAYERLOADRESULT Ret;
		memset(&Ret, 0, sizeof(Ret));
		Ret.dwValidMask |= PLAYERLOADRESULT_PLAYERFACE;
		Ret.pFaceModel = ThreadLoadFaceModel(m_iProfession, m_iGender, m_CustomizeData.bodyID);
		if( !Ret.pFaceModel )
			return false;

		if( !SetPlayerLoadedResult(Ret) )
		{
			A3DRELEASE(Ret.pFaceModel);
			return false;
		}
	}
	else
	{
		QueueECModelForLoad(MTL_ECM_PLAYER_FACE, m_PlayerInfo.cid, m_dwBornStamp, GetPos(), NULL,
			m_iProfession, m_iGender, m_CustomizeData.bodyID);
	}

	return true;
}

bool CECPlayer::QueueLoadDummyModel(int iShape, bool bAtOnce)
{
	int iShapeType = PLAYERMODEL_GETTYPE(iShape);
	int iShapeID = PLAYERMODEL_GETID(iShape);
	
	// ensure this function only used in dummy model loading
	ASSERT(iShapeType != PLAYERMODEL_MAJOR && iShapeType < PLAYERMODEL_MAX);
	
	// If model already loaded, use it directly.
	if(m_aShapeID[iShapeType] == iShapeID && m_pModels[iShapeType])
	{
		ApplyShapeModelChange(m_pModels[iShapeType]);
		return true;
	}
	
	// try to load a new dummy model
	if (bAtOnce || !IsLoadThreadReady())
	{
		CECModel* pDummyModel = NULL;
		
		if (!LoadDummyModel(iShape, &pDummyModel) || !pDummyModel)
			return false;
		
		if (!SetDummyLoadResult(iShape, pDummyModel))
		{
			A3DRELEASE(pDummyModel);
			return false;
		}
	}
	else
	{
		int iType = (m_iCID == OCID_HOST_NAVIGATER) ? MTL_ECM_HOST_NAVIGATER_DUMMY:MTL_ECM_PLAYER_DUMMY;
		QueueECModelForLoad(
			iType, 
			m_PlayerInfo.cid,
			m_dwBornStamp,
			GetPos(),			
			NULL,
			iShape);
	}
	
	return true;
}


void CECPlayer::RideOnPet(int id, unsigned short color)
{
	m_CandPet.id		= id;
	m_CandPet.color	= color;
	m_RidingPet.id		= id;
	m_RidingPet.color	= color;
}

void CECPlayer::GetOffPet(bool bResetData)
{
	int iBuddyId = 0;
	m_CandPet.Reset();

	SetNamePos(A3DVECTOR3(0.0f));

	if (m_AttachMode != enumAttachNone)
	{
		iBuddyId = m_iBuddyId;
		DetachBuddy(NULL, bResetData);
	}

	if (m_pPetModel)
	{
		m_pPetModel->RemoveChildModel(_hanger_ride, false);
		A3DRELEASE(m_pPetModel);
	}

	if( bResetData )
		m_RidingPet.Reset();

	//	Re-calculate player's AABB
	CalcPlayerAABB();
	SetUseGroundNormal(ShouldUseGroundNormalForCurrentShapeModel());
	SetPos(GetPos());

	if (iBuddyId)
		AttachBuddy(iBuddyId);
	else
		PlayAction(GetMoveStandAction(false, IsFighting()), true);
}

#define SET_SKIN_SHOW_MASK(index, b) \
{ \
	dwSkinShowMask |= (1 << (index)); \
	if (b) \
		dwSkinShowFlag |= (1 << (index)); \
	else \
		dwSkinShowFlag &= ~(1 << (index)); \
}

inline void _get_skin_show_mask(
	DWORD& dwChangeMask,
	int& nLowerMethod,
	DWORD& dwSkinShowMask,
	DWORD& dwSkinShowFlag,
	bool bFashion)
{

	for (int i = enumSkinShowUpperBody; i <= enumSkinShowUpperLowerAndWrist; i++)
	{
		if ((1 << i) & dwChangeMask)
		{
			switch (i)
			{
			case enumSkinShowUpperBody:
				if (bFashion)
				{
					SET_SKIN_SHOW_MASK(SKIN_FASHION_WRIST_INDEX, true)
					SET_SKIN_SHOW_MASK(SKIN_FASHION_LOWER_INDEX, true)
			
					if (nLowerMethod == enumSkinShowLowerAndFoot)
						SET_SKIN_SHOW_MASK(SKIN_FASHION_FOOT_INDEX, false)
					else
						SET_SKIN_SHOW_MASK(SKIN_FASHION_FOOT_INDEX, true)
				}
				else
				{
					SET_SKIN_SHOW_MASK(SKIN_WRIST_INDEX, true)
					SET_SKIN_SHOW_MASK(SKIN_LOWER_INDEX, true)

					if (nLowerMethod == enumSkinShowLowerAndFoot)
						SET_SKIN_SHOW_MASK(SKIN_FOOT_INDEX, false)
					else
						SET_SKIN_SHOW_MASK(SKIN_FOOT_INDEX, true)
				}
				break;
			case enumSkinShowWrist:
				break;
			case enumSkinShowLowerBody:
				nLowerMethod = enumSkinShowLowerBody;
				if (bFashion)
					SET_SKIN_SHOW_MASK(SKIN_FASHION_FOOT_INDEX, true)
				else
					SET_SKIN_SHOW_MASK(SKIN_FOOT_INDEX, true)
				break;
			case enumSkinShowFoot:
				break;
			case enumSkinShowUpperAndLower:
				if (bFashion)
				{
					SET_SKIN_SHOW_MASK(SKIN_FASHION_WRIST_INDEX, true)
					SET_SKIN_SHOW_MASK(SKIN_FASHION_LOWER_INDEX, false)
					SET_SKIN_SHOW_MASK(SKIN_FASHION_FOOT_INDEX, true)
				}
				else
				{
					SET_SKIN_SHOW_MASK(SKIN_WRIST_INDEX, true)
					SET_SKIN_SHOW_MASK(SKIN_LOWER_INDEX, false)
					SET_SKIN_SHOW_MASK(SKIN_FOOT_INDEX, true)
				}
				break;
			case enumSkinShowUpperAndWrist:
				if (bFashion)
				{
					SET_SKIN_SHOW_MASK(SKIN_FASHION_WRIST_INDEX, false)
					SET_SKIN_SHOW_MASK(SKIN_FASHION_LOWER_INDEX, true)

					if (nLowerMethod == enumSkinShowLowerAndFoot)
						SET_SKIN_SHOW_MASK(SKIN_FASHION_FOOT_INDEX, false)
					else
						SET_SKIN_SHOW_MASK(SKIN_FASHION_FOOT_INDEX, true)
				}
				else
				{
					SET_SKIN_SHOW_MASK(SKIN_WRIST_INDEX, false)
					SET_SKIN_SHOW_MASK(SKIN_LOWER_INDEX, true)

					if (nLowerMethod == enumSkinShowLowerAndFoot)
						SET_SKIN_SHOW_MASK(SKIN_FOOT_INDEX, false)
					else
						SET_SKIN_SHOW_MASK(SKIN_FOOT_INDEX, true)
				}
				break;
			case enumSkinShowLowerAndFoot:
				nLowerMethod = enumSkinShowLowerAndFoot;
				if (bFashion)
					SET_SKIN_SHOW_MASK(SKIN_FASHION_FOOT_INDEX, false)
				else
					SET_SKIN_SHOW_MASK(SKIN_FOOT_INDEX, false)
				break;
			case enumSkinShowUpperLowerAndWrist:
				if (bFashion)
				{
					SET_SKIN_SHOW_MASK(SKIN_FASHION_WRIST_INDEX, false)
					SET_SKIN_SHOW_MASK(SKIN_FASHION_LOWER_INDEX, false)
					SET_SKIN_SHOW_MASK(SKIN_FASHION_FOOT_INDEX, true)
				}
				else
				{
					SET_SKIN_SHOW_MASK(SKIN_WRIST_INDEX, false)
					SET_SKIN_SHOW_MASK(SKIN_LOWER_INDEX, false)
					SET_SKIN_SHOW_MASK(SKIN_FOOT_INDEX, true)
				}
				break;
			}
		}
	}
}

bool CECPlayer::SetEquipsLoadedResult(EquipsLoadResult& Result, bool bUpdateAtOnce)
{
	if (!GetMajorModel())
		return false;

	A3DSkinModel* pA3DModel = GetMajorModel()->GetA3DSkinModel();
	DWORD dwSkinShowMask = 0, dwSkinShowFlag = 0;

	_get_skin_show_mask(Result.dwShowMask, m_nLowerEquipMethod, dwSkinShowMask, dwSkinShowFlag, false);
	_get_skin_show_mask(Result.dwFashionShowMask, m_nLowerFashionEquipMethod, dwSkinShowMask, dwSkinShowFlag, true);

	if( !ShouldUseClothedModel() )
	{
		for (int i = 0; i < NUM_SKIN_INDEX; i++)
		{
			if( i == SKIN_HEAD_INDEX || i == SKIN_BODY_INDEX )
				continue;

			if( Result.aSkins[i][0] ) g_pGame->ReleaseA3DSkin(Result.aSkins[i][0]);
			if( Result.aSkins[i][1] ) g_pGame->ReleaseA3DSkin(Result.aSkins[i][1]);
			if( Result.aSkins[i][2] ) g_pGame->ReleaseA3DSkin(Result.aSkins[i][2]);

			Result.aSkins[i][0] = Result.aSkins[i][1] = Result.aSkins[i][2] = NULL;
		}
	}
	else
	{
		for (int i = 0; i < NUM_SKIN_INDEX; i++)
		{
			DWORD dwMask = (1 << i);

			if (dwMask & Result.dwSkinChangeMask)
			{
				ReplaceCurSkin(i, NULL);

				g_pGame->ReleaseA3DSkin(m_aSkins[i][0]);
				g_pGame->ReleaseA3DSkin(m_aSkins[i][1]);
				g_pGame->ReleaseA3DSkin(m_aSkins[i][2]);

				m_aSkins[i][0] = Result.aSkins[i][0];
				m_aSkins[i][1] = Result.aSkins[i][1];
				m_aSkins[i][2] = Result.aSkins[i][2];

				for (int j = 0; j < 3; j++)
				{
					if (NULL != m_aSkins[i][j])
						m_aSkins[i][j]->SetAlphaSortWeight(_skin_alpha_map[i]);
				}
			}

			if (dwMask & dwSkinShowMask)
				pA3DModel->ShowSkin(i, (dwMask & dwSkinShowFlag) != 0);
		}
	}

	if (bUpdateAtOnce)
		UpdateCurSkins();

	if (Result.bWingChanged)
	{
		m_wingType = Result.wingType;
		m_pPlayerModel->RemoveChildModel(_wing);
		m_pPlayerModel->RemoveChildModel(_wing2);

		if (m_wingType == WINGTYPE_FLYSWORD || m_wingType == WINGTYPE_WING) {
			if (Result.pWing) {
				m_pPlayerModel->AddChildModel(
					_wing,
					false,
					UsingWing() ? "HH_chibang" : "HH_feijian",
					Result.pWing,
					UsingWing() ? "CC_chibang" : "CC_feijian");
			}
		} else if (m_wingType == WINGTYPE_DOUBLEWHEEL) {
			if (Result.pWing) {
				m_pPlayerModel->AddChildModel(
					_wing,
					false,
					_hh_left_foot,
					Result.pWing,
					_cc_fenghuolun);
			}
			if (Result.pWing2) {
				m_pPlayerModel->AddChildModel(
					_wing2,
					false,
					_hh_right_foot,
					Result.pWing2,
					_cc_fenghuolun);
			}
		}

		if (Result.pWing && Result.pWing->GetA3DSkinModel())
		{
			Result.pWing->GetA3DSkinModel()->SetAlphaSortID(m_PlayerInfo.cid);
			Result.pWing->GetA3DSkinModel()->SetAlphaSortWeight(SKIN_SORT_WING);
		}
		
		if (Result.pWing2 && Result.pWing2->GetA3DSkinModel())
		{
			Result.pWing2->GetA3DSkinModel()->SetAlphaSortID(m_PlayerInfo.cid);
			Result.pWing2->GetA3DSkinModel()->SetAlphaSortWeight(SKIN_SORT_WING);
		}

		if (Result.pWing || Result.pWing2)
		{
			// HACK: ensure weapon added after wings to avoid alpha sort bug
			if(!Result.bWeaponChanged && !Result.bFashionWeaponChanged){				
				DetachWeapon();
			}
		}
	}

	ShowWing(IsFlying());

	if (Result.bWeaponChanged || Result.bFashionWeaponChanged)
	{
		if (Result.bFashionWeaponChanged)
		{
			m_iFashionWeaponType = Result.uFashionAttackType;
			m_strLeftFashionWeapon = Result.strLeftFashionWeapon;
			m_strRightFashionWeapon = Result.strRightFashionWeapon;
		}
		if (Result.bWeaponChanged) 
		{
			m_uAttackType = Result.uAttackType;
			m_strLeftWeapon = Result.strLeftWeapon;
			m_strRightWeapon = Result.strRightWeapon;
		}		
		SetWeaponResult(Result);
	}else if (Result.bWingChanged && (Result.pWing || Result.pWing2)){
		AttachWeapon();
	}

	int idFullSuite = SearchFullSuite();
	if( idFullSuite == 0 )
	{
		RemoveFullSuiteGFX();
		m_idFullSuite = 0;
	}
	else if( idFullSuite != m_idFullSuite )
	{
		RemoveFullSuiteGFX();
		m_idFullSuite = idFullSuite;
		AddFullSuiteGFX();
	}

	if( Result.stoneUpperBody != 0xff && m_stoneUpperBody != Result.stoneUpperBody )
	{
		RemoveUpperBodyStones();
		m_stoneUpperBody = Result.stoneUpperBody;
		AddUpperBodyStones();
	}
	if( Result.stoneWrist != 0x00ff && m_stoneWrist != Result.stoneWrist )
	{
		RemoveWristStones();
		m_stoneWrist = Result.stoneWrist;
		AddWristStones();
	}
	if( Result.stoneLowerBody != 0x00ff && m_stoneLowerBody != Result.stoneLowerBody )
	{
		RemoveLowerBodyStones();
		m_stoneLowerBody = Result.stoneLowerBody;
		AddLowerBodyStones();
	}
	if( Result.stoneFoot != 0x00ff && m_stoneFoot != Result.stoneFoot )
	{
		RemoveFootStones();
		m_stoneFoot = Result.stoneFoot;
		AddFootStones();
	}	
	if( Result.stoneWeapon != 0x00ff )
	{
		m_stoneWeaponShown = 0;
		m_stoneWeapon = Result.stoneWeapon;
		AddWeaponStones();
	}else if (IsShowFashionWeapon()){
		m_stoneWeapon = 0;
		m_stoneWeaponShown = 0;		
	}
	ScaleChildModel();
	return true;
}

void CECPlayer::SetRidingPetColor(unsigned short clr)
{
	m_RidingPet.color = clr;
	
	A3DCOLOR clrModel;
	if (!m_RidingPet.GetColor(clrModel))
		clrModel = RIDINGPET::GetDefaultColor();

	SetRidingPetColor(m_pPetModel, clrModel);
}

void CECPlayer::SetRidingPetColor(CECModel *pModel, A3DCOLOR clr)
{
	//	设置修改模型上的颜色

	while (true)
	{
		if (!pModel) break;

		A3DSkinModel *pSkinModel = pModel->GetA3DSkinModel();
		if (!pSkinModel)	break;

		for (int i = 0; i < pSkinModel->GetSkinNum(); ++ i)
		{
			A3DSkin *pSkin = pSkinModel->GetA3DSkin(i);
			if (!pSkin) continue;
			for(int idTex=0; idTex<pSkin->GetTextureNum(); idTex ++)
			{
				A3DTexture * pTex = pSkin->GetTexture(idTex);
				if( pTex && pTex->IsShaderTexture() )
				{
					A3DShader * pShader = (A3DShader *) pTex;
					if (strstr(pShader->GetMapFile(), "rewu.sdr"))
						pShader->GetGeneralProps().dwTFactor = clr;
				}
			}
		}

		break;
	}
}


bool CECPlayer::SetPetLoadResult(CECModel* pPetModel)
{
	ASSERT(pPetModel);

	if (!m_RidingPet.id || !GetMajorModel() || m_CandPet.id || !pPetModel)
		return false;

	if (m_pPetModel)
	{
		RIDINGPET pet = m_RidingPet;
		GetOffPet();
		m_RidingPet = pet;
	}

	const A3DVECTOR3 vCurPos = GetPos();

	m_aabbServer.Center = vCurPos + A3DVECTOR3(0.0f, m_aabbServer.Extents.y, 0.0f);
	m_aabbServer.CompleteMinsMaxs();

	m_pPetModel = pPetModel;

	//	将宠物颜色设置到模型上
	SetRidingPetColor(m_RidingPet.color);

	SetUseGroundNormal(true);
	m_pPetModel->SetAffectedByParent(false);
	m_pPetModel->SetPos(vCurPos);
	m_pPetModel->SetDirAndUp(GetDir(), GetUp());

	int iIndex;
	A3DSkeletonHook* pHook = GetMajorModel()->GetA3DSkinModel()->GetSkeleton()->GetHook(_cc_ride, &iIndex);
	if (pHook) pHook->SetFixDirFlag(true);

	int iBuddyId = m_iBuddyId;
	if (m_AttachMode != enumAttachNone)
		DetachBuddy();

	m_pPetModel->AddChildModel(_hanger_ride, false, _hh_ride, GetMajorModel(), _cc_ride);
	m_pPetModel->GetA3DSkinModel()->Update(0);

	if (iBuddyId)
		AttachBuddy(iBuddyId);
	else
		PlayAction(GetMoveStandAction(false, IsFighting()), true);

	return true;
}

//	Set loaded model to player object, this function is used in multithread loading process
bool CECPlayer::SetPlayerLoadedResult(EC_PLAYERLOADRESULT& Ret)
{
	int i;

	if(!ShouldUseModel())
		return false;

	// model loading
	if(Ret.dwValidMask & PLAYERLOADRESULT_PLAYERMODEL)
	{
		if (GetMajorModel())
			return false;
		
		// set the major model
		m_pPlayerModel = Ret.pPlayerModel;
		m_pPlayerModel->SetAffectedByParent(false);
		m_pPlayerModel->GetA3DSkinModel()->SetInheritTransFlag(false);
		m_pModels[PLAYERMODEL_MAJOR] = m_pPlayerModel;
		m_GfxRecords.clear();
		
		RecreateActionController();
		RecreateBodyController();
	}

	// dummy model loading
	if(Ret.dwValidMask & PLAYERLOADRESULT_DUMMYMODEL)
	{
		if( !SetDummyLoadResult(Ret.iShape, Ret.pDummyModel) )
			A3DRELEASE(Ret.pDummyModel);
	}
	
	// pet model loading
	if(Ret.dwValidMask & PLAYERLOADRESULT_PETMODEL)
	{
		if (Ret.pPetModel && !SetPetLoadResult(Ret.pPetModel))
			A3DRELEASE(Ret.pPetModel);
	}

	// player model loading
	if(Ret.dwValidMask & PLAYERLOADRESULT_PLAYERMODEL)
	{
		if (!m_pPetModel)
		{
			m_pPlayerModel->SetPos(GetPos());
			m_pPlayerModel->SetDirAndUp(GetDir(), GetUp());
			m_pPlayerModel->GetA3DSkinModel()->Update(0);
			SetUseGroundNormal(ShouldUseGroundNormalForCurrentShapeModel());
			// make the player stand at beginning
			PlayAction(ACT_STAND, true, 0);
		}

		for (i = 0; i < SIZE_ALL_EQUIPIVTR; i++)
		{
			if (m_aEquips[i] > 0 && IsShownEquip(i) && GetMajorModel())
			{
				DWORD dwRealID = GetRealElementID(i, m_aEquips[i]);
				GetMajorModel()->OnScriptChangeEquip(dwRealID, 1, false);
			}
		}
	}
	
	// equips model loading
	if(Ret.dwValidMask & PLAYERLOADRESULT_EQUIPMODEL)
	{
		bool bOnlyEquip = (0 == (Ret.dwValidMask & (~PLAYERLOADRESULT_EQUIPMODEL)));
		if(!SetEquipsLoadedResult(Ret.EquipResult, bOnlyEquip) && bOnlyEquip)
		{
			return false;
		}
	}
	
	// player model loading
	if(Ret.dwValidMask & PLAYERLOADRESULT_PLAYERMODEL)
	{
		ReplaceCurSkin(SKIN_BODY_INDEX, NULL);
		ReplaceCurSkin(SKIN_HEAD_INDEX, NULL);

		for (i = 0; i < 3; i++)
		{		
 			g_pGame->ReleaseA3DSkin(m_aSkins[SKIN_BODY_INDEX][i]);
 			g_pGame->ReleaseA3DSkin(m_aSkins[SKIN_HEAD_INDEX][i]);
			m_aSkins[SKIN_BODY_INDEX][i] = Ret.EquipResult.aSkins[SKIN_BODY_INDEX][i];
			m_aSkins[SKIN_HEAD_INDEX][i] = Ret.EquipResult.aSkins[SKIN_HEAD_INDEX][i];
			m_pBodyShader[i] = Ret.pBodyShaders[i];
		}

		if (ShouldUseFaceModel()){
			SetFaceModel(Ret.pFaceModel);
		}else{
			SetFaceModel(NULL);
			A3DRELEASE(Ret.pFaceModel);
		}

		//	Update some customized data
		SetBodyColor(m_CustomizeData.colorBody);
		UpdateBodyScales();

		//	Set model's position and orientation
		m_pPlayerModel->SetPos(GetPos());
		m_pPlayerModel->SetDirAndUp(GetDir(), GetUp());
		
		//  Update the transparent level
		SetTransparent(GetTransparentLimit());

		// Set skin model ID, so same ID can deal with the aphla sort in same space
		if (NULL != m_pPlayerModel->GetA3DSkinModel())
		{
			m_pPlayerModel->GetA3DSkinModel()->SetAlphaSortID(m_PlayerInfo.cid);
			m_pPlayerModel->GetA3DSkinModel()->SetAlphaSortWeight(SKIN_SORT_DEFAULT);
		}

		//	Force to update model once completely to avoid rendering error
		m_pPlayerModel->SetAutoUpdateFlag(false);
		m_pPlayerModel->Tick(1);
		m_pPlayerModel->SetAutoUpdateFlag(true);
		if( m_pFaceModel ) m_pFaceModel->Tick(1);

		m_dwResFlags = RESFG_ALL;
		OnAllResourceReady();
		UpdateCurSkins();
		UpdateGodEvilSprite();
	}

	// 加载玩家的自定义脸数据
	if( Ret.dwValidMask & PLAYERLOADRESULT_PLAYERFACE ){
		if( !ShouldUseFaceModel() || !GetMajorModel() ){
			A3DRELEASE(Ret.pFaceModel);
		}else{
			SetFaceModel(Ret.pFaceModel);
		}
	}

	return true;
}

void CECPlayer::SetFaceModel(CECFace *pFaceModel){
	if (pFaceModel == m_pFaceModel){
		return;
	}
	ReleaseFaceModel();
	if (!pFaceModel){
		return;
	}
	m_pFaceModel = pFaceModel;
	if (m_bIsChangingFace){
		m_pFaceModel->SetFaceData(m_CustomizeData.faceData);
	}else{
		if( m_CustomizeData.faceData.idFaceTex == 0 && (m_iProfession != PROF_ORC || m_iGender != GENDER_MALE) ){
			m_CustomizeData.faceData = *m_pFaceModel->GetFaceData();
			m_pFaceModel->UpdateAll();
		}else{
			m_pFaceModel->SetFaceData(m_CustomizeData.faceData);
		}		
		if (InFashionMode()){
			UpdateHairModel(true, m_aEquips[EQUIPIVTR_FASHION_HEAD]);
		}else{
			UpdateHairModel(true, m_aEquips[EQUIPIVTR_HEAD]);
		}
	}
	AttachFaceModel();
}

void CECPlayer::AttachFaceModel(){
	if (!m_pFaceModel){
		return;
	}
	if (m_pFaceModel->GetA3DSkinModel()){
		m_pFaceModel->GetA3DSkinModel()->SetAlphaSortID(m_PlayerInfo.cid);
		m_pFaceModel->GetA3DSkinModel()->SetAlphaSortWeight(SKIN_SORT_HEAD);
	}
	if (GetMajorModel()){
		m_pFaceModel->SetParent(GetMajorModel()->GetA3DSkinModel());
		GetMajorModel()->ShowSkin(SKIN_HEAD_INDEX, false);
		if (GetMajorModel()->FindComboModel(m_pFaceModel->GetECModel()) < 0){
			GetMajorModel()->AddComboModel(m_pFaceModel->GetECModel());
		}
	}
}

bool CECPlayer::SetDummyLoadResult(int iShape, CECModel* pModel)
{
	int iShapeType = PLAYERMODEL_GETTYPE(iShape);
	int iShapeID = PLAYERMODEL_GETID(iShape);
	a_LogOutput(1, "CECPlayer::SetDummyLoadResult(iShape=%d)(iShapeType=%d,iShapeID=%d)", iShape, iShapeType, iShapeID);
	
	if (iShapeType == PLAYERMODEL_MAJOR ||
		iShapeType < 0 || iShapeType >= PLAYERMODEL_MAX){
		a_LogOutput(1, "ApplyShapeModelChange invalid(iShapeType=%d, iShapeID=%d)", iShapeType, iShapeID);
		ASSERT(false);
		return false;
	}
	if (iShape != m_iShape){	//	2014-8-4:频繁变身时，资源加载落后于m_iShape变化（后者在处理协议时即修改），此时，丢弃结果
		a_LogOutput(1, "ApplyShapeModelChange ignored cause shape mismatch(m_iShape=%d, iShapeType=%d, iShapeID=%d)", m_iShape, iShapeType, iShapeID);
		return false;
	}

	CECModel* pOldModel = m_pModels[iShapeType];
	if(pOldModel != pModel)
	{
		m_pModels[iShapeType] = pModel;
		m_aShapeID[iShapeType] = iShapeID;
		if(pModel) pModel->SetAffectedByParent(false);
	}
	
	ApplyShapeModelChange(pModel);

	if (pOldModel && pOldModel != pModel && pOldModel != GetPlayerModel()){
		if (IsLoadThreadReady()){
			QueueECModelForRelease(pOldModel);
		}else{
			pOldModel->Release();
			delete pOldModel;
		}
	}
	
	return true;
}

bool CECPlayer::UpdateGodEvilSprite()
{
	// 简单模型不显示小精灵
	if( !ShouldUseClothedModel())
		return false;

	// first determine new sprite id.
	CECSprite::SPRITE_ID idSprite = CECSprite::SPRITE_NULL;
	if( GetBasicProps().iLevel2 >= 20 )
	{
		switch(GetBasicProps().iLevel2)
		{
		case 20:
			idSprite = CECSprite::SPRITE_GOD1;
			break;
		case 21:
			idSprite = CECSprite::SPRITE_GOD2;
			break;
		case 22:
			idSprite = CECSprite::SPRITE_GOD3;
			break;

		case 30:
			idSprite = CECSprite::SPRITE_EVIL1;
			break;
		case 31:
			idSprite = CECSprite::SPRITE_EVIL2;
			break;
		case 32:
			idSprite = CECSprite::SPRITE_EVIL3;
			break;
		}
	}

	if( m_pSprite && idSprite != m_pSprite->GetSpriteID() )
	{
		// first try to release old ones.
		if( m_pSprite )
		{
			m_pSprite->Release();
			delete m_pSprite;
			m_pSprite = NULL;
		}
	}

	if( NULL == m_pSprite && idSprite != CECSprite::SPRITE_NULL )
	{
		m_pSprite = new CECSprite();
		if( !m_pSprite->Init(idSprite, this) )
		{
			m_pSprite->Release();
			delete m_pSprite;
			m_pSprite = NULL;
			a_LogOutput(1, "CECPlayer::UpdateGodEvilSprite(), failed to load sprite!");
		}
		else
			m_pSprite->SetPos(GetPos() + A3DVECTOR3(0.0f, 2.5f, 0.0f));
	}

	return true;
}

bool CECPlayer::UpdateGoblin()
{

	return true;
}

//  Render goblin or sprite
void CECPlayer::RenderGoblinOrSprite(CECViewport* pViewport)
{	
}

inline void _fade_out_gfx(A3DGFXEx*& pGfx, float fDeltaTime)
{
	if (!pGfx) return;

	if (pGfx->GetState() != ST_STOP)
	{
		float fAlpha = pGfx->GetAlpha();
		fAlpha -= fDeltaTime;

		if (fAlpha <= 0)
		{
			pGfx->Stop();
		//	g_pGame->GetA3DGFXExMan()->CacheReleasedGfx(pGfx);
			g_pGame->GetGFXCaster()->ReleaseGFXEx(pGfx);
			pGfx = NULL;
		}
		else
			pGfx->SetAlpha(fAlpha);
	}
	else
	{
	//	g_pGame->GetA3DGFXExMan()->CacheReleasedGfx(pGfx);
		g_pGame->GetGFXCaster()->ReleaseGFXEx(pGfx);
		pGfx = NULL;
	}
}

//	Tick routine
bool CECPlayer::Tick(DWORD dwDeltaTime)
{
	CECObject::Tick(dwDeltaTime);
	if (m_pBodyController){
		m_pBodyController->Tick(dwDeltaTime);
	}

	DWORD dwRealTime = g_pGame->GetRealTickTime();

	UpdateCurSkins();
	UpdateBoothModel();
	UpdateBoothBar();

	//	Update last said words
// 	if (m_pPateLastWords1 && m_pPateLastWords1->GetItemNum())
// 	{
// 		if (m_strLastSayCnt.IncCounter(dwDeltaTime))
// 		{
// 			//	Clear string
// 			m_strLastSayCnt.Reset();
// 			m_pPateLastWords1->Clear();
// 			m_pPateLastWords2->Clear();
// 		}
// 		else
// 		{
// 			m_pPateLastWords1->Tick(dwDeltaTime);
// 			m_pPateLastWords2->Tick(dwDeltaTime);
// 		}
// 	}

	A3DTerrainWater* pWater = g_pGame->GetGameRun()->GetWorld()->GetTerrainWater();
	float fDeltaTime = dwDeltaTime / 1000.0f;

	A3DVECTOR3 vPos = GetPos();
	float fWaterHei = pWater->GetWaterHeight(vPos);
	float fGrndHei = g_pGame->GetGameRun()->GetWorld()->GetTerrainHeight(vPos);
	float fWaterOff = vPos.y - fWaterHei;

	if (fWaterHei > fGrndHei  + .3f &&
		((fWaterOff > -10.f && fWaterOff <= 0) || (fWaterOff > 0 && fWaterOff < 10.0f && IsFlying())))
	{
		A3DMATRIX4 mat;

		if (pWater->IsUnderWater(g_pGame->GetViewport()->GetA3DCamera()->GetPos()))
			vPos.y = fWaterHei - .01f;
		else
			vPos.y = fWaterHei + .01f;

		A3DVECTOR3 vDir = GetDir();
		vDir.y = 0;
		vDir.Normalize();
		mat = a3d_TransformMatrix(vDir, g_vAxisY, vPos);

		if (IsPlayerMoving())
		{
			if (!m_pWaterWaveMove)
				m_pWaterWaveMove = g_pGame->GetGFXCaster()->LoadGFXEx(res_GFXFile(RES_GFX_WATER_WAVE_MOVE));

			if (m_pWaterWaveMove)
			{
				if (m_pWaterWaveMove->GetState() == ST_STOP
				 || m_pWaterWaveMove->GetAlpha() < 1.0f)
				{
					m_pWaterWaveMove->SetAlpha(1.0f);
					float fSpeed = GetFlySpeed() / GetSwimSpeed();
					if (fSpeed > 2.0f) fSpeed = 2.0f;
					if (IsFlying()) m_pWaterWaveMove->SetPlaySpeed(fSpeed);
					else m_pWaterWaveMove->SetPlaySpeed(1.0f);
					m_pWaterWaveMove->Start(true);
				}

				m_pWaterWaveMove->SetParentTM(mat);
			}

			_fade_out_gfx(m_pWaterWaveStill, fDeltaTime);
		}
		else
		{
			if (!m_pWaterWaveStill)
				m_pWaterWaveStill = g_pGame->GetGFXCaster()->LoadGFXEx(res_GFXFile(RES_GFX_WATER_WAVE_STILL));

			if (m_pWaterWaveStill)
			{
				if (m_pWaterWaveStill->GetState() == ST_STOP
				 || m_pWaterWaveStill->GetAlpha() < 1.0f)
				{
					m_pWaterWaveStill->SetAlpha(1.0f);
					m_pWaterWaveStill->Start(true);
				}

				m_pWaterWaveStill->SetParentTM(mat);
			}

			_fade_out_gfx(m_pWaterWaveMove, fDeltaTime);
		}
	}
	else
	{
		_fade_out_gfx(m_pWaterWaveStill, fDeltaTime);
		_fade_out_gfx(m_pWaterWaveMove, fDeltaTime);
	}

	bool bFadeAirBubble = true;
	bool bFadeSwimBubble = true;

	if (!pWater->IsUnderWater(g_pGame->GetViewport()->GetA3DCamera()->GetPos()))
	{
		if (m_pAirBubble)
		{
			m_pAirBubble->Stop();
		//	g_pGame->GetA3DGFXExMan()->CacheReleasedGfx(m_pAirBubble);
			g_pGame->GetGFXCaster()->ReleaseGFXEx(m_pAirBubble);
			m_pAirBubble = NULL;
		}

		if (m_pSwimBubble)
		{
			m_pSwimBubble->Stop();
			g_pGame->GetGFXCaster()->ReleaseGFXEx(m_pSwimBubble);
			m_pSwimBubble = NULL;
		}

		bFadeAirBubble = false;
		bFadeSwimBubble = false;
	}
	else if (m_iMoveEnv == MOVEENV_WATER)
	{
		if (IsPlayerMoving())
		{
			if (!m_pSwimBubble)
				m_pSwimBubble = g_pGame->GetGFXCaster()->LoadGFXEx(res_GFXFile(RES_GFX_SWIM_BUBBLE));

			if (m_pSwimBubble)
			{
				if (m_pSwimBubble->GetState() == ST_STOP
				 || m_pSwimBubble->GetAlpha() < 1.0f)
				{
					m_pSwimBubble->SetAlpha(1.0f);
					m_pSwimBubble->Start(true);
				}

				A3DMATRIX4 mat = GetAbsoluteTM();
				A3DVECTOR3 vOff = GetPos() + GetDir() * .8f;
				vOff.y += 1.1f;
				mat.SetRow(3, vOff);
				m_pSwimBubble->SetParentTM(mat);
			}

			bFadeSwimBubble = false;
		}
		else
		{
			if (!m_pAirBubble)
				m_pAirBubble = g_pGame->GetGFXCaster()->LoadGFXEx(res_GFXFile(RES_GFX_AIR_BUBBLE));

			if (m_pAirBubble)
			{
				if (m_pAirBubble->GetState() == ST_STOP
				 || m_pAirBubble->GetAlpha() < 1.0f)
				{
					m_pAirBubble->SetAlpha(1.0f);
					m_pAirBubble->Start(true);
				}

				A3DMATRIX4 mat = GetAbsoluteTM();
				A3DVECTOR3 vOff = GetPos() + GetDir() * .1f;
				vOff.y += 1.6f;
				mat.SetRow(3, vOff);
				m_pAirBubble->SetParentTM(mat);
			}

			bFadeAirBubble = false;
		}
	}

	if (bFadeAirBubble) _fade_out_gfx(m_pAirBubble, fDeltaTime);
	if (bFadeSwimBubble) _fade_out_gfx(m_pSwimBubble, fDeltaTime);

	//	Update bubble text
	if (m_pBubbleTexts)
		m_pBubbleTexts->Tick(dwDeltaTime);

	if( m_pvp.iDuelState == DUEL_ST_INDUEL )
	{
		if (!m_pDuelStateGFX)
			m_pDuelStateGFX = g_pGame->GetGFXCaster()->LoadGFXEx(res_GFXFile(RES_GFX_INDUEL));

		if (m_pDuelStateGFX)
		{
			if (m_pDuelStateGFX->GetState() == ST_STOP || m_pDuelStateGFX->GetAlpha() < 1.0f)
			{
				m_pDuelStateGFX->SetAlpha(1.0f);
				m_pDuelStateGFX->Start(true);
			}

			A3DMATRIX4 mat = GetAbsoluteTM();
			m_pDuelStateGFX->SetParentTM(mat);
		}
	}
	else
	{
		if (m_pDuelStateGFX)
		{
			m_pDuelStateGFX->Stop();
			g_pGame->GetGFXCaster()->ReleaseGFXEx(m_pDuelStateGFX);
			m_pDuelStateGFX = NULL;
		}
	}

	if( m_pSprite )
		m_pSprite->Tick(dwDeltaTime);
	
	if (m_idCandBuddy) AttachBuddy(m_idCandBuddy);

	if (m_CandPet.id && GetMajorModel())
	{
		const char* szPetPath = GetRidingPetFileName(m_CandPet.id);
		QueueLoadPetModel(szPetPath, false);
		m_CandPet.Reset();
	}

	DoSkillStateAction();

	return true;
}

//	Process message
bool CECPlayer::ProcessMessage(const ECMSG& Msg)
{
	return true;
}

//	Play a damaged effect
void CECPlayer::Damaged(int nDamage, DWORD dwModifier/* 0 */,int skill /* 0 */)
{
}
bool CECPlayer::OnDamaged(int skill)
{
	return false;	
}
//	Play an attack effect
void CECPlayer::PlayAttackEffect(int idTarget, int idSkill, int skillLevel, int nDamage,
							DWORD dwModifier, int nAttackSpeed, int* piAttackTime/* NULL */, int nSection)
{
}

//	Set absolute position
void CECPlayer::SetPos(const A3DVECTOR3& vPos)
{
	CECObject::SetPos(vPos);

	m_aabb.Center = vPos + A3DVECTOR3(0.0f, m_aabb.Extents.y, 0.0f);
	m_aabb.CompleteMinsMaxs();
	m_aabbServer.Center = vPos + A3DVECTOR3(0.0f, m_aabbServer.Extents.y, 0.0f);
	m_aabbServer.CompleteMinsMaxs();

	if (m_pPetModel)
		m_pPetModel->SetPos(vPos);
	else if(m_pPlayerModel)
	{
		m_pPlayerModel->SetPos(vPos);
	}

	if (m_iBuddyId && !m_bHangerOn)
	{
	}

	//	test code ...
	/*
	if (g_pGame->GetGameRun()->GetGameState() == CECGameRun::GS_GAME)
	{
		float fHei = g_pGame->GetGameRun()->GetWorld()->GetTerrainHeight(vPos);
		if (fHei > vPos.y + 0.001f)
		{
			g_pGame->RuntimeDebugInfo(0xffffffff, _AL("%f"), fHei - vPos.y);
		}
	}
	*/
}

//	Set absolute forward and up direction
void CECPlayer::SetDirAndUp(const A3DVECTOR3& vDir, const A3DVECTOR3& vUp)
{
	CECObject::SetDirAndUp(vDir, vUp);

	A3DVECTOR3 vNewDir = GetDir();
	A3DVECTOR3 vNewUp = GetUp();

	if (m_pPetModel)
		m_pPetModel->SetDirAndUp(vNewDir, vNewUp);
	else if(m_pPlayerModel)
	{
		m_pPlayerModel->SetDirAndUp(vNewDir, vNewUp);
	}
}

bool CECPlayer::SupportCastSkillWhenMove()const{
	return m_pActionController != NULL && m_pActionController->SupportCastSkillWhenMove();
}

bool CECPlayer::CanCombineWithMoveForSkill(int idSkill)const{
	return m_pActionController != NULL && m_pActionController->CanCombineWithMoveForSkill(idSkill);
}

//	移动、停止移动时调用此函数，用于处理移动攻击中模型朝向和转向
void CECPlayer::StartModelMove(const A3DVECTOR3& vMoveDir, const A3DVECTOR3& vHeadUp, DWORD dwSwitchDirTime){
	if (!m_pBodyController){
		if (dwSwitchDirTime > 0){
			SetDestDirAndUp(vMoveDir, vHeadUp, dwSwitchDirTime);
		}else{
			SetDirAndUp(vMoveDir, vHeadUp);
		}
	}else{
		if (dwSwitchDirTime > 0){
			m_pBodyController->Move(vMoveDir, vHeadUp);
		}else{
			m_pBodyController->MoveImmediatelyTo(vMoveDir, vHeadUp);
		}
	}
}
void CECPlayer::StopModelMove(const A3DVECTOR3& vMoveDir, const A3DVECTOR3& vHeadUp, DWORD dwSwitchDirTime){
	if (!m_pBodyController){
		if (dwSwitchDirTime > 0){
			SetDestDirAndUp(vMoveDir, vHeadUp, dwSwitchDirTime);
		}else{
			SetDirAndUp(vMoveDir, vHeadUp);
		}
	}else{
		m_pBodyController->StopMove(vMoveDir, vHeadUp);
	}
}
void CECPlayer::StopModelMove(){
	if (m_pBodyController){
		m_pBodyController->StopMove();
	}
}

void CECPlayer::ChangeModelMoveDirAndUp(const A3DVECTOR3& vMoveDir, const A3DVECTOR3& vHeadUp){
	if (!m_pBodyController){
		SetDirAndUp(vMoveDir, vHeadUp);
	}else{
		m_pBodyController->ChangeMoveDirAndUp(vMoveDir, vHeadUp);	//	根据当前移动状态，调整移动方向或模型朝向
	}
}

void CECPlayer::ChangeModelTargetDirAndUp(const A3DVECTOR3 &vDir, const A3DVECTOR3 &vUp){
	if (!m_pBodyController){
		SetDirAndUp(vDir, vUp);
	}else{
		m_pBodyController->ChangeModelTargetDirAndUp(vDir, vUp);
	}
}

A3DVECTOR3 CECPlayer::GetModelMoveDir()const{	//	移动时、返回移动朝向，静止时、返回模型朝向
	if (!m_pBodyController){
		return GetDir();
	}else{
		return m_pBodyController->GetModelMoveDir();
	}
}

bool CECPlayer::PlayNonSkillActionWithName(int iAction, const char* szActName, bool bRestart/* =true */, int nTransTime/* =200 */, bool bNoFx/* =false */, bool *pActFlag/* =NULL */, DWORD dwFlagMode/* =COMACT_FLAG_MODE_NONE */){
	return m_pActionController
		&& m_pActionController->PlayNonSkillActionWithName(iAction, szActName, bRestart, nTransTime, bNoFx, pActFlag, dwFlagMode);
}

bool CECPlayer::QueueNonSkillActionWithName(int iAction, const char* szActName, int nTransTime/* =200 */, bool bForceStopPrevAct/* =false */, bool bNoFx/* =false */, bool bResetSpeed/* =false */, bool bResetActFlag/* =false */, bool *pNewActFlag/* =NULL */, DWORD dwNewFlagMode/* =COMACT_FLAG_MODE_NONE */){
	return m_pActionController
		&& m_pActionController->QueueNonSkillActionWithName(iAction, szActName, nTransTime, bForceStopPrevAct, bNoFx, bResetSpeed, bResetActFlag, pNewActFlag, dwNewFlagMode);
}

bool CECPlayer::PlaySkillCastActionWithName(int idSkill, const char *szActName, bool bNoFX/* =false */){
	return m_pActionController
		&& m_pActionController->PlaySkillCastActionWithName(idSkill, szActName, bNoFX);
}

bool CECPlayer::PlaySkillAttackActionWithName(int idSkill, const char *szActName, bool bNoFX/* =false */, bool *pActFlag/* =NULL */, DWORD dwFlagMode/* =COMACT_FLAG_MODE_NONE */){
	return m_pActionController
		&& m_pActionController->PlaySkillAttackActionWithName(idSkill, szActName, bNoFX, pActFlag, dwFlagMode);
}

bool CECPlayer::QueueSkillAttackActionWithName(int idSkill, const char *szActName, int nTransTime/* =200 */, bool bNoFX/* =false */, bool bResetSpeed/* =false */, bool bResetActFlag/* =false */, bool *pNewActFlag/* =NULL */, DWORD dwNewFlagMode/* =COMACT_FLAG_MODE_NONE */){
	return m_pActionController
		&& m_pActionController->QueueSkillAttackActionWithName(idSkill, szActName, nTransTime, bNoFX, bResetSpeed, bResetActFlag, pNewActFlag, dwNewFlagMode);
}

bool CECPlayer::PlayWoundActionWithName(const char* szActName){
	return m_pActionController
		&& m_pActionController->PlayWoundActionWithName(szActName);
}

void CECPlayer::ClearComActFlagAllRankNodes(bool bSignalCurrent){
	if (m_pActionController){
		m_pActionController->ClearComActFlagAllRankNodes(bSignalCurrent);
	}
}

void CECPlayer::StopChannelAction(){
	if (m_pActionController){
		m_pActionController->StopChannelAction();
	}
}

void CECPlayer::StopSkillCastAction(){
	if (m_pActionController){
		m_pActionController->StopSkillCastAction();
	}
}

void CECPlayer::StopSkillAttackAction(){
	if (m_pActionController){
		m_pActionController->StopSkillAttackAction();
	}
}

bool CECPlayer::IsPlayingAction()const{
	return GetLowerBodyAction() != -1;
}

bool CECPlayer::IsPlayingAction(int iAction)const{
	return m_pActionController ? m_pActionController->IsPlayingAction(iAction) : false;
}

bool CECPlayer::IsPlayingCastingSkillAction()const{
	return m_pActionController ? m_pActionController->IsPlayingCastingSkillAction() : false;
}

bool CECPlayer::IsPlayingMoveAction()const{
	return m_pActionController ? m_pActionController->IsPlayingMoveAction() : false;
}

bool CECPlayer::IsPlayingCastingSkillAndMoveActions()const{
	return IsPlayingCastingSkillAction() && IsPlayingMoveAction();
}

int CECPlayer::GetLowerBodyAction()const{
	return m_pActionController ? m_pActionController->GetLowerBodyAction() : -1;
}

bool CECPlayer::PlayAction(int iAction, bool bRestart/* true */, int iTransTime/* 200 */, bool bQueue/* false */){
	return PlayActionWithConfig(iAction, 0, bRestart, iTransTime, bQueue);
}

bool CECPlayer::PlayActionWithConfig(int iAction, int actionConfigID, bool bRestart/* =true */, int iTransTime/* =200 */, bool bQueue/* =false */){
	if (iAction < 0 || iAction >= ACT_MAX){
		ASSERT(false);
		return false;
	}
	if (actionConfigID > 0){
		DATA_TYPE dt(DT_INVALID);
		const void *p = g_pGame->GetElementDataMan()->get_data_ptr(actionConfigID, ID_SPACE_CONFIG, dt);
		if (dt == DT_PLAYER_ACTION_INFO_CONFIG){
			PLAYER_ACTION actionConfig;
			actionConfig.type = (PLAYER_ACTION_TYPE)iAction;
			actionConfig.data = (PLAYER_ACTION_INFO_CONFIG *)p;
			return PlayActionWithConfig(iAction, actionConfig, bRestart, iTransTime, bQueue);
		}else{
			ASSERT(false);
			a_LogOutput(1, "CECPlayer::PlayActionWithConfig, invalid action config ID(%d)", actionConfigID);
		}
	}
	return PlayActionWithConfig(iAction, m_PlayerActions[iAction], bRestart, iTransTime, bQueue);
}

//	Play model action by weapon and relative action index
bool CECPlayer::PlayActionWithConfig(int iAction, const PLAYER_ACTION &actionConfig, bool bRestart/* true */, int iTransTime/* 200 */, bool bQueue/* false */)
{
	if (m_pPlayerModel)
	{
		if (iAction != ACT_WOUNDED)
		{
			if (iAction == ACT_TAKEOFF){
				ShowWing(true);
			}else if (iAction == ACT_LANDON){
				ShowWing(false);
			}

			PLAYER_ACTION action = actionConfig;
			if( !action.data )
				return false;

			if( !bQueue )
			{
				// update show weapon states.
				m_bShowWeapon = action.data->hide_weapon ? false : true;
			}
			
			char szAct[512];
			int weapon_type = GetShowingWeaponType();
			if( iAction >= ACT_ATTACK_1 && iAction <= ACT_ATTACK_4 )
			{
				// attack action should not be played by this function
				return false;
			}
			else if( iAction == ACT_ATTACK_TOSS )
			{
				// we use toss second part action as a common action 
				sprintf(szAct, "%s_%s落", action.data->action_prefix, action.data->action_weapon_suffix[weapon_type].suffix);
				if( !bQueue ){
					PlayNonSkillActionWithName(iAction, szAct, bRestart, iTransTime);
				}else{
					QueueNonSkillActionWithName(iAction, szAct, iTransTime, bRestart, false, false, true, NULL, COMACT_FLAG_MODE_ONCE_MULTIIGNOREGFX);
				}
			}
			else if( (iAction >= ACT_EXP_WAVE && iAction <= ACT_EXP_KISS_END) || (iAction >= ACT_EXP_FIGHT && iAction <= ACT_EXP_DANCE) )
			{
				// 表情动作
				AString strFashionAction;
				if (iAction == ACT_EXP_DANCE && InFashionMode())
					strFashionAction = GetFashionActionName();
				sprintf(szAct, "%s_%s"
					, strFashionAction.IsEmpty() ? action.data->action_prefix : strFashionAction
					, action.data->action_weapon_suffix[weapon_type].suffix);
				if( !bQueue ){
					PlayNonSkillActionWithName(iAction, szAct, bRestart, iTransTime);
				}else{
					QueueNonSkillActionWithName(iAction, szAct, iTransTime, false, false, false, true, NULL, COMACT_FLAG_MODE_ONCE_MULTIIGNOREGFX);
				}
			}
			else if (iAction == ACT_EXP_FASHIONWEAPON)
			{
				if (CanShowFashionWeapon(m_uAttackType, m_iFashionWeaponType))
				{
					AString strAction;
					int idEquipment = m_aEquips[EQUIPIVTR_FASHION_WEAPON] & 0x0000ffff;
					GetFashionActionNameByID(idEquipment,strAction);
					if (!strAction.IsEmpty())
					{
						sprintf(szAct, "%s_%s", strAction, action.data->action_weapon_suffix[weapon_type].suffix);
						if( !bQueue ){
							PlayNonSkillActionWithName(iAction, szAct, bRestart, iTransTime);
						}else{
							QueueNonSkillActionWithName(iAction, szAct, iTransTime, false, false, false, true, NULL, COMACT_FLAG_MODE_ONCE_MULTIIGNOREGFX);
						}
					}
				}				
			}
			else
			{
				// 通用动作
				if (m_pPetModel)
				{
					if( iAction == ACT_FIGHTSTAND )
						action = m_PlayerActions[ACT_STAND];
					else if( iAction >= ACT_PICKUP && iAction <= ACT_PICKUP_MATTER )
						action = m_PlayerActions[ACT_STAND];

					if( iAction == ACT_TWO_KISS )
						sprintf(szAct, "站立_亲吻_双骑_通用");
					else
						sprintf(szAct, "%s_%s", action.data->action_prefix, "骑乘_通用");

					const char * szPetAct = NULL;
					if( !bQueue )
						m_pPetModel->PlayActionByName(szPetAct, 1.0f, bRestart, iTransTime, true);
					else
						m_pPetModel->QueueAction(szPetAct, iTransTime);
					// now weapon wanted.
					m_bShowWeapon = false;
				}
				else if (m_bHangerOn)
				{
					if( iAction == ACT_FIGHTSTAND )
						action = m_PlayerActions[ACT_STAND];
					else if( iAction >= ACT_PICKUP && iAction <= ACT_PICKUP_MATTER )
						action = m_PlayerActions[ACT_STAND];

					if( iAction == ACT_TWO_KISS )
					{
						if (m_AttachMode == enumAttachRideOnPlayer)
							sprintf(szAct, "抚摸_绑定_通用");
						else if (m_AttachMode == enumAttachHugPlayer)
							sprintf(szAct, "亲吻_绑定_通用");
						else if (m_AttachMode == enumAttachRideOnPet)
							sprintf(szAct, "亲吻_双骑_通用");
					}
					else
					{
						if (m_AttachMode == enumAttachRideOnPlayer)
							sprintf(szAct, "%s_%s", action.data->action_prefix, "双骑_通用");
						else if (m_AttachMode == enumAttachHugPlayer)
						{
							if( iAction != ACT_RUN )
								sprintf(szAct, "%s", "站立_绑定_通用");
							else
								sprintf(szAct, "%s", "奔跑_绑定_通用");
						}
						else if (m_AttachMode == enumAttachRideOnPet)
							sprintf(szAct, "%s_%s", action.data->action_prefix, "双骑_通用");
					}
					// now weapon wanted.
					m_bShowWeapon = false;
				}
				else if (m_AttachMode == enumAttachHugPlayer || m_AttachMode == enumAttachRideOnPlayer)
				{
					if( iAction == ACT_FIGHTSTAND )
						action = m_PlayerActions[ACT_STAND];
					else if( iAction >= ACT_PICKUP && iAction <= ACT_PICKUP_MATTER )
						action = m_PlayerActions[ACT_STAND];

					if( iAction == ACT_TWO_KISS )
					{
						action = m_PlayerActions[GetMoveStandAction(false, false)];
						if (m_AttachMode == enumAttachHugPlayer)
							sprintf(szAct, "%s_%s", action.data->action_prefix, "亲吻_绑定_通用");
						else
							sprintf(szAct, "%s_%s", action.data->action_prefix, "抚摸_绑定_通用");
					}
					else
						sprintf(szAct, "%s_%s", action.data->action_prefix, "绑定_通用");
					// now weapon wanted.
					m_bShowWeapon = false;
				}
				else
					sprintf(szAct, "%s_%s", action.data->action_prefix, action.data->action_weapon_suffix[weapon_type].suffix);


				CECModel* pRightHandWeapon = GetRightHandWeapon();
				if( !bQueue )
				{
					PlayNonSkillActionWithName(iAction, szAct, bRestart, iTransTime);

					if(pRightHandWeapon && IsUsingMagicWeapon())
						pRightHandWeapon->PlayActionByName(_GenWeaponActionName(szAct, m_iGender), 1.0f, bRestart, iTransTime, true, iAction);
				}
				else
				{
					QueueNonSkillActionWithName(iAction, szAct, iTransTime, bRestart, false, false, true, NULL, COMACT_FLAG_MODE_ONCE_MULTIIGNOREGFX);
					
					if(pRightHandWeapon && IsUsingMagicWeapon())
						pRightHandWeapon->QueueAction(_GenWeaponActionName(szAct, m_iGender), iTransTime, iAction, bRestart ? true : false);
				}
			}
		}
		else	//	iAction == ACT_WOUNDED
		{
			PLAYER_ACTION action = m_PlayerActions[iAction];

			PlayWoundActionWithName(action.data->action_name);

			if (m_pPetModel)
			{
			}
		}

		if( !bQueue )
		{
			if( !m_bShowWeapon )
				ShowWeapon(false);
			else
			{
				// 巫师跳跃时需要隐藏法宝
				if(IsUsingMagicWeapon() && (iAction == ACT_JUMP_START || iAction == ACT_JUMP_LAND || iAction == ACT_JUMP_LOOP))
					ShowWeapon(false);
				else
					ShowWeapon(true);
			}
			
			//	根据播放动作，更改武器悬挂位置
			UpdateWeaponHangerPosByAction(iAction);
		}
		return true;
	}
	else
		return false;
}

// Play skill cast action 
bool CECPlayer::PlaySkillCastAction(int idSkill)
{
	return true;
}

//	Set part extend properties
void CECPlayer::SetPartExtendProps(int iPropIdx, void* pData)
{
	switch (iPropIdx)
	{
	case EXTPROPIDX_BASE:
	
		m_ExtProps.bs = *(ROLEEXTPROP_BASE*)pData;
		break;
	
	case EXTPROPIDX_MOVE:
	
		m_ExtProps.mv = *(ROLEEXTPROP_MOVE*)pData;
		break;
	
	case EXTPROPIDX_ATTACK:
	
		m_ExtProps.ak = *(ROLEEXTPROP_ATK*)pData;
		break;
	
	case EXTPROPIDX_DEF:

		m_ExtProps.df = *(ROLEEXTPROP_DEF*)pData;
		break;
	
	default:
		ASSERT(0);
		return;
	}
}

/*
 *	Add By Zhangyu, 12.22.04
 */

void CECPlayer::ShowEquipments(
	int nProf,
	int nGender,
	const int* pEquipmentID,
	__int64 ChangeMask,
	EquipsLoadResult* pResult,
	bool bSimpleModel)
{
	DATA_TYPE	dt;
	A3DCOLOR	color;

	// keep old things.
	pResult->stoneWeapon	= 0x00ff;
	pResult->stoneUpperBody = 0x00ff;
	pResult->stoneWrist		= 0x00ff;
	pResult->stoneLowerBody = 0x00ff;
	pResult->stoneFoot		= 0x00ff;
	
	for (__int64 i = 0; i < SIZE_ALL_EQUIPIVTR; i++)
	{
		if (((1 << i) & ChangeMask))
		{
			int idEquipment = pEquipmentID[i];
			if((i >= EQUIPIVTR_FASHION_BODY && i <= EQUIPIVTR_FASHION_WRIST ) ||
				i == EQUIPIVTR_FASHION_HEAD || i == EQUIPIVTR_FASHION_WEAPON)
			{
				WORD wColor = (idEquipment & 0xffff0000) >> 16;
				idEquipment &= 0x0000ffff;
				color = FASHION_WORDCOLOR_TO_A3DCOLOR(wColor);
			}
			else
			{
				if( i == EQUIPIVTR_WEAPON )
				{
					pResult->stoneWeapon = (idEquipment & 0xffff0000) >> 16;
				}
				if( i == EQUIPIVTR_BODY )
				{
					pResult->stoneUpperBody = (idEquipment & 0xffff0000) >> 16;
				}
				if( i == EQUIPIVTR_WRIST )
				{
					pResult->stoneWrist = (idEquipment & 0xffff0000) >> 16;
				}
				if( i == EQUIPIVTR_LEG )
				{
					pResult->stoneLowerBody = (idEquipment & 0xffff0000) >> 16;
				}
				if( i == EQUIPIVTR_FOOT )
				{
					pResult->stoneFoot = (idEquipment & 0xffff0000) >> 16;
				}
				idEquipment &= 0x0000ffff;
			}

			if (idEquipment)
			{
				const void* pEquip = g_pGame->GetElementDataMan()->get_data_ptr(
					idEquipment,
					ID_SPACE_ESSENCE,
					dt);

				if (pEquip)
				{
					switch (dt)
					{
					case DT_WEAPON_ESSENCE:
						ChangeWeapon(pResult, static_cast<const WEAPON_ESSENCE*>(pEquip));
						continue;
					case DT_ARMOR_ESSENCE:
						if( !bSimpleModel )
						{
							ChangeArmor(
								static_cast<const ARMOR_ESSENCE*>(pEquip),
								pResult->dwShowMask,
								pResult->dwSkinChangeMask,
								nProf,
								nGender,
								pResult->aSkins);
						}
						continue;
					case DT_WINGMANWING_ESSENCE:
						pResult->wingType = WINGTYPE_WING;
						ChangeWing(pResult, static_cast<const WINGMANWING_ESSENCE*>(pEquip)->file_model);
						continue;
					case DT_FLYSWORD_ESSENCE:
						pResult->wingType = FlyMode2WingType(static_cast<const FLYSWORD_ESSENCE*>(pEquip)->fly_mode);
						if (pResult->wingType != WINGTYPE_DOUBLEWHEEL) {
							ChangeWing(pResult, static_cast<const FLYSWORD_ESSENCE*>(pEquip)->file_model);
						} else {
							ChangeWing(pResult, static_cast<const FLYSWORD_ESSENCE*>(pEquip)->file_model, 
								static_cast<const FLYSWORD_ESSENCE*>(pEquip)->file_model2);
						}		
						continue;
					case DT_FASHION_ESSENCE:
						{
							const FASHION_ESSENCE* pFashion = static_cast<const FASHION_ESSENCE*>(pEquip);
							if (pFashion->equip_location == enumSkinShowHand)
							{
								pResult->uFashionAttackType = pFashion->action_type;
								pResult->bFashionWeaponChanged = true;
								pResult->strLeftFashionWeapon = pFashion->file_model_left;
								pResult->strRightFashionWeapon = pFashion->file_model_right;
								ChangeWeapon(pResult, pFashion->file_model_left, pFashion->file_model_right);
							}
							else
							{
								if( !bSimpleModel )
								{
									ChangeFashion(
										pFashion,
										pResult->dwFashionShowMask,
										pResult->dwSkinChangeMask,
										nProf,
										nGender,
										pResult->aSkins,
										color
										);
								}
							}

							continue;
						}
					default:
						continue;
					}
				}
				else
					a_LogOutput(1, "Equipment Invent: WrongEquipType: Id = %d", idEquipment);
			}

			if( bSimpleModel )	// 简单模型不加载默认装备
				continue;

			if (i == EQUIPIVTR_BODY)
			{
				pResult->dwShowMask |= (1 << enumSkinShowUpperBody);
				pResult->dwSkinChangeMask |= (1 << SKIN_UPPER_BODY_INDEX);
				pResult->stoneUpperBody = 0;
				ChangeDefaultUpper(pResult->aSkins[SKIN_UPPER_BODY_INDEX], nProf, nGender);
			}
			else if (i == EQUIPIVTR_LEG)
			{
				pResult->dwShowMask |= (1 << enumSkinShowLowerBody);
				pResult->dwSkinChangeMask |= (1 << SKIN_LOWER_INDEX);
				pResult->stoneLowerBody = 0;
				ChangeDefaultLower(pResult->aSkins[SKIN_LOWER_INDEX], nProf, nGender);
			}
			else if (i == EQUIPIVTR_WRIST)
			{
				pResult->dwShowMask |= (1 << enumSkinShowWrist);
				pResult->dwSkinChangeMask |= (1 << SKIN_WRIST_INDEX);
				pResult->stoneWrist = 0;
				ChangeDefaultWrist(pResult->aSkins[SKIN_WRIST_INDEX], nProf, nGender);
			}
			else if (i == EQUIPIVTR_FOOT)
			{
				pResult->dwShowMask |= (1 << enumSkinShowFoot);
				pResult->dwSkinChangeMask |= (1 << SKIN_FOOT_INDEX);
				pResult->stoneFoot = 0;
				ChangeDefaultFoot(pResult->aSkins[SKIN_FOOT_INDEX], nProf, nGender);
			}
			else if (i == EQUIPIVTR_WEAPON)
			{
				pResult->bWeaponChanged = true;	
				pResult->stoneWeapon = 0;
				pResult->uAttackType = DEFAULT_ACTION_TYPE;
			}
			else if (i == EQUIPIVTR_FLYSWORD)
				pResult->bWingChanged = true;
			else if (i == EQUIPIVTR_FASHION_BODY)
			{
				pResult->dwFashionShowMask |= (1 << enumSkinShowUpperBody);
				pResult->dwSkinChangeMask |= (1 << SKIN_FASHION_UPPER_BODY_INDEX);
				ChangeDefaultFashionUpper(pResult->aSkins[SKIN_FASHION_UPPER_BODY_INDEX], nProf, nGender);
			}
			else if (i == EQUIPIVTR_FASHION_LEG)
			{
				pResult->dwFashionShowMask |= (1 << enumSkinShowLowerBody);
				pResult->dwSkinChangeMask |= (1 << SKIN_FASHION_LOWER_INDEX);
				ChangeDefaultFashionLower(pResult->aSkins[SKIN_FASHION_LOWER_INDEX], nProf, nGender);
			}
			else if (i == EQUIPIVTR_FASHION_WRIST)
			{
				pResult->dwFashionShowMask |= (1 << enumSkinShowWrist);
				pResult->dwSkinChangeMask |= (1 << SKIN_FASHION_WRIST_INDEX);
				ChangeDefaultFashionWrist(pResult->aSkins[SKIN_FASHION_WRIST_INDEX], nProf, nGender);
			}
			else if (i == EQUIPIVTR_FASHION_FOOT)
			{
				pResult->dwFashionShowMask |= (1 << enumSkinShowFoot);
				pResult->dwSkinChangeMask |= (1 << SKIN_FASHION_FOOT_INDEX);
				ChangeDefaultFashionFoot(pResult->aSkins[SKIN_FASHION_FOOT_INDEX], nProf, nGender);
			}
			else if (i == EQUIPIVTR_FASHION_HEAD)
			{
				int aaa = 1; // for test
			}
			else if (i == EQUIPIVTR_FASHION_WEAPON)
			{
				pResult->bFashionWeaponChanged = true;
				pResult->uFashionAttackType = DEFAULT_ACTION_TYPE;
			}
		}
	}
}

void CECPlayer::ShowEquipments(const int* pEquipmentID, bool bLoadAtOnce, bool bForceLoad)
{
	if( !GetMajorModel() || !GetMajorModel()->GetA3DSkinModel())
		return;

	__int64 Mask = 0;

	for (__int64 i = 0; i < SIZE_ALL_EQUIPIVTR; i++)
	{
		if( bForceLoad )
		{
			if( !((1 << i) & m_i64EquipDisabled) || pEquipmentID[i] == 0 )
				Mask |= (1 << i);
		}
		else
		{
			if ( pEquipmentID[i] != m_aEquips[i] )
			{
				if (IsShownEquip(i))
				{
					if (m_aEquips[i] > 0 )
					{
						DWORD dwRealID = GetRealElementID(i, m_aEquips[i]);
						GetMajorModel()->OnScriptChangeEquip(dwRealID, 0, false);
					}

					if (pEquipmentID[i] > 0)
					{
						DWORD dwRealID = GetRealElementID(i, pEquipmentID[i]);
						GetMajorModel()->OnScriptChangeEquip(dwRealID, 1, false);
					}
				}
				
				if( // not disable
					!((1 << i) & m_i64EquipDisabled)
					// remove equip  
					|| pEquipmentID[i] == 0 )
				{
					Mask |= (1 << i);
				}

				m_aEquips[i] = pEquipmentID[i];
			}
		}
	}
	int aEquips[IVTRSIZE_EQUIPPACK];
	memcpy(aEquips, pEquipmentID, sizeof(aEquips));
	DecideWeaponLoad(aEquips, Mask);

	QueueLoadEquips(aEquips, Mask, bLoadAtOnce);
}

void CECPlayer::ChangeEquipDisableMask(__int64 Mask)
{
	// equipment disable will affect the player animation

	int aChangedEquip[SIZE_ALL_EQUIPIVTR];
	int aRestoreEquip[SIZE_ALL_EQUIPIVTR];

	// backup the equipments info
	ASSERT(sizeof(aRestoreEquip) == sizeof(m_aEquips));
	memcpy(aRestoreEquip, m_aEquips, sizeof(m_aEquips));

	for (__int64 i = 0; i < SIZE_ALL_EQUIPIVTR; i++)
	{
		__int64 flagOld = (1 << i) & m_i64EquipDisabled;
		__int64 flagCur = (1 << i) & Mask;
		
		if(flagCur != flagOld)
		{
			if(flagCur)
			{
				// this equipment should be disabled
				aChangedEquip[i] = 0;
			}
			else
			{
				// this equipment should be restored
				aChangedEquip[i] = m_aEquips[i];
				m_aEquips[i] = 0;
			}
		}
		else
		{
			aChangedEquip[i] = m_aEquips[i];
		}
	}
	m_i64EquipDisabled = Mask;

	// remove disabled equipments
	ShowEquipments(aChangedEquip, false);
	// restore the equipments info
	memcpy(m_aEquips, aRestoreEquip, sizeof(m_aEquips));
}

bool CECPlayer::ChangeWing(
	EquipsLoadResult* pResult,
	const char* szModeFile,
	const char* szModeFile2)
{
	if (!szModeFile[0]) return false;

	pResult->bWingChanged = true;

	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile((const char*)glb_ArmorReplaceShader_ReflectPrefix, A3DSkinMan::SHADERREPLACE_USERDEFINE);

	CECModel* pWing = new CECModel;

	if (pWing->Load(szModeFile, true, A3DSkinModel::LSF_UNIQUESKIN))
	{
		pWing->Show(false);
		pResult->pWing = pWing;
	}
	else
		delete pWing;

	if (szModeFile2 != NULL) {
		CECModel* pRightFoot = new CECModel;
		
		if (pRightFoot->Load(szModeFile2, true, A3DSkinModel::LSF_UNIQUESKIN))
		{
			pRightFoot->Show(false);
			pResult->pWing2 = pRightFoot;
		}
		else
			delete pRightFoot;
	}

	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
	
	return true;
}

inline void _SetWeaponMaterial(A3DSkinModel* pModel)
{
	if (!pModel) return;

	int nNumSkin = pModel->GetSkinNum();
	A3DSkin * pSkin;

	for (int i = 0; i < nNumSkin; i++)
	{
		pSkin = pModel->GetA3DSkin(i);

		if( pSkin )
		{
			int nNumMaterial = pSkin->GetMaterialNum();

			for (int i = 0; i < nNumMaterial; i++)
			{
				A3DMATERIALPARAM param = pSkin->GetMaterial(i)->GetMaterialParam();
				param.Specular = WEAPON_SPECULAR;
				param.Power = WEAPON_POWER;
				pSkin->GetMaterial(i)->SetMaterialParam(param);
			}
		}
	}
}

bool CECPlayer::ChangeWeapon(EquipsLoadResult* pResult, const char* szLeft, const char* szRight)
{
	if (pResult->bFashionWeaponChanged)
		g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile((const char*)glb_FashionReplaceShader, A3DSkinMan::SHADERREPLACE_USERDEFINE);
	else
		g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile((const char*)glb_ArmorReplaceShader_ReflectPrefix, A3DSkinMan::SHADERREPLACE_USERDEFINE);

	if (szLeft[0])
	{
		CECModel* pWeapon = new CECModel;

		if (pWeapon->Load(szLeft, true, A3DSkinModel::LSF_UNIQUESKIN))
		{
			_SetWeaponMaterial(pWeapon->GetA3DSkinModel());
			pResult->pLeftHandWeapon = pWeapon;
		}
		else
			delete pWeapon;
	}

	if (szRight[0])
	{
		CECModel* pWeapon = new CECModel;

		if (pWeapon->Load(szRight, true, A3DSkinModel::LSF_UNIQUESKIN))
		{
			_SetWeaponMaterial(pWeapon->GetA3DSkinModel());
			pResult->pRightHandWeapon = pWeapon;
		}
		else
			delete pWeapon;
	}

	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
	return true;
}

void CECPlayer::ChangeWeapon(EquipsLoadResult* pResult, const WEAPON_ESSENCE* pWeapon)
{
	DATA_TYPE dt;

	const WEAPON_SUB_TYPE* pSubType =
		(const WEAPON_SUB_TYPE*) g_pGame->GetElementDataMan()->get_data_ptr(
			pWeapon->id_sub_type,
			ID_SPACE_ESSENCE,
			dt);

	if (pSubType == NULL || dt != DT_WEAPON_SUB_TYPE)
	{
		a_LogOutput(1, "Equipment Invent: Wrong Weapon SubType, Essence Id = %d", pWeapon->id);
		return;
	}
	
	pResult->uAttackType = pSubType->action_type;
	pResult->strLeftWeapon = pWeapon->file_model_left;
	pResult->strRightWeapon = pWeapon->file_model_right;
	pResult->bWeaponChanged = true;	

	ChangeWeapon(
		pResult,
		pWeapon->file_model_left,
		pWeapon->file_model_right);
}

void CECPlayer::ChangeArmor(
	const ARMOR_ESSENCE* pArmor,
	DWORD& dwShowMask,
	DWORD& dwSkinChangeMask,
	int nProf,
	int nGender,
	A3DSkin* pSkins[][3]
	)
{
	static const int _location_skin_map[] =
	{
		0,							// none
		SKIN_UPPER_BODY_INDEX,		// enumSkinShowUpperBody
		SKIN_WRIST_INDEX,			// enumSkinShowWrist
		SKIN_LOWER_INDEX,			// enumSkinShowLowerBody
		SKIN_FOOT_INDEX,			// enumSkinShowFoot
		SKIN_UPPER_BODY_INDEX,		// enumSkinShowUpperAndLower
		SKIN_UPPER_BODY_INDEX,		// enumSkinShowUpperAndWrist
		SKIN_LOWER_INDEX,			// enumSkinShowLowerAndFoot
		SKIN_UPPER_BODY_INDEX		// enumSkinShowUpperLowerAndWrist
	};

	int nLocation = pArmor->equip_location;
	if (nLocation < enumSkinShowUpperBody || nLocation > enumSkinShowUpperLowerAndWrist)
		return;

	char szPath[MAX_PATH];
	_GenSkinPath(szPath, nProf, nGender, pArmor->realname);

	int nSkinIndex = _location_skin_map[nLocation];
	dwShowMask |= (1 << nLocation);
	dwSkinChangeMask |= (1 << nSkinIndex);

	ChangeArmor(
		szPath,
		pSkins[nSkinIndex],
		nLocation,
		false,
		nProf,
		nGender
		);
}

bool CECPlayer::ChangeArmor(
	const char* strSkinFile,
	A3DSkin* aSkins[3],
	int nLocation,
	bool bDefault,
	int nProf,
	int nGender)
{
	switch (nLocation)
	{
	case enumSkinShowUpperBody:

		if (!LoadPlayerSkin(aSkins, SKIN_UPPER_BODY_INDEX, strSkinFile))
		{
			a_LogOutput(1, "Equipment Invent: Wrong Skin: %s", strSkinFile);
			if (!bDefault) ChangeDefaultUpper(aSkins, nProf, nGender);
		}

		break;

	case enumSkinShowWrist:

		if (!LoadPlayerSkin(aSkins, SKIN_WRIST_INDEX, strSkinFile))
		{
			a_LogOutput(1, "Equipment Invent: Wrong Skin: %s", strSkinFile);
			if (!bDefault) ChangeDefaultWrist(aSkins, nProf, nGender);
		}

		break;

	case enumSkinShowLowerBody:

		if (!LoadPlayerSkin(aSkins, SKIN_LOWER_INDEX, strSkinFile))
		{
			a_LogOutput(1, "Equipment Invent: Wrong Skin: %s", strSkinFile);
			if (!bDefault) ChangeDefaultLower(aSkins, nProf, nGender);
		}

		break;

	case enumSkinShowFoot:

		if (!LoadPlayerSkin(aSkins, SKIN_FOOT_INDEX, strSkinFile))
		{
			a_LogOutput(1, "Equipment Invent: Wrong Skin: %s", strSkinFile);
			if (!bDefault) ChangeDefaultFoot(aSkins, nProf, nGender);
		}

		break;

	case enumSkinShowUpperAndLower:

		if (!LoadPlayerSkin(aSkins, SKIN_UPPER_BODY_INDEX, strSkinFile))
		{
			a_LogOutput(1, "Equipment Invent: Wrong Skin: %s", strSkinFile);
			if (!bDefault) ChangeDefaultUpper(aSkins, nProf, nGender);
		}

		break;

	case enumSkinShowUpperAndWrist:

		if (!LoadPlayerSkin(aSkins, SKIN_UPPER_BODY_INDEX, strSkinFile))
		{
			a_LogOutput(1, "Equipment Invent: Wrong Skin: %s", strSkinFile);
			if (!bDefault) ChangeDefaultUpper(aSkins, nProf, nGender);
		}

		break;

	case enumSkinShowLowerAndFoot:

		if (!LoadPlayerSkin(aSkins, SKIN_LOWER_INDEX, strSkinFile))
		{
			a_LogOutput(1, "Equipment Invent: Wrong Skin: %s", strSkinFile);
			if (!bDefault) ChangeDefaultLower(aSkins, nProf, nGender);
		}

		break;

	case enumSkinShowUpperLowerAndWrist:

		if (!LoadPlayerSkin(aSkins, SKIN_UPPER_BODY_INDEX, strSkinFile))
		{
			a_LogOutput(1, "Equipment Invent: Wrong Skin: %s", strSkinFile);
			if (!bDefault) ChangeDefaultUpper(aSkins, nProf, nGender);
		}

		break;

	case enumSkinShowNone:

		break;

	default:
		a_LogOutput(1, "Equipment Invent: Wrong Location: %d", nLocation);
		break;
	}

	return true;
}

void CECPlayer::ChangeFashion(
	const FASHION_ESSENCE* pFashion,
	DWORD& dwShowMask,
	DWORD& dwSkinChangeMask,
	int nProf,
	int nGender,
	A3DSkin* pSkins[][3],
	A3DCOLOR color)
{
	static const int _location_skin_map[] =
	{
		0,									// none
		SKIN_FASHION_UPPER_BODY_INDEX,		// enumSkinShowUpperBody
		SKIN_FASHION_WRIST_INDEX,			// enumSkinShowWrist
		SKIN_FASHION_LOWER_INDEX,			// enumSkinShowLowerBody
		SKIN_FASHION_FOOT_INDEX,			// enumSkinShowFoot
		SKIN_FASHION_UPPER_BODY_INDEX,		// enumSkinShowUpperAndLower
		SKIN_FASHION_UPPER_BODY_INDEX,		// enumSkinShowUpperAndWrist
		SKIN_FASHION_LOWER_INDEX,			// enumSkinShowLowerAndFoot
		SKIN_FASHION_UPPER_BODY_INDEX,		// enumSkinShowUpperLowerAndWrist
	};

	int nLocation = pFashion->equip_location;
	if (nLocation < enumSkinShowUpperBody || nLocation > enumSkinShowUpperLowerAndWrist)
		return;

	char szPath[MAX_PATH];
	_GenSkinPath(szPath, nProf, nGender, pFashion->realname);

	int nSkinIndex = _location_skin_map[nLocation];
	dwShowMask |= (1 << nLocation);
	dwSkinChangeMask |= (1 << nSkinIndex);

	ChangeFashion(
		szPath,
		pSkins[nSkinIndex],
		nLocation,
		false,
		nProf,
		nGender,
		color
		);
}

bool CECPlayer::ChangeFashion(
	const char* strSkinFile,
	A3DSkin* aSkins[3],
	int nLocation,
	bool bDefault,
	int nProf,
	int nGender,
	A3DCOLOR color)
{
	switch (nLocation)
	{
	case enumSkinShowUpperBody:

		if (!LoadPlayerSkin(aSkins, SKIN_FASHION_UPPER_BODY_INDEX, strSkinFile))
		{
			a_LogOutput(1, "Equipment Invent: Wrong Skin: %s", strSkinFile);
			if (!bDefault) ChangeDefaultFashionUpper(aSkins, nProf, nGender);
		}

		break;

	case enumSkinShowWrist:

		if (!LoadPlayerSkin(aSkins, SKIN_FASHION_WRIST_INDEX, strSkinFile))
		{
			a_LogOutput(1, "Equipment Invent: Wrong Skin: %s", strSkinFile);
			if (!bDefault) ChangeDefaultFashionWrist(aSkins, nProf, nGender);
		}

		break;

	case enumSkinShowLowerBody:

		if (!LoadPlayerSkin(aSkins, SKIN_FASHION_LOWER_INDEX, strSkinFile))
		{
			a_LogOutput(1, "Equipment Invent: Wrong Skin: %s", strSkinFile);
			if (!bDefault) ChangeDefaultFashionLower(aSkins, nProf, nGender);
		}

		break;

	case enumSkinShowFoot:

		if (!LoadPlayerSkin(aSkins, SKIN_FASHION_FOOT_INDEX, strSkinFile))
		{
			a_LogOutput(1, "Equipment Invent: Wrong Skin: %s", strSkinFile);
			if (!bDefault) ChangeDefaultFashionFoot(aSkins, nProf, nGender);
		}

		break;

	case enumSkinShowUpperAndLower:

		if (!LoadPlayerSkin(aSkins, SKIN_FASHION_UPPER_BODY_INDEX, strSkinFile))
		{
			a_LogOutput(1, "Equipment Invent: Wrong Skin: %s", strSkinFile);
			if (!bDefault) ChangeDefaultFashionUpper(aSkins, nProf, nGender);
		}

		break;

	case enumSkinShowUpperAndWrist:

		if (!LoadPlayerSkin(aSkins, SKIN_FASHION_UPPER_BODY_INDEX, strSkinFile))
		{
			a_LogOutput(1, "Equipment Invent: Wrong Skin: %s", strSkinFile);
			if (!bDefault) ChangeDefaultFashionUpper(aSkins, nProf, nGender);
		}

		break;

	case enumSkinShowLowerAndFoot:

		if (!LoadPlayerSkin(aSkins, SKIN_FASHION_LOWER_INDEX, strSkinFile))
		{
			a_LogOutput(1, "Equipment Invent: Wrong Skin: %s", strSkinFile);
			if (!bDefault) ChangeDefaultFashionLower(aSkins, nProf, nGender);
		}

		break;

	case enumSkinShowUpperLowerAndWrist:

		if (!LoadPlayerSkin(aSkins, SKIN_FASHION_UPPER_BODY_INDEX, strSkinFile))
		{
			a_LogOutput(1, "Equipment Invent: Wrong Skin: %s", strSkinFile);
			if (!bDefault) ChangeDefaultFashionUpper(aSkins, nProf, nGender);
		}

		break;

	case enumSkinShowNone:

		break;

	default:
		a_LogOutput(1, "Equipment Invent: Wrong Location: %d", nLocation);
		break;
	}

	// now get the shader and set color to it.
	for(int i=0; i<3; i++)
	{
		A3DSkin * pSkin = aSkins[i];
		if (!pSkin) continue;

		for(int idTex=0; idTex<pSkin->GetTextureNum(); idTex ++)
		{
			A3DTexture * pTex = pSkin->GetTexture(idTex);
			if( pTex && pTex->IsShaderTexture() )
			{
				A3DShader * pShader = (A3DShader *) pTex;
				pShader->GetGeneralProps().dwTFactor = color;
			}
		}
	}

	return true;
}

// play attack action
bool CECPlayer::PlayAttackAction(int nAttackSpeed, int* piAttackTime/* =NULL */, bool *pActFlag/* =NULL */)
{
	if (!m_pPlayerModel) 
		return false;

	int nRand = rand() % 4;
	char szAct[512];
	szAct[0] = '\0';

	int weapon_type = GetShowingWeaponType();

	int nTime1, nTime2, iAction = ACT_ATTACK_1 + nRand;
	const PLAYER_ACTION& action = m_PlayerActions[iAction];

	if( !action.data || !action.data->action_prefix[0] )
		return false;

	ShowWeaponByConfig(action.data);

	CECModel* pRightHandWeapon = GetRightHandWeapon();
	bool bHideFX = false;
	
	if( GetMoveEnv() == MOVEENV_GROUND )
	{
		sprintf(szAct, "%s_%s起", action.data->action_prefix, action.data->action_weapon_suffix[weapon_type].suffix);
		PlayNonSkillActionWithName(iAction, szAct, true, 200, bHideFX, pActFlag, COMACT_FLAG_MODE_ONCE_MULTIIGNOREGFX);

		if(pRightHandWeapon && IsUsingMagicWeapon())
			pRightHandWeapon->PlayActionByName(_GenWeaponActionName(szAct, m_iGender), 1.0f, true, 200, true, iAction, bHideFX);

		nTime1 = m_pPlayerModel->GetComActTimeSpanByName(szAct);

		sprintf(szAct, "%s_%s落", action.data->action_prefix, action.data->action_weapon_suffix[weapon_type].suffix);
		QueueNonSkillActionWithName(iAction, szAct, 0, false, bHideFX);

		if(pRightHandWeapon && IsUsingMagicWeapon())
			pRightHandWeapon->QueueAction(_GenWeaponActionName(szAct, m_iGender), 0, iAction, false, false, bHideFX);

		nTime2 = m_pPlayerModel->GetComActTimeSpanByName(szAct);
	}
	else
	{
		const char *szActionMiddleName = NULL;
		if( (/*UsingWing()*/m_wingType == WINGTYPE_WING && IsFlying()) || (GetProfession() == PROF_ANGEL) || (GetProfession() == PROF_ARCHOR)  || (GetProfession() == PROF_MONK) || (GetProfession() == PROF_GHOST) )
		{
			szActionMiddleName = "空中翅膀";
		}else{
			szActionMiddleName = "空中飞剑";
		}
		sprintf(szAct, "%s_%s_%s起", action.data->action_prefix, szActionMiddleName, action.data->action_weapon_suffix[weapon_type].suffix);
		PlayNonSkillActionWithName(iAction, szAct, true, 200, bHideFX, pActFlag, COMACT_FLAG_MODE_ONCE_MULTIIGNOREGFX);
		
		if(pRightHandWeapon && IsUsingMagicWeapon())
			pRightHandWeapon->PlayActionByName(_GenWeaponActionName(szAct, m_iGender), 1.0f, true, 200, true, iAction, bHideFX);
		
		nTime1 = m_pPlayerModel->GetComActTimeSpanByName(szAct);
		
		sprintf(szAct, "%s_%s_%s落", action.data->action_prefix, szActionMiddleName, action.data->action_weapon_suffix[weapon_type].suffix);
		QueueNonSkillActionWithName(iAction, szAct, 0, false, bHideFX);
		
		if(pRightHandWeapon && IsUsingMagicWeapon())
			pRightHandWeapon->QueueAction(_GenWeaponActionName(szAct, m_iGender), 0, iAction, false, false, bHideFX);
		
		nTime2 = m_pPlayerModel->GetComActTimeSpanByName(szAct);
	}

	// make attack action end up with a stand to eliminate the still animation if a long delay occurs.
	const PLAYER_ACTION& stand_action = m_PlayerActions[ACT_FIGHTSTAND];
	sprintf(szAct, "%s_%s", stand_action.data->action_prefix, stand_action.data->action_weapon_suffix[weapon_type].suffix);
	QueueNonSkillActionWithName(ACT_FIGHTSTAND, szAct, 300, false, bHideFX, true);	//	2012-1-31: 增加设置 bResetSpeed=true，避免 ACT_FIGHTSTAND 动作受攻击动作速度修改影响(m_pPlayerModel->SetPlaySpeed)而播放过快

	if(pRightHandWeapon && IsUsingMagicWeapon())
		pRightHandWeapon->QueueAction(_GenWeaponActionName(szAct, m_iGender), 300, iAction, false, false, bHideFX, true);	//	2012-1-31: 同上

	// now we try to adjust the player's animation to meet synchronize with attack speed
	if( nAttackSpeed > 0 )
	{
		float vScale = (nTime1 + nTime2) / (float)nAttackSpeed;
		if (vScale > 0.0f){
			m_pPlayerModel->SetPlaySpeed(vScale);			
			if(pRightHandWeapon && IsUsingMagicWeapon())
				pRightHandWeapon->SetPlaySpeed(vScale);
		}
	}

	if (piAttackTime)
		*piAttackTime = nTime1 + nTime2;

	//	根据播放动作，更改武器悬挂位置
	UpdateWeaponHangerPosByAction(iAction);

	return true;
}
static void _GetSkillSectionActionName(char* szAct,int idSkill,int nSection)
{
}
// play skill attack action
bool CECPlayer::PlaySkillAttackAction(int idSkill, int nAttackSpeed, int* piAttackTime/* =NULL */,int nSection /* = 0 */, bool *pActFlag/* =NULL */)
{
	return true;
}

//	Change the color of body
void CECPlayer::SetBodyColor(A3DCOLOR color)
{
	if( GetProfession() == PROF_ORC && GetGender() == GENDER_MALE ) // 妖兽男的皮肤不调色
		m_CustomizeData.colorBody = 0xffffffff;
	else
		m_CustomizeData.colorBody = color;

	for(int i=0; i<3; i++)
	{
		if (m_pBodyShader[i])
		{
			m_pBodyShader[i]->GetGeneralProps().dwTFactor = m_CustomizeData.colorBody;
		}
	}
}

void CECPlayer::SetBodyHeadScale(unsigned char vScale)
{
	m_CustomizeData.headScale = vScale;
	UpdateBodyScales();
}

void CECPlayer::SetBodyUpScale(unsigned char vScale)
{
	m_CustomizeData.upScale = vScale;
	UpdateBodyScales();
}

void CECPlayer::SetBodyWaistScale(unsigned char vScale)
{
	m_CustomizeData.waistScale = vScale;
	UpdateBodyScales();
}

void CECPlayer::SetBodyArmWidth(unsigned char vScale)
{
	m_CustomizeData.armWidth = vScale;
	UpdateBodyScales();
}

void CECPlayer::SetBodyLegWidth(unsigned char vScale)
{
	m_CustomizeData.legWidth = vScale;
	UpdateBodyScales();
}

void CECPlayer::SetBodyBreastScale(unsigned char vScale)
{
	m_CustomizeData.breastScale = vScale;
	UpdateBodyScales();
}

void CECPlayer::UpdateBodyScales()
{
	if (!GetPlayerModel())
		return;

	A3DSkinModel * pSkinModel = GetPlayerModel()->GetA3DSkinModel();

	if( pSkinModel )
	{
		A3DVECTOR3 vecScale;
		A3DSkeleton * pSkeleton = pSkinModel->GetSkeleton();

		// head adjust
		vecScale.x = 1.0f;
		float fHeadScaleYZ = 1.0f;
		if (GetProfession() == PROF_YEYING || GetProfession() == PROF_YUEXIAN){
			//	玩家使用大头时，Neck 不随头部缩放更易接受。老职业维持原状
			fHeadScaleYZ = TransformScaleFromIntToFloat( m_CustomizeData.upScale, m_CustomizeFactor.fScaleUpFactor, SCALE_UP_FACTOR);
		}else{
			fHeadScaleYZ = TransformScaleFromIntToFloat(m_CustomizeData.headScale, m_CustomizeFactor.fScaleHeadFactor, SCALE_HEAD_FACTOR);
		}
		vecScale.y = vecScale.z = fHeadScaleYZ;

		A3DBone * pBoneNeck = pSkeleton->GetBone("Bip01 Neck", NULL);
		if( pBoneNeck )
		{
			pBoneNeck->SetScaleFactor(vecScale);
			pBoneNeck->SetScaleType(A3DBone::SCALE_LOCAL_NOOFFSET);
		}

		vecScale.x = vecScale.y = vecScale.z = TransformScaleFromIntToFloat( m_CustomizeData.headScale, m_CustomizeFactor.fScaleHeadFactor, SCALE_HEAD_FACTOR);

		A3DBone * pBoneHead = pSkeleton->GetBone("Bip01 Head", NULL);
		if( pBoneHead )
		{
			pBoneHead->SetScaleFactor(vecScale);
			pBoneHead->SetScaleType(A3DBone::SCALE_WHOLE);
		}

		if( m_iGender == GENDER_FEMALE )
		{
			// female's breast adjust
			vecScale.x = TransformScaleFromIntToFloat( m_CustomizeData.breastScale, m_CustomizeFactor.fScaleBreastFactor, SCALE_BREAST_FACTOR);
			vecScale.y = vecScale.z = TransformScaleFromIntToFloat( m_CustomizeData.breastScale,m_CustomizeFactor.fScaleBreastFactor, SCALE_BREAST_FACTOR);

			A3DBone * pBoneFBreast = pSkeleton->GetBone("Bone01", NULL);
			if( pBoneFBreast )
			{
				pBoneFBreast->SetScaleFactor(vecScale);
				pBoneFBreast->SetScaleType(A3DBone::SCALE_LOCAL);
			}
		}

		// first adjust clavicles
		vecScale.x = TransformScaleFromIntToFloat( m_CustomizeData.upScale, m_CustomizeFactor.fScaleUpFactor, SCALE_UP_FACTOR);
		vecScale.y = vecScale.z =TransformScaleFromIntToFloat( m_CustomizeData.upScale, m_CustomizeFactor.fScaleUpFactor, SCALE_UP_FACTOR);

		A3DBone * pBoneLClavicle = pSkeleton->GetBone("Bip01 L Clavicle", NULL);
		if( pBoneLClavicle )
		{
			pBoneLClavicle->SetScaleFactor(vecScale);
			pBoneLClavicle->SetScaleType(A3DBone::SCALE_LOCAL);
		}
		A3DBone * pBoneRClavicle = pSkeleton->GetBone("Bip01 R Clavicle", NULL);
		if( pBoneRClavicle )
		{
			pBoneRClavicle->SetScaleFactor(vecScale);
			pBoneRClavicle->SetScaleType(A3DBone::SCALE_LOCAL);
		}
		
		// then breast
		vecScale.x = 1.0f;
		vecScale.y = vecScale.z = TransformScaleFromIntToFloat( m_CustomizeData.upScale,m_CustomizeFactor.fScaleUpFactor, SCALE_UP_FACTOR);
		
		A3DBone * pBoneBreast = pSkeleton->GetBone("Bip01 Spine2", NULL);
		if( pBoneBreast )
		{
			pBoneBreast->SetScaleFactor(vecScale);
			pBoneBreast->SetScaleType(A3DBone::SCALE_LOCAL);
		}

		// chest
		vecScale.x = 1.0f;
		vecScale.y = vecScale.z = TransformScaleFromIntToFloat( m_CustomizeData.upScale,m_CustomizeFactor.fScaleUpFactor, SCALE_UP_FACTOR);

		A3DBone * pBoneChest = pSkeleton->GetBone("Bip01 Spine1", NULL);
		if( pBoneChest )
		{
			pBoneChest->SetScaleFactor(vecScale);
			pBoneChest->SetScaleType(A3DBone::SCALE_LOCAL);
		}

		// waist
		vecScale.x = 1.0f;
		vecScale.y = vecScale.z = TransformScaleFromIntToFloat( m_CustomizeData.waistScale,m_CustomizeFactor.fScaleWaistFactor,  SCALE_WAIST_FACTOR);

		A3DBone * pBoneWaist = pSkeleton->GetBone("Bip01 Spine", NULL);
		if( pBoneWaist )
		{
			pBoneWaist->SetScaleFactor(vecScale);
			pBoneWaist->SetScaleType(A3DBone::SCALE_LOCAL_NOOFFSET);
		}

		// pelvis
		vecScale.x = 1.0f;
		vecScale.y = vecScale.z = TransformScaleFromIntToFloat( m_CustomizeData.waistScale,m_CustomizeFactor.fScaleWaistFactor, SCALE_WAIST_FACTOR);

		A3DBone * pBonePelvis = pSkeleton->GetBone("Bip01 Pelvis", NULL);
		if( pBonePelvis )
		{
			pBonePelvis->SetScaleFactor(vecScale);
			pBonePelvis->SetScaleType(A3DBone::SCALE_LOCAL);
		}

		// arm
		vecScale.x = 1.0f;
		vecScale.y = vecScale.z = TransformScaleFromIntToFloat( m_CustomizeData.armWidth,m_CustomizeFactor.fWidthArmFactor, WIDTH_ARM_FACTOR);
		
		A3DBone * pBoneLUpperArm = pSkeleton->GetBone("Bip01 L UpperArm", NULL);
		if( pBoneLUpperArm )
		{
			pBoneLUpperArm->SetScaleFactor(vecScale);
			pBoneLUpperArm->SetScaleType(A3DBone::SCALE_LOCAL);
		}
		A3DBone * pBoneRUpperArm = pSkeleton->GetBone("Bip01 R UpperArm", NULL);
		if( pBoneRUpperArm )
		{
			pBoneRUpperArm->SetScaleFactor(vecScale);
			pBoneRUpperArm->SetScaleType(A3DBone::SCALE_LOCAL);
		}
		A3DBone * pBoneLForeArm = pSkeleton->GetBone("Bip01 L Forearm", NULL);
		if( pBoneLForeArm )
		{
			pBoneLForeArm->SetScaleFactor(vecScale);
			pBoneLForeArm->SetScaleType(A3DBone::SCALE_LOCAL);
		}
		A3DBone * pBoneRForeArm = pSkeleton->GetBone("Bip01 R Forearm", NULL);
		if( pBoneRForeArm )
		{
			pBoneRForeArm->SetScaleFactor(vecScale);
			pBoneRForeArm->SetScaleType(A3DBone::SCALE_LOCAL);
		}

		// leg
		vecScale.x = 1.0f;
		vecScale.y = vecScale.z = TransformScaleFromIntToFloat( m_CustomizeData.legWidth, m_CustomizeFactor.fWidthLegFactor, WIDTH_ARM_FACTOR);

		A3DBone * pBoneLThigh = pSkeleton->GetBone("Bip01 L Thigh", NULL);
		if( pBoneLThigh )
		{
			pBoneLThigh->SetScaleFactor(vecScale);
			pBoneLThigh->SetScaleType(A3DBone::SCALE_LOCAL);
		}
		A3DBone * pBoneRThigh = pSkeleton->GetBone("Bip01 R Thigh", NULL);
		if( pBoneRThigh )
		{
			pBoneRThigh->SetScaleFactor(vecScale);
			pBoneRThigh->SetScaleType(A3DBone::SCALE_LOCAL);
		}

		vecScale.x = 1.0f;
		vecScale.y = vecScale.z = TransformScaleFromIntToFloat( m_CustomizeData.legWidth, m_CustomizeFactor.fWidthLegFactor, WIDTH_LEG_FACTOR);

		A3DBone * pBoneLCalf = pSkeleton->GetBone("Bip01 L Calf", NULL);
		if( pBoneLCalf )
		{
			pBoneLCalf->SetScaleFactor(vecScale);
			pBoneLCalf->SetScaleType(A3DBone::SCALE_LOCAL);
		}
		A3DBone * pBoneRCalf = pSkeleton->GetBone("Bip01 R Calf", NULL);
		if( pBoneRCalf )
		{
			pBoneRCalf->SetScaleFactor(vecScale);
			pBoneRCalf->SetScaleType(A3DBone::SCALE_LOCAL);
		}
		ScaleBody(m_fScaleBySkill);
	}
}

//更新头发模型及贴图(与头盔模型（或时装头饰）显示相关)
void CECPlayer::UpdateHairModel(bool bUpdateAll, int iArmetID)
{
	if( (m_iProfession == PROF_ORC) ||
		(!m_pFaceModel || !m_pFaceModel->GetFaceData()) ||
		(iArmetID == -1))
		return;

	iArmetID &= 0xffff;

	//  iArmetID为0时，恢复形象的默认个性化数据
	int idHairModel = m_CustomizeData.faceData.idHairModel;
	int idHairTex	= m_CustomizeData.faceData.idHairTex;

	if(iArmetID != 0)
	{
		// 根据iArmetID找到相应头盔(或时装头饰)，读取对应的idHairModel和idHairTex
		DATA_TYPE dt;
		ARMOR_ESSENCE * pArmorHead = (ARMOR_ESSENCE *) g_pGame->GetElementDataMan()->get_data_ptr(iArmetID, ID_SPACE_ESSENCE, dt);
		
		if(dt != DT_ARMOR_ESSENCE)	// 非头盔id，则为时装头饰id
		{
			FASHION_ESSENCE* pFashionHead = (FASHION_ESSENCE*)g_pGame->GetElementDataMan()->get_data_ptr(iArmetID, ID_SPACE_ESSENCE, dt);
			ASSERT(dt == DT_FASHION_ESSENCE);

			if(dt == DT_FASHION_ESSENCE && pFashionHead)
			{
				idHairModel = pFashionHead->id_hair;
				idHairTex = pFashionHead->id_hair_texture;
			}			
		}
		else if( pArmorHead )
		{
			if(pArmorHead->id_hair != 0)
				idHairModel = pArmorHead->id_hair;

			if(pArmorHead->id_hair_texture != 0)
				idHairTex = pArmorHead->id_hair_texture;
		}
	}

	m_pFaceModel->GetFaceData()->idHairModel = idHairModel;
	m_pFaceModel->GetFaceData()->idHairTex	 = idHairTex;

	if (bUpdateAll)
		m_pFaceModel->UpdateAll();
}

bool CECPlayer::ValidateCustomizeData(int nProf, int nGender, PLAYER_CUSTOMIZEDATA &data)
{
	bool modified(false);

	static const char *s_hairModel[NUM_PROFESSION][2] = {
		{"facedata\\武侠男\\预定义\\01武侠男标准1.ini", "facedata\\法师女\\预定义\\武侠女标准1.ini"},  // 武侠
		{"facedata\\武侠男\\预定义\\法师男标准1.ini", "facedata\\法师女\\预定义\\01法师女标准1.ini"},  // 法师
		{"facedata\\影族男\\预定义\\巫师男标准01.ini", "facedata\\影族女\\预定义\\巫师女标准01.ini"},  // 巫师
		{"", "facedata\\法师女\\预定义\\妖精女标准1.ini"}, // 妖精
		{"facedata\\妖兽男\\预定义\\妖兽男标准.ini", ""},  // 妖兽
		{"facedata\\影族男\\预定义\\刺客男标准01.ini", "facedata\\影族女\\预定义\\刺客女标准01.ini"},  // 刺客
		{"facedata\\武侠男\\预定义\\羽灵男标准1.ini", "facedata\\法师女\\预定义\\羽芒女标准1.ini"},	   // 羽芒
		{"facedata\\武侠男\\预定义\\羽灵男标准1.ini", "facedata\\法师女\\预定义\\羽灵女标准1.ini"},    // 羽灵
		{"facedata\\灵族男\\预定义\\剑灵男标准01.ini", "facedata\\灵族女\\预定义\\剑灵女标准01.ini"},  // 剑灵
		{"facedata\\灵族男\\预定义\\魅灵男标准01.ini", "facedata\\灵族女\\预定义\\魅灵女标准01.ini"},  // 魅灵
		{"facedata\\胧族男\\预定义\\夜影男标准01.ini", "facedata\\胧族女\\预定义\\夜影女标准01.ini"},  // 夜影
		{"facedata\\胧族男\\预定义\\月仙男标准01.ini", "facedata\\胧族女\\预定义\\月仙女标准01.ini"},  // 月仙
	};

	static int s_idHairModel[NUM_PROFESSION][2] = {0};
	static int s_idHairTex[NUM_PROFESSION][2] = {0};
	static bool s_loaded[NUM_PROFESSION][2];

	int &defaultHairModel = s_idHairModel[nProf][nGender];
	int &defaultHairTex = s_idHairTex[nProf][nGender];
	bool &loaded = s_loaded[nProf][nGender];

	// Check and load default hair model and tex
	if (!loaded)
	{
		defaultHairModel = 0;
		defaultHairTex = 0;
		loaded = true;

		// Get from file
		const char *szFile = s_hairModel[nProf][nGender];
		
		if (szFile && strlen(szFile)>0)
		{
			AIniFile IniFile;
			if(!IniFile.Open(szFile))
			{
				a_LogOutput(1, "CECPlayer::ValidateCustomizeData, Failed to open file %s", szFile);
			}
			else
			{
				AString strSection = "Hair";
				defaultHairModel = IniFile.GetValueAsInt(strSection, "idHairModel", 0);
				defaultHairTex = IniFile.GetValueAsInt(strSection, "idHairTex", 0);
				IniFile.Close();
			}
		}
	}

	elementdataman* pElementDataMan = g_pGame->GetElementDataMan();

	unsigned short &idHairModel = data.faceData.idHairModel;
	unsigned short &idHairTex = data.faceData.idHairTex;

	DATA_TYPE datatype;
	if (defaultHairModel && idHairModel)
	{
		FACE_HAIR_ESSENCE* pEssence = (FACE_HAIR_ESSENCE*)pElementDataMan->get_data_ptr(idHairModel, ID_SPACE_FACE, datatype);
		if (pEssence && datatype==DT_FACE_HAIR_ESSENCE && pEssence->fashion_head_only)
		{
			// Hair model is invalid
			a_LogOutput(1, "CECPlayer::ValidateCustomizeData, idHairModel(%d) is invalid and set to default", idHairModel);
			idHairModel = defaultHairModel;
			modified = true;
		}
	}
	if (defaultHairTex && idHairTex)
	{
		FACE_TEXTURE_ESSENCE* pEssence = (FACE_TEXTURE_ESSENCE*)pElementDataMan->get_data_ptr(idHairTex, ID_SPACE_FACE, datatype);
		if (pEssence && datatype==DT_FACE_TEXTURE_ESSENCE && pEssence->fashion_head_only)
		{
			// Hair tex is invalid
			a_LogOutput(1, "CECPlayer::ValidateCustomizeData, idHairTex(%d) is invalid and set to default", idHairTex);
			idHairTex = defaultHairTex;
			modified = true;
		}
	}
	
	// only allowed the third eye in these professions
	unsigned short &idThirdEye = data.faceData.idThirdEye;
	if( nProf != PROF_MEILING &&
		nProf != PROF_JIANLING && 
		idThirdEye != 0)
	{
		a_LogOutput(1, "CECPlayer::ValidateCustomizeData, idThirdEye(%d) is invalid and set to default", idThirdEye);
		idThirdEye = 0;
		modified = true;
	}

	return modified;
}

//	Change customize data
bool CECPlayer::ChangeCustomizeData(const PLAYER_CUSTOMIZEDATA& data, bool bApply/* true */)
{
	if( data.dwVersion >= CUSTOMIZE_DATA_VERSION_1 &&
		data.dwVersion <= CUSTOMIZE_DATA_VERSION &&
		data.faceData.scaleUp > 0.0f )
	{
		if( !m_pFaceModel || !GetMajorModel() )
			bApply = false;

		if (bApply && m_CustomizeData.bodyID != data.bodyID)
		{
			ASSERT(ShouldUseClothedModel());	// 这里不可能是简单模型模式

			//	We should reload a new body
			if (!LoadBodySkin(data.bodyID))
				return false;

			if (!LoadFaceModel(m_iProfession, m_iGender, data.bodyID))
			{
				a_LogOutput(1, "CECPlayer::ChangeCustomizeData, Failed to load face model");
				return false;
			}
		}

		m_CustomizeData = data;

		// 某些不允许的个性化情况（如时装头巾被用来替换非时装状态下的默认头发）
		ValidateCustomizeData(GetProfession(), GetGender(), m_CustomizeData);

		if (bApply)
		{
			if (m_pFaceModel)
				m_pFaceModel->SetFaceData(m_CustomizeData.faceData);

			if(!InFashionMode())
				UpdateHairModel(true);
	
			SetBodyColor(data.colorBody);
			UpdateBodyScales();
		}

		SetResReadyFlag(RESFG_CUSTOM, true);
		return true;
	}
	else
	{
		if( m_pFaceModel )
			m_CustomizeData.faceData = *m_pFaceModel->GetFaceData();

		SetResReadyFlag(RESFG_CUSTOM, true);
		return false;
	}
}

// 更新形象的默认个性化数据
bool CECPlayer::SaveAsDefaultCustomizeData()
{
	const CECFace::FACE_CUSTOMIZEDATA& faceData = m_CustomizeData.faceData;

	AIniFile IniFile;
	char szFile[256];
	sprintf(szFile, "userdata\\character\\character%d%d.ini", m_iProfession, m_iGender);
	
	IniFile.Open(szFile);

	//3庭3
	AString strSection = "3Parts";
	IniFile.WriteIntValue(strSection, "scaleUp", faceData.scaleUp);
	IniFile.WriteIntValue(strSection, "scaleMiddle", faceData.scaleMiddle);
	IniFile.WriteIntValue(strSection, "scaleDown", faceData.scaleDown);

	//脸型融合3
	strSection = "BlendFace";
	IniFile.WriteIntValue(strSection, "idFaceShape1", faceData.idFaceShape1);
	IniFile.WriteIntValue(strSection, "idFaceShape2", faceData.idFaceShape2);
	IniFile.WriteIntValue(strSection, "blendFaceShape", faceData.blendFaceShape);

	//脸盘调整3
	strSection = "Face";
	IniFile.WriteIntValue(strSection, "scaleFaceH", faceData.scaleFaceH);
	IniFile.WriteIntValue(strSection, "scaleFaceV", faceData.scaleFaceV);
	IniFile.WriteIntValue(strSection, "idFaceTex", faceData.idFaceTex);

	//法令
	strSection = "Faling";
	IniFile.WriteIntValue(strSection, "idFalingSkin", faceData.idFalingSkin);
	IniFile.WriteIntValue(strSection, "idFalingTex", faceData.idFalingTex);

	//额头5
	strSection = "Forehead";
	IniFile.WriteIntValue(strSection, "offsetForeheadH", faceData.offsetForeheadH);
	IniFile.WriteIntValue(strSection, "offsetForeheadV", faceData.offsetForeheadV);
	IniFile.WriteIntValue(strSection, "offsetForeheadZ", faceData.offsetForeheadZ);
	IniFile.WriteIntValue(strSection, "rotateForehead", faceData.rotateForehead);
	IniFile.WriteIntValue(strSection, "scaleForehead", faceData.scaleForehead);

	//颧骨5
	strSection = "YokeBone";
	IniFile.WriteIntValue(strSection, "offsetYokeBoneH", faceData.offsetYokeBoneH);
	IniFile.WriteIntValue(strSection, "offsetYokeBoneV", faceData.offsetYokeBoneV);
	IniFile.WriteIntValue(strSection, "offsetYokeBoneZ", faceData.offsetYokeBoneZ);
	IniFile.WriteIntValue(strSection, "rotateYokeBone", faceData.rotateYokeBone);
	IniFile.WriteIntValue(strSection, "scaleYokeBone", faceData.scaleYokeBone);
	
	//脸颊4
	strSection = "Cheek";
	IniFile.WriteIntValue(strSection, "offsetCheekH", faceData.offsetCheekH);
	IniFile.WriteIntValue(strSection, "offsetCheekV", faceData.offsetCheekV);
	IniFile.WriteIntValue(strSection, "offsetCheekZ", faceData.offsetCheekZ);
	IniFile.WriteIntValue(strSection, "scaleCheek", faceData.scaleCheek);

	//下巴4
	strSection = "Chain";
	IniFile.WriteIntValue(strSection, "offsetChainV", faceData.offsetChainV);
	IniFile.WriteIntValue(strSection, "offsetChainZ", faceData.offsetChainZ);
	IniFile.WriteIntValue(strSection, "rotateChain", faceData.rotateChain);
	IniFile.WriteIntValue(strSection, "scaleChainH", faceData.scaleChainH);

	//颌骨6
	strSection = "Jaw";
	IniFile.WriteIntValue(strSection, "offsetJawH", faceData.offsetJawH);
	IniFile.WriteIntValue(strSection, "offsetJawV", faceData.offsetJawV);
	IniFile.WriteIntValue(strSection, "offsetJawZ", faceData.offsetJawZ);
	IniFile.WriteIntValue(strSection, "scaleJawSpecial", faceData.scaleJawSpecial);
	IniFile.WriteIntValue(strSection, "scaleJawH", faceData.scaleJawH);
	IniFile.WriteIntValue(strSection, "scaleJawV", faceData.scaleJawV);

	//眼睛18
	strSection = "Eye";
	IniFile.WriteIntValue(strSection, "idThirdEye", faceData.idThirdEye);
	IniFile.WriteIntValue(strSection, "idEyeBaseTex", faceData.idEyeBaseTex);
	IniFile.WriteIntValue(strSection, "idEyeHighTex", faceData.idEyeHighTex);
	IniFile.WriteIntValue(strSection, "idEyeBallTex", faceData.idEyeBallTex);
	IniFile.WriteIntValue(strSection, "idEyeShape", faceData.idEyeShape);
	IniFile.WriteIntValue(strSection, "scaleEyeH", faceData.scaleEyeH);
	IniFile.WriteIntValue(strSection, "scaleEyeV", faceData.scaleEyeV);
	IniFile.WriteIntValue(strSection, "rotateEye", faceData.rotateEye);
	IniFile.WriteIntValue(strSection, "offsetEyeH", faceData.offsetEyeH);
	IniFile.WriteIntValue(strSection, "offsetEyeV", faceData.offsetEyeV);
	IniFile.WriteIntValue(strSection, "offseteyeZ", faceData.offsetEyeZ);
	IniFile.WriteIntValue(strSection, "scaleEyeBall", faceData.scaleEyeBall);

	IniFile.WriteIntValue(strSection, "scaleEyeH2", faceData.scaleEyeH2);
	IniFile.WriteIntValue(strSection, "scaleEyeV2", faceData.scaleEyeV2);
	IniFile.WriteIntValue(strSection, "rotateEye2", faceData.rotateEye2);
	IniFile.WriteIntValue(strSection, "offsetEyeH2", faceData.offsetEyeH2);
	IniFile.WriteIntValue(strSection, "offsetEyeV2", faceData.offsetEyeV2);
	IniFile.WriteIntValue(strSection, "offseteyeZ2", faceData.offsetEyeZ2);
	IniFile.WriteIntValue(strSection, "scaleEyeBall2", faceData.scaleEyeBall2);
	
	//眉毛14
	strSection = "Brow";
	IniFile.WriteIntValue(strSection, "idBrowTex", faceData.idBrowTex);
	IniFile.WriteIntValue(strSection, "idBrowShape", faceData.idBrowShape);
	IniFile.WriteIntValue(strSection, "scaleBrowH", faceData.scaleBrowH);
	IniFile.WriteIntValue(strSection, "scaleBrowV", faceData.scaleBrowV);
	IniFile.WriteIntValue(strSection, "rotateBrow", faceData.rotateBrow);
	IniFile.WriteIntValue(strSection, "offsetBrowH", faceData.offsetBrowH);
	IniFile.WriteIntValue(strSection, "offsetBrowV", faceData.offsetBrowV);
	IniFile.WriteIntValue(strSection, "offsetBrowZ", faceData.offsetBrowZ);

	IniFile.WriteIntValue(strSection, "scaleBrowH2", faceData.scaleBrowH2);
	IniFile.WriteIntValue(strSection, "scaleBrowV2", faceData.scaleBrowV2);
	IniFile.WriteIntValue(strSection, "rotateBrow2", faceData.rotateBrow2);
	IniFile.WriteIntValue(strSection, "offsetBrowH2", faceData.offsetBrowH2);
	IniFile.WriteIntValue(strSection, "offsetBrowV2", faceData.offsetBrowV2);
	IniFile.WriteIntValue(strSection, "offsetBrowZ2", faceData.offsetBrowZ2);

	//鼻子8
	strSection = "Nose";
	IniFile.WriteIntValue(strSection, "idNoseTex", faceData.idNoseTex);
	IniFile.WriteIntValue(strSection, "idNoseTipShape", faceData.idNoseTipShape);
	IniFile.WriteIntValue(strSection, "scaleNoseTipH", faceData.scaleNoseTipH);
	IniFile.WriteIntValue(strSection, "scaleNoseTipV", faceData.scaleNoseTipV);
	IniFile.WriteIntValue(strSection, "scaleNoseTipZ", faceData.scaleNoseTipZ);
	IniFile.WriteIntValue(strSection, "offsetNoseTipV", faceData.offsetNoseTipV);
	IniFile.WriteIntValue(strSection, "idNoseBridgeShape", faceData.idNoseBridgeShape);
	IniFile.WriteIntValue(strSection, "scaleBridgeTipH", faceData.scaleBridgeTipH);
	IniFile.WriteIntValue(strSection, "offsetBridgeTipZ", faceData.offsetBridgeTipZ);

	//嘴9
	strSection = "Mouth";
	IniFile.WriteIntValue (strSection, "idMouthUpLipLine", faceData.idMouthUpLipLine);
	IniFile.WriteIntValue(strSection, "idMouthMidLipLine", faceData.idMouthMidLipLine);
	IniFile.WriteIntValue(strSection, "idMouthDownLipLine", faceData.idMouthDownLipLine);
	IniFile.WriteIntValue(strSection, "thickUpLip", faceData.thickUpLip);
	IniFile.WriteIntValue(strSection, "thickDownLip", faceData.thickDownLip);
	IniFile.WriteIntValue(strSection, "scaleMouthH", faceData.scaleMouthH);
	IniFile.WriteIntValue(strSection, "offsetMouthV", faceData.offsetMouthV);
	IniFile.WriteIntValue(strSection, "offsetMOuthZ", faceData.offsetMouthZ);
	IniFile.WriteIntValue(strSection, "idMouthTex", faceData.idMouthTex);
	IniFile.WriteIntValue(strSection, "offsetCornerOfMouthSpecial", faceData.offsetCornerOfMouthSpecial);
	
	IniFile.WriteIntValue(strSection, "scaleMouthH2", faceData.scaleMouthH2);
	IniFile.WriteIntValue(strSection, "offsetCornerOfMouthSpecial2", faceData.offsetCornerOfMouthSpecial2);

	//耳朵3
	strSection = "Ear";
	IniFile.WriteIntValue(strSection, "idEarShape", faceData.idEarShape);
	IniFile.WriteIntValue(strSection, "scaleEar", faceData.scaleEar);
	IniFile.WriteIntValue(strSection, "offsetEarV", faceData.offsetEarV);
	
	//头发1
	strSection = "Hair";
	IniFile.WriteIntValue(strSection, "idHairModel", faceData.idHairModel);
	IniFile.WriteIntValue(strSection, "idHairTex", faceData.idHairTex);

	//胡子2
	strSection = "Moustache";
	IniFile.WriteIntValue(strSection, "idMoustacheTex", faceData.idMoustacheTex);
	IniFile.WriteIntValue(strSection, "idMoustacheSkin", faceData.idMoustacheSkin);
	IniFile.WriteIntValue(strSection, "idGoateeTex", faceData.idGoateeTex);

	//各部分颜色7
	strSection = "Color";
	IniFile.WriteIntValue(strSection, "colorHair", faceData.colorHair);
	IniFile.WriteIntValue(strSection, "colorFace", faceData.colorFace);
	IniFile.WriteIntValue(strSection, "colorEye", faceData.colorEye);
	IniFile.WriteIntValue(strSection, "colorBrow", faceData.colorBrow);
	IniFile.WriteIntValue(strSection, "colorMouth", faceData.colorMouth);
	IniFile.WriteIntValue(strSection, "colorEyeBall", faceData.colorEyeBall);
	IniFile.WriteIntValue(strSection, "colorMoustache", faceData.colorMoustache);

	//身体参数
	strSection = "Body";
	IniFile.WriteIntValue(strSection, "bodyID", m_CustomizeData.bodyID);
	IniFile.WriteIntValue(strSection, "colorBody", m_CustomizeData.colorBody);
	IniFile.WriteIntValue(strSection, "headScale", m_CustomizeData.headScale);
	IniFile.WriteIntValue(strSection, "upScale", m_CustomizeData.upScale);
	IniFile.WriteIntValue(strSection, "waistScale", m_CustomizeData.waistScale);
	IniFile.WriteIntValue(strSection, "armWidth", m_CustomizeData.armWidth);
	IniFile.WriteIntValue(strSection, "legWidth", m_CustomizeData.legWidth);
	IniFile.WriteIntValue(strSection, "breastScale", m_CustomizeData.breastScale);

	IniFile.Save(szFile);
	IniFile.Close();

	return true;
}

// 载入形象的默认个性化数据
bool CECPlayer::LoadDefaultCustomizeData(int prof, int gender, PLAYER_CUSTOMIZEDATA &data)
{
	memset(&data, 0, sizeof(data));
	data.dwVersion = CUSTOMIZE_DATA_VERSION;
	data.colorBody = 0xffffffff;
	data.headScale = 128;
	data.upScale = 128;
	data.waistScale = 128;
	data.armWidth = 128;
	data.legWidth = 128;
	data.breastScale = 128;

	AIniFile IniFile;
	char szFile[256];
	sprintf(szFile, "userdata\\character\\character%d%d.ini", prof, gender);
	
	if(!IniFile.Open(szFile))
	{
		a_LogOutput(1, "CECPlayer::LoadDefaultCustomizeData, Failed to open file %s", szFile);
		return false;
	}
	//3庭3
	AString strSection = "3Parts";
	data.faceData.scaleUp = IniFile.GetValueAsInt(strSection, "scaleUp", data.faceData.scaleUp);
	data.faceData.scaleMiddle = IniFile.GetValueAsInt(strSection, "scaleMiddle", data.faceData.scaleMiddle);
	data.faceData.scaleDown = IniFile.GetValueAsInt(strSection, "scaleDown", data.faceData.scaleDown);

	//脸型融合3
	strSection = "BlendFace";
	data.faceData.idFaceShape1 = IniFile.GetValueAsInt(strSection, "idFaceShape1", data.faceData.idFaceShape1);
	data.faceData.idFaceShape2 = IniFile.GetValueAsInt(strSection, "idFaceShape2", data.faceData.idFaceShape2);
	data.faceData.blendFaceShape = IniFile.GetValueAsInt(strSection, "blendFaceShape", data.faceData.blendFaceShape);

	//脸盘调整3
	strSection = "Face";
	data.faceData.scaleFaceH = IniFile.GetValueAsInt(strSection, "scaleFaceH", data.faceData.scaleFaceH);
	data.faceData.scaleFaceV = IniFile.GetValueAsInt(strSection, "scaleFaceV", data.faceData.scaleFaceV);
	data.faceData.idFaceTex = IniFile.GetValueAsInt(strSection, "idFaceTex", data.faceData.idFaceTex);

	//法令
	strSection = "Faling";
	data.faceData.idFalingSkin = IniFile.GetValueAsInt(strSection, "idFalingSkin", data.faceData.idFalingSkin);
	data.faceData.idFalingTex = IniFile.GetValueAsInt(strSection, "idFalingTex", data.faceData.idFalingTex);

	//额头5
	strSection = "Forehead";
	data.faceData.offsetForeheadH = IniFile.GetValueAsInt(strSection, "offsetForeheadH", data.faceData.offsetForeheadH);
	data.faceData.offsetForeheadV = IniFile.GetValueAsInt(strSection, "offsetForeheadV", data.faceData.offsetForeheadV);
	data.faceData.offsetForeheadZ = IniFile.GetValueAsInt(strSection, "offsetForeheadZ", data.faceData.offsetForeheadZ);
	data.faceData.rotateForehead = IniFile.GetValueAsInt(strSection, "rotateForehead", data.faceData.rotateForehead);
	data.faceData.scaleForehead = IniFile.GetValueAsInt(strSection, "scaleForehead", data.faceData.scaleForehead);

	//颧骨5
	strSection = "YokeBone";
	data.faceData.offsetYokeBoneH = IniFile.GetValueAsInt(strSection, "offsetYokeBoneH", data.faceData.offsetYokeBoneH);
	data.faceData.offsetYokeBoneV = IniFile.GetValueAsInt(strSection, "offsetYokeBoneV", data.faceData.offsetYokeBoneV);
	data.faceData.offsetYokeBoneZ = IniFile.GetValueAsInt(strSection, "offsetYokeBoneZ", data.faceData.offsetYokeBoneZ);
	data.faceData.rotateYokeBone = IniFile.GetValueAsInt(strSection, "rotateYokeBone", data.faceData.rotateYokeBone);
	data.faceData.scaleYokeBone = IniFile.GetValueAsInt(strSection, "scaleYokeBone", data.faceData.scaleYokeBone);
	
	//脸颊4
	strSection = "Cheek";
	data.faceData.offsetCheekH = IniFile.GetValueAsInt(strSection, "offsetCheekH", data.faceData.offsetCheekH);
	data.faceData.offsetCheekV = IniFile.GetValueAsInt(strSection, "offsetCheekV", data.faceData.offsetCheekV);
	data.faceData.offsetCheekZ = IniFile.GetValueAsInt(strSection, "offsetCheekZ", data.faceData.offsetCheekZ);
	data.faceData.scaleCheek = IniFile.GetValueAsInt(strSection, "scaleCheek", data.faceData.scaleCheek);

	//下巴4
	strSection = "Chain";
	data.faceData.offsetChainV = IniFile.GetValueAsInt(strSection, "offsetChainV", data.faceData.offsetChainV);
	data.faceData.offsetChainZ = IniFile.GetValueAsInt(strSection, "offsetChainZ", data.faceData.offsetChainZ);
	data.faceData.rotateChain = IniFile.GetValueAsInt(strSection, "rotateChain", data.faceData.rotateChain);
	data.faceData.scaleChainH = IniFile.GetValueAsInt(strSection, "scaleChainH", data.faceData.scaleChainH);

	//颌骨6
	strSection = "Jaw";
	data.faceData.offsetJawH = IniFile.GetValueAsInt(strSection, "offsetJawH", data.faceData.offsetJawH);
	data.faceData.offsetJawV = IniFile.GetValueAsInt(strSection, "offsetJawV", data.faceData.offsetJawV);
	data.faceData.offsetJawZ = IniFile.GetValueAsInt(strSection, "offsetJawZ", data.faceData.offsetJawZ);
	data.faceData.scaleJawSpecial = IniFile.GetValueAsInt(strSection, "scaleJawSpecial", data.faceData.scaleJawSpecial);
	data.faceData.scaleJawH = IniFile.GetValueAsInt(strSection, "scaleJawH", data.faceData.scaleJawH);
	data.faceData.scaleJawV = IniFile.GetValueAsInt(strSection, "scaleJawV", data.faceData.scaleJawV);

	//眼睛18
	strSection = "Eye";
	data.faceData.idThirdEye	= IniFile.GetValueAsInt(strSection, "idThirdEye", data.faceData.idThirdEye);
	data.faceData.idEyeBaseTex = IniFile.GetValueAsInt(strSection, "idEyeBaseTex", data.faceData.idEyeBaseTex);
	data.faceData.idEyeHighTex = IniFile.GetValueAsInt(strSection, "idEyeHighTex", data.faceData.idEyeHighTex);
	data.faceData.idEyeBallTex = IniFile.GetValueAsInt(strSection, "idEyeBallTex", data.faceData.idEyeBallTex);
	data.faceData.idEyeShape = IniFile.GetValueAsInt(strSection, "idEyeShape", data.faceData.idEyeShape);
	data.faceData.scaleEyeH = IniFile.GetValueAsInt(strSection, "scaleEyeH", data.faceData.scaleEyeH);
	data.faceData.scaleEyeV = IniFile.GetValueAsInt(strSection, "scaleEyeV", data.faceData.scaleEyeV);
	data.faceData.rotateEye = IniFile.GetValueAsInt(strSection, "rotateEye", data.faceData.rotateEye);
	data.faceData.offsetEyeH = IniFile.GetValueAsInt(strSection, "offsetEyeH", data.faceData.offsetEyeH);
	data.faceData.offsetEyeV = IniFile.GetValueAsInt(strSection, "offsetEyeV", data.faceData.offsetEyeV);
	data.faceData.offsetEyeZ = IniFile.GetValueAsInt(strSection, "offseteyeZ", data.faceData.offsetEyeZ);
	data.faceData.scaleEyeBall = IniFile.GetValueAsInt(strSection, "scaleEyeBall", data.faceData.scaleEyeBall);
	
	data.faceData.scaleEyeH2 = IniFile.GetValueAsInt(strSection, "scaleEyeH2", data.faceData.scaleEyeH2);
	data.faceData.scaleEyeV2 = IniFile.GetValueAsInt(strSection, "scaleEyeV2", data.faceData.scaleEyeV2);
	data.faceData.rotateEye2= IniFile.GetValueAsInt(strSection, "rotateEye2", data.faceData.rotateEye2);
	data.faceData.offsetEyeH2 = IniFile.GetValueAsInt(strSection, "offsetEyeH2", data.faceData.offsetEyeH2);
	data.faceData.offsetEyeV2 = IniFile.GetValueAsInt(strSection, "offsetEyeV2", data.faceData.offsetEyeV2);
	data.faceData.offsetEyeZ2 = IniFile.GetValueAsInt(strSection, "offseteyeZ2", data.faceData.offsetEyeZ2);
	data.faceData.scaleEyeBall2 = IniFile.GetValueAsInt(strSection, "scaleEyeBall2", data.faceData.scaleEyeBall2);

	//眉毛14
	strSection = "Brow";
	data.faceData.idBrowTex = IniFile.GetValueAsInt(strSection, "idBrowTex", data.faceData.idBrowTex);
	data.faceData.idBrowShape = IniFile.GetValueAsInt(strSection, "idBrowShape", data.faceData.idBrowShape);
	data.faceData.scaleBrowH = IniFile.GetValueAsInt(strSection, "scaleBrowH", data.faceData.scaleBrowH);
	data.faceData.scaleBrowV = IniFile.GetValueAsInt(strSection, "scaleBrowV", data.faceData.scaleBrowV);
	data.faceData.rotateBrow = IniFile.GetValueAsInt(strSection, "rotateBrow", data.faceData.rotateBrow);
	data.faceData.offsetBrowH = IniFile.GetValueAsInt(strSection, "offsetBrowH", data.faceData.offsetBrowH);
	data.faceData.offsetBrowV = IniFile.GetValueAsInt(strSection, "offsetBrowV", data.faceData.offsetBrowV);
	data.faceData.offsetBrowZ = IniFile.GetValueAsInt(strSection, "offsetBrowZ", data.faceData.offsetBrowZ);

	data.faceData.scaleBrowH2 = IniFile.GetValueAsInt(strSection, "scaleBrowH2", data.faceData.scaleBrowH2);
	data.faceData.scaleBrowV2 = IniFile.GetValueAsInt(strSection, "scaleBrowV2", data.faceData.scaleBrowV2);
	data.faceData.rotateBrow2 = IniFile.GetValueAsInt(strSection, "rotateBrow2", data.faceData.rotateBrow2);
	data.faceData.offsetBrowH2 = IniFile.GetValueAsInt(strSection, "offsetBrowH2", data.faceData.offsetBrowH2);
	data.faceData.offsetBrowV2 = IniFile.GetValueAsInt(strSection, "offsetBrowV2", data.faceData.offsetBrowV2);
	data.faceData.offsetBrowZ2 = IniFile.GetValueAsInt(strSection, "offsetBrowZ2", data.faceData.offsetBrowZ2);

	//鼻子9
	strSection = "Nose";
	data.faceData.idNoseTex = IniFile.GetValueAsInt(strSection, "idNoseTex", data.faceData.idNoseTex);
	data.faceData.idNoseTipShape = IniFile.GetValueAsInt(strSection, "idNoseTipShape", data.faceData.idNoseTipShape);
	data.faceData.scaleNoseTipH = IniFile.GetValueAsInt(strSection, "scaleNoseTipH", data.faceData.scaleNoseTipH);
	data.faceData.scaleNoseTipV = IniFile.GetValueAsInt(strSection, "scaleNoseTipV", data.faceData.scaleNoseTipV);
	data.faceData.scaleNoseTipZ = IniFile.GetValueAsInt(strSection, "scaleNoseTipZ", data.faceData.scaleNoseTipZ);
	data.faceData.offsetNoseTipV = IniFile.GetValueAsInt(strSection, "offsetNoseTipV", data.faceData.offsetNoseTipV);
	data.faceData.idNoseBridgeShape = IniFile.GetValueAsInt(strSection, "idNoseBridgeShape", data.faceData.idNoseBridgeShape);
	data.faceData.scaleBridgeTipH = IniFile.GetValueAsInt(strSection, "scaleBridgeTipH", data.faceData.scaleBridgeTipH);
	data.faceData.offsetBridgeTipZ = IniFile.GetValueAsInt(strSection, "OffsetBridgeTipZ", data.faceData.offsetBridgeTipZ);
	
	//嘴9
	strSection = "Mouth";
	data.faceData.idMouthUpLipLine = IniFile.GetValueAsInt (strSection, "idMouthUpLipLine", data.faceData.idMouthUpLipLine);
	data.faceData.idMouthMidLipLine = IniFile.GetValueAsInt(strSection, "idMouthMidLipLine", data.faceData.idMouthMidLipLine);
	data.faceData.idMouthDownLipLine = IniFile.GetValueAsInt(strSection, "idMouthDownLipLine", data.faceData.idMouthDownLipLine);
	data.faceData.thickUpLip = IniFile.GetValueAsInt(strSection, "thickUpLip", data.faceData.thickUpLip);
	data.faceData.thickDownLip = IniFile.GetValueAsInt(strSection, "thickDownLip", data.faceData.thickDownLip);
	data.faceData.scaleMouthH = IniFile.GetValueAsInt(strSection, "scaleMouthH", data.faceData.scaleMouthH);
	
	data.faceData.offsetMouthV = IniFile.GetValueAsInt(strSection, "offsetMouthV", data.faceData.offsetMouthV);
	data.faceData.offsetMouthZ = IniFile.GetValueAsInt(strSection, "offsetMOuthZ", data.faceData.offsetMouthZ);
	data.faceData.idMouthTex = IniFile.GetValueAsInt(strSection, "idMouthTex", data.faceData.idMouthTex);
	data.faceData.offsetCornerOfMouthSpecial = IniFile.GetValueAsInt(strSection, "offsetCornerOfMouthSpecial", data.faceData.offsetCornerOfMouthSpecial);

	data.faceData.scaleMouthH2 = IniFile.GetValueAsInt(strSection, "scaleMouthH2", data.faceData.scaleMouthH2);
	data.faceData.offsetCornerOfMouthSpecial2 = IniFile.GetValueAsInt(strSection, "offsetCornerOfMouthSpecial2", data.faceData.offsetCornerOfMouthSpecial2);

	//耳朵3
	strSection = "Ear";
	data.faceData.idEarShape = IniFile.GetValueAsInt(strSection, "idEarShape", data.faceData.idEarShape);
	data.faceData.scaleEar = IniFile.GetValueAsInt(strSection, "scaleEar", data.faceData.scaleEar);
	data.faceData.offsetEarV = IniFile.GetValueAsInt(strSection, "offsetEarV", data.faceData.offsetEarV);
	
	//头发1
	strSection = "Hair";
	data.faceData.idHairModel = IniFile.GetValueAsInt(strSection, "idHairModel", data.faceData.idHairModel);
	data.faceData.idHairTex = IniFile.GetValueAsInt(strSection, "idHairTex", data.faceData.idHairTex);

	//胡子2
	strSection = "Moustache";
	data.faceData.idMoustacheTex = IniFile.GetValueAsInt(strSection, "idMoustacheTex", data.faceData.idMoustacheTex);
	data.faceData.idMoustacheSkin = IniFile.GetValueAsInt(strSection, "idMoustacheSkin", data.faceData.idMoustacheSkin);
	data.faceData.idGoateeTex = IniFile.GetValueAsInt(strSection, "idGoateeTex", data.faceData.idGoateeTex);

	//各部分颜色7
	strSection = "Color";
	data.faceData.colorHair = IniFile.GetValueAsInt(strSection, "colorHair", data.faceData.colorHair);
	data.faceData.colorFace = IniFile.GetValueAsInt(strSection, "colorFace", data.faceData.colorFace);
	data.faceData.colorEye = IniFile.GetValueAsInt(strSection, "colorEye", data.faceData.colorEye);
	data.faceData.colorBrow = IniFile.GetValueAsInt(strSection, "colorBrow", data.faceData.colorBrow);
	data.faceData.colorMouth = IniFile.GetValueAsInt(strSection, "colorMouth", data.faceData.colorMouth);
	data.faceData.colorEyeBall = IniFile.GetValueAsInt(strSection, "colorEyeBall", data.faceData.colorEyeBall);
	data.faceData.colorMoustache = IniFile.GetValueAsInt(strSection, "colorMoustache", data.faceData.colorMoustache);

	// 身体参数
	strSection = "Body";
	data.bodyID = IniFile.GetValueAsInt(strSection, "bodyID", data.bodyID);
	data.colorBody = IniFile.GetValueAsInt(strSection, "colorBody", data.colorBody);
	data.headScale = IniFile.GetValueAsInt(strSection, "headScale", data.headScale);
	data.upScale = IniFile.GetValueAsInt(strSection, "upScale", data.upScale);
	data.waistScale = IniFile.GetValueAsInt(strSection, "waistScale", data.waistScale);
	data.armWidth = IniFile.GetValueAsInt(strSection, "armWidth", data.armWidth);
	data.legWidth = IniFile.GetValueAsInt(strSection, "legWidth", data.legWidth);
	data.breastScale = IniFile.GetValueAsInt(strSection, "breastScale", data.breastScale);
         
	IniFile.Close();
	return true;
}

// Store player's customize data for later restore
void CECPlayer::StoreCustomizeData()
{
	memcpy(&m_OldCustomizeData, &m_CustomizeData, sizeof(PLAYER_CUSTOMIZEDATA));
}

// Retore player's customize data
void CECPlayer::RestoreCustomizeData()
{
	ChangeCustomizeData(m_OldCustomizeData);
}

//	Show / hide wing
void CECPlayer::ShowWing(bool bShow)
{
	if (m_pPlayerModel)
	{
		CECModel* pWing = m_pPlayerModel->GetChildModel(_wing);
		if (pWing) pWing->Show(bShow);

		CECModel* pWing2 = m_pPlayerModel->GetChildModel(_wing2);
		if (pWing2) pWing2->Show(bShow);
	}
}

enumWingType CECPlayer::FlyMode2WingType(unsigned int flymode) {
	switch (flymode){
	case 0:
		return WINGTYPE_FLYSWORD;
	case 1:
		return WINGTYPE_WING;
	case 2:
		return WINGTYPE_DOUBLEWHEEL;
	default:
		ASSERT(false && AString().Format("unknow fly mode: %d", flymode));
		return WINGTYPE_FLYSWORD;
	}
}

//	Show / hide wing
void CECPlayer::ShowWeapon(bool bShow)
{
	if (m_pPlayerModel)
	{
		CECModel* pLeftHandWeapon = GetLeftHandWeapon();
		if (pLeftHandWeapon)
			pLeftHandWeapon->Show(bShow);

		CECModel* pRightHandWeapon = GetRightHandWeapon();
		if (pRightHandWeapon)
			pRightHandWeapon->Show(bShow);
	}
}

void CECPlayer::ShowWeaponByConfig(const PLAYER_ACTION_INFO_CONFIG *p){
	m_bShowWeapon = p->hide_weapon ? false : true;
	ShowWeapon(m_bShowWeapon);
}

//	Add money amount
int CECPlayer::AddMoneyAmount(int iAmount)
{
	m_iMoneyCnt += iAmount;
	if (m_iMoneyCnt < 0)
	{
		g_pGame->RuntimeDebugInfo(RTDCOL_WARNING, _AL("Player money is negative"));
		m_iMoneyCnt = 0;
	}

	return m_iMoneyCnt;
}

//	Render titles / names / talks above player's header
bool CECPlayer::RenderName(CECViewport* pViewport, DWORD dwFlags)
{
	return true;
}

//	Render booth name
int CECPlayer::RenderBoothName(CECViewport* pViewport, int y)
{
	return 0;
}

//	Render bar above player's header
bool CECPlayer::RenderBars(CECViewport* pViewport)
{
	if (!FillPateContent(pViewport))
		return true;

	int x, y, cx=100, cy=3;
	A3DFlatCollector* pFlat = g_pGame->GetA3DEngine()->GetA3DFlatCollector();
	CECConfigs* pConfigs = g_pGame->GetConfigs();
	DWORD colEmpty = A3DCOLORRGB(234, 159, 90);

	x = m_PateContent.iBaseX - (cx >> 1);
	y = m_PateContent.iCurY - 3;

	if (IsHostPlayer())
	{
		//	Draw exp bar
		if (false/*pConfigs->GetVideoSettings().bPlayerEXP*/)
		{
			if (m_BasicProps.iExp < 0)
				pFlat->AddRect_2D(x, y, x+cx-1, y+cy-1, colEmpty);
			else
			{
				int iLen = (int)ceil(cx * ((float)m_BasicProps.iExp / GetLevelUpExp(m_BasicProps.iLevel)));
				pFlat->AddRect_2D(x, y, x+cx-1, y+cy-1, colEmpty);
				pFlat->AddRect_2D(x, y, x+iLen-1, y+cy-1, A3DCOLORRGB(249, 44, 255));
			}

			y -= cy + 1;
		}

		//	Draw MP bar
		if (pConfigs->GetVideoSettings().bPlayerMP)
		{
			if (m_ExtProps.bs.max_mp && m_BasicProps.iCurMP)
			{
				int iLen = (int)ceil(cx * ((float)m_BasicProps.iCurMP / m_ExtProps.bs.max_mp));
				a_Clamp(iLen, 1, cx);
				pFlat->AddRect_2D(x, y, x+cx-1, y+cy-1, colEmpty);
				pFlat->AddRect_2D(x, y, x+iLen-1, y+cy-1, A3DCOLORRGB(57, 151, 255));
			}

			y -= cy + 1;
		}
	}

	//	Draw HP bar
	if (pConfigs->GetVideoSettings().bPlayerHP)
	{
		if (m_ExtProps.bs.max_hp && m_BasicProps.iCurHP)
		{
			int iLen = (int)ceil(cx * ((float)m_BasicProps.iCurHP / m_ExtProps.bs.max_hp));
			a_Clamp(iLen, 1, cx);
			pFlat->AddRect_2D(x, y, x+cx-1, y+cy-1, colEmpty);
			pFlat->AddRect_2D(x, y, x+iLen-1, y+cy-1, A3DCOLORRGB(255, 57, 62));
		}

		y -= cy + 1;
	}

	m_PateContent.iCurY	= y;

	return true;
}

//	Fill pate content
//	Return false if pate content is un-visible
bool CECPlayer::FillPateContent(CECViewport* pViewport)
{
	//	If m_PateContent.iVisible != 0, pate content has been filled
	//	for this frame
	if (m_PateContent.iVisible)
		return m_PateContent.iVisible == 2 ? true : false;

	A3DVIEWPORTPARAM* pViewPara = pViewport->GetA3DViewport()->GetParam();

	//	Calculate basic center position on screen
	A3DVECTOR3 vPos;
	
	if (IsInChariot())
	{
		vPos = m_aabb.Center + g_vAxisY * GetDummyModel(PLAYERMODEL_DUMMYTYPE2)->GetModelAABB().Extents.y * 2.f;
	}
	else 
		vPos = m_aabb.Center + g_vAxisY * m_aabb.Extents.y;
	
	if (GetGender() == GENDER_MALE){	//	男模型比较高，拉近时名字容易嵌到身体里面
		vPos.y += 0.1f;
	}

	if((IsRidingOnPet() && m_pPetModel))
	{
		const A3DAABB &aabb = GetPlayerPickAABB();
		vPos = aabb.Center + A3DVECTOR3(0, aabb.Extents.y, 0);
	}

	// 打坐使用特殊模型时，名字需要抬高位置 by SunXuewei 2009-11-18
	if(IsSitting() && (GetRace() == RACE_GHOST || GetRace() == RACE_OBORO))
	{
		static float scaleRatio[NUM_RACE][NUM_GENDER] = {{0.0f, 0.0f},	//	人类
			{0.0f, 0.0f},	//	妖族
			{0.0f, 0.0f},	//	羽人
			{1.0f, 0.5f},	//	汐族
			{0.0f, 0.0f},	//	灵族
			{1.0f, 1.0f},	//	胧族
		};
		vPos.y += m_aabb.Extents.y * scaleRatio[GetRace()][GetGender()];
	}
	
	// 胧族飞行时，名字需要抬高位置
	if (IsFlying() && GetRace() == RACE_OBORO) {
		vPos.y += m_aabb.Extents.y * 0.5f;
	}

	A3DVECTOR3 vScrPos;
	pViewport->GetA3DViewport()->Transform(vPos, vScrPos);
	if (vScrPos.z < pViewPara->MinZ || vScrPos.z > pViewPara->MaxZ)
	{
		m_PateContent.iVisible = 1;		//	In-visible
		return false;
	}

	m_PateContent.iVisible	= 2;
	m_PateContent.iBaseX	= (int)vScrPos.x;
	m_PateContent.iBaseY	= (int)vScrPos.y-10;
	m_PateContent.iCurY		= m_PateContent.iBaseY;
	m_PateContent.z			= vScrPos.z;

	return true;
}

float CECPlayer::GetGroundSpeed()
{
//	return m_bWalkRun ? g_pGame->GetConfigs()->GetHostRunSpeed() : m_ExtProps.mv.walk_speed;
	return m_bWalkRun ? m_ExtProps.mv.run_speed : m_ExtProps.mv.walk_speed;
}

//	Set resources ready flag
void CECPlayer::SetResReadyFlag(DWORD dwFlag, bool bSet)
{
	if (bSet)
	{
		m_dwResFlags |= dwFlag;
	}
	else
		m_dwResFlags &= ~dwFlag;
}

//	When all resources are ready, this function will be called
void CECPlayer::OnAllResourceReady()
{
/*	if (!m_pLevelUpGFX)
		m_pLevelUpGFX = g_pGame->GetGFXCaster()->LoadGFXEx(res_GFXFile(RES_GFX_LEVELUP));

	if (!m_pWaterWaveStill)
		m_pWaterWaveStill = g_pGame->GetGFXCaster()->LoadGFXEx(res_GFXFile(RES_GFX_WATER_WAVE_STILL));

	if (!m_pWaterWaveMove)
		m_pWaterWaveMove = g_pGame->GetGFXCaster()->LoadGFXEx(res_GFXFile(RES_GFX_WATER_WAVE_MOVE));

	if (!m_pAirBubble)
		m_pAirBubble = g_pGame->GetGFXCaster()->LoadGFXEx(res_GFXFile(RES_GFX_AIR_BUBBLE));

	if (!m_pTransformGfx)
		m_pTransformGfx = g_pGame->GetGFXCaster()->LoadGFXEx(res_GFXFile(RES_GFX_AIR_TRANSFORM));
*/
	if (m_iShape) TransformShape(GetShapeMask());
}

//	Caculate player's AABB base on profession and gender
void CECPlayer::CalcPlayerAABB()
{
	int iIndex = m_iProfession * NUM_GENDER + m_iGender;

	m_aabb.Extents = aExts[iIndex] * m_fScaleBySkill;
	m_aabbServer.Extents = aExts[iIndex];

	static const MOVECONST aMoveConsts[NUM_PROFESSION*NUM_GENDER] = 
	{
		//	武侠
		//	fStepHei	fMinAirHei	fMinWaterHei	fShoreDepth	fWaterSurf
		{	0.8f,		1.6f,		0.3f,			1.6f,		0.6f	},
		{	0.8f,		1.6f,		0.3f,			1.5f,		0.55f	},
		//	法师
		{	0.8f,		1.6f,		0.3f,			1.6f,		0.6f	},
		{	0.8f,		1.6f,		0.3f,			1.5f,		0.55f	},
		//	巫师
		{	0.8f,		1.6f,		0.3f,			1.6f,		0.6f	},
		{	0.8f,		1.6f,		0.3f,			1.5f,		0.55f	},
		//	妖精
		{	0.8f,		1.6f,		0.3f,			1.6f,		0.6f	},
		{	0.8f,		1.6f,		0.3f,			1.5f,		0.55f	},
 		//	妖兽
		{	0.8f,		1.6f,		0.3f,			1.8f,		0.7f	},
		{	0.8f,		1.6f,		0.3f,			1.6f,		0.6f	},
 		//	刺客
		{	0.8f,		1.6f,		0.3f,			1.6f,		0.6f	},
		{	0.8f,		1.6f,		0.3f,			1.5f,		0.55f	},
 		//	羽芒
		{	0.8f,		1.6f,		0.3f,			1.6f,		0.6f	},
		{	0.8f,		1.6f,		0.3f,			1.5f,		0.55f	},
 		//	羽灵
		{	0.8f,		1.6f,		0.3f,			1.6f,		0.6f	},
		{	0.8f,		1.6f,		0.3f,			1.5f,		0.55f	},
 		//	剑灵
		{	0.8f,		1.6f,		0.3f,			1.6f,		0.6f	},
		{	0.8f,		1.6f,		0.3f,			1.5f,		0.55f	},
 		//	魅灵
		{	0.8f,		1.6f,		0.3f,			1.6f,		0.6f	},
		{	0.8f,		1.6f,		0.3f,			1.5f,		0.55f	},
		//	夜影
		{	0.8f,		1.6f,		0.3f,			1.6f,		0.6f	},
		{	0.8f,		1.6f,		0.3f,			1.5f,		0.55f	},
		//	月仙
		{	0.8f,		1.6f,		0.3f,			1.6f,		0.6f	},
		{	0.8f,		1.6f,		0.3f,			1.5f,		0.55f	},
	};

	m_MoveConst = aMoveConsts[iIndex];
}

//	Get player's model aabb
const A3DAABB & CECPlayer::GetModelAABB()
{
	return GetPlayerPickAABB();
}

//	Get player's skin model
A3DSkinModel * CECPlayer::GetA3DSkinModel()
{
	if( m_pPlayerModel )
		return m_pPlayerModel->GetA3DSkinModel();
	else
		return NULL;
}

//	Get player's pick AABB
const A3DAABB& CECPlayer::GetPlayerPickAABB()
{
	if (m_pPlayerModel && m_pPlayerModel->GetA3DSkinModel())
		return m_pPlayerModel->GetA3DSkinModel()->GetModelAABB();
	else
		return m_aabb;
}

//	Get property move or stand action
int CECPlayer::GetMoveStandAction(bool bMove, bool bFight/* false */)const
{
	int iMoveEnv = m_iMoveEnv;
	if (m_AttachMode != enumAttachNone)
	{
		bFight = false;
		if( m_bHangerOn )
			iMoveEnv = MOVEENV_GROUND;
	}

	int iAction = ACT_STAND;

	if (bMove)
	{
		//	Play appropriate actions
		if (iMoveEnv == MOVEENV_GROUND)
		{
			if (m_bWalkRun)
				iAction = ACT_RUN;
			else
				iAction = ACT_WALK;
		}
		else if (iMoveEnv == MOVEENV_AIR)
		{
			if (/*UsingWing()*/ m_wingType == WINGTYPE_WING)
				iAction = ACT_FLY;
			else
				iAction = ACT_FLY_SWORD;
		}
		else if (iMoveEnv == MOVEENV_WATER){
			if (CanCombineWithMoveForSkill()){
				iAction = ACT_SWIM_FOR_MOVESKILL;
			}else{
				iAction = ACT_SWIM;
			}
		}
	}
	else
	{
		//	Play appropriate actions
		if (iMoveEnv == MOVEENV_GROUND)
		{
			if (bFight)
				iAction = ACT_FIGHTSTAND;
			else
				iAction = ACT_STAND;
		}
		else if (iMoveEnv == MOVEENV_AIR)
		{
			if (/*UsingWing()*/ m_wingType == WINGTYPE_WING)
				iAction = ACT_HANGINAIR;
			else
				iAction = ACT_HANGINAIR_SWORD;
		}
		else if (iMoveEnv == MOVEENV_WATER)
			iAction = ACT_HANGINWATER;
	}

	return iAction;
}

bool CECPlayer::IsMoveStandAction(int iAction){
	const static int s_MoveStandAction[] = {
		CECPlayer::ACT_RUN,
		CECPlayer::ACT_WALK,
		CECPlayer::ACT_FLY,
		CECPlayer::ACT_FLY_SWORD,
		CECPlayer::ACT_SWIM,
		CECPlayer::ACT_SWIM_FOR_MOVESKILL,
		CECPlayer::ACT_STAND,
		CECPlayer::ACT_FIGHTSTAND,
		CECPlayer::ACT_HANGINAIR,
		CECPlayer::ACT_HANGINAIR_SWORD,
		CECPlayer::ACT_HANGINWATER,
	};
	const int *begin = &s_MoveStandAction[0];
	const int *end = &s_MoveStandAction[ARRAY_SIZE(s_MoveStandAction)-1];
	return std::find(begin, end, iAction) != end;
}

//	Check water moving environment
bool CECPlayer::CheckWaterMoveEnv(const A3DVECTOR3& vPos, float fWaterHei, float fGndHei)
{
	bool bRet = false;
/*	if (vPos.y < fWaterHei - m_MoveConst.fShoreDepth)
		bRet = true;
	else if (vPos.y < fWaterHei && fWaterHei - fGndHei > m_MoveConst.fShoreDepth)
		bRet = true;
*/
//	if (vPos.y < fWaterHei - m_MoveConst.fWaterSurf - m_aabb.Extents.y)
//	if (vPos.y < fWaterHei - m_MoveConst.fWaterSurf + 0.01f)

/*
	if (vPos.y < fWaterHei - m_MoveConst.fShoreDepth)
		bRet = true;
	else if (vPos.y < fWaterHei - m_MoveConst.fWaterSurf + 0.01f && fWaterHei - fGndHei > m_MoveConst.fShoreDepth)
		bRet = true;
*/

	//@note : modify the water test. By Kuiwu[12/10/2005]
	if (vPos.y + m_aabbServer.Extents.y  <  fWaterHei - m_MoveConst.fWaterSurf - 0.001f)
	{
		bRet = true;
	}
	//else if (vPos.y + m_aabb.Extents.y < fWaterHei- m_MoveConst.fWaterSurf + 1E-4f  &&  vPos.y - fGndHei >= 0.2f )
	else if (vPos.y + m_aabbServer.Extents.y < fWaterHei- m_MoveConst.fWaterSurf + 1E-4f  &&  vPos.y - fGndHei >= 0.01f )
	{
		bRet = true;
	}

	return bRet;

}

//	Set last said words
void CECPlayer::SetLastSaidWords(const ACHAR* szWords, int nEmotionSet, CECIvtrItem *pItem)
{
}


void CECPlayer::SetFactionPVPMask(unsigned char mask)
{
	m_factionPVPMask = mask;
}

bool CECPlayer::IsInFactionPVP()const
{
	return (m_factionPVPMask & 0x01) != 0;
}

bool CECPlayer::CanAttackFactionPVPMineCar()const
{
	return (m_factionPVPMask & 0x02) != 0;
}

bool CECPlayer::CanAttackFactionPVPMineBase()const
{
	return (m_factionPVPMask & 0x04) != 0;
}

//	Set faction ID
void CECPlayer::SetFactionID(int id)
{
	m_idFaction = id; 
	if (!m_idFaction)
		return;
	//	Reset faction icon
	if (!m_pFactionDecal)
		m_pFactionDecal = new CECSpriteDecal;
}

void CECPlayer::OnFactionNameChange(){
}

//	Set faction role
void CECPlayer::SetFRoleID(int role)
{ 
	m_idFRole = role; 
}

//	On start binding buddy
void CECPlayer::OnStartBinding(int idMule, int idRider)
{
	m_bCandHangerOn = (idRider == GetCharacterID()) ? true : false;
	m_idCandBuddy = m_bCandHangerOn ? idMule : idRider;
}

CECPlayer * CECPlayer::GetBuddy()const{
	CECPlayer *result = NULL;
	return result;
}

bool CECPlayer::CanBindBuddy()const{
	return GetBuddyState() == 0
		&& !IsInvisible()
		&& GetShapeType() != PLAYERMODEL_DUMMYTYPE2
		&& (GetRace() != RACE_OBORO || GetShapeType() != PLAYERMODEL_PROFESSION);
}

//	Set booth name
void CECPlayer::SetBoothName(const ACHAR* szName)
{
	m_strBoothName = szName;
}

//	Is specified id a member of our team ?
bool CECPlayer::IsTeamMember(int idPlayer)
{
		return false;
}

//	Set new visible extend states
void CECPlayer::SetNewExtendStates(int start, const DWORD* pData, int count)
{
	if(!pData || start + count > OBJECT_EXT_STATE_COUNT)
	{
		ASSERT(false);
		a_LogOutput(1, "CECPlayer::SetNewExtendStates, invalid data(start=%d, count=%d)", start, count);
		return;
	}

	ShowExtendStates(start, pData, count);
	memcpy(m_aExtStates + start, pData, sizeof(DWORD) * count );
	if(GetExtState(111))
	{
		// 武侠旋转技能 太虚·诛神诀
		m_PlayerActions = _turning_actions;
	}
	else
	{
		m_PlayerActions = _default_actions;
	}
}

void CECPlayer::ClearShowExtendStates()
{
	DWORD aExtStates[OBJECT_EXT_STATE_COUNT] = {0};
	ShowExtendStates(0, aExtStates, OBJECT_EXT_STATE_COUNT, true);
}

void CECPlayer::ShowExtendStates(int start, const DWORD* pData, int count, bool bIgnoreOptimize/* =false */)
{
}

void CECPlayer::SetExtendStatesToWeapon()
{
	//	根据玩家的当前状态，将相关特效等挂到当前武器上（当前武器可能刚更换，挂在原武器上的状态特效因删除而消失）
	//	避免因武器更换等原因导致不能正确指示状态
	//
}

void CECPlayer::SetTeamRequireText(const TEAMREQ& Req)
{
	//	Build pate string
	CECStringTab* pFixStr = g_pGame->GetFixedMsgTab();
	m_strTeamReq = _AL("");

	if (Req.bShowLevel)
	{
		const ACHAR* szProf = g_pGame->GetGameRun()->GetProfName(Req.iProfession);
		m_strTeamReq.Format(pFixStr->GetWideString(FIXMSG_LEVELANDPROF), Req.iLevel, szProf);
		m_strTeamReq += _AL(" ");
	}

	if (!Req.iType)
		m_strTeamReq += pFixStr->GetWideString(FIXMSG_SEARCHTEAM);
	else
		m_strTeamReq += pFixStr->GetWideString(FIXMSG_SEARCHTEAMMEM);
}

void CECPlayer::SetTeamRequire(const TEAMREQ& Req, bool bConfirm)
{
}

//	Decompress advertisement data
void CECPlayer::DecompAdvData(int iData1, int iData2)
{
	TEAMREQ Req;
	Req.bShowReq	= (iData1 & (1 << 31)) ? true : false;
	Req.bShowLevel	= (iData1 & (1 << 30)) ? true : false;
	Req.iType		= (iData1 & 0x00ff0000) >> 16;
	Req.iLevel		= (iData1 & 0x0000ff00) >> 8;
	Req.iProfession	= iData1 & 0xff;

	SetTeamRequire(Req, false);
}

//	Message handlers
void CECPlayer::OnMsgPlayerExtState(const ECMSG& Msg)
{
	using namespace S2C;

	if (Msg.dwParam2 == UPDATE_EXT_STATE)
	{
		cmd_update_ext_state* pCmd = (cmd_update_ext_state*)Msg.dwParam1;
		ASSERT(pCmd && pCmd->id == m_PlayerInfo.cid);
		SetNewExtendStates(0, pCmd->states, OBJECT_EXT_STATE_COUNT);
	}
	else if (Msg.dwParam2 == ICON_STATE_NOTIFY)
	{
		cmd_icon_state_notify cmd;
		if (!cmd.Initialize((const void *)Msg.dwParam1, Msg.dwParam3))
		{
			ASSERT(0);
			return;
		}

		ASSERT(cmd.id == m_PlayerInfo.cid);
		
		m_aIconStates = cmd.states;
		if (m_aIconStates.size() > 1)
			qsort(m_aIconStates.begin(), m_aIconStates.size(), sizeof (m_aIconStates[0]), glb_IconStateCompare);
	}
}

void CECPlayer::OnMsgEnchantResult(const ECMSG& Msg)
{
}

void CECPlayer::OnMsgPlayerAdvData(const ECMSG& Msg)
{
}

void CECPlayer::OnMsgPlayerPVP(const ECMSG& Msg)
{
}

void CECPlayer::RemoveEquipGfx()
{
	//	移除显示武器、防具、套装特效
	RemoveUpperBodyStones();
	RemoveWristStones();
	RemoveLowerBodyStones();
	RemoveFootStones();
	RemoveFullSuiteGFX();
}

void CECPlayer::AddEquipGfx()
{
	//	显示武器、防具、套装特效
	if (GetMajorModel() != NULL && !InFashionMode()){
		AddUpperBodyStones();
		AddWristStones();
		AddLowerBodyStones();
		AddFootStones();
		AddFullSuiteGFX();
	}
}

void CECPlayer::OnMsgSwitchFashionMode(const ECMSG& Msg)
{	
	using namespace S2C;
	cmd_player_enable_fashion* pCmd = (cmd_player_enable_fashion*)Msg.dwParam1;
	m_bFashionMode = pCmd->is_enabble ? true : false;

	UpdateCurSkins();
	OnSwitchFashionWeapon();
	if (InFashionMode()){
		RemoveEquipGfx();
	}else{
		AddEquipGfx();
	}

	for (int i = 0; i < SIZE_ALL_EQUIPIVTR; i++)
	{
		if (m_aEquips[i] <= 0)
			continue;

		int nEquipFlag = 0;
		if (IsShownEquip(i))
		{
			nEquipFlag = 1;
		}

		if ( GetMajorModel() )
		{
			DWORD dwRealID = GetRealElementID(i, m_aEquips[i]);
			GetMajorModel()->OnScriptChangeEquip(dwRealID, nEquipFlag, false);
		}
	}

	if (InFashionMode())
		UpdateHairModel(true, m_aEquips[EQUIPIVTR_FASHION_HEAD]);
	else
		UpdateHairModel(true, m_aEquips[EQUIPIVTR_HEAD]);
}

void CECPlayer::OnMsgPlayerEffect(const ECMSG& Msg)
{
	using namespace S2C;
	
	if (Msg.dwParam2 == PLAYER_ENABLE_EFFECT)
	{
		cmd_player_enable_effect* pCmd = (cmd_player_enable_effect*)Msg.dwParam1;
		ApplyEffect(pCmd->effect, true);
	}
	else if (Msg.dwParam2 == PLAYER_DISABLE_EFFECT)
	{
		cmd_player_disable_effect* pCmd = (cmd_player_disable_effect*)Msg.dwParam1;
		DiscardEffect(pCmd->effect);
	}
}

void CECPlayer::OnMsgChangeNameColor(const ECMSG& Msg)
{
}

void CECPlayer::OnMsgPlayerMount(const ECMSG& Msg)
{
}

void CECPlayer::PlayTaoistEffect()
{
}

bool CECPlayer::CanPlayTaoistEffect(int originalLevel2, int newLevel2, bool bFirstTime)
{
	bool result(false);
	return result;
}

void CECPlayer::SetLevel2(int level2, bool bFirstTime)
{
	int lastLevel2 = m_BasicProps.iLevel2;
	m_BasicProps.iLevel2 = level2;
	if (CanPlayTaoistEffect(lastLevel2, level2, bFirstTime)){
		PlayTaoistEffect();
	}
}

bool CECPlayer::IsGodEvilConvert(int originalLevel2, int newLevel2)
{
	return false;
}

void CECPlayer::OnMsgPlayerLevel2(const ECMSG& Msg)
{
}

void CECPlayer::OnMsgKingChanged(const ECMSG &Msg)
{
}

static A3DSkinModel::LIGHTINFO MakePlayerModelLightInfoForAUI(const A3DVECTOR3 &vLightDir){	
	A3DSkinModel::LIGHTINFO result;
	
	memset(&result, 0, sizeof(result));
	
	const A3DLIGHTPARAM& lp = g_pGame->GetDirLight()->GetLightparam();
	result.colDirDiff	= lp.Diffuse;
	result.colDirSpec	= lp.Specular;
	result.vLightDir	= vLightDir;
	result.colAmbient	= A3DCOLORVALUE(0.5f, 0.5f, 0.5f, 1.0f) + g_pGame->GetA3DDevice()->GetAmbientValue();
	result.colAmbient.r = min(result.colAmbient.r, 1.0f);
	result.colAmbient.g = min(result.colAmbient.g, 1.0f);
	result.colAmbient.b = min(result.colAmbient.b, 1.0f);
	result.colAmbient.a = min(result.colAmbient.a, 1.0f);
	result.colPtDiff = A3DCOLORVALUE(0.0f, 0.0f, 0.0f, 1.0f);
	result.colPtAmb = A3DCOLORVALUE(0.0f, 0.0f, 0.0f, 1.0f);
	result.fPtRange = 0.01f;
	result.vPtAtten = A3DVECTOR3(0.0f);
	result.vPtLightPos = A3DVECTOR3(0.0f);
	result.bPtLight = true;
	
	return result;
}

// player's render for shadow call back function
bool PlayerRenderForShadow(A3DViewport * pViewport, void * pArg)
{
	CECPlayer * pPlayer = (CECPlayer *) pArg;
	g_pGame->GetA3DDevice()->SetZTestEnable(true);
	g_pGame->GetA3DDevice()->SetZWriteEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(false);
	g_pGame->GetA3DDevice()->SetAlphaFunction(A3DCMP_GREATEREQUAL);
	g_pGame->GetA3DDevice()->SetAlphaRef(84);

	CECViewport viewport;
	viewport.InitFromA3D(pViewport);

//	if (pPlayer->GetBoothState() == 2)
//		pPlayer->GetBoothModel()->RenderAtOnce(pViewport, A3DSkinModel::RAO_NOTEXTURE | A3DSkinModel::RAO_NOMATERIAL, false);
	if (pPlayer->GetPetModel())
	{
		if (pPlayer->IsShapeModelChanged())
		{
			pPlayer->GetPetModel()->GetA3DSkinModel()->RenderAtOnce(pViewport, A3DSkinModel::RAO_NOMATERIAL, false);
		}
		else
		{
			bool bShowHeadOld = pPlayer->GetPlayerModel()->IsSkinShown(SKIN_HEAD_INDEX);
			pPlayer->GetPlayerModel()->ShowSkin(SKIN_HEAD_INDEX, true);
			pPlayer->GetPetModel()->GetA3DSkinModel()->RenderAtOnce(pViewport, A3DSkinModel::RAO_NOMATERIAL, false);
			pPlayer->GetPlayerModel()->ShowSkin(SKIN_HEAD_INDEX, bShowHeadOld);
		}
	}
	else if (pPlayer->IsShapeModelChanged())
		pPlayer->GetPlayerModel()->GetA3DSkinModel()->RenderAtOnce(pViewport, A3DSkinModel::RAO_NOMATERIAL, false);
	else
	{
		bool bShowHeadOld = pPlayer->GetPlayerModel()->IsSkinShown(SKIN_HEAD_INDEX);
		pPlayer->GetPlayerModel()->ShowSkin(SKIN_HEAD_INDEX, true);
		pPlayer->GetPlayerModel()->GetA3DSkinModel()->RenderAtOnce(pViewport, A3DSkinModel::RAO_NOMATERIAL, false);
		pPlayer->GetPlayerModel()->ShowSkin(SKIN_HEAD_INDEX, bShowHeadOld);
	}

	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(false);
	g_pGame->GetA3DDevice()->SetZTestEnable(true);
	g_pGame->GetA3DDevice()->SetZWriteEnable(true);
	return true;
}

typedef abase::vector<bool>	SkinShowFlagArray;
static SkinShowFlagArray GetSkinShowFlag(A3DSkinModel *pModel){
	SkinShowFlagArray result;
	if (pModel){
		result.reserve(pModel->GetSkinNum());
		for (int i(0); i < pModel->GetSkinNum(); ++ i){
			bool bShown(false);
			if (A3DSkinModel::SKIN *pSkinItem = pModel->GetSkinItem(i)){
				bShown = pSkinItem->bRender;
			}
			result.push_back(bShown);
		}
	}
	return result;
}

static SkinShowFlagArray GetShowSomeSkinOnlyFlag(A3DSkinModel *pModel, int skinIndex){
	SkinShowFlagArray result;
	if (pModel){
		result.reserve(pModel->GetSkinNum());
		for (int i(0); i < pModel->GetSkinNum(); ++ i){
			result.push_back(i == skinIndex ? true : false);
		}
	}
	return result;
}

static SkinShowFlagArray GetShowHeadOnlyFlag(A3DSkinModel *pModel){
	return GetShowSomeSkinOnlyFlag(pModel, SKIN_HEAD_INDEX);
}

static SkinShowFlagArray GetHideAllSkinFlag(A3DSkinModel *pModel){
	return GetShowSomeSkinOnlyFlag(pModel, -1);
}

static void ShowSkin(A3DSkinModel *pModel, const SkinShowFlagArray &showSkinFlagArray){
	if (pModel){
		for (size_t i(0); i < showSkinFlagArray.size(); ++ i){
			pModel->ShowSkin(i, showSkinFlagArray[i]);
		}
	}
}

typedef abase::vector<SkinShowFlagArray> ChildModelsSkinShowFlagArray;
static ChildModelsSkinShowFlagArray GetChildModelsSkinShowFlag(A3DSkinModel *pModel){
	ChildModelsSkinShowFlagArray result;
	if (pModel){
		result.reserve(pModel->GetChildModelNum());
		for (int i(0); i < pModel->GetChildModelNum(); ++ i){
			result.push_back(GetSkinShowFlag(pModel->GetChildModel(i)));
		}
	}
	return result;
}

static ChildModelsSkinShowFlagArray GetHideChildModelsSkinShowFlag(A3DSkinModel *pModel){
	ChildModelsSkinShowFlagArray result;
	if (pModel){
		result.reserve(pModel->GetChildModelNum());
		for (int i(0); i < pModel->GetChildModelNum(); ++ i){
			result.push_back(GetHideAllSkinFlag(pModel->GetChildModel(i)));
		}
	}
	return result;
}

static void ShowChildModelsSkin(A3DSkinModel *pModel, const ChildModelsSkinShowFlagArray &childModelsShowSkinFlagArray){
	if (pModel){
		for (size_t i(0); i < childModelsShowSkinFlagArray.size(); ++ i){
			ShowSkin(pModel->GetChildModel(i), childModelsShowSkinFlagArray[i]);
		}
	}
}

void PlayerRenderPortraitNoFace(const A3DRECT &rc, DWORD param1, DWORD param2, DWORD param3)
{
	static const float	headsize[NUM_PROFESSION*NUM_GENDER] = {
		0.221f, 0.200f,	//	武侠
		0.221f, 0.200f,	//	法师
		0.221f, 0.200f,	//	巫师
		0.221f, 0.200f,	//	妖精
		0.294f, 0.200f,	//	妖兽
		0.221f, 0.200f,	//	刺客
		0.221f, 0.200f,	//	羽芒
		0.221f, 0.200f,	//	羽灵
		0.221f, 0.200f,	//	剑灵
		0.221f, 0.200f,	//	魅灵
		0.221f, 0.200f,	//	夜影
		0.221f, 0.200f,	//	月仙
	};

	static const float	pose[NUM_PROFESSION*NUM_GENDER][2] = { 
		{1.1f, 0.02f}, {0.9f, 0.04f},	//	武侠
		{1.1f, 0.02f}, {0.9f, 0.04f},	//	法师
		{0.9f, 0.02f}, {0.9f, 0.04f},	//	巫师
		{0.9f, 0.04f}, {0.8f, 0.04f},	//	妖精
		{1.3f, 0.05f}, {0.8f, 0.04f},	//	妖兽
		{0.9f, 0.02f}, {0.9f, 0.04f},	//	刺客
		{0.8f, 0.02f}, {0.6f, 0.04f},	//	羽芒
		{0.9f, 0.02f}, {0.7f, 0.04f},	//	羽灵
		{1.0f, 0.02f}, {0.8f, 0.04f},	//	剑灵
		{1.0f, 0.02f}, {0.8f, 0.04f},	//	魅灵
		{1.0f, 0.02f}, {0.8f, 0.04f},	//	夜影
		{1.0f, 0.02f}, {0.8f, 0.04f},	//	月仙
	};

	// CECFace的 "脖子对齐"骨头的初始相对矩阵，确定相机位置时会用到
	// 由于此时没有CECFace, 故写死在这里
	static const A3DMATRIX4 matNeck(
		0.0f, 0.996917f, 0.0784576f, 0.0f,
		-1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.0784576f, 0.996917f, 0.0f,
		0.0f, -0.0454048f, -0.00635493f, 1.0f
		);

	if( param1 == 0 )
		return;

	CECPlayer * pPlayer = (CECPlayer *) param1;
	float		vDeg = (int)param2 * 0.01f;
	CECModel* pMajorModel = pPlayer->GetMajorModel();
	if( !pMajorModel ) return;
	A3DSkinModel* pModel = pMajorModel->GetA3DSkinModel();
	if( !pModel ) return;
	A3DSkin* pHeadSkin = pMajorModel->GetA3DSkin(SKIN_HEAD_INDEX);
	if( !pHeadSkin ) return;

	A3DViewport * pCurViewport = g_pGame->GetA3DEngine()->GetActiveViewport();

	A3DViewport * pPortraitViewport = g_pGame->GetPortraitRender()->GetPortraitViewport();
	A3DCameraBase * pCamera = pPortraitViewport->GetCamera();

	A3DVIEWPORTPARAM viewParam = *pPortraitViewport->GetParam();
	viewParam.X = rc.left;
	viewParam.Y = rc.top;
	viewParam.Width = rc.right - rc.left;
	viewParam.Height = rc.bottom - rc.top;
	pPortraitViewport->SetParam(&viewParam);
	((A3DCamera *) pCamera)->SetProjectionParam(-1.0f, -1.0f, -1.0f, ((float)viewParam.Width) / viewParam.Height);

	A3DBone* pHeadBone = pModel->GetSkeleton()->GetBone("Bip01 Head", NULL);
	A3DMATRIX4 matHead = a3d_RotateY(vDeg) * (InverseTM(matNeck) * pHeadBone->GetAbsoluteTM());
	A3DVECTOR3 vecPos = matHead.GetRow(3);
	A3DVECTOR3 vecDir = Normalize(matHead.GetRow(2));
	A3DVECTOR3 vecUp = Normalize(matHead.GetRow(1));

	// 计算头部肖像的缩放
	float vHeadScale = 1.0f;
	int index = pPlayer->GetProfession() * NUM_GENDER + pPlayer->GetGender();
	A3DVECTOR3 vHeadTop = A3DVECTOR3(0.0f, pHeadSkin->GetInitMeshAABB().Extents.y, 0.0f) * pHeadBone->GetAbsoluteTM();
	A3DVECTOR3 vHeadBottom = A3DVECTOR3(0.0f, 0.0f, 0.0f) * pHeadBone->GetAbsoluteTM();
	float fHeadHeight = (vHeadTop - vHeadBottom).Magnitude();
	vHeadScale = fHeadHeight / headsize[index] * (pPlayer->IsChangingFace() ? 1.0f : pPlayer->GetPortraitCameraScale());

	static float s_scalePos = 1.5f;
	float *p = &s_scalePos;
	static float s_scaleUp = 1.5f;
	p = &s_scaleUp;
	pCamera->SetPos(vecPos + vecDir * (pose[index][0] * vHeadScale * s_scalePos) + vecUp * pose[index][1] * vHeadScale * s_scaleUp);
	pCamera->SetDirAndUp(-vecDir, vecUp);

	pPortraitViewport->Active();
	pPortraitViewport->ClearDevice();

	CECViewport viewport;
	viewport.InitFromA3D(pPortraitViewport);
	
	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(true);

	//	保存原有透明度（显示头像时需屏蔽透明度)
	float fTransparent = pModel->GetTransparent();

	//	重置透明度
	pModel->SetTransparent(-1.0f);
	
	//	设置专用光照，使不受场景光照影响
	pModel->SetLightInfo(MakePlayerModelLightInfoForAUI(pCamera->GetDir()));

	//	隐藏其它皮肤
	SkinShowFlagArray savedSkinShowFlag = GetSkinShowFlag(pModel);
	ChildModelsSkinShowFlagArray savedChildModelShowFlag = GetChildModelsSkinShowFlag(pModel);
	ShowSkin(pModel, GetShowHeadOnlyFlag(pModel));
	ShowChildModelsSkin(pModel, GetHideChildModelsSkinShowFlag(pModel));

	// 打坐使用特殊模型时，需要对头像显示特殊处理 by SunXuewei 2009-11-18
	if(pPlayer->IsSitting() && (pPlayer->GetRace() == RACE_GHOST || pPlayer->GetRace() == RACE_OBORO))
	{
		int iMaterialMethod = pModel->GetMaterialMethod();
		pModel->SetMaterialMethod(A3DSkinModel::MTL_ORIGINAL);
		pModel->RenderAtOnce(viewport.GetA3DViewport(), 0, false);
		pModel->SetMaterialMethod(iMaterialMethod);
	}
	else
	{
		pModel->RenderAtOnce(viewport.GetA3DViewport(), 0, false);
	}

	//	恢复隐藏的皮肤
	ShowSkin(pModel, savedSkinShowFlag);
	ShowChildModelsSkin(pModel, savedChildModelShowFlag);

	//	渲染脸部引用的GFX特效（如灵族的额饰特效）
	//	需使用正确的 alpha 模式
	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(false);
	g_pGame->GetA3DGFXExMan()->RenderAllGfx(pPortraitViewport, true);

	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(false);

	//	恢复原有透明度
	pModel->SetTransparent(fTransparent);

	pCurViewport->Active();
}

void PlayerRenderPortrait(const A3DRECT &rc, DWORD param1, DWORD param2, DWORD param3)
{
	static float	headsize[NUM_PROFESSION*NUM_GENDER] = {
		0.221f, 0.200f,	//	武侠
		0.221f, 0.200f,	//	法师
		0.221f, 0.200f,	//	巫师
		0.221f, 0.200f,	//	妖精
		0.294f, 0.200f,	//	妖兽
		0.221f, 0.200f,	//	刺客
		0.221f, 0.200f,	//	羽芒
		0.221f, 0.200f,	//	羽灵
		0.221f, 0.200f,	//	剑灵
		0.221f, 0.200f,	//	魅灵
		0.221f, 0.200f,	//	夜影
		0.221f, 0.200f,	//	月仙
	};

	static float	pose[NUM_PROFESSION*NUM_GENDER][2] = { 
		{0.7f, 0.04f}, {0.6f, 0.04f},	//	武侠
		{0.7f, 0.04f}, {0.6f, 0.04f},	//	法师
		{0.7f, 0.04f}, {0.6f, 0.04f},	//	巫师
		{0.7f, 0.04f}, {0.7f, 0.05f},	//	妖精
		{0.9f, 0.06f}, {0.6f, 0.04f},	//	妖兽
		{0.7f, 0.04f}, {0.6f, 0.04f},	//	刺客
		{0.7f, 0.04f}, {0.6f, 0.04f},	//	羽芒
		{0.7f, 0.04f}, {0.6f, 0.04f},	//	羽灵
		{0.7f, 0.04f}, {0.6f, 0.04f},	//	剑灵
		{0.7f, 0.04f}, {0.6f, 0.04f},	//	魅灵
		{0.7f, 0.04f}, {0.6f, 0.04f},	//	夜影
		{0.7f, 0.04f}, {0.6f, 0.04f},	//	月仙
	};

	if( param1 == 0 )
		return;

	CECPlayer * pPlayer = (CECPlayer *) param1;
	float		vDeg = (int)param2 * 0.01f;
	CECFace * pFace = pPlayer->GetFaceModel();
	if (!pFace)
	{
		PlayerRenderPortraitNoFace(rc, param1, param2, param3);
		return;
	}

	A3DViewport * pCurViewport = g_pGame->GetA3DEngine()->GetActiveViewport();

	A3DViewport * pPortraitViewport = g_pGame->GetPortraitRender()->GetPortraitViewport();
	A3DCameraBase * pCamera = pPortraitViewport->GetCamera();

	A3DVIEWPORTPARAM viewParam = *pPortraitViewport->GetParam();
	viewParam.X = rc.left;
	viewParam.Y = rc.top;
	viewParam.Width = rc.right - rc.left;
	viewParam.Height = rc.bottom - rc.top;
	pPortraitViewport->SetParam(&viewParam);
	((A3DCamera *) pCamera)->SetProjectionParam(-1.0f, -1.0f, -1.0f, ((float)viewParam.Width) / viewParam.Height);

	A3DMATRIX4 matHead = a3d_RotateY(vDeg) * pFace->GetFaceTM();
	A3DVECTOR3 vecPos = matHead.GetRow(3);
	A3DVECTOR3 vecDir = Normalize(matHead.GetRow(2));
	A3DVECTOR3 vecUp = Normalize(matHead.GetRow(1));

	int index = pPlayer->GetProfession() * NUM_GENDER + pPlayer->GetGender();

	float vHeadScale = 1.0f;
	A3DSkinModel * pFaceModel = pPlayer->GetFaceModel()->GetA3DSkinModel();
	A3DBone * pBoneUp = pFaceModel->GetSkeleton()->GetBone("脸盘010", NULL);
	A3DBone * pBoneLow = pFaceModel->GetSkeleton()->GetBone("脸盘120", NULL);

	float vDis = Magnitude(pBoneUp->GetAbsoluteTM().GetRow(3) - pBoneLow->GetAbsoluteTM().GetRow(3));
	vHeadScale = vDis / headsize[index] * pPlayer->GetPortraitCameraScale();
	
	pCamera->SetPos(vecPos + vecDir * (pose[index][0] * vHeadScale * 1.3f) + vecUp * pose[index][1] * vHeadScale * 1.05f);
	pCamera->SetDirAndUp(-vecDir, vecUp);
	pPortraitViewport->Active();
	pPortraitViewport->ClearDevice();

	CECViewport viewport;
	viewport.InitFromA3D(pPortraitViewport);	
	
	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(true);

	//	保存原有透明度（显示头像时需屏蔽透明度)
	float fTransparent = pFace->GetTransparent();

	//	重置透明度
	pFace->SetTransparent(-1.0f);

	A3DSkinModel::LIGHTINFO light = MakePlayerModelLightInfoForAUI(pCamera->GetDir());
	if(pPlayer->IsSitting() && (pPlayer->GetRace() == RACE_GHOST || pPlayer->GetRace() == RACE_OBORO))
	{
		// 打坐使用特殊模型时，需要对头像显示特殊处理 by SunXuewei 2009-11-18
		int iMaterialMethod = pFaceModel->GetMaterialMethod();

		pFaceModel->SetMaterialMethod(A3DSkinModel::MTL_ORIGINAL);

		pFace->Render(&viewport, true, false, &light);

		pFaceModel->SetMaterialMethod(iMaterialMethod);
	}
	else
	{
		pFace->Render(&viewport, true, false, &light);
	}
	
	//	渲染脸部引用的GFX特效（如灵族的额饰特效）
	//	需使用正确的 alpha 模式
	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(false);
	g_pGame->GetA3DGFXExMan()->RenderAllGfx(pPortraitViewport, true);

	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(false);
	
	//	恢复原有透明度
	pFace->SetTransparent(fTransparent);

	pCurViewport->Active();
}

void PlayerRenderDemonstration(const A3DRECT &rc, DWORD param1, DWORD param2, DWORD param3)
{
	static const float _dist[NUM_PROFESSION][2] =
	{
		{ -9.2f, -8.4f },	//	武侠
		{ -9.2f, -8.4f },	//	法师
		{ -9.2f, -8.4f },	//	僧侣
		{ -9.2f, -8.4f },	//	妖精
		{ -9.5f, -8.4f },	//	妖兽
		{ -9.2f, -8.4f },	//	魅灵
		{ -9.2f, -8.4f },	//	羽芒
		{ -9.2f, -8.4f },	//	羽灵
		{ -9.2f, -8.4f },	//	剑灵
		{ -9.2f, -8.4f },	//	魅灵
		{ -9.2f, -8.4f },	//	夜影
		{ -9.2f, -8.4f },	//	月仙
	};
	static const float _height[NUM_PROFESSION][2] = 
	{
		{ 0.95f, 0.85f },	//	武侠
		{ 0.95f, 0.85f },	//	法师
		{ 0.95f, 0.85f },	//	僧侣
		{ 0.95f, 0.85f },	//	妖精
		{ 1.0f, 0.85f },	//	妖兽
		{ 0.95f, 0.85f },	//	魅灵
		{ 0.95f, 0.85f },	//	羽芒
		{ 0.95f, 0.85f },	//	羽灵
		{ 0.95f, 0.85f },	//	剑灵
		{ 0.95f, 0.85f },	//	魅灵
		{ 0.95f, 0.85f },	//	夜影
		{ 0.95f, 0.85f },	//	月仙
	};
	static const float _nearest_dist = -1.f;
	static const float _x_max = 1.f;

	if( param1 == 0 )
		return;

	CECPlayer * pPlayer = (CECPlayer *) param1;
	float vDeg = DEG2RAD(param2);
	char nDist = (param3 >> 8) & 0xff;
	char nHeight = param3 & 0xff;
	char nLeftRight = (param3 >> 16) & 0xff;

	A3DViewport * pCurViewport = g_pGame->GetA3DEngine()->GetActiveViewport();

	A3DViewport * pPortraitViewport = g_pGame->GetPortraitRender()->GetPortraitViewport();
	A3DCameraBase * pCamera = pPortraitViewport->GetCamera();

	A3DRECT rcClamped = rc;
	if (rcClamped.left < (int)pCurViewport->GetParam()->X){
		rcClamped.left = (int)pCurViewport->GetParam()->X;
	}
	if (rcClamped.top < (int)pCurViewport->GetParam()->Y){
		rcClamped.top = (int)pCurViewport->GetParam()->Y;
	}
	if (rcClamped.right > (int)pCurViewport->GetParam()->X + (int)pCurViewport->GetParam()->Width){
		rcClamped.right = (int)pCurViewport->GetParam()->X + (int)pCurViewport->GetParam()->Width;
	}
	if (rcClamped.bottom > (int)pCurViewport->GetParam()->Y + (int)pCurViewport->GetParam()->Height){
		rcClamped.bottom = (int)pCurViewport->GetParam()->Y + (int)pCurViewport->GetParam()->Height;
	}

	A3DVIEWPORTPARAM viewParam = *pPortraitViewport->GetParam();
	viewParam.X = rcClamped.left;
	viewParam.Y = rcClamped.top;
	viewParam.Width = rcClamped.right - rcClamped.left;
	viewParam.Height = rcClamped.bottom - rcClamped.top;
	pPortraitViewport->SetParam(&viewParam);
	((A3DCamera *) pCamera)->SetProjectionParam(-1.0f, -1.0f, -1.0f, ((float)viewParam.Width) / viewParam.Height);

	int nGender = pPlayer->GetGender();
	int nProfession = pPlayer->GetProfession();
	float fDist = nDist / 100.f * (_nearest_dist - _dist[nProfession][nGender]) + _dist[nProfession][nGender];
	float fHeight = (nHeight + 100) / 100.f * _height[nProfession][nGender];
	float fXpos = nLeftRight / 50.f * _x_max - _x_max;
	A3DMATRIX4 mat = a3d_RotateY(vDeg);
	A3DVECTOR3 vecPos = A3DVECTOR3(fXpos, fHeight + (/*pPlayer->UsingWing()*/pPlayer->GetWingType() == WINGTYPE_WING ? 0.3f : 0.0f), fDist) * mat;
	A3DVECTOR3 vecDir = A3DVECTOR3(0, 0, 1.0f) * mat;
	A3DVECTOR3 vecUp = A3DVECTOR3(0, 1.0f, 0);

	pCamera->SetPos(vecPos);
	pCamera->SetDirAndUp(vecDir, vecUp);
	pPortraitViewport->Active();
	pPortraitViewport->ClearDevice();

	A3DSkinModel::LIGHTINFO light = MakePlayerModelLightInfoForAUI(pCamera->GetDir());
	if (pPlayer->GetPlayerModel()->GetA3DSkinModel())
		pPlayer->GetPlayerModel()->GetA3DSkinModel()->SetLightInfo(light);

	//	开始渲染

	//	设置图元收集对象
	A3DEngine* pA3DEngine = g_pGame->GetA3DEngine();
	A3DSkinRender* psr = g_pGame->GetSkinRender2();
	A3DSkinRender* psrold = pA3DEngine->GetA3DSkinMan()->GetCurSkinRender();
	pA3DEngine->GetA3DSkinMan()->SetCurSkinRender(psr);

	//	渲染 非Alpha 物体
	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(false);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaFunction(A3DCMP_GREATER);
	g_pGame->GetA3DDevice()->SetAlphaRef(84);

	//	CECModel::Render 中不更新光照参数，以免试衣模型受环境光影响
	CECModel *pPlayerModel = pPlayer->GetPlayerModel();
	bool bUpdateFlagOld = pPlayerModel->GetUpdateLightInfoFlag();
	pPlayerModel->SetUpdateLightInfoFlag(false);
	pPlayerModel->Render(pPortraitViewport);
	pPlayerModel->SetUpdateLightInfoFlag(bUpdateFlagOld);

	if (pPlayer->GetFaceModel())
	{
		CECViewport v;
		v.InitFromA3D(pPortraitViewport);
		pPlayer->GetFaceModel()->Render(&v, true, false, &light);
	}
	
	psr->Render(pPortraitViewport, false);
	psr->ResetRenderInfo(false);

	//	渲染 Alpha 物体
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(false);
	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(true);

	psr->RenderAlpha(pPortraitViewport);
	psr->ResetRenderInfo(true);

	g_pGame->GetA3DGFXExMan()->RenderAllGfx(pPortraitViewport, true);

	//	恢复原有设置
	pA3DEngine->GetA3DSkinMan()->SetCurSkinRender(psrold);
	pCurViewport->Active();
}

bool CECPlayer::HasWingModel()const
{
	return m_pPlayerModel != NULL
		&& (m_pPlayerModel->GetChildModel(_wing) || m_pPlayerModel->GetChildModel(_wing2));
}

//把比例从int 转换到float
float CECPlayer::TransformScaleFromIntToFloat(int nScale, float fScaleFactor, float fMax)
{
	if(fScaleFactor >=  1.0)
		fScaleFactor = 0.99f;

	if(fScaleFactor >=  fMax)
		fScaleFactor = fMax;

	else if ( fScaleFactor < 0.0f)
		fScaleFactor = 0.0f;

	float fScale  = (nScale - 128.0f)/128.0f;

	fScale = fScale * fScaleFactor + 1.0f;

	return  fScale;
}

void CECPlayer::InitCustomizeFactor(void)
{
	m_CustomizeFactor.fScaleHeadFactor = SCALE_HEAD_FACTOR;
	m_CustomizeFactor.fScaleUpFactor = SCALE_UP_FACTOR;
	m_CustomizeFactor.fScaleWaistFactor = SCALE_WAIST_FACTOR;
	m_CustomizeFactor.fWidthArmFactor = WIDTH_ARM_FACTOR;
	m_CustomizeFactor.fWidthLegFactor = WIDTH_LEG_FACTOR;
	m_CustomizeFactor.fScaleBreastFactor = SCALE_BREAST_FACTOR;
}

bool CECPlayer::LoadCustomizeFactorFromIni(void)
{
	AIniFile IniFile;

	if (!IniFile.Open("Configs\\CustomizeFactor.ini"))
	{
		a_LogOutput(1, "CECPlayer::LoadFaceShapeFactorFromINIFile, Failed to open file Configs\\CustomizeFactor.ini");
		return false;
	}

	AString	strSection = "Stature";
	m_CustomizeFactor.fScaleHeadFactor = IniFile.GetValueAsFloat(strSection, "HeadScaleFactor",m_CustomizeFactor.fScaleHeadFactor);
	m_CustomizeFactor.fScaleUpFactor = IniFile.GetValueAsFloat(strSection, "UpScaleFactor", m_CustomizeFactor.fScaleUpFactor);
	m_CustomizeFactor.fScaleWaistFactor = IniFile.GetValueAsFloat(strSection, "WaistScaleFactor", m_CustomizeFactor.fScaleWaistFactor);
	m_CustomizeFactor.fWidthArmFactor = IniFile.GetValueAsFloat(strSection, "ArmWidthFactor", m_CustomizeFactor.fWidthArmFactor);
	m_CustomizeFactor.fWidthLegFactor = IniFile.GetValueAsFloat(strSection, "LegWidthFactor", m_CustomizeFactor.fWidthLegFactor);
	m_CustomizeFactor.fScaleBreastFactor = IniFile.GetValueAsFloat(strSection, "BreastScaleFactor", m_CustomizeFactor.fScaleBreastFactor);
	
	IniFile.Close();

	return true;
}

//	Print bubble text
void CECPlayer::BubbleText(int iIndex, DWORD dwNum, int p1/* 0 */)
{
}

bool CECPlayer::IsClothesOn()
{
	if( m_bFashionMode )
	{
		if( m_iGender == GENDER_MALE )
		{
			if( !m_aCurSkins[SKIN_FASHION_LOWER_INDEX] )
				return false;
		}
		else
		{
			if( !m_aCurSkins[SKIN_FASHION_UPPER_BODY_INDEX] ||
				!m_aCurSkins[SKIN_FASHION_LOWER_INDEX] )
				return false;
		}
	}
	else
	{
		if( !m_aCurSkins[SKIN_UPPER_BODY_INDEX] ||
			!m_aCurSkins[SKIN_LOWER_INDEX] )
			return false;
	}

	return true;
}

bool CECPlayer::UpdateCurSkins()
{
	if (GetMajorModel())
	{
		int	i;
		for(i=0; i<NUM_SKIN_INDEX; i++)
		{
			int lodLevel = 0;
			//	first of all update lod based skin
			if( m_fDistToCamera > 20.0f )
				lodLevel = 2;
			else if( m_fDistToCamera > 10.0f )
				lodLevel = 1;
			
			if( m_bFashionMode )
			{
				if (i >= SKIN_UPPER_BODY_INDEX && i <= SKIN_FOOT_INDEX)
				{
					ReplaceCurSkin(i, NULL);
				}
				else
				{
					ReplaceCurSkin(i, m_aSkins[i][lodLevel]);
				}
			}
			else
			{
				if (i >= SKIN_FASHION_UPPER_BODY_INDEX && i <= SKIN_FASHION_FOOT_INDEX)
				{
					ReplaceCurSkin(i, NULL);
				}
				else
				{
					ReplaceCurSkin(i, m_aSkins[i][lodLevel]);
				}
			}
		}
	}

	return true;
}

//吃变形丸
bool CECPlayer::EatChgPill(const char* pszPillFile, float vCamScale)
{
// 	if(!m_ChangePill.IsEmpty())
// 		return false;
// 
// 	if(!m_ChangePill.Load(pszPillFile))
// 		return false;
// 	
// 	if( (m_ChangePill.GetCharacter() != m_iProfession) || (m_ChangePill.GetGender() != m_iGender))
// 		return false;
// 	
// 	if( m_ChangePill.IsEmpty())
// 		return false;
// 	
// 	//备份原有个性化数据
// 	BackupCustomizeData();
// 
// 	//设置变形丸数据
// 	for( int i = 0; i < m_ChangePill.GetChgPillDataCount(); i++)
// 	{
// 		SetChgPillData(m_ChangePill.GetChgPillData(i)->nKey, m_ChangePill.GetChgPillData(i)->nValue);
// 	}
// 
// 	//变形
// 	ChangeCustomizeData(m_CustomizeData);
// 	
// 	m_vPortraitCamScale = vCamScale;
// 	m_bIsChangingFace = true;
	return true;
}

//设置变形丸数据
bool CECPlayer::SetChgPillData(int nID, int nData)
{
	return false;
}

//备份个性化数据(与变形丸相关)
void CECPlayer::BackupCustomizeData(void)
{
	memcpy(&m_ChgPllCustomizeData, &m_CustomizeData, sizeof(PLAYER_CUSTOMIZEDATA));
}

//还原备份个性化数据(与变形丸相关)
bool CECPlayer::DiscardChgPill(void)
{
// 	if( m_ChangePill.IsEmpty())
// 		return false;
// 
// 	ChangeCustomizeData(m_ChgPllCustomizeData);
// 
// 	m_ChangePill.ClearChangePillData();
// 
// 	m_vPortraitCamScale = 1.0f;
// 	m_bIsChangingFace = false;
	return true;
}

//	Apply effect on player
bool CECPlayer::ApplyEffect(int iEffect, bool bCheckArray)
{
	if (bCheckArray)
	{
		//	Check whether player has had this effect
		for (int i=0; i < m_aCurEffects.GetSize(); i++)
		{
			if (m_aCurEffects[i] == iEffect)
				return true;
		}

		m_aCurEffects.UniquelyAdd(iEffect);
	}

	CECIvtrItem* pItem = CECIvtrItem::CreateItem(iEffect, 0, 1);
	if (!pItem)
		return false;

	switch (pItem->GetClassID())
	{
	case CECIvtrItem::ICID_FACEPILL:
	{
		const char* szFile = ((CECIvtrFacePill*)pItem)->GetPillFile(m_iProfession, m_iGender);
		if (szFile)
			EatChgPill(szFile, ((CECIvtrFacePill*)pItem)->GetDBEssence()->camera_scale);
		
		break;
	}
	}

	delete pItem;
	return true;
}

//	Discard effect from player
void CECPlayer::DiscardEffect(int iEffect)
{
	//	From from effect records
	for (int i=0; i < m_aCurEffects.GetSize(); i++)
	{
		if (m_aCurEffects[i] == iEffect)
		{
			m_aCurEffects.RemoveAtQuickly(i);
			break;
		}
	}

	CECIvtrItem* pItem = CECIvtrItem::CreateItem(iEffect, 0, 1);
	if (!pItem)
		return;

	switch (pItem->GetClassID())
	{
	case CECIvtrItem::ICID_FACEPILL:

		DiscardChgPill();
		break;
	}

	delete pItem;
}

//	Check whether player has effect of specified type
bool CECPlayer::HasEffectType(int iEffType)
{
	int i;

	switch (iEffType)
	{
	case EFF_FACEPILL:
	{
		elementdataman* pDataMan = g_pGame->GetElementDataMan();

		//	Get item data type
		for (i=0; i < m_aCurEffects.GetSize(); i++)
		{
			DATA_TYPE DataType = pDataMan->get_data_type(m_aCurEffects[i], ID_SPACE_ESSENCE);
			if (DataType == DT_FACEPILL_ESSENCE)
				return true;
		}

		break;
	}
	}

	return false;
}

// Play start use item action
bool CECPlayer::PlayStartUseItemAction(int tid)
{
	DATA_TYPE dt;
	const void * pDBEssence = g_pGame->GetElementDataMan()->get_data_ptr(tid, ID_SPACE_ESSENCE, dt);

	switch(dt)
	{
	case DT_MEDICINE_ESSENCE:
		break;

	case DT_TOWNSCROLL_ESSENCE:
	case DT_UNIONSCROLL_ESSENCE:
		PlayAction(ACT_USEITEM);
		PlayAction(ACT_USEITMELOOP, true, 0, true);
		break;

	case DT_REVIVESCROLL_ESSENCE:
		if( IsDead() )
			break;
		else
		{
			// play an action use revive scroll to revive somebody else
			PlayAction(ACT_USEITEM);
			PlayAction(ACT_USEITMELOOP, true, 0, true);
		}
		break;

	case DT_WAR_TANKCALLIN_ESSENCE:
		if( IsDead() )
			break;
		else
		{
			// play an action use revive scroll to revive somebody else
			PlayAction(ACT_USEITEM);
			PlayAction(ACT_USEITMELOOP, true, 0, true);
		}
		break;

	case DT_FIREWORKS_ESSENCE:
		if( IsDead() )
			break;
		else
		{
			PlayAction(ACT_EXP_ASSAULT);
		}
		break;
	}

	return true;
}
bool CECPlayer::PlayGatherMonsterSpiritAction()
{
	PlayAction(ACT_USEITEM);
	PlayAction(ACT_USEITMELOOP, true, 0, true);
	return true;
}
// Play use item effect
bool CECPlayer::PlayUseItemEffect(int tid, const void* pData, size_t sz)
{
	return true;
}

float CECPlayer::GetEquipGfxScale()
{
	float vScale = 1.0f;
	if( m_iGender == GENDER_FEMALE )
		vScale = 0.6f;
	else if( m_iProfession == PROF_ORC )
		vScale = 1.3f;
	return vScale;
}

void CECPlayer::AddUpperBodyStones()
{
	if (InFashionMode()){
		RemoveUpperBodyStones();
		return;
	}
	const WORD &stone = m_stoneUpperBody;
	WORD &stoneShown = m_stoneUpperBodyShown;
	if (stoneShown == stone){
		return;
	}
	if (stoneShown != 0){
		RemoveUpperBodyStones();
	}
	if (stone == 0){
		return;
	}
	int nEquipIndex = EQUIPIVTR_BODY;
	float vScale = GetEquipGfxScale();
	AString strGFX = GetArmorStoneGfx(stone, nEquipIndex);
	if (PlayGfx(strGFX, "HH_Spine", vScale)){
		stoneShown = stone;
	}
	strGFX = GetArmorStoneGfx(stone, nEquipIndex, 1);
	if (PlayGfx(strGFX, "HH_Shangshen02", vScale)){
		stoneShown = stone;
	}
	if (PlayGfx(strGFX, "HH_Shangshen05", vScale)){
		stoneShown = stone;
	}
}

void CECPlayer::RemoveUpperBodyStones()
{
	WORD &stoneShown = m_stoneUpperBodyShown;
	if (stoneShown == 0) return;
	int nEquipIndex = EQUIPIVTR_BODY;
	AString strGFX = GetArmorStoneGfx(stoneShown, nEquipIndex);
	RemoveGfx(strGFX, "HH_Spine");
	strGFX = GetArmorStoneGfx(stoneShown, nEquipIndex, 1);
	RemoveGfx(strGFX, "HH_Shangshen02");	
	RemoveGfx(strGFX, "HH_Shangshen05");
	stoneShown = 0;
}

void CECPlayer::AddWristStones()
{
	if (InFashionMode()){
		RemoveWristStones();
		return;
	}
	const WORD &stone = m_stoneWrist;
	WORD &stoneShown = m_stoneWristShown;
	if (stoneShown == stone){
		return;
	}
	if (stoneShown != 0){
		RemoveWristStones();
	}
	if (stone == 0){
		return;
	}
	int nEquipIndex = EQUIPIVTR_WRIST;
	float vScale = GetEquipGfxScale();
	AString strGFX = GetArmorStoneGfx(stone, nEquipIndex);
	if (PlayGfx(strGFX, "HH_shou03", vScale)){
		stoneShown = stone;
	}
	if (PlayGfx(strGFX, "HH_shou04", vScale)){
		stoneShown = stone;
	}
}

void CECPlayer::RemoveWristStones()
{
	WORD &stoneShown = m_stoneWristShown;
	if (stoneShown == 0) return;
	int nEquipIndex = EQUIPIVTR_WRIST;
	AString strGFX = GetArmorStoneGfx(stoneShown, nEquipIndex);
	RemoveGfx(strGFX, "HH_shou03");	
	RemoveGfx(strGFX, "HH_shou04");
	stoneShown = 0;
}

void CECPlayer::AddLowerBodyStones()
{
	if (InFashionMode()){
		RemoveLowerBodyStones();
		return;
	}
	const WORD &stone = m_stoneLowerBody;
	WORD &stoneShown = m_stoneLowerBodyShown;
	if (stoneShown == stone){
		return;
	}
	if (stoneShown != 0){
		RemoveLowerBodyStones();
	}
	if (stone == 0){
		return;
	}
	int nEquipIndex = EQUIPIVTR_LEG;
	float vScale = GetEquipGfxScale();
	AString strGFX = GetArmorStoneGfx(stone, nEquipIndex);
	if (PlayGfx(strGFX, "HH_xiashen02", vScale)){
		stoneShown = stone;
	}
	if (PlayGfx(strGFX, "HH_xiashen04", vScale)){
		stoneShown = stone;
	}
}

void CECPlayer::RemoveLowerBodyStones()
{
	WORD &stoneShown = m_stoneLowerBodyShown;
	if (stoneShown == 0) return;
	int nEquipIndex = EQUIPIVTR_LEG;
	AString strGFX = GetArmorStoneGfx(stoneShown, nEquipIndex);
	RemoveGfx(strGFX, "HH_xiashen02");	
	RemoveGfx(strGFX, "HH_xiashen04");
	stoneShown = 0;
}

void CECPlayer::AddFootStones()
{
	if (InFashionMode()){
		RemoveFootStones();
		return;
	}
	const WORD &stone = m_stoneFoot;
	WORD &stoneShown = m_stoneFootShown;
	if (stoneShown == stone){
		return;
	}
	if (stoneShown != 0){
		RemoveFootStones();
	}
	if (stone == 0){
		return;
	}
	int nEquipIndex = EQUIPIVTR_FOOT;
	float vScale = GetEquipGfxScale();
	AString strGFX = GetArmorStoneGfx(stone, nEquipIndex);
	if (PlayGfx(strGFX, "HH_xie02", vScale)){
		stoneShown = stone;
	}
	if (PlayGfx(strGFX, "HH_xie04", vScale)){
		stoneShown = stone;
	}
}

void CECPlayer::RemoveFootStones()
{
	WORD &stoneShown = m_stoneFootShown;
	if (stoneShown == 0) return;
	int nEquipIndex = EQUIPIVTR_FOOT;
	AString strGFX = GetArmorStoneGfx(stoneShown, nEquipIndex);
	RemoveGfx(strGFX, "HH_xie02");	
	RemoveGfx(strGFX, "HH_xie04");
	stoneShown = 0;
}

AString CECPlayer::GetSharpenerGfx(WORD status)
{
	//	计算被磨刀石磨过的装备应该显示的特效
	//	若装备未被磨过，则返回空
	//
	AString strGfxFile;

	BYTE idColor = (status>>8);	//	高8位指示磨刀石控制的选项
	if (idColor != 0)
	{
		static const char * szGFXFile[] =
		{
			"无",
			"物理攻击1级",
			"物理攻击2级",
			"物理攻击3级",
			"法术攻击1级",
			"法术攻击2级",
			"法术攻击3级",
			"生命增强1级",
			"生命增强2级",
			"生命增强3级",
			"属性增强1级",
			"属性增强2级",
			"属性增强3级",
			"特殊属性1级",
			"特殊属性2级",
			"特殊属性3级",
		};
		const int nGFXFile = sizeof(szGFXFile)/sizeof(szGFXFile[0]);
		if (idColor >= nGFXFile)
			idColor = nGFXFile-1;
		strGfxFile.Format("程序联入\\磨刀石光效\\磨刀石光效%s.gfx", szGFXFile[idColor]);
	}

	return strGfxFile;
}

AString CECPlayer::GetArmorStoneGfx(WORD status, int nEquipIndex, int nEquipParam0)
{
	//	根据防具类型、防具的GFX属性，查找对应的GFX路径
	//	status: 颜色参数
	//	nEquipIndex: 防具装备在装备包裹中的位置
	//	nEquipParam0: 附加参数
	
	AString strGfxFile = GetSharpenerGfx(status);
	
	if (strGfxFile.IsEmpty() && status != 0)
	{
		const char * szColors[] = {"无", "红色", "白色", "金色", "蓝色", "紫色", "绿色", "混合"};
		int			idLevel = 1;
		int			idColor = status & 0x00ff;
		if( idColor > 7 )
		{
			idColor %= 8;
			idLevel = 2;
		}
		switch(nEquipIndex)
		{
		case EQUIPIVTR_BODY:
			if (nEquipParam0 == 0)
			{
				strGfxFile.Format("程序联入\\装备宝石镶嵌\\%s%s%d级.gfx", szColors[idColor], "胸甲", idLevel);
			}
			else if (nEquipParam0 == 1)
			{
				strGfxFile.Format("程序联入\\装备宝石镶嵌\\%s%s%d级.gfx", szColors[idColor], "肩甲", idLevel);
			}
			break;
			
		case EQUIPIVTR_WRIST:
			strGfxFile.Format("程序联入\\装备宝石镶嵌\\%s%s%d级.gfx", szColors[idColor], "腕甲", idLevel);
			break;
			
		case EQUIPIVTR_LEG:
			strGfxFile.Format("程序联入\\装备宝石镶嵌\\%s%s%d级.gfx", szColors[idColor], "腿甲", idLevel);
			break;
			
		case EQUIPIVTR_FOOT:
			strGfxFile.Format("程序联入\\装备宝石镶嵌\\%s%s%d级.gfx", szColors[idColor], "靴甲", idLevel);
			break;
		}
	}

	return strGfxFile;
}

AString CECPlayer::GetWeaponStoneGfx(WORD status)
{
	//	查找武器对应的GFX路径
	//	status: 颜色参数
	
	AString strGfxFile = GetSharpenerGfx(status);
	
	if (strGfxFile.IsEmpty() && status != 0)
	{
		const char * szColors[] = {"无", "红", "白", "金", "蓝", "紫", "绿", "混"};
		int			idColor1 = status & 0x7;
		int			idColor2 = (status >> 3) & 0x7;
		
		if( idColor1 > idColor2 )
		{
			int temp = idColor1;
			idColor1 = idColor2;
			idColor2 = temp;
		}
		strGfxFile.Format("程序联入\\武器宝石镶嵌\\%s%s.gfx", szColors[idColor1], szColors[idColor2]);
	}

	return strGfxFile;
}

void CECPlayer::AddWeaponStones()
{
	//	向当前武器模型（非时装武器）添加宝石特效
	if (!GetPlayerModel() || IsShowFashionWeapon()){
		return;	//	没显示武器模型
	}
	if (m_stoneWeaponShown == m_stoneWeapon){
		return;	//	特效没变化
	}
	if (m_stoneWeaponShown != 0){
		RemoveWeaponStones();
	}
	if (m_stoneWeapon == 0){
		return;
	}
	AString strGFX = GetWeaponStoneGfx(m_stoneWeapon);
	if (strGFX.IsEmpty()){
		return;
	}
	float vScale = GetEquipGfxScale();
	if (CECModel* pLeftHandWeapon = GetLeftHandWeapon()){
		const char * leftGFXHookPos = GetLeftWeaponGFXHookPos(pLeftHandWeapon);
		pLeftHandWeapon->PlayGfx(strGFX, leftGFXHookPos, vScale);
		m_stoneWeaponShown = m_stoneWeapon;
	}
	if (CECModel* pRightHandWeapon = GetRightHandWeapon()){
		const char * rightGFXHookPos = GetRightWeaponGFXHookPos(pRightHandWeapon);
		pRightHandWeapon->PlayGfx(strGFX, rightGFXHookPos, vScale);
		m_stoneWeaponShown = m_stoneWeapon;
	}
}

void CECPlayer::RemoveWeaponStones()
{
	//	移除当前武器（非时装武器）上的特效（宝石镶嵌、磨刀石）
	if (m_stoneWeaponShown == 0) return;
	if (GetPlayerModel() && !IsShowFashionWeapon()){
		AString strGFX = GetWeaponStoneGfx(m_stoneWeaponShown);
		if (!strGFX.IsEmpty()){
			if (CECModel* pLeftHandWeapon = GetLeftHandWeapon()){
				const char * leftGFXHookPos = GetLeftWeaponGFXHookPos(pLeftHandWeapon);
				pLeftHandWeapon->RemoveGfx(strGFX, leftGFXHookPos);
			}
			if (CECModel* pRightHandWeapon = GetRightHandWeapon()){
				const char * rightGFXHookPos = GetRightWeaponGFXHookPos(pRightHandWeapon);
				pRightHandWeapon->RemoveGfx(strGFX, rightGFXHookPos);
			}
		}
	}
	m_stoneWeaponShown = 0;
}

void CECPlayer::AddFullSuiteGFX()
{
	if (InFashionMode()){
		RemoveFullSuiteGFX();
		return;
	}
	if (m_idFullSuiteShown == m_idFullSuite){
		return;
	}
	if (m_idFullSuiteShown != 0){
		RemoveFullSuiteGFX();
	}
	if (m_idFullSuite == 0){
		return;
	}
	DATA_TYPE dt;
	SUITE_ESSENCE * pSuite = (SUITE_ESSENCE *) g_pGame->GetElementDataMan()->get_data_ptr(m_idFullSuite, ID_SPACE_ESSENCE, dt);
	if (dt == DT_SUITE_ESSENCE && pSuite->file_gfx[0]){
		float vScale = GetEquipGfxScale();
		if (PlayGfx(pSuite->file_gfx + 4, "HH_Spine", vScale)){
			m_idFullSuiteShown = m_idFullSuite;
		}
	}
}

void CECPlayer::RemoveFullSuiteGFX()
{
	if (m_idFullSuiteShown == 0) return;
	DATA_TYPE dt;
	SUITE_ESSENCE * pSuite = (SUITE_ESSENCE *) g_pGame->GetElementDataMan()->get_data_ptr(m_idFullSuiteShown, ID_SPACE_ESSENCE, dt);
	if (dt == DT_SUITE_ESSENCE && pSuite->file_gfx[0] ){
		RemoveGfx(pSuite->file_gfx + 4, "HH_Spine");	
	}
	m_idFullSuiteShown = 0;
}

//	Set player's transparence
void CECPlayer::SetTransparent(float fTrans)
{
	if (m_pPlayerModel)
		m_pPlayerModel->SetTransparent(fTrans);

	if (m_pFaceModel)
	{
		m_pFaceModel->SetTransparent(fTrans);
		m_pFaceModel->AutoHairTrans(fTrans >= 0.0f ? false : true);
	}
}

//	Get player name color
DWORD CECPlayer::GetNameColor()
{
	DWORD dwNameCol = NAMECOL_MAUVE;

	if (IsInBattle())	//	Player is in battle
	{
		if (m_iBattleCamp == GP_BATTLE_CAMP_INVADER)
			dwNameCol = NAMECOL_BC_RED;
		else	//	(m_iBattleCamp == GP_BATTLE_CAMP_DEFENDER)
			dwNameCol = NAMECOL_BC_BLUE;
	}
	else if (IsPariah())
	{
		switch (m_byPariahLvl)
		{
		case 0:		dwNameCol = NAMECOL_RED0;	break;
		case 1:		dwNameCol = NAMECOL_RED1;	break;
		default:	dwNameCol = NAMECOL_RED2;	break;
		}
	}
	else if (IsInvader())
		dwNameCol = NAMECOL_PINK;
	else if (m_pvp.bEnable)
		dwNameCol = NAMECOL_WHITE;

	return dwNameCol;
}

//	Check whether player in a same battle camp
bool CECPlayer::InSameBattleCamp(CECPlayer* pPlayer)
{
	if (!pPlayer || m_iBattleCamp == GP_BATTLE_CAMP_NONE ||
		m_iBattleCamp != pPlayer->GetBattleCamp())
		return false;

	return true;
}

//	Check whether specified npc in a same battle camp
bool CECPlayer::InSameBattleCamp(CECNPC* pNPC)
{
	return false;
}

//	Change player's tank leader state
void CECPlayer::ChangeTankLeader(int idTank, bool bBecomeLeader)
{
	if (!IsInBattle())
		return;

	static const char* szHeadHook = "HH_Head";

	//	Get effect file name base on player's battle camp
	const char* szGFXFile;
	if (m_iBattleCamp == GP_BATTLE_CAMP_INVADER)
		szGFXFile = res_GFXFile(RES_GFX_TANKLEADER_RED);
	else
		szGFXFile = res_GFXFile(RES_GFX_TANKLEADER_BLUE);

	//	When player leave battle, idTank == 0
	if (!idTank)
	{
		//	Remove effect if there is one
		RemoveGfx(szGFXFile, szHeadHook, PLAYERMODEL_TYPEALL);
		
		m_aBattleTanks.RemoveAll(false);
		return;
	}

	if (bBecomeLeader)
	{
		//	No matter how many tanks are controlled by player, ensure only
		//	one effect is played
		if (!m_aBattleTanks.GetSize())
		{
			PlayGfx(szGFXFile, szHeadHook, 1.0, PLAYERMODEL_TYPEALL);
		}

		int iIndex = m_aBattleTanks.Find(idTank);
		if (iIndex < 0)
		{
			m_aBattleTanks.Add(idTank);

			if (IsHostPlayer())
				g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_OBTAINTANK);
		}
	}
	else
	{
		if (m_aBattleTanks.GetSize())
		{
			//	Check whether tank has been controlled by us
			int iIndex = m_aBattleTanks.Find(idTank);
			if (iIndex >= 0)
			{
				m_aBattleTanks.RemoveAtQuickly(iIndex);

				if (IsHostPlayer())
					g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_LOSETANK);
			}

			//	Remove effect only when no tank is controlled by player
			if (!m_aBattleTanks.GetSize())
			{
				RemoveGfx(szGFXFile, szHeadHook, PLAYERMODEL_TYPEALL);
			}
		}
	}
}

//	Update player's tank leader state
void CECPlayer::UpdateTankLeader()
{
}

void CECPlayer::SetSpouse(int idSpouse)
{
	 m_idSpouse = idSpouse; 
}

void CECPlayer::SetGoblinRenderCnt(int iCnt)
{
	m_GoblinRenderCnt.SetCounter(iCnt);
}

void CECPlayer::SetRenderGoblin(bool bRender)
{
	m_bRenderGoblin = bRender;
}

// Does an equipment need to show
bool CECPlayer::IsShownEquip(int nEquipIndex)
{
	if((nEquipIndex >= EQUIPIVTR_FASHION_BODY && nEquipIndex <= EQUIPIVTR_FASHION_WRIST) ||
		nEquipIndex == EQUIPIVTR_FASHION_HEAD || nEquipIndex == EQUIPIVTR_FASHION_WEAPON)
		return m_bFashionMode;
	else
		return !m_bFashionMode;
}

// Get real element id
DWORD CECPlayer::GetRealElementID(int nEquipIndex, DWORD dwEquipID)
{
	// Some equipments use first 16 bits to store color info, so we need to clear color.

//	DWORD dwRealID = dwEquipID;
//	if (nEquipIndex >= EQUIPIVTR_FASHION_BODY && nEquipIndex <= EQUIPIVTR_FASHION_WRIST)
	DWORD dwRealID = dwEquipID & 0x0000ffff;

	return dwRealID;
}

bool CECPlayer::GetWeaponMajorType(unsigned int &id_major_type){
	bool result(false);
	int idWeapon = GetWeaponID();
	if (idWeapon && idWeapon != 0xffff){
		DATA_TYPE dt;
		WEAPON_ESSENCE * pWeapon = (WEAPON_ESSENCE *) g_pGame->GetElementDataMan()->get_data_ptr(idWeapon, ID_SPACE_ESSENCE, dt);
		ASSERT(dt == DT_WEAPON_ESSENCE);
		if (dt == DT_WEAPON_ESSENCE && pWeapon){
			id_major_type = pWeapon->id_major_type;
			result = true;
		}
	}
	return result;
}

// 是否正在使用法宝类武器
bool CECPlayer::IsUsingMagicWeapon()
{
	if (GetProfession() == PROF_MONK){
		unsigned int id_major_type(0);
		if (GetWeaponMajorType(id_major_type) &&
			id_major_type == 25333){		//	法宝
			return true;
		}
	}
	return false;
}

//	是否正在使用胧族武器（胧刀类、月镰类）
bool CECPlayer::IsUsingOboroWeapon(){
	return IsUsingLongKnifeWeapon() || IsUsingSickleWeapon();
}

bool CECPlayer::IsUsingLongKnifeWeapon(){
	if (GetProfession() == PROF_YEYING || GetProfession() == PROF_YUEXIAN){
		unsigned int id_major_type(0);
		if (GetWeaponMajorType(id_major_type)){
			return id_major_type == 44878;	//	胧刀
		}
	}
	return false;
}

bool CECPlayer::IsUsingSickleWeapon(){
	if (GetProfession() == PROF_YEYING || GetProfession() == PROF_YUEXIAN){
		unsigned int id_major_type(0);
		if (GetWeaponMajorType(id_major_type)){
			return id_major_type == 44879;	//	月镰
		}
	}
	return false;
}

CECModel * CECPlayer::GetBoothModel()
{
	return m_pBoothModel;
}

CECModel * CECPlayer::LoadBoothModel(const AString path)
{
	CECModel *pBoothModel = NULL;
	
	if (af_IsFileExist(path))
	{
		// 为减少不必要的错误信息输出，仅当文件存在时尝试加载
		
		g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile(
			(const char*)glb_BoothReplaceShader, A3DSkinMan::SHADERREPLACE_USERDEFINE);
		
		pBoothModel = new CECModel;
		if (!pBoothModel || !pBoothModel->Load(path))
		{
			a_LogOutput(1, "CECPlayerMan::LoadBoothModel, Failed to load default booth models %s", (const char *)path);
			A3DRELEASE(pBoothModel);
			pBoothModel = NULL;
		}
		else
		{
			A3DSkinModel *pBoothSkinModel = pBoothModel->GetA3DSkinModel();
			if (pBoothSkinModel)
				pBoothSkinModel->EnableSpecular(true);
		}
		
		g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
	}
	
	return pBoothModel;
}

void CECPlayer::SetBoothModel(const char *idBoothModel)
{
	ClearBoothModel();
	
	// 使用默认模型
	if (!idBoothModel || !idBoothModel[0]){
		AString defModelFile = res_ModelFile(GetGender()==GENDER_MALE?RES_MOD_DEFAULT_BOOTH_M:RES_MOD_DEFAULT_BOOTH_F);
		m_pBoothModel = LoadBoothModel(defModelFile);
		return;
	}

	// 尝试直接加载（一般用于不区分男女的模型）
	if (m_pBoothModel = LoadBoothModel(idBoothModel)){
		return;
	}
	
	// 按上述路径未查找到相应路径，尝试按照性别修改路径名
	AString path = idBoothModel;
	if (path.GetLength() <= 4){
		return;
	}
	path.CutRight(4);
	path += (GetGender()==GENDER_MALE ? "男" : "女");
	path += ".ecm";
	m_pBoothModel = LoadBoothModel(path);
}

void CECPlayer::ClearBoothModel(){
	if (!m_bBoothModelLoaded){
		return;
	}
	if (m_pBoothModel){
		A3DRELEASE(m_pBoothModel);
	}
	m_iBoothModelCertificateID = -1;
	m_bBoothModelLoaded = false;
}

void CECPlayer::UpdateBoothModel(){
	if (m_iBoothState != 2){		//	不是摆摊状态不需要模型
		ClearBoothModel();
		return;
	}
	if (!ShouldUseBoothModel()){	//	不需要显示模型时不显示模型
		ClearBoothModel();
		return;
	}
	if (GetCertificateID() == m_iBoothModelCertificateID){
		return;
	}
	const char *szBoothModel = "";
	if (const SELL_CERTIFICATE_ESSENCE *pEssence = CECElementDataHelper::GetCertificate(GetCertificateID())){
		szBoothModel = pEssence->show_model;
	}
	SetBoothModel(szBoothModel);
	m_iBoothModelCertificateID = GetCertificateID();
	m_bBoothModelLoaded = true;
}

void CECPlayer::SetBoothBar(const char* szFile)
{
}

void CECPlayer::ClearBoothBar(){
	if (!m_iBoothBarCertificateID){
		return;
	}
	SetBoothBar("");
	m_iBoothBarCertificateID = -1;
}

void CECPlayer::UpdateBoothBar(){
	if (m_iBoothState != 2){
		ClearBoothBar();
		return;
	}
	if (GetCertificateID() == m_iBoothBarCertificateID){
		return;
	}
	const char *szBoothBar = "";
	if (const SELL_CERTIFICATE_ESSENCE *pEssence = CECElementDataHelper::GetCertificate(GetCertificateID())){
		if (*pEssence->name_image){
			szBoothBar = pEssence->name_image + 9;	//	跳过 surfaces/
		}
	}
	SetBoothBar(szBoothBar);
	m_iBoothBarCertificateID = GetCertificateID();
}

void CECPlayer::ReplaceCurSkin(int nSkinIndex, A3DSkin *pNewSkin)
{
	if (GetMajorModel())
	{
		if (nSkinIndex>=0 && nSkinIndex<NUM_SKIN_INDEX)
		{
			const A3DSkin *pOldSkin = GetMajorModel()->GetA3DSkin(nSkinIndex);
			if (pOldSkin != pNewSkin)
			{
				GetMajorModel()->ReplaceSkin(nSkinIndex, pNewSkin, false);
				m_aCurSkins[nSkinIndex] = pNewSkin;
			}
		}
	}
}

bool CECPlayer::IsFrog()
{
	return GetShapeType() == PLAYERMODEL_DUMMYTYPE2 &&
		   GetShapeID() == RES_MOD_MONEYFROG;
}

bool CECPlayer::SetWeaponHangerPos(WeaponHangerPosition newWeaponHangerPos)
{
	//	设置新的武器悬挂位置
	//
	bool bRet(false);

	while (true)
	{
		if (m_weaponHangerPos == newWeaponHangerPos)
		{
			bRet = true;
			break;
		}

		if (!GetPlayerModel())
			break;

		//	获取已有武器模型
		CECModel *pLeftHandWeapon = GetLeftHandWeapon();
		CECModel *pRightHandWeapon = GetRightHandWeapon();

		//	获取模型的显示状态
		bool bShowLeft = pLeftHandWeapon && pLeftHandWeapon->IsShown();
		bool bShowRight = pRightHandWeapon && pRightHandWeapon->IsShown();

		//	删除旧有模型的挂接状态，但保留模型自身
		DetachWeapon();

		//	挂接原有武器模型到新挂点
		m_weaponHangerPos = newWeaponHangerPos;
		AttachWeapon();

		//	恢复显示状态
		if (pLeftHandWeapon)
			pLeftHandWeapon->Show(bShowLeft);
		if (pRightHandWeapon)
			pRightHandWeapon->Show(bShowRight);
		
		bRet = true;
		break;
	}

	return bRet;
}

//	根据当前武器、查询 Player 模型上的挂点
//
const char * CECPlayer::GetLeftWeaponHookPos(WeaponHangerPosition p)
{
	const char *pRet = NULL;
	switch(p)
	{
	case WEAPON_HANGER_HAND:
		pRet = _hh_left_hand_weapon;
		break;

	case WEAPON_HANGER_SHOULDER:
		pRet = _hh_left_shoulder_weapon;
		break;
	}
	return pRet;
}

const char * CECPlayer::GetRightWeaponHookPos(WeaponHangerPosition p)
{
	const char *pRet = NULL;
	switch(p)
	{
	case WEAPON_HANGER_HAND:
		if (IsUsingMagicWeapon()){
			pRet = _hh_qiu;
		}else if (IsUsingSickleWeapon()){
			pRet = _hh_right_hand_nickle_weapon;
		}else{
			pRet = _hh_right_hand_weapon;
		}
		break;

	case WEAPON_HANGER_SHOULDER:
		pRet = _hh_right_shoulder_weapon;
		break;
	}
	return pRet;
}

//	根据当前武器、查询武器自身上与玩家挂点位置对接的挂点名称
//
const char * CECPlayer::GetLeftWeaponOwnHookPos(CECModel *pModel)
{
	A3DSkinModel *pSkinModel = pModel->GetA3DSkinModel();
	return pSkinModel && (pSkinModel->GetSkeletonHook(_cc_weapon, true)!=NULL) ? _cc_weapon : NULL;
}


const char * CECPlayer::GetRightWeaponOwnHookPos(CECModel *pModel)
{
	const char *pRet = NULL;

	A3DSkinModel *pSkinModel = pModel->GetA3DSkinModel();
	if (pSkinModel)
	{
		if (pSkinModel->GetSkeletonHook(_cc_weapon, true))
			pRet = _cc_weapon;
		else if (pSkinModel->GetSkeletonHook(_cc_qiu, true))
			pRet = _cc_qiu;
	}

	return pRet;
}

const char * CECPlayer::GetWeaponOwnHookPos(CECModel *pModel, bool bLeft)
{
	return bLeft
		? GetLeftWeaponOwnHookPos(pModel)
		: GetRightWeaponOwnHookPos(pModel);
}

//	获取武器上GFX的挂点
//
const char *CECPlayer::GetLeftWeaponGFXHookPos(CECModel *pModel)
{
	return GetLeftWeaponOwnHookPos(pModel);
}

const char * CECPlayer::GetRightWeaponGFXHookPos(CECModel *pModel)
{
	const char * rightHookPos = NULL;
	if (IsMagicWeapon(pModel))
	{
		//	右手持法球时，不能使用武器上的 CC_qiu 挂点位置作为特效的悬挂点（因为处在人脚下）
		rightHookPos = _hh_qiu_base;
	}
	else
	{
		rightHookPos = GetRightWeaponOwnHookPos(pModel);
	}
	return rightHookPos;
}

const char * CECPlayer::GetWeaponGFXHookPos(CECModel *pModel, bool bLeft)
{
	return bLeft
		? GetLeftWeaponGFXHookPos(pModel)
		: GetRightWeaponGFXHookPos(pModel);
}

CECModel * CECPlayer::GetLeftHandWeapon()
{
	return m_pLeftHandWeapon;
}

CECModel * CECPlayer::GetRightHandWeapon()
{
	return m_pRightHandWeapon;
}

bool CECPlayer::IsWeaponHookPos(const char *szHH, bool *pbLeft /* = NULL */, CECModel **ppWeaponModel /* = NULL */)
{
	//	判断给定的人身上的挂点是否为武器挂点，并返回对应武器模型和左右位置
	//
	bool bWeaponHook(false);
	if (!stricmp(szHH, _hh_left_hand_weapon) ||
		!stricmp(szHH, _hh_left_shoulder_weapon))
	{
		if (ppWeaponModel)
			*ppWeaponModel = GetLeftHandWeapon();
		if (pbLeft)
			*pbLeft = true;
		bWeaponHook = true;
	}
	else if (!stricmp(szHH, _hh_right_hand_weapon)
		|| !stricmp(szHH, _hh_right_shoulder_weapon)
		|| !stricmp(szHH, _hh_right_hand_nickle_weapon))
	{
		if (ppWeaponModel)
			*ppWeaponModel = GetRightHandWeapon();
		if (pbLeft)
			*pbLeft = false;
		bWeaponHook = true;
	}
	return bWeaponHook;
}


void CECPlayer::UpdateWeaponHangerPosByAction(int iAction)
{
	if (GetProfession() == PROF_JIANLING)
	{
		WeaponHangerPosition weaponPos = m_weaponHangerPos;

		int changeAction = 0;	//	修改选项含义：0 维持原样，1 修改为背在背上，2 修改为握在手上

		//	根据要播放的动作、确定武器悬挂位置如何修改
		static byte s_WeaponChangeAction[ACT_MAX] = 
		{
			1,// ACT_STAND = 0,				// 站立
			2,// ACT_FIGHTSTAND,			// 战斗站立
			0,// ACT_WALK,					// 行走
			0,// ACT_RUN,					// 奔跑
			0,// ACT_JUMP_START,			// 起跳
			
			// 5
			0,// ACT_JUMP_LOOP,				// 起跳空中循环
			0,// ACT_JUMP_LAND,				// 跳跃落地
			1,// ACT_SWIM,					// 游动
			1,// ACT_HANGINWATER,			// 水中漂浮
			0,// ACT_TAKEOFF,				// 翅膀起飞 should be ACT_TAKEOFF_WING
			
			// 10
			0,// ACT_HANGINAIR,				// 翅膀悬停 should be ACT_HANGINAIR_WING
			0,// ACT_FLY,					// 翅膀前进 should be ACT_FLY_WING
			0,// ACT_FLYDOWN,				// 翅膀高空下降 should be ACT_FLYDOWN_WING_HIGH
			0,// ACT_FLYDOWN_WING_LOW,		// 翅膀低空下降 should be ACT_FLYDOWN_WING_LOW
			0,// ACT_LANDON,				// 翅膀落地 should be ACT_LAND_WING
			
			// 15
			0,// ACT_TAKEOFF_SWORD,			// 飞剑起飞
			0,// ACT_HANGINAIR_SWORD,		// 飞剑悬停
			0,// ACT_FLY_SWORD,				// 飞剑前进
			0,// ACT_FLYDOWN_SWORD_HIGH,	// 飞剑高空下降
			0,// ACT_FLYDOWN_SWORD_LOW,		// 飞剑低空下降
			
			// 20
			0,// ACT_LANDON_SWORD, 			// 飞剑落地 
			1,// ACT_SITDOWN,				// 打坐
			1,// ACT_SITDOWN_LOOP,			// 打坐循环
			1,// ACT_STANDUP,				// 打坐站起
			0,// ACT_WOUNDED,				// 受伤
			
			// 25
			0,// ACT_GROUNDDIE,				// 陆地死亡
			0,// ACT_GROUNDDIE_LOOP,		// 死亡地面循环
			0,// ACT_WATERDIE,				// 水中死亡
			0,// ACT_WATERDIE_LOOP,			// 死亡水中循环
			0,// ACT_AIRDIE_ST,				// 空中死亡
			
			// 30
			0,// ACT_AIRDIE,				// 空中死亡下落循环
			0,// ACT_AIRDIE_ED,				// 空中死亡落地
			0,// ACT_AIRDIE_LAND_LOOP,		// 死亡落地循环
			0,// ACT_REVIVE,				// 复活
			1,// ACT_CUSTOMIZE,				// 长休闲动作
			
			// 35
			0,// ACT_STRIKEBACK,			// 被击退
			0,// ACT_STRIKEDOWN,			// 被击倒
			0,// ACT_STRIKEDOWN_LOOP,		// 被击倒倒地循环
			0,// ACT_STRIKEDOWN_STANDUP,	// 被击倒站起
			1,// ACT_PICKUP,				// 采摘
			
			// 40
			1,// ACT_PICKUP_LOOP,			// 采摘植物循环
			1,// ACT_PICKUP_STANDUP,		// 采摘站起
			0,// ACT_PICKUP_MATTER,			// 捡东西
			0,// ACT_GAPE,					// 伸懒腰
			0,// ACT_LOOKAROUND,			// 四处张望
			
			// 45
			2,// ACT_PLAYWEAPON,			// 转动兵器
			0,// ACT_EXP_WAVE,				// 招手
			0,// ACT_EXP_NOD,				// 点头
			0,// ACT_EXP_SHAKEHEAD,			// 摇头
			0,// ACT_EXP_SHRUG,				// 耸肩膀
			
			// 50
			0,// ACT_EXP_LAUGH,				// 大笑
			0,// ACT_EXP_ANGRY,				// 生气
			0,// ACT_EXP_STUN,				// 晕倒
			0,// ACT_EXP_DEPRESSED,			// 沮丧
			0,// ACT_EXP_KISSHAND,			// 飞吻
			
			// 55
			0,// ACT_EXP_SHY,				// 害羞
			0,// ACT_EXP_SALUTE,			// 抱拳
			0,// ACT_EXP_SITDOWN,			// 坐下
			0,// ACT_EXP_SITDOWN_LOOP,		// 坐下循环
			0,// ACT_EXP_SITDOWN_STANDUP,	// 坐下站起
			
			// 60
			0,// ACT_EXP_ASSAULT,			// 冲锋
			0,// ACT_EXP_THINK,				// 思考
			0,// ACT_EXP_DEFIANCE,			// 挑衅
			0,// ACT_EXP_VICTORY,			// 胜利
			0,// ACT_EXP_KISS,				// 亲吻
			
			// 65
			0,// ACT_EXP_KISS_LOOP,			// 亲吻循环
			0,// ACT_EXP_KISS_END,			// 亲吻结束
			2,// ACT_ATTACK_1,				// 普攻1
			2,// ACT_ATTACK_2,				// 普攻2
			2,// ACT_ATTACK_3,				// 普攻3
			
			// 70
			2,// ACT_ATTACK_4,				// 普攻4
			1,// ACT_ATTACK_TOSS,			// 放暗器
			0,// ACT_TRICK_RUN,				// 跑动中的花招
			0,// ACT_TRICK_JUMP,			// 跳跃中的花招
			0,// ACT_FLY_GLIDE,				// 翅膀滑翔
			
			// 75
			0,// ACT_FLY_GLIDE_SWORD,		// 飞剑滑翔
			0,// ACT_EXP_FIGHT,				// 战斗
			0,// ACT_EXP_ATTACK1,			// 攻击1
			0,// ACT_EXP_ATTACK2,			// 攻击2
			0,// ACT_EXP_ATTACK3,			// 攻击3
			
			// 80
			0,// ACT_EXP_ATTACK4,			// 攻击4
			0,// ACT_EXP_DEFENCE,			// 防御
			0,// ACT_EXP_FALL,				// 摔倒
			0,// ACT_EXP_FALLONGROUND,		// 倒地
			0,// ACT_EXP_LOOKAROUND,		// 张望
			
			// 85
			0,// ACT_EXP_DANCE,				// 舞蹈
			2,// ACT_EXP_FASHIONWEAPON		// 时装武器
			1,// ACT_USEITEM,				// 通用的使用物品动作
			1,// ACT_USEITMELOOP,			// 通用的使用物品循环动作
			0,// ACT_TWO_KISS,				// 双人亲吻
			1,// ACT_USING_TARGET_ITEM,		// 使用道具
		};

		if (iAction >= 0 && iAction < ACT_MAX)
			changeAction = s_WeaponChangeAction[iAction];

		switch(changeAction)
		{
		case 1:
			weaponPos = WEAPON_HANGER_SHOULDER;
			break;

		case 2:
			weaponPos = WEAPON_HANGER_HAND;
			break;
		}

		//	实施悬挂位置修改
		SetWeaponHangerPos(weaponPos);
	}
}

void CECPlayer::UpdatePetCureGFX(DWORD dwDeltaTime)
{
}

bool CECPlayer::IsPetCureGFX(const AString &strGFXFile)
{
	//	判断指定光效是否为宠物治疗光效
	//
	bool bRet(false);

	while (true)
	{
		AString strTitle;
		if (!af_GetFileTitle(strGFXFile, strTitle) ||
			strTitle.IsEmpty())
			break;

		const char *szTitle = (const char *)strTitle;
		if (strstr(szTitle, "电弧") != szTitle)
			break;

		//	只要是电弧光效都返回 true
		bRet = true;
		break;
	}

	return bRet;
}

bool CECPlayer::TestProcessPetCureGFX(const AString &strGFXFile, bool bLoad, int iState)
{
	bool bRet(false);

	while (true)
	{
		if (!IsPetCureGFX(strGFXFile))
			break;

		//	只要是电弧光效都返回 true
		bRet = true;

		A3DGFXExMan *pGFXExMan = g_pGame->GetA3DGFXExMan();

		if (!bLoad)
		{
			//	要消除旧有状态，判断是否加载过
			if (m_pPetCureGFXtate == iState)
			{
				//	状态正确，判断是否已加载
				if (m_pPetCureGFX)
				{
					//	曾经加载，现释放
					pGFXExMan->CacheReleasedGfx(m_pPetCureGFX);
					m_pPetCureGFX =	NULL;
					m_pPetCureGFXtate = -1;
				}
			}

			//	处理完成
			break;
		}

		//	加载新特效

		//	先释放已经加载
		if (m_pPetCureGFX)
		{
			//	释放已加载
			pGFXExMan->CacheReleasedGfx(m_pPetCureGFX);
			m_pPetCureGFX =	NULL;
			m_pPetCureGFXtate = -1;
		}
		
		//	加载新的特效
		m_pPetCureGFX = pGFXExMan->LoadGfx(g_pGame->GetA3DDevice(), strGFXFile);
		if (!m_pPetCureGFX)
			break;

		//	设置默认参数
		m_pPetCureGFX->SetScale(1.0f);

		//	初始为不可见
		m_pPetCureGFX->SetVisible(false);

		//	记录对应状态下标
		m_pPetCureGFXtate = iState;

		//	更新位置
		UpdatePetCureGFX(0);

		break;
	}

	return bRet;
}

void CECPlayer::UpdatePosRelatedGFX(DWORD dwDeltaTime)
{
	//	位置相关 GFX 的更新，需要在玩家位置计算完成后执行，以减少延迟
	//
	UpdatePetCureGFX(dwDeltaTime);
	UpdateMultiObjectEffect(dwDeltaTime);
}

void CECPlayer::RenderPetCureGFX()
{
	if (m_pPetCureGFX && m_pPetCureGFX->IsVisible())
	{
		A3DGFXExMan *pGFXExMan = g_pGame->GetA3DGFXExMan();
		pGFXExMan->RegisterGfx(m_pPetCureGFX);
	}
}

bool CECPlayer::IsMagicWeapon(CECModel *pModel)
{
	//	根据武器的挂点、判断是否为法宝类武器
	//
	bool bRet(false);
	if (pModel)
	{
		A3DSkinModel *pSkinModel = pModel->GetA3DSkinModel();
		if (pSkinModel)
		{
			//	同 GetRightWeaponOwnHookPos 保持一致
			//	武器模板上无 _cc_weapon 挂点、但有 _cc_qiu 挂点时
			//	认定为法宝武器
			//
			if (pSkinModel->GetSkeletonHook(_cc_weapon, true))
				bRet = false;
			else if (pSkinModel->GetSkeletonHook(_cc_qiu, true))
				bRet = true;
		}
	}
	return bRet;
}

AString CECPlayer::GetFashionActionName()
{
	//	查找时装装备上携带的动作名称
	//
	AString strAction;

	static int local_check_order[] = 
	{
		EQUIPIVTR_FASHION_HEAD,
		EQUIPIVTR_FASHION_BODY,
		EQUIPIVTR_FASHION_WRIST,
		EQUIPIVTR_FASHION_LEG,
		EQUIPIVTR_FASHION_FOOT,
	};

	int idEquipment(0);
	for (int i = 0; i < sizeof(local_check_order)/sizeof(local_check_order[0]); ++ i)
	{
		//	按顺序查找指定服装
		idEquipment = m_aEquips[local_check_order[i]] & 0x0000ffff;
		GetFashionActionNameByID(idEquipment, strAction);

		if (!strAction.IsEmpty())
		{
			//	发现非空时装动作，即返回
			break;
		}
	}

	return strAction;
}


void CECPlayer::SetShape(int iShape)
{
	// The shape id from the server is a 8-bit number
	// The meaning of each bit:
	// | 7  6 | 5  4  3  2  1  0 |
	// |-TYPE-|-----Model ID-----|
	int iNewShape = (iShape & 0xff); // only accept 8bit
	// 对旧的Shape数据数据进行修正	
	FixOldShapeInfo(iNewShape);
	
	// 职业变身，Model ID要经转化
	if( PLAYERMODEL_GETTYPE(iNewShape) == PLAYERMODEL_PROFESSION )
	{
		int iRealID = _GetProfessionTransformModelID(
			m_iProfession, m_iGender, PLAYERMODEL_GETID(iNewShape));
		iNewShape = 0x40 | iRealID;
	}
	// 非职业变身，Model ID为EC_Resource.h里的值，故不做处理
	else
	{

	}

	// store the original data into 8~15 bit
	m_iShape = iNewShape | ((iShape & 0xff) << 8);
}

void CECPlayer::FixOldShapeInfo(int& iShape)
{
	// 那个时候shape为0代表没有变身，非0代表职业变身
	if(iShape != 0 && PLAYERMODEL_GETTYPE(iShape) == 0)
		iShape |= 0x40;
}
unsigned char CECPlayer::GetShapeMask() const
{
	// restore the original data from 8~15 bit
	return static_cast<unsigned char>((m_iShape & 0xff00) >> 8);
}

//	Get player's current model type
int CECPlayer::GetShapeType() const
{
	return PLAYERMODEL_GETTYPE(m_iShape);
}

//	Get player's current model id
int CECPlayer::GetShapeID() const
{
	return PLAYERMODEL_GETID(m_iShape);
}

//  Play Gfx on Models
bool CECPlayer::PlayGfx(const char* szPath, const char* szHook, float fScale /*1.0f*/, unsigned int iShapeTypeMask /*(1<<PLAYERMODEL_MAJOR)*/, bool bForceNoRecord /*false*/)
{
	bool bPlayed(false);
	bool bSkipRecord = (iShapeTypeMask != PLAYERMODEL_TYPEALL) || bForceNoRecord;	
	for(int i=0;i<PLAYERMODEL_MAX;i++)
	{
		if (m_pModels[i] && (iShapeTypeMask & (1<<i)))
		{
			if (IsCurrentModel(m_pModels[i])){
				m_pModels[i]->PlayGfx(szPath, szHook, fScale);
				bPlayed = true;
			}
			if(bSkipRecord)	continue;
			A3DGFXEx* pGfx = m_pModels[i]->GetGfx(szPath, szHook);
			if (pGfx && pGfx->IsInfinite()){
				GFXRECORD rec;
				rec.strPath = szPath;
				rec.strHook = szHook;
				rec.fScale = fScale;
				AString key = rec.strPath + rec.strHook;
				m_GfxRecords[key] = rec;
				bSkipRecord = true;
			}
		}
	}
	return bPlayed;
}

void CECPlayer::RemoveGfx(const char* szPath, const char* szHook, unsigned int iShapeTypeMask /*(1<<PLAYERMODEL_MAJOR)*/)
{
	bool bSkipRecord = false;
	
	for(int i=0;i<PLAYERMODEL_MAX;i++)
	{
		if( m_pModels[i] && (iShapeTypeMask & (1<<i)) )
		{
			m_pModels[i]->RemoveGfx(szPath, szHook);
			
			// remove the gfx info when it is unnecessary
			if(!bSkipRecord)
			{
				bSkipRecord = true;
				AString key = AString(szPath) + szHook;
				m_GfxRecords.erase(key);
			}
		}
	}
}

bool CECPlayer::IsShapeModelChanged() const
{ 
	return m_pModels[PLAYERMODEL_MAJOR] != m_pPlayerModel;
}

bool CECPlayer::IsShapeModelReady() const
{	
	if(GetShapeID() == 0)
	{
		// logic transform only, no model changed
		return m_pPlayerModel == m_pModels[PLAYERMODEL_MAJOR];
	}
	else
	{
		// whether the target model is set to current using model
		return m_pPlayerModel == m_pModels[min(PLAYERMODEL_MAX-1, GetShapeType())];
	}
}

bool CECPlayer::ShouldUseGroundNormalForCurrentShapeModel()const{
	return IsShapeModelChanged() && !IsTransofrmModelLikeHuman(GetProfession(), GetGender(), GetShapeID());
}

bool CECPlayer::GetExtState(int n)
{
	const int bitSize = sizeof(DWORD) * 8;
	const int totalBitSize = bitSize * OBJECT_EXT_STATE_COUNT;
	if(n < 0 || n >= totalBitSize)
		return false;

	int index = n / bitSize;
	int bitOffset = n % bitSize;
	return (m_aExtStates[index] & (1 << bitOffset)) ? true : false;
}

bool CECPlayer::StartAdjustTransparency(float fCur, float fDest, DWORD dwTime)
{
	// use current value for starting
	if(fCur < 0.f) fCur = m_fCurTrans;
	if(fDest < 0.f) fDest = 0.f;

	// ignore the invalid params
	if(dwTime == 0 || fabs(fDest - m_fDstTrans) < 0.0001f || fabs(fDest - fCur) < 0.0001f)
	{
		return false;
	}

	// start the counter to change the transparency
	m_fCurTrans		= fCur;
	m_fDstTrans		= fDest;
	m_fTransDelta	= (fDest - m_fCurTrans) / dwTime;

	m_TransCnt.SetPeriod(dwTime);
	m_TransCnt.Reset();

	return true;
}

float CECPlayer::UpdateTransparency(DWORD dwDeltaTime)
{
	//	Use this valid to change model's transparency
	if (!m_TransCnt.IsFull() && m_fDstTrans != m_fCurTrans)
	{
		m_fCurTrans += m_fTransDelta * dwDeltaTime;
		if (m_fTransDelta > 0.0f)
			a_ClampRoof(m_fCurTrans, m_fDstTrans);
		else
			a_ClampFloor(m_fCurTrans, m_fDstTrans);
	}

	return (m_fCurTrans <= 0.f) ? -1.0f : m_fCurTrans;
}

float CECPlayer::GetTransparentLimit()
{
	if(GetExtState(109))//无敌状态的光效ID
	{
		return 0.7f;//无敌
	}
	else if (m_dwStates & GP_STATE_INVISIBLE)
	{
		return 0.7f;//隐身
	}
	else if (!IsSelectable())
	{
		return 0.5f;//无法选中
	}
	else if(GetExtState(110))//雷神变状态光效ID
	{
		return 0.3f;//雷神变
	}

	return -1.f;
}

void CECPlayer::AddMultiObjectEffect(int idTarget,char cType)
{
	if (cType < 0 || cType > 2)
		return;

	static const char* szBasePath = "策划联入\\状态效果\\";

	MULTIOBJECT_EFFECT mo;
	mo.cType = cType;
	mo.iTarget = idTarget;

	MOEffectMAP::iterator it = m_mapMOEffect.find(mo);
	if (it == m_mapMOEffect.end())
	{
		AString strGfxFile(_multiobject_effect[cType]);
		strGfxFile = szBasePath + strGfxFile;
		A3DGFXExMan *pGFXExMan = g_pGame->GetA3DGFXExMan();
		A3DGFXEx* pGfx = pGFXExMan->LoadGfx(g_pGame->GetA3DDevice(), strGfxFile);
		if (!pGfx)
			return;
		pGfx->SetScale(1.0f);
		pGfx->SetVisible(false);

		if (IsAllResReady() && m_pPlayerModel)
			UpdateOneMultiObjectEffect(idTarget,pGfx,0);

		m_mapMOEffect[mo] = pGfx;
	}
}

void CECPlayer::RemoveMultiObjectEffect(int idTarget,char cType)
{
	MULTIOBJECT_EFFECT mo;
	mo.cType = cType;
	mo.iTarget = idTarget;

	MOEffectMAP::iterator it = m_mapMOEffect.find(mo);
	if (it != m_mapMOEffect.end())
	{
		A3DGFXExMan *pGFXExMan = g_pGame->GetA3DGFXExMan();
		pGFXExMan->CacheReleasedGfx(it->second);

		m_mapMOEffect.erase(mo);
	}
}

void CECPlayer::UpdateOneMultiObjectEffect(int idTarget,A3DGFXEx* pGfx,DWORD dwDeltaTime)
{
}

void CECPlayer::UpdateMultiObjectEffect(DWORD dwDeltaTime)
{
	for (MOEffectMAP::iterator it = m_mapMOEffect.begin();it != m_mapMOEffect.end();++it)
	{
		UpdateOneMultiObjectEffect(it->first.iTarget,it->second,dwDeltaTime);
	}
}

void CECPlayer::RenderMultiObjectGFX()
{
	A3DGFXExMan *pGFXExMan = g_pGame->GetA3DGFXExMan();
	
	for (MOEffectMAP::iterator it = m_mapMOEffect.begin();it != m_mapMOEffect.end();++it)
	{
		if (it->second && it->second->IsVisible())
		{
			pGFXExMan->RegisterGfx(it->second);
		}
	}
}

void CECPlayer::ClearBubbleText()
{
	if (m_pBubbleTexts)
		m_pBubbleTexts->ClearDecalList();
}

A3DAABB CECPlayer::GetShadowAABB()
{
	A3DAABB shadowAABB;

	shadowAABB.Clear();

	shadowAABB.Merge(m_aabb);

	if (IsRidingOnPet() && m_pPetModel && m_pPetModel->GetA3DSkinModel())
		shadowAABB.Merge(m_pPetModel->GetModelAABB());
	else if (GetPlayerModel() && GetPlayerModel()->GetA3DSkinModel())
		shadowAABB.Merge(GetPlayerModel()->GetModelAABB());

	return shadowAABB;
}

void CECPlayer::SetCountry(int id)
{
}

bool CECPlayer::GetShowName()const
{
	//	正常显示人物名称时返回 true
	return true;
}

bool CECPlayer::GetShowNameInCountryWar()
{
	//	需要以国战特有的方式显示名称时返回 true
	return !GetShowName() && GetCountry();
}

ACString CECPlayer::GetNameInCountryWar()
{
	//	以国家和职业代替角色名
	ACString strName;
	return strName;
}

const FASHION_WEAPON_CONFIG* CECPlayer::GetFashionConfig()
{
	static FASHION_WEAPON_CONFIG* pFashionConfig = NULL;
	if (NULL == pFashionConfig)
	{
		elementdataman *pDataMan = g_pGame->GetElementDataMan();
		DATA_TYPE DataType;
		unsigned int tid = pDataMan->get_first_data_id(ID_SPACE_CONFIG,DataType);
		
		while(tid)
		{
			if(DataType == DT_FASHION_WEAPON_CONFIG)
			{
				pFashionConfig = (FASHION_WEAPON_CONFIG *)pDataMan->get_data_ptr(tid,ID_SPACE_CONFIG, DataType);
				break;
			}
			tid = pDataMan->get_next_data_id(ID_SPACE_CONFIG, DataType);
		}
	}
	return pFashionConfig;
}
bool CECPlayer::IsFashionWeaponTypeFit(int weapon_type, int fashion_weapon_type)
{
	if (fashion_weapon_type < 0 || fashion_weapon_type >= NUM_WEAPON_TYPE) return false;
	const FASHION_WEAPON_CONFIG* pConfig = GetFashionConfig();
	if (NULL == pConfig) 
	{
		ASSERT(!"Fashion weapon config load failed!");
		a_LogOutput(1, "CECPlayer::GetFashionConfig, Failed to load fashion weapon config");
		return false;
	}
	int fashion_weapon_mask = pConfig->action_mask[fashion_weapon_type];
	return (fashion_weapon_mask & (1 << GetWeaponType(weapon_type))) != 0;
}
bool CECPlayer::CanShowFashionWeapon(int weapon_type, int fashion_weapon_type)
{
	return IsFashionWeaponTypeFit(weapon_type, fashion_weapon_type) && InFashionMode();
}

#define CLEAR_WEAPON(index) \
{ \
	Mask &= ~(1 << (__int64)index); \
	pEquipmentID[index] = -1; \
}

#define SET_WEAPON(index, id) \
{ \
	Mask |= (1 << (__int64)index); \
	pEquipmentID[index] = id; \
}

#define RECORD_NORMAL_WEAPON(file_model_left, file_model_right, action_type) \
{ \
	m_strLeftWeapon = file_model_left; \
	m_strRightWeapon = file_model_right; \
	m_uAttackType = action_type; \
}

#define RECORD_FASHION_WEAPON(file_model_left, file_model_right, index) \
{ \
	m_strLeftFashionWeapon = file_model_left; \
	m_strRightFashionWeapon = file_model_right; \
	m_iFashionWeaponType = index; \
}

void CECPlayer::DecideWeaponLoad(int* pEquipmentID, __int64& Mask)
{
	bool bNormalWeaponChanged = ((1 << EQUIPIVTR_WEAPON) & Mask) != 0;
	bool bFashionWeaponChanged = ((1 << EQUIPIVTR_FASHION_WEAPON) & Mask) != 0;
	const WEAPON_ESSENCE* pWeapon = NULL;
	const FASHION_ESSENCE* pFashion = NULL;
	const WEAPON_SUB_TYPE* pSubType = NULL;
	int Index[2] = {EQUIPIVTR_WEAPON, EQUIPIVTR_FASHION_WEAPON};
	for (int i = 0; i < 2; ++i)
	{
		int idEquipment = pEquipmentID[Index[i]];
		idEquipment &= 0x0000ffff;
		DATA_TYPE	dt;
		if (idEquipment)
		{
			const void* pEquip = g_pGame->GetElementDataMan()->get_data_ptr(
				idEquipment,
				ID_SPACE_ESSENCE,
				dt);
			if (dt == DT_WEAPON_ESSENCE)
			{
				pWeapon = (const WEAPON_ESSENCE*)pEquip;
				pSubType =(const WEAPON_SUB_TYPE*) g_pGame->GetElementDataMan()->get_data_ptr(
					pWeapon->id_sub_type,
					ID_SPACE_ESSENCE,
					dt);
				if (dt != DT_WEAPON_SUB_TYPE) pSubType = NULL;
			}
			else if (dt == DT_FASHION_ESSENCE) pFashion = (const FASHION_ESSENCE*)pEquip;
		}
	}

	bool bCanShowFashionWeaponBefore = CanShowFashionWeapon(m_uAttackType, m_iFashionWeaponType);
	bool bCanShowFashionWeapon(false);
	// 普通武器改变
	if (bNormalWeaponChanged)
	{
		// 时装武器改变
		if (bFashionWeaponChanged)
		{
			// 装备时装武器和普通武器
			if (pSubType && pFashion)
			{
				if (CanShowFashionWeapon(pSubType->action_type, pFashion->action_type))
				{
					CLEAR_WEAPON(EQUIPIVTR_WEAPON);
				}
				else
				{
					CLEAR_WEAPON(EQUIPIVTR_FASHION_WEAPON);
				}
			}
			// 装备普通武器
			else if(pSubType && !pFashion)
			{

			}
			// 装备时装武器
			else if (!pSubType && pFashion)
			{
				if (CanShowFashionWeapon(m_uAttackType, pFashion->action_type))
				{
					CLEAR_WEAPON(EQUIPIVTR_WEAPON);
				}
				else
				{
					CLEAR_WEAPON(EQUIPIVTR_FASHION_WEAPON);
				}
			}
			// 没有装备任务武器
			else
			{

			}
			if (pSubType) RECORD_NORMAL_WEAPON(
				pWeapon->file_model_left, pWeapon->file_model_right, pSubType->action_type);
			if (pFashion) RECORD_FASHION_WEAPON(
				pFashion->file_model_left, pFashion->file_model_right, pFashion->action_type);
		}
		else
		{
			// 装备普通武器
			if (pSubType)
			{
				bCanShowFashionWeapon = CanShowFashionWeapon(pSubType->action_type, m_iFashionWeaponType);
				if (bCanShowFashionWeapon)
				{
					CLEAR_WEAPON(EQUIPIVTR_WEAPON);
					if (!bCanShowFashionWeaponBefore)
						SET_WEAPON(EQUIPIVTR_FASHION_WEAPON, m_aEquips[EQUIPIVTR_FASHION_WEAPON]);
				}
				RECORD_NORMAL_WEAPON( 
					pWeapon->file_model_left, pWeapon->file_model_right, pSubType->action_type);
			}
			// 脱下普通武器
			else 
			{
				bCanShowFashionWeapon = CanShowFashionWeapon(DEFAULT_ACTION_TYPE, m_iFashionWeaponType);
				if (bCanShowFashionWeapon)
				{
					CLEAR_WEAPON(EQUIPIVTR_WEAPON);
					if (!bCanShowFashionWeaponBefore)
						SET_WEAPON(EQUIPIVTR_FASHION_WEAPON, m_aEquips[EQUIPIVTR_FASHION_WEAPON]);
				}
				RECORD_NORMAL_WEAPON((char*)NULL, (char*)NULL, DEFAULT_ACTION_TYPE);
			}
		}
	}
	else if (bFashionWeaponChanged)
	{
		// 装备时装武器
		if (pFashion)
		{
			bCanShowFashionWeapon = CanShowFashionWeapon(m_uAttackType, pFashion->action_type);
			if (!bCanShowFashionWeapon)
			{
				CLEAR_WEAPON(EQUIPIVTR_FASHION_WEAPON);
				if (bCanShowFashionWeaponBefore)
					SET_WEAPON(EQUIPIVTR_WEAPON, m_aEquips[EQUIPIVTR_WEAPON]);
			}
			// 能显示时装武器，此处要把m_stoneWeapon清零
			else CLEAR_WEAPON(EQUIPIVTR_WEAPON);
			RECORD_FASHION_WEAPON( 
				pFashion->file_model_left, pFashion->file_model_right, pFashion->action_type);
		}
		// 脱下时装武器
		else
		{
			if (0 != m_aEquips[EQUIPIVTR_WEAPON])
			{
				CLEAR_WEAPON(EQUIPIVTR_FASHION_WEAPON);
				if (bCanShowFashionWeaponBefore)
					SET_WEAPON(EQUIPIVTR_WEAPON, m_aEquips[EQUIPIVTR_WEAPON]);
			}
			RECORD_FASHION_WEAPON((char*)NULL, (char*)NULL, DEFAULT_ACTION_TYPE);
		}
	}
}

void CECPlayer::SetWeaponResult(EquipsLoadResult& Result)
{
	ReleaseWeapon();
	m_weaponHangerPos = WEAPON_HANGER_HAND;
	if (GetProfession() == PROF_JIANLING && !IsFighting()){
		//	剑灵非战斗状态下，武器挂在肩膀上
		m_weaponHangerPos = WEAPON_HANGER_SHOULDER;
	}
	m_pLeftHandWeapon = Result.pLeftHandWeapon;
	m_pRightHandWeapon = Result.pRightHandWeapon;	
	AttachWeapon();	
	if (Result.bFashionWeaponChanged){
		if (m_pLeftHandWeapon){
			m_pLeftHandWeapon->GetA3DSkinModel()->SetAlphaSortID(m_PlayerInfo.cid);
			m_pLeftHandWeapon->GetA3DSkinModel()->SetAlphaSortWeight(SKIN_SORT_WEAPON);
		}		
		if (m_pRightHandWeapon){
			m_pRightHandWeapon->GetA3DSkinModel()->SetAlphaSortID(m_PlayerInfo.cid);
			m_pRightHandWeapon->GetA3DSkinModel()->SetAlphaSortWeight(SKIN_SORT_WEAPON);
		}
	}
	if (m_pLeftHandWeapon || m_pRightHandWeapon){
		//	有武器存在，将当前还存在的状态特效，补充挂到新的武器上
		//	避免因武器更换等原因导致不能正确指示状态
		SetExtendStatesToWeapon();
	}
	
	// now show weapon according to old weapon states.
	ShowWeapon(m_bShowWeapon);
}

void CECPlayer::OnSwitchFashionWeapon()
{
	if (!GetMajorModel() || !IsFashionWeaponTypeFit(m_uAttackType, m_iFashionWeaponType))
		return;
	
	EquipsLoadResult Result;
	memset(&Result, 0, sizeof(Result));
	if (InFashionMode())
	{
		ChangeWeapon(&Result, m_strLeftFashionWeapon, m_strRightFashionWeapon);
	}
	else
	{
		ChangeWeapon(&Result, m_strLeftWeapon, m_strRightWeapon);
	}
	SetWeaponResult(Result);	
	m_stoneWeaponShown = 0;
	if (InFashionMode()){
		m_stoneWeapon = 0;
	}else{
		m_stoneWeapon = m_aEquips[EQUIPIVTR_WEAPON] >> 16;
		AddWeaponStones();
	}
}

int CECPlayer::GetShowingWeaponType()
{
	int weapon_type = 0;
	if (CanShowFashionWeapon(m_uAttackType, m_iFashionWeaponType) && m_aEquips[EQUIPIVTR_FASHION_WEAPON] != 0)
	{
		weapon_type = (m_iFashionWeaponType == DEFAULT_ACTION_TYPE || !IsWeaponAttached()) ? 
			10 : m_iFashionWeaponType;
	}
	else
	{
		weapon_type = (m_uAttackType == DEFAULT_ACTION_TYPE || !IsWeaponAttached()) ? 
			10 : m_uAttackType;		
	}
	return weapon_type;
}

// 更新经脉属性
void CECPlayer::SetMeridiansProp(const MeridiansProp& meridianProp)
{
	m_meridiansProp = meridianProp;
}

bool CECPlayer::IsShowFashionWeapon()
{
	//	当前状态是否应该显示时装武器模型，返回 true 不保证模型已经加载完成并显示
	//
	return  m_aEquips[EQUIPIVTR_FASHION_WEAPON] != 0
		&& CanShowFashionWeapon(m_uAttackType, m_iFashionWeaponType);
}

void CECPlayer::OptimizeShowExtendStates()
{
}

void CECPlayer::OptimizeWeaponStoneGfx()
{
}

void CECPlayer::OptimizeArmorStoneGfx()
{
}

void CECPlayer::OptimizeSuiteGfx()
{
}
const TITLE_CONFIG* CECPlayer::GetTitleConfig(unsigned short id)
{
	DATA_TYPE DataType = DT_INVALID;	
	const void* pDataPtr = g_pGame->GetElementDataMan()->get_data_ptr(id, ID_SPACE_CONFIG, DataType);
	if (DataType != DT_TITLE_CONFIG && DataType != DT_COMPLEX_TITLE_CONFIG)
		return NULL;
	
	const TITLE_CONFIG* pData = (const TITLE_CONFIG*)pDataPtr;
	return pData;

}
A3DBone* CECPlayer::ScaleRootBone(CECModel* pModel, float scale)
{
	int num_bone, j, root_index;
	A3DBone* pBone = NULL;
	A3DSkeleton* pSkeleton = NULL;
	if (pModel) {
		pSkeleton = pModel->GetA3DSkinModel()->GetSkeleton();
		if (pSkeleton) {
			num_bone = pSkeleton->GetRootBoneNum();
			for (j = 0; j < num_bone; ++j) {
				root_index = pSkeleton->GetRootBone(j);
				pBone = pSkeleton->GetBone(root_index);
				if (pBone) {
					pBone->SetScaleFactor(A3DVECTOR3(scale, scale, scale));
					pBone->SetScaleType(A3DBone::SCALE_WHOLE);
					break;
				}
			}	
		}
	}
	return pBone;
}
void CECPlayer::ScaleChildModel()
{
	CECModel* pModel;
	int num_child = m_pPlayerModel->GetChildCount();
	int i;
	AString strCCFeijian("CC_feijian");
	for (i = 0; i < num_child; ++i) {
		pModel = m_pPlayerModel->GetChildModel(i);
		// 飞剑的CC_feijian挂点没有挂在骨骼上，跳过飞剑
		if (pModel->GetCCName() == strCCFeijian)
			continue;
		ScaleRootBone(pModel, m_fScaleBySkill);
		pModel->SetGfxScale(m_fScaleBySkill);
	}
}
void CECPlayer::ScaleBody(float fScale)
{
	m_fScaleBySkill = fScale;
	if (!m_pPlayerModel) return;
	A3DSkeleton * pSkeleton = m_pPlayerModel->GetA3DSkinModel()->GetSkeleton();
	
	// 缩放root bone
	A3DBone* pRootBone = ScaleRootBone(m_pPlayerModel, fScale);
	
	// 脚底位移修正
	if (pRootBone) {
		float offset = pRootBone->GetOriginalMatrix().GetRow(3).y * (1.f - fScale);	
		pSkeleton->SetFootOffset(offset);
	}
	// 缩放武器、飞行器
	ScaleChildModel();

	// 缩放gfx
	m_pPlayerModel->SetGfxScale(fScale);
	// 缩放包围盒
	CalcPlayerAABB();
	A3DVECTOR3 vPos = GetPos();
	m_aabb.Center = vPos + A3DVECTOR3(0.0f, m_aabb.Extents.y, 0.0f);
	m_aabb.CompleteMinsMaxs();
	m_aabbServer.Center = vPos + A3DVECTOR3(0.0f, m_aabbServer.Extents.y, 0.0f);
	m_aabbServer.CompleteMinsMaxs();
}
void CECPlayer::LoadMonsterSpiritGFX(int index, int res_index)
{
	int count = sizeof(m_pMonsterSpiritGFX) / sizeof(m_pMonsterSpiritGFX[0]);
	if (index >= 0 && index < count) {
		A3DGFXExMan *pGFXExMan = g_pGame->GetA3DGFXExMan();
		if (m_pMonsterSpiritGFX[index] == NULL) {
			m_pMonsterSpiritGFX[index] = pGFXExMan->LoadGfx(g_pGame->GetA3DDevice(), res_GFXFile(res_index));
			if (!m_pMonsterSpiritGFX[index]) return;
			m_pMonsterSpiritGFX[index]->SetScale(1.0f);
			m_pMonsterSpiritGFX[index]->Start(true);
		}
	}
}
void CECPlayer::StartMonsterSpiritConnectGfx(int mine_id, A3DVECTOR3 pos)
{
	LoadMonsterSpiritGFX(0, RES_GFX_MONSTER_SPIRIT_LINE);
	m_iMonsterSpiritMineID = mine_id;
	m_posMonsterSpirit = pos;
}
void CECPlayer::StopMonsterSpiritConnectGfx()
{
	A3DGFXExMan *pGFXExMan = g_pGame->GetA3DGFXExMan();
	if (pGFXExMan) pGFXExMan->CacheReleasedGfx(m_pMonsterSpiritGFX[0]);
	m_pMonsterSpiritGFX[0] = NULL;	
	m_iMonsterSpiritMineID = 0;
}
void CECPlayer::StartMonsterSpiritBallGfx()
{
	LoadMonsterSpiritGFX(1, RES_GFX_MONSTER_SPIRIT_BALL);
	if (m_pMonsterSpiritGFX[1]) {
		m_pMonsterSpiritGFX[1]->SetPos(m_posMonsterSpirit);
		m_stateMonsterSpirit = BALL_STATE_RISING;
	}
}
void CECPlayer::UpdateMonsterSpiritGfx(DWORD dwDeltaTime)
{
	UpdateOneMultiObjectEffect(m_iMonsterSpiritMineID, m_pMonsterSpiritGFX[0], dwDeltaTime);
	if (m_stateMonsterSpirit == BALL_STATE_NONE) {}
	else if (m_stateMonsterSpirit == BALL_STATE_DISAPPER) {
		A3DGFXExMan *pGFXExMan = g_pGame->GetA3DGFXExMan();
		if (pGFXExMan) pGFXExMan->CacheReleasedGfx(m_pMonsterSpiritGFX[1]);
		m_pMonsterSpiritGFX[1] = NULL;	
		m_stateMonsterSpirit = BALL_STATE_NONE;
	} else {
		A3DVECTOR3 targetPos = GetPos();
		if (m_pPlayerModel){
			int index;
			if (A3DBone *pBone = m_pPlayerModel->GetA3DSkinModel()->GetSkeleton()->GetBone("Bip01 Head", &index)){
				targetPos = pBone->GetAbsoluteTM().GetRow(3);
			}
		}
		A3DVECTOR3 pos;
		if (m_stateMonsterSpirit == BALL_STATE_RISING) {
			float deltaY = 1.f * dwDeltaTime / 1000;
			pos = m_pMonsterSpiritGFX[1]->GetPos();
			pos.y += deltaY;
			m_pMonsterSpiritGFX[1]->SetPos(pos);
			if (pos.y > targetPos.y) m_stateMonsterSpirit = BALL_STATE_FOLLOW;
		} else if (m_stateMonsterSpirit == BALL_STATE_FOLLOW) {
			pos = m_pMonsterSpiritGFX[1]->GetPos();
			A3DVECTOR3 direction = targetPos - pos;
			if (direction.Magnitude() < 1.f) m_stateMonsterSpirit = BALL_STATE_DISAPPER;
			direction.Normalize();
			pos += (direction * (10.f * dwDeltaTime / 1000));
			m_pMonsterSpiritGFX[1]->SetPos(pos);
		}
		m_pMonsterSpiritGFX[1]->TickAnimation(dwDeltaTime);
	}
}

void CECPlayer::RenderMonsterSpiritGfx()
{
	A3DGFXExMan *pGFXExMan = g_pGame->GetA3DGFXExMan();
	unsigned int count = sizeof(m_pMonsterSpiritGFX) / sizeof(m_pMonsterSpiritGFX[0]);	
	for (unsigned int i = 0; i < count; ++i)
	{
		if (m_pMonsterSpiritGFX[i] && m_pMonsterSpiritGFX[i]->IsVisible())
			pGFXExMan->RegisterGfx(m_pMonsterSpiritGFX[i]);
	}	
}
int CECPlayer::HasSkillStateForAction()
{
	for (int i=0;i<sizeof(m_sciStateIDForStateAction)/sizeof(m_sciStateIDForStateAction[0]);i++)
	{
		if (GetExtState(m_sciStateIDForStateAction[i]))
			return m_sciStateIDForStateAction[i];
	}
	return 0;
}
void CECPlayer::PlayEnterBattleGfx()
{
	if (m_ReincarnationCount) {
		AString strGfx;
		strGfx.Format(res_GFXFile(RES_GFX_BATTLEFURY), m_ReincarnationCount);
		PlayGfx(strGfx, NULL, 1.0, PLAYERMODEL_TYPEALL);
	}
}
bool CECPlayer::IsInChariot()
{
	return false;
}

bool CECPlayer::ChangeFashionColor(int equipSlot, unsigned short newColor){	
	if (EQUIPIVTR_FASHION_BODY != equipSlot &&
		EQUIPIVTR_FASHION_LEG != equipSlot &&
		EQUIPIVTR_FASHION_FOOT != equipSlot &&
		EQUIPIVTR_FASHION_WRIST != equipSlot){
		return false;	//	不是时装位置
	}
	if (!GetRealElementID(equipSlot, m_aEquips[equipSlot])){
		return false;	//	没有装备
	}
	int equipID = m_aEquips[equipSlot];
	unsigned short currentColor = ((equipID >> 16) & 0x0000ffff);
	if (currentColor == newColor){
		return true;	//	与已有颜色相同，下面就不用麻烦检查了
	}
	int skinIndex = -1;
	switch (equipSlot){
	case EQUIPIVTR_FASHION_BODY:
		skinIndex = SKIN_FASHION_UPPER_BODY_INDEX;
		break;
	case EQUIPIVTR_FASHION_LEG:
		skinIndex = SKIN_FASHION_LOWER_INDEX;
		break;
	case EQUIPIVTR_FASHION_FOOT:
		skinIndex = SKIN_FASHION_FOOT_INDEX;
		break;
	case EQUIPIVTR_FASHION_WRIST:
		skinIndex = SKIN_FASHION_WRIST_INDEX;
		break;
	}
	A3DCOLOR a3dColor = FASHION_WORDCOLOR_TO_A3DCOLOR(newColor);
	for (int i(0); i < sizeof(m_aSkins[0])/sizeof(m_aSkins[0][0]); ++ i){
		if (A3DSkin *pSkin = m_aSkins[skinIndex][i]){
			for(int idTex=0; idTex<pSkin->GetTextureNum(); idTex ++){
				A3DTexture * pTex = pSkin->GetTexture(idTex);
				if (pTex && pTex->IsShaderTexture()){
					A3DShader * pShader = (A3DShader *) pTex;
					pShader->GetGeneralProps().dwTFactor = a3dColor;	//	Loop 每件 Skin，直接设置到每个 Shader 中
				}
			}
		}
	}
	int newEquipID = (m_aEquips[equipSlot] & 0x0000ffff) | (newColor << 16);
	m_aEquips[equipSlot] = newEquipID;
	return true;
}

void CECPlayer::RecreateActionController(){
	if (!m_pPlayerModel){
		delete m_pActionController;
		m_pActionController = NULL;
		return;
	}
	if (!m_pActionController){
		m_pActionController = new CECPlayerActionController;
	}
	m_pActionController->Bind(this, m_pPlayerModel);
}

void CECPlayer::RecreateBodyController(){
	if (!m_pPlayerModel){
		delete m_pBodyController;
		m_pBodyController = NULL;
		return;
	}
	if (!SupportCastSkillWhenMove()){
		return;
	}
	if (!m_pBodyController){
		m_pBodyController = new CECPlayerBodyController;
	}
	if (!m_pBodyController->Bind(this, m_pPlayerModel)){
		delete m_pBodyController;
		m_pBodyController = NULL;
	}
}

void CECPlayer::TurnFaceTo(int idTarget, DWORD dwTime){
	if (idTarget){
		if (idTarget == GetCharacterID()){
			return;
		}
		if (IsWorkMoveRunning() && !IsPlayingCastingSkillAndMoveActions()){	//	移动时一般应面向移动方向、只在移动施法中才转向
			return;
		}
	}
	if (!m_pBodyController){
		CECObject::TurnFaceTo(idTarget, dwTime);
		return;
	}
	m_pBodyController->TurnFaceTo(idTarget);
}

void CECPlayer::SetName(const ACHAR *szName){
	m_strName = szName;
}

void CECPlayer::CloneSimplePropertyTo(CECPlayer *player)const{
	//	CECObject
	player->SetPos(GetPos());
	player->SetDirAndUp(GetDir(), GetUp());
	player->SetGroundNormal(GetGroundNormal());
	player->SetUseGroundNormal(GetUseGroundNormal());

	//	CECPlayer	
	player->m_PlayerInfo = GetPlayerInfo();
	player->SetBornStamp(GetBornStamp());
	player->SetName(GetName());
	player->SetProps(&GetBasicProps(), &GetExtendProps());	
	player->m_iGender		= GetGender();
	player->m_iProfession	= GetProfession();
	player->m_pvp			= GetPVPInfo();
	player->m_aIconStates	= GetIconStates();
	player->SetMoneyAmount(GetMoneyAmount());
	player->m_iMaxMoney		= GetMaxMoneyAmount();
	player->m_i64EquipDisabled = m_i64EquipDisabled;
	player->SetBoothState(GetBoothState());
	player->m_factionPVPMask= m_factionPVPMask;
	player->m_RidingPet		= GetRidingPetInfo();
	player->m_iBattleCamp	= GetBattleCamp();
	player->m_dwGMFlags		= m_dwGMFlags;
	player->SetSpouse(GetSpouse());
	player->m_idForce		= GetForce();
	player->SetCountry(GetCountry());
	player->SetMoveMode(GetMoveMode());
	player->SetMoveEnv(GetMoveEnv());
	player->SetWalkRunFlag(GetWalkRunFlag());
	player->m_MoveConst		= m_MoveConst;
	player->m_aabbServer	= m_aabbServer;
	player->m_aabb			= m_aabb;
	player->m_dwStates		= m_dwStates;
	player->m_dwStates2		= m_dwStates2;
	player->m_fTouchRad		= GetTouchRadius();
	player->m_byPariahLvl	= GetPariahLevel();
	player->SetBoothName(GetBoothName());
	
	player->m_CustomizeFactor = m_CustomizeFactor;
	player->ChangeCustomizeData(m_CustomizeData);	
	for (int i=0; i < GetEffectCount(); i++)
		player->ApplyEffect(GetEffect(i), true);
	
	player->SetFactionID(GetFactionID());
	player->SetCurPetID(GetCurPetID());
	player->SetCurrentTitle(GetCurrentTitle());
	player->SetReputation(GetReputation());
	player->SetFashionMode(InFashionMode());
	player->SetFRoleID(GetFRoleID());
	player->SetMeridiansProp(GetMeridiansProp());
	player->SetReincarnationCount(GetReincarnationCount());
	player->SetRealmLevel(GetRealmLevel());
	player->ScaleBody(GetScaleBySkill());
	player->SetTeamRequire(GetTeamRequire(), false);
	player->SetNewExtendStates(0, m_aExtStates, OBJECT_EXT_STATE_COUNT);
	player->m_GoblinRenderCnt = m_GoblinRenderCnt;
	player->m_bRenderGoblin	= m_bRenderGoblin;
}

int CECPlayer::SearchFullSuite()
{
	DATA_TYPE DataType = DT_INVALID;
	elementdataman* pDataMan = g_pGame->GetElementDataMan();
	CECGame::SuiteEquipTable& SuiteMapTab = g_pGame->GetSuiteEquipTable();
	
	int i;
	typedef abase::hashtab<int, int, abase::_hash_function> SuiteTable;
	SuiteTable SuiteTab(32);
	
	//	Check how many suits of equipment host have dressed
	for (i=0; i < SIZE_ALL_EQUIPIVTR; i++)
	{
		int idEquip = GetEquippedItem(i);
		if (!idEquip)
			continue;
		
		CECGame::SuiteEquipTable::pair_type pair = SuiteMapTab.get(idEquip);
		if (!pair.second)
			continue;
		
		int idSuite = *pair.first;
		
		const void* pData = pDataMan->get_data_ptr(idSuite, ID_SPACE_ESSENCE, DataType);
		if (DataType != DT_SUITE_ESSENCE)
			continue;
		
		const SUITE_ESSENCE* pSuiteEss = (const SUITE_ESSENCE*)pData;
		if (pSuiteEss->file_gfx[0])
			SuiteTab.put(idSuite, pSuiteEss->max_equips);
	}
	
	if (!SuiteTab.size())
		return 0;	//	No suite was found
	
	//	Check whether suite is full
	SuiteTable::iterator it = SuiteTab.begin();
	for (; it != SuiteTab.end(); ++it)
	{
		int idSuite = *it.key();
		int iMaxNum = *it.value();
		
		if (iMaxNum == GetEquippedSuiteItem(idSuite))
			return idSuite;
	}
	
	return 0;
}

int CECPlayer::GetEquippedSuiteItem(int idSuite, int* aItems/* NULL */)
{
	CECGame::SuiteEquipTable& SuiteTab = g_pGame->GetSuiteEquipTable();
	
	int i, iItemCnt = 0;
	
	for (i=0; i < SIZE_ALL_EQUIPIVTR; i++)
	{
		int idEquip = GetEquippedItem(i);
		if (!idEquip)
			continue;
		
		CECGame::SuiteEquipTable::pair_type pair = SuiteTab.get(idEquip);
		if (!pair.second || idSuite != *pair.first)
			continue;
		
		if (aItems)
			aItems[iItemCnt] = idEquip;
		
		iItemCnt++;
	}
	
	return iItemCnt;
}

int CECPlayer::GetEquippedItem(int index)const{
	if (index < 0 || index >= SIZE_ALL_EQUIPIVTR){
		ASSERT(false);
		return 0;
	}
	return m_aEquips[index] & 0xffff;
}

void CECPlayer::DetachWeapon(){
	if (IsWeaponAttached()){
		ASSERT(GetPlayerModel() != NULL);
		if (GetLeftHandWeapon()){
			GetPlayerModel()->RemoveChildModel(_left_hand_weapon, false);
		}
		if (GetRightHandWeapon()){
			GetPlayerModel()->RemoveChildModel(_right_hand_weapon, false);
		}
		m_bWeaponAttached = false;
	}
}

bool CECPlayer::AttachWeapon(){
	bool result(false);
	ASSERT(!IsWeaponAttached());
	while (GetPlayerModel() && (GetLeftHandWeapon() || GetRightHandWeapon())){
		A3DSkinModel *pSkinModel = GetPlayerModel()->GetA3DSkinModel();
		if (!pSkinModel ||
			!pSkinModel->GetSkeleton()){
			break;
		}
		if (!pSkinModel->GetSkeletonHook(GetLeftWeaponHookPos(m_weaponHangerPos),true) ||
			!pSkinModel->GetSkeletonHook(GetRightWeaponHookPos(m_weaponHangerPos),true)){
			break;
		}
		if (GetLeftHandWeapon()){
			GetPlayerModel()->AddChildModel(
				_left_hand_weapon,
				false,
				GetLeftWeaponHookPos(m_weaponHangerPos),
				GetLeftHandWeapon(),
				GetLeftWeaponOwnHookPos(GetLeftHandWeapon()));
		}	
		if (GetRightHandWeapon()){
			GetPlayerModel()->AddChildModel(
				_right_hand_weapon,
				false,
				GetRightWeaponHookPos(m_weaponHangerPos),
				GetRightHandWeapon(),
				GetRightWeaponOwnHookPos(GetRightHandWeapon()));
		}
		m_bWeaponAttached = true;
		result = true;
		break;
	}
	return result;
}

void CECPlayer::ReleaseWeapon(){
	DetachWeapon();
	if (m_pLeftHandWeapon){
		A3DRELEASE(m_pLeftHandWeapon);
	}
	if (m_pRightHandWeapon){
		A3DRELEASE(m_pRightHandWeapon);
	}
}

bool CECPlayer::IsWeaponAttached()const{
	return m_bWeaponAttached;
}

int CECPlayer::GetRace()const{
	return CECProfConfig::Instance().GetRaceByProfession(GetProfession());
}

bool CECPlayer::ShouldUseFaceModel() const {
	return true;
}

bool CECPlayer::ShouldUseClothedModel() const {
	return true;
}

bool CECPlayer::ShouldUseModel() const {
	return true;
}

bool CECPlayer::ShouldUseBoothModel()const{
	if (m_iBoothState != 2){	//	不是摆摊状态不需要模型
		return false;
	}
	return true;
}