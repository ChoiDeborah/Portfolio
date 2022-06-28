#pragma once

class CViewMgr;
class CTileMgr;
class CWordMgr;
class CScene;
class CSceneMgr
{
	DECLARE_SINGLETON(CSceneMgr)

private:
	CSceneMgr();
	~CSceneMgr();

public:
	void Update();
	void LateUpdate();
	void Render();

private:
	void Release();

public:
	void SetPlayerStartPos(const D3DXVECTOR3& vPos) { m_vPlayerStartPos = vPos; }
	void SetMapName(wstring wstrMap)				{ m_wstrCurMap = wstrMap; }
	void SetSceneLoadFailed()						{ m_bIsLoadFailed = true; }

	void SetNextScene(SCENEID eID, wstring wstrMap = L"");
	void ChangeScene(SCENEID eID);

	void SetEvent(int iEvent, bool bIsOnOff);
	void SetQuest(int iQuest, bool bIsOnOff);
	void SetIsEventScene(int iEvent, bool bIsOnOff);

	
public:
	void DrawFadeBox();

public:
	const bool			GetEvent(int iEvent) const			{ return (0 != (m_iEvent & iEvent));		}
	const bool			GetQuest(int iQuest) const			{ return (0 != (m_iQuest & iQuest));		}
	const bool			GetIsEventScene(int iEvent) const	{ return (0 != (m_iEventScene & iEvent));	}

	const SCENEID&		GetPreScene() const					{ return m_ePreScene;			}
	const SCENEID&		GetCurScene() const					{ return m_eCurScene;			}
	const wstring&		GetMapName() const					{ return m_wstrCurMap;			}
	const wstring&		GetEventName() const				{ return m_wstrEventName;		}

	const D3DXVECTOR3&	GetPlayerStartPos() const			{ return m_vPlayerStartPos;		}
	const SCENE_STATE	GetSceneState() const				{ return m_eSceneState;			}

	int					GetStageNum()						{ return m_iStageNum;			}
	int					GetPreStageNum()					{ return m_iPreStageNum;		}
	int					GetJellyCount()						{ return m_iJellyCount;			}

public:
	void SetStageNum(int iNum)		{ m_iPreStageNum = m_iStageNum;  m_iStageNum = iNum;	}
	void SetJellyCount(int iCount)	{ m_iJellyCount = iCount;								}


// 사용자 정의
private:
	D3DXVECTOR3		m_vPlayerStartPos	= { SCENE_LOAD_POS, SCENE_LOAD_POS, 0.f };

	wstring			m_wstrCurMap		= L"Map0";
	wstring			m_wstrPrevMap		= L"Map0";

	wstring			m_wstrEventName		= L"Event0";

	SCENE_STATE		m_eSceneState		= SCENE_STATE::START_SCENE;
	SCENEID			m_eCurScene			= SCENEID::SCENEID_END;
	SCENEID			m_ePreScene			= SCENEID::SCENEID_END;
	SCENEID			m_eNextScene		= SCENEID::SCENEID_END;

// 포인터
private:
	CDeviceMgr*		m_pDeviceMgr		= nullptr;
	CTextureMgr*	m_pTextureMgr		= nullptr;
	CKeyMgr*		m_pKeyMgr			= nullptr;
	CMouseMgr*		m_pMouseMgr			= nullptr;
	CTileMgr*		m_pTileMgr			= nullptr;
	CViewMgr*		m_pViewMgr			= nullptr;
	CWordMgr*		m_pWordMgr			= nullptr;

	CScene*			m_pScene			= nullptr;

	const TEX_INFO*	m_pFadeTex			= nullptr;


public:
	int				m_iMaxCount = 0;
	int				m_iCount = 0;

// 기본 자료형
private:
	float			m_fFadeAlpha		= 255.f; // Fade Alpha Value

	int				m_iJellyCount		= 0;

	int				m_iMaxStageNum		= 8;
	int				m_iPreStageNum		= 0;
	int				m_iStageNum			= 0;

	int				m_iPause			= 0;

	int				m_iQuest			= 0;	// Quest Valuse
	int				m_iEvent			= 0;	// Event Value (Use Flag)
	int				m_iEventScene		= 0;	// Event Scene Value;

	bool			m_bIsLoadFailed		= false;
};

