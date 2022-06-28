#pragma once
#include "Effect.h"
class CEventObject;
class CAttackEffect :
	public CEffect
{
public:
	CAttackEffect();
	virtual ~CAttackEffect();

public:
	// CEffect을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual int Update() override;
	virtual int LateUpdate() override;
	virtual void Render() override;

protected:
	virtual HRESULT LateInit() override;
	virtual void Release() override;
	virtual void FrameOnceEnd() override;
	virtual void FrameEvent() override;

public:
	void		SetAttackCount(int iCount)			{ m_iAttackCount = iCount;		}
	void		SetDamageTiming(int iTiming);
	void		SetFromObject(CGameObject* pFrom)	{ m_pFromObj = pFrom;			}
	void		SetDamage(int iDamage)				{ m_iDamage = iDamage;			}
	void		SetDebuff(DWORD dwDebuff)			{ m_dwDebuff = dwDebuff;		}
	void		SetColor(DWORD dwColor)				{ m_dwColor = dwColor;			}

public:
	DWORD		GetDebuff()							{ return m_dwDebuff;			}
	int			GetDamage()							{ return m_iDamage;				}
	int			GetAttackCount()					{ return m_iAttackCount;		}
	CGameObject* GetFromObject()					{ return m_pFromObj;			}

protected:
	CGameObject*	m_pFromObj	= nullptr;

protected:
	DWORD		m_dwColor		= D3DCOLOR_ARGB(255, 255, 255, 255);
	DWORD		m_dwDebuff		= 0;
	int			m_iDamage		= 0;

	int			m_iAttackCount	= 1;

	int			m_iDamageTiming = 1;
};

