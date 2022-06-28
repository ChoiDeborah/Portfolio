#pragma once
class CObjMgr;
class CSceneMgr;
class CKeyMgr;
class CUIMgr;
class CViewMgr;
class CItemData;
class CSkillData;
class CGameObject;
class CMouseMgr
{
	DECLARE_SINGLETON(CMouseMgr)

private:
	CMouseMgr();
	~CMouseMgr();

public:
	void Initialize();
	void Update();
	void Render();

private:
	void FrameMove();

public:
	void SetFrame		(const wstring& wstrObjKey, const wstring& wstrStateKey, MOUSE_TYPE eType);
	void SetItemData	(WND_ID eWnd, int iIndex, CItemData* pItemData);
	void SetSkillData	(WND_ID eWnd, int iIndex, CSkillData* pSkillData);


	void SetUIClick		(bool bIsUIClick)			{ m_bIsUIClick = bIsUIClick;			}
	void SetAttackGround(bool bIsAttackGround)		{ m_bIsAttackGround = bIsAttackGround;	}
	

public:
	CGameObject*		GetRayTarget()				{ return m_pRayTarget;		}
	MOUSE_TYPE			GetMouseType()				{ return m_eMouseType;		}
	
	bool				GetAttackGround()			{ return m_bIsAttackGround; }
	bool				GetIsUIClick()				{ return m_bIsUIClick;		}

	int					GetFromItemIndex()			{ return m_iItemSlotIndex;	}
	WND_ID				GetFromWndID()				{ return m_eWndID;			}
	CItemData*			GetItemData()				{ return m_pItemData;		}
	CSkillData*			GetSkillData()				{ return m_pSkillData;		}

public:
	static D3DXVECTOR3	GetMouse();
	

private:
	FRAME			m_tFrame;

	wstring			m_wstrObjKey;
	wstring			m_wstrStateKey;

private:
	CDeviceMgr*		m_pDeviceMgr		= nullptr;
	CTextureMgr*	m_pTextureMgr		= nullptr;
	CTimeMgr*		m_pTimeMgr			= nullptr;
	CKeyMgr*		m_pKeyMgr			= nullptr;
	CObjMgr*		m_pObjMgr			= nullptr;
	CSceneMgr*		m_pSceneMgr			= nullptr;
	CUIMgr*			m_pUIMgr			= nullptr;
	CViewMgr*		m_pViewMgr			= nullptr;
	CSoundMgr*		m_pSoundMgr			= nullptr;


	const TEX_INFO*	m_pTexInfo			= nullptr;

	CGameObject*	m_pRayTarget		= nullptr;

	CItemData*		m_pItemData			= nullptr;
	CSkillData*		m_pSkillData		= nullptr;

private:
	MOUSE_TYPE		m_eMouseType		= MOUSE_END;

	WND_ID			m_eWndID			= WNDID_END;	// 인벤 아이디
	int				m_iItemSlotIndex	= 0;			// 아이템 드롭 시

	bool			m_bIsUIClick		= false;
	bool			m_bIsAttackGround	= false;
};

