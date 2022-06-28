#pragma once
class CScene
{
public:
	CScene();
	virtual ~CScene();

public:
	virtual HRESULT Initialize() PURE;
	virtual void Update() PURE;
	virtual void LateUpdate() PURE;
	virtual void Render() PURE;

protected:
	virtual void Release();

protected:
	HRESULT LoadMap(const wstring& wstrFileName);

	void CreateObj(const TILE_INFO& tTileInfo, INFO& tInfo);


// 사용자정의
protected:
	vector<TILE_INFO*>		m_vecObj;
	
	wstring					m_wstrObjKey	= L"BackGround";
	wstring					m_wstrStateKey	= L"Map";

// 포인터
protected:
	CDeviceMgr*				m_pDeviceMgr	= CDeviceMgr::GetInstance();
	CTextureMgr*			m_pTextureMgr	= CTextureMgr::GetInstance();
	CKeyMgr*				m_pKeyMgr		= CKeyMgr::GetInstance();
	CObjMgr*				m_pObjMgr		= CObjMgr::GetInstance();
	CViewMgr*				m_pViewMgr		= CViewMgr::GetInstance();
	CTileMgr*				m_pTileMgr		= CTileMgr::GetInstance();
	CSceneMgr*				m_pSceneMgr		= CSceneMgr::GetInstance();
	CWordMgr*				m_pWordMgr		= CWordMgr::GetInstance();
	CSoundMgr*				m_pSoundMgr		= CSoundMgr::GetInstance();

	const TEX_INFO*			m_pTexInfo		= nullptr;
};

