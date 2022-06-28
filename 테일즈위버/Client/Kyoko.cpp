#include "stdafx.h"
#include "Kyoko.h"

#include "Player.h"

#include "NormalEffect.h"
#include "AttackEffect.h"

#include "Chain.h"
#include "Fire.h"

CKyoko::CKyoko()
{
}

CKyoko::~CKyoko()
{
}

void CKyoko::SetIdleState()
{
	NULL_CHECK(m_pBody);

	if (SCENEID::SCENEID_STAGE == m_pSceneMgr->GetCurScene())
	{
		m_pBody->SetFrameKey(L"Kyoko", m_pBody->AddWString_Dir_8(L"Idle", m_pBody->GetDirection()), m_pBody->STATE::IDLE, m_pBody->GetDirection(), FRAME_LOOP);
	}
	else if (SCENEID::SCENEID_STAGE == m_pSceneMgr->GetCurScene())
	{
		m_pBody->SetFrameKey(L"Kyoko", m_pBody->AddWString_Dir_8(L"Fight_Idle", m_pBody->GetDirection()), m_pBody->STATE::IDLE, m_pBody->GetDirection(), FRAME_LOOP);
	}
}

void CKyoko::SetRunState()
{
	NULL_CHECK(m_pBody);
	DIRECTION eDir = m_pBody->ChangeDirToAngle(m_pBody->GetAngle());

	if (dynamic_cast<CPlayer*>(m_pBody)->GetIdentity().bIsRun)
	{
		m_pBody->SetFrameKey(L"Kyoko", m_pBody->AddWString_Dir_8(L"Run", eDir), m_pBody->STATE::WALK, eDir, FRAME_LOOP);
	}
	else 
	{
		m_pBody->SetFrameKey(L"Kyoko", m_pBody->AddWString_Dir_8(L"Walk", eDir), m_pBody->STATE::WALK, eDir, FRAME_LOOP);
	}
}

void CKyoko::SetAttackState()
{
	NULL_CHECK(m_pBody);
	DIRECTION eDir = m_pBody->ChangeDirToAngle(m_pBody->GetAngle());

	int iAttackType = dynamic_cast<CPlayer*>(m_pBody)->GetCombo() % 3;

	m_pBody->SetCurState(m_pBody->STATE::END);

	switch (iAttackType)
	{
	case 0: 
		m_pBody->SetFrameKey(L"Kyoko", m_pBody->AddWString_Dir_8(L"Attack1", eDir), m_pBody->STATE::ATTACK1, eDir, FRAME_ONCE);
		break;

	case 1:
		m_pBody->SetFrameKey(L"Kyoko", m_pBody->AddWString_Dir_8(L"Attack1", eDir), m_pBody->STATE::ATTACK2, eDir, FRAME_ONCE);
		break;

	case 2:
		m_pBody->SetFrameKey(L"Kyoko", m_pBody->AddWString_Dir_8(L"Attack1", eDir), m_pBody->STATE::ATTACK3, eDir, FRAME_ONCE);
		break;
	}
}

void CKyoko::SetHitState()
{
	//m_pBody->SetFrameKey(L"Kyoko", m_pBody->AddWString_Dir_8(L"Damaged", m_pBody->GetDirection()), m_pBody->STATE::HIT, m_pBody->GetDirection(), FRAME_ONCE);
}

