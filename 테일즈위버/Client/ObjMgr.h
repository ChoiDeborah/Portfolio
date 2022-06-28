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
	void ReleaseObj(OBJID eID); // ���� �Ѿ� ���� ��� ���� ���� ������Ʈ�� ����


public:
	OBJLIST&			GetObjList(OBJID eID) { return m_ObjList[eID]; }
	const UNIT_INFO*	GetUnitData(const wstring& wstrName);
	CItemData*			GetItemData(const wstring& wstrName);
	CGameObject*		GetTarget(CGameObject* pObj, OBJID eID);
	CGameObject*		GetPlayer();
	
	
	bool				InstanceExists(OBJID eID);

	HRESULT				LoadItemData(const wstring wstrFilePath);
	HRESULT				LoadUnitData(const wstring wstrFilePath);	


// ����� ����
private:
	map<wstring, UNIT_INFO*>	m_mapUnitData;				// ���� ������
	map<wstring, CItemData*>	m_mapItemData;				// ������ ������

	vector<CGameObject*>		m_vecRender[LAYER_END];
	OBJLIST						m_ObjList[OBJ_END];

// ������
private:
	CSceneMgr*					m_pSceneMgr = nullptr;
	CViewMgr*					m_pViewMgr	= nullptr;

// �⺻ �ڷ���
	int							m_iEvent	= NO_EVENT;
};

