#include "stdafx.h"
#include "ComboEffect.h"


CComboEffect::CComboEffect()
{
}


CComboEffect::~CComboEffect()
{
}

HRESULT CComboEffect::Initialize()
{
	m_eLayer = LAYER_EFFECT;
	m_eDirection = NO_DIRECTION;
	
	m_wstrStateKey = L"HitNumber";

	return S_OK;
}

int CComboEffect::Update()
{
	CGameObject::LateInit();

	if (nullptr != m_pTarget)
		m_tInfo.vPos = m_pTarget->GetInfo().vPos - D3DXVECTOR3{ 0.f, 150.f, 0.f };

	Pattern();

	return m_bIsDead;
}

int CComboEffect::LateUpdate()
{
	CGameObject::UpdateView();
	if (!m_bIsDead)
		FrameMove();

	return NO_EVENT;
}

void CComboEffect::Render()
{
	if (m_vecTexInfo.empty())
		return;

	D3DXMATRIX matScale, matTrans, matRotZ;

	const TEX_INFO*	pTexInfo = m_pTextureMgr->GetTexInfo(L"Effect", L"Combo");
	if (nullptr != pTexInfo)
	{
		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + 40, m_tInfo.vPos.y, 0.f);
		D3DXMatrixRotationZ(&matRotZ, m_fAngle);
		m_tInfo.matWorld = matScale * matRotZ * matTrans;
		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &pTexInfo->vOffset, nullptr, pTexInfo->dwBlend);
	}


	pTexInfo = m_pTextureMgr->GetTexInfo(L"Effect", m_wstrStateKey, 8);	// ±âÁØ

	for (int i = int(m_vecTexInfo.size()) - 1; i >= 0; --i)
	{
		if (nullptr != m_vecTexInfo[i])
		{
			D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
			D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + 40 + (pTexInfo->tImgInfo.Width * i) * m_tInfo.vSize.x + pTexInfo->tImgInfo.Width * 0.5f * m_tInfo.vSize.x, m_tInfo.vPos.y, 0.f);
			D3DXMatrixRotationZ(&matRotZ, m_fAngle);
			m_tInfo.matWorld = matScale * matRotZ * matTrans;
			m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
			m_pDeviceMgr->GetSprite()->Draw(m_vecTexInfo[i]->pTexture, nullptr, &m_vecTexInfo[i]->vOffset, nullptr, m_vecTexInfo[i]->dwBlend);
		}
	}
}

HRESULT CComboEffect::LateInit()
{
	// 0ÀÏ ¾Æ´Ò ¶§ ÄÞº¸
	if (0 != m_iDamage)
	{
		int iPowCount = 10;
		while (true)
		{
			++m_iCount;

			if (iPowCount > m_iDamage)
				break;

			iPowCount *= 10;
		}

		int iNum = m_iDamage;

		for (int i = 0; i < m_iCount; ++i)
		{
			iPowCount /= 10;
			int iNumber = iNum / iPowCount;

			m_pTexInfo = m_pTextureMgr->GetTexInfo(L"Effect", m_wstrStateKey, iNumber);
			if (nullptr != m_pTexInfo)
				m_vecTexInfo.push_back(m_pTexInfo);

			iNum %= iPowCount;
		}
	}

	return S_OK;
}

void CComboEffect::Release()
{
}

void CComboEffect::FrameOnceEnd()
{
}

void CComboEffect::FrameEvent()
{
}

void CComboEffect::Pattern()
{
	switch (m_iPattern)
	{
	case 0:
		m_tInfo.vSize.x += 0.1f;
		m_tInfo.vSize.y += 0.1f;
		if (2.f <= m_tInfo.vSize.y)
			m_iPattern = 1;
		break;

	case 1:
		m_tInfo.vSize.x -= 0.1f;
		m_tInfo.vSize.y -= 0.1f;
		if (1.f >= m_tInfo.vSize.y)
		{
			m_iPattern = 2;
			m_bIsDead = true;
		}
		break;
	}
}
