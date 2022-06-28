#include "stdafx.h"
#include "Jelly.h"

#include "Buddy.h"

#include "NormalEffect.h"
#include "AttackEffect.h"

CJelly::CJelly()
{
}


CJelly::~CJelly()
{
}

void CJelly::SetIdleState()
{
	NULL_CHECK(m_pBody);

	m_pBody->SetFrameKey(L"Jelly", m_pBody->AddWString_Dir_4_Cross(L"Idle", m_pBody->GetDirection()), m_pBody->STATE::IDLE, m_pBody->GetDirection(), FRAME_LOOP);
}

void CJelly::SetRunState()
{
	NULL_CHECK(m_pBody);
	DIRECTION eDir = m_pBody->ChangeDirToAngle(m_pBody->GetAngle());
	m_pBody->SetDirection(DIRECTION::NO_DIRECTION);
	m_pBody->SetFrameKey(L"Jelly", m_pBody->AddWString_Dir_4_Cross(L"Walk", eDir), m_pBody->STATE::WALK, eDir, FRAME_LOOP);
}

void CJelly::SetAttackState()
{
	NULL_CHECK(m_pBody);
	if (m_pBody->STATE::ATTACK1 != m_pBody->GetCurState())
	{
		DIRECTION eDir = m_pBody->ChangeDirToAngle(m_pBody->GetAngle());
		m_pBody->SetDirection(DIRECTION::NO_DIRECTION);
		m_pBody->SetFrameKey(L"Jelly", m_pBody->AddWString_Dir_4_Cross(L"Attack", eDir), m_pBody->STATE::ATTACK, eDir, FRAME_ONCE);
	}
}

void CJelly::SetHitState()
{
	NULL_CHECK(m_pBody);
	if (OBJID::OBJ_NPC == m_pBody->GetObjID())
	{
		m_pWordMgr->AddWord(new WORD_DATA(L"Jelly", L"»ß!-", L"Character", L"Jelly"));
		m_pWordMgr->StartWord(false);
	}
}

void CJelly::Attack(SKILLID eSkillID)
{
}

void CJelly::FrameEvent()
{
	NULL_CHECK(m_pBody);
	switch (m_pBody->GetCurState()) {
	case CGameObject::STATE::ATTACK:
		if (nullptr != m_pBody->GetTarget() && !m_pBody->GetTarget()->GetDead())
		{
			if (6 == m_pBody->GetFrame().iFrameCur)
			{
				CGameObject* pAttackEffect =
					m_pBody->CreateEffect(
						m_pBody->GetInfo().vPos, D3DXVECTOR3{ 0.f, 0.f, 0.f }, L"Effect", L"Ring", true, OBJ_EFFECT, m_pBody->GetTarget(), m_pBody, FRAME_ONCE, true);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamage(1000 + rand()%500);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDebuff(DEBUFF_FREZE | DEBUFF_CONTINUOUS_DAMAGE);
			}
		}
		break;
	}
}
