#include "stdafx.h"
#include "EventObject.h"

#include "Character.h"

#include "NormalEffect.h"
#include "AttackEffect.h"
#include "DamageEffect.h"

// Controller
#include "Kyoko.h"

#include "Anais.h"
#include "Jelly.h"

// Monster
#include "DarkCow.h"
#include "Fixy.h"

// Npc
#include "Linda.h"
#include "Benya.h"

CEventObject::CEventObject()
{
	m_pVecTile = m_pTileMgr->GetTileVec();
	
	const TEX_INFO* pTarget = m_pTextureMgr->GetTexInfo(L"Effect", L"ObjectTarget");
	if (nullptr != pTarget)
	{
		m_tObjectTarget.fFrameTime = 0.f;
		m_tObjectTarget.fFrameDelay = pTarget->m_fFrameDelay;
		m_tObjectTarget.iFrameCur = 0;
		m_tObjectTarget.iFrameEnd = m_pTextureMgr->GetTexCount(L"Effect", L"ObjectTarget");
	}
}


CEventObject::~CEventObject()
{
}

void CEventObject::DrawHpBar()
{
	const TEX_INFO* pTexSmallBar = m_pTextureMgr->GetTexInfo(L"UI", L"MiniBar1");
	NULL_CHECK(pTexSmallBar);

	D3DXMATRIX matScale, matTrans, matWorld;

	// SmallBar1
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - float(pTexSmallBar->tImgInfo.Width) * 0.5f, m_tInfo.vPos.y + 20.f, 0.f);
	matWorld = matScale * matTrans;
	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexSmallBar->pTexture, nullptr,
		&pTexSmallBar->vOffset, nullptr, pTexSmallBar->dwBlend);

	// Small Hp Bar
	const TEX_INFO* pTexSmallHpBar = m_pTextureMgr->GetTexInfo(L"UI", L"MiniHpBar");
	NULL_CHECK(pTexSmallHpBar);
	D3DXMatrixScaling(&matScale, float(m_tUnitData.iHp) / m_tUnitData.iMaxHp, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - float(pTexSmallBar->tImgInfo.Width) * 0.5f, m_tInfo.vPos.y + 20.f, 0.f);
	matWorld = matScale * matTrans;
	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexSmallHpBar->pTexture, nullptr,
		&pTexSmallHpBar->vOffset, nullptr, pTexSmallHpBar->dwBlend);
}

void CEventObject::DrawHpBar2()
{
	const TEX_INFO* pTexSmallBar = m_pTextureMgr->GetTexInfo(L"UI", L"MiniBar0");
	NULL_CHECK(pTexSmallBar);

	D3DXMATRIX matScale, matTrans, matWorld;

	// SmallBar1
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - float(pTexSmallBar->tImgInfo.Width) * 0.5f, m_tInfo.vPos.y + 20.f, 0.f);
	matWorld = matScale * matTrans;
	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexSmallBar->pTexture, nullptr,
		&pTexSmallBar->vOffset, nullptr, pTexSmallBar->dwBlend);

	// Small Hp Bar
	const TEX_INFO* pTexSmallHpBar = m_pTextureMgr->GetTexInfo(L"UI", L"MiniHpBar");
	NULL_CHECK(pTexSmallHpBar);
	D3DXMatrixScaling(&matScale, float(m_tUnitData.iHp) / m_tUnitData.iMaxHp, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - float(pTexSmallBar->tImgInfo.Width) * 0.5f, m_tInfo.vPos.y + 20.f, 0.f);
	matWorld = matScale * matTrans;
	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexSmallHpBar->pTexture, nullptr,
		&pTexSmallHpBar->vOffset, nullptr, pTexSmallHpBar->dwBlend);

	// Small Mp Bar
	const TEX_INFO* pTexSmallMpBar = m_pTextureMgr->GetTexInfo(L"UI", L"MiniMpBar");
	NULL_CHECK(pTexSmallMpBar);
	D3DXMatrixScaling(&matScale, float(m_tUnitData.iMp) / m_tUnitData.iMaxMp, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - float(pTexSmallBar->tImgInfo.Width) * 0.5f, m_tInfo.vPos.y + 20.f + pTexSmallMpBar->tImgInfo.Height, 0.f);
	matWorld = matScale * matTrans;
	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexSmallMpBar->pTexture, nullptr,
		&pTexSmallMpBar->vOffset, nullptr, pTexSmallMpBar->dwBlend);
}

