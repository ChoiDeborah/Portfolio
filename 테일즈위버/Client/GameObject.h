#pragma once
#include "Alarm.h"

class CGameObject
{
public:
	enum STATE {
		ATTACK, ATTACK1, ATTACK2, ATTACK3, 
		SKILL, DEAD,
		HIT, IDLE, WALK, RUN, SIT,
		EFFECT,
		END
	};

public:
	CGameObject();
	virtual ~CGameObject();

public:
	virtual HRESULT Initialize()	PURE;
	virtual int		Update()		PURE;
	virtual int		LateUpdate()	PURE;
	virtual void	Render()		PURE;
	virtual void	MiniRender(D3DXVECTOR3 vPivot, float fPer);

protected:
	virtual HRESULT LateInit();
	virtual void	Release()		PURE;
	
	virtual void	FrameOnceEnd()	PURE;
	virtual void	FrameEvent()	PURE;

public:
	HRESULT			SetFrameKey(const wstring & wstrObjKey, const wstring & wstrStateKey, STATE eCurState, DIRECTION eDir, FRAMESTATE eFrameState);
	void			DrawCollRect();

	static CGameObject*	CreateEffect(
		D3DXVECTOR3 vPos,						// ��ġ
		D3DXVECTOR3 vOffset,					// ��ġ ������
		wstring wstrObjKey,						// ������Ʈ Ű
		wstring wstrStateKey = L"",				// ���� Ű
		bool	bIsAttack = false,				// ���� ����Ʈ ����
		OBJID eID = OBJ_EFFECT,					// ������Ʈ ID
		CGameObject* pTarget = nullptr,			// Ÿ�� ������Ʈ
		CGameObject* pFrom = nullptr,			// From ������Ʈ
		FRAMESTATE eFrameState = FRAME_ONCE,	// ������ ����
		bool	bIsFollow = false				// ����
	);

	static CGameObject*	CreateDamageEffect(
		D3DXVECTOR3 vPos,
		int	iDamage = 0,
		int iType = 0,
		wstring wstrStateKey = L"HitNumber"
	);

	static CGameObject*	CreateItem(
		const wstring&	wstrName,
		D3DXVECTOR3		vStartPos,
		D3DXVECTOR3		vTargetPos,
		int				iCount, 
		int				iPrice = 0
	);

protected:
	void				FrameMove();
	void				InitCollRect();
	void				InitCollVertex();
	
	void				UpdateView();

	bool				IsOutPos();

public:
	wstring				AddWString_Dir_8(const wstring& wstrText, DIRECTION eDir);
	wstring				AddWString_Dir_4(const wstring& wstrText, DIRECTION eDir);
	wstring				AddWString_Dir_4_Cross(const wstring& wstrText, DIRECTION eDir);
	wstring				AddWString_Dir_2(const wstring& wstrText, DIRECTION eDir);
	DIRECTION			ChangeDirToAngle(float fRadian);

	float				GetRadianAngle(D3DXVECTOR3& vTarget, D3DXVECTOR3& vStart);
	float				GetRadianAngle(D3DXVECTOR3& vDir);


public:
	const bool			GetDead()		const { return m_bIsDead;			}
	const int			GetDepth()		const { return m_iDepth;			}
	const INFO&			GetInfo()		const { return m_tInfo;				}
	const OBB&			GetOBB()		const { return m_tOBB;				}
	const RENDER_LAYER	GetLayer()		const { return m_eLayer;			}
	const DIRECTION		GetDirection()	const { return m_eDirection;		}
	const OBJID			GetObjID()		const { return m_OBJID;				}
	const bool			GetActive()		const { return m_bIsActive;			}
	const float			GetAngle()		const { return m_fAngle;			}
	const STATE			GetCurState()	const { return m_eCurState;			}
	const FRAME&		GetFrame()		const { return m_tFrame;			}
	CGameObject*		GetTarget()		const { return m_pTarget;			}


	bool				GetIsMouseTarget()	{ return m_bIsMouseTarget;		}

public:
	void	SetName(const wstring& wstrName){ m_wstrName = wstrName;		}
	void	SetPos		(D3DXVECTOR3& vPos)	{ m_tInfo.vPos = vPos;			}
	void	SetDirection(DIRECTION dir)		{ m_eDirection = dir;			}
	void	SetDepth	(int iDepth)		{ m_iDepth = iDepth;			}
	void	SetActive	(bool bIsActive)	{ m_bIsActive = bIsActive;		}
	void	SetObjID	(OBJID eID)			{ m_OBJID = eID;				}
	void	SetInfo		(INFO& tInfo)		{ m_tInfo = tInfo;				}
	void	SetMouseTarget(bool bIsTarget)	{ m_bIsMouseTarget = bIsTarget; }

	void	SetDead()						{ m_bIsDead = true;				}
	void	SetTarget(CGameObject* pTarget) { m_pTarget = pTarget;			}
	void	SetLayer(RENDER_LAYER eLayer)	{ m_eLayer = eLayer;			}
	void	SetCurState(STATE eState)		{ m_eCurState = eState;			}


// ����� ���� �ڷ���
protected:
	FRAME			m_tFrame;					// ��������Ʈ ������

	INFO			m_tInfo;					// ��ġ, ũ��
	OBB				m_tOBB;						// �浹 �ڽ�

	wstring			m_wstrName;					// ������Ʈ �̸�
	wstring			m_wstrObjKey;				// ������Ʈ �̹��� Ű
	wstring			m_wstrStateKey;				// ���� �̹��� Ű

// enum ��
protected:
	RENDER_LAYER	m_eLayer;					// ���� ���̾�
	OBJID			m_OBJID;					// ������Ʈ �̸�
	DIRECTION		m_eDirection = NO_DIRECTION;// �ٶ󺸴� ����


	FRAMESTATE		m_eFrameState = FRAME_LOOP;	// ������ ����

	STATE			m_eCurState = END;
	STATE			m_ePreState = END;

//  ������
protected:
	CDeviceMgr*		m_pDeviceMgr	= CDeviceMgr::GetInstance();
	CTextureMgr*	m_pTextureMgr	= CTextureMgr::GetInstance();
	CViewMgr*		m_pViewMgr		= CViewMgr::GetInstance();
	CTimeMgr*		m_pTimeMgr		= CTimeMgr::GetInstance();
	CSceneMgr*		m_pSceneMgr		= CSceneMgr::GetInstance();
	CObjMgr*		m_pObjMgr		= CObjMgr::GetInstance();
	CSoundMgr*		m_pSoundMgr		= CSoundMgr::GetInstance();

	const TEX_INFO* m_pTexInfo		= nullptr;
	CGameObject*	m_pTarget		= nullptr;


// �⺻ �ڷ���
protected:
	float	m_fAngle		= 0.f;		// ����

	int		m_iDepth;					// ����
	float	m_fAlpha		= 1.f;		// ���� ��

	float	m_fViewX		= CViewMgr::GetInstance()->GetViewX();
	float	m_fViewY		= CViewMgr::GetInstance()->GetViewY();

	bool	m_bIsDead		= false;	// ���� Ȯ��
	bool	m_bIsActive		= true;		// ������Ʈ On / Off

	bool	m_bIsInit		= false;	// LateInit Ȯ��
	bool	m_bIsMouseTarget = false;
};

