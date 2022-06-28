#include "stdafx.h"
#include "TileMgr.h"

IMPLEMENT_SINGLETON(CTileMgr)

CTileMgr::CTileMgr()
	:m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance())
{
}

CTileMgr::~CTileMgr()
{
	Release();
}

void CTileMgr::Initialize(int iTileX, int iTileY, int iMapNum)
{
	Release();

	m_iTileX = iTileX;
	m_iTileY = iTileY;

	m_iMapNum = iMapNum;

	m_pBackInfo = m_pTextureMgr->GetTexInfo(L"Background", L"Map", iMapNum);
	
	m_vecTile.reserve(m_iTileX * m_iTileY);
}

void CTileMgr::Render()
{
	D3DXMATRIX matScale, matTrans, matWorld;
	const TEX_INFO* pTexInfo = nullptr;

	// 배경
	if (nullptr != m_pBackInfo) {
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		m_pDeviceMgr->GetSprite()->SetTransform(&matScale);
		m_pDeviceMgr->GetSprite()->Draw(m_pBackInfo->pTexture, nullptr,
			&m_pBackInfo->vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	// 타일
	if (CSceneMgr::GetInstance()->GetEvent(EVENT_TILE_RENDER))
	{
		float fViewX = CViewMgr::GetInstance()->GetViewX();
		float fViewY = CViewMgr::GetInstance()->GetViewY();

		for (size_t i = 0; i < m_vecTile.size(); ++i)
		{
			// 렌더 컬링
			if ((m_vecTile[i]->vPos.x < fViewX - WINCX * 0.55f
				|| m_vecTile[i]->vPos.x  > fViewX + WINCX * 0.55f
				|| m_vecTile[i]->vPos.y < fViewY - WINCY * 0.55f
				|| m_vecTile[i]->vPos.y > fViewY + WINCY * 0.55f))
				continue;


			pTexInfo = m_pTextureMgr->GetTexInfo(m_vecTile[i]->wstrObjKey, m_vecTile[i]->wstrStateKey, m_vecTile[i]->byDrawID);
			if (nullptr == pTexInfo) continue;

			D3DXMatrixTranslation(&matTrans,
				m_vecTile[i]->vPos.x,
				m_vecTile[i]->vPos.y, 0.f);

			matWorld = matTrans;

			m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
			m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
				&pTexInfo->vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		}
	}
}

void CTileMgr::Release()
{
	ResetAstar();

	for_each(m_vecTile.begin(), m_vecTile.end(), SafeDelete<TILE_INFO*>);
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();

	m_iTileX = 0;
	m_iTileY = 0;

	m_pBackInfo = nullptr;
}

HRESULT CTileMgr::InsertTile(TILE_INFO* pTile)
{
	NULL_CHECK_MSG_RETURN(pTile, L"InsetTile nullptr Error", E_FAIL);

	m_vecTile.push_back(pTile);
	return S_OK;
}

int CTileMgr::GetTileIndex(const D3DXVECTOR3 & vPos)
{
	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		if (IsPicking(vPos, i))
			return (int)i;
	}

	return -1;
}

bool CTileMgr::IsPicking(const D3DXVECTOR3 & vPos, size_t iIndex)
{
	// 내적 (데카르트 기준)

	// 마름모의 꼭지점 시계방향으로 12시, 3시, 6시, 9시를 구한다.
	D3DXVECTOR3 vPoint[4] =
	{
		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y + TILECY * 0.5f, 0.f),
		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x + TILECX * 0.5f, m_vecTile[iIndex]->vPos.y, 0.f),
		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y - TILECY * 0.5f, 0.f),
		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x - TILECX * 0.5f, m_vecTile[iIndex]->vPos.y, 0.f),
	};

	// 시계방향으로 4개의 방향벡터를 구한다.
	D3DXVECTOR3 vDir[4] =
	{
		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3]
	};

	// 위에서 구한 방향벡터의 법선벡터들을 구한다.
	D3DXVECTOR3 vNormal[4] = {};

	for (int i = 0; i < 4; ++i)
		D3DXVec3Cross(&vNormal[i], &D3DXVECTOR3(0.f, 0.f, 1.f), &vDir[i]);

	// 각 꼭지점에서 마우스 위치로 향하는 벡터 4개 구한다.
	D3DXVECTOR3 vMouseDir[4] =
	{
		vPos - vPoint[0],
		vPos - vPoint[1],
		vPos - vPoint[2],
		vPos - vPoint[3]
	};

	// vNormal와 vMouseDir을 내적해서 모두 음수(둔각)가 나오면 true
	for (int i = 0; i < 4; ++i)
	{
		// 하나라도 양수(예각)가 나오면 false
		if (0.f < D3DXVec3Dot(&vNormal[i], &vMouseDir[i]))
			return false;
	}

	return true;
}

