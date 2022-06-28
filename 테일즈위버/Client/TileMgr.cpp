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

	// ���
	if (nullptr != m_pBackInfo) {
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		m_pDeviceMgr->GetSprite()->SetTransform(&matScale);
		m_pDeviceMgr->GetSprite()->Draw(m_pBackInfo->pTexture, nullptr,
			&m_pBackInfo->vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	// Ÿ��
	if (CSceneMgr::GetInstance()->GetEvent(EVENT_TILE_RENDER))
	{
		float fViewX = CViewMgr::GetInstance()->GetViewX();
		float fViewY = CViewMgr::GetInstance()->GetViewY();

		for (size_t i = 0; i < m_vecTile.size(); ++i)
		{
			// ���� �ø�
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
	// ���� (��ī��Ʈ ����)

	// �������� ������ �ð�������� 12��, 3��, 6��, 9�ø� ���Ѵ�.
	D3DXVECTOR3 vPoint[4] =
	{
		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y + TILECY * 0.5f, 0.f),
		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x + TILECX * 0.5f, m_vecTile[iIndex]->vPos.y, 0.f),
		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y - TILECY * 0.5f, 0.f),
		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x - TILECX * 0.5f, m_vecTile[iIndex]->vPos.y, 0.f),
	};

	// �ð�������� 4���� ���⺤�͸� ���Ѵ�.
	D3DXVECTOR3 vDir[4] =
	{
		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3]
	};

	// ������ ���� ���⺤���� �������͵��� ���Ѵ�.
	D3DXVECTOR3 vNormal[4] = {};

	for (int i = 0; i < 4; ++i)
		D3DXVec3Cross(&vNormal[i], &D3DXVECTOR3(0.f, 0.f, 1.f), &vDir[i]);

	// �� ���������� ���콺 ��ġ�� ���ϴ� ���� 4�� ���Ѵ�.
	D3DXVECTOR3 vMouseDir[4] =
	{
		vPos - vPoint[0],
		vPos - vPoint[1],
		vPos - vPoint[2],
		vPos - vPoint[3]
	};

	// vNormal�� vMouseDir�� �����ؼ� ��� ����(�а�)�� ������ true
	for (int i = 0; i < 4; ++i)
	{
		// �ϳ��� ���(����)�� ������ false
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

	// Ÿ�� �ε����� ���Ѵ�.
	m_iStart = GetTileIndex(vStartPos);
	m_iGoal = GetTileIndex(vGoalPos);

	if (m_iStart == -1 || m_iGoal == -1			// ��ġ�� �� �� ���� ���
		|| m_iStart == m_iGoal					// ���۰� �� ������ ���� ���
		|| m_vecTile[m_iGoal]->byOption == 1)	// �� ������ ���� ���ϴ� Ÿ���� ���
		return;
	
	ResetAstar();

	MakePath(pListBest, vStartPos, vGoalPos);
}

void CTileMgr::MakePath(list<D3DXVECTOR3>* pListBest, const D3DXVECTOR3 & vStartPos, const D3DXVECTOR3 & vGoalPos)
{
	// �ֻ��� ���
	ASTAR_NODE* pParent = new ASTAR_NODE;
	pParent->pParent = nullptr;
	pParent->iIndex = m_iStart;
	pParent->fCost = 0.f;

	m_lstClose.push_back(pParent);
	
	//D3DXVECTOR3 vDir = vGoalPos - vStartPos;

	// ã��
	while (true)
	{
		if (pParent->iIndex % (m_iTileX * 2) != (m_iTileX * 2) - 1) {
			// ������ ��
			if (pParent->iIndex >= m_iTileX)
				CheckOption(pParent->iIndex - (m_iTileX - (pParent->iIndex / m_iTileX) % 2), pParent);

			//������ �Ʒ�
			if (pParent->iIndex < (m_iTileX * m_iTileY) - m_iTileX)
				CheckOption(pParent->iIndex + (m_iTileX + (pParent->iIndex / m_iTileX) % 2), pParent);
		}

		
		if (pParent->iIndex % (m_iTileX * 2) != 0) {
			//���� �Ʒ�
			if (pParent->iIndex < (m_iTileX * m_iTileY) - m_iTileX)
				CheckOption(pParent->iIndex + (m_iTileX + (pParent->iIndex / m_iTileX) % 2) - 1, pParent);

			//���� ��
			if (pParent->iIndex >= m_iTileX)
				CheckOption(pParent->iIndex - (m_iTileX + (pParent->iIndex / m_iTileX + 1) % 2), pParent);
		}

		if (m_lstOpen.empty())
			break;


		// ����
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


		// ���� ��� ����
		if (pParent->iIndex == m_iGoal)
		{
			// �� ���� ���� �־���
			pListBest->push_front(m_vecTile[pParent->iIndex]->vPos);//vGoalPos);//pParent->iIndex);

			m_vecTile[pParent->iIndex]->byDrawID = 3;

			D3DXVECTOR3 vStart	= m_vecTile[pParent->iIndex]->vPos;//*pListBest->begin();
			D3DXVECTOR3 vEnd;

			while (true)
			{
				// ���⼭ �־��� �� ���� ������� �˻� (�浹Ÿ���� �ִ��� �˻�)
				
				// ���� �ȿ� �ִ� Ÿ�� �� pListBest->begin() -> pParent->pParent���� ���̿� ������ ����� �ִ��� �˻�
				// ������ ����� ������ pParent�� �־��ش�.
				
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

				// ����
				float fA = (vEnd.y - vStart.y) / (vEnd.x - vStart.x);
				
				for (size_t i =0; i< m_vecTile.size(); ++i)
				{
					if (m_vecTile[i]->vPos.x > fCurringX && m_vecTile[i]->vPos.x < fCurringCX
						&& m_vecTile[i]->vPos.y > fCurringY && m_vecTile[i]->vPos.y < fCurringCY)
					{
						// �� ��
						D3DXVECTOR3 vTilePoint[4] =
						{
							{ m_vecTile[i]->vPos.x, m_vecTile[i]->vPos.y - TILECY * 0.5f, 0.f },
							{ m_vecTile[i]->vPos.x, m_vecTile[i]->vPos.y + TILECY * 0.5f, 0.f },
							{ m_vecTile[i]->vPos.x + TILECX * 0.5f, m_vecTile[i]->vPos.y, 0.f },
							{ m_vecTile[i]->vPos.x - TILECX * 0.5f, m_vecTile[i]->vPos.y, 0.f }
						};

						// ���� ������
						// y - (y2 - y1) / (x2 - x2) * (x - x1 + y1)
						float fInLine = -vTilePoint[0].y + fA * (vTilePoint[0].x - vStart.x) + vStart.y;

						bool bIsInner = false;

						for (int j = 1; j < 4; ++j)
						{
							fInLine *= -vTilePoint[j].y + fA * (vTilePoint[j].x - vStart.x) + vStart.y;

							// ������ �� �ȿ� ����
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

		// ����Ʈ ��� �� �������� �� �� �ִ� ��� ã��

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

// ����Ʈ�� �ش� �ε����� �ִ��� ����
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

// Ÿ�� �ɼ� üũ
void CTileMgr::CheckOption(const int & iIndex, ASTAR_NODE* pParent)
{
	if (1 != m_vecTile[iIndex]->byOption && CheckIndex(iIndex))
	{
		ASTAR_NODE* pNode = MakeNode(iIndex, pParent);
		m_lstOpen.push_back(pNode);
	}
}