#pragma once

class CObserver;
class CSubject
{
public:
	CSubject();
	virtual ~CSubject();

public:
	void Subscribe(CObserver* pObserver);
	void UnSubscribe(CObserver* pObserver);
	void Notify(int iMessage);

protected:
	list<CObserver*>	m_ObserverLst;
};

