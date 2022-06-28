#pragma once
#include "IconData.h"

class CSkillData
	: public CIconData
{
public:
	CSkillData();
	CSkillData(ICON_TYPE eIconType, SKILL_TYPE eType, SKILLID eID, const wstring& wstrObjKey, const wstring& wstrStateKey, float fColltime = 0.f)
		: CIconData(eIconType, wstrObjKey, wstrStateKey, fColltime), m_eSkillType(eType), m_eSkillID(eID) {}

	virtual ~CSkillData();


public:
	virtual void	UseEvent() override;

public:
	CObjMgr*		m_pObjMgr	= CObjMgr::GetInstance();
	CUIMgr*			m_pUIMgr	= CUIMgr::GetInstance();

	SKILL_TYPE		m_eSkillType;
	SKILLID			m_eSkillID;
};

