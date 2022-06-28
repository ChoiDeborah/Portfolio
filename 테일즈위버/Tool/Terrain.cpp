#include "stdafx.h"
#include "Terrain.h"
#include "ToolView.h"
#include "MainDialog.h"

#include "EventObj.h"

CTerrain::CTerrain()
{
}


CTerrain::~CTerrain()
{
	Release();
}

int CTerrain::GetTileIndex(const D3DXVECTOR3& vPos)
{
	for (size_t i = 0; i < m_pMainDialog->m_MapTool.m_pMapData->vecTile.size(); ++i)
	{
		if (IsPicking(vPos, i))
			return (int)i;
	}

	return -1;
}

void CTerrain::SetView(CToolView* pView)
{
	m_pView = pView;
}

void CTerrain::SetMainDialog(CMainDialog * pMainDialog)
{
	m_pMainDialog = pMainDialog;
}


void CTerrain::Update()
{
	switch (m_pMainDialog->m_Tab.GetCurSel())
	{
	case SPRITE_TOOL:	SpriteToolUpdate(); break;	// ��������Ʈ ��
	case UNIT_TOOL:		UnitToolUpdate();	break;	// ���� ��
	case MAP_TOOL:		MapToolUpdate();	break;	// �� ��
	case WORD_TOOL:		WordToolUpdate();	break;	// ���� ��
	case EVENT_TOOL:	EventToolUpdate();	break;	// �̺�Ʈ ��
	}
}

void CTerrain::Render()
{
	switch (m_pMainDialog->m_Tab.GetCurSel())
	{
	case SPRITE_TOOL:	SpriteToolRender();		break;	// ��������Ʈ ��
	case UNIT_TOOL:		UnitToolRender();		break;	// ���� ��
	case MAP_TOOL:		MapToolRender();		break;	// �� ��
	case WORD_TOOL:		WordToolRender();		break;	// ���� ��
	case EVENT_TOOL:	EventToolRender();		break;	// �̺�Ʈ ��
	}
}

HRESULT CTerrain::Initialize()
{
	HRESULT hr = m_pTextureMgr->LoadTexture(CTextureMgr::MULTI_TEXTURE,
		L"../Texture/BackGround/Tile/TileOption%d.png",
		L"BackGround", L"TileOption", -0.5f, -0.5f, 5);
	FAILED_CHECK_MSG_RETURN(hr, L"Terrain Tile Texture Load Failed", E_FAIL);

	hr = m_pTextureMgr->LoadTexture(CTextureMgr::MULTI_TEXTURE,
		L"../Texture/BackGround/Map/Map%d.png",
		L"BackGround", L"Map", 0.f, 0.f, 11);
	FAILED_CHECK_MSG_RETURN(hr, L"Terrain Map Texture Load Failed", E_FAIL);

	return S_OK;
}

void CTerrain::Release()
{
}

void CTerrain::TileChange(
	const D3DXVECTOR3& vPos, BYTE byDrawID, BYTE byOption)
{
	NULL_CHECK(m_pMainDialog->m_MapTool.m_pMapData);

	// ������Ʈ ��ŷ
	if (0 == m_pMainDialog->m_MapTool.m_iRadio) {
		NULL_CHECK(m_pMainDialog->m_MapTool.m_pUnitInfo);

		int iIndex = GetTileIndex(vPos);

		TILE_INFO* pTileInfo = new TILE_INFO;
		pTileInfo->vPos			= m_pMainDialog->m_MapTool.m_pMapData->vecTile[iIndex]->vPos;
		pTileInfo->wstrName		= m_pMainDialog->m_MapTool.m_pUnitInfo->wstrName;
		pTileInfo->wstrObjKey	= m_pMainDialog->m_MapTool.m_pUnitInfo->wstrObjKey;
		pTileInfo->wstrStateKey = m_pMainDialog->m_MapTool.m_pUnitInfo->wstrStateKey;
		pTileInfo->byOption		= m_pMainDialog->m_MapTool.m_pUnitInfo->byOption;

		m_pMainDialog->m_MapTool.m_pMapData->vecObj.push_back(pTileInfo);

		TCHAR szPos[MAX_STR] = L"";
		swprintf_s(szPos, L"\t | (%.2f, %.2f)", pTileInfo->vPos.x, pTileInfo->vPos.y);
		m_pMainDialog->m_MapTool.m_ListBoxObject.AddString((pTileInfo->wstrObjKey + szPos).c_str());
	}
	// Ÿ�� ��ŷ
	else if (1 == m_pMainDialog->m_MapTool.m_iRadio) {
		int iIndex = GetTileIndex(vPos);
		if (-1 == iIndex)
			return;

		int iDrawID = m_pMainDialog->m_MapTool.m_ListBoxTile.GetCurSel();
		if (iDrawID != m_pMainDialog->m_MapTool.m_pMapData->vecTile[iIndex]->byDrawID) {
			m_pMainDialog->m_MapTool.m_pMapData->vecTile[iIndex]->byDrawID = iDrawID;
			m_pMainDialog->m_MapTool.m_pMapData->vecTile[iIndex]->byOption = iDrawID;
		}
	}
}

