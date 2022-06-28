#pragma once
#include "Observer.h"
class CPlayerObserver :
	public CObserver
{
public:
	CPlayerObserver();
	virtual ~CPlayerObserver();

public:
	UNIT_INFO&		GetPlayerUnitData();
	IDENTITY_DATA&	GetPlayerIdentity();
	INFO&			GetPlayerInfo();
	
public:
	// CObserver을(를) 통해 상속됨
	virtual void Update(int iMessage) override;

public:
	static CPlayerObserver* Create();

private:
	UNIT_INFO		m_tPlayerData;
	IDENTITY_DATA	m_tPlayerIdentity;
	INFO			m_tPlayerInfo;
};

