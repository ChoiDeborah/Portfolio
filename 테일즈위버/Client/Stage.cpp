#include "stdafx.h"
#include "Stage.h"

#include "Item.h"

CStage::CStage()
{
}

CStage::~CStage()
{
	Release();
}

HRESULT CStage::Initialize()
{
	HRESULT hr = LoadMap(m_pSceneMgr->GetMapName());
	FAILED_CHECK_MSG_RETURN(hr, L"LoadMap Failed", E_FAIL);

	m_pWordMgr->Initialize();
	CMouseMgr::GetInstance()->Initialize();

	if (0 == m_pSceneMgr->GetStageNum())
	{
		m_pSoundMgr->StopSound(CHANNEL_ID::CHANNEL_BGM);
		m_pSoundMgr->PlayBGM(L"Village1.wav");
	}
	else if (2 == m_pSceneMgr->GetStageNum())
	{
		m_pSoundMgr->StopSound(CHANNEL_ID::CHANNEL_BGM);
		m_pSoundMgr->PlayBGM(L"BGM_Boss.mp3");
	}

	return S_OK;
}

void CStage::Update()
{
	m_pUIMgr->Update();
	m_pObjMgr->Update();
	m_pViewMgr->Update();

	if (!m_pSceneMgr->GetIsEventScene(EVENT_SCENE0))
	{
		if (2 == m_pSceneMgr->GetStageNum())
		{
			m_pSceneMgr->SetIsEventScene(EVENT_SCENE0, true);
			m_pSceneMgr->SetNextScene(SCENEID::SCENEID_EVENT, L"Event2");
		}
	}
	else if (!m_pSceneMgr->GetIsEventScene(EVENT_SCENE1))
	{
		if (3 == m_pSceneMgr->GetStageNum())
		{
			m_pSceneMgr->SetIsEventScene(EVENT_SCENE1, true);
			m_pSceneMgr->SetNextScene(SCENEID::SCENEID_EVENT, L"Event3");
		}
	}
	else if (!m_pSceneMgr->GetIsEventScene(EVENT_SCENE2))
	{
		if (6 == m_pSceneMgr->GetStageNum())
		{
			m_pSceneMgr->SetIsEventScene(EVENT_SCENE2, true);
			m_pSceneMgr->SetNextScene(SCENEID::SCENEID_EVENT, L"Event4");
		}
	}
}

void CStage::LateUpdate()
{
	m_pObjMgr->LateUpdate();
}

void CStage::Render()
{
	m_pTileMgr->Render();
	m_pObjMgr->Render();

	if (m_pSceneMgr->GetEvent(EVENT_FADE_DARK))
	{
		m_pDeviceMgr->GetSprite()->End();
		m_pDeviceMgr->GetDevice()->EndScene();

		m_pDeviceMgr->GetDevice()->BeginScene();
		m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

		D3DXMATRIX matScale, matTrans, matWorld;

		const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"Effect", L"Dark");
		if (nullptr != pTexInfo)
		{
			D3DXMatrixScaling(&matScale, float(CLIENTCX) / pTexInfo->tImgInfo.Width, float(CLIENTCY) / pTexInfo->tImgInfo.Height, 0.f);
			D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);
			matWorld = matScale * matTrans;
			m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
			m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
				&pTexInfo->vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		m_pDeviceMgr->GetSprite()->End();
		m_pDeviceMgr->GetDevice()->EndScene();

		m_pDeviceMgr->GetDevice()->BeginScene();
		m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
	}

	m_pUIMgr->Render();
}

void CStage::Release()
{
	m_pObjMgr->ResetObjList();
	m_pTileMgr->Release();
}