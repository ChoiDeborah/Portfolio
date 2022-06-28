#include "stdafx.h"
#include "NormalEffect.h"


CNormalEffect::CNormalEffect()
{
}


CNormalEffect::~CNormalEffect()
{
}

HRESULT CNormalEffect::Initialize()
{
	m_eLayer = LAYER_EFFECT;
	
	return S_OK;
}

int CNormalEffect::Update()
{
	if (nullptr != m_pTarget) {
		if (!m_pTarget->GetDead())
		{
			if (m_bIsFollow)
			{
				m_tInfo.vPos.x = m_pTarget->GetInfo().vPos.x;
				m_tInfo.vPos.y = m_pTarget->GetInfo().vPos.y;
			}
		}
	}

	return m_bIsDead;
}

int CNormalEffect::LateUpdate()
{
	CGameObject::UpdateView();
	if (!m_bIsDead)
		FrameMove();

	return NO_EVENT;
}

void CNormalEffect::Render()
{
	m_pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrObjKey, m_wstrStateKey, m_tFrame.iFrameCur);
	NULL_CHECK(m_pTexInfo);

	D3DXMATRIX matScale, matTrans, matRotZ;

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	D3DXMatrixRotationZ(&matRotZ, m_fAngle);
	m_tInfo.matWorld = matScale * matRotZ * matTrans;
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&(m_pTexInfo->vOffset + m_vOffset), nullptr, m_pTexInfo->dwBlend);
}

HRESULT CNormalEffect::LateInit()
{
	return E_NOTIMPL;
}

void CNormalEffect::Release()
{
}

void CNormalEffect::FrameOnceEnd()
{
	m_bIsDead = true;
}

void CNormalEffect::FrameEvent()
{
}
