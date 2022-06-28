#include "stdafx.h"
#include "Scene.h"

// Object
#include "Creator.h"

#include "Player.h"
#include "Monster.h"
#include "Npc.h"
#include "Buddy.h"
#include "Item.h"

// Puzzle
#include "PuzzleObject.h"
#include "PuzzleTile.h"

// Controller
#include "Kyoko.h"

#include "Anais.h"
#include "Jelly.h"

// Monster
#include "DarkCow.h"
#include "Fixy.h"
#include "Cat.h"

// Npc
#include "Linda.h"
#include "Benya.h"

CScene::CScene()
{
}


CScene::~CScene()
{
	Release();
}

void CScene::Release() {
	for (auto& Tile : m_vecObj)
		SafeDelete(Tile);
	m_vecObj.clear();
	m_vecObj.shrink_to_fit();
}

HRESULT CScene::LoadMap(const wstring & wstrFileName)
{
	wstring wstrFilePath = L"../Data/MapData/";
	wstrFilePath += wstrFileName;
	wstrFilePath += L".txt";

	wifstream LoadFile;
	LoadFile.open(wstrFilePath, ios::in);

	if (!LoadFile.good()) {
		FAILED_CHECK_MSG_RETURN(E_FAIL, L"Load Failed", E_FAIL);
	}
	
	// 맵 이미지 번호
	TCHAR szMapImageNum[MAX_STR] = L"";
	LoadFile.getline(szMapImageNum, MAX_STR);

	// 타일X, 타일Y
	TCHAR szTileX[MAX_STR] = L"";
	TCHAR szTileY[MAX_STR] = L"";
	LoadFile.getline(szTileX, MAX_STR, '|');
	LoadFile.getline(szTileY, MAX_STR);
	int iTileX = int(_wtoi(szTileX));
	int iTileY = int(_wtoi(szTileY));
	m_pTileMgr->Initialize(iTileX, iTileY, int(_wtoi(szMapImageNum)));
	m_pViewMgr->SetRoomSize({ float(TILECX * (iTileX - 1)), float(TILECY * 0.5f * (iTileY - 1)), 0.f });

	// 각 타일 DrawID, Option
	TCHAR szDrawID[MAX_STR] = L"";
	TCHAR szOption[MAX_STR] = L"";
	TILE_INFO* pTile = nullptr;
	for (int i = 0; i < iTileY; ++i)
	{
		for (int j = 0; j < iTileX; ++j)
		{
			float fX = j * TILECX + (i % 2) * (TILECX * 0.5f);
			float fY = i * (TILECY * 0.5f);

			pTile = new TILE_INFO;
			pTile->vPos = { fX, fY, 0.f };
			pTile->vSize = { 1.f, 1.f, 0.f };

			LoadFile.getline(szDrawID, MAX_STR, '|');
			LoadFile.getline(szOption, MAX_STR);

			pTile->byDrawID = int(_wtoi(szDrawID));
			pTile->byOption = int(_wtoi(szOption));

			m_pTileMgr->InsertTile(pTile);
		}
	}

	// 오브젝트 개수
	TCHAR szObjNum[MAX_STR] = L"";
	LoadFile.getline(szObjNum, MAX_STR);
	int iObjNum = int(_wtoi(szObjNum));

	if (0 < iObjNum) {
		// 각 오브젝트 wstrName, wstrObjKey, wstrStateKey, vPos.x, vPos.y, vPos.z, vScale.x, vScale.y, vScale.z
		TCHAR szObjID[MAX_STR] = L"";
		TCHAR szName[MAX_STR] = L"";
		TCHAR szObjKey[MAX_STR] = L"";
		TCHAR szStateKey[MAX_STR] = L"";
		TCHAR szPosX[MAX_STR] = L"";
		TCHAR szPosY[MAX_STR] = L"";
		TCHAR szPosZ[MAX_STR] = L"";
		TCHAR szScaleX[MAX_STR] = L"";
		TCHAR szScaleY[MAX_STR] = L"";
		TCHAR szScaleZ[MAX_STR] = L"";

		TILE_INFO	tTileInfo;

		for (int i = 0; i < iObjNum; ++i)
		{
			INFO tInfo;

			LoadFile.getline(szObjID,		MAX_STR, '|');
			LoadFile.getline(szName,		MAX_STR, '|');
			LoadFile.getline(szObjKey,		MAX_STR, '|');
			LoadFile.getline(szStateKey,	MAX_STR, '|');
			LoadFile.getline(szPosX,		MAX_STR, '|');
			LoadFile.getline(szPosY,		MAX_STR, '|');
			LoadFile.getline(szPosZ,		MAX_STR, '|');
			LoadFile.getline(szScaleX,		MAX_STR, '|');
			LoadFile.getline(szScaleY,		MAX_STR, '|');
			LoadFile.getline(szScaleZ,		MAX_STR);
			
			tTileInfo.byOption		= int(_wtoi(szObjID));
			tTileInfo.wstrName		= szName;
			tTileInfo.wstrObjKey	= szObjKey;
			tTileInfo.wstrStateKey	= szStateKey;
			tTileInfo.vPos			= D3DXVECTOR3(float(_wtof(szPosX)), float(_wtof(szPosY)), float(_wtof(szPosZ)));
			tTileInfo.vSize			= D3DXVECTOR3(float(_wtof(szScaleX)), float(_wtof(szScaleY)), float(_wtof(szScaleZ)));

			tInfo.vPos				= D3DXVECTOR3(float(_wtof(szPosX)), float(_wtof(szPosY)), float(_wtof(szPosZ)));
			tInfo.vSize				= D3DXVECTOR3(float(_wtof(szScaleX)), float(_wtof(szScaleY)), float(_wtof(szScaleZ)));

			CreateObj(tTileInfo, tInfo);
		}
	}

	LoadFile.close();
	return S_OK;
}


