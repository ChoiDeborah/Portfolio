#include "stdafx.h"
#include "Fixy.h"

#include "Monster.h"

#include "NormalEffect.h"
#include "AttackEffect.h"

CFixy::CFixy()
{
}


CFixy::~CFixy()
{
}

void CFixy::SetIdleState()
{
	NULL_CHECK(m_pBody);
	m_pBody->SetFrameKey(L"Fixy", m_pBody->AddWString_Dir_4_Cross(L"Idle", m_pBody->GetDirection()), m_pBody->STATE::IDLE, m_pBody->GetDirection(), FRAME_LOOP);
}

void CFixy::SetRunState()
{
	NULL_CHECK(m_pBody);
	DIRECTION eDir = m_pBody->ChangeDirToAngle(m_pBody->GetAngle());
	m_pBody->SetFrameKey(L"Fixy", m_pBody->AddWString_Dir_4_Cross(L"Walk", eDir), m_pBody->STATE::WALK, eDir, FRAME_LOOP);
}

void CFixy::SetAttackState()
{
	NULL_CHECK(m_pBody);
	if (m_pBody->STATE::ATTACK1 != m_pBody->GetCurState() && m_pBody->STATE::ATTACK2 != m_pBody->GetCurState())
	{
		DIRECTION eDir = m_pBody->ChangeDirToAngle(m_pBody->GetAngle());

		int iRand = rand() % 2;
		if (0 == iRand)
		{
			m_pBody->SetFrameKey(L"Fixy", m_pBody->AddWString_Dir_4_Cross(L"Attack", eDir), m_pBody->STATE::ATTACK1, eDir, FRAME_ONCE);
		}
		else
		{
			m_pBody->SetFrameKey(L"Fixy", m_pBody->AddWString_Dir_4_Cross(L"Attack", eDir), m_pBody->STATE::ATTACK2, eDir, FRAME_ONCE);
		}
	}
}

void CFixy::SetHitState()
{
	//m_pBody->SetFrameKey(L"Fixy", m_pBody->AddWString_Dir_4_Cross(L"Hit", m_pBody->GetDirection()), m_pBody->STATE::HIT, m_pBody->GetDirection(), FRAME_ONCE);
}

void CFixy::Attack(SKILLID eSkillID)
{
}

void CFixy::FrameEvent()
{
	NULL_CHECK(m_pBody);
	switch (m_pBody->GetCurState()) {
	case CGameObject::STATE::ATTACK1:
		if (nullptr != m_pBody->GetTarget() && !m_pBody->GetTarget()->GetDead())
		{
			if (1 == m_pBody->GetFrame().iFrameCur)
			{
				m_pViewMgr->SetShakeX(15.f);

				CGameObject* pAttackEffect =
					m_pBody->CreateEffect(
						m_pBody->GetTarget()->GetInfo().vPos, D3DXVECTOR3{ 0.f, 0.f, 0.f }, L"Effect", L"Thunder", true, OBJ_EFFECT, m_pBody->GetTarget(), m_pBody, FRAME_ONCE);
				pAttackEffect->SetObjID(OBJID::OBJ_EFFECT);
				pAttackEffect->SetLayer(RENDER_LAYER::LAYER_OBJ);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetFollow(true);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamage(50 + rand() % 50);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamageTiming(6);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDebuff(DEBUFF_ELECTRIC_SHOCK);
			}
		}
		break;

	case CGameObject::STATE::ATTACK2:
		if (nullptr != m_pBody->GetTarget() && !m_pBody->GetTarget()->GetDead())
		{
			if (1 == m_pBody->GetFrame().iFrameCur)
			{
				m_pViewMgr->SetShakeX(15.f);

				CGameObject* pAttackEffect =
					m_pBody->CreateEffect(
						m_pBody->GetTarget()->GetInfo().vPos, D3DXVECTOR3{ 0.f, 0.f, 0.f }, L"Effect", L"Thunder2", true, OBJ_EFFECT, m_pBody->GetTarget(), m_pBody, FRAME_ONCE);
				pAttackEffect->SetObjID(OBJID::OBJ_EFFECT);
				pAttackEffect->SetLayer(RENDER_LAYER::LAYER_OBJ);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetFollow(true);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamage(20 + rand() % 50);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetAttackCount(2);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamageTiming(6);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDebuff(DEBUFF_BLINED);
			}
		}
		break;
	}
}
