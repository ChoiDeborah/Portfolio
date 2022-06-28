#pragma once

class CGameObject;

template<typename T>
class CAbstractFactory
{
public:
	static CGameObject* CreateObj()
	{
		CGameObject* pObj = new T;
		pObj->Initialize();

		return pObj;
	}

	static CGameObject* CreateObj(CGameObject* pTarget)
	{
		CGameObject* pObj = new T;
		pObj->Initialize();
		dynamic_cast<CMovableObj*>(pObj)->SetTarget(pTarget);

		return pObj;
	}

	static CGameObject* CreateObj(D3DXVECTOR3 vPos)
	{
		CGameObject* pObj = new T;
		pObj->Initialize();
		pObj->SetPos(vPos);

		return pObj;
	}

	static CGameObject* CreateObj(INFO tInfo)
	{
		CGameObject* pObj = new T;
		pObj->Initialize();
		pObj->SetInfo(tInfo);

		return pObj;
	}

	static CGameObject* CreateObj(const TILE_INFO& tTileInfo, INFO tInfo)
	{
		CGameObject* pObj = new T;

		pObj->SetName(tTileInfo.wstrName);

		pObj->Initialize();
		pObj->SetObjID(OBJID(tTileInfo.byOption));
		pObj->SetInfo(tInfo);

		return pObj;
	}
};