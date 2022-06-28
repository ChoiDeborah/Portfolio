#pragma once

class CToolView;
class CMainDialog;
class CTerrain
{
private:
	CTerrain();

public:
	virtual ~CTerrain();

public:
	int GetTileIndex(const D3DXVECTOR3& vPos);

public:
	void SetView(CToolView* pView);
	void SetMainDialog(CMainDialog* pMainDialog);

public:
	// CGameObject을(를) 통해 상속됨
	void Update();
	void Render();

private:
	HRESULT Initialize();
	void Release();

public:
	void TileChange(const D3DXVECTOR3& vPos, BYTE byDrawID, BYTE byOption = 0);
	
private:
	bool IsPicking(const D3DXVECTOR3& vPos, size_t iIndex);

	void SpriteToolUpdate();
	void UnitToolUpdate();
	void MapToolUpdate();
	void WordToolUpdate();
	void EventToolUpdate();

	void SpriteToolRender();
	void UnitToolRender();
	void MapToolRender();
	void WordToolRender();
	void EventToolRender();


public:
	static CTerrain* Create(CToolView* pView);

private:
	CDeviceMgr*			m_pDeviceMgr	= CDeviceMgr::GetInstance();
	CTextureMgr*		m_pTextureMgr	= CTextureMgr::GetInstance();
	CViewMgr*			m_pViewMgr		= CViewMgr::GetInstance();
	CKeyMgr*			m_pKeyMgr		= CKeyMgr::GetInstance();

	CToolView*			m_pView			= nullptr;
	CMainDialog*		m_pMainDialog	= nullptr;

private:
	int		m_iTileX	= 0;
	int		m_iTileY	= 0;

	bool	m_ObjectCollRender = false;

	bool	m_bIsInit	= false;
};

