#include "stdafx.h"
#include "Item.h"

#include "ItemEquipData.h"
#include "ItemUsableData.h"

#include "InvenWnd.h"

CItem::CItem()
{
}


CItem::~CItem()
{
}

HRESULT CItem::Initialize()
{
	m_eLayer = LAYER_OBJ;

	return S_OK;
}

HRESULT CItem::LateInit()
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

int CItem::Update()
{
	CGameObject::LateInit();

	m_bIsMouseTarget = false;

	if (m_bIsDead)
		return OBJ_DEAD;

	Move();

	return NO_EVENT;
}

int CItem::LateUpdate()
{
	CGameObject::UpdateView();
	FrameMove();

	return NO_EVENT;
}

void CItem::Render()
{
	m_pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrObjKey, m_wstrStateKey);
	NULL_CHECK(m_pTexInfo);

	D3DXMATRIX matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	m_tInfo.matWorld = matScale * matTrans;
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	
	// 렌더 이미지
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&m_pTexInfo->vOffset, nullptr, m_pTexInfo->dwBlend);

	//CGameObject::DrawCollRect();
	if (m_bIsMouseTarget)
		DrawNameBar();
}


void CItem::Release()
{
}

void CItem::FrameOnceEnd()
{
}

void CItem::FrameEvent()
{
}

