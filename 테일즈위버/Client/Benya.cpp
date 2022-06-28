#include "stdafx.h"
#include "Benya.h"

#include "Monster.h"

#include "NormalEffect.h"
#include "AttackEffect.h"

#include "Npc.h"

#include "ItemEquipData.h"
#include "ItemUsableData.h"

#include "ShopWnd.h"

CBenya::CBenya()
{
}


CBenya::~CBenya()
{
}

void CBenya::SetIdleState()
{
	NULL_CHECK(m_pBody);
	m_pBody->SetFrameKey(L"NPC", L"Benya", m_pBody->STATE::IDLE, m_pBody->GetDirection(), FRAME_LOOP);
}

void CBenya::SetRunState()
{
	NULL_CHECK(m_pBody);
}

void CBenya::SetAttackState()
{
	NULL_CHECK(m_pBody);
}

void CBenya::SetHitState()
{
	// Talk �̺�Ʈ ó��
	if (!m_pSceneMgr->GetQuest(EVENT_QUEST0))
	{
		m_pSceneMgr->SetQuest(EVENT_QUEST0, true);
		m_pWordMgr->AddWord(new WORD_DATA(L"Linda", L"�� ���� �Ƴ��� ���þ�?", L"Character", L"Word_Benya"));
		m_pWordMgr->AddWord(new WORD_DATA(L"Kyoko", L"�� ���ڱ� �����߰� �þ����?", L"Character", L"Kyoko"));
		m_pWordMgr->AddWord(new WORD_DATA(L"Linda", L"���� �ʿ��� ���ڱ� �����°� ����!", L"Character", L"Word_Benya"));
		m_pWordMgr->AddWord(new WORD_DATA(L"Jelly", L"��!", L"Character", L"Jelly"));
		m_pWordMgr->AddWord(new WORD_DATA(L"Linda", L"�� ���� �� ã���� �� �־�?", L"Character", L"Word_Benya"));
		m_pWordMgr->AddWord(new WORD_DATA(L"Kyoko", L"�˰ڽ��ϴ�. �ٳ�ðԿ�", L"Character", L"Kyoko"));
		m_pWordMgr->StartWord(false);
	}
	else
	{
		m_pWordMgr->AddWord(new WORD_DATA(L"Linda", L"������..! ������!", L"Character", L"Word_Benya"));
		m_pWordMgr->AddWord(new WORD_DATA(L"Jelly", L"����~ ��!", L"Character", L"Jelly"));
		m_pWordMgr->StartWord(false);
	}
}

void CBenya::Attack(SKILLID eSkillID)
{
	NULL_CHECK(m_pBody);
}

void CBenya::FrameEvent()
{
	NULL_CHECK(m_pBody);
}
