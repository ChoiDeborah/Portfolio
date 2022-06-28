#include "stdafx.h"
#include "Title.h"


CTitle::CTitle()
{
}


CTitle::~CTitle()
{
	Release();
}

HRESULT CTitle::Initialize() {
	m_pViewMgr->Initialize();

	CUIMgr::GetInstance()->Initialize();

	HRESULT hr = CTextureMgr::GetInstance()->LoadTexture(
		CTextureMgr::SINGLE_TEXTURE, L"../Texture/BackGround/Logo/READY_MENU.png", L"Title", L"", 0, 0, 0);
	FAILED_CHECK_MSG_RETURN(hr, L"Logo Texture Load Failed", E_FAIL);
	m_pTexInfo = CTextureMgr::GetInstance()->GetTexInfo(L"Title");

	m_pSoundMgr->StopSound(CHANNEL_ID::CHANNEL_BGM);
	m_pSoundMgr->PlayBGM(L"BGM_Menu.mp3");

	if (nullptr != m_pTexInfo)
	{
		float fW = m_pViewMgr->GetRoomW() / m_pTexInfo->tImgInfo.Width;
		float fH = m_pViewMgr->GetRoomH() / m_pTexInfo->tImgInfo.Height;

		m_tGameStart.vPos = { 445.f * fW, 505.f * fH, 0.f };
		m_tGameStart.vSize = { 140.f * fW, 30.f * fH, 0.f };

		m_tShowOpenning.vPos = { 445.f * fW, 536.f * fH, 0.f };
		m_tShowOpenning.vSize = { 140.f* fW, 30.f * fH, 0.f };

		m_tCredit.vPos = { 445.f * fW, 573.f * fH, 0.f };
		m_tCredit.vSize = { 140.f * fW, 30.f * fH, 0.f };

		m_tExit.vPos = { 445.f * fW, 604.f * fH, 0.f };
		m_tExit.vSize = { 140.f * fW, 30.f * fH, 0.f };
	}
	return S_OK;
}

void CTitle::Update() {
	D3DXVECTOR3 vMouse = m_pMouseMgr->GetMouse();

	if (0 == m_tGameStart.iHover && m_tGameStart.IsRectInPoint(vMouse))
		m_pSoundMgr->PlaySound(L"barover.MP3", CHANNEL_ID::CHANNEL_WINDOW);
	if (0 == m_tShowOpenning.iHover && m_tShowOpenning.IsRectInPoint(vMouse))
		m_pSoundMgr->PlaySound(L"barover.MP3", CHANNEL_ID::CHANNEL_WINDOW);
	if (0 == m_tCredit.iHover && m_tCredit.IsRectInPoint(vMouse))
		m_pSoundMgr->PlaySound(L"barover.MP3", CHANNEL_ID::CHANNEL_WINDOW);
	if (0 == m_tExit.iHover && m_tExit.IsRectInPoint(vMouse))
		m_pSoundMgr->PlaySound(L"barover.MP3", CHANNEL_ID::CHANNEL_WINDOW);

	if (m_pKeyMgr->KeyDown(VK_LBUTTON))
	{
		if (m_tGameStart.IsRectInPoint(vMouse))
		{
			m_pSoundMgr->PlaySound(L"Start.mp3", CHANNEL_ID::CHANNEL_EFFECT);
			m_pSceneMgr->SetNextScene(SCENEID::SCENEID_EVENT, L"Event0");//L"test4");
		}
		else if (m_tShowOpenning.IsRectInPoint(vMouse))
		{
		}
		else if (m_tCredit.IsRectInPoint(vMouse))
		{
			m_tCreditWnd.ShowWindow(true);
		}
		else if (m_tExit.IsRectInPoint(vMouse))
		{
			exit(true);
		}
	}

	m_tCreditWnd.Update();
}

void CTitle::LateUpdate()
{
}

