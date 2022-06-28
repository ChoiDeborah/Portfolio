#include "stdafx.h"
#include "ObjMgr.h"
#include "GameObject.h"
#pragma comment(lib, "msimg32.lib")


#include "ItemEquipData.h"
#include "ItemUsableData.h"

// �̱���
IMPLEMENT_SINGLETON(CObjMgr)

CObjMgr::CObjMgr()
	: m_pSceneMgr (CSceneMgr::GetInstance()),
	m_pViewMgr (CViewMgr::GetInstance())
{
}


CObjMgr::~CObjMgr()
{
	Release();
}

const UNIT_INFO* CObjMgr::GetUnitData(const wstring& wstrName)
{
	auto iter_find = m_mapUnitData.find(wstrName);
	if (iter_find != m_mapUnitData.end())
		return iter_find->second;

	return nullptr;
}

CItemData * CObjMgr::GetItemData(const wstring & wstrName)
{
	auto iter_find = m_mapItemData.find(wstrName);
	if (iter_find != m_mapItemData.end())
		return iter_find->second;

	return nullptr;
}

CGameObject* CObjMgr::GetTarget(CGameObject * pObj, OBJID eID)
{
	if (m_ObjList[eID].empty())
		return nullptr;

	CGameObject* pTarget = nullptr;
	float fDistance = 0.f;

	pTarget = m_ObjList[eID].front();

	float fW = pTarget->GetInfo().vPos.x - pObj->GetInfo().vPos.x;
	float fH = pTarget->GetInfo().vPos.y - pObj->GetInfo().vPos.y;
	fDistance = sqrtf(fW * fW + fH * fH);

	for (auto _pTarget : m_ObjList[eID])
	{
		float fW = _pTarget->GetInfo().vPos.x - pObj->GetInfo().vPos.x;
		float fH = _pTarget->GetInfo().vPos.y - pObj->GetInfo().vPos.y;
		float fCmpDist = sqrtf(fW * fW + fH * fH);

		if (fCmpDist < fDistance)
		{
			pTarget = _pTarget;
			fDistance = fCmpDist;
		}
	}

	return pTarget;
}

CGameObject* CObjMgr::GetPlayer() {
	return (!m_ObjList[OBJ_PLAYER].empty()) ? m_ObjList[OBJ_PLAYER].back() : nullptr;
}

bool CObjMgr::InstanceExists(OBJID eID) {
	return (m_ObjList[eID].empty());
}

