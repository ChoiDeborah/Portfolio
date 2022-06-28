#include "stdafx.h"
#include "InfoWnd.h"

#include "Player.h"

CInfoWnd::CInfoWnd()
{
}


CInfoWnd::~CInfoWnd()
{
	Release();
}

HRESULT CInfoWnd::Initialize()
{
	m_pWndTex = m_pTextureMgr->GetTexInfo(L"UI", L"InfoWnd");
	if (nullptr != m_pWndTex) {
		m_tWndInfo.vPos = { float(CLIENTCX - m_pWndTex->tImgInfo.Width) - 18.f, 0.f, 0.f };
		m_tWndInfo.vSize = { float(m_pWndTex->tImgInfo.Width), float(m_pWndTex->tImgInfo.Height), 0.f };
		m_tWndInfo.vOffset = m_pWndTex->vOffset;

		m_tMoveInfo.vSize = { float(m_pWndTex->tImgInfo.Width), 30.f, 0.f };
		m_tMoveInfo.vParentPos = m_tWndInfo.vPos;
		m_tMoveInfo.vPos = { 0.f, 0.f, 0.f };
		m_tMoveInfo.vOffset = m_tWndInfo.vOffset;

		m_pExitTex = m_pTextureMgr->GetTexInfo(L"Button", L"XBtn", m_tExitInfo.iHover);
		if (nullptr != m_pExitTex)
		{
			m_tExitInfo.vParentPos = m_tWndInfo.vPos;
			m_tExitInfo.vPos = { float(m_pWndTex->tImgInfo.Width) - 18.f, 11.f, 0.f };
			m_tExitInfo.vSize = { float(m_pExitTex->tImgInfo.Width), float(m_pExitTex->tImgInfo.Height), 0.f };
			m_tExitInfo.vOffset = m_pExitTex->vOffset;
		}
	}

	m_pExpBar	= m_pTextureMgr->GetTexInfo(L"UI", L"InfoExpBar");

	m_pStatusUp = m_pTextureMgr->GetTexInfo(L"Button", L"StatusUp");

	if (nullptr != m_pStatusUp) {
		tStabBtn.vPos = { 112.f, 182.f, 0.f };
		tStabBtn.vParentPos = m_tWndInfo.vPos;
		tStabBtn.vSize = { float(m_pStatusUp->tImgInfo.Width), float(m_pStatusUp->tImgInfo.Height), 0.f };
		tStabBtn.vOffset = m_pStatusUp->vOffset;

		tHackBtn.vPos = { 112.f, 197.f, 0.f };
		tHackBtn.vParentPos = m_tWndInfo.vPos;
		tHackBtn.vSize = { float(m_pStatusUp->tImgInfo.Width), float(m_pStatusUp->tImgInfo.Height), 0.f };
		tHackBtn.vOffset = m_pStatusUp->vOffset;

		tIntBtn.vPos = { 112.f, 212.f, 0.f };
		tIntBtn.vParentPos = m_tWndInfo.vPos;
		tIntBtn.vSize = { float(m_pStatusUp->tImgInfo.Width), float(m_pStatusUp->tImgInfo.Height), 0.f };
		tIntBtn.vOffset = m_pStatusUp->vOffset;

		tDefBtn.vPos = { 112.f, 227.f, 0.f };
		tDefBtn.vParentPos = m_tWndInfo.vPos;
		tDefBtn.vSize = { float(m_pStatusUp->tImgInfo.Width), float(m_pStatusUp->tImgInfo.Height), 0.f };
		tDefBtn.vOffset = m_pStatusUp->vOffset;

		tMrBtn.vPos = { 112.f, 242.f, 0.f };
		tMrBtn.vParentPos = m_tWndInfo.vPos;
		tMrBtn.vSize = { float(m_pStatusUp->tImgInfo.Width), float(m_pStatusUp->tImgInfo.Height), 0.f };
		tMrBtn.vOffset = m_pStatusUp->vOffset;

		tDexBtn.vPos = { 112.f, 257.f, 0.f };
		tDexBtn.vParentPos = m_tWndInfo.vPos;
		tDexBtn.vSize = { float(m_pStatusUp->tImgInfo.Width), float(m_pStatusUp->tImgInfo.Height), 0.f };
		tDexBtn.vOffset = m_pStatusUp->vOffset;

		tAgiBtn.vPos = { 112.f, 272.f, 0.f };
		tAgiBtn.vParentPos = m_tWndInfo.vPos;
		tAgiBtn.vSize = { float(m_pStatusUp->tImgInfo.Width), float(m_pStatusUp->tImgInfo.Height), 0.f };
		tAgiBtn.vOffset = m_pStatusUp->vOffset;
	}

	return S_OK;
}