void CTitle::Render() {
	m_pViewMgr->Update();

	NULL_CHECK(m_pTexInfo);

	float fW = m_pViewMgr->GetRoomW() / m_pTexInfo->tImgInfo.Width;
	float fH = m_pViewMgr->GetRoomH() / m_pTexInfo->tImgInfo.Height;

	D3DXMATRIX matScale, matTrans, matWorld;

	D3DXMatrixScaling(&matScale, fW, fH, 1.f);
	D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);
	matWorld = matScale * matTrans;
	CDeviceMgr::GetInstance()->GetSprite()->SetTransform(&matWorld);
	CDeviceMgr::GetInstance()->GetSprite()->Draw(m_pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXVECTOR3 vMouse = m_pMouseMgr->GetMouse();
	if (m_tGameStart.IsRectInPoint(vMouse))
	{
		m_pDeviceMgr->GetSprite()->End();
		m_pDeviceMgr->GetDevice()->EndScene();

		m_pDeviceMgr->GetDevice()->BeginScene();
		m_pDeviceMgr->GetLine()->Begin();

		m_pViewMgr->Render();

		D3DXVECTOR2 pLine[5] = {
			D3DXVECTOR2{ m_tGameStart.vPos.x, m_tGameStart.vPos.y },
			D3DXVECTOR2{ m_tGameStart.vPos.x + m_tGameStart.vSize.x, m_tGameStart.vPos.y },
			D3DXVECTOR2{ m_tGameStart.vPos.x + m_tGameStart.vSize.x, m_tGameStart.vPos.y + m_tGameStart.vSize.y },
			D3DXVECTOR2{ m_tGameStart.vPos.x, m_tGameStart.vPos.y + m_tGameStart.vSize.y },
			D3DXVECTOR2{ m_tGameStart.vPos.x, m_tGameStart.vPos.y }
		};
		m_pDeviceMgr->GetLine()->Draw(pLine, 5, D3DCOLOR_ARGB(255, 0, 255, 255));

		m_pDeviceMgr->GetLine()->End();
		m_pDeviceMgr->GetDevice()->EndScene();

		m_pDeviceMgr->GetDevice()->BeginScene();
		m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

		m_pViewMgr->Render();
	}
	else if (m_tShowOpenning.IsRectInPoint(vMouse))
	{
		m_pDeviceMgr->GetSprite()->End();
		m_pDeviceMgr->GetDevice()->EndScene();

		m_pDeviceMgr->GetDevice()->BeginScene();
		m_pDeviceMgr->GetLine()->Begin();

		m_pViewMgr->Render();

		D3DXVECTOR2 pLine[5] = {
			D3DXVECTOR2{ m_tShowOpenning.vPos.x, m_tShowOpenning.vPos.y },
			D3DXVECTOR2{ m_tShowOpenning.vPos.x + m_tShowOpenning.vSize.x, m_tShowOpenning.vPos.y },
			D3DXVECTOR2{ m_tShowOpenning.vPos.x + m_tShowOpenning.vSize.x, m_tShowOpenning.vPos.y + m_tShowOpenning.vSize.y },
			D3DXVECTOR2{ m_tShowOpenning.vPos.x, m_tShowOpenning.vPos.y + m_tShowOpenning.vSize.y },
			D3DXVECTOR2{ m_tShowOpenning.vPos.x, m_tShowOpenning.vPos.y }
		};
		m_pDeviceMgr->GetLine()->Draw(pLine, 5, D3DCOLOR_ARGB(255, 0, 255, 255));

		m_pDeviceMgr->GetLine()->End();
		m_pDeviceMgr->GetDevice()->EndScene();

		m_pDeviceMgr->GetDevice()->BeginScene();
		m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

		m_pViewMgr->Render();
	}
	else if (m_tCredit.IsRectInPoint(vMouse))
	{
		m_pDeviceMgr->GetSprite()->End();
		m_pDeviceMgr->GetDevice()->EndScene();

		m_pDeviceMgr->GetDevice()->BeginScene();
		m_pDeviceMgr->GetLine()->Begin();

		m_pViewMgr->Render();

		D3DXVECTOR2 pLine[5] = {
			D3DXVECTOR2{m_tCredit.vPos.x, m_tCredit.vPos.y },
			D3DXVECTOR2{m_tCredit.vPos.x +m_tCredit.vSize.x, m_tCredit.vPos.y },
			D3DXVECTOR2{m_tCredit.vPos.x +m_tCredit.vSize.x, m_tCredit.vPos.y + m_tCredit.vSize.y },
			D3DXVECTOR2{m_tCredit.vPos.x, m_tCredit.vPos.y + m_tCredit.vSize.y },
			D3DXVECTOR2{m_tCredit.vPos.x, m_tCredit.vPos.y }
		};
		m_pDeviceMgr->GetLine()->Draw(pLine, 5, D3DCOLOR_ARGB(255, 0, 255, 255));

		m_pDeviceMgr->GetLine()->End();
		m_pDeviceMgr->GetDevice()->EndScene();

		m_pDeviceMgr->GetDevice()->BeginScene();
		m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

		m_pViewMgr->Render();
	}
	else if (m_tExit.IsRectInPoint(vMouse))
	{
		m_pDeviceMgr->GetSprite()->End();
		m_pDeviceMgr->GetDevice()->EndScene();

		m_pDeviceMgr->GetDevice()->BeginScene();
		m_pDeviceMgr->GetLine()->Begin();

		m_pViewMgr->Render();

		D3DXVECTOR2 pLine[5] = {
			D3DXVECTOR2{m_tExit.vPos.x, m_tExit.vPos.y },
			D3DXVECTOR2{m_tExit.vPos.x + m_tExit.vSize.x, m_tExit.vPos.y },
			D3DXVECTOR2{m_tExit.vPos.x + m_tExit.vSize.x, m_tExit.vPos.y + m_tExit.vSize.y },
			D3DXVECTOR2{m_tExit.vPos.x, m_tExit.vPos.y + m_tExit.vSize.y },
			D3DXVECTOR2{m_tExit.vPos.x, m_tExit.vPos.y }
		};
		m_pDeviceMgr->GetLine()->Draw(pLine, 5, D3DCOLOR_ARGB(255, 0, 255, 255));

		m_pDeviceMgr->GetLine()->End();
		m_pDeviceMgr->GetDevice()->EndScene();

		m_pDeviceMgr->GetDevice()->BeginScene();
		m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

		m_pViewMgr->Render();
	}

	m_tCreditWnd.Render();
}

void CTitle::Release() 
{
}
