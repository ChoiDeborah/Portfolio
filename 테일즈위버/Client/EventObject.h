#pragma once
#include "GameObject.h"
class CCharacter;
class CEventObject :
	public CGameObject
{


public:
	CEventObject();
	virtual ~CEventObject();

public:
	virtual HRESULT Initialize()	PURE;
	virtual int		Update()		PURE;
	virtual int		LateUpdate()	PURE;
	virtual void	Render()		PURE;

protected:
	virtual void	Release()		PURE;
	virtual HRESULT LateInit()		PURE;
	virtual void	FrameOnceEnd()	PURE;
	virtual void	FrameEvent()	PURE;

protected:
	virtual void	Attack()				PURE;
	virtual void	Hit()					PURE;

	virtual void	AtarMoveEnd()			PURE;
	virtual void	AstarMoveSetState()		PURE;
	virtual bool	AstartExitEvent()		PURE;

public:
	void	DrawHpBar();
	void	DrawHpBar2();
	void	DrawNameBar();
	void	DrawPosition();
	void	DrawChatting();

protected:
	void	Recovery();
	void	AStarMove();	
	void	DebuffUpdate();

public:
	template <typename T>
	T* CreateObj(OBJID eID, D3DXVECTOR3 vPos)
	{
		CGameObject* add = CAbstractFactory<T>::CreateObj(this);
		T* obj = (T*)add;
		CObjMgr::GetInstance()->AddObject(add, eID);
		obj->SetPos(vPos);

		return obj;
	}

public:
	void		CreateCharacter(const wstring& wstrName);
	void		SetCharacter(CCharacter* pCharacter);
	void		InsertDamage(DAMAGE_DATE* pDamageData) { m_lstDamageQue.push_back(pDamageData); }
	UNIT_INFO&	GetUnitData() { return m_tUnitData; }

	void		SetChatting(const wstring& wstrChat) { m_bIsChat = true; m_tChatAlarm.SetAlarm(5.f, false);  m_wstrChat = wstrChat; }

// ����� ����
protected:
	list<DAMAGE_DATE*>			m_lstDamageQue;	// ������ ť

	list<D3DXVECTOR3>			m_lstBest;		// ���̽�Ÿ ���

	UNIT_INFO					m_tUnitData;	// ���� ����
	D3DXVECTOR3					m_vEndPos;		// ������ ��ġ

	CAlarm						m_tAttackAlarm;	// ���� ������ �˶�
	CAlarm						m_tHitAlarm;	// �ǰ� ������ �˶�
	CAlarm						m_tDebuffAlarm;	// ����� �˶�
	CAlarm						m_tDebuffDamage;// ����� ������ �˶�
	CAlarm						m_tEffectDamage;// ���� ����Ʈ ������ �˶�

	CAlarm						m_tRevoceryAlarm;	// �ڵ� ȸ�� �˶�

	CAlarm						m_tChatAlarm;		// ä�� ������

	FRAME						m_tObjectTarget;

	wstring						m_wstrChat	= L"";

// ������
protected:
	CTileMgr*					m_pTileMgr	= CTileMgr::GetInstance();
	CMouseMgr*					m_pMouseMgr = CMouseMgr::GetInstance();
	CWordMgr*					m_pWordMgr	= CWordMgr::GetInstance();
	CUIMgr*						m_pUIMgr	= CUIMgr::GetInstance();

	const vector<TILE_INFO*>*	m_pVecTile	= nullptr;

	CCharacter*					m_pCharacter = nullptr;

// �⺻ �ڷ���
protected:
	DWORD	m_dwDebuff			= 0;		// �����

	float	m_fSpeed			= 0.f;		// ���� ���ǵ�

	bool	m_bIsAttack			= false;	// ���� Ȯ��
	bool	m_bIsMove			= false;	// �̵� Ȯ��
	bool	m_bIsHit			= false;	// �ǰ� Ȯ��
	bool	m_bIsChat			= false;	// ä�� Ȯ��
};