void CInfoWnd::Update()
{
	CUIWnd::WindowMove();
	UpdateState();
	tStabBtn.vParentPos
		= tHackBtn.vParentPos
		= tIntBtn.vParentPos
		= tDefBtn.vParentPos
		= tMrBtn.vParentPos
		= tDexBtn.vParentPos
		= tAgiBtn.vParentPos
		= m_tWndInfo.vPos;

	D3DXVECTOR3 vMouse = m_pMouseMgr->GetMouse();
	if ((0 == m_tExitInfo.iHover && m_tExitInfo.IsRectInPoint(vMouse)))
		m_pSoundMgr->PlaySound(L"barover.MP3", CHANNEL_ID::CHANNEL_WINDOW);

	/*if ((0 == tStabBtn.iHover && tStabBtn.IsRectInPoint(vMouse)))
		m_pSoundMgr->PlaySound(L"barover.MP3", CHANNEL_ID::CHANNEL_WINDOW);
	if ((0 == tHackBtn.iHover && tHackBtn.IsRectInPoint(vMouse)))
		m_pSoundMgr->PlaySound(L"barover.MP3", CHANNEL_ID::CHANNEL_WINDOW);
	if ((0 == tIntBtn.iHover && tIntBtn.IsRectInPoint(vMouse))	)
		m_pSoundMgr->PlaySound(L"barover.MP3", CHANNEL_ID::CHANNEL_WINDOW);
	if ((0 == tDefBtn.iHover && tDefBtn.IsRectInPoint(vMouse))	)
		m_pSoundMgr->PlaySound(L"barover.MP3", CHANNEL_ID::CHANNEL_WINDOW);
	if ((0 == tMrBtn.iHover && tMrBtn.IsRectInPoint(vMouse))	)
		m_pSoundMgr->PlaySound(L"barover.MP3", CHANNEL_ID::CHANNEL_WINDOW);
	if ((0 == tDexBtn.iHover && tDexBtn.IsRectInPoint(vMouse))	)
		m_pSoundMgr->PlaySound(L"barover.MP3", CHANNEL_ID::CHANNEL_WINDOW);
	if ((0 == tAgiBtn.iHover && tAgiBtn.IsRectInPoint(vMouse))	)
		m_pSoundMgr->PlaySound(L"barover.MP3", CHANNEL_ID::CHANNEL_WINDOW);*/
}