HRESULT CObjMgr::LoadItemData(const wstring wstrFilePath)
{
	wifstream LoadFile;

	LoadFile.open(wstrFilePath.c_str(), ios::in);

	if (!LoadFile.good())
		return E_FAIL;

	for (auto &ItemData : m_mapItemData)
		SafeDelete(ItemData.second);
	m_mapItemData.clear();

	TCHAR szCount[MAX_STR] = L"";
	LoadFile.getline(szCount, MAX_STR);
	int iCount = int(_wtoi(szCount));

	if (0 < iCount)
	{
		TCHAR szObjKey[MAX_STR] = L"";		// �ؽ��� ������Ʈ Ű
		TCHAR szStateKey[MAX_STR] = L"";	// �ؽ��� ���� Ű

		TCHAR szItemType[MAX_STR] = L"";	// ������ Ÿ��
		TCHAR szItemID[MAX_STR] = L"";		// ������ ���̵�

		TCHAR szItemCollTime[MAX_STR] = L""; // ������ ��Ÿ��

		TCHAR szPrice[MAX_STR] = L"";		// ������ ����

		TCHAR szItemCount[MAX_STR] = L"";	// ����
		TCHAR szWeight[MAX_STR] = L"";		// ����

		TCHAR szColor[MAX_STR] = L"";		// ����


		TCHAR szHP[MAX_STR] = L"";	// ü��
		TCHAR szMP[MAX_STR] = L"";	// ����
		TCHAR szSP[MAX_STR] = L"";	// ���׹̳�

		TCHAR szDurability[MAX_STR] = L"";	// ������

		TCHAR szSTAB[MAX_STR] = L"";	// ���
		TCHAR szHACK[MAX_STR] = L"";	// ����
		TCHAR szINT[MAX_STR] = L"";	// ���� ����
		TCHAR szDEX[MAX_STR] = L"";	// ����
		TCHAR szDEF[MAX_STR] = L"";	// ���� ���
		TCHAR szMR[MAX_STR] = L"";	// ���� ���
		TCHAR szAGI[MAX_STR] = L"";	// ȸ����

		TCHAR szLightning[MAX_STR] = L"";	// ���� �Ӽ�
		TCHAR szFire[MAX_STR] = L"";		// �� �Ӽ�
		TCHAR szEarth[MAX_STR] = L"";		// �� �Ӽ�
		TCHAR szLight[MAX_STR] = L"";		// �� �Ӽ�

		TCHAR szIce[MAX_STR] = L"";			// ���� �Ӽ�
		TCHAR szWind[MAX_STR] = L"";		// �ٶ� �Ӽ�
		TCHAR szDark[MAX_STR] = L"";		// ���� �Ӽ�
		TCHAR szNormal[MAX_STR] = L"";		// �� �Ӽ�


		CItemData* pItemData;
		for (int i = 0; i < iCount; ++i) {
			LoadFile.getline(szObjKey, MAX_STR, '|');		// �ؽ��� ������Ʈ Ű
			LoadFile.getline(szStateKey, MAX_STR, '|');		// �ؽ��� ���� Ű

			LoadFile.getline(szItemCollTime, MAX_STR, '|');		// ��Ÿ��

			LoadFile.getline(szPrice, MAX_STR, '|');		// ����

			LoadFile.getline(szItemType, MAX_STR, '|');		// ������ Ÿ��
			LoadFile.getline(szItemID, MAX_STR, '|');		// ������ ���̵�
			
			LoadFile.getline(szItemCount, MAX_STR, '|');	// ����
			LoadFile.getline(szWeight, MAX_STR, '|');		// ����
			
			auto& iter_find = m_mapUnitData.find(szStateKey);
			if (iter_find != m_mapUnitData.end())
				continue;

			ITEM_TYPE eType = ITEM_TYPE(_wtoi(szItemType));

			switch (eType)
			{
			case ITEM_TYPE::ITEM_EQUIP:
				LoadFile.getline(szColor, MAX_STR, '|');				// ����

				LoadFile.getline(szDurability, MAX_STR, '|');		// ������

				LoadFile.getline(szHP, MAX_STR, '|');	// ü��
				LoadFile.getline(szMP, MAX_STR, '|');	// ����
				LoadFile.getline(szSP, MAX_STR, '|');	// ���׹̳�

				LoadFile.getline(szSTAB, MAX_STR, '|');	// ���
				LoadFile.getline(szHACK, MAX_STR, '|');	// ����
				LoadFile.getline(szINT, MAX_STR, '|');	// ���� ����
				LoadFile.getline(szDEX, MAX_STR, '|');	// ����
				LoadFile.getline(szDEF, MAX_STR, '|');	// ���� ���
				LoadFile.getline(szMR, MAX_STR, '|');	// ���� ���
				LoadFile.getline(szAGI, MAX_STR, '|');	// ȸ����

				LoadFile.getline(szLightning, MAX_STR, '|');	// ���� �Ӽ�
				LoadFile.getline(szFire, MAX_STR, '|');			// �� �Ӽ�
				LoadFile.getline(szEarth, MAX_STR, '|');		// �� �Ӽ�
				LoadFile.getline(szLight, MAX_STR, '|');		// �� �Ӽ�

				LoadFile.getline(szIce, MAX_STR, '|');			// ���� �Ӽ�
				LoadFile.getline(szWind, MAX_STR, '|');			// �ٶ� �Ӽ�
				LoadFile.getline(szDark, MAX_STR, '|');			// ���� �Ӽ�
				LoadFile.getline(szNormal, MAX_STR);			// �� �Ӽ�

				pItemData = new CItemEquipData(
					ICON_TYPE::ICON_ITEM,
					eType,
					ITEMID(_wtoi(szItemID)),
					szObjKey,
					szStateKey,
					float(_wtof(szItemCollTime)),
					int(_wtoi(szItemCount)),
					int(_wtoi(szWeight)),
					DWORD(_wtoi(szColor)));

				pItemData->iPrice = int(_wtoi(szPrice));
				dynamic_cast<CItemEquipData*>(pItemData)->iMaxDurability =
					dynamic_cast<CItemEquipData*>(pItemData)->iDurability = int(_wtoi(szDurability));

				dynamic_cast<CItemEquipData*>(pItemData)->iMaxHp = int(_wtoi(szHP));
				dynamic_cast<CItemEquipData*>(pItemData)->iMaxMp = int(_wtoi(szMP));
				dynamic_cast<CItemEquipData*>(pItemData)->iMaxSp = int(_wtoi(szSP));

				dynamic_cast<CItemEquipData*>(pItemData)->tStatInfo.iStab = int(_wtoi(szSTAB));
				dynamic_cast<CItemEquipData*>(pItemData)->tStatInfo.iHack = int(_wtoi(szHACK));
				dynamic_cast<CItemEquipData*>(pItemData)->tStatInfo.iInt = int(_wtoi(szINT));
				dynamic_cast<CItemEquipData*>(pItemData)->tStatInfo.iDex = int(_wtoi(szDEX));
				dynamic_cast<CItemEquipData*>(pItemData)->tStatInfo.iDef = int(_wtoi(szDEF));
				dynamic_cast<CItemEquipData*>(pItemData)->tStatInfo.iMr = int(_wtoi(szMR));
				dynamic_cast<CItemEquipData*>(pItemData)->tStatInfo.iAgi = int(_wtoi(szAGI));

				dynamic_cast<CItemEquipData*>(pItemData)->tStatInfo.iLightning = int(_wtoi(szLightning));
				dynamic_cast<CItemEquipData*>(pItemData)->tStatInfo.iFire = int(_wtoi(szFire));
				dynamic_cast<CItemEquipData*>(pItemData)->tStatInfo.iEarth = int(_wtoi(szEarth));
				dynamic_cast<CItemEquipData*>(pItemData)->tStatInfo.iLight = int(_wtoi(szLight));

				dynamic_cast<CItemEquipData*>(pItemData)->tStatInfo.iIce = int(_wtoi(szIce));
				dynamic_cast<CItemEquipData*>(pItemData)->tStatInfo.iWind = int(_wtoi(szWind));
				dynamic_cast<CItemEquipData*>(pItemData)->tStatInfo.iDark = int(_wtoi(szDark));
				dynamic_cast<CItemEquipData*>(pItemData)->tStatInfo.iNormal = int(_wtoi(szNormal));

				break;

			case ITEM_TYPE::ITEM_USE:
				LoadFile.getline(szColor, MAX_STR, '|');	// ����
				LoadFile.getline(szHP, MAX_STR, '|');		// ü��
				LoadFile.getline(szMP, MAX_STR, '|');		// ����
				LoadFile.getline(szSP, MAX_STR);			// ���׹̳�

				pItemData = new CItemUsableData(
					ICON_TYPE::ICON_ITEM,
					eType,
					ITEMID(_wtoi(szItemID)),
					szObjKey,
					szStateKey,
					float(_wtof(szItemCollTime)),
					int(_wtoi(szItemCount)),
					int(_wtoi(szWeight)),
					DWORD(_wtoi(szColor)));

				pItemData->iPrice = int(_wtoi(szPrice));
				dynamic_cast<CItemUsableData*>(pItemData)->iMaxHp = int(_wtoi(szHP));
				dynamic_cast<CItemUsableData*>(pItemData)->iMaxMp = int(_wtoi(szMP));
				dynamic_cast<CItemUsableData*>(pItemData)->iMaxSp = int(_wtoi(szSP));
				break;

			case ITEM_TYPE::ITEM_ETC:
				LoadFile.getline(szColor, MAX_STR);				// ����

				pItemData = new CItemData(
					ICON_TYPE::ICON_ITEM,
					eType,
					ITEMID(_wtoi(szItemID)),
					szObjKey,
					szStateKey,
					float(_wtof(szItemCollTime)),
					int(_wtoi(szItemCount)),
					int(_wtoi(szWeight)),
					DWORD(_wtoi(szColor)));

					pItemData->iPrice = int(_wtoi(szPrice));
				break;
			}

			m_mapItemData.insert({ szStateKey, pItemData });
		}
	}

	LoadFile.close();

	return S_OK;
}

