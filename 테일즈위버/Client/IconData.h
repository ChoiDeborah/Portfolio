#pragma once
class CIconData
{
public:
	CIconData();
	CIconData(ICON_TYPE eType, const wstring& wstrObjKey, const wstring& wstrStateKey, float fColltime = 0.f)
		: eIcon(eType), wstrObjKey(wstrObjKey), wstrStateKey(wstrStateKey) 
	{
		this->fColltime = fColltime;
	}
	virtual ~CIconData();

public:
	virtual void	UseEvent() PURE;

public:
	wstring		wstrObjKey = L"";
	wstring		wstrStateKey = L"";

	float		fColltime = 0.f;

	ICON_TYPE	eIcon;
};

