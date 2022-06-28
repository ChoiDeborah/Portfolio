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

// 사용자 정의
protected:
	list<DAMAGE_DATE*>			m_lstDamageQue;	// 데미지 큐

	list<D3DXVECTOR3>			m_lstBest;		// 에이스타 경로

	UNIT_INFO					m_tUnitData;	// 유닛 정보
	D3DXVECTOR3					m_vEndPos;		// 마지막 위치

	CAlarm						m_tAttackAlarm;	// 공격 딜레이 알람
	CAlarm						m_tHitAlarm;	// 피격 딜레이 알람
	CAlarm						m_tDebuffAlarm;	// 디버프 알람
	CAlarm						m_tDebuffDamage;// 디버프 데미지 알람
	CAlarm						m_tEffectDamage;// 광역 이펙트 데미지 알람

	CAlarm						m_tRevoceryAlarm;	// 자동 회복 알람

	CAlarm						m_tChatAlarm;		// 채팅 딜레이

	FRAME						m_tObjectTarget;

	wstring						m_wstrChat	= L"";

// 포인터
protected:
	CTileMgr*					m_pTileMgr	= CTileMgr::GetInstance();
	CMouseMgr*					m_pMouseMgr = CMouseMgr::GetInstance();
	CWordMgr*					m_pWordMgr	= CWordMgr::GetInstance();
	CUIMgr*						m_pUIMgr	= CUIMgr::GetInstance();

	const vector<TILE_INFO*>*	m_pVecTile	= nullptr;

	CCharacter*					m_pCharacter = nullptr;

// 기본 자료형
protected:
	DWORD	m_dwDebuff			= 0;		// 디버프

	float	m_fSpeed			= 0.f;		// 기준 스피드

	bool	m_bIsAttack			= false;	// 공격 확인
	bool	m_bIsMove			= false;	// 이동 확인
	bool	m_bIsHit			= false;	// 피격 확인
	bool	m_bIsChat			= false;	// 채팅 확인
};

