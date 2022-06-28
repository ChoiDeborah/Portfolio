#include "stdafx.h"
#include "Subject.h"
#include "Observer.h"

CSubject::CSubject()
{
}


CSubject::~CSubject()
{
	m_ObserverLst.clear();
}

void CSubject::Subscribe(CObserver* pObserver)
{
	NULL_CHECK(pObserver);

	auto iter_find = find(m_ObserverLst.begin(), m_ObserverLst.end(), pObserver);

	if (m_ObserverLst.end() != iter_find)
		return;

	m_ObserverLst.push_back(pObserver);
}

void CSubject::UnSubscribe(CObserver* pObserver)
{
	NULL_CHECK(pObserver);

	auto iter_find = find(m_ObserverLst.begin(), m_ObserverLst.end(), pObserver);

	if (m_ObserverLst.end() == iter_find)
		return;

	m_ObserverLst.erase(iter_find);
}

void CSubject::Notify(int iMessage)
{
	for (auto& pObserver : m_ObserverLst)
		pObserver->Update(iMessage);
}