void CInfoWnd::Render()
{
	if (!m_bIsShow)
		return;

	D3DXMATRIX matScale, matTrans, matWorld;

	if (nullptr != m_pWndTex) {
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x, m_tWndInfo.vPos.y, 0.f);
		matWorld = matScale * matTrans;

		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pWndTex->pTexture, nullptr,
			&m_tWndInfo.vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		// 닫기 버튼
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tExitInfo.vParentPos.x + m_tExitInfo.vPos.x, m_tExitInfo.vParentPos.y + m_tExitInfo.vPos.y, 0.f);
		matWorld = matScale * matTrans;
		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pExitTex->pTexture, nullptr,
			&m_tExitInfo.vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		//m_pUIMgr->DrawRect(m_tWndInfo);
		//m_pUIMgr->DrawRect(m_tMoveInfo);

		const TEX_INFO* pCharacterTex = m_pTextureMgr->GetTexInfo(L"Character", L"Kyoko");
		if (nullptr != pCharacterTex)
		{
			D3DXMatrixScaling(&matScale, 85.f/float(pCharacterTex->tImgInfo.Width), 101.f / float(pCharacterTex->tImgInfo.Height), 0.f);
			D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x + 4.f, m_tWndInfo.vPos.y + 26.f, 0.f);
			matWorld = matScale * matTrans;

			m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
			m_pDeviceMgr->GetSprite()->Draw(pCharacterTex->pTexture, nullptr,
				&D3DXVECTOR3{0.f, 0.f, 0.f}, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		// Exp바
		//36, 148
		if (nullptr != m_pExpBar) {
			D3DXMatrixScaling(&matScale, float(m_pUIMgr->GetPlayerData().iExp) / m_pUIMgr->GetPlayerIdentity().iMaxExp, 1.f, 0.f);
			D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x + 36.f, m_tWndInfo.vPos.y + 148.f, 0.f);
			matWorld = matScale * matTrans;

			m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
			m_pDeviceMgr->GetSprite()->Draw(m_pExpBar->pTexture, nullptr,
				&m_pExpBar->vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		// 스탯 증가 버튼
		if (nullptr != m_pStatusUp) {
			// Stab
			if (tStabBtn.bIsShow)
			{
				D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
				D3DXMatrixTranslation(&matTrans, tStabBtn.vParentPos.x + tStabBtn.vPos.x, tStabBtn.vParentPos.y + tStabBtn.vPos.y, 0.f);
				matWorld = matScale * matTrans;
				m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
				m_pDeviceMgr->GetSprite()->Draw(m_pStatusUp->pTexture, nullptr, &m_pStatusUp->vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
			}

			// Hack
			if (tHackBtn.bIsShow)
			{
				D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
				D3DXMatrixTranslation(&matTrans, tHackBtn.vParentPos.x + tHackBtn.vPos.x, tHackBtn.vParentPos.y + tHackBtn.vPos.y, 0.f);
				matWorld = matScale * matTrans;
				m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
				m_pDeviceMgr->GetSprite()->Draw(m_pStatusUp->pTexture, nullptr, &m_pStatusUp->vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
			}

			// Int
			if (tIntBtn.bIsShow)
			{
				D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
				D3DXMatrixTranslation(&matTrans, tIntBtn.vParentPos.x + tIntBtn.vPos.x, tIntBtn.vParentPos.y + tIntBtn.vPos.y, 0.f);
				matWorld = matScale * matTrans;
				m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
				m_pDeviceMgr->GetSprite()->Draw(m_pStatusUp->pTexture, nullptr, &m_pStatusUp->vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
			}

			// Def
			if (tDefBtn.bIsShow)
			{
				D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
				D3DXMatrixTranslation(&matTrans, tDefBtn.vParentPos.x + tDefBtn.vPos.x, tDefBtn.vParentPos.y + tDefBtn.vPos.y, 0.f);
				matWorld = matScale * matTrans;
				m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
				m_pDeviceMgr->GetSprite()->Draw(m_pStatusUp->pTexture, nullptr, &m_pStatusUp->vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
			}

			// Mr
			if (tMrBtn.bIsShow)
			{
				D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
				D3DXMatrixTranslation(&matTrans, tMrBtn.vParentPos.x + tMrBtn.vPos.x, tMrBtn.vParentPos.y + tMrBtn.vPos.y, 0.f);
				matWorld = matScale * matTrans;
				m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
				m_pDeviceMgr->GetSprite()->Draw(m_pStatusUp->pTexture, nullptr, &m_pStatusUp->vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
			}

			// Dex
			if (tDexBtn.bIsShow)
			{
				D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
				D3DXMatrixTranslation(&matTrans, tDexBtn.vParentPos.x + tDexBtn.vPos.x, tDexBtn.vParentPos.y + tDexBtn.vPos.y, 0.f);
				matWorld = matScale * matTrans;
				m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
				m_pDeviceMgr->GetSprite()->Draw(m_pStatusUp->pTexture, nullptr, &m_pStatusUp->vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
			}

			// Agi
			if (tAgiBtn.bIsShow)
			{
				D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
				D3DXMatrixTranslation(&matTrans, tAgiBtn.vParentPos.x + tAgiBtn.vPos.x, tAgiBtn.vParentPos.y + tAgiBtn.vPos.y, 0.f);
				matWorld = matScale * matTrans;
				m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
				m_pDeviceMgr->GetSprite()->Draw(m_pStatusUp->pTexture, nullptr, &m_pStatusUp->vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
			}
		}


		// 정보 텍스트
		{
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
			m_pDeviceMgr->GetSprite()->SetTransform(&matScale);
			RECT rt;
			wstring wstrText;

			// 속성
			// 번개 속성
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 108.f), int(m_tWndInfo.vPos.y + 26.f),
				int(m_tWndInfo.vPos.x + 130.f), int(m_tWndInfo.vPos.y + 38.f));
			m_pDeviceMgr->DrawFont(to_wstring(m_pUIMgr->GetPlayerData().tStatInfo.iLightning + m_pUIMgr->GetPlayerData().tAddStatInfo.iLightning), &rt, DT_LEFT);

			// 불 속성
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 108.f), int(m_tWndInfo.vPos.y + 51.f),
				int(m_tWndInfo.vPos.x + 130.f), int(m_tWndInfo.vPos.y + 63.f));
			m_pDeviceMgr->DrawFont(to_wstring(m_pUIMgr->GetPlayerData().tStatInfo.iFire + m_pUIMgr->GetPlayerData().tAddStatInfo.iFire), &rt, DT_LEFT);

			// 땅 속성
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 108.f), int(m_tWndInfo.vPos.y + 76.f),
				int(m_tWndInfo.vPos.x + 130.f), int(m_tWndInfo.vPos.y + 88.f));
			m_pDeviceMgr->DrawFont(to_wstring(m_pUIMgr->GetPlayerData().tStatInfo.iEarth + m_pUIMgr->GetPlayerData().tAddStatInfo.iEarth), &rt, DT_LEFT);

			// 빛 속성
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 108.f), int(m_tWndInfo.vPos.y + 101.f),
				int(m_tWndInfo.vPos.x + 130.f), int(m_tWndInfo.vPos.y + 113.f));
			m_pDeviceMgr->DrawFont(to_wstring(m_pUIMgr->GetPlayerData().tStatInfo.iLight + m_pUIMgr->GetPlayerData().tAddStatInfo.iLight), &rt, DT_LEFT);


			// 얼음 속성
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 149.f), int(m_tWndInfo.vPos.y + 38.f),
				int(m_tWndInfo.vPos.x + 171.f), int(m_tWndInfo.vPos.y + 50.f));
			m_pDeviceMgr->DrawFont(to_wstring(m_pUIMgr->GetPlayerData().tStatInfo.iIce + m_pUIMgr->GetPlayerData().tAddStatInfo.iIce), &rt, DT_LEFT);

			// 바람 속성
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 149.f), int(m_tWndInfo.vPos.y + 63.f),
				int(m_tWndInfo.vPos.x + 171.f), int(m_tWndInfo.vPos.y + 75.f));
			m_pDeviceMgr->DrawFont(to_wstring(m_pUIMgr->GetPlayerData().tStatInfo.iWind + m_pUIMgr->GetPlayerData().tAddStatInfo.iWind), &rt, DT_LEFT);

			// 어둠 속성
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 149.f), int(m_tWndInfo.vPos.y + 88.f),
				int(m_tWndInfo.vPos.x + 171.f), int(m_tWndInfo.vPos.y + 100.f));
			m_pDeviceMgr->DrawFont(to_wstring(m_pUIMgr->GetPlayerData().tStatInfo.iDark + m_pUIMgr->GetPlayerData().tAddStatInfo.iDark), &rt, DT_LEFT);

			// 노말 속성
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 149.f), int(m_tWndInfo.vPos.y + 113.f),
				int(m_tWndInfo.vPos.x + 171.f), int(m_tWndInfo.vPos.y + 125.f));
			m_pDeviceMgr->DrawFont(to_wstring(m_pUIMgr->GetPlayerData().tStatInfo.iNormal + m_pUIMgr->GetPlayerData().tAddStatInfo.iNormal), &rt, DT_LEFT);

			////////////////////////////////////////////////////////////////////////////////////////

			// Level
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 208.f), int(m_tWndInfo.vPos.y + 37.f),
				int(m_tWndInfo.vPos.x + 238.f), int(m_tWndInfo.vPos.y + 50.f));
			m_pDeviceMgr->DrawFont(to_wstring(m_pUIMgr->GetPlayerData().iLevel), &rt, DT_CENTER);

			// 이름
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 208.f), int(m_tWndInfo.vPos.y + 56.f),
				int(m_tWndInfo.vPos.x + 333.f), int(m_tWndInfo.vPos.y + 69.f));
			m_pDeviceMgr->DrawFont(m_pUIMgr->GetPlayerData().wstrName, &rt, DT_CENTER);

			// 타입
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 208.f), int(m_tWndInfo.vPos.y + 75.f),
				int(m_tWndInfo.vPos.x + 333.f), int(m_tWndInfo.vPos.y + 88.f));
			m_pDeviceMgr->DrawFont(L"찌르기형", &rt, DT_CENTER);

			// 칭호
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 208.f), int(m_tWndInfo.vPos.y + 94.f),
				int(m_tWndInfo.vPos.x + 333.f), int(m_tWndInfo.vPos.y + 107.f));
			m_pDeviceMgr->DrawFont(L"5252 믿고있었다구!", &rt, DT_CENTER);

			////////////////////////////////////////////////////////////////////////////////

			// CurExp
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 37.f), int(m_tWndInfo.vPos.y + 135.f),
				int(m_tWndInfo.vPos.x + 127.f), int(m_tWndInfo.vPos.y + 145.f));
			m_pDeviceMgr->DrawFont(to_wstring(m_pUIMgr->GetPlayerData().iExp), &rt, DT_RIGHT);

			// MinExp
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 140.f), int(m_tWndInfo.vPos.y + 135.f),
				int(m_tWndInfo.vPos.x + 230.f), int(m_tWndInfo.vPos.y + 145.f));
			m_pDeviceMgr->DrawFont(to_wstring(m_pUIMgr->GetPlayerIdentity().iMinExp), &rt, DT_RIGHT);

			// MaxExp
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 245.f), int(m_tWndInfo.vPos.y + 135.f),
				int(m_tWndInfo.vPos.x + 335.f), int(m_tWndInfo.vPos.y + 145.f));
			m_pDeviceMgr->DrawFont(to_wstring(m_pUIMgr->GetPlayerIdentity().iMaxExp), &rt, DT_RIGHT);


			////////////////////////////////////////////////////////////////////////////////

			// Status
			// STAB
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 45.f), int(m_tWndInfo.vPos.y + 182.f),
				int(m_tWndInfo.vPos.x + 107.f), int(m_tWndInfo.vPos.y + 194.f));
			wstrText = IsAddStat(m_pUIMgr->GetPlayerData().tAddStatInfo.iStab, to_wstring(m_pUIMgr->GetPlayerData().tStatInfo.iStab));
			m_pDeviceMgr->DrawFont(wstrText, &rt, DT_CENTER);

			// HACK
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 45.f), int(m_tWndInfo.vPos.y + 197.f),
				int(m_tWndInfo.vPos.x + 107.f), int(m_tWndInfo.vPos.y + 209.f));
			wstrText = IsAddStat(m_pUIMgr->GetPlayerData().tAddStatInfo.iHack, to_wstring(m_pUIMgr->GetPlayerData().tStatInfo.iHack));
			m_pDeviceMgr->DrawFont(wstrText, &rt, DT_CENTER);

			// INT
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 45.f), int(m_tWndInfo.vPos.y + 212.f),
				int(m_tWndInfo.vPos.x + 107.f), int(m_tWndInfo.vPos.y + 224.f));
			wstrText = IsAddStat(m_pUIMgr->GetPlayerData().tAddStatInfo.iInt, to_wstring(m_pUIMgr->GetPlayerData().tStatInfo.iInt));
			m_pDeviceMgr->DrawFont(wstrText, &rt, DT_CENTER);

			// DEF
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 45.f), int(m_tWndInfo.vPos.y + 227.f),
				int(m_tWndInfo.vPos.x + 107.f), int(m_tWndInfo.vPos.y + 239.f));
			wstrText = IsAddStat(m_pUIMgr->GetPlayerData().tAddStatInfo.iDef, to_wstring(m_pUIMgr->GetPlayerData().tStatInfo.iDef));
			m_pDeviceMgr->DrawFont(wstrText, &rt, DT_CENTER);

			// MR
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 45.f), int(m_tWndInfo.vPos.y + 242.f),
				int(m_tWndInfo.vPos.x + 107.f), int(m_tWndInfo.vPos.y + 254.f));
			wstrText = IsAddStat(m_pUIMgr->GetPlayerData().tAddStatInfo.iMr, to_wstring(m_pUIMgr->GetPlayerData().tStatInfo.iMr));
			m_pDeviceMgr->DrawFont(wstrText, &rt, DT_CENTER);

			// DEX
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 45.f), int(m_tWndInfo.vPos.y + 257.f),
				int(m_tWndInfo.vPos.x + 107.f), int(m_tWndInfo.vPos.y + 269.f));
			wstrText = IsAddStat(m_pUIMgr->GetPlayerData().tAddStatInfo.iDex, to_wstring(m_pUIMgr->GetPlayerData().tStatInfo.iDex));
			m_pDeviceMgr->DrawFont(wstrText, &rt, DT_CENTER);

			// AGI
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 45.f), int(m_tWndInfo.vPos.y + 272.f),
				int(m_tWndInfo.vPos.x + 107.f), int(m_tWndInfo.vPos.y + 284.f));
			wstrText = IsAddStat(m_pUIMgr->GetPlayerData().tAddStatInfo.iAgi, to_wstring(m_pUIMgr->GetPlayerData().tStatInfo.iAgi));
			m_pDeviceMgr->DrawFont(wstrText, &rt, DT_CENTER);

			// HP
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 207.f), int(m_tWndInfo.vPos.y + 182.f),
				int(m_tWndInfo.vPos.x + 335.f), int(m_tWndInfo.vPos.y + 194.f));
			wstrText = IsAddStat(0, to_wstring(m_pUIMgr->GetPlayerData().iHp) + L" / " + to_wstring(m_pUIMgr->GetPlayerData().iMaxHp));
			m_pDeviceMgr->DrawFont(wstrText, &rt, DT_CENTER);

			// MP
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 207.f), int(m_tWndInfo.vPos.y + 197.f),
				int(m_tWndInfo.vPos.x + 335.f), int(m_tWndInfo.vPos.y + 209.f));
			wstrText = IsAddStat(0, to_wstring(m_pUIMgr->GetPlayerData().iMp) + L" / " + to_wstring(m_pUIMgr->GetPlayerData().iMaxMp));
			m_pDeviceMgr->DrawFont(wstrText, &rt, DT_CENTER);

			// SP
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 207.f), int(m_tWndInfo.vPos.y + 212.f),
				int(m_tWndInfo.vPos.x + 335.f), int(m_tWndInfo.vPos.y + 224.f));
			wstrText = IsAddStat(0, to_wstring(m_pUIMgr->GetPlayerData().iSp) + L" / " + to_wstring(m_pUIMgr->GetPlayerData().iMaxSp));
			m_pDeviceMgr->DrawFont(wstrText, &rt, DT_CENTER);

			// SEED
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 207.f), int(m_tWndInfo.vPos.y + 227.f),
				int(m_tWndInfo.vPos.x + 335.f), int(m_tWndInfo.vPos.y + 239.f));
			m_pDeviceMgr->DrawFont(to_wstring(m_pUIMgr->GetPlayerIdentity().iSeed), &rt, DT_CENTER);

			// POINT
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 207.f), int(m_tWndInfo.vPos.y + 242.f),
				int(m_tWndInfo.vPos.x + 335.f), int(m_tWndInfo.vPos.y + 254.f));
			m_pDeviceMgr->DrawFont(to_wstring(m_pUIMgr->GetPlayerIdentity().iPoint), &rt, DT_CENTER);

			// NP
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 207.f), int(m_tWndInfo.vPos.y + 257.f),
				int(m_tWndInfo.vPos.x + 335.f), int(m_tWndInfo.vPos.y + 269.f));
			m_pDeviceMgr->DrawFont(to_wstring(m_pUIMgr->GetPlayerIdentity().iNp), &rt, DT_CENTER);

			////////////////////////////////////////////////////////////////////////////////////////

			// 필요 스탯
			// STAB
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 129.f), int(m_tWndInfo.vPos.y + 182.f),
				int(m_tWndInfo.vPos.x + 160.f), int(m_tWndInfo.vPos.y + 194.f));
			wstrText = IsAddStat(0, to_wstring(m_iNeedStab));
			m_pDeviceMgr->DrawFont(wstrText, &rt, DT_CENTER);

			// HACK
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 129.f), int(m_tWndInfo.vPos.y + 197.f),
				int(m_tWndInfo.vPos.x + 160.f), int(m_tWndInfo.vPos.y + 209.f));
			wstrText = IsAddStat(0, to_wstring(m_iNeedHack));
			m_pDeviceMgr->DrawFont(wstrText, &rt, DT_CENTER);

			// INT
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 129.f), int(m_tWndInfo.vPos.y + 212.f),
				int(m_tWndInfo.vPos.x + 160.f), int(m_tWndInfo.vPos.y + 224.f));
			wstrText = IsAddStat(0, to_wstring(m_iNeedInt));
			m_pDeviceMgr->DrawFont(wstrText, &rt, DT_CENTER);

			// DEF
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 129.f), int(m_tWndInfo.vPos.y + 227.f),
				int(m_tWndInfo.vPos.x + 160.f), int(m_tWndInfo.vPos.y + 239.f));
			wstrText = IsAddStat(0, to_wstring(m_iNeedDef));
			m_pDeviceMgr->DrawFont(wstrText, &rt, DT_CENTER);

			// MR
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 129.f), int(m_tWndInfo.vPos.y + 242.f),
				int(m_tWndInfo.vPos.x + 160.f), int(m_tWndInfo.vPos.y + 254.f));
			wstrText = IsAddStat(0, to_wstring(m_iNeedMr));
			m_pDeviceMgr->DrawFont(wstrText, &rt, DT_CENTER);

			// DEX
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 129.f), int(m_tWndInfo.vPos.y + 257.f),
				int(m_tWndInfo.vPos.x + 160.f), int(m_tWndInfo.vPos.y + 269.f));
			wstrText = IsAddStat(0, to_wstring(m_iNeedDex));
			m_pDeviceMgr->DrawFont(wstrText, &rt, DT_CENTER);

			// AGI
			SetRect(&rt,
				int(m_tWndInfo.vPos.x + 129.f), int(m_tWndInfo.vPos.y + 272.f),
				int(m_tWndInfo.vPos.x + 160.f), int(m_tWndInfo.vPos.y + 284.f));
			wstrText = IsAddStat(0, to_wstring(m_iNeedAgi));
			m_pDeviceMgr->DrawFont(wstrText, &rt, DT_CENTER);
		}
	}
}

