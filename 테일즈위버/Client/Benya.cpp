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
	// Talk ÀÌº¥Æ® Ã³¸®
	if (!m_pSceneMgr->GetQuest(EVENT_QUEST0))
	{
		m_pSceneMgr->SetQuest(EVENT_QUEST0, true);
		m_pWordMgr->AddWord(new WORD_DATA(L"Linda", L"³» µ¿»ý ¾Æ³ª°¡ ¸øºÃ¾î?", L"Character", L"Word_Benya"));
		m_pWordMgr->AddWord(new WORD_DATA(L"Kyoko", L"¿Ö °©ÀÚ±â Á©¸®»ß°¡ ´Ã¾î³­°ÅÁÒ?", L"Character", L"Kyoko"));
		m_pWordMgr->AddWord(new WORD_DATA(L"Linda", L"´øÀü ÂÊ¿¡¼­ °©ÀÚ±â ¸ô·Á¿Â°Å ÀÖÁö!", L"Character", L"Word_Benya"));
		m_pWordMgr->AddWord(new WORD_DATA(L"Jelly", L"»ß!", L"Character", L"Jelly"));
		m_pWordMgr->AddWord(new WORD_DATA(L"Linda", L"³» µ¿»ý Á» Ã£¾ÆÁÙ ¼ö ÀÖ¾î?", L"Character", L"Word_Benya"));
		m_pWordMgr->AddWord(new WORD_DATA(L"Kyoko", L"¾Ë°Ú½À´Ï´Ù. ´Ù³à¿Ã°Ô¿ä", L"Character", L"Kyoko"));
		m_pWordMgr->StartWord(false);
	}
	else
	{
		m_pWordMgr->AddWord(new WORD_DATA(L"Linda", L"Á©¸®»ß..! ÄÚÇÇÆÎ!", L"Character", L"Word_Benya"));
		m_pWordMgr->AddWord(new WORD_DATA(L"Jelly", L"¸®»ß~ »ß!", L"Character", L"Jelly"));
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
