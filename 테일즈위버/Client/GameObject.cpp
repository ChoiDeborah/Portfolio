#include "stdafx.h"
#include "GameObject.h"

#include "NormalEffect.h"
#include "DamageEffect.h"

#include "Item.h"

#include "ItemEquipData.h"
#include "ItemUsableData.h"

CGameObject::CGameObject()
{
}

CGameObject::~CGameObject()
{
}

void CGameObject::MiniRender(D3DXVECTOR3 vPivot, float fPer)
{
	wstring wstrState = L"";
	switch (m_OBJID)
	{
	case OBJ_PLAYER: wstrState = L"PlayerPoint"; break;
	case OBJ_MONSTER: wstrState = L"MonsterPoint"; break;
	case OBJ_NPC: wstrState = L"NPCPoint"; break;
	case OBJ_BUDDY: wstrState = L"BuddyPoint"; break;
	default: return;
	}
	
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"UI", wstrState);
	NULL_CHECK(pTexInfo);

	D3DXMATRIX matScale, matTrans;

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, vPivot.x + m_tInfo.vPos.x * fPer, vPivot.y + m_tInfo.vPos.y * fPer, 0.f);
	m_tInfo.matWorld = matScale * matTrans;
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);

	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&pTexInfo->vOffset, nullptr, D3DCOLOR_ARGB(167, 255, 255, 255));
}

HRESULT CGameObject::LateInit()
{
	if (!m_bIsInit)
	{
		InitCollRect();
		this->LateInit();
		m_bIsInit = true;
	}

	return S_OK;
}

DIRECTION CGameObject::ChangeDirToAngle(float fRadian)
{
	float fDegree = D3DXToDegree(fRadian);
	if (0.f > fDegree)
		fDegree += 360.f;

	if ((fDegree > 345 && fDegree <= 360) || (fDegree <= 15 && fDegree >= 0))
		return RIGHT;
	else if (fDegree <= 75)
		return RIGHT_UP;
	else if (fDegree <= 105)
		return UP;
	else if (fDegree <= 165)
		return LEFT_UP;
	else if (fDegree <= 195)
		return LEFT;
	else if (fDegree <= 255)
		return LEFT_DOWN;
	else if (fDegree <= 285)
		return DOWN;
	else if (fDegree <= 345)
		return RIGHT_DOWN;

	return RIGHT;
}


float CGameObject::GetRadianAngle(D3DXVECTOR3& vTarget, D3DXVECTOR3& vStart)
{
	//D3DXVec3Normalize(&vMousePos, &(CMouseMgr::GetMouse() + m_pViewMgr->GetViewPos()));

	D3DXVECTOR3 vTargetDir;
	D3DXVec3Normalize(&vTargetDir, &(vTarget - vStart));

	float fAngle = acosf(D3DXVec3Dot(&D3DXVECTOR3{ 1.f, 0.f, 0.f }, &vTargetDir));
	if (0 < vTargetDir.y)
		fAngle = -fAngle;

	return fAngle;
}

float CGameObject::GetRadianAngle(D3DXVECTOR3 & vDir)
{
	D3DXVec3Normalize(&vDir, &vDir);

	float fAngle = acosf(D3DXVec3Dot(&D3DXVECTOR3{ 1.f, 0.f, 0.f }, &vDir));
	if (0 < vDir.y)
		fAngle = -fAngle;

	return fAngle;
}

bool CGameObject::IsOutPos()
{
	return (0 >= m_tInfo.vPos.x || CViewMgr::GetInstance()->GetRoomW() <= m_tInfo.vPos.x ||
		0 >= m_tInfo.vPos.y || CViewMgr::GetInstance()->GetRoomH() <= m_tInfo.vPos.y);
}