HRESULT CObjMgr::LoadUnitData(const wstring wstrFilePath)
{
	wifstream LoadFile;

	LoadFile.open(wstrFilePath.c_str(), ios::in);

	if (!LoadFile.good())
		return E_FAIL;

	for (auto &ObjData : m_mapUnitData)
		SafeDelete(ObjData.second);
	m_mapUnitData.clear();
	
	TCHAR szCount[MAX_STR] = L"";
	LoadFile.getline(szCount, MAX_STR);
	int iCount = int(_wtoi(szCount));

	if (0 < iCount)
	{
		TCHAR szName[MAX_STR]		= L"";	// �̸�
		TCHAR szObjKey[MAX_STR]		= L"";	// �ؽ��� ������Ʈ Ű
		TCHAR szStateKey[MAX_STR]	= L"";	// �ؽ��� ���� Ű

		TCHAR szObjID[MAX_STR]		= L"";	// ������Ʈ ���̵�

		TCHAR szLevel[MAX_STR]		= L"";	// ����
		TCHAR szExp[MAX_STR]		= L"";	// ����ġ

		TCHAR szHP[MAX_STR]			= L"";	// ü��
		TCHAR szMP[MAX_STR]			= L"";	// ����
		TCHAR szSP[MAX_STR]			= L"";	// ���׹̳�

		TCHAR szSTAB[MAX_STR]		= L"";	// ���
		TCHAR szHACK[MAX_STR]		= L"";	// ����
		TCHAR szINT[MAX_STR]		= L"";	// ���� ����
		TCHAR szDEX[MAX_STR]		= L"";	// ����
		TCHAR szDEF[MAX_STR]		= L"";	// ���� ���
		TCHAR szMR[MAX_STR]			= L"";	// ���� ���
		TCHAR szAGI[MAX_STR]		= L"";	// ȸ����

		TCHAR szLightning[MAX_STR]	= L"";	// ���� �Ӽ�
		TCHAR szFire[MAX_STR]		= L"";	// �� �Ӽ�
		TCHAR szEarth[MAX_STR]		= L"";	// �� �Ӽ�
		TCHAR szLight[MAX_STR]		= L"";	// �� �Ӽ�

		TCHAR szIce[MAX_STR]		= L"";	// ���� �Ӽ�
		TCHAR szWind[MAX_STR]		= L"";	// �ٶ� �Ӽ�
		TCHAR szDark[MAX_STR]		= L"";	// ���� �Ӽ�
		TCHAR szNormal[MAX_STR]		= L"";	// �� �Ӽ�

		TCHAR szIsFirst[MAX_STR]	= L"";	// ���� / �İ�

		TCHAR szFindDst[MAX_STR]	= L"";	// �÷��̾� �߰� �Ÿ�

		TCHAR szCollLeft[MAX_STR]	= L"";	// �浹��Ʈ Left
		TCHAR szCollTop[MAX_STR]	= L"";	// �浹��Ʈ Top
		TCHAR szCollRight[MAX_STR]	= L"";	// �浹��Ʈ Right
		TCHAR szCollBottom[MAX_STR] = L"";	// �浹��Ʈ Bottom

		UNIT_INFO* pUnitInfo;
		for (int i = 0; i < iCount; ++i) {
			LoadFile.getline(szName,		MAX_STR, '|');	// �̸�
			LoadFile.getline(szObjKey,		MAX_STR, '|');	// �ؽ��� ������Ʈ Ű
			LoadFile.getline(szStateKey,	MAX_STR, '|');	// �ؽ��� ���� Ű

			LoadFile.getline(szObjID,		MAX_STR, '|');	// ������Ʈ ���̵�

			LoadFile.getline(szLevel,		MAX_STR, '|');	// ����
			LoadFile.getline(szExp,			MAX_STR, '|');	// ����ġ

			LoadFile.getline(szHP,			MAX_STR, '|');	// ü��
			LoadFile.getline(szMP,			MAX_STR, '|');	// ����
			LoadFile.getline(szSP,			MAX_STR, '|');	// ���׹̳�

			LoadFile.getline(szSTAB,		MAX_STR, '|');	// ���
			LoadFile.getline(szHACK,		MAX_STR, '|');	// ����
			LoadFile.getline(szINT,			MAX_STR, '|');	// ���� ����
			LoadFile.getline(szDEX,			MAX_STR, '|');	// ����
			LoadFile.getline(szDEF,			MAX_STR, '|');	// ���� ���
			LoadFile.getline(szMR,			MAX_STR, '|');	// ���� ���
			LoadFile.getline(szAGI,			MAX_STR, '|');	// ȸ����

			LoadFile.getline(szLightning,	MAX_STR, '|');	// ���
			LoadFile.getline(szFire,		MAX_STR, '|');	// ����
			LoadFile.getline(szEarth,		MAX_STR, '|');	// ���� ����
			LoadFile.getline(szLight,		MAX_STR, '|');	// ����

			LoadFile.getline(szIce,			MAX_STR, '|');	// ���� ���
			LoadFile.getline(szWind,		MAX_STR, '|');	// ���� ���
			LoadFile.getline(szDark,		MAX_STR, '|');	// ȸ����
			LoadFile.getline(szNormal,		MAX_STR, '|');	// ȸ����

			LoadFile.getline(szIsFirst,		MAX_STR, '|');	// ���� / �İ�

			LoadFile.getline(szFindDst,		MAX_STR, '|');	// �� �߰� �Ÿ�

			LoadFile.getline(szCollLeft,	MAX_STR, '|');	// �浹��Ʈ Left
			LoadFile.getline(szCollTop,		MAX_STR, '|');	// �浹��Ʈ Top
			LoadFile.getline(szCollRight,	MAX_STR, '|');	// �浹��Ʈ Right
			LoadFile.getline(szCollBottom,	MAX_STR);		// �浹��Ʈ Bottom

			auto& iter_find = m_mapUnitData.find(szName);
			if (iter_find != m_mapUnitData.end())
				continue;

			pUnitInfo = new UNIT_INFO;

			pUnitInfo->wstrName		= szName;
			pUnitInfo->wstrObjKey	= szObjKey;
			pUnitInfo->wstrStateKey = szStateKey;

			pUnitInfo->iObjID		= int(_wtoi(szObjID));

			pUnitInfo->iLevel		= int(_wtoi(szLevel));
			pUnitInfo->iExp			= int(_wtoi(szExp));

			pUnitInfo->iMaxHp		= pUnitInfo->iHp = int(_wtoi(szHP));
			pUnitInfo->iMaxMp		= pUnitInfo->iMp = int(_wtoi(szMP));
			pUnitInfo->iMaxSp		= pUnitInfo->iSp = int(_wtoi(szSP));

			pUnitInfo->tStatInfo.iStab		= int(_wtoi(szSTAB));
			pUnitInfo->tStatInfo.iHack		= int(_wtoi(szHACK));
			pUnitInfo->tStatInfo.iInt		= int(_wtoi(szINT));
			pUnitInfo->tStatInfo.iDex		= int(_wtoi(szDEX));
			pUnitInfo->tStatInfo.iDef		= int(_wtoi(szDEF));
			pUnitInfo->tStatInfo.iMr		= int(_wtoi(szMR));
			pUnitInfo->tStatInfo.iAgi		= int(_wtoi(szAGI));

			pUnitInfo->tStatInfo.iLightning	= int(_wtoi(szLightning));
			pUnitInfo->tStatInfo.iFire		= int(_wtoi(szFire));
			pUnitInfo->tStatInfo.iEarth		= int(_wtoi(szEarth));
			pUnitInfo->tStatInfo.iLight		= int(_wtoi(szLight));

			pUnitInfo->tStatInfo.iIce		= int(_wtoi(szIce));
			pUnitInfo->tStatInfo.iWind		= int(_wtoi(szWind));
			pUnitInfo->tStatInfo.iDark		= int(_wtoi(szDark));
			pUnitInfo->tStatInfo.iNormal	= int(_wtoi(szNormal));

			pUnitInfo->iIsFirstAtt	= int(_wtoi(szIsFirst));
			pUnitInfo->fFindDst		= float(_wtof(szFindDst));
			
			pUnitInfo->vCollStart.x = float(_wtof(szCollLeft));
			pUnitInfo->vCollStart.y = float(_wtof(szCollTop));
			pUnitInfo->vCollEnd.x	= float(_wtof(szCollRight));
			pUnitInfo->vCollEnd.y	= float(_wtof(szCollBottom));

			m_mapUnitData.insert({ szName, pUnitInfo });
		}
	}

	LoadFile.close();

	return S_OK;
}