void CTileMgr::ResetAstar()
{
	for_each(m_lstOpen.begin(), m_lstOpen.end(), SafeDelete<ASTAR_NODE*>);
	m_lstOpen.clear();

	for_each(m_lstClose.begin(), m_lstClose.end(), SafeDelete<ASTAR_NODE*>);
	m_lstClose.clear();
}


void CTileMgr::AStarStartPos(list<D3DXVECTOR3>* pListBest, const D3DXVECTOR3 & vStartPos, const D3DXVECTOR3 & vGoalPos)
{
	NULL_CHECK(pListBest);

	pListBest->clear();

	// 타일 인덱스를 구한다.
	m_iStart = GetTileIndex(vStartPos);
	m_iGoal = GetTileIndex(vGoalPos);

	if (m_iStart == -1 || m_iGoal == -1			// 위치가 잘 못 됐을 경우
		|| m_iStart == m_iGoal					// 시작과 끝 지점이 같을 경우
		|| m_vecTile[m_iGoal]->byOption == 1)	// 끝 지점이 가지 못하는 타일인 경우
		return;
	
	ResetAstar();

	MakePath(pListBest, vStartPos, vGoalPos);
}

void CTileMgr::MakePath(list<D3DXVECTOR3>* pListBest, const D3DXVECTOR3 & vStartPos, const D3DXVECTOR3 & vGoalPos)
{
	// 최상위 노드
	ASTAR_NODE* pParent = new ASTAR_NODE;
	pParent->pParent = nullptr;
	pParent->iIndex = m_iStart;
	pParent->fCost = 0.f;

	m_lstClose.push_back(pParent);
	
	//D3DXVECTOR3 vDir = vGoalPos - vStartPos;

	// 찾기
	while (true)
	{
		if (pParent->iIndex % (m_iTileX * 2) != (m_iTileX * 2) - 1) {
			// 오른쪽 위
			if (pParent->iIndex >= m_iTileX)
				CheckOption(pParent->iIndex - (m_iTileX - (pParent->iIndex / m_iTileX) % 2), pParent);

			//오른쪽 아래
			if (pParent->iIndex < (m_iTileX * m_iTileY) - m_iTileX)
				CheckOption(pParent->iIndex + (m_iTileX + (pParent->iIndex / m_iTileX) % 2), pParent);
		}

		
		if (pParent->iIndex % (m_iTileX * 2) != 0) {
			//왼쪽 아래
			if (pParent->iIndex < (m_iTileX * m_iTileY) - m_iTileX)
				CheckOption(pParent->iIndex + (m_iTileX + (pParent->iIndex / m_iTileX) % 2) - 1, pParent);

			//왼쪽 위
			if (pParent->iIndex >= m_iTileX)
				CheckOption(pParent->iIndex - (m_iTileX + (pParent->iIndex / m_iTileX + 1) % 2), pParent);
		}

		if (m_lstOpen.empty())
			break;


		// 정렬
		m_lstOpen.sort(
			[](ASTAR_NODE* pNode1, ASTAR_NODE* pNode2)
		{
			return pNode1->fCost < pNode2->fCost;
		});

		auto iter = m_lstOpen.begin();
		if (nullptr == *iter)
			break;


		pParent = *iter;

		m_lstClose.push_back(*iter);
		m_lstOpen.erase(iter);


		// 최적 경로 저장
		if (pParent->iIndex == m_iGoal)
		{
			// 골 지점 먼저 넣어줌
			pListBest->push_front(m_vecTile[pParent->iIndex]->vPos);//vGoalPos);//pParent->iIndex);

			m_vecTile[pParent->iIndex]->byDrawID = 3;

			D3DXVECTOR3 vStart	= m_vecTile[pParent->iIndex]->vPos;//*pListBest->begin();
			D3DXVECTOR3 vEnd;

			while (true)
			{
				// 여기서 넣어줄 때 최적 경로인지 검사 (충돌타일이 있는지 검사)
				
				// 직선 안에 있는 타일 중 pListBest->begin() -> pParent->pParent와의 사이에 못가는 블록이 있는지 검사
				// 못가는 블록이 있으면 pParent를 넣어준다.
				
				if (pParent->pParent->iIndex == m_iStart)
					break;

				vEnd = m_vecTile[pParent->pParent->iIndex]->vPos;

				float fCurringX = vStart.x - TILECX * 0.5f;
				float fCurringCX = vEnd.x + TILECX * 0.5f;
				if (vStart.x > vEnd.x)
				{
					fCurringX = vEnd.x - TILECX * 0.5f;
					fCurringCX = vStart.x + TILECX * 0.5f;
				}

				float fCurringY = vStart.y - TILECY * 0.5f;
				float fCurringCY = vEnd.y + TILECY * 0.5f;
				if (vStart.y > vEnd.y)
				{
					fCurringY = vEnd.y - TILECY * 0.5f;
					fCurringCY = vStart.y + TILECY * 0.5f;
				}

				// 기울기
				float fA = (vEnd.y - vStart.y) / (vEnd.x - vStart.x);
				
				for (size_t i =0; i< m_vecTile.size(); ++i)
				{
					if (m_vecTile[i]->vPos.x > fCurringX && m_vecTile[i]->vPos.x < fCurringCX
						&& m_vecTile[i]->vPos.y > fCurringY && m_vecTile[i]->vPos.y < fCurringCY)
					{
						// 각 점
						D3DXVECTOR3 vTilePoint[4] =
						{
							{ m_vecTile[i]->vPos.x, m_vecTile[i]->vPos.y - TILECY * 0.5f, 0.f },
							{ m_vecTile[i]->vPos.x, m_vecTile[i]->vPos.y + TILECY * 0.5f, 0.f },
							{ m_vecTile[i]->vPos.x + TILECX * 0.5f, m_vecTile[i]->vPos.y, 0.f },
							{ m_vecTile[i]->vPos.x - TILECX * 0.5f, m_vecTile[i]->vPos.y, 0.f }
						};

						// 직선 방정식
						// y - (y2 - y1) / (x2 - x2) * (x - x1 + y1)
						float fInLine = -vTilePoint[0].y + fA * (vTilePoint[0].x - vStart.x) + vStart.y;

						bool bIsInner = false;

						for (int j = 1; j < 4; ++j)
						{
							fInLine *= -vTilePoint[j].y + fA * (vTilePoint[j].x - vStart.x) + vStart.y;

							// 마름모가 선 안에 있음
							if (0.f >= fInLine)
							{
								if (1 == m_vecTile[i]->byOption)
								{
									m_vecTile[pParent->iIndex]->byDrawID = 3;
									pListBest->push_front(m_vecTile[pParent->iIndex]->vPos);
									vStart = *pListBest->begin();
									bIsInner = true;
									break;
								}
							}
						}
						if (bIsInner)
							break;
					}
				}


				pParent = pParent->pParent;
			}
			break;
		}

		// 베스트 경로 중 직선으로 갈 수 있는 경로 찾기

	}
}