// 충돌박스 초기화
void CGameObject::InitCollRect()
{
	m_tOBB.vPos = m_tInfo.vPos;
	m_tOBB.vRight = { 1.f, 0.f, 0.f };
	m_tOBB.vUp = {0.f, 1.f, 0.f};

	m_pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrObjKey, m_wstrStateKey, m_tFrame.iFrameCur);
	if (nullptr != m_pTexInfo) {
		m_tOBB.vCollSize = { float(m_pTexInfo->tImgInfo.Width), float(m_pTexInfo->tImgInfo.Height), 0.f };
		m_tOBB.vOffset = m_pTexInfo->vOffset;
		InitCollVertex();
	}

	D3DXMATRIX matRotZ;
	D3DXMatrixRotationZ(&matRotZ, m_fAngle);
	m_tOBB.matWorld = matRotZ;

	D3DXVec3TransformCoord(&m_tOBB.vRight, &m_tOBB.vRight, &matRotZ);
	D3DXVec3TransformCoord(&m_tOBB.vUp, &m_tOBB.vUp, &matRotZ);
}

void CGameObject::InitCollVertex()
{
	m_tOBB.vVertex[0] = { -m_tOBB.vOffset.x, -m_tOBB.vOffset.y, 0.f };
	m_tOBB.vVertex[1] = { m_tOBB.vCollSize.x - m_tOBB.vOffset.x, -m_tOBB.vOffset.y, 0.f };
	m_tOBB.vVertex[2] = { m_tOBB.vCollSize.x - m_tOBB.vOffset.x, m_tOBB.vCollSize.y - m_tOBB.vOffset.y, 0.f };
	m_tOBB.vVertex[3] = { -m_tOBB.vOffset.x, m_tOBB.vCollSize.y - m_tOBB.vOffset.y, 0.f };
}

// 충돌 박스 그림
void CGameObject::DrawCollRect()
{
	m_tOBB.vPos = m_tInfo.vPos;
	D3DXVECTOR3 vRenderPos = m_tInfo.vPos;
	
	m_pDeviceMgr->GetSprite()->End();
	m_pDeviceMgr->GetDevice()->EndScene();

	m_pDeviceMgr->GetDevice()->BeginScene();
	m_pDeviceMgr->GetLine()->Begin();

	m_pViewMgr->Render();

	D3DXVECTOR2 pLine[5] = {};
	D3DXMATRIX matTmp;
	D3DXMatrixTranslation(&matTmp, vRenderPos.x, vRenderPos.y, 0.f);

	for (int i = 0; i < 4; ++i) {
		D3DXVECTOR3 vTmp;
		D3DXVec3TransformCoord(&vTmp, &m_tOBB.vVertex[i], &matTmp);
		pLine[i] = { vTmp.x, vTmp.y };
	}
	pLine[4] = pLine[0];
	m_pDeviceMgr->GetLine()->Draw(pLine, 5, D3DCOLOR_ARGB(255, 0, 255, 255));


	/*D3DXVECTOR2 pAxisRight[2] = {
		{ vRenderPos.x, vRenderPos.y },
		{ vRenderPos.x+m_tOBB.vRight.x * 100.f, vRenderPos.y+m_tOBB.vRight.y * 100.f },
	};
	m_pDeviceMgr->GetLine()->Draw(pAxisRight, 2, D3DCOLOR_ARGB(255, 0, 255, 0));

	D3DXVECTOR2 pAxisUp[2] = {
		{ vRenderPos.x, vRenderPos.y },
		{ vRenderPos.x + m_tOBB.vUp.x * 100.f, vRenderPos.y + m_tOBB.vUp.y * 100.f },
	};
	m_pDeviceMgr->GetLine()->Draw(pAxisUp, 2, D3DCOLOR_ARGB(255, 255, 0, 0));*/

	m_pDeviceMgr->GetLine()->End();
	m_pDeviceMgr->GetDevice()->EndScene();

	m_pDeviceMgr->GetDevice()->BeginScene();
	m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

	m_pViewMgr->Render();
}

