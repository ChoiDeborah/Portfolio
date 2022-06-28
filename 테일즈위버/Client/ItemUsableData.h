#pragma once
#include "ItemData.h"
class CItemUsableData :
	public CItemData
{
public:
	CItemUsableData();
	CItemUsableData(ICON_TYPE eIconType, ITEM_TYPE eType, ITEMID eID, const wstring& wstrObjKey, const wstring& wstrStateKey, float fColltime = 0.f, int iCount = 1, int iWeight = 0, DWORD dwColor = D3DCOLOR_ARGB(255, 255, 255, 255))
		: CItemData(eIconType, eType, eID, wstrObjKey, wstrStateKey, fColltime, iCount, iWeight, dwColor) {}
	virtual ~CItemUsableData();

public:
	virtual void	UseEvent() override;

public:
	CObjMgr*	m_pObjMgr	= CObjMgr::GetInstance();
	CUIMgr*		m_pUIMgr	= CUIMgr::GetInstance();

public:
	int		iMaxHp = 0;			// 전체 체력
	int		iMaxMp = 0;			// 전체 마나
	int		iMaxSp = 0;			// 전체 스테미너
};