void CObjMgr::AddObject(CGameObject * pObj, OBJID eID)
{
	pObj->SetObjID(eID);
	m_ObjList[eID].push_back(pObj);
}

void CObjMgr::Initialize()
{
	LoadItemData(L"../Data/ItemData/ItemData.txt");
	LoadUnitData(L"../Data/ObjData/ObjData.txt");
}

void CObjMgr::Update()
{
	if (m_pSceneMgr->GetEvent(EVENT_ALL_STOP))
		return;

	for (int i = 0; i < OBJ_END; ++i)
	{
		auto iter_begin = m_ObjList[i].begin();
		auto iter_end = m_ObjList[i].end();
		for (; iter_begin != iter_end; )
		{
			if (nullptr == (*iter_begin))
			{
				iter_begin = m_ObjList[i].erase(iter_begin);
				continue;
			}

			if (OBJ_ATTACK == i || OBJ_PLAYER_ATTACK == i ||  OBJ_PLAYER == i || !m_pSceneMgr->GetEvent(EVENT_XPLAYER_ALL_STOP)) {
				m_iEvent = (*iter_begin)->Update();

				if (SCENE_CHANGE == m_iEvent)
					return;
				else if (OBJ_DEAD == m_iEvent)
				{
					SafeDelete(*iter_begin);
					iter_begin = m_ObjList[i].erase(iter_begin);
				} 
				else
					++iter_begin;
			}
			else
				++iter_begin;
		}
	}
}

