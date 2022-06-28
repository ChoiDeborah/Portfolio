#pragma once
#include "AttackEffect.h"
class CDamageEffect :
	public CAttackEffect
{
public:
	CDamageEffect();
	virtual ~CDamageEffect();

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

private:
	void	Pattern();

public:
	void	SetDamageType(int iType) { m_iDamageType = iType; }

private:
	vector<const TEX_INFO*>	m_vecTexInfo;

private:
	float	m_fJumMaxGravity	= -8.f;
	float	m_fJumPower			= 8.f;
	float	m_fJumSpeed			= 0.8f;
	float	m_fJumAccPower		= 0.2f;
	float	m_fJumAcc			= 0.0f;

	int		m_iCount			= 0;

	int		m_iDamageType		= 0;
	int		m_iPattern			= 0;

	bool	m_bIsJump			= false;
};