void CKyoko::Attack(SKILLID eSkillID)
{
	NULL_CHECK(m_pBody);

	switch (eSkillID)
	{
	case SKILLID_VERCUT:
		{
			m_pSoundMgr->PlaySound(L"ATTACK.wav", CHANNEL_ID::CHANNEL_ATTACK);
			DIRECTION eDir = m_pBody->ChangeDirToAngle(m_pBody->GetAngle());
			m_pBody->SetCurState(m_pBody->STATE::END);
			m_pBody->SetFrameKey(L"Kyoko", m_pBody->AddWString_Dir_8(L"Attack2", eDir), m_pBody->STATE::ATTACK2, eDir, FRAME_ONCE);
		}
		break;

	case SKILLID_STAB: 
		{
			m_pSoundMgr->PlaySound(L"SecondAtt.wav", CHANNEL_ID::CHANNEL_ATTACK);
			DIRECTION eDir = m_pBody->ChangeDirToAngle(m_pBody->GetAngle());
			m_pBody->SetCurState(m_pBody->STATE::END);
			m_pBody->SetFrameKey(L"Kyoko", m_pBody->AddWString_Dir_8(L"Attack1", eDir), m_pBody->STATE::ATTACK1, eDir, FRAME_ONCE);
		}
		break;


	case SKILLID_ACTIVE0:
	case SKILLID_ACTIVE1:
	case SKILLID_ACTIVE2:
	case SKILLID_ACTIVE3:
		{
		if (m_pBody->GetUnitData().iMp < 10)
		{
			m_pSoundMgr->PlaySound(L"NoMana.MP3", CHANNEL_ID::CHANNEL_ATTACK);
			return;
		}
		m_pBody->GetUnitData().iMp -= 10;

			m_pSoundMgr->PlaySound(L"SecondAtt.wav", CHANNEL_ID::CHANNEL_ATTACK);
			DIRECTION eDir = m_pBody->ChangeDirToAngle(m_pBody->GetAngle());
			m_pBody->SetCurState(m_pBody->STATE::END);
			m_pBody->SetFrameKey(L"Kyoko", m_pBody->AddWString_Dir_8(L"Attack1", eDir), m_pBody->STATE::ATTACK3, eDir, FRAME_ONCE);

			wstring wstrDir = m_pBody->AddWString_Dir_2(L"", eDir);

			CGameObject* pAttackEffect = nullptr;

			//int iAttackType = dynamic_cast<CPlayer*>(m_pBody)->GetCombo() % 4;
			switch (m_iCombo % 4)
			{
			case 0:
				pAttackEffect =
					m_pBody->CreateEffect(
						m_pBody->GetInfo().vPos, D3DXVECTOR3{ 0.f, 0.f, 0.f },
						L"Creature", L"Attack0" + wstrDir, true, OBJ_PLAYER_ATTACK, m_pBody, nullptr, FRAME_ONCE, true);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetAttackCount(1);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamageTiming(-1);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamage(700 + rand() % 200);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDebuff(DEBUFF_FREZE | DEBUFF_CONTINUOUS_DAMAGE);
				break;

			case 1:
				pAttackEffect =
					m_pBody->CreateEffect(
						m_pBody->GetInfo().vPos, D3DXVECTOR3{ 0.f, 0.f, 0.f },
						L"Creature", L"Attack1" + wstrDir, true, OBJ_PLAYER_ATTACK, m_pBody, nullptr, FRAME_ONCE, true);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetAttackCount(2);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamageTiming(-1);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamage(700 + rand() % 200);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDebuff(DEBUFF_FREZE | DEBUFF_CONTINUOUS_DAMAGE);
				break;

			case 2:
				pAttackEffect =
					m_pBody->CreateEffect(
						m_pBody->GetInfo().vPos, D3DXVECTOR3{ 0.f, 0.f, 0.f },
						L"Creature", L"Attack2" + wstrDir, true, OBJ_PLAYER_ATTACK, m_pBody, nullptr, FRAME_ONCE, true);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetAttackCount(3);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamageTiming(-1);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamage(700 + rand() % 200);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDebuff(DEBUFF_FREZE | DEBUFF_CONTINUOUS_DAMAGE);
				break;

			case 3:
				pAttackEffect =
					m_pBody->CreateEffect(
						m_pBody->GetInfo().vPos, D3DXVECTOR3{ 0.f, 0.f, 0.f },
						L"Creature", L"Attack3" + wstrDir, true, OBJ_PLAYER_ATTACK, m_pBody, nullptr, FRAME_ONCE, true);
				//const_cast<INFO*>(&pAttackEffect->GetInfo())->vSize = { 0.5f, 0.5f, 0.f };
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetAttackCount(4);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamageTiming(-1);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamage(700 + rand() % 200);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDebuff(DEBUFF_FREZE | DEBUFF_CONTINUOUS_DAMAGE);
				break;
			}
			++m_iCombo;
		}
		break;

	case SKILLID_HORCUT:
		if (m_pBody->GetUnitData().iMp < 10)
		{
			m_pSoundMgr->PlaySound(L"NoMana.MP3", CHANNEL_ID::CHANNEL_ATTACK);
			return;
		}
		m_pBody->GetUnitData().iMp -= 10;
		m_pSoundMgr->PlaySound(L"MultiAttack3.WAV", CHANNEL_ID::CHANNEL_ATTACK);
		m_eSkillNum = PLAYER_SKILL::SKILL2;
		m_pBody->SetFrameKey(L"Kyoko", m_pBody->AddWString_Dir_4_Cross(L"Attack1", m_pBody->GetDirection()), m_pBody->STATE::SKILL, m_pBody->GetDirection(), FRAME_ONCE);

	case SKILLID_ACTIVE4:
		if (m_pBody->GetUnitData().iMp < 10)
		{
			m_pSoundMgr->PlaySound(L"NoMana.MP3", CHANNEL_ID::CHANNEL_ATTACK);
			return;
		}
		m_pBody->GetUnitData().iMp -= 10;
		m_pSoundMgr->PlaySound(L"SKILL_A.MP3", CHANNEL_ID::CHANNEL_ATTACK);
		m_eSkillNum = PLAYER_SKILL::SKILL3;
		m_pBody->SetFrameKey(L"Kyoko", m_pBody->AddWString_Dir_4_Cross(L"Attack1", m_pBody->GetDirection()), m_pBody->STATE::SKILL, m_pBody->GetDirection(), FRAME_ONCE);
		break;

	case SKILLID_RUSHCRASH:
		if (m_pBody->GetUnitData().iMp < 10)
		{
			m_pSoundMgr->PlaySound(L"NoMana.MP3", CHANNEL_ID::CHANNEL_ATTACK);
			return;
		}
		m_pBody->GetUnitData().iMp -= 10;
		m_pBody->SetCurState(m_pBody->IDLE);
		m_eSkillNum = PLAYER_SKILL::SKILL4;
		m_pBody->SetFrameKey(L"Kyoko", m_pBody->AddWString_Dir_4_Cross(L"Skill", m_pBody->GetDirection()), m_pBody->STATE::SKILL, m_pBody->GetDirection(), FRAME_ONCE);
		break;

	case SKILLID_STARBUSTSTREAM:
	{
		if (m_pBody->GetUnitData().iMp < 10)
		{
			m_pSoundMgr->PlaySound(L"NoMana.MP3", CHANNEL_ID::CHANNEL_ATTACK);
			return;
		}
		m_pBody->GetUnitData().iMp -= 10;

		m_pSoundMgr->PlaySound(L"SKILL_A.MP3", CHANNEL_ID::CHANNEL_ATTACK);

		m_eSkillNum = PLAYER_SKILL::SKILL1;
		m_pBody->SetFrameKey(L"Kyoko", m_pBody->AddWString_Dir_4_Cross(L"Attack1", m_pBody->GetDirection()), m_pBody->STATE::SKILL, m_pBody->GetDirection(), FRAME_ONCE);

		CGameObject* pChain = CAbstractFactory<CChain>::CreateObj(m_pBody->GetInfo().vPos + D3DXVECTOR3{ float(rand() % 30), float(rand() % 30), 0.f });
		dynamic_cast<CChain*>(pChain)->SetPlayer(m_pBody);
		dynamic_cast<CChain*>(pChain)->SetMirrol(true);
		m_pObjMgr->AddObject(pChain, OBJ_EFFECT);

		pChain = CAbstractFactory<CChain>::CreateObj(m_pBody->GetInfo().vPos + D3DXVECTOR3{ float(rand() % 30), float(rand() % 30), 0.f });
		dynamic_cast<CChain*>(pChain)->SetPlayer(m_pBody);
		m_pObjMgr->AddObject(pChain, OBJ_EFFECT);

		CGameObject* pAttackEffect =
			m_pBody->CreateEffect(
				m_pBody->GetInfo().vPos, D3DXVECTOR3{ 0.f, 0.f, 0.f },
				L"Creature", L"Attack11", true, OBJ_PLAYER_ATTACK, m_pBody, nullptr, FRAME_ONCE, true);
		dynamic_cast<CAttackEffect*>(pAttackEffect)->SetAttackCount(1);
		dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamageTiming(-1);
		dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamage(700 + rand() % 200);
		dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDebuff(DEBUFF_FREZE | DEBUFF_CONTINUOUS_DAMAGE);
	}
	break;

	case SKILLID_FIREWAVE:
	{
		if (m_pBody->GetUnitData().iMp < 10)
		{
			m_pSoundMgr->PlaySound(L"NoMana.MP3", CHANNEL_ID::CHANNEL_ATTACK);
			return;
		}
		m_pBody->GetUnitData().iMp -= 10;

		CGameObject* pFire = CAbstractFactory<CFire>::CreateObj(m_pBody->GetInfo().vPos);
		dynamic_cast<CFire*>(pFire)->SetPlayer(m_pBody);
		m_pObjMgr->AddObject(pFire, OBJ_EFFECT);

		m_pBody->SetCurState(m_pBody->IDLE);
		m_eSkillNum = PLAYER_SKILL::SKILL5;
		m_pBody->SetFrameKey(L"Kyoko", m_pBody->AddWString_Dir_4_Cross(L"Attack1", m_pBody->GetDirection()), m_pBody->STATE::SKILL, m_pBody->GetDirection(), FRAME_ONCE);
	}
		break;
	}
}

