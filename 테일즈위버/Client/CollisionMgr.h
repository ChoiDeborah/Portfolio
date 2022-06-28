#pragma once

class CGameObject;
class CCollisionMgr
{
public:
	CCollisionMgr();
	~CCollisionMgr();

public:
	static bool CheckOneToN(const OBB& dst, const OBJLIST& srcLst, CGameObject** pObject = nullptr);
	static bool CheckOneToOne(const OBB& tDst, const OBB& tSrc, CGameObject** pObject = nullptr);
};

