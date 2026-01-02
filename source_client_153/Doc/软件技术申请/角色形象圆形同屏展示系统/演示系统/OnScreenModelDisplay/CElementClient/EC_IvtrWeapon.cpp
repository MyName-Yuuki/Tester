/*
 * FILE: EC_IvtrWeapon.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/11/19
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_IvtrWeapon.h"
#include "EC_Game.h"
#include "EC_FixedMsg.h"
#include "EC_GameRun.h"
#include "EC_RTDebug.h"
#include "elementdataman.h"
#include "EC_Configs.h"

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


///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECIvtrWeapon
//	
///////////////////////////////////////////////////////////////////////////

CECIvtrWeapon::CECIvtrWeapon(int tid, int expire_date) : CECIvtrEquip(tid, expire_date)
{
	m_iCID	= ICID_WEAPON;

	memset(&m_Essence, 0, sizeof (m_Essence));

	//	Get database data
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType;
	m_pDBEssence	= (WEAPON_ESSENCE*)pDB->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	m_pDBMajorType	= (WEAPON_MAJOR_TYPE*)pDB->get_data_ptr(m_pDBEssence->id_major_type, ID_SPACE_ESSENCE, DataType);
	m_pDBSubType	= (WEAPON_SUB_TYPE*)pDB->get_data_ptr(m_pDBEssence->id_sub_type, ID_SPACE_ESSENCE, DataType);

	m_iPileLimit	= m_pDBEssence->pile_num_max;
	m_iPrice		= m_pDBEssence->price;
	m_iShopPrice	= m_pDBEssence->shop_price;
	m_iRepairFee	= m_pDBEssence->repairfee;
	m_i64EquipMask	= EQUIP_MASK64_WEAPON;
	m_iFixProps		= m_pDBEssence->fixed_props;
	m_iProcType		= m_pDBEssence->proc_type;
	m_iReputationReq= m_pDBEssence->require_reputation;
}

CECIvtrWeapon::CECIvtrWeapon(const CECIvtrWeapon& s) : CECIvtrEquip(s)
{
	m_pDBEssence	= s.m_pDBEssence;
	m_pDBMajorType	= s.m_pDBMajorType;
	m_pDBSubType	= s.m_pDBSubType;
	m_Essence		= s.m_Essence;
}

CECIvtrWeapon::~CECIvtrWeapon()
{
}

//	Set item detail information
bool CECIvtrWeapon::SetItemInfo(BYTE* pInfoData, int iDataLen)
{
	CECIvtrEquip::SetItemInfo(pInfoData, iDataLen);

	if (!pInfoData || !iDataLen)
		return true;
	
	try
	{
		CECDataReader dr(pInfoData, iDataLen);

		//	Skip equip requirements and endurance
		dr.Offset(6 * sizeof (short), SEEK_CUR);
		dr.Offset(2 * sizeof (int), SEEK_CUR);

		int iEssenceSize = dr.Read_short();
		ASSERT(iEssenceSize == sizeof (IVTR_ESSENCE_WEAPON));
	
		//	Skip maker's information
		dr.Read_BYTE();
		int iMakerLen = dr.Read_BYTE();
		dr.Offset(iMakerLen, SEEK_CUR);
	
		m_Essence = *(IVTR_ESSENCE_WEAPON*)dr.Read_Data(iEssenceSize);
		
		// 加入附加属性说明
		if(m_pDBEssence->fixed_props != 0 && m_pDBEssence->probability_addon_num0 != 1.0f) 
		{
			//	Get database data
			elementdataman* pDataMan = g_pGame->GetElementDataMan();
			CECGame::ItemExtPropTable& PropTab = g_pGame->GetItemExtPropTable();

			int i, iSize = 0;
			for(i=0;i<32;i++)
			{
				if(m_pDBEssence->addons[i].id_addon != 0)
					iSize ++;
			}

			if(iSize > 0 && m_aProps.GetSize() == 0)
			{
				m_aProps.SetSize(iSize, 5);
				for(i=0;i<32;i++)
				{
					if(m_pDBEssence->addons[i].id_addon != 0)
					{
						PROPERTY& Prop = m_aProps[i];
						Prop.iType = m_pDBEssence->addons[i].id_addon;
						Prop.bEmbed = false;
						Prop.bSuite = false;
						Prop.bEngraved = false;
						Prop.bLocal = true;

						CECGame::ItemExtPropTable::pair_type Pair = PropTab.get(Prop.iType);
						BYTE bType = Pair.second ? *Pair.first : 0xff;

						DATA_TYPE DataType = DT_INVALID;
						const void* pData = pDataMan->get_data_ptr(m_pDBEssence->addons[i].id_addon, ID_SPACE_ADDON, DataType);
						if (DataType != DT_EQUIPMENT_ADDON)
						{
							ASSERT(DataType == DT_EQUIPMENT_ADDON);
							return 0;
						}

						const EQUIPMENT_ADDON* pAddon = (const EQUIPMENT_ADDON*)pData;
						Prop.iNumParam = pAddon->num_params;						

						for(int j=0; j < Prop.iNumParam; j++)
						{
							if(j==0)
								Prop.aParams[0] = pAddon->param1;
							else if(j==1)
								Prop.aParams[1] = pAddon->param2;
							else if(j==2)
								Prop.aParams[2] = pAddon->param3;
						}
					}
				}
			}
		}
	}
	catch (CECException& e)
	{
		ASSERT(0);
		a_LogOutput(1, "CECIvtrWeapon::SetItemInfo, data read error (%d)", e.GetType());
		return false;
	}

	return true;
}

//	Get item default information from database
void CECIvtrWeapon::DefaultInfo()
{
	m_iLevelReq		= m_pDBEssence->require_level;
	m_iStrengthReq	= m_pDBEssence->require_strength;
	m_iAgilityReq	= m_pDBEssence->require_agility;
	m_iReputationReq= m_pDBEssence->require_reputation;
	m_iCurEndurance	= m_pDBEssence->durability_min * ENDURANCE_SCALE;
	m_iMaxEndurance = m_pDBEssence->durability_min * ENDURANCE_SCALE;
}

//	Get item icon file name
const char* CECIvtrWeapon::GetIconFile()
{
	return m_pDBEssence->file_icon;
}

//	Get item name
const wchar_t* CECIvtrWeapon::GetName()
{
	return m_pDBEssence->name;
}

const wchar_t* CECIvtrWeapon::GetPreviewInfo()
{
	int aPEEVals[MAX_PEEINDEX];
	int aRefines[MAX_REFINEINDEX];
	memset(aPEEVals, 0, sizeof (aPEEVals));
	memset(aRefines, 0, sizeof (aRefines));
	m_strDesc = _AL("");
	BuildAddOnPropDesc(aPEEVals, aRefines);
	CECStringTab* pDescTab = g_pGame->GetItemDesc();
	//	Physical damage
	if (m_Essence.damage_low || m_Essence.damage_high || aRefines[REFINE_PHYDAMAGE])
	{
		AddDescText(ITEMDESC_COL_WHITE, false, pDescTab->GetWideString(ITEMDESC_PHYDAMAGE));
		AddDescText(ITEMDESC_COL_WHITE, true, _AL(" %d-%d"), m_Essence.damage_low - aPEEVals[PEEI_PHYDAMAGE] + aRefines[REFINE_PHYDAMAGE], 
			m_Essence.damage_high - aPEEVals[PEEI_PHYDAMAGE] - aPEEVals[PEEI_MAX_PHYDAMAGE] + aRefines[REFINE_PHYDAMAGE]);
	}
	
	//	Magic damage
	if (m_Essence.magic_damage_low || m_Essence.magic_damage_high || aRefines[REFINE_MAGICDAMAGE])
	{
		AddDescText(ITEMDESC_COL_WHITE, false, pDescTab->GetWideString(ITEMDESC_MAGICDAMAGE));
		AddDescText(ITEMDESC_COL_WHITE, true, _AL(" %d-%d"), m_Essence.magic_damage_low - aPEEVals[PEEI_MAGICDAMAGE] + aRefines[REFINE_MAGICDAMAGE], 
			m_Essence.magic_damage_high - aPEEVals[PEEI_MAGICDAMAGE] - aPEEVals[PEEI_MAX_MAGICDAMAGE] + aRefines[REFINE_MAGICDAMAGE]);
	}

	return m_strDesc;
}

bool CECIvtrWeapon::GetRefineEffectFor(ACString & strEffect, const RefineEffect &rhs)
{
	strEffect.Empty();
	if (!m_bNeedUpdate){
		CECStringTab* pDescTab = g_pGame->GetItemDesc();
		switch (rhs.m_refineIndex)
		{
		case REFINE_PHYDAMAGE:
			strEffect.Format(_AL("%s%s %d-%d %s(+%d)")
				, rhs.GetClrAttribute()
				, pDescTab->GetWideString(ITEMDESC_PHYDAMAGE)
				, m_Essence.damage_low - rhs.m_aPEEVals[PEEI_PHYDAMAGE] + rhs.m_aRefines[REFINE_PHYDAMAGE] + rhs.GetIncEffect()
				, m_Essence.damage_high - rhs.m_aPEEVals[PEEI_PHYDAMAGE] - rhs.m_aPEEVals[PEEI_MAX_PHYDAMAGE] + rhs.m_aRefines[REFINE_PHYDAMAGE] + rhs.GetIncEffect()
				, rhs.GetClrEffect()
				, rhs.GetIncEffect());
			break;
			
		case REFINE_MAGICDAMAGE:
			strEffect.Format(_AL("%s%s %d-%d %s(+%d)")
				, rhs.GetClrAttribute()
				, pDescTab->GetWideString(ITEMDESC_MAGICDAMAGE)
				, m_Essence.magic_damage_low - rhs.m_aPEEVals[PEEI_MAGICDAMAGE] + rhs.m_aRefines[REFINE_MAGICDAMAGE] + rhs.GetIncEffect()
				, m_Essence.magic_damage_high - rhs.m_aPEEVals[PEEI_MAGICDAMAGE] - rhs.m_aPEEVals[PEEI_MAX_MAGICDAMAGE] + rhs.m_aRefines[REFINE_MAGICDAMAGE] + rhs.GetIncEffect()
				, rhs.GetClrEffect()
				, rhs.GetIncEffect());
			break;
		}
	}
	return !strEffect.IsEmpty();
}

//	Get item description text
const wchar_t* CECIvtrWeapon::GetNormalDesc(bool bRepair)
{
	return m_strDesc;
}

//	Get weapon attack speed index
int CECIvtrWeapon::GetAttackSpeedIndex()
{
	float fCurSpeed = m_Essence.attack_speed * 0.05f - m_pDBSubType->attack_speed;
	if (fCurSpeed < -0.101f)
		return ITEMDESC_ATKSPD_VERYFAST;
	else if (fCurSpeed < -0.001f)
		return ITEMDESC_ATKSPD_FAST;
	else if (fCurSpeed < 0.001f)
		return ITEMDESC_ATKSPD_NORMAL;
	else if (fCurSpeed <= 0.101f)
		return ITEMDESC_ATKSPD_SLOW;
	else
		return ITEMDESC_ATKSPD_VERYSLOW;
}

//	Does this equipment has random property ?
bool CECIvtrWeapon::HasRandomProp()
{
	for (int i=0; i < m_aProps.GetSize(); i++)
	{
		const PROPERTY& prop = m_aProps[i];
		if (!prop.bEmbed && !prop.bEngraved && prop.iType == 472)
			return true;
	}

	return false;
}

//	Get number of material needed to refine equipment
int CECIvtrWeapon::GetRefineMaterialNum()
{
	return m_pDBEssence->material_need;
}

int CECIvtrWeapon::GetRefineAddOn()
{
	return m_pDBEssence->levelup_addon;
}
//	Get drop model for shown
const char * CECIvtrWeapon::GetDropModel()
{
	return m_pDBEssence->file_matter;
}

bool CECIvtrWeapon::IsRare() const
{ 
	return CECIvtrEquip::IsRare() || m_Essence.weapon_level >= 6;
}

int CECIvtrWeapon::GetItemLevel() const
{
	return m_Essence.weapon_level;
}