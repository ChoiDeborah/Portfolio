#include "stdafx.h"
#include "Chain.h"

#include "NormalEffect.h"
#include "AttackEffect.h"

CChain::CChain()
{
}

CChain::~CChain()
{
}

void CChain::Release()
{
}

HRESULT CChain::Initialize()
{
	m_eLayer = LAYER_EFFECT;

	m_fSpeed = 2000.f;

	//m_tPosAlarm.SetAlarm(0.001f, true);
	return S_OK;
}

HRESULT CChain::LateInit()
{
	m_wstrObjKey = L"Effect";
	m_wstrStateKey = L"ChainHead";

	CGameObject::InitCollRect();

	m_tInfo.vDir = { 1.f, 0.f, 0.f };

	if (nullptr == m_pPlayer)
		m_pPlayer = m_pObjMgr->GetPlayer();

	int iCount = 0;

	m_vecTarget.push_back(m_pPlayer);

	OBJLIST lstMonster = m_pObjMgr->GetObjList(OBJ_MONSTER);
	
	for (auto& Monster : lstMonster)
	{
		float fDist = D3DXVec3Length(&(Monster->GetInfo().vPos - m_pPlayer->GetInfo().vPos));

		if (1000.f > fDist)
		{
			m_vecTarget.push_back(Monster);
			++iCount;
			if (iCount > 10)
				break;
		}
	}

	if (size_t(2) > m_vecTarget.size())
		m_bIsDead = true;
	else
	{
		/*sort(m_vecTarget.begin() + 1, m_vecTarget.end(), [&](CGameObject* pObjA, CGameObject* pObjB) {
			float fDistA = D3DXVec3Length(&(pObjA->GetInfo().vPos - m_pPlayer->GetInfo().vPos));
			float fDistB = D3DXVec3Length(&(pObjB->GetInfo().vPos - m_pPlayer->GetInfo().vPos));

			return (fDistA > fDistB);
		});*/

		m_pTarget = m_vecTarget[m_iCurTarget];
		//m_vecTarget.pop_back();

		// 벡터에 위치 넣어줘야함 //
		m_tInfo.vPos = m_pPlayer->GetInfo().vPos;
		//m_vecPrevPos.push_back(m_tInfo.vPos);

		if (nullptr == m_pTarget)
			m_bIsDead = true;
	}

	for (int j = 0; j < 100; ++j)
	{
		for (size_t i = 1; i < m_vecTarget.size()-1; ++i)
		{
			int iRand = int(rand() % (m_vecTarget.size()-2) + 1);
			CGameObject* pTmp = m_vecTarget[i];
			m_vecTarget[i] = m_vecTarget[iRand];
			m_vecTarget[iRand] = pTmp;
		}
	}
	
	return S_OK;
}

int CChain::Update()
{
	CGameObject::LateInit();


	if (m_bIsDead)
		return OBJ_DEAD;
	else
	{
		if (nullptr == m_pPlayer)
			m_bIsDead = true;
				
		Attack();

		/*if (m_tPosAlarm.CheckAlarm())
		{
			m_vecPrevPos.push_back(m_tInfo.vPos);
		}*/
	}
	
	return NO_EVENT;
}

int CChain::LateUpdate()
{
	CGameObject::UpdateView();

	return NO_EVENT;
}

