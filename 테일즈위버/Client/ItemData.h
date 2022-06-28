#pragma once
#include "IconData.h"

class CItemData
	: public CIconData
{
public:
	CItemData();
	CItemData(ICON_TYPE eIconType, ITEM_TYPE eType, ITEMID eID, const wstring& wstrObjKey, const wstring& wstrStateKey, float fColltime = 0.f, int iCount = 1, int iWeight = 0, DWORD dwColor = D3DCOLOR_ARGB(255, 255, 255, 255))
		: CIconData(eIconType, wstrObjKey, wstrStateKey, fColltime), eItemType(eType), eItemID(eID), iCount(iCount), iWeight(iWeight), dwColor(dwColor) {}
	virtual ~CItemData();


public:
	virtual wstring	GetOption() { return to_wstring(iCount); }
	virtual void	UseEvent() override {}

public:
	void SetPrice(int iPrice) { this->iPrice = iPrice; }

public:
	int	GetPrice() { return iPrice; }

public:

	ITEM_TYPE		eItemType = ITEM_TYPE::ITEM_ETC;
	ITEMID			eItemID = ITEMID::ITEMID_ETC;

	int				iPrice = 0;
	unsigned int	iWeight = 0;
	
	unsigned int	iCount = 1;

	DWORD			dwColor = D3DCOLOR_ARGB(255, 255, 255, 255);
};

