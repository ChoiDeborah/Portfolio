#include "stdafx.h"
#include "KeyMgr.h"

IMPLEMENT_SINGLETON(CKeyMgr)

CKeyMgr::CKeyMgr()
{
}

CKeyMgr::~CKeyMgr()
{
}

bool CKeyMgr::KeyPressing(int iKey)
{
	if (::GetAsyncKeyState(iKey) & 0x8000)
		return true;

	return false;
}

bool CKeyMgr::KeyDown(int iKey)
{
	if (!m_bKey[iKey] && ::GetAsyncKeyState(iKey) & 0x8000)
	{
		m_bKey[iKey] = !m_bKey[iKey];
		return true;
	}

	return false;
}

bool CKeyMgr::KeyUp(int iKey)
{
	if (m_bKey[iKey] && !(::GetAsyncKeyState(iKey) & 0x8000))
	{
		m_bKey[iKey] = !m_bKey[iKey];
		return true;
	}

	return false;
}

void CKeyMgr::Update()
{
	for (int i = 0; i < MAX_KEY; ++i)
	{
		if (!m_bKey[i] && ::GetAsyncKeyState(i) & 0x8000)
			m_bKey[i] = !m_bKey[i];

		if (m_bKey[i] && !(::GetAsyncKeyState(i) & 0x8000))
			m_bKey[i] = !m_bKey[i];
	}
}