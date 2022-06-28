#include "stdafx.h"
#include "Npc.h"

#include "Anais.h"

#include "Player.h"
#include "ComboEffect.h"
#include "DamageEffect.h"

#include "Linda.h"
#include "Benya.h"
#include "Jelly.h"
#include "Anais.h"


CNpc::CNpc()
{
}

CNpc::~CNpc()
{
}

void CNpc::Release()
{
	if (nullptr != m_pCharacter)
		SafeDelete(m_pCharacter);
}

HRESULT CNpc::Initialize()
{
	m_eLayer = LAYER_OBJ;

	m_fSpeed = 0.f;
	return S_OK;
}

HRESULT CNpc::LateInit()
{
	const UNIT_INFO* pUnitInfo = m_pObjMgr->GetUnitData(m_wstrName);
	if (nullptr != pUnitInfo)
	{
		m_tUnitData = *pUnitInfo;

		m_tOBB.vPos = m_tInfo.vPos;
		m_tOBB.vCollSize = m_tUnitData.vCollEnd - m_tUnitData.vCollStart;
		m_tOBB.vOffset = -m_tUnitData.vCollStart;
		CGameObject::InitCollVertex();
	}

	if (nullptr != m_pCharacter)
		m_pCharacter->SetIdleState();
	else
	{
		m_wstrObjKey = m_tUnitData.wstrObjKey;
		m_wstrStateKey = m_tUnitData.wstrStateKey;

		m_tFrame.fFrameDelay = m_pTextureMgr->GetTexInfo(m_wstrObjKey, m_wstrStateKey)->m_fFrameDelay;
		m_tFrame.iFrameEnd = m_pTextureMgr->GetTexCount(m_wstrObjKey, m_wstrStateKey);

		if (!lstrcmp(m_wstrStateKey.c_str(), L"Linda"))
			SetCharacter(new CLinda);
		else if (!lstrcmp(m_wstrStateKey.c_str(), L"Benya"))
			SetCharacter(new CBenya);
		else if (!lstrcmp(m_wstrStateKey.c_str(), L"Jelly"))
			SetCharacter(new CJelly);
		else if (!lstrcmp(m_wstrStateKey.c_str(), L"Anais"))
			SetCharacter(new CAnais);
	}

	m_vStartPos = m_tInfo.vPos;

	return S_OK;
}

int CNpc::Update()
{
	CGameObject::LateInit();

	m_bIsMouseTarget = false;

	if (m_bIsDead)
		return OBJ_DEAD;
	

	if (m_bIsTalk)
	{
		if (!m_pWordMgr->GetWord())
			m_bIsTalk = false;
	}

	return NO_EVENT;
}

int CNpc::LateUpdate()
{
	CGameObject::UpdateView();
	FrameMove();

	return NO_EVENT;
}

void CNpc::Render()
{
	m_pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrObjKey, m_wstrStateKey, m_tFrame.iFrameCur);
	NULL_CHECK(m_pTexInfo);

	D3DXMATRIX matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x * m_pTexInfo->m_iMirror, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	m_tInfo.matWorld = matScale * matTrans;
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);

	DWORD dwColor = m_pTexInfo->dwBlend;

	CGameObject* pPlayer = m_pObjMgr->GetPlayer();
	if (nullptr != pPlayer)
	{
		if (m_tInfo.vPos.y >= pPlayer->GetInfo().vPos.y
			&& CCollisionMgr::CheckOneToOne(m_tOBB, m_pObjMgr->GetPlayer()->GetOBB()))
			dwColor = D3DCOLOR_ARGB(128, D3DCOLOR_GET_R(dwColor), D3DCOLOR_GET_G(dwColor), D3DCOLOR_GET_B(dwColor));
	}

	// ±×¸²ÀÚ
	const TEX_INFO* pShadowTexInfo = m_pTextureMgr->GetTexInfo(L"Effect", L"Shadow", 0);
	if (nullptr != pShadowTexInfo) {
		m_pDeviceMgr->GetSprite()->Draw(pShadowTexInfo->pTexture, nullptr,
			&pShadowTexInfo->vOffset, nullptr, D3DCOLOR_ARGB(D3DCOLOR_GET_A(dwColor), 0, 0, 0));
	}

	// ·»´õ ÀÌ¹ÌÁö
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&m_pTexInfo->vOffset, nullptr, dwColor);

	if (!lstrcmp(L"Linda", m_wstrStateKey.c_str()) && !m_pSceneMgr->GetQuest(EVENT_QUEST0))
	{
		// ´À³¦Ç¥ Ãâ·Â
		const TEX_INFO* pExclamationMark = m_pTextureMgr->GetTexInfo(L"UI", L"ExclamationMark");
		if (nullptr != pExclamationMark)
		{
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
			D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y - m_pTexInfo->tImgInfo.Height, 0.f);
			m_tInfo.matWorld = matScale * matTrans;
			m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
			m_pDeviceMgr->GetSprite()->Draw(pExclamationMark->pTexture, nullptr,
				&pExclamationMark->vOffset, nullptr, dwColor);
		}
	}


	if (m_bIsMouseTarget) CEventObject::DrawNameBar();
}


void CNpc::FrameOnceEnd()
{
	NULL_CHECK(m_pCharacter);
	m_pCharacter->SetIdleState();
}

void CNpc::FrameEvent()
{
	NULL_CHECK(m_pCharacter);
	m_pCharacter->FrameEvent();
}

void CNpc::AtarMoveEnd()
{
	m_tMovingAlarm.SetAlarm(1.f, false);
	m_bIsMove = false;

	NULL_CHECK(m_pCharacter);
	m_pCharacter->SetIdleState();
}

void CNpc::AstarMoveSetState()
{
	NULL_CHECK(m_pCharacter);
	m_pCharacter->SetRunState();
}

bool CNpc::AstartExitEvent()
{
	if (nullptr != m_pTarget)
	{
		float fDistance = D3DXVec3Length(&(m_pTarget->GetInfo().vPos - m_tInfo.vPos));

		if (fDistance < TILECX)
		{
			m_bIsMove = false;
			m_bIsAttack = true;
			return true;
		}
	}

	return false;
}

void CNpc::Attack()
{
}


void CNpc::Hit()
{
	
}

void CNpc::Talk()
{
	if (nullptr != m_pCharacter)
		m_pCharacter->SetHitState();
	else
	{
		if (!lstrcmp(m_wstrStateKey.c_str(), L"Quest"))
		{
			m_pWordMgr->AddWord(new WORD_DATA(L"²ÁÁê", L"18»ì ²ÁÁê¿¹¿ä", L"Character", L"Word_Per"));
			m_pWordMgr->AddWord(new WORD_DATA(L"Kyoko", L"¿Í¿ì.. ^^;", L"Character", L"Kyoko"));
			m_pWordMgr->StartWord(false);
		}
	}
}