void CItem::Move()
{
	if (m_bIsDead)
		return;

	switch (m_iState)
	{
		// 땅에 떨어지는 중
	case 0:
	{
		if (!m_bIsJump)
		{
			m_tFrame.fFrameDelay = 1.f;
			m_tFrame.fFrameTime = 0.f;

			m_vStartPos = m_tInfo.vPos;
			m_fJumAcc = 0.f;
			m_tInfo.vDir = m_vTargetPos - m_vStartPos;


			//float fTop = m_vTargetPos.y - 100.f;
			//
			//// 최고 높이일 경우
			////m_fJumAcc = m_fJumPower / GRAVITY / 0.25f;
			////m_fJumAcc / m_fJumAccPower; // 몇 번 만에 최고 높이인지
			//
			////((m_fJumPower / GRAVITY / 0.25f) / m_fJumAccPower) == 1
			//
			//m_fSpeed = D3DXVec3Length(&(m_vTargetPos - m_vStartPos));
			//
			//float fDif = fTop - m_vStartPos.y;
			////m_fJumPower = sqrtf(fDif * 39.2f / (1 - 39.2f*39.2f)) * ((sqrtf(fDif * 39.2f / (1 - 39.2f*39.2f))) / (GRAVITY * 0.25f) / m_fJumAccPower);
			//m_fJumAcc = (m_fJumPower / GRAVITY / 0.25f);
			//m_fJumPower = (fDif + 0.5f * GRAVITY * m_fJumAcc) / m_fJumAcc;
			//cout << "m_fJumPower : " << m_fJumPower << endl;
			//cout << "X : " << m_tInfo.vPos.x << ", Y : " << m_tInfo.vPos.y << endl;

			m_bIsJump = true;
		}
		else
		{
			//m_tFrame.fFrameTime += m_pTimeMgr->GetDeltaTime();
			//if (m_tFrame.fFrameDelay > m_tFrame.fFrameTime)
			//{
			//	//m_fJumAcc += m_tFrame.fFrameTime;
			//	float fJumpSpeed = m_fJumPower * m_fJumAcc - m_fJumAcc * m_fJumAcc * GRAVITY * 0.25f;
			//	m_tInfo.vPos.y = m_vStartPos.y - fJumpSpeed * m_tFrame.fFrameTime;
			//	// 상승
			//	if (0 <= fJumpSpeed) {
			//	}
			//	else
			//	{
			//		m_iState = 1;
			//		cout << "상승 X : " << m_tInfo.vPos.x << ", Y : " << m_tInfo.vPos.y << endl;
			//	}
			//}
			
			m_tInfo.vPos += m_tInfo.vDir * m_pTimeMgr->GetDeltaTime() * 5.f;

			float fDistance = D3DXVec3Length(&(m_vTargetPos - m_tInfo.vPos));
			if (5.f >= fDistance)
			{
				m_iState = 1;
			}
		}
	}
	break;

	// 땅에 떨어진 상태
	case 1:
		m_bIsJump = false;
		break;

		// 인벤토리로 들어가는 상태
	case 2:
	{
		if (!m_bIsJump)
		{
			m_vTargetPos = m_pViewMgr->GetViewPos() + D3DXVECTOR3{ float(CLIENTCX), 120.f, 0.f };
			m_vStartPos = m_tInfo.vPos;
			m_fJumAcc = 0.f;
			m_bIsJump = true;
		}
		else
		{
			m_tInfo.vPos += (m_vTargetPos - m_vStartPos) * m_pTimeMgr->GetDeltaTime();

			float fDistance = D3DXVec3Length(&(m_vTargetPos - m_tInfo.vPos));
			if (5.f >= fDistance)
			{
				dynamic_cast<CInvenWnd*>(m_pUIMgr->GetWindow(WND_ID::WNDID_INVEN))->AddItem(m_pItemData);
				
				m_bIsDead = true;
			}

			//   거 
			// 속 시

			/*0 == m_fJumPower * m_fJumAcc * m_fJumSpeed - m_fJumAcc * m_fJumAcc * 0.5f * GRAVITY * 0.5f;

			m_fJumAcc += m_fJumAccPower;
			float fJumpSpeed = m_fJumPower * m_fJumAcc * m_fJumSpeed - m_fJumAcc * m_fJumAcc * 0.5f * GRAVITY * 0.5f;
			if (m_fJumMaxGravity > fJumpSpeed) fJumpSpeed = m_fJumMaxGravity;
			m_tInfo.vPos.y -= fJumpSpeed;*/

			
		}

		//	m_fJumAcc += m_fJumAccPower;
		//	float fJumpSpeed = m_fJumPower * m_fJumAcc * m_fJumSpeed - m_fJumAcc * m_fJumAcc * 0.5f * GRAVITY * 0.5f;
		//	if (m_fJumMaxGravity > fJumpSpeed) fJumpSpeed = m_fJumMaxGravity;
		//	m_tInfo.vPos.y -= fJumpSpeed;

		//	// 상승
		//	if (0 <= fJumpSpeed) {
		//	}
		//	// 하강
		//	else if (-m_fJumPower*1.3f >= fJumpSpeed) {

		//	}

		//	float fDistance = D3DXVec3Length(&(m_vTargetPos - m_tInfo.vPos));
		//	if (5.f >= fDistance)
		//		m_bIsDead = true;
		//}
		//float fDistX = m_vTargetPos.x - m_vStartPos.x;
		//m_tInfo.vPos.x += fDistX * m_pTimeMgr->GetDeltaTime();
	}
	break;
	}
}

void CItem::DrawNameBar()
{
	const TEX_INFO* pTexSmallBar = m_pTextureMgr->GetTexInfo(L"UI", L"NameBar");
	NULL_CHECK(pTexSmallBar);

	D3DXMATRIX matScale, matTrans, matWorld;

	// NameBar
	D3DXMatrixScaling(&matScale, 0.6f, 0.4f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - 1.f, m_tInfo.vPos.y + 35.f, 0.f);
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
		int((m_tInfo.vPos.y + 35.f) - pTexSmallBar->tImgInfo.Height * 0.5f * 0.4f),
		int((m_tInfo.vPos.x - 1.f) + pTexSmallBar->tImgInfo.Width * 0.5f * 0.6f),
		int((m_tInfo.vPos.y + 35.f) + pTexSmallBar->tImgInfo.Height * 0.5f * 0.4f)
	);
	m_pDeviceMgr->DrawFont(m_wstrStateKey.c_str(), &rt, DT_CENTER | DT_NOCLIP);
}
