#include "stdafx.h"
#include "Creator.h"

#include "Monster.h"

// Monster
#include "DarkCow.h"
#include "Fixy.h"

CCreator::CCreator()
{
}


CCreator::~CCreator()
{
}

HRESULT CCreator::Initialize()
{
	m_eLayer = LAYER_EFFECT;
	m_tCreateAlarm.SetAlarm(0.1f, false);
	return S_OK;
}

HRESULT CCreator::LateInit()
{
	return S_OK;
}

int CCreator::Update()
{
	CGameObject::LateInit();

	if (m_bIsSetting)
	{
		if (!m_bIsCreate)
		{
			if (nullptr == m_pTarget)
			{
				m_pTarget = CAbstractFactory<CMonster>::CreateObj(m_tTileInfo, m_tInfo);

				if (!lstrcmp(m_tTileInfo.wstrObjKey.c_str(), L"DarkCow"))
					dynamic_cast<CEventObject*>(m_pTarget)->SetCharacter(new CDarkCow);
				else if (!lstrcmp(m_tTileInfo.wstrObjKey.c_str(), L"Fixy"))
					dynamic_cast<CEventObject*>(m_pTarget)->SetCharacter(new CFixy);
				m_pObjMgr->AddObject(m_pTarget, OBJID::OBJ_MONSTER);

				CreateEffect(m_tInfo.vPos, D3DXVECTOR3{ 0.f, 0.f, 0.f }, L"Effect", L"Casting", false, OBJ_EFFECT, nullptr, nullptr, FRAME_ONCE);

				m_bIsCreate = true;
			}
		}
		else
		{
			if (nullptr == m_pTarget)
			{
				if (m_tCreateAlarm.CheckAlarm())
				{
					m_bIsCreate = false;
				}
			}
			else if (m_pTarget->GetDead())
			{
				m_pTarget = nullptr;
				m_tCreateAlarm.SetAlarm(2.f, false);
			}
		}
	}

	return NO_EVENT;
}

int CCreator::LateUpdate()
{
	
	return NO_EVENT;
}

void CCreator::Render()
{
}

void CCreator::Release()
{
}

void CCreator::FrameOnceEnd()
{
}

void CCreator::FrameEvent()
{
}

void CCreator::SetCreateData(const TILE_INFO & tTileInfo, INFO & tInfo)
{
	m_tTileInfo = tTileInfo;
	m_tInfo = tInfo;
	m_bIsSetting = true;
}