bool CTerrain::IsPicking(const D3DXVECTOR3 & vPos, size_t iIndex)
{
	auto* pTileVec = &m_pMainDialog->m_MapTool.m_pMapData->vecTile;

	// ���� (��ī��Ʈ ����)

	// �������� ������ �ð�������� 12��, 3��, 6��, 9�ø� ���Ѵ�.
	D3DXVECTOR3 vPoint[4] =
	{
		D3DXVECTOR3(m_pMainDialog->m_MapTool.m_pMapData->vecTile[iIndex]->vPos.x, m_pMainDialog->m_MapTool.m_pMapData->vecTile[iIndex]->vPos.y + TILECY * 0.5f, 0.f),
		D3DXVECTOR3(m_pMainDialog->m_MapTool.m_pMapData->vecTile[iIndex]->vPos.x + TILECX * 0.5f, m_pMainDialog->m_MapTool.m_pMapData->vecTile[iIndex]->vPos.y, 0.f),
		D3DXVECTOR3(m_pMainDialog->m_MapTool.m_pMapData->vecTile[iIndex]->vPos.x, m_pMainDialog->m_MapTool.m_pMapData->vecTile[iIndex]->vPos.y - TILECY * 0.5f, 0.f),
		D3DXVECTOR3(m_pMainDialog->m_MapTool.m_pMapData->vecTile[iIndex]->vPos.x - TILECX * 0.5f, m_pMainDialog->m_MapTool.m_pMapData->vecTile[iIndex]->vPos.y, 0.f),
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

// ��������Ʈ �� ������Ʈ
void CTerrain::SpriteToolUpdate()
{
}

// ���� �� ������Ʈ
void CTerrain::UnitToolUpdate()
{
	if (0 == m_pMainDialog->m_UnitTool.m_Tab.GetCurSel()) {
		UNIT_INFO* pUnitInfo = &m_pMainDialog->m_UnitTool.m_MonsterSetting.m_tUnitInfo;

		if (m_pKeyMgr->KeyPressing(VK_LBUTTON))
		{
			POINT point = {};
			GetCursorPos(&point);
			::ScreenToClient(m_pView->m_hWnd, &point);

			CRect rt;
			m_pView->GetClientRect(&rt);

			if (0 <= point.x && 0 <= point.y &&
				rt.right >= point.x && rt.bottom >= point.y) {

				D3DXVECTOR3 vMouse =
				{
					float(point.x) - VIEWCX * 0.5f,//GetScrollPos(0),
					float(point.y) - VIEWCY * 0.5f,//GetScrollPos(1),
					0.f
				};

				if (!m_ObjectCollRender) {
					pUnitInfo->vCollStart = vMouse;
					pUnitInfo->vCollEnd = vMouse;
					m_ObjectCollRender = true;
				}
				else
					pUnitInfo->vCollEnd = vMouse;
				
				m_pMainDialog->m_UnitTool.m_MonsterSetting.UpdateData(FALSE);
			}
		}
		else {
			m_ObjectCollRender = false;
		}

		
	}
}

// �� �� ������Ʈ
void CTerrain::MapToolUpdate()
{
	if ((0 == m_pMainDialog->m_MapTool.m_iRadio && m_pKeyMgr->KeyDown(VK_LBUTTON)) ||
		(1 == m_pMainDialog->m_MapTool.m_iRadio && m_pKeyMgr->KeyPressing(VK_LBUTTON))) {
		POINT point = {};
		GetCursorPos(&point);
		::ScreenToClient(m_pView->m_hWnd, &point);

		CRect rt;
		m_pView->GetClientRect(&rt);

		if (0 <= point.x && 0 <= point.y &&
			rt.right >= point.x && rt.bottom >= point.y) {

			D3DXVECTOR3 vMouse =
			{
				float(point.x) + m_pViewMgr->GetViewX(),//GetScrollPos(0),
				float(point.y) + m_pViewMgr->GetViewY(),//GetScrollPos(1),
				0.f
			};

			TileChange(vMouse, 0, 0);
		}
	}
}

void CTerrain::WordToolUpdate()
{
}

void CTerrain::EventToolUpdate()
{
	NULL_CHECK(m_pMainDialog->m_EventTool.m_pSeleteNotify);

	if (m_pKeyMgr->KeyPressing(VK_LBUTTON)) {
		POINT point = {};
		GetCursorPos(&point);
		::ScreenToClient(m_pView->m_hWnd, &point);

		CRect rt;
		m_pView->GetClientRect(&rt);

		if (0 <= point.x && 0 <= point.y &&
			rt.right >= point.x && rt.bottom >= point.y) {

			D3DXVECTOR3 vMouse =
			{
				float(point.x) + m_pViewMgr->GetViewX(),//GetScrollPos(0),
				float(point.y) + m_pViewMgr->GetViewY(),//GetScrollPos(1),
				0.f
			};

			m_pMainDialog->m_EventTool.UpdateData(TRUE);
			m_pMainDialog->m_EventTool.m_tCurNotify.vPos = vMouse;
			m_pMainDialog->m_EventTool.UpdateData(FALSE);
			m_pMainDialog->m_EventTool.Invalidate(FALSE);
			m_pMainDialog->m_EventTool.OnBnClickedNotifyUpdate();
		}
	}
}


// ��������Ʈ �� ����
void CTerrain::SpriteToolRender()
{
	D3DXMATRIX matScale, matTrans, matWorld;
	const TEX_INFO* pTexInfo = nullptr;

	// ���� Ÿ��
	pTexInfo = m_pTextureMgr->GetTexInfo(L"Tile100", L"", 0);
	NULL_CHECK(pTexInfo);
	D3DXMatrixTranslation(&matTrans, VIEWCX * 0.5f, VIEWCY * 0.5f, 0.f);
	m_pDeviceMgr->GetSprite()->SetTransform(&matTrans);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3{ pTexInfo->tImgInfo.Width * 0.5f, pTexInfo->tImgInfo.Height * 0.5f, 0.f }, nullptr, pTexInfo->dwBlend);


	// ���� ���̾�
	pTexInfo = m_pTextureMgr->GetTexInfo(L"BackGround", L"TileOption", 4);
	NULL_CHECK(pTexInfo);
	D3DXMatrixTranslation(&matTrans, VIEWCX * 0.5f, VIEWCY * 0.5f, 0.f);
	m_pDeviceMgr->GetSprite()->SetTransform(&matTrans);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&pTexInfo->vOffset, nullptr, pTexInfo->dwBlend);


	SPRITE_INFO* pSpriteInfo = &m_pMainDialog->m_SpriteTool.m_tSpriteInfo;
	NULL_CHECK(pSpriteInfo);

	wstring wstrObjKey = m_pMainDialog->m_SpriteTool.m_wstrObjKey;
	wstring wstrStateKey = m_pMainDialog->m_SpriteTool.m_wstrStateKey;

	// �̹���
	pTexInfo = m_pTextureMgr->GetTexInfo(wstrObjKey, wstrStateKey, m_pMainDialog->m_SpriteTool.m_tFrame.iFrameCur);
	NULL_CHECK(pTexInfo);

	D3DXMatrixScaling(&matScale,
		float(pSpriteInfo->iMirrorX), 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, VIEWCX * 0.5f, VIEWCY * 0.5f, 0.f);

	float fOffsetX = (0 > pSpriteInfo->vOffset.x) ? -pSpriteInfo->vOffset.x * pTexInfo->tImgInfo.Width : pSpriteInfo->vOffset.x;
	float fOffsetY = (0 > pSpriteInfo->vOffset.y) ? -pSpriteInfo->vOffset.y * pTexInfo->tImgInfo.Height : pSpriteInfo->vOffset.y;

	// �簢�� ������
	if (m_pMainDialog->m_SpriteTool.m_CheckDrawLine.GetCheck()){
		m_pDeviceMgr->GetSprite()->End();
		m_pDeviceMgr->GetDevice()->EndScene();

		m_pDeviceMgr->GetDevice()->BeginScene();
		m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

		D3DXVECTOR2 pLine[5] = {};
		D3DXVECTOR3	vVertex[4] = {
			{ -fOffsetX, -fOffsetY, 0.f },
			{ pTexInfo->tImgInfo.Width - fOffsetX, -fOffsetY, 0.f },
			{ pTexInfo->tImgInfo.Width - fOffsetX, pTexInfo->tImgInfo.Height - fOffsetY, 0.f },
			{ -fOffsetX, pTexInfo->tImgInfo.Height - fOffsetY, 0.f }
		};

		for (int i = 0; i < 4; ++i) {
			D3DXVECTOR3 vTmp;
			D3DXVec3TransformCoord(&vTmp, &vVertex[i], &(matScale * matTrans));
			pLine[i] = { vTmp.x, vTmp.y };
		}
		pLine[4] = pLine[0];
		m_pDeviceMgr->GetLine()->Draw(pLine, 5, D3DCOLOR_ARGB(255, 0, 255, 255));

		m_pDeviceMgr->GetSprite()->End();
		m_pDeviceMgr->GetDevice()->EndScene();

		m_pDeviceMgr->GetDevice()->BeginScene();
		m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
	}

	matWorld = matScale * matTrans;
	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);

	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3{ fOffsetX, fOffsetY, 0.f }, nullptr, pSpriteInfo->GetBlendColor());
}

