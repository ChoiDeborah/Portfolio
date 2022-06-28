#include "stdafx.h"
#include "DarkCow.h"

#include "Monster.h"

#include "NormalEffect.h"
#include "AttackEffect.h"

CDarkCow::CDarkCow()
{
}


CDarkCow::~CDarkCow()
{
}

void CDarkCow::SetIdleState()
{
	NULL_CHECK(m_pBody);
	m_pBody->SetFrameKey(L"DarkCow", m_pBody->AddWString_Dir_4_Cross(L"Idle", m_pBody->GetDirection()), m_pBody->STATE::IDLE, m_pBody->GetDirection(), FRAME_LOOP);
}

void CDarkCow::SetRunState()
{
	NULL_CHECK(m_pBody);
	DIRECTION eDir = m_pBody->ChangeDirToAngle(m_pBody->GetAngle());
	m_pBody->SetFrameKey(L"DarkCow", m_pBody->AddWString_Dir_4_Cross(L"Walk", eDir), m_pBody->STATE::WALK, eDir, FRAME_LOOP);
}

void CDarkCow::SetAttackState()
{
	NULL_CHECK(m_pBody);
	if (m_pBody->STATE::ATTACK1 != m_pBody->GetCurState())
	{
		m_pSoundMgr->PlaySound(L"Cow.wav", CHANNEL_ID::CHANNEL_MONSTER_ATTACK);
		DIRECTION eDir = m_pBody->ChangeDirToAngle(m_pBody->GetAngle());
		m_pBody->SetFrameKey(L"DarkCow", m_pBody->AddWString_Dir_4_Cross(L"Attack", eDir), m_pBody->STATE::ATTACK1, eDir, FRAME_ONCE);
	}
}

void CDarkCow::SetHitState()
{
	//m_pBody->SetFrameKey(L"Kyoko", m_pBody->AddWString_Dir_8(L"Damaged", m_pBody->GetDirection()), m_pBody->STATE::HIT, m_pBody->GetDirection(), FRAME_ONCE);
}

void CDarkCow::Attack(SKILLID eSkillID)
{
}

void CDarkCow::FrameEvent()
{
	NULL_CHECK(m_pBody);
	switch (m_pBody->GetCurState()) {
	case CGameObject::STATE::ATTACK1:
		if (nullptr != m_pBody->GetTarget() && !m_pBody->GetTarget()->GetDead())
		{
			if (7 == m_pBody->GetFrame().iFrameCur)
			{
				m_pViewMgr->SetShakeX(15.f);

				CGameObject* pAttackEffect =
					m_pBody->CreateEffect(
						m_pBody->GetInfo().vPos, D3DXVECTOR3{ 0.f, 0.f, 0.f }, L"Effect", L"Cray", true, OBJ_EFFECT, m_pBody->GetTarget(), m_pBody, FRAME_ONCE);
				pAttackEffect->SetLayer(RENDER_LAYER::LAYER_OBJ);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamage(50 + rand()%50);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDebuff(DEBUFF_BLINED);
			}
		}
		break;
	}
}
