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
	vector<EVENT_NOTIFY*>	m_vecNotify;	// �̺�Ʈ�� (�ð������� �־�� ��)
	FRAME					m_tFrame;		// �ִϸ��̼�
	EVENT_NOTIFY			m_tCurState;	// ���� ����
	D3DXVECTOR3				m_vDir;			// ����

	wstring					m_wstrName;		// ������Ʈ �̸�

private:
	CTimeMgr*				m_pTimeMgr		= nullptr;
	CTextureMgr*			m_pTextureMgr	= nullptr;

	EVENT_NOTIFY*			m_pNextNotify	= nullptr;

private:
	size_t					m_iCurCount		= 0;	// ���� ����ϰ� �ִ� �̺�Ʈ ��ȣ
	size_t					m_iNextCount	= 0;	// ���� �̺�Ʈ ��ȣ

	int						m_iViewObj	= false;
};

