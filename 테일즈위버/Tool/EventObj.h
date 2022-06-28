#pragma once
class CEventObj
{
public:
	CEventObj(EVENT_NOTIFY* pNotify);
	~CEventObj();

public:
	void InsertNotify(EVENT_NOTIFY* pNotify);

	void ResetNotify();
	void UpdateNotify(float fTime);

	void FindNextNotify(float fTime);

	void Release();

public:
	void SetName(wstring wstrName) { m_wstrName = wstrName; }
	void SetViewObj(int iIsViewObj) { m_iViewObj = iIsViewObj; }

public:
	wstring GetName()		{ return m_wstrName; }
	int		GetViewObj()	{ return m_iViewObj; }

public:
	vector<EVENT_NOTIFY*>*	GetVecNotify()	{ return &m_vecNotify; }
	EVENT_NOTIFY*			GetCurState()	{ return &m_tCurState; }
	int						GetCurFrame()	{ return m_tFrame.iFrameCur; }

private:
	vector<EVENT_NOTIFY*>	m_vecNotify;	// 이벤트들 (시간순으로 넣어야 함)
	FRAME					m_tFrame;		// 애니메이션
	EVENT_NOTIFY			m_tCurState;	// 현재 상태
	D3DXVECTOR3				m_vDir;			// 방향

	wstring					m_wstrName;		// 오브젝트 이름

private:
	CTimeMgr*				m_pTimeMgr		= nullptr;
	CTextureMgr*			m_pTextureMgr	= nullptr;

	EVENT_NOTIFY*			m_pNextNotify	= nullptr;

private:
	size_t					m_iCurCount		= 0;	// 현재 사용하고 있는 이벤트 번호
	size_t					m_iNextCount	= 0;	// 다음 이벤트 번호

	int						m_iViewObj	= false;
};

