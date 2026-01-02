// Filename	: EC_ScreenEffectControl.cpp
// Creator	: zhangyitian
// Date		: 2014/8/5

#include "EC_ScreenEffect.h"
#include "EC_TimeSafeChecker.h"
#include "EC_ShakeCamera.h"
#include "EC_Viewport.h"
#include "EC_UIManager.h"

#include <AUIDialog.h>
#include <AMSoundBufferMan.h>
#include <AMSoundBuffer.h>
#include <AMSoundEngine.h>

CECScreenEffectMan::CECScreenEffectMan() {
	for (int i = 0; i < CECScreenEffect::EFFECT_NUM; i++) {
		m_pScreenEffect[i] = NULL;
	}
}

CECScreenEffectMan::~CECScreenEffectMan() {
	for (int i = 0; i < CECScreenEffect::EFFECT_NUM; i++) {
		if (m_pScreenEffect[i]) {
			m_pScreenEffect[i]->Finish();
			CECScreenEffect::DeleteEffect(m_pScreenEffect[i]);
			m_pScreenEffect[i] = NULL;
		}
	}
}

void CECScreenEffectMan::StartEffect(CECScreenEffect::enumScreenEffectType type, int iTime) {
	ASSERT(type != CECScreenEffect::EFFECT_NUM);
	if (m_pScreenEffect[type]) {
		m_pScreenEffect[type]->Finish();
		CECScreenEffect::DeleteEffect(m_pScreenEffect[type]);
		m_pScreenEffect[type] = NULL;
	}

	m_pScreenEffect[type] = CECScreenEffect::CreateEffect(type, iTime);
	if (m_pScreenEffect[type]) {
		m_pScreenEffect[type]->Start();
		m_iTime[type] = iTime;
		m_dwStartTime[type] = GetTickCount();
	}
}

void CECScreenEffectMan::FinishEffect(CECScreenEffect::enumScreenEffectType type) {
	ASSERT(type != CECScreenEffect::EFFECT_NUM);
	if (m_pScreenEffect[type]) {
		m_pScreenEffect[type]->Finish();
		CECScreenEffect::DeleteEffect(m_pScreenEffect[type]);
		m_pScreenEffect[type] = NULL;
	}
}

void CECScreenEffectMan::Tick() {
	for (int i = 0; i < CECScreenEffect::EFFECT_NUM; i++) {
		if (m_pScreenEffect[i] == NULL) {
			continue;
		}
		m_pScreenEffect[i]->Tick();
		if (CECTimeSafeChecker::ElapsedTimeFor(m_dwStartTime[i]) >= m_iTime[i]) {
			m_pScreenEffect[i]->Finish();
			CECScreenEffect::DeleteEffect(m_pScreenEffect[i]);
			m_pScreenEffect[i] = NULL;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

CECScreenEffect * CECScreenEffect::CreateEffect(enumScreenEffectType type, int iTime) {
	if (EFFECT_SCREENSHAKE == type) {
		return new CECScreenEffectShake();
	} else if (EFFECT_REDSPARK == type) {
		return new CECScreenEffectRedSpark();
	} else if (EFFECT_SCREENBLACKFADEIN == type) {
		CECScreenEffectBlackScreen * pEffect = new CECScreenEffectBlackScreen();
		pEffect->SetParam(iTime, true);
		return pEffect;
	} else if (EFFECT_SCREENBLACKFADEOUT == type) {
		CECScreenEffectBlackScreen * pEffect = new CECScreenEffectBlackScreen();
		pEffect->SetParam(iTime, false);
		return pEffect;
	} 
	else if (EFFECT_SCREENGRAY == type) {
		return new CECScreenEffectGrayScreen();
	} else {
		return NULL;
	}
}

void CECScreenEffect::DeleteEffect(CECScreenEffect* pEffect) {
	delete pEffect;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

CECScreenEffectShake::CECScreenEffectShake() {
	m_pShakeCamera = NULL;
}

CECScreenEffectShake::~CECScreenEffectShake() {
	if (m_pShakeCamera) {
		delete m_pShakeCamera;
	}
}

void CECScreenEffectShake::Start() {
	m_dwLastTime = GetTickCount();
	m_pShakeCamera = CECShakeCameraBase::CreateShakeCam(0, true);
	m_pShakeCamera->Start();
}

void CECScreenEffectShake::Finish() {
	m_pShakeCamera->Stop();
	delete m_pShakeCamera;
	m_pShakeCamera = NULL;
}

void CECScreenEffectShake::Tick() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

CECScreenEffectRedSpark::CECScreenEffectRedSpark() {
	m_pSoundBuffer = NULL;
}

void CECScreenEffectRedSpark::Tick() {
}

void CECScreenEffectRedSpark::Start() {
}

void CECScreenEffectRedSpark::Finish() {
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CECScreenEffectBlackScreen::SetParam(int iStayTime, bool bFadeIn) {
	m_iStayTime = iStayTime;
	m_bFadeIn = bFadeIn;
}

void CECScreenEffectBlackScreen::Tick() {
}

void CECScreenEffectBlackScreen::Start() {
}

void CECScreenEffectBlackScreen::Finish() {
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CECScreenEffectGrayScreen::s_bEnableGrayEffect = false;

void CECScreenEffectGrayScreen::Tick() {

}

void CECScreenEffectGrayScreen::Start() {
	s_bEnableGrayEffect = true;
}

void CECScreenEffectGrayScreen::Finish() {
	s_bEnableGrayEffect = false;
}

bool CECScreenEffectGrayScreen::IsEnableGrayEffect() {
	return s_bEnableGrayEffect;
}