void CKyoko::FrameEvent()
{
	NULL_CHECK(m_pBody);
	switch (m_pBody->GetCurState()) {
	case CGameObject::STATE::ATTACK1:
		if (nullptr != m_pBody->GetTarget() && !m_pBody->GetTarget()->GetDead())
		{
			switch (m_pBody->GetFrame().iFrameCur)
			{
			case 1:
			{
				m_pViewMgr->SetShakeX(5.f);
				
				CGameObject* pAttackEffect =
					m_pBody->CreateEffect(
						m_pBody->GetTarget()->GetInfo().vPos, D3DXVECTOR3{ -float(rand() % 20 - 10), float(rand() % 20 - 10) + m_pBody->GetOBB().vCollSize.y * 0.5f, 0.f },
						L"Effect", L"Hit", true, OBJ_EFFECT, m_pBody->GetTarget(), m_pBody, FRAME_ONCE);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamage(1000 + rand() % 500);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamageTiming(1);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDebuff(DEBUFF_POISION);
			} break;
			}
		}
		break;

	case CGameObject::STATE::ATTACK2:
		if (nullptr != m_pBody->GetTarget() && !m_pBody->GetTarget()->GetDead())
		{
			switch (m_pBody->GetFrame().iFrameCur)
			{
			case 1:
			{
				CGameObject* pAttackEffect =
					m_pBody->CreateEffect(
						m_pBody->GetTarget()->GetInfo().vPos, D3DXVECTOR3{ -float(rand() % 20 - 10), float(rand() % 20 - 10) + m_pBody->GetOBB().vCollSize.y * 0.5f, 0.f },
						L"Effect", L"Hit", true, OBJ_EFFECT, m_pBody->GetTarget(), m_pBody, FRAME_ONCE);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamage(1500 + rand() % 500);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamageTiming(1);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDebuff(DEBUFF_FREZE);
			} break;
			}
		}
		break;

	case CGameObject::STATE::ATTACK3:
		if (nullptr != m_pBody->GetTarget() && !m_pBody->GetTarget()->GetDead())
		{
			switch (m_pBody->GetFrame().iFrameCur)
			{
			case 1:
			{
				m_pViewMgr->SetShakeX(15.f);

				{
					CGameObject* pAttackEffect =
						m_pBody->CreateEffect(
							m_pBody->GetTarget()->GetInfo().vPos, D3DXVECTOR3{ -float(rand() % 20 - 10), float(rand() % 20 - 10) + m_pBody->GetOBB().vCollSize.y * 0.5f, 0.f },
							L"Effect", L"Hit", true, OBJ_EFFECT, m_pBody->GetTarget(), m_pBody, FRAME_ONCE);
					dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamage(1500 + rand() % 500);
					dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamageTiming(1);
					dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDebuff(DEBUFF_FREZE);

					//DIRECTION eDir = m_pBody->ChangeDirToAngle(m_pBody->GetAngle());
					//wstring wstrDir = m_pBody->AddWString_Dir_2(L"", eDir);
					//
					////int iAttackType = dynamic_cast<CPlayer*>(m_pBody)->GetCombo() % 4;
					//switch (m_iCombo % 4)
					//{
					//case 0:
					//	pAttackEffect =
					//		m_pBody->CreateEffect(
					//			m_pBody->GetInfo().vPos, D3DXVECTOR3{ 0.f, 0.f, 0.f },
					//			L"Creature", L"Attack0" + wstrDir, true, OBJ_PLAYER_ATTACK, m_pBody, nullptr, FRAME_ONCE, true);
					//	dynamic_cast<CAttackEffect*>(pAttackEffect)->SetAttackCount(1);
					//	dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamageTiming(-1);
					//	dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamage(700 + rand() % 200);
					//	dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDebuff(DEBUFF_FREZE | DEBUFF_CONTINUOUS_DAMAGE);
					//	break;

					//case 1:
					//	pAttackEffect =
					//		m_pBody->CreateEffect(
					//			m_pBody->GetInfo().vPos, D3DXVECTOR3{ 0.f, 0.f, 0.f },
					//			L"Creature", L"Attack1" + wstrDir, true, OBJ_PLAYER_ATTACK, m_pBody, nullptr, FRAME_ONCE, true);
					//	dynamic_cast<CAttackEffect*>(pAttackEffect)->SetAttackCount(2);
					//	dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamageTiming(-1);
					//	dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamage(700 + rand() % 200);
					//	dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDebuff(DEBUFF_FREZE | DEBUFF_CONTINUOUS_DAMAGE);
					//	break;

					//case 2:
					//	pAttackEffect =
					//		m_pBody->CreateEffect(
					//			m_pBody->GetInfo().vPos, D3DXVECTOR3{ 0.f, 0.f, 0.f },
					//			L"Creature", L"Attack2" + wstrDir, true, OBJ_PLAYER_ATTACK, m_pBody, nullptr, FRAME_ONCE, true);
					//	dynamic_cast<CAttackEffect*>(pAttackEffect)->SetAttackCount(3);
					//	dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamageTiming(-1);
					//	dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamage(700 + rand() % 200);
					//	dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDebuff(DEBUFF_FREZE | DEBUFF_CONTINUOUS_DAMAGE);
					//	break;

					//case 3:
					//	pAttackEffect =
					//		m_pBody->CreateEffect(
					//			m_pBody->GetInfo().vPos, D3DXVECTOR3{ 0.f, 0.f, 0.f },
					//			L"Creature", L"Attack3" + wstrDir, true, OBJ_PLAYER_ATTACK, m_pBody, nullptr, FRAME_ONCE, true);
					//	//const_cast<INFO*>(&pAttackEffect->GetInfo())->vSize = { 0.5f, 0.5f, 0.f };
					//	dynamic_cast<CAttackEffect*>(pAttackEffect)->SetAttackCount(4);
					//	dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamageTiming(-1);
					//	dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamage(700 + rand() % 200);
					//	dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDebuff(DEBUFF_FREZE | DEBUFF_CONTINUOUS_DAMAGE);
					//	break;
					//}
					//++m_iCombo;
				}
				/*CGameObject* pAttackEffect =
					m_pBody->CreateEffect(
						m_pBody->GetTarget()->GetInfo().vPos, D3DXVECTOR3{ -float(rand() % 20 - 10), float(rand() % 20 - 10) + m_pBody->GetOBB().vCollSize.y * 0.5f, 0.f },
						L"Effect", L"Hit", true, OBJ_EFFECT, m_pBody->GetTarget(), m_pBody, FRAME_ONCE);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamage(2000 + rand() % 500);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamageTiming(1);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDebuff(DEBUFF_POISION);*/
			} break;
			}
		}
		break;

	case CGameObject::STATE::SKILL:
		switch (m_eSkillNum)
		{
		case PLAYER_SKILL::SKILL2:
			if (1 == m_pBody->GetFrame().iFrameCur)
			{
				m_pViewMgr->SetShakeX(10.f);

				DIRECTION eDir = m_pBody->ChangeDirToAngle(m_pBody->GetAngle());
				wstring wstrDir = m_pBody->AddWString_Dir_2(L"", eDir);
				
				CGameObject* pAttackEffect = nullptr;

				//int iAttackType = dynamic_cast<CPlayer*>(m_pBody)->GetCombo() % 4;
				switch (m_iCombo % 4)
				{
				case 0:
					pAttackEffect =
						m_pBody->CreateEffect(
							m_pBody->GetInfo().vPos, D3DXVECTOR3{ 0.f, 0.f, 0.f },
							L"Creature", L"Attack0" + wstrDir, true, OBJ_PLAYER_ATTACK, m_pBody, nullptr, FRAME_ONCE, true);
					dynamic_cast<CAttackEffect*>(pAttackEffect)->SetAttackCount(1);
					dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamageTiming(-1);
					dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamage(700 + rand() % 200);
					dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDebuff(DEBUFF_FREZE | DEBUFF_CONTINUOUS_DAMAGE);
					break;

				case 1:
					pAttackEffect =
						m_pBody->CreateEffect(
							m_pBody->GetInfo().vPos, D3DXVECTOR3{ 0.f, 0.f, 0.f },
							L"Creature", L"Attack1" + wstrDir, true, OBJ_PLAYER_ATTACK, m_pBody, nullptr, FRAME_ONCE, true);
					dynamic_cast<CAttackEffect*>(pAttackEffect)->SetAttackCount(2);
					dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamageTiming(-1);
					dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamage(700 + rand() % 200);
					dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDebuff(DEBUFF_FREZE | DEBUFF_CONTINUOUS_DAMAGE);
					break;

				case 2:
					pAttackEffect =
						m_pBody->CreateEffect(
							m_pBody->GetInfo().vPos, D3DXVECTOR3{ 0.f, 0.f, 0.f },
							L"Creature", L"Attack2" + wstrDir, true, OBJ_PLAYER_ATTACK, m_pBody, nullptr, FRAME_ONCE, true);
					dynamic_cast<CAttackEffect*>(pAttackEffect)->SetAttackCount(3);
					dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamageTiming(-1);
					dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamage(700 + rand() % 200);
					dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDebuff(DEBUFF_FREZE | DEBUFF_CONTINUOUS_DAMAGE);
					break;

				case 3:
					pAttackEffect =
						m_pBody->CreateEffect(
							m_pBody->GetInfo().vPos, D3DXVECTOR3{ 0.f, 0.f, 0.f },
							L"Creature", L"Attack3" + wstrDir, true, OBJ_PLAYER_ATTACK, m_pBody, nullptr, FRAME_ONCE, true);
					//const_cast<INFO*>(&pAttackEffect->GetInfo())->vSize = { 0.5f, 0.5f, 0.f };
					dynamic_cast<CAttackEffect*>(pAttackEffect)->SetAttackCount(4);
					dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamageTiming(-1);
					dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamage(700 + rand() % 200);
					dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDebuff(DEBUFF_FREZE | DEBUFF_CONTINUOUS_DAMAGE);
					break;
				}
				++m_iCombo;
			}
			break;

		case PLAYER_SKILL::SKILL3:
		{
			if (1 == m_pBody->GetFrame().iFrameCur)
			{
				CGameObject* pAttackEffect =
					m_pBody->CreateEffect(
						m_pBody->GetInfo().vPos, D3DXVECTOR3{ 0.f, 0.f, 0.f },
						L"Creature", L"Attack4", true, OBJ_PLAYER_ATTACK, nullptr, nullptr, FRAME_ONCE, false);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamage(500 + rand() % 250);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetAttackCount(20);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamageTiming(1);
				dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDebuff(DEBUFF_FREZE);
			}
		}
			break;

		// 지정 사슬
		case PLAYER_SKILL::SKILL4:
			if (26 == m_pBody->GetFrame().iFrameCur)
			{
				m_pViewMgr->SetShakeX(20.f);

				OBJLIST pObjMonster = m_pObjMgr->GetObjList(OBJ_MONSTER);

				for (auto Monster : pObjMonster)
				{
					if (!Monster->GetDead())
					{
						if (500.f > D3DXVec3Length(&(Monster->GetInfo().vPos - m_pBody->GetInfo().vPos)))
						{
							m_vecSkillTarget.push_back(Monster);
						}
					}
				}

				//소팅
				sort(m_vecSkillTarget.begin(), m_vecSkillTarget.end(), [&](CGameObject* pA, CGameObject* pB) {
					float fDistA = D3DXVec3Length(&(pA->GetInfo().vPos - m_pBody->GetInfo().vPos));
					float fDistB = D3DXVec3Length(&(pB->GetInfo().vPos - m_pBody->GetInfo().vPos));

					return fDistA < fDistB;
				});
			}

			else if (27 <= m_pBody->GetFrame().iFrameCur && 33 > m_pBody->GetFrame().iFrameCur)
			{
				int iNum = (int(m_vecSkillTarget.size()) - 1) - (m_pBody->GetFrame().iFrameCur - 27);
				CGameObject* pTarget = nullptr;
				do
				{
					pTarget = nullptr;
					if (0 <= iNum && int(m_vecSkillTarget.size()) > iNum)
						pTarget = m_vecSkillTarget[iNum];
					
					--iNum;
				} while (-1 <= iNum && (nullptr == pTarget || (nullptr != pTarget && pTarget->GetDead())));
				
				if (nullptr != pTarget)
				{
					CGameObject* pEffect = m_pBody->CreateEffect(pTarget->GetInfo().vPos, D3DXVECTOR3{ 0.f, 0.f, 0.f }, L"Effect", L"BossSpell", false, OBJ_EFFECT, pTarget, nullptr, FRAME_ONCE);
					dynamic_cast<CEffect*>(pEffect)->SetFollow(true);
				}
			}

			else if (33 == m_pBody->GetFrame().iFrameCur)
			{
				m_pViewMgr->SetShakeX(20.f);

				OBJLIST pObjMonster = m_pObjMgr->GetObjList(OBJ_MONSTER);
				for (auto Monster : m_vecSkillTarget)
				{
					if (nullptr != Monster && !Monster->GetDead())
					{
						if (500.f > D3DXVec3Length(&(Monster->GetInfo().vPos - m_pBody->GetInfo().vPos)))
						{
							wstring wstrDir = (Monster->GetInfo().vPos.x > m_pBody->GetInfo().vPos.x) ? L"_R" : L"_L";

							CGameObject* pAttackEffect =
								m_pBody->CreateEffect(
									m_pBody->GetInfo().vPos, D3DXVECTOR3{ 0.f, 0.f, 0.f },
									L"Creature", L"Attack8" + wstrDir, true, OBJ_EFFECT, Monster, m_pBody, FRAME_ONCE, true);
							dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamage(900 + rand() % 400);
							dynamic_cast<CAttackEffect*>(pAttackEffect)->SetAttackCount(10);
							dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamageTiming(1);
							//dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamageTiming(-1);
							dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDebuff(DEBUFF_FREZE);

							/*CGameObject* pAttackEffect =
								m_pBody->CreateEffect(
									Monster->GetInfo().vPos, D3DXVECTOR3{ 0.f, 0.f, 0.f },
									L"Effect", L"Chain", true, OBJ_EFFECT, Monster, m_pBody, FRAME_ONCE, true);
							const_cast<INFO*>(&pAttackEffect->GetInfo())->vSize = { 0.5f, 0.5f, 0.f };
							dynamic_cast<CAttackEffect*>(pAttackEffect)->SetAttackCount(10);
							dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamageTiming(-1);
							dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamage(1000 + rand() % 300);
							dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDebuff(DEBUFF_POISION);*/
						}
					}
				}
			}
			else if (m_pBody->GetFrame().iFrameEnd - 3 <= m_pBody->GetFrame().iFrameCur)
			{
				m_vecSkillTarget.clear();
				m_vecSkillTarget.shrink_to_fit();
			}
		}

		break;

		case PLAYER_SKILL::SKILL5:
		{
			DIRECTION eDir = m_pBody->ChangeDirToAngle(m_pBody->GetAngle());
			//wstring wstrDir = m_pBody->AddWString_Dir_2(L"Attack10", eDir);

			CGameObject* pAttackEffect =
				m_pBody->CreateEffect(
					m_pBody->GetInfo().vPos, D3DXVECTOR3{ 0.f, 0.f, 0.f },
					L"Effect", L"WaterJail", true, OBJ_PLAYER_ATTACK, nullptr, m_pBody, FRAME_ONCE, true);

			dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamage(900 + rand() % 400);
			dynamic_cast<CAttackEffect*>(pAttackEffect)->SetAttackCount(4);
			dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDamageTiming(1);
			dynamic_cast<CAttackEffect*>(pAttackEffect)->SetDebuff(DEBUFF_POISION);
		}
		break;
	}
}
