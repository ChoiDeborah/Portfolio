#pragma once
#include "ItemData.h"
class CItemEquipData :
	public CItemData
{
public:
	CItemEquipData();
	CItemEquipData(ICON_TYPE eIconType, ITEM_TYPE eType, ITEMID eID, const wstring& wstrObjKey, const wstring& wstrStateKey, float fColltime = 0.f, int iCount = 1, int iWeight = 0, DWORD dwColor = D3DCOLOR_ARGB(255, 255, 255, 255))
		: CItemData(eIconType, eType, eID, wstrObjKey, wstrStateKey, fColltime, iCount, iWeight, dwColor) {}
	virtual ~CItemEquipData();

public:
	virtual wstring	GetOption() { return to_wstring(iDurability) + L"/" + to_wstring(iMaxDurability); }
	virtual void	UseEvent() override;

public:
	STAT_INFO	tStatInfo;		// 스탯 정보

public:
	int		iMaxHp = 0;			// 전체 체력
	int		iMaxMp = 0;			// 전체 마나
	int		iMaxSp = 0;			// 전체 스테미너

	unsigned int	iMaxDurability = 1;
	unsigned int	iDurability = 1;
};