void CObjMgr::LateUpdate()
{
	for (int i = 0; i < OBJ_END; ++i)
	{
		auto iter_begin = m_ObjList[i].begin();
		auto iter_end = m_ObjList[i].end();
		for (; iter_begin != iter_end; )
		{
			if (OBJ_ATTACK == i || OBJ_PLAYER_ATTACK == i || OBJ_PLAYER == i || !m_pSceneMgr->GetEvent(EVENT_XPLAYER_ALL_STOP)) {
				m_iEvent = (*iter_begin)->LateUpdate();

				if (SCENE_CHANGE == m_iEvent)
					return;
				else if (OBJ_DEAD == m_iEvent)
				{
					SafeDelete(*iter_begin);
					iter_begin = m_ObjList[i].erase(iter_begin);
				}
				else
					++iter_begin;
			}
			else
				++iter_begin;
		}
	}
}

void CObjMgr::Render()
{
	float fCuringX = m_pViewMgr->GetViewX() - WINCX * 0.55f;
	float fCuringY = m_pViewMgr->GetViewY() - WINCY * 0.55f;
	float fCuringW = m_pViewMgr->GetViewX() + WINCX * 0.55f;
	float fCuringH = m_pViewMgr->GetViewY() + WINCY * 0.55f;

	for (int i = 0; i < LAYER_END; ++i)
		m_vecRender[i].clear();


	if (SCENE_CHANGE != m_iEvent) {
		// �з� �۾�
		CGameObject* pPlayer = GetPlayer();
		
		for (int i = 0; i < OBJ_END; ++i) {
			for (auto pObj : m_ObjList[i]) {
				// ���� �ø�
				if (pObj->GetInfo().vPos.x < fCuringX || pObj->GetInfo().vPos.x > fCuringW || 
					pObj->GetInfo().vPos.y < fCuringY || pObj->GetInfo().vPos.y > fCuringH)
					continue;

				RENDER_LAYER eLayer = pObj->GetLayer();
				m_vecRender[eLayer].push_back(pObj);
			}
		}

		// ���� (Depth�� ��)
		sort(m_vecRender[LAYER_OBJ].begin(), m_vecRender[LAYER_OBJ].end(), 
			[](auto& obj1, auto& obj2)->bool 
		{ 
			return (obj1->GetInfo().vPos.y < obj2->GetInfo().vPos.y); 
		});//return (obj1->GetDepth() < obj2->GetDepth()); });

		// ����
		for (int i = 0; i < LAYER_END; ++i) {
			for (auto& pObj : m_vecRender[i]) {
				pObj->Render();
				if (CSceneMgr::GetInstance()->GetEvent(EVENT_RECT_RENDER))
					pObj->DrawCollRect();
			}
		}
	}
}