void CEventObject::DrawNameBar()
{
	const TEX_INFO* pTexSmallBar = m_pTextureMgr->GetTexInfo(L"UI", L"NameBar");
	NULL_CHECK(pTexSmallBar);

	D3DXMATRIX matScale, matTrans, matWorld;

	// NameBar
	D3DXMatrixScaling(&matScale, 0.6f, 0.4f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - 1.f, m_tInfo.vPos.y + 50.f, 0.f);
	matWorld = matScale * matTrans;
	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexSmallBar->pTexture, nullptr,
		&pTexSmallBar->vOffset, nullptr, pTexSmallBar->dwBlend);
	
	// Name Font
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	m_pDeviceMgr->GetSprite()->SetTransform(&matScale);
	RECT rt;
	SetRect(&rt, 
		int((m_tInfo.vPos.x - 1.f) - pTexSmallBar->tImgInfo.Width * 0.5f * 0.6f),
		int((m_tInfo.vPos.y + 55.f) - pTexSmallBar->tImgInfo.Height * 0.5f * 0.4f),
		int((m_tInfo.vPos.x - 1.f) + pTexSmallBar->tImgInfo.Width * 0.5f * 0.6f),
		int((m_tInfo.vPos.y + 55.f) + pTexSmallBar->tImgInfo.Height * 0.5f * 0.4f)
	);
	
	m_pDeviceMgr->DrawFont(m_tUnitData.wstrName, &rt, DT_CENTER | DT_NOCLIP);
}

void CEventObject::DrawPosition()
{
	const TEX_INFO* pTexSmallBar = m_pTextureMgr->GetTexInfo(L"UI", L"NameBar");
	NULL_CHECK(pTexSmallBar);

	// Name Font
	D3DXMATRIX matScale, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	m_pDeviceMgr->GetSprite()->SetTransform(&matScale);
	RECT rt;
	SetRect(&rt,
		int((m_tInfo.vPos.x - 1.f) - pTexSmallBar->tImgInfo.Width * 0.5f * 0.6f),
		int((m_tInfo.vPos.y + 40.f) - pTexSmallBar->tImgInfo.Height * 0.5f * 0.4f),
		int((m_tInfo.vPos.x - 1.f) + pTexSmallBar->tImgInfo.Width * 0.5f * 0.6f),
		int((m_tInfo.vPos.y + 40.f) + pTexSmallBar->tImgInfo.Height * 0.5f * 0.4f)
	);

	wstring wstrPos = L"[" + to_wstring(int(m_pViewMgr->GetRoomW())) + L", " + to_wstring(int(m_pViewMgr->GetRoomH())) + L"] " + to_wstring(int(m_tInfo.vPos.x)) + L", " + to_wstring(int(m_tInfo.vPos.y));
	m_pDeviceMgr->DrawFont(wstrPos, &rt, DT_CENTER | DT_NOCLIP);
}

void CEventObject::DrawChatting()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrObjKey, m_wstrStateKey);
	NULL_CHECK(pTexInfo);

	size_t iLength = m_wstrChat.length();

	if (0 >= iLength)
		return;

	float fWidth = (iLength < 10) ? float(iLength) : 10.f;
	fWidth *= 15.f;//14.f;
	int iHeight = iLength / 10;

	const TEX_INFO* pTexBlackBox = m_pTextureMgr->GetTexInfo(L"Background", L"BlackBack");
	NULL_CHECK(pTexBlackBox);

	D3DXMATRIX matScale, matTrans, matWorld;

	// NameBar
	D3DXMatrixScaling(&matScale, fWidth / float(pTexBlackBox->tImgInfo.Width), (18.f * (iHeight+1)) / float(pTexBlackBox->tImgInfo.Height), 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - fWidth * 0.5f, m_tInfo.vPos.y - m_tOBB.vCollSize.y - 37.f - 19.f * iHeight, 0.f);
	matWorld = matScale * matTrans;
	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexBlackBox->pTexture, nullptr,
		&pTexBlackBox->vOffset, nullptr, D3DCOLOR_ARGB(128, 0, 0, 0));

	// Draw Chat
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	m_pDeviceMgr->GetSprite()->SetTransform(&matScale);
	RECT rt;
	SetRect(&rt,
		int(m_tInfo.vPos.x - fWidth * 0.5f),
		int(m_tInfo.vPos.y - m_tOBB.vCollSize.y - 36.f - 18.f * iHeight),
		int(m_tInfo.vPos.x + fWidth * 0.5f),
		int(m_tInfo.vPos.y - m_tOBB.vCollSize.y - 18.f)
	);

	m_pDeviceMgr->DrawFont(m_wstrChat, &rt, DT_CENTER | DT_VCENTER | DT_NOCLIP | DT_WORDBREAK);
}

