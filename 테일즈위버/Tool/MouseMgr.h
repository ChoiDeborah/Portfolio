#pragma once
class CMouseMgr
{
	DECLARE_SINGLETON(CMouseMgr)

private:
	CMouseMgr();
	~CMouseMgr();

public:
	void Render();

public:
	static D3DXVECTOR3 GetMouse();
};