void CScene::CreateObj(const TILE_INFO& tTileInfo, INFO& tInfo)
{
	CGameObject* pObject = nullptr;

	switch (tTileInfo.byOption)
	{

	case OBJ_ENVIRONMENT:
	{
		pObject = CAbstractFactory<CStaticObject>::CreateObj(tInfo);
		pObject->SetFrameKey(tTileInfo.wstrObjKey, tTileInfo.wstrStateKey, CGameObject::IDLE, NO_DIRECTION, FRAME_ONCE);
	}
	break;

	case OBJ_COLLISION:
	{
		pObject = CAbstractFactory<CStaticObject>::CreateObj(tTileInfo, tInfo);
	}
	break;

	case OBJ_EVENT:
	{
		if (!lstrcmp(tTileInfo.wstrObjKey.c_str(), L"Puzzle"))
		{
			pObject = CAbstractFactory<CPuzzleObject>::CreateObj(tTileInfo, tInfo);
			pObject->SetFrameKey(tTileInfo.wstrObjKey, tTileInfo.wstrStateKey, CGameObject::IDLE, NO_DIRECTION, FRAME_ONCE);
		}
		else if (!lstrcmp(tTileInfo.wstrObjKey.c_str(), L"PuzzleTile"))
		{
			pObject = CAbstractFactory<CPuzzleTile>::CreateObj(tTileInfo, tInfo);
			pObject->SetFrameKey(tTileInfo.wstrObjKey, tTileInfo.wstrStateKey, CGameObject::IDLE, NO_DIRECTION, FRAME_ONCE);
		}
	}
	break;

	case OBJ_PLAYER:
	{
		if (SCENEID::SCENEID_TITLE != m_pSceneMgr->GetPreScene())
		{
			auto pTileVec = m_pTileMgr->GetTileVec();

			// 다음 씬 일 경우
			if (m_pSceneMgr->GetStageNum() > m_pSceneMgr->GetPreStageNum())
			{
				for (auto& Tile : *pTileVec)
				{
					if (2 == Tile->byOption)
					{
						tInfo.vPos = Tile->vPos;
						break;
					}
				}
			}
			// 이전 씬일 경우
			else
			{
				for (auto& Tile : *pTileVec)
				{
					if (3 == Tile->byOption)
					{
						tInfo.vPos = Tile->vPos;
						break;
					}
				}
			}
		}

		pObject = CAbstractFactory<CPlayer>::CreateObj(tTileInfo, tInfo);
		m_pViewMgr->SetViewObj(pObject->GetInfo().vPos);

		
	}
	break;

	case OBJ_MONSTER:
	{
		pObject = CAbstractFactory<CMonster>::CreateObj(tTileInfo, tInfo);
		if (!lstrcmp(tTileInfo.wstrObjKey.c_str(), L"DarkCow"))
			dynamic_cast<CEventObject*>(pObject)->SetCharacter(new CDarkCow);
		else if (!lstrcmp(tTileInfo.wstrObjKey.c_str(), L"Fixy"))
			dynamic_cast<CEventObject*>(pObject)->SetCharacter(new CFixy);
		else if (!lstrcmp(tTileInfo.wstrObjKey.c_str(), L"Jelly"))
			dynamic_cast<CEventObject*>(pObject)->SetCharacter(new CJelly);
		else if (!lstrcmp(tTileInfo.wstrObjKey.c_str(), L"CatBoss"))
			dynamic_cast<CEventObject*>(pObject)->SetCharacter(new CCat);
	}
	break;

	case OBJ_NPC:
	{
		pObject = CAbstractFactory<CNpc>::CreateObj(tTileInfo, tInfo);

		if (!lstrcmp(tTileInfo.wstrObjKey.c_str(), L"Linda"))
			dynamic_cast<CEventObject*>(pObject)->SetCharacter(new CLinda);
		else if (!lstrcmp(tTileInfo.wstrObjKey.c_str(), L"Benya"))
			dynamic_cast<CEventObject*>(pObject)->SetCharacter(new CBenya);
		else if (!lstrcmp(tTileInfo.wstrObjKey.c_str(), L"Jelly"))
			dynamic_cast<CEventObject*>(pObject)->SetCharacter(new CJelly);
		else if (!lstrcmp(tTileInfo.wstrObjKey.c_str(), L"Anais"))
			dynamic_cast<CEventObject*>(pObject)->SetCharacter(new CAnais);
	}
	break;

	case OBJ_BUDDY:
	{
		pObject = CAbstractFactory<CBuddy>::CreateObj(tTileInfo, tInfo);

		if (!lstrcmp(tTileInfo.wstrObjKey.c_str(), L"Anais"))
			dynamic_cast<CEventObject*>(pObject)->SetCharacter(new CAnais);
		else if (!lstrcmp(tTileInfo.wstrObjKey.c_str(), L"Jelly"))
			dynamic_cast<CEventObject*>(pObject)->SetCharacter(new CJelly);
	}
	break;

	case OBJ_CREATOR:
	{
		pObject = CAbstractFactory<CCreator>::CreateObj(tTileInfo, tInfo);
		dynamic_cast<CCreator*>(pObject)->SetCreateData(tTileInfo, tInfo);
	}
	break;

	case OBJ_ITEM:
	{
		pObject = CAbstractFactory<CItem>::CreateObj(tTileInfo, tInfo);
		pObject->SetFrameKey(tTileInfo.wstrObjKey, tTileInfo.wstrStateKey, CGameObject::STATE::END, DIRECTION::NO_DIRECTION, FRAME_LOOP);
	}
	break;

	}

	if (nullptr != pObject)
		m_pObjMgr->AddObject(pObject, OBJID(tTileInfo.byOption));
}
