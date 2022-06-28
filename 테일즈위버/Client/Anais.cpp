#include "stdafx.h"
#include "Anais.h"

#include "Buddy.h"

#include "NormalEffect.h"
#include "AttackEffect.h"

CAnais::CAnais()
{
}


CAnais::~CAnais()
{
}

void CAnais::SetIdleState()
{
	NULL_CHECK(m_pBody);

	if (SCENEID::SCENEID_STAGE == m_pSceneMgr->GetCurScene())
	{
		if (0 < int(m_pObjMgr->GetObjList(OBJ_MONSTER).size()))
			m_pBody->SetFrameKey(L"Anais", m_pBody->AddWString_Dir_8(L"Fight_Idle", m_pBody->GetDirection()), m_pBody->STATE::IDLE, m_pBody->GetDirection(), FRAME_LOOP);
		else
			m_pBody->SetFrameKey(L"Anais", m_pBody->AddWString_Dir_8(L"Idle", m_pBody->GetDirection()), m_pBody->STATE::IDLE, m_pBody->GetDirection(), FRAME_LOOP);
	}
}

void CAnais::SetRunState()
{
	NULL_CHECK(m_pBody);
	DIRECTION eDir = m_pBody->ChangeDirToAngle(m_pBody->GetAngle());

	/*if (SCENEID::SCENEID_STAGE == m_pSceneMgr->GetCurScene())
	{
		m_pBody->SetFrameKey(L"Anais", m_pBody->AddWString_Dir_8(L"Walk", eDir), m_pBody->STATE::WALK, eDir, FRAME_LOOP);
	}
	else if (SCENEID::SCENEID_STAGE == m_pSceneMgr->GetCurScene())
	{*/
	m_pBody->SetFrameKey(L"Anais", m_pBody->AddWString_Dir_8(L"Run", eDir), m_pBody->STATE::WALK, eDir, FRAME_LOOP);
	//}
}

void CAnais::SetAttackState()
{
	NULL_CHECK(m_pBody);
	if (m_pBody->STATE::ATTACK1 != m_pBody->GetCurState())
	{
		DIRECTION eDir = m_pBody->ChangeDirToAngle(m_pBody->GetAngle());
		m_pBody->SetFrameKey(L"Anais", m_pBody->AddWString_Dir_4_Cross(L"Attack", eDir), m_pBody->STATE::ATTACK, eDir, FRAME_ONCE);
	}
}

void CAnais::SetHitState()
{
	NULL_CHECK(m_pBody);
	if (OBJID::OBJ_NPC == m_pBody->GetObjID())
	{
		m_pWordMgr->AddWord(new WORD_DATA(L"Anais", L"Á©¸®»ß ¸»¶û¸»¶ûÇÞ..! ³Ê¹« ÁÁ¾Æ!", L"Character", L"Anais"));
		m_pWordMgr->AddWord(new WORD_DATA(L"Kyoko", L";;;", L"Character", L"Kyoko"));
		m_pWordMgr->StartWord(false);
	}
}

void CAnais::Attack(SKILLID eSkillID)
{
}

void CAnais::FrameEvent()
{
	NULL_CHECK(m_pBody);
	switch (m_pBody->GetCurState()) {
	case CGameObject::STATE::ATTACK:
		if (nullptr != m_pBody->GetTarget() && !m_pBody->GetTarget()->GetDead())
		{
			if (4 == m_pBody->GetFrame().iFrameCur)
			{
				CGameObject* pAttackEffect =
					m_pBody->CreateEffect(
						m_pBody->GetInfo().vPos, D3DXVECTOR3{0.f, 0.f, 0.f}, L"Effect", L"BossSkill", true, OBJ_EFFECT, m_pBody->GetTarget(), m_pBody, FRAME_ONCE, true);
				//pAttackEffect->SetLayer(RENDER_LAYER::LAYER_OBJ);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamage(1000 + rand()%500);
			}
		}
		break;
	}
}
