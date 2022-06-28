#include "stdafx.h"
#include "Linda.h"

#include "Monster.h"

#include "NormalEffect.h"
#include "AttackEffect.h"

#include "Npc.h"

#include "ShopWnd.h"

CLinda::CLinda()
{
}


CLinda::~CLinda()
{
}

void CLinda::SetIdleState()
{
	NULL_CHECK(m_pBody);
	m_pBody->SetFrameKey(L"NPC", L"Linda", m_pBody->STATE::IDLE, m_pBody->GetDirection(), FRAME_LOOP);
}

void CLinda::SetRunState()
{
	NULL_CHECK(m_pBody);
}

void CLinda::SetAttackState()
{
	NULL_CHECK(m_pBody);
}

void CLinda::SetHitState()
{
	dynamic_cast<CShopWnd*>(CUIMgr::GetInstance()->GetWindow(WND_ID::WNDID_SHOP))->DeleteAllItem();

	CUIMgr::GetInstance()->AddItem(WND_ID::WNDID_SHOP, L"Seed");
	CUIMgr::GetInstance()->AddItem(WND_ID::WNDID_SHOP, L"RedPotion");
	CUIMgr::GetInstance()->AddItem(WND_ID::WNDID_SHOP, L"HeavyArmor");
	CUIMgr::GetInstance()->AddItem(WND_ID::WNDID_SHOP, L"BlueHub");
	CUIMgr::GetInstance()->AddItem(WND_ID::WNDID_SHOP, L"BlueAx");
	CUIMgr::GetInstance()->ShowShop();
}

void CLinda::Attack(SKILLID eSkillID)
{
	NULL_CHECK(m_pBody);
}

void CLinda::FrameEvent()
{
	NULL_CHECK(m_pBody);
}
