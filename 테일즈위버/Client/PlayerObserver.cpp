#include "stdafx.h"
#include "PlayerObserver.h"


CPlayerObserver::CPlayerObserver()
{
}


CPlayerObserver::~CPlayerObserver()
{
}

UNIT_INFO& CPlayerObserver::GetPlayerUnitData()
{
	return m_tPlayerData;
}

IDENTITY_DATA& CPlayerObserver::GetPlayerIdentity()
{
	return m_tPlayerIdentity;
}

INFO& CPlayerObserver::GetPlayerInfo()
{
	return m_tPlayerInfo;
}

void CPlayerObserver::Update(int iMessage)
{
	// Pull Observer ¹æ½Ä
	void* pData = CPlayerSubject::GetInstance()->GetData(iMessage);
	NULL_CHECK(pData);

	switch (iMessage)
	{
	case CPlayerSubject::PLAYER_UNIT_DATA:
		m_tPlayerData = *reinterpret_cast<UNIT_INFO*>(pData);
		break;

	case CPlayerSubject::PLAYER_IDENTITY:
		m_tPlayerIdentity = *reinterpret_cast<IDENTITY_DATA*>(pData);
		break;

	case CPlayerSubject::PLAYER_INFO:
		m_tPlayerInfo = *reinterpret_cast<INFO*>(pData);
		break;
	}
}

CPlayerObserver* CPlayerObserver::Create()
{
	return new CPlayerObserver;
}
