#include "stdafx.h"
#include "StaticObject.h"


CStaticObject::CStaticObject()
{
}


CStaticObject::~CStaticObject()
{
}

HRESULT CStaticObject::Initialize()
{
	m_eLayer = LAYER_OBJ;

	// 충돌 처리 박스 정보 (x크기, y크기, x오프셋, y오프셋)
	
	m_tOBB.vCollSize = { 40.f, 90.f, 0.f };
	m_tOBB.vOffset = { 0.f, 32.f, 0.f };
	CGameObject::InitCollVertex();

	return S_OK;
}

HRESULT CStaticObject::LateInit()
{
	return S_OK;
}

int CStaticObject::Update()
{
	CGameObject::LateInit();

	return NO_EVENT;
}

int CStaticObject::LateUpdate()
{
	CGameObject::UpdateView();
	FrameMove();

	return NO_EVENT;
}

void CStaticObject::Render()
{
	m_pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrObjKey, m_wstrStateKey, m_tFrame.iFrameCur);
	NULL_CHECK(m_pTexInfo);

	D3DXMATRIX matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	m_tInfo.matWorld = matScale * matTrans;
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);

	CGameObject* pPlayer = m_pObjMgr->GetPlayer();

	DWORD dwColor = m_pTexInfo->dwBlend;

	if (nullptr != pPlayer)
	{
		if (m_tInfo.vPos.y >= pPlayer->GetInfo().vPos.y
			&& CCollisionMgr::CheckOneToOne(m_tOBB, m_pObjMgr->GetPlayer()->GetOBB()))
			dwColor = D3DCOLOR_ARGB(128, D3DCOLOR_GET_R(m_pTexInfo->dwBlend), D3DCOLOR_GET_G(m_pTexInfo->dwBlend), D3DCOLOR_GET_B(m_pTexInfo->dwBlend));
	}

	// 렌더 이미지
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&m_pTexInfo->vOffset, nullptr, dwColor);

	//CGameObject::DrawCollRect();
}


void CStaticObject::Release()
{
}

void CStaticObject::FrameOnceEnd()
{
}

void CStaticObject::FrameEvent()
{
}
