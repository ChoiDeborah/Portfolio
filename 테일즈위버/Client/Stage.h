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
	// CScene��(��) ���� ��ӵ�
	virtual HRESULT Initialize() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual void Release() override;


// ������
private:
	CUIMgr*			m_pUIMgr	= CUIMgr::GetInstance();
	CGameObject*	m_pPlayer	= nullptr;
};

