#pragma once
#include "UIWnd.h"

#define SLOT_SIZE 8

class CEquipWnd :
	public CUIWnd
{
public:
	CEquipWnd();
	virtual ~CEquipWnd();

public:
	// Inherited via CUIWnd
	virtual HRESULT Initialize() override;
	virtual void	Update() override;
	virtual void	Render() override;
	virtual void	Release() override;

	virtual void	LClickDown()	override;
	virtual void	LClickUp()		override;

	virtual void	RClickDown()	override;

public:
	CItemData*	GetItem(int iIndex);
	void		SetItem(int iIndex, CItemData* pItemData);
	void		DeleteItem(int iIndex);

	void		AddStat(STAT_INFO* pStatInfo);

public:
	static CEquipWnd* Create();

private:
	CItemData*		m_pItem[SLOT_SIZE];

	CWndInfo		m_tItem[SLOT_SIZE];
};

