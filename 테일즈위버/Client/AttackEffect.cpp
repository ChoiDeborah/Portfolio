#include "stdafx.h"
#include "AttackEffect.h"
#include "EventObject.h"

#include "DamageEffect.h"


CAttackEffect::CAttackEffect()
{
}

CAttackEffect::~CAttackEffect()
{
	Release();
}

HRESULT CAttackEffect::Initialize()
{
	m_eLayer = LAYER_EFFECT;
	m_eDirection = NO_DIRECTION;

	return S_OK;
}

int CAttackEffect::Update()
{
	CGameObject::LateInit();

	if (m_OBJID != OBJID::OBJ_PLAYER_ATTACK && m_OBJID != OBJID::OBJ_ATTACK)
	{
		if (nullptr != m_pTarget) {
			if (!m_pTarget->GetDead())
			{
				if (m_bIsFollow)
				{
					m_tInfo.vPos.x = m_pTarget->GetInfo().vPos.x;
					m_tInfo.vPos.y = m_pTarget->GetInfo().vPos.y;
				}
			}
		}
	}
	else
	{
		if (nullptr != m_pFromObj) {
			if (!m_pFromObj->GetDead())
			{
				if (m_bIsFollow)
				{
					m_tInfo.vPos.x = m_pFromObj->GetInfo().vPos.x;
					m_tInfo.vPos.y = m_pFromObj->GetInfo().vPos.y;
				}
			}
		}
	}

	return m_bIsDead;
}

int CAttackEffect::LateUpdate()
{
	CGameObject::UpdateView();
	if (!m_bIsDead)
		FrameMove();

	return NO_EVENT;
}

void CAttackEffect::Render()
{
	m_pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrObjKey, m_wstrStateKey, m_tFrame.iFrameCur);
	NULL_CHECK(m_pTexInfo);

	D3DXMATRIX matScale, matTrans, matRotZ;

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x * m_pTexInfo->m_iMirror, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	D3DXMatrixRotationZ(&matRotZ, m_fAngle);
	m_tInfo.matWorld = matScale * matRotZ * matTrans;
	m_pDeviceMgr->GetSprite()->SetTransform(&m_tInfo.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&(m_pTexInfo->vOffset + m_vOffset), nullptr, m_dwColor);//m_pTexInfo->dwBlend);

	//CGameObject::DrawCollRect();
}

HRESULT CAttackEffect::LateInit()
{
	const TEX_INFO*	pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrObjKey, m_wstrStateKey);
	if (nullptr != pTexInfo)
	{
		m_tOBB.vPos = m_tInfo.vPos;
		m_tOBB.vCollSize = D3DXVECTOR3{ float(pTexInfo->tImgInfo.Width), float(pTexInfo->tImgInfo.Height), 0.f };
		m_tOBB.vOffset = (1 == pTexInfo->m_iMirror) ? pTexInfo->vOffset : D3DXVECTOR3{ m_tOBB.vCollSize.x - pTexInfo->vOffset.x, pTexInfo->vOffset.y, 0.f };
		CGameObject::InitCollVertex();

		m_tFrame.fFrameTime = 0.f;
		m_tFrame.fFrameDelay = pTexInfo->m_fFrameDelay;

		m_tFrame.iFrameCur = 0;
		m_tFrame.iFrameEnd = m_pTextureMgr->GetTexCount(m_wstrObjKey, m_wstrStateKey);
	}

	return S_OK;
}

void CAttackEffect::Release()
{
}

void CAttackEffect::FrameOnceEnd()
{
	m_bIsDead = true;
}

