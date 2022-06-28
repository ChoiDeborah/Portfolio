#include "stdafx.h"
#include "DamageEffect.h"


CDamageEffect::CDamageEffect()
{
}


CDamageEffect::~CDamageEffect()
{
}

HRESULT CDamageEffect::Initialize()
{
	m_eLayer = LAYER_EFFECT;
	m_eDirection = NO_DIRECTION;
	
	m_wstrStateKey = L"HitNumber";

	return S_OK;
}

int CDamageEffect::Update()
{
	CGameObject::LateInit();

	Pattern();

	return m_bIsDead;
}

int CDamageEffect::LateUpdate()
{
	CGameObject::UpdateView();
	if (!m_bIsDead)
		FrameMove();

	return NO_EVENT;
}

void CDamageEffect::Render()
{
	if (nullptr == m_pTextureMgr)
		return;

	D3DXMATRIX matScale, matTrans, matRotZ;

	// 크리티컬 이미지
	if (2 == m_iDamageType)
	{
		const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"Effect", L"Critical");
		if (nullptr != pTexInfo)
		{
			D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
			D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
			D3DXMatrixRotationZ(&matRotZ, m_fAngle);
			m_tInfo.matWorld = matScale * matRotZ * matTrans;
			m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
			m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
				&pTexInfo->vOffset, nullptr, pTexInfo->dwBlend);
		}
	}
	else
	{
		// 데미지 폰트
		if (0 == m_iDamage)
		{
			const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"Effect", L"Block");
			if (nullptr != pTexInfo)
			{
				D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
				D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
				D3DXMatrixRotationZ(&matRotZ, m_fAngle);
				m_tInfo.matWorld = matScale * matRotZ * matTrans;
				m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
				m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
					&pTexInfo->vOffset, nullptr, pTexInfo->dwBlend);
			}
		}

		else
		{
			const TEX_INFO*	pTexInfo = m_pTextureMgr->GetTexInfo(L"Effect", m_wstrStateKey, 8);	// 기준

			for (int i = int(m_vecTexInfo.size()) - 1; i >= 0; --i)
			{
				if (nullptr != m_vecTexInfo[i] && nullptr != pTexInfo)
				{
					D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
					D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + pTexInfo->tImgInfo.Width * (i - m_vecTexInfo.size() * 0.5f) * m_tInfo.vSize.x, m_tInfo.vPos.y, 0.f);
					D3DXMatrixRotationZ(&matRotZ, m_fAngle);
					m_tInfo.matWorld = matScale * matRotZ * matTrans;
					m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
					m_pDeviceMgr->GetSprite()->Draw(m_vecTexInfo[i]->pTexture, nullptr,
						&m_vecTexInfo[i]->vOffset, nullptr, m_vecTexInfo[i]->dwBlend);
				}
			}
		}
	}
}

HRESULT CDamageEffect::LateInit()
{
	if (2 == m_iDamageType)
	{
		
	}
	else
	{
		// 0일 때 스턱
		if (0 >= m_iDamage)
		{
			m_iDamage = 0;
			//m_pTexInfo = m_pTextureMgr->GetTexInfo(L"Effect", L"Block");
			if (nullptr != m_pTexInfo)
				m_vecTexInfo.push_back(m_pTexInfo);
		}

		// 0이 아닐 때 데미지 스킨 넣어주기
		else {
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
	}

	return S_OK;
}

void CDamageEffect::Release()
{
}

void CDamageEffect::FrameOnceEnd()
{
}

void CDamageEffect::FrameEvent()
{
}

void CDamageEffect::Pattern()
{
	switch (m_iDamageType)
	{
	// 도트 공격
	case 0:
		m_tInfo.vPos.y -= 7.f;
		switch (m_iPattern)
		{
		case 0:
			m_tInfo.vSize.x += 0.1f;
			m_tInfo.vSize.y += 0.2f;
			if (3.f <= m_tInfo.vSize.y)
				m_iPattern = 1;
			break;

		case 1:
			m_tInfo.vSize.x -= 0.1f;
			m_tInfo.vSize.y -= 0.2f;
			if (1.f >= m_tInfo.vSize.y)
			{
				m_iPattern = 2;
				m_bIsDead = true;
			}
			break;
		}
		break;


	// 일반 공격
	case 1:
		if (!m_bIsJump)
		{
			m_fJumAcc = 0.f;
			m_fJumAcc += m_fJumAccPower;
			m_tInfo.vPos.y -= m_fJumPower * m_fJumAcc * m_fJumSpeed - m_fJumAcc * m_fJumAcc * 0.5f * GRAVITY * 0.5f;

			m_bIsJump = true;
		}
		else
		{
			m_tInfo.vPos.x += 3.f;

			m_fJumAcc += m_fJumAccPower;
			float fJumpSpeed = m_fJumPower * m_fJumAcc * m_fJumSpeed - m_fJumAcc * m_fJumAcc * 0.5f * GRAVITY * 0.5f;
			if (m_fJumMaxGravity > fJumpSpeed) fJumpSpeed = m_fJumMaxGravity;
			m_tInfo.vPos.y -= fJumpSpeed;

			// 상승
			if (0 <= fJumpSpeed) {
			}
			// 하강
			else if (-m_fJumPower*1.3f >= fJumpSpeed){
				
			}
		}
		
		switch (m_iPattern)
		{
		case 0:
			m_tInfo.vSize.x += 0.05f;
			m_tInfo.vSize.y += 0.05f;
			if (1.5f <= m_tInfo.vSize.y)
				m_iPattern = 1;
			break;

		case 1:
			m_tInfo.vSize.x -= 0.05f;
			m_tInfo.vSize.y -= 0.05f;
			if (0.3f >= m_tInfo.vSize.y)
			{
				m_tInfo.vSize.x = 0.3f;
				m_tInfo.vSize.y = 0.3f;
				m_iPattern = 2;
				m_bIsDead = true;
			}
			break;
		}
		break;


	// 크리티컬 스킨
	case 2:
		switch (m_iPattern)
		{
		case 0:
			m_tInfo.vSize.x += 0.1f;
			m_tInfo.vSize.y += 0.2f;
			if (3.f <= m_tInfo.vSize.y)
				m_iPattern = 1;
			break;

		case 1:
			m_tInfo.vSize.x -= 0.1f;
			m_tInfo.vSize.y -= 0.2f;
			if (1.f >= m_tInfo.vSize.y)
			{
				m_iPattern = 2;
				m_bIsDead = true;
			}
			break;
		}
		break;
	}
}