void CObjMgr::MiniRender(D3DXVECTOR3 vPivot, float fPer)
{
	for (auto& pObj : m_vecRender[LAYER_OBJ]) {
		pObj->MiniRender(vPivot, fPer);
	}
}

void CObjMgr::Release()
{
	for (int i = 0; i < OBJ_END; ++i)
	{
		for_each(m_ObjList[i].begin(), m_ObjList[i].end(), SafeDelete<CGameObject*>);
		m_ObjList[i].clear();
	}

	for (int i = 0; i < LAYER_END; ++i)
		m_vecRender[i].clear();

	for (auto& ObjData : m_mapUnitData)
		SafeDelete(ObjData.second);
	m_mapUnitData.clear();

	for (auto& ItemData : m_mapItemData)
		SafeDelete(ItemData.second);
	m_mapItemData.clear();
}

void CObjMgr::ResetObjList()
{
	for (int i = 0; i < OBJ_END; ++i)
	{
		for_each(m_ObjList[i].begin(), m_ObjList[i].end(), SafeDelete<CGameObject*>);
		m_ObjList[i].clear();
	}

	for (int i = 0; i < LAYER_END; ++i)
		m_vecRender[i].clear();
}

void CObjMgr::ReleaseObj(OBJID eID) {
	for_each(m_ObjList[eID].begin(), m_ObjList[eID].end(), SafeDelete<CGameObject*>);
	m_ObjList[eID].clear();
}