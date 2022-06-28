#pragma once
#include "Subject.h"
class CPlayerSubject :
	public CSubject
{
	DECLARE_SINGLETON(CPlayerSubject)

public:
	enum DATA_TYPE { PLAYER_UNIT_DATA, PLAYER_IDENTITY, PLAYER_INFO };

private:
	CPlayerSubject();
	virtual ~CPlayerSubject();

public:
	void* GetData(int iMessage);

public:
	void AddData(int iMessage, void* pData);

private:
	map<int, void*>	m_mapData;
};

