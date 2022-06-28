#include "stdafx.h"
#include "WarpGate.h"


CWarpGate::CWarpGate()
{
}


CWarpGate::~CWarpGate()
{
}

HRESULT CWarpGate::Initialize()
{
	m_eLayer = LAYER_OBJ;

	return S_OK;
}

int CWarpGate::Update()
{
	CGameObject::LateInit();

	return NO_EVENT;
}

int CWarpGate::LateUpdate()
{
	CGameObject::UpdateView();
	FrameMove();

	return NO_EVENT;
}

void CWarpGate::Render()
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

	// ·»´õ ÀÌ¹ÌÁö
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&m_pTexInfo->vOffset, nullptr, dwColor);
}

HRESULT CWarpGate::LateInit()
{
	m_wstrSceneName = m_wstrName;
	const UNIT_INFO* pUnitInfo = m_pObjMgr->GetUnitData(m_wstrStateKey);
	if (nullptr != pUnitInfo)
	{
		m_wstrObjKey = pUnitInfo->wstrObjKey;
		m_wstrStateKey = pUnitInfo->wstrStateKey;

		m_tOBB.vPos = m_tInfo.vPos;
		m_tOBB.vCollSize = pUnitInfo->vCollEnd - pUnitInfo->vCollStart;
		m_tOBB.vOffset = -pUnitInfo->vCollStart;
		CGameObject::InitCollVertex();
	}

	return S_OK;
}

void CWarpGate::Release()
{
}

void CWarpGate::FrameOnceEnd()
{
}

void CWarpGate::FrameEvent()
{
}

void CWarpGate::ChangeScene()
{
	m_pSceneMgr->SetNextScene(m_eSceneID, m_wstrSceneName);
}