// ���� �� ����
void CTerrain::UnitToolRender()
{
	D3DXMATRIX matScale, matTrans, matWorld;
	const TEX_INFO* pTexInfo = nullptr;

	// �� ��� 
	pTexInfo = m_pTextureMgr->GetTexInfo(L"BackGround", L"Map", 3);
	if (nullptr != pTexInfo) {
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, float(-m_pView->GetScrollPos(0)), float(-m_pView->GetScrollPos(1)), 0.f);
		matWorld = matScale * matTrans;
		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&pTexInfo->vOffset, nullptr, pTexInfo->dwBlend);
	}


	// ���� ���̾�
	pTexInfo = m_pTextureMgr->GetTexInfo(L"BackGround", L"TileOption", 3);
	if (nullptr != pTexInfo) {
		D3DXMatrixTranslation(&matTrans, VIEWCX * 0.5f, VIEWCY * 0.5f, 0.f);
		m_pDeviceMgr->GetSprite()->SetTransform(&matTrans);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&pTexInfo->vOffset, nullptr, pTexInfo->dwBlend);
	}

	// �̹���
	pTexInfo = m_pTextureMgr->GetTexInfo(m_pMainDialog->m_UnitTool.m_tObjectInfo.wstrObjKey, m_pMainDialog->m_UnitTool.m_tObjectInfo.wstrStateKey, m_pMainDialog->m_UnitTool.m_tFrame.iFrameCur);
	if (nullptr != pTexInfo) {
		D3DXMatrixScaling(&matScale, float(pTexInfo->m_iMirror), 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, VIEWCX * 0.5f, VIEWCY * 0.5f, 0.f);
		matWorld = matScale * matTrans;
		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);

		const TEX_INFO* pShadowInfo = m_pTextureMgr->GetTexInfo(L"Shadow");
		if (nullptr != pShadowInfo) {
			m_pDeviceMgr->GetSprite()->Draw(pShadowInfo->pTexture, nullptr,
				&D3DXVECTOR3{ pShadowInfo->tImgInfo.Width * 0.5f, pShadowInfo->tImgInfo.Height * 0.4f, 0.f }, nullptr, pShadowInfo->dwBlend);
		}

		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&pTexInfo->vOffset, nullptr, pTexInfo->dwBlend);


		// �浹 ����
		if (0 == m_pMainDialog->m_UnitTool.m_Tab.GetCurSel()) {
			if (m_pMainDialog->m_UnitTool.m_MonsterSetting.m_CheckShowCollRect.GetCheck()) {
				m_pDeviceMgr->GetSprite()->End();
				m_pDeviceMgr->GetDevice()->EndScene();

				UNIT_INFO* pUnitInfo = &m_pMainDialog->m_UnitTool.m_MonsterSetting.m_tUnitInfo;

				D3DXVECTOR2 pLine[5] = {};
				D3DXVECTOR3	vVertex[4] = {
					{ pUnitInfo->vCollStart.x, pUnitInfo->vCollStart.y, 0.f },
					{ pUnitInfo->vCollEnd.x, pUnitInfo->vCollStart.y, 0.f },
					{ pUnitInfo->vCollEnd.x, pUnitInfo->vCollEnd.y, 0.f },
					{ pUnitInfo->vCollStart.x, pUnitInfo->vCollEnd.y, 0.f },
				};

				for (int j = 0; j < 4; ++j) {
					D3DXVECTOR3 vTmp;
					D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
					D3DXVec3TransformCoord(&vTmp, &vVertex[j], &(matScale * matTrans));
					pLine[j] = { vTmp.x, vTmp.y };
				}
				pLine[4] = pLine[0];
				m_pDeviceMgr->GetLine()->Draw(pLine, 5, D3DCOLOR_ARGB(255, 0, 255, 0));

				m_pDeviceMgr->GetDevice()->BeginScene();
				m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
			}
		}
	}


	// ��� ȿ��
	pTexInfo = m_pTextureMgr->GetTexInfo(L"Dark");
	if (nullptr != pTexInfo) {
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, VIEWCX * 0.5f - pTexInfo->tImgInfo.Width * 0.5f, VIEWCY * 0.5f - pTexInfo->tImgInfo.Height * 0.5f, 0.f);
		matWorld = matScale * matTrans;
		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&pTexInfo->vOffset, nullptr, pTexInfo->dwBlend);
	}
}