CGameObject * CGameObject::CreateEffect(
	D3DXVECTOR3 vPos,
	D3DXVECTOR3 vOffset,
	wstring wstrObjKey,
	wstring wstrStateKey,
	bool	bIsAttack,
	OBJID eID,
	CGameObject* pTarget,
	CGameObject* pFrom,
	FRAMESTATE eFrameState,
	bool	bIsFollow)
{
	CGameObject* pInstance = nullptr;

	if (bIsAttack || OBJID::OBJ_ATTACK == eID || OBJID::OBJ_PLAYER_ATTACK == eID)
	{
		pInstance = CAbstractFactory<CAttackEffect>::CreateObj(vPos);
		if (nullptr != pInstance)
			dynamic_cast<CAttackEffect*>(pInstance)->SetFromObject(pFrom);
	}
	else
	{
		pInstance = CAbstractFactory<CNormalEffect>::CreateObj(vPos);
	}

	if (nullptr != pInstance)
	{
		pInstance->SetObjID(eID);
		pInstance->SetFrameKey(wstrObjKey, wstrStateKey, EFFECT, DOWN, eFrameState);
		pInstance->SetTarget(pTarget);
		dynamic_cast<CEffect*>(pInstance)->SetFollow(bIsFollow);
		dynamic_cast<CEffect*>(pInstance)->SetOffset(vOffset);
		CObjMgr::GetInstance()->AddObject(pInstance, eID);
	}

	return pInstance;
}

CGameObject* CGameObject::CreateDamageEffect(D3DXVECTOR3 vPos, int iDamage, int iType, wstring wstrStateKey)
{
	CGameObject* pInstance = nullptr;

	pInstance = CAbstractFactory<CDamageEffect>::CreateObj(vPos);

	if (nullptr != pInstance)
	{
		pInstance->SetObjID(OBJ_EFFECT);
		pInstance->SetFrameKey(L"Effect", wstrStateKey, EFFECT, DOWN, FRAME_ONCE);
		dynamic_cast<CDamageEffect*>(pInstance)->SetDamage(iDamage);
		dynamic_cast<CDamageEffect*>(pInstance)->SetDamageType(iType);
		CObjMgr::GetInstance()->AddObject(pInstance, OBJ_EFFECT);
	}

	return pInstance;
}

CGameObject* CGameObject::CreateItem(const wstring& wstrName, D3DXVECTOR3 vStartPos, D3DXVECTOR3 vTargetPos, int iCount, int iPrice)
{
	CItemData* pItem = CObjMgr::GetInstance()->GetItemData(wstrName);

	if (nullptr == pItem)
		return nullptr;
	
	CGameObject* pInstance = CAbstractFactory<CItem>::CreateObj(vStartPos);
	if (nullptr != pInstance)
	{
		dynamic_cast<CItem*>(pInstance)->SetTargetPos(vTargetPos);
		dynamic_cast<CItem*>(pInstance)->SetItemState(0);
		
		CItemData* pItemData = nullptr;
		switch (pItem->eItemType)
		{
		case ITEM_TYPE::ITEM_EQUIP:
			pItemData = new CItemEquipData(*(dynamic_cast<CItemEquipData*>(pItem)));
			break;

		case ITEM_TYPE::ITEM_USE:
			pItemData = new CItemUsableData(*(dynamic_cast<CItemUsableData*>(pItem)));
			break;

		case ITEM_TYPE::ITEM_ETC:
			pItemData = new CItemData(*(dynamic_cast<CItemData*>(pItem)));
			break;
		}
		pItemData->iCount = iCount;

		if (-1 < iPrice)
			pItemData->SetPrice(iPrice);

		dynamic_cast<CItem*>(pInstance)->SetItemData(pItemData);
		pInstance->SetFrameKey(pItemData->wstrObjKey, pItemData->wstrStateKey, CGameObject::STATE::EFFECT, DIRECTION::NO_DIRECTION, FRAME_ONCE);

		CObjMgr::GetInstance()->AddObject(pInstance, OBJID::OBJ_ITEM);
	}

	return pInstance;
}

void CGameObject::UpdateView()
{
	// OBB
	m_tOBB.vPos = m_tInfo.vPos;

	// Depth
	m_iDepth = int(m_tInfo.vPos.y);
	if (nullptr != m_pTexInfo)
		m_iDepth += int((m_pTexInfo->tImgInfo.Height - m_pTexInfo->vOffset.y) * 0.9f);

	// View
	m_fViewX = m_pViewMgr->GetViewX();
	m_fViewY = m_pViewMgr->GetViewY();
}

wstring CGameObject::AddWString_Dir_8(const wstring& wstrText, DIRECTION eDir)
{
	switch (eDir) {
	case DIRECTION::UP:				return wstrText + L"_U";
	case DIRECTION::DOWN:			return wstrText + L"_D";

	case DIRECTION::RIGHT:			return wstrText + L"_R";
	case DIRECTION::LEFT:			return wstrText + L"_L";

	case DIRECTION::RIGHT_UP:		return wstrText + L"_RU";
	case DIRECTION::LEFT_UP:		return wstrText + L"_LU";

	case DIRECTION::RIGHT_DOWN:		return wstrText + L"_RD";
	case DIRECTION::LEFT_DOWN:		return wstrText + L"_LD";
	}
	return wstrText;
}

