#include "stdafx.h"
#include "SkillData.h"

#include "Player.h"

CSkillData::CSkillData()
{
}


CSkillData::~CSkillData()
{
}

void CSkillData::UseEvent()
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObjMgr::GetInstance()->GetPlayer());
	if (nullptr != pPlayer)
	{
		pPlayer->UseSkill(m_eSkillID);
	}
}