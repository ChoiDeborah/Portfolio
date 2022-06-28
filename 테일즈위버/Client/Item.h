#pragma once
#include "GameObject.h"
#include "ItemData.h"

class CItem :
	public CGameObject
{
public:
	CItem();
	virtual ~CItem();

public:
	virtual HRESULT Initialize() override;
	virtual int		Update() override;
	virtual int		LateUpdate() override;
	virtual void	Render() override;

protected:
	virtual HRESULT LateInit();
	virtual void	Release() override;
	virtual void	FrameOnceEnd() override;
	virtual void	FrameEvent() override;

private:
	void	Move();

public:
	void SetItemData(CItemData* pItemData)	{ m_pItemData = pItemData;	}
	void SetItemState(int iState)			{ m_iState = iState;	}
	void SetTargetPos(D3DXVECTOR3& vPos)	{ m_vTargetPos = vPos;	}

public:
	CItemData*	GetItemType()				{ return m_pItemData;	}
	int			GetItemState()				{ return m_iState;		}
	
	void		DrawNameBar();


protected:
	D3DXVECTOR3 m_vStartPos;		// ���� ����
	D3DXVECTOR3	m_vTargetPos;		// �� ���� ����

	FRAME		m_tFrame;

	wstring		m_wstrName			= L"";	// �̸�

protected:
	CUIMgr*		m_pUIMgr			= CUIMgr::GetInstance();

protected:
	CItemData*	m_pItemData			= nullptr;

	int			m_iState			= 1;	// 0:���� �������� ��, 1:���� �������ִ� ����, 2:�κ��丮�� ���� ����

	float		m_fJumMaxGravity	= -8.f;
	float		m_fJumPower			= 2.f;
	float		m_fJumSpeed			= 1.f;
	float		m_fJumAccPower		= 0.1f;
	float		m_fJumAcc			= 0.0f;

	float		m_fSpeed			= 0.f;

	bool		m_bIsJump			= false;
};