wstring CGameObject::AddWString_Dir_4(const wstring& wstrText, DIRECTION eDir)
{
	switch (eDir) {
	case DIRECTION::UP:				return wstrText + L"_U";
	case DIRECTION::DOWN:			return wstrText + L"_D";

	case DIRECTION::RIGHT_UP:
	case DIRECTION::RIGHT_DOWN:
	case DIRECTION::RIGHT:			return wstrText + L"_R";

	case DIRECTION::LEFT_UP:
	case DIRECTION::LEFT_DOWN:
	case DIRECTION::LEFT:			return wstrText + L"_L";
	}
	return wstrText;
}

wstring CGameObject::AddWString_Dir_4_Cross(const wstring& wstrText, DIRECTION eDir)
{
	switch (eDir) {
	case DIRECTION::UP:
	case DIRECTION::LEFT_UP:		return wstrText + L"_LU";

	case DIRECTION::DOWN:
	case DIRECTION::LEFT:
	case DIRECTION::LEFT_DOWN:		return wstrText + L"_LD";

	case DIRECTION::RIGHT_UP:		return wstrText + L"_RU";
	
	case DIRECTION::RIGHT:
	case DIRECTION::RIGHT_DOWN:		return wstrText + L"_RD";
	}
	return wstrText;
}

wstring CGameObject::AddWString_Dir_2(const wstring & wstrText, DIRECTION eDir)
{
	switch (eDir) {
	case DIRECTION::LEFT_UP:
	case DIRECTION::DOWN:
	case DIRECTION::LEFT:
	case DIRECTION::LEFT_DOWN:		return wstrText + L"_L";

	case DIRECTION::UP:
	case DIRECTION::RIGHT_UP:
	case DIRECTION::RIGHT:
	case DIRECTION::RIGHT_DOWN:		return wstrText + L"_R";
	}
	return wstrText;
}

HRESULT CGameObject::SetFrameKey(const wstring & wstrObjKey, const wstring & wstrStateKey, STATE eCurState, DIRECTION eDir, FRAMESTATE eFrameState)
{
	m_eCurState = eCurState;
	if (eDir != m_eDirection || m_ePreState != m_eCurState) {
		m_eDirection = eDir;
		m_wstrObjKey = wstrObjKey;
		m_wstrStateKey = wstrStateKey;

		m_tInfo.vSize.x = 1.f;
		m_tInfo.vSize.y = 1.f;

		// AddDirWString(m_wstrStateKey);

		m_tFrame.iFrameCur = 0;
		m_tFrame.iFrameEnd = m_pTextureMgr->GetTexCount(m_wstrObjKey, m_wstrStateKey);

		const TEX_INFO*	pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrObjKey, m_wstrStateKey);
		NULL_CHECK_RETURN(pTexInfo, E_FAIL);

		m_tFrame.fFrameDelay = pTexInfo->m_fFrameDelay;

		m_tFrame.fFrameTime = CTimeMgr::GetInstance()->GetDeltaTime();

		m_eFrameState = eFrameState;
		m_ePreState = m_eCurState;
	}

	return S_OK;
}

void CGameObject::FrameMove() {
	if (0 > m_tFrame.fFrameDelay)
		return;

	m_tFrame.fFrameTime += CTimeMgr::GetInstance()->GetDeltaTime();
	if (m_tFrame.fFrameDelay < m_tFrame.fFrameTime) {
		m_tFrame.fFrameTime = 0.f;
		++m_tFrame.iFrameCur;

		if (m_tFrame.iFrameCur >= m_tFrame.iFrameEnd) {
			if (FRAME_ONCE == m_eFrameState)
			{
				m_tFrame.iFrameCur = m_tFrame.iFrameEnd - 1;
				this->FrameOnceEnd();
			}
			else
				m_tFrame.iFrameCur = 0;
		}

		this->FrameEvent();
	}
}