void CChain::Render()
{
	if (nullptr == m_pPlayer)
		return;

	m_fSinMove *= -1.f;

	D3DXVECTOR3 vMoveDir = m_tInfo.vPos - m_pPlayer->GetInfo().vPos;

	D3DXMATRIX matScale, matRotZ, matTrans;

	if (0.f > m_fAngle)
		m_fAngle += PI * 2.f;

	// 체인 링
	

	D3DXVECTOR3 vStartPos;
	D3DXVECTOR3 vEndPos;
	D3DXVECTOR3 vDist;
	D3DXVECTOR3 vRingPos;

	int iRand = rand() % 9;
	m_pTexInfo = m_pTextureMgr->GetTexInfo(L"Effect", L"ElecShock", iRand);//L"ChainRing", 0);
	NULL_CHECK(m_pTexInfo);

	// 이전에 들렸던 노드
	for (int i= m_iCurTarget; i> 0; --i)
	{
		if (nullptr != m_vecTarget[i] && !m_vecTarget[i]->GetDead() &&
			nullptr != m_vecTarget[i-1] && !m_vecTarget[i-1]->GetDead()) 
		{
			vStartPos = m_vecTarget[i - 1]->GetInfo().vPos - D3DXVECTOR3 { 0.f, m_pPlayer->GetOBB().vCollSize.y * 0.5f, 0.f };
			vEndPos = m_vecTarget[i]->GetInfo().vPos - D3DXVECTOR3{ 0.f, m_pPlayer->GetOBB().vCollSize.y * 0.5f, 0.f };

			vDist = vEndPos - vStartPos;

			float fLength = D3DXVec3Length(&vDist);
			int iNum = int(fLength / 40);//m_pTexInfo->tImgInfo.Height);

			vDist /= float(iNum);

			for (int j = 0; j < iNum; ++j)
			{
				int iRand = rand() % 9;
				m_pTexInfo = m_pTextureMgr->GetTexInfo(L"Effect", L"ElecShock", iRand);//L"ChainRing", 0);
				NULL_CHECK(m_pTexInfo);

				D3DXVECTOR3 vSin;
				D3DXVec3Normalize(&vSin, &vDist);
				if (0 == j%2)
					D3DXVec3Cross(&vSin, &D3DXVECTOR3{ 0.f, 0.f, 1.f }, &vSin);
				else
					D3DXVec3Cross(&vSin, &vSin, &D3DXVECTOR3{ 0.f, 0.f, 1.f });

				vRingPos = vStartPos + vDist * float(j) + vSin * m_fSinMove * 10.f;
				
				D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
				D3DXMatrixRotationZ(&matRotZ, -m_fAngle);
				D3DXMatrixTranslation(&matTrans, vRingPos.x, vRingPos.y, 0.f);
				m_tInfo.matWorld = matScale * matRotZ * matTrans;
				m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
				m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
					&m_pTexInfo->vOffset, nullptr, D3DCOLOR_ARGB(200, 255, 255, 255)/*m_pTexInfo->dwBlend*/);
			}
		}
	}

	// 현재 이동하고있는 노드
	if (nullptr != m_vecTarget[m_iCurTarget] && !m_vecTarget[m_iCurTarget]->GetDead()) 
	{
		vStartPos = m_vecTarget[m_iCurTarget]->GetInfo().vPos - D3DXVECTOR3{ 0.f, m_pPlayer->GetOBB().vCollSize.y * 0.5f, 0.f };
		vEndPos = m_tInfo.vPos;

		vDist = vEndPos - vStartPos;

		float fLength = D3DXVec3Length(&vDist);
		int iNum = int(fLength / 40); // m_pTexInfo->tImgInfo.Height);

		vDist /= float(iNum);

		for (int j = 0; j < iNum; ++j)
		{
			int iRand = rand() % 9;
			m_pTexInfo = m_pTextureMgr->GetTexInfo(L"Effect", L"ElecShock", iRand);//L"ChainRing", 0);
			NULL_CHECK(m_pTexInfo);

			D3DXVECTOR3 vSin;
			D3DXVec3Normalize(&vSin, &vDist);
			if (0 == j % 2)
				D3DXVec3Cross(&vSin, &D3DXVECTOR3{ 0.f, 0.f, 1.f }, &vSin);
			else
				D3DXVec3Cross(&vSin, &vSin, &D3DXVECTOR3{ 0.f, 0.f, 1.f });

			vRingPos = vStartPos + vDist * float(j) + vSin * m_fSinMove * 10.f;

			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
			D3DXMatrixRotationZ(&matRotZ, -m_fAngle);
			D3DXMatrixTranslation(&matTrans, vRingPos.x, vRingPos.y, 0.f);
			m_tInfo.matWorld = matScale * matRotZ * matTrans;
			m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
			m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
				&m_pTexInfo->vOffset, nullptr, D3DCOLOR_ARGB(200, 255, 255, 255));
		}
	}

	/*for (auto& RingPos : m_vecPrevPos)
	{
		D3DXMatrixScaling(&matScale, 2.f, 2.f, 0.f);
		D3DXMatrixRotationZ(&matRotZ, -m_fAngle);
		D3DXMatrixTranslation(&matTrans, RingPos.x, RingPos.y, 0.f);
		m_tInfo.matWorld = matScale * matRotZ * matTrans;
		m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
			&m_pTexInfo->vOffset, nullptr, m_pTexInfo->dwBlend);
	}*/

	// 체인 머리
	m_pTexInfo = m_pTextureMgr->GetTexInfo(L"Effect", L"ChainHead", 0);
	NULL_CHECK(m_pTexInfo);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixRotationZ(&matRotZ, -m_fAngle + PI * 0.25f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	m_tInfo.matWorld = matScale * matRotZ * matTrans;
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&m_pTexInfo->vOffset, nullptr, m_pTexInfo->dwBlend);
}


void CChain::FrameOnceEnd()
{
	
}

void CChain::FrameEvent()
{
	
}

void CChain::AtarMoveEnd()
{
	
}

void CChain::AstarMoveSetState()
{
	
}

bool CChain::AstartExitEvent()
{
	return false;
}

void CChain::Attack()
{
	if (!m_bIsGrab)
	{
		if (nullptr != m_pTarget)
		{
			if (m_pTarget->GetDead()) {
				if (m_vecTarget.begin() + m_iCurTarget + 1 == m_vecTarget.end())
				{
					m_bIsDead = true;
				}
				else
				{
					m_pTarget = m_vecTarget[++m_iCurTarget];
					//m_vecTarget.pop_back();
				}
			}
			else
			{
				m_tInfo.vDir = (m_pTarget->GetInfo().vPos - D3DXVECTOR3{ 0.f, m_pPlayer->GetOBB().vCollSize.y * 0.5f, 0.f }) - m_tInfo.vPos;
				m_fAngle = CGameObject::GetRadianAngle(m_tInfo.vDir);

				D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
				m_tInfo.vPos += m_tInfo.vDir * m_fSpeed *m_pTimeMgr->GetDeltaTime();

				if (CCollisionMgr::CheckOneToOne(m_tOBB, m_pTarget->GetOBB()))
				{
					CGameObject* pAttackEffect =
						CreateEffect(
							m_pTarget->GetInfo().vPos, D3DXVECTOR3{ float(rand() % 20 - 10), float(rand() % 20 - 10) - m_pTarget->GetOBB().vCollSize.y * 0.5f, 0.f },
							L"Effect", L"Hit", true, OBJ_EFFECT, m_pTarget, m_pPlayer, FRAME_ONCE);

					dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamage(500 + rand() % 500);
					dynamic_cast<CAttackEffect*>(pAttackEffect)->SetAttackCount(5);
					dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamageTiming(1);


					if (m_vecTarget.begin() + m_iCurTarget + 1 == m_vecTarget.end())
					{
						m_bIsGrab = true;
					}
					else
					{
						m_pTarget = m_vecTarget[++m_iCurTarget];
					}
				}
			}
		}
	}
	else
	{
		m_bIsDead = true;
		return;
	}
}

void CChain::Hit()
{
}