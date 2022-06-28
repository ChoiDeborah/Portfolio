#include "stdafx.h"
#include "ItemUsableData.h"

#include "Player.h"

CItemUsableData::CItemUsableData()
{
}


CItemUsableData::~CItemUsableData()
{
}

void CItemUsableData::UseEvent()
{
	if (0 < iCount - 1)
		--iCount;
	else
		iCount = 0;

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pObjMgr->GetPlayer());

	if (!lstrcmp(wstrStateKey.c_str(), L"RedHub") ||
		!lstrcmp(wstrStateKey.c_str(), L"SmallRedPotion") ||
		!lstrcmp(wstrStateKey.c_str(), L"RedPotion"))
	{
		m_pUIMgr->AddHealth(iMaxHp, 0, 0);
		m_pUIMgr->InsertChat(L"", wstrStateKey + wstrStateKey + L"을(를) 사용하였습니다. (HP+" + to_wstring(iMaxHp) + L")");
		pPlayer->CreateEffect(pPlayer->GetInfo().vPos, D3DXVECTOR3{ 0.f, pPlayer->GetOBB().vCollSize.y * 0.5f, 0.f }, L"Effect", L"HpPotion", false, OBJ_EFFECT, pPlayer, false, FRAME_ONCE, true);
	}
	else if (!lstrcmp(wstrStateKey.c_str(), L"BlueHub") ||
		!lstrcmp(wstrStateKey.c_str(), L"BluePotion"))
	{
		m_pUIMgr->AddHealth(0, iMaxMp, 0);
		m_pUIMgr->InsertChat(L"", wstrStateKey + L"을(를) 사용하였습니다. (MP+" + to_wstring(iMaxMp) + L")");
		pPlayer->CreateEffect(pPlayer->GetInfo().vPos, D3DXVECTOR3{ 0.f, pPlayer->GetOBB().vCollSize.y * 0.5f, 0.f }, L"Effect", L"MpPotion", false, OBJ_EFFECT, pPlayer, false, FRAME_ONCE, true);
	}
	else if (!lstrcmp(wstrStateKey.c_str(), L"GreenHub"))
	{
		m_pUIMgr->AddHealth(0, 0, iMaxSp);
		m_pUIMgr->InsertChat(L"", wstrStateKey + L"을(를) 사용하였습니다. (SP+" + to_wstring(iMaxSp) + L")");
		pPlayer->CreateEffect(pPlayer->GetInfo().vPos, D3DXVECTOR3{ 0.f, pPlayer->GetOBB().vCollSize.y * 0.5f, 0.f }, L"Effect", L"MpPotion", false, OBJ_EFFECT, pPlayer, false, FRAME_ONCE, true);
	}
	else if (!lstrcmp(wstrStateKey.c_str(), L"ReturnFeather"))
	{
		m_pUIMgr->InsertChat(L"", wstrStateKey + L"을(를) 사용하였습니다.");
		CSceneMgr::GetInstance()->SetNextScene(SCENEID::SCENEID_STAGE, L"Map1");
	}
}
