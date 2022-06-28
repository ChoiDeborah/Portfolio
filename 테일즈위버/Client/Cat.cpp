#include "stdafx.h"
#include "Cat.h"

#include "Monster.h"

#include "NormalEffect.h"
#include "AttackEffect.h"

CCat::CCat()
{
}


CCat::~CCat()
{
}

void CCat::SetIdleState()
{
	NULL_CHECK(m_pBody);
	m_pBody->SetFrameKey(L"CatBoss", m_pBody->AddWString_Dir_4_Cross(L"Idle", m_pBody->GetDirection()), m_pBody->STATE::IDLE, m_pBody->GetDirection(), FRAME_LOOP);
}

void CCat::SetRunState()
{
	NULL_CHECK(m_pBody);
	DIRECTION eDir = m_pBody->ChangeDirToAngle(m_pBody->GetAngle());
	m_pBody->SetFrameKey(L"CatBoss", m_pBody->AddWString_Dir_4_Cross(L"Run", eDir), m_pBody->STATE::WALK, eDir, FRAME_LOOP);
}

void CCat::SetAttackState()
{
	NULL_CHECK(m_pBody);
	if (m_pBody->STATE::ATTACK1 != m_pBody->GetCurState() && m_pBody->STATE::ATTACK2 != m_pBody->GetCurState())
	{
		DIRECTION eDir = m_pBody->ChangeDirToAngle(m_pBody->GetAngle());

		m_pSoundMgr->PlaySound(L"Cat.wav", CHANNEL_ID::CHANNEL_WINDOW);

		int iRand = rand() % 3;
		switch (iRand)
		{
		case 0:
			m_pBody->SetFrameKey(L"CatBoss", m_pBody->AddWString_Dir_4_Cross(L"Attack", eDir), m_pBody->STATE::ATTACK1, eDir, FRAME_ONCE);
			break;

		case 1:
			m_pBody->SetFrameKey(L"CatBoss", m_pBody->AddWString_Dir_4_Cross(L"Attack", eDir), m_pBody->STATE::ATTACK2, eDir, FRAME_ONCE);
			break;

		case 2:
			m_pBody->SetFrameKey(L"CatBoss", m_pBody->AddWString_Dir_4_Cross(L"Attack", eDir), m_pBody->STATE::ATTACK3, eDir, FRAME_ONCE);
			break;
		}
	}
}

void CCat::SetHitState()
{
	//m_pBody->SetFrameKey(L"Cat", m_pBody->AddWString_Dir_4_Cross(L"Hit", m_pBody->GetDirection()), m_pBody->STATE::HIT, m_pBody->GetDirection(), FRAME_ONCE);
}

void CCat::Attack(SKILLID eSkillID)
{
}

void CCat::FrameEvent()
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
						m_pBody->GetTarget()->GetInfo().vPos, D3DXVECTOR3{ 0.f, 0.f, 0.f }, L"Effect", L"WaterJail", true, OBJ_EFFECT, m_pBody->GetTarget(), m_pBody, FRAME_ONCE);
				pAttackEffect->SetObjID(OBJID::OBJ_EFFECT);
				pAttackEffect->SetLayer(RENDER_LAYER::LAYER_OBJ);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetFollow(true);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamage(20 + rand() % 50);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetAttackCount(5);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamageTiming(6);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDebuff(DEBUFF_BLINED);
			}
		}
		break;

	case CGameObject::STATE::ATTACK3:
		if (nullptr != m_pBody->GetTarget() && !m_pBody->GetTarget()->GetDead())
		{
			if (1 == m_pBody->GetFrame().iFrameCur)
			{
				m_pViewMgr->SetShakeX(15.f);

				CGameObject* pAttackEffect =
					m_pBody->CreateEffect(
						m_pBody->GetTarget()->GetInfo().vPos, D3DXVECTOR3{ 0.f, 0.f, 0.f }, L"Effect", L"Blast", true, OBJ_EFFECT, m_pBody->GetTarget(), m_pBody, FRAME_ONCE);
				pAttackEffect->SetObjID(OBJID::OBJ_EFFECT);
				pAttackEffect->SetLayer(RENDER_LAYER::LAYER_OBJ);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetFollow(true);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamage(50 + rand() % 50);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamageTiming(6);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDebuff(DEBUFF_ELECTRIC_SHOCK);
			}
		}
		break;
	}
}