void CEventObject::Recovery()
{
	if (m_tRevoceryAlarm.CheckAlarm())
	{
		// 체력 회복
		m_tUnitData.iHp++;
		if (m_tUnitData.iMaxHp < m_tUnitData.iHp)
			m_tUnitData.iHp = m_tUnitData.iMaxHp;

		// 마나 회복
		m_tUnitData.iMp++;
		if (m_tUnitData.iMaxMp < m_tUnitData.iMp)
			m_tUnitData.iMp = m_tUnitData.iMaxMp;


		// 스테미너 회복
		m_tUnitData.iSp += 5;
		if (m_tUnitData.iMaxSp < m_tUnitData.iSp)
			m_tUnitData.iSp = m_tUnitData.iMaxSp;
	}

}

void CEventObject::AStarMove()
{
	NULL_CHECK(m_pVecTile);

	if (m_lstBest.empty())
	{
		this->AtarMoveEnd();
		m_bIsMove = false;
		return;
	}

	D3DXVECTOR3 vNextPos = m_lstBest.front();

	m_tInfo.vDir = vNextPos - m_tInfo.vPos;
	float fDistance = D3DXVec3Length(&m_tInfo.vDir);
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

	m_fAngle = acosf(D3DXVec3Dot(&D3DXVECTOR3{1.f, 0.f, 0.f}, &m_tInfo.vDir));
	if (0 < m_tInfo.vDir.y)
		m_fAngle = -m_fAngle;

	this->AstarMoveSetState();

	m_tInfo.vPos += m_tInfo.vDir * m_fSpeed *m_pTimeMgr->GetDeltaTime();

	if (fDistance < 5.f)
		m_lstBest.pop_front();

	if (this->AstartExitEvent())
	{
		m_lstBest.clear();
	}
}

void CEventObject::DebuffUpdate()
{
	if (0 < m_dwDebuff)
	{
		m_fSpeed = (m_dwDebuff & DEBUFF_FREZE) ? 150.f : ((m_dwDebuff & DEBUFF_IMMOVABLE) ? 0.f : 300.f);

		if (m_tDebuffDamage.CheckAlarm())
		{
			if (m_dwDebuff & DEBUFF_CONTINUOUS_DAMAGE ||
				m_dwDebuff & DEBUFF_POISION || 
				m_dwDebuff & DEBUFF_ELECTRIC_SHOCK)
			{
				CreateEffect(m_tInfo.vPos, D3DXVECTOR3{ 0.f, m_tOBB.vCollSize.y * 0.5f, 0.f }, L"Effect", L"ElecShock", false, OBJ_EFFECT, this, nullptr, FRAME_ONCE, true);
				InsertDamage(new DAMAGE_DATE(int(m_tUnitData.iMaxHp / 20), 0));
			}
		}

		if (m_tDebuffAlarm.CheckAlarm())
		{
			m_fSpeed = 300.f;

			m_pSceneMgr->SetEvent(EVENT_FADE_DARK, false);

			m_dwDebuff = 0;
		}
	}
}

void CEventObject::CreateCharacter(const wstring & wstrName)
{
	if (nullptr != m_pCharacter)
		SafeDelete(m_pCharacter);

	if (!lstrcmp(wstrName.c_str(), L"DarkCow"))
		SetCharacter(new CDarkCow);
	else if (!lstrcmp(wstrName.c_str(), L"Fixy"))
		SetCharacter(new CFixy);
	else if (!lstrcmp(wstrName.c_str(), L"Linda"))
		SetCharacter(new CLinda);
	else if (!lstrcmp(wstrName.c_str(), L"Benya"))
		SetCharacter(new CBenya);
	else if (!lstrcmp(wstrName.c_str(), L"Anais"))
		SetCharacter(new CAnais);
	else if (!lstrcmp(wstrName.c_str(), L"Jelly"))
		SetCharacter(new CJelly);
	else if (!lstrcmp(wstrName.c_str(), L"Kyoko"))
		SetCharacter(new CKyoko);
}

void CEventObject::SetCharacter(CCharacter* pCharacter)
{
	SafeDelete(m_pCharacter);
	if (nullptr != pCharacter)
	{
		m_pCharacter = pCharacter;
		m_pCharacter->SetBody(this);
		m_eDirection = DIRECTION::LEFT_DOWN;
		m_pCharacter->SetIdleState();
	}
}
