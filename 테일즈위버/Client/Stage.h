#pragma once
#include "Scene.h"

class CGameObject;
class CStage :
	public CScene
{
public:
	CStage();
	virtual ~CStage();

public:
	// CScene을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual void Release() override;


// 포인터
private:
	CUIMgr*			m_pUIMgr	= CUIMgr::GetInstance();
	CGameObject*	m_pPlayer	= nullptr;
};

