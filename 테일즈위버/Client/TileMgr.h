#pragma once
class CTileMgr
{
	DECLARE_SINGLETON(CTileMgr)

private:
	CTileMgr();
	~CTileMgr();

public:
	void		Initialize(int iTileX, int iTileY, int iMapNum);
	void		Render();

public:
	void		Release();

public:
	HRESULT		InsertTile(TILE_INFO* pTile);
	void		AStarStartPos(list<D3DXVECTOR3>* pListBest, const D3DXVECTOR3& vStartPos, const D3DXVECTOR3& vGoalPos);

private:
	void		ResetAstar();
	void		MakePath(list<D3DXVECTOR3>* pListBest, const D3DXVECTOR3 & vStartPos, const D3DXVECTOR3 & vGoalPos);

private:
	ASTAR_NODE*	MakeNode(int iIndex, ASTAR_NODE* pParent);
	bool		IsPicking(const D3DXVECTOR3 & vPos, size_t iIndex);
	bool		CheckIndex(const int& iIndex);
	void		CheckOption(const int& iIndex, ASTAR_NODE* pParent);
	

public:
	const vector<TILE_INFO*>*	GetTileVec() { return &m_vecTile; }

	int			GetTileIndex(const D3DXVECTOR3& vPos);
	int			GetMapNum() { return m_iMapNum; }

// 사용자 정의형
private:
	vector<TILE_INFO*>	m_vecTile;	// 타일 벡터

	list<ASTAR_NODE*>	m_lstOpen;
	list<ASTAR_NODE*>	m_lstClose;

// 포인터
private:
	CDeviceMgr*		m_pDeviceMgr	= nullptr;
	CTextureMgr*	m_pTextureMgr = nullptr;

	const TEX_INFO*	m_pBackInfo		= nullptr;

// 기본 자료형
private:
	int		m_iMapNum = 0;

	int		m_iTileX				= 0;
	int		m_iTileY				= 0;

	int		m_iStart				= 0;
	int		m_iGoal					= 0;
};