// �� �� ����
void CTerrain::MapToolRender()
{
	D3DXMATRIX matScale, matTrans, matWorld;
	const TEX_INFO* pTexInfo = nullptr;
	
	if (nullptr != m_pMainDialog->m_MapTool.m_pMapData)
	{
		int iMapNum = m_pMainDialog->m_MapTool.m_pMapData->iMapImageNum;
		const TEX_INFO* pBackInfo = m_pTextureMgr->GetTexInfo(L"BackGround", L"Map", iMapNum);

		// �� ��� 
		if (nullptr != pBackInfo) {
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
			D3DXMatrixTranslation(&matTrans, float(-m_pViewMgr->GetViewX()), float(-m_pViewMgr->GetViewY()), 0.f);
			matWorld = matScale * matTrans;
			m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
			m_pDeviceMgr->GetSprite()->Draw(pBackInfo->pTexture, nullptr,
				&pBackInfo->vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}

	// Ÿ��
	TILE_INFO* pTileInfo = nullptr;
	NULL_CHECK(m_pMainDialog->m_MapTool.m_pMapData);
	if (m_pMainDialog->m_MapTool.m_ButtonDrawLine.GetCheck()) {
		// Ÿ�� ���
		for (size_t i = 0; i < m_pMainDialog->m_MapTool.m_pMapData->vecTile.size(); ++i)
		{
			pTileInfo = m_pMainDialog->m_MapTool.m_pMapData->vecTile[i];

			pTexInfo = m_pTextureMgr->GetTexInfo(L"BackGround", L"TileOption", pTileInfo->byDrawID);
			NULL_CHECK(pTexInfo);

			D3DXMatrixScaling(&matScale,
				pTileInfo->vSize.x,
				pTileInfo->vSize.y, 0.f);
			D3DXMatrixTranslation(&matTrans,
				pTileInfo->vPos.x - m_pViewMgr->GetViewX(), //m_pView->GetScrollPos(0)
				pTileInfo->vPos.y - m_pViewMgr->GetViewY(), //m_pView->GetScrollPos(1)
				0.f);

			matWorld = matScale * matTrans;

			m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
			m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
				&pTexInfo->vOffset, nullptr, pTexInfo->dwBlend);
		}

		/*// ���̷�Ʈ ��Ʈ ���
		for (size_t i = 0; i < m_pMainDialog->m_MapTool.m_pMapData->vecTile.size(); ++i)
		{
			pTileInfo = m_pMainDialog->m_MapTool.m_pMapData->vecTile[i];
			
			D3DXMatrixScaling(&matScale,
				pTileInfo->vSize.x,
				pTileInfo->vSize.y, 0.f);
			D3DXMatrixTranslation(&matTrans,
				pTileInfo->vPos.x - m_pViewMgr->GetViewX(), //m_pView->GetScrollPos(0)
				pTileInfo->vPos.y - m_pViewMgr->GetViewY(), //m_pView->GetScrollPos(1)
				0.f);

			matWorld = matScale * matTrans;

			m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);

			// ���̷�Ʈ ��Ʈ ���
			TCHAR szIndex[MAX_STR] = L"";
			swprintf_s(szIndex, L"%d", i);
			m_pDeviceMgr->GetFont()->DrawText(m_pDeviceMgr->GetSprite(),
				szIndex, lstrlen(szIndex), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		}*/
	}



	
	
	// ���� ����
	// ����
	vector<TILE_INFO*> vecRender = m_pMainDialog->m_MapTool.m_pMapData->vecObj;
	sort(vecRender.begin(), vecRender.end(), [](TILE_INFO* pV1, TILE_INFO* pV2) {
		return pV1->vPos.y < pV2->vPos.y;
	});

	// Ÿ�� ���� �� 50% ����ȭ
	DWORD dwRadio = (0 == m_pMainDialog->m_MapTool.m_iRadio) ? 0 : D3DCOLOR_ARGB(125, 0, 0, 0);

	for (size_t i = 0; i < vecRender.size(); ++i)
	{
		pTileInfo = vecRender[i];

		pTexInfo = m_pTextureMgr->GetTexInfo(pTileInfo->wstrObjKey, pTileInfo->wstrStateKey, 0);
		if (nullptr == pTexInfo)
			continue;

		D3DXMatrixScaling(&matScale,
			pTileInfo->vSize.x,
			pTileInfo->vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans,
			pTileInfo->vPos.x - m_pViewMgr->GetViewX(), //m_pView->GetScrollPos(0)
			pTileInfo->vPos.y - m_pViewMgr->GetViewY(), //m_pView->GetScrollPos(1)
			0.f);

		matWorld = matScale * matTrans;

		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);

		const TEX_INFO* pShadowInfo = m_pTextureMgr->GetTexInfo(L"Shadow");
		if (nullptr != pShadowInfo) {
			m_pDeviceMgr->GetSprite()->Draw(pShadowInfo->pTexture, nullptr,
				&D3DXVECTOR3{ pShadowInfo->tImgInfo.Width * 0.5f, pShadowInfo->tImgInfo.Height * 0.4f, 0.f }, nullptr, pShadowInfo->dwBlend);
		}

		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&pTexInfo->vOffset, nullptr, pTexInfo->dwBlend - dwRadio);
	}


	if (0 == m_pMainDialog->m_MapTool.m_iRadio)
	{
		vector<TILE_INFO*>* pVecTile = &m_pMainDialog->m_MapTool.m_pMapData->vecObj;

		// �簢�� ������
		for (size_t i = 0; i < pVecTile->size(); ++i)
		{
			if (i == m_pMainDialog->m_MapTool.m_ListBoxObject.GetCurSel()) {
				m_pDeviceMgr->GetSprite()->End();
				m_pDeviceMgr->GetDevice()->EndScene();

				pTileInfo = (*pVecTile)[i];
				pTexInfo = m_pTextureMgr->GetTexInfo(pTileInfo->wstrObjKey, pTileInfo->wstrStateKey, 0);
				if (nullptr == pTexInfo)
					continue;

				D3DXMatrixScaling(&matScale,
					pTileInfo->vSize.x,
					pTileInfo->vSize.y, 0.f);
				D3DXMatrixTranslation(&matTrans,
					pTileInfo->vPos.x - m_pViewMgr->GetViewX(), //m_pView->GetScrollPos(0)
					pTileInfo->vPos.y - m_pViewMgr->GetViewY(), //m_pView->GetScrollPos(1)
					0.f);

				D3DXVECTOR2 pLine[5] = {};
				D3DXVECTOR3	vVertex[4] = {
					{ -pTexInfo->vOffset.x, -pTexInfo->vOffset.y, 0.f },
					{ pTexInfo->tImgInfo.Width - pTexInfo->vOffset.x, -pTexInfo->vOffset.y, 0.f },
					{ pTexInfo->tImgInfo.Width - pTexInfo->vOffset.x, pTexInfo->tImgInfo.Height - pTexInfo->vOffset.y, 0.f },
					{ -pTexInfo->vOffset.x, pTexInfo->tImgInfo.Height - pTexInfo->vOffset.y, 0.f }
				};

				for (int j = 0; j < 4; ++j) {
					D3DXVECTOR3 vTmp;
					D3DXVec3TransformCoord(&vTmp, &vVertex[j], &(matScale * matTrans));
					pLine[j] = { vTmp.x, vTmp.y };
				}
				pLine[4] = pLine[0];
				m_pDeviceMgr->GetLine()->Draw(pLine, 5, D3DCOLOR_ARGB(255, 255, 0, 0));
				
				m_pDeviceMgr->GetDevice()->BeginScene();
				m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
			}
		}

		pTexInfo = m_pMainDialog->m_MapTool.m_pSelectObj;
		NULL_CHECK(pTexInfo);

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans,
			CMouseMgr::GetInstance()->GetMouse().x,
			CMouseMgr::GetInstance()->GetMouse().y,
			0.f);

		matWorld = matScale * matTrans;

		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&pTexInfo->vOffset, nullptr, pTexInfo->dwBlend);
	}

	
	if (m_pMainDialog->m_MapTool.m_ButtonIsDark.GetCheck())
	{
		// ��� ȿ��
		pTexInfo = m_pTextureMgr->GetTexInfo(L"Dark");
		NULL_CHECK(pTexInfo);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, VIEWCX * 0.5f - pTexInfo->tImgInfo.Width * 0.5f, VIEWCY * 0.5f - pTexInfo->tImgInfo.Height * 0.5f, 0.f);
		matWorld = matScale * matTrans;
		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&pTexInfo->vOffset, nullptr, pTexInfo->dwBlend);
	}
}

