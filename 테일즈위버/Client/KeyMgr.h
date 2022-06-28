#pragma once

#define MAX_KEY 0xff

class CKeyMgr
{
	DECLARE_SINGLETON(CKeyMgr)

private:
	CKeyMgr();
	~CKeyMgr();

public:
	bool KeyPressing(int iKey);
	bool KeyDown(int iKey);
	bool KeyUp(int iKey);

public:
	void Update();

private:
	bool m_bKey[MAX_KEY] = {};
};