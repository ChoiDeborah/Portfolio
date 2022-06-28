#include "stdafx.h"
#include "PlayerSubject.h"

IMPLEMENT_SINGLETON(CPlayerSubject)

CPlayerSubject::CPlayerSubject()
{
}


CPlayerSubject::~CPlayerSubject()
{
	m_mapData.clear();
}

void* CPlayerSubject::GetData(int iMessage)
{
	auto iter_find = m_mapData.find(iMessage);

	if (m_mapData.end() == iter_find)
		return nullptr;

	return iter_find->second;
}

void CPlayerSubject::AddData(int iMessage, void* pData)
{
	NULL_CHECK(pData);

	auto iter_find = m_mapData.find(iMessage);

	if (m_mapData.end() != iter_find)
	{
		iter_find->second = pData;
		return;
	}

	m_mapData.insert({ iMessage, pData });

	CSubject::Notify(iMessage);
}