void CTerrain::WordToolRender()
{
	D3DXMATRIX matScale, matTrans, matWorld;
	
	// �� ��� 
	const TEX_INFO* pBackInfo = m_pTextureMgr->GetTexInfo(L"BackGround", L"Map", 4);

	if (nullptr != pBackInfo) {
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, float(-m_pViewMgr->GetViewX()), float(-m_pViewMgr->GetViewY()), 0.f);
		matWorld = matScale * matTrans;
		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pBackInfo->pTexture, nullptr,
			&pBackInfo->vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	// ���� â
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"UI", L"WordBox");
	if (nullptr != pTexInfo)
	{
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, float(VIEWCX * 0.5f -m_pViewMgr->GetViewX()), float(VIEWCY - m_pViewMgr->GetViewY()), 0.f);
		matWorld = matScale * matTrans;
		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&pTexInfo->vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

// �̺�Ʈ �� ����
void CTerrain::EventToolRender()
{
	D3DXMATRIX matScale, matTrans, matWorld;
	const TEX_INFO* pTexInfo = nullptr;


	// �� ��� 
	if (nullptr != m_pMainDialog->m_EventTool.m_pMapData)
	{
		int iMapNum = m_pMainDialog->m_EventTool.m_pMapData->iMapImageNum;
		const TEX_INFO* pBackInfo = m_pTextureMgr->GetTexInfo(L"BackGround", L"Map", iMapNum);

		if (nullptr != pBackInfo) {
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
			D3DXMatrixTranslation(&matTrans, float(-m_pViewMgr->GetViewX()), float(-m_pViewMgr->GetViewY()), 0.f);
			matWorld = matScale * matTrans;
			m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
			m_pDeviceMgr->GetSprite()->Draw(pBackInfo->pTexture, nullptr,
				&pBackInfo->vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}

	// ������Ʈ
	for (size_t i = 0; i < m_pMainDialog->m_EventTool.m_vecEventObj.size(); ++i)
	{
		CEventObj* pEventObj = m_pMainDialog->m_EventTool.m_vecEventObj[i];

		if (nullptr != pEventObj)
		{
			EVENT_NOTIFY* pNotify = pEventObj->GetCurState();
			if (nullptr == pNotify)
				continue;
			
			// Path Render
			if (m_pMainDialog->m_EventTool.m_CheckPathRender.GetCheck()/* && m_pMainDialog->m_EventTool.m_LisBoxObject.GetCurSel() == i*/)
			{
				for (size_t j = 0; j < (*pEventObj->GetVecNotify()).size(); ++j)
				{
					EVENT_NOTIFY* pPathNotify = (*pEventObj->GetVecNotify())[j];

					pTexInfo = m_pTextureMgr->GetTexInfo(pPathNotify->wstrObjKey, pPathNotify->wstrStateKey, 0);
					if (nullptr == pTexInfo)
						continue;

					D3DXMatrixScaling(&matScale, 1.f * pTexInfo->m_iMirror, 1.f, 0.f);
					D3DXMatrixTranslation(&matTrans, pPathNotify->vPos.x - m_pViewMgr->GetViewX(), pPathNotify->vPos.y - m_pViewMgr->GetViewY(), 0.f);
					matWorld = matScale * matTrans;
					m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);

					DWORD dwColor = D3DCOLOR_ARGB(127, D3DCOLOR_GET_R(pTexInfo->dwBlend), D3DCOLOR_GET_G(pTexInfo->dwBlend), D3DCOLOR_GET_B(pTexInfo->dwBlend));

					m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
						&pTexInfo->vOffset, nullptr, dwColor);

					// ���� �簢��
					if (m_pMainDialog->m_EventTool.m_ListBoxEventPath.GetCurSel() == j)
					{
						m_pDeviceMgr->GetSprite()->End();
						m_pDeviceMgr->GetDevice()->EndScene();

						
						D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
						D3DXMatrixTranslation(&matTrans, pPathNotify->vPos.x - m_pViewMgr->GetViewX(), pPathNotify->vPos.y - m_pViewMgr->GetViewY(), 0.f);

						D3DXVECTOR2 pLine[5] = {};
						D3DXVECTOR3	vVertex[4] = {
							{ -pTexInfo->vOffset.x, -pTexInfo->vOffset.y, 0.f },
							{ pTexInfo->tImgInfo.Width - pTexInfo->vOffset.x, -pTexInfo->vOffset.y, 0.f },
							{ pTexInfo->tImgInfo.Width - pTexInfo->vOffset.x, pTexInfo->tImgInfo.Height - pTexInfo->vOffset.y, 0.f },
							{ -pTexInfo->vOffset.x, pTexInfo->tImgInfo.Height - pTexInfo->vOffset.y, 0.f }
						};

						for (int j = 0; j < 4; ++j) {
							D3DXVECTOR3 vTmp;
							D3DXVec3TransformCoord(&vTmp, &vVertex[j], &(matScale * matTrans));
							pLine[j] = { vTmp.x, vTmp.y };
						}
						pLine[4] = pLine[0];
						m_pDeviceMgr->GetLine()->Draw(pLine, 5, D3DCOLOR_ARGB(255, 255, 0, 0));

						m_pDeviceMgr->GetDevice()->BeginScene();
						m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
					}
				}
			}

			// Obj Render
			if (pNotify->bIsActive)
			{
				pTexInfo = m_pTextureMgr->GetTexInfo(pNotify->wstrObjKey, pNotify->wstrStateKey, pEventObj->GetCurFrame());
				if (nullptr == pTexInfo)
					continue;

				D3DXMatrixScaling(&matScale, 1.f * pTexInfo->m_iMirror, 1.f, 0.f);
				D3DXMatrixTranslation(&matTrans, pNotify->vPos.x - m_pViewMgr->GetViewX(), pNotify->vPos.y - m_pViewMgr->GetViewY(), 0.f);
				matWorld = matScale * matTrans;
				m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
				m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
					&pTexInfo->vOffset, nullptr, pTexInfo->dwBlend);

				// �簢�� ������
				/*m_pDeviceMgr->GetSprite()->End();
				m_pDeviceMgr->GetDevice()->EndScene();

				D3DXVECTOR2 pLine[5] = {};
				D3DXVECTOR3	vVertex[4] = {
					{ -pTexInfo->vOffset.x, -pTexInfo->vOffset.y, 0.f },
					{ pTexInfo->tImgInfo.Width - pTexInfo->vOffset.x, -pTexInfo->vOffset.y, 0.f },
					{ pTexInfo->tImgInfo.Width - pTexInfo->vOffset.x, pTexInfo->tImgInfo.Height - pTexInfo->vOffset.y, 0.f },
					{ -pTexInfo->vOffset.x, pTexInfo->tImgInfo.Height - pTexInfo->vOffset.y, 0.f }
				};

				for (int j = 0; j < 4; ++j) {
					D3DXVECTOR3 vTmp;
					D3DXVec3TransformCoord(&vTmp, &vVertex[j], &(matScale * matTrans));
					pLine[j] = { vTmp.x, vTmp.y };
				}
				pLine[4] = pLine[0];
				m_pDeviceMgr->GetLine()->Draw(pLine, 5, D3DCOLOR_ARGB(255, 0, 255, 0));

				m_pDeviceMgr->GetDevice()->BeginScene();
				m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);*/
			}
		}
	}
}


CTerrain* CTerrain::Create(CToolView* pView)
{
	NULL_CHECK_RETURN(pView, nullptr);

	CTerrain* pInstance = new CTerrain;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	pInstance->SetView(pView);	

	return pInstance;
}