void CAttackEffect::FrameEvent()
{
	if (m_OBJID != OBJID::OBJ_PLAYER_ATTACK && m_OBJID != OBJID::OBJ_ATTACK)
	{
		if (nullptr == m_pTarget || (nullptr != m_pTarget && m_pTarget->GetDead()) ||
			nullptr == m_pFromObj || (nullptr != m_pFromObj && m_pFromObj->GetDead()))
		{
			m_bIsDead = true;
			return;
		}
		if (m_tFrame.iFrameCur == m_iDamageTiming)
		{
			CGameObject* pFrom = (nullptr == m_pFromObj || (nullptr != m_pFromObj && m_pFromObj->GetDead())) ? nullptr : m_pFromObj;

			//UNIT_INFO	tFromUnitInfo = (dynamic_cast<CEventObject*>(m_pFromObj)->GetUnitData());
			//UNIT_INFO	tToUnitInfo = (dynamic_cast<CEventObject*>(m_pTarget)->GetUnitData());

			for (int i = 0; i < m_iAttackCount; ++i)
			{
				
				// ������ ����

				// ���� : 3.f * (DEF + ���� �հ�)
				// ���� : 3.f * (MR + ���� �հ�)


				//float fDamage = 0.f;

				//switch (m_eAttackType)
				//{
				//	// ��� : 2.1f * STAB + 1.08f * HACK ::: ��� ���ݷ� : [6.67f * ���� �հ� + 1.f * ���� ���ݷ� �հ�]  ::: (���� 2.1 ��� ���ݷ��� 6.7ȿ��)
				//case ATTACK_STAB:
				//	fDamage = (2.1f * (pFromUnitInfo->iStab + pFromUnitInfo->iAddStab) + 1.08f *  (pFromUnitInfo->iHack + pFromUnitInfo->iAddHack)) - (3.f * (pToUnitInfo->iDef + pToUnitInfo->iAddDef));
				//	break;

				//	// ���� : 1.08f * STAB + 2.1f * HACK ::: ��� ���ݷ� : [1.f * ���� �հ� + 6.67f * ���� ���ݷ� �հ�] ::: (�� 2.1 ���� ���ݷ��� 6.7ȿ��)
				//case ATTACK_HACK:
				//	fDamage = (1.08f *  (pFromUnitInfo->iStab + pFromUnitInfo->iAddStab) + 2.1f *  (pFromUnitInfo->iHack + pFromUnitInfo->iAddHack)) - (3.f * (pToUnitInfo->iDef + pToUnitInfo->iAddDef));
				//	break;

				//	// ���� : 2.4f * INT + 0.6f * MR ::: ��� ���ݷ� : [5.95f * �����հ� + 1.08f * ���� �հ�]
				//case ATTACK_INT:
				//	fDamage = (2.4f * (pFromUnitInfo->iInt + pFromUnitInfo->iAddInt) + 0.6f *  (pFromUnitInfo->iMr + pFromUnitInfo->iAddMr)) - (3.f * (pToUnitInfo->iMr + pToUnitInfo->iAddMr));
				//	break;
				//}


				// ���� ũ���� : ��ų ũ�� + [ ��� ũ�� * { (�ڽ��� AGI * 2) / (�ڽ��� AGI + ����� AGI) } ]
				if (nullptr == m_pTarget || (nullptr != m_pTarget && m_pTarget->GetDead())
					|| OBJ_ITEM == m_pTarget->GetObjID() || OBJ_EVENT == m_pTarget->GetObjID() || OBJ_EFFECT == m_pTarget->GetObjID())
					return;

				m_pSoundMgr->PlaySound(L"BeAtt.wav"/*L"BeAtt.wav"*/, CHANNEL_ID::CHANNEL_HIT);
				//m_pSoundMgr->PlaySound(L"Block.MP3", CHANNEL_ID::CHANNEL_HIT);
				//m_pSoundMgr->PlaySound(L"BossColl.wav"/*L"BeAtt.wav"*/, CHANNEL_ID::CHANNEL_HIT);
				dynamic_cast<CEventObject*>(m_pTarget)->InsertDamage(new DAMAGE_DATE(int(m_iDamage), 1, m_dwDebuff, pFrom));
			}
		}
	}
}

void CAttackEffect::SetDamageTiming(int iTiming)
{
	if (0 <= iTiming)
	{
		m_iDamageTiming = (iTiming >= (m_pTextureMgr->GetTexCount(m_wstrObjKey, m_wstrStateKey) - 1)) ? (m_pTextureMgr->GetTexCount(m_wstrObjKey, m_wstrStateKey) - 1) : iTiming;
	}
	else
	{
		m_iDamageTiming = (m_pTextureMgr->GetTexCount(m_wstrObjKey, m_wstrStateKey) - 1) / (-iTiming);
	}
}
