#include "stdafx.h"
#include "PuzzleTile.h"

#include "Item.h"


CPuzzleTile::CPuzzleTile()
{
}


CPuzzleTile::~CPuzzleTile()
{
}

HRESULT CPuzzleTile::Initialize()
{
	return S_OK;
}

int CPuzzleTile::Update()
{
	// 타일 밟았을 때
	OBJLIST Itemlst = m_pObjMgr->GetObjList(OBJ_ITEM);

	m_iWeight = 0;

	for (auto& Item : Itemlst)
	{
		if (nullptr != Item)
		{
			if (CCollisionMgr::CheckOneToOne(m_tOBB, Item->GetOBB()))
				m_iWeight += dynamic_cast<CItem*>(Item)->GetItemType()->iWeight;
		}
	}

	m_bIsWeight = (3 <= m_iWeight);	

	return NO_EVENT;
}

int CPuzzleTile::LateUpdate()
{
	CGameObject::UpdateView();

	return NO_EVENT;
}

void CPuzzleTile::Render()
{
	int iIndex = (m_bIsWeight) ? 1 : 0;

	m_pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrObjKey, m_wstrStateKey, iIndex);
	NULL_CHECK_MSG(m_pTexInfo, m_wstrStateKey.c_str());

	D3DXMATRIX matScale, matTrans;
	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x * m_pTexInfo->m_iMirror, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	m_tInfo.matWorld = matScale * matTrans;
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr, &m_pTexInfo->vOffset, nullptr, m_pTexInfo->dwBlend);
}

HRESULT CPuzzleTile::LateInit()
{
	m_pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrObjKey, m_wstrStateKey);

	if (nullptr != m_pTexInfo)
	{
		m_tOBB.vPos = m_tInfo.vPos;
		m_tOBB.vCollSize = D3DXVECTOR3{ float(m_pTexInfo->tImgInfo.Width), float(m_pTexInfo->tImgInfo.Height), 0.f };
		m_tOBB.vOffset = m_pTexInfo->vOffset;
		CGameObject::InitCollVertex();
	}

	return S_OK;
}

void CPuzzleTile::Release()
{
}

void CPuzzleTile::FrameOnceEnd()
{
}

void CPuzzleTile::FrameEvent()
{
}
