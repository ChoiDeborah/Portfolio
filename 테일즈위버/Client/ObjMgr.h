#pragma once
class CGameObject;
class CItemData;
class CObjMgr
{
	DECLARE_SINGLETON(CObjMgr)

private:
	CObjMgr();
	~CObjMgr();

public:
	void AddObject(CGameObject* pObj, OBJID eID);
	void Initialize();
	void Update();
	void LateUpdate();
	void Render();
	void MiniRender(D3DXVECTOR3 vPivot, float fPer);
	void Release();

	void ResetObjList();

public:
	void ReleaseObj(OBJID eID); // 씬을 넘어 갔을 경우 이전 씬의 오브젝트만 제거


public:
	OBJLIST&			GetObjList(OBJID eID) { return m_ObjList[eID]; }
	const UNIT_INFO*	GetUnitData(const wstring& wstrName);
	CItemData*			GetItemData(const wstring& wstrName);
	CGameObject*		GetTarget(CGameObject* pObj, OBJID eID);
	CGameObject*		GetPlayer();
	
	
	bool				InstanceExists(OBJID eID);

	HRESULT				LoadItemData(const wstring wstrFilePath);
	HRESULT				LoadUnitData(const wstring wstrFilePath);	


// 사용자 정의
private:
	map<wstring, UNIT_INFO*>	m_mapUnitData;				// 유닛 데이터
	map<wstring, CItemData*>	m_mapItemData;				// 아이템 데이터

	vector<CGameObject*>		m_vecRender[LAYER_END];
	OBJLIST						m_ObjList[OBJ_END];

// 포인터
private:
	CSceneMgr*					m_pSceneMgr = nullptr;
	CViewMgr*					m_pViewMgr	= nullptr;

// 기본 자료형
	int							m_iEvent	= NO_EVENT;
};

