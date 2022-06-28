#pragma once
#include "GameObject.h"

class CEffectImp;
class CEffect :
	public CGameObject
{
public:
	enum EFFECTTYPE { SAFE_EFFECT, ATTACK_EFFECT };

public:
	CEffect();
	virtual ~CEffect();

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize()	PURE;
	virtual int		Update()		PURE;
	virtual int		LateUpdate()	PURE;
	virtual void	Render()		PURE;

protected:
	virtual HRESULT LateInit()		PURE;
	virtual void	Release()		PURE;

protected:
	virtual void	FrameOnceEnd()	PURE;
	virtual void	FrameEvent()	PURE;

public:
	void			SetAngle(float fAngle)			{ m_fAngle = fAngle;		}
	void			SetFollow(bool bIsFollow)		{ m_bIsFollow = bIsFollow;	}
	void			SetOffset(D3DXVECTOR3 vOffset)	{ m_vOffset = vOffset;		}

protected:
	D3DXVECTOR3		m_vOffset = {0.f, 0.f, 0.f};
	EFFECTTYPE		m_eType;
	bool			m_bIsFollow = false;
};