ASTAR_NODE* CTileMgr::MakeNode(int iIndex, ASTAR_NODE* pParent)
{
	ASTAR_NODE* pNode = new ASTAR_NODE;
	pNode->iIndex = iIndex;
	pNode->pParent = pParent;

	D3DXVECTOR3 vDistance = m_vecTile[iIndex]->vPos - m_vecTile[m_iStart]->vPos;//m_vecTile[pParent->iIndex]->vPos;
	float fGoal = D3DXVec3Length(&vDistance); // g (goal)

	vDistance = m_vecTile[iIndex]->vPos - m_vecTile[m_iGoal]->vPos;
	float fHeuristic = D3DXVec3Length(&vDistance); // h (heuristic)

	pNode->fCost = fGoal + fHeuristic; // f = g + h

	return pNode;
}

// 리스트에 해당 인덱스가 있는지 조사
bool CTileMgr::CheckIndex(const int & iIndex)
{
	for (auto& iter : m_lstOpen)
	{
		if (iter->iIndex == iIndex)
			return false;
	}

	for (auto& iter : m_lstClose)
	{
		if (iter->iIndex == iIndex)
			return false;
	}

	return true;
}

// 타일 옵션 체크
void CTileMgr::CheckOption(const int & iIndex, ASTAR_NODE* pParent)
{
	if (1 != m_vecTile[iIndex]->byOption && CheckIndex(iIndex))
	{
		ASTAR_NODE* pNode = MakeNode(iIndex, pParent);
		m_lstOpen.push_back(pNode);
	}
}