void CInfoWnd::Release()
{
}

wstring CInfoWnd::IsAddStat(int iAddStat, const wstring& wstrBeforeStat)
{
	wstring wstrStat = wstrBeforeStat;
	if (0 < iAddStat)
		wstrStat += L"(+" + to_wstring(iAddStat) + L")";

	return wstrStat;
}

void CInfoWnd::LClickDown()
{
	if (!m_bIsShow)
		return;

	D3DXVECTOR3 vMouse = m_pMouseMgr->GetMouse();

	tStabBtn.vParentPos 
		=tHackBtn.vParentPos
		=tIntBtn.vParentPos
		=tDefBtn.vParentPos
		=tMrBtn.vParentPos
		=tDexBtn.vParentPos
		=tAgiBtn.vParentPos
		= m_tWndInfo.vPos;

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pObjMgr->GetPlayer());

	if (tStabBtn.bIsShow && tStabBtn.IsRectInPoint(vMouse))
	{
		m_pSoundMgr->PlaySound(L"BtnClick.wav", CHANNEL_ID::CHANNEL_WINDOW);

		m_pUIMgr->GetPlayerData().tStatInfo.iStab += 1;

		m_pUIMgr->GetPlayerData().iHp = (m_pUIMgr->GetPlayerData().iMaxHp += 15);
		m_pUIMgr->GetPlayerData().iMp = (m_pUIMgr->GetPlayerData().iMaxMp += 2);
		m_pUIMgr->GetPlayerData().iSp = (m_pUIMgr->GetPlayerData().iMaxSp += 4);

		m_pUIMgr->GetPlayerIdentity().iPoint -= m_iNeedStab;
	}

	if (tHackBtn.bIsShow && tHackBtn.IsRectInPoint(vMouse))
	{
		m_pSoundMgr->PlaySound(L"BtnClick.wav", CHANNEL_ID::CHANNEL_WINDOW);

		m_pUIMgr->GetPlayerData().tStatInfo.iHack += 1;

		m_pUIMgr->GetPlayerData().iHp = (m_pUIMgr->GetPlayerData().iMaxHp += 7);
		m_pUIMgr->GetPlayerData().iMp = (m_pUIMgr->GetPlayerData().iMaxMp += 1);
		m_pUIMgr->GetPlayerData().iSp = (m_pUIMgr->GetPlayerData().iMaxSp += 8);

		m_pUIMgr->GetPlayerIdentity().iPoint -= m_iNeedHack;
	}

	if (tIntBtn.bIsShow && tIntBtn.IsRectInPoint(vMouse))
	{
		m_pSoundMgr->PlaySound(L"BtnClick.wav", CHANNEL_ID::CHANNEL_WINDOW);

		m_pUIMgr->GetPlayerData().tStatInfo.iInt += 1;

		m_pUIMgr->GetPlayerData().iHp = (m_pUIMgr->GetPlayerData().iMaxHp += 1);
		m_pUIMgr->GetPlayerData().iMp = (m_pUIMgr->GetPlayerData().iMaxMp += 6);
		m_pUIMgr->GetPlayerData().iSp = (m_pUIMgr->GetPlayerData().iMaxSp += 2);

		m_pUIMgr->GetPlayerIdentity().iPoint -= m_iNeedInt;
	}

	if (tDefBtn.bIsShow && tDefBtn.IsRectInPoint(vMouse))
	{
		m_pSoundMgr->PlaySound(L"BtnClick.wav", CHANNEL_ID::CHANNEL_WINDOW);

		m_pUIMgr->GetPlayerData().tStatInfo.iDef += 1;

		m_pUIMgr->GetPlayerData().iHp = (m_pUIMgr->GetPlayerData().iMaxHp += 40);
		m_pUIMgr->GetPlayerData().iMp = (m_pUIMgr->GetPlayerData().iMaxMp += 0);
		m_pUIMgr->GetPlayerData().iSp = (m_pUIMgr->GetPlayerData().iMaxSp += 3);

		m_pUIMgr->GetPlayerIdentity().iPoint -= m_iNeedDef;
	}

	if (tMrBtn.bIsShow && tMrBtn.IsRectInPoint(vMouse))
	{
		m_pSoundMgr->PlaySound(L"BtnClick.wav", CHANNEL_ID::CHANNEL_WINDOW);

		m_pUIMgr->GetPlayerData().tStatInfo.iMr += 1;

		m_pUIMgr->GetPlayerData().iHp = (m_pUIMgr->GetPlayerData().iMaxHp += 2);
		m_pUIMgr->GetPlayerData().iMp = (m_pUIMgr->GetPlayerData().iMaxMp += 20);
		m_pUIMgr->GetPlayerData().iSp = (m_pUIMgr->GetPlayerData().iMaxSp += 6);

		m_pUIMgr->GetPlayerIdentity().iPoint -= m_iNeedMr;
	}

	if (tDexBtn.bIsShow && tDexBtn.IsRectInPoint(vMouse))
	{
		m_pSoundMgr->PlaySound(L"BtnClick.wav", CHANNEL_ID::CHANNEL_WINDOW);

		m_pUIMgr->GetPlayerData().tStatInfo.iDex += 1;

		m_pUIMgr->GetPlayerData().iHp = (m_pUIMgr->GetPlayerData().iMaxHp += 5);
		m_pUIMgr->GetPlayerData().iMp = (m_pUIMgr->GetPlayerData().iMaxMp += 3);
		m_pUIMgr->GetPlayerData().iSp = (m_pUIMgr->GetPlayerData().iMaxSp += 30);

		m_pUIMgr->GetPlayerIdentity().iPoint -= m_iNeedDex;
	}

	if (tAgiBtn.bIsShow && tAgiBtn.IsRectInPoint(vMouse))
	{
		m_pSoundMgr->PlaySound(L"BtnClick.wav", CHANNEL_ID::CHANNEL_WINDOW);

		m_pUIMgr->GetPlayerData().tStatInfo.iAgi += 1;

		m_pUIMgr->GetPlayerData().iHp = (m_pUIMgr->GetPlayerData().iMaxHp += 8);
		m_pUIMgr->GetPlayerData().iMp = (m_pUIMgr->GetPlayerData().iMaxMp += 2);
		m_pUIMgr->GetPlayerData().iSp = (m_pUIMgr->GetPlayerData().iMaxSp += 10);

		m_pUIMgr->GetPlayerIdentity().iPoint -= m_iNeedAgi;
	}

	if (nullptr != pPlayer)
		pPlayer->UpdateData();

	UpdateState();
}

void CInfoWnd::LClickUp()
{
}

void CInfoWnd::RClickDown()
{
}

void CInfoWnd::UpdateState()
{
	tStabBtn.bIsShow = (m_pUIMgr->GetPlayerIdentity().iPoint >= m_iNeedStab);
	tHackBtn.bIsShow = (m_pUIMgr->GetPlayerIdentity().iPoint >= m_iNeedHack);
	tIntBtn.bIsShow = (m_pUIMgr->GetPlayerIdentity().iPoint >= m_iNeedInt);
	tDefBtn.bIsShow = (m_pUIMgr->GetPlayerIdentity().iPoint >= m_iNeedDef);
	tMrBtn.bIsShow = (m_pUIMgr->GetPlayerIdentity().iPoint >= m_iNeedMr);
	tDexBtn.bIsShow = (m_pUIMgr->GetPlayerIdentity().iPoint >= m_iNeedDex);
	tAgiBtn.bIsShow = (m_pUIMgr->GetPlayerIdentity().iPoint >= m_iNeedAgi);
}
