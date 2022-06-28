#pragma once
#include "StaticObject.h"
class CPuzzleObject :
	public CStaticObject
{
public:
	CPuzzleObject();
	virtual ~CPuzzleObject();

public:
	virtual HRESULT Initialize() override;
	virtual int Update() override;
	virtual int LateUpdate() override;
	virtual void Render() override;

protected:
	virtual HRESULT LateInit();
	virtual void Release() override;
	virtual void FrameOnceEnd() override;
	virtual void FrameEvent() override;

public:
	int		GetWeight() { return m_iWeight; }

private:
	int		m_iWeight	= 0;
	bool	m_bIsWeight = false;
};

