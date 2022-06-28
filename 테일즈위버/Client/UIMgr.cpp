#include "stdafx.h"
#include "UIMgr.h"

#include "PlayerObserver.h"

#include "EventObject.h"

#include "Player.h"

#include "ItemEquipData.h"
#include "ItemUsableData.h"

#include "InfoWnd.h"
#include "EquipWnd.h"
#include "InvenWnd.h"
#include "SkillWnd.h"
#include "ChatWnd.h"
#include "QuickWnd.h"
#include "ShopWnd.h"
#include "ComboWnd.h"
#include "ComboSetWnd.h"
#include "MiniMapWnd.h"
#include "CountWnd.h"

IMPLEMENT_SINGLETON(CUIMgr)

CUIMgr::CUIMgr()
	:	m_pDeviceMgr(CDeviceMgr::GetInstance()),
		m_pTextureMgr(CTextureMgr::GetInstance()),
		m_pViewMgr(CViewMgr::GetInstance()),
		m_pMouseMgr(CMouseMgr::GetInstance()),
		m_pKeyMgr(CKeyMgr::GetInstance()),
		m_pSoundMgr(CSoundMgr::GetInstance()),
		m_pSubject(CPlayerSubject::GetInstance())		
{
}

CUIMgr::~CUIMgr()
{
	Release();
}

HRESULT CUIMgr::Initialize()
{
	LoadExpData(L"../Data/ExpData/ExpData.txt");

	m_tChatAlarm.SetAlarm(60.f, true);

	if (nullptr == m_pObserver)
	{
		m_pObserver = CPlayerObserver::Create();
		m_pSubject->Subscribe(m_pObserver);
	}

	if (nullptr == m_pInfoWnd)
	{
		m_pInfoWnd = new CInfoWnd;
		m_pInfoWnd->Initialize();
	}

	if (nullptr == m_pInvenWnd)
	{
		m_pInvenWnd = new CInvenWnd;
		m_pInvenWnd->Initialize();
	}

	if (nullptr == m_pEquipWnd)
	{
		m_pEquipWnd = new CEquipWnd;
		m_pEquipWnd->Initialize();
	}

	if (nullptr == m_pSkillWnd)
	{
		m_pSkillWnd = new CSkillWnd;
		m_pSkillWnd->Initialize();
	}

	if (nullptr == m_pComboWnd)
	{
		m_pComboWnd = new CComboWnd;
		m_pComboWnd->Initialize();
	}

	if (nullptr == m_pComboSetWnd)
	{
		m_pComboSetWnd = new CComboSetWnd;
		m_pComboSetWnd->Initialize();
	}

	if (nullptr == m_pChatWnd)
	{
		m_pChatWnd = new CChatWnd;
		m_pChatWnd->Initialize();
	}

	if (nullptr == m_pQuickWnd)
	{
		m_pQuickWnd = new CQuickWnd;
		m_pQuickWnd->Initialize();
	}

	if (nullptr == m_pShopWnd)
	{
		m_pShopWnd = new CShopWnd;
		m_pShopWnd->Initialize();
	}

	if (nullptr == m_pMiniMapWnd)
	{
		m_pMiniMapWnd = new CMiniMapWnd;
		m_pMiniMapWnd->Initialize();
	}

	if (nullptr == m_pCountWnd)
	{
		m_pCountWnd = new CCountWnd;
		m_pCountWnd->Initialize();
	}


	// 왼쪽 상단 미니 정보창
	m_pMiniInfoTex	= m_pTextureMgr->GetTexInfo(L"UI", L"MiniInfo");	
	m_pHpTex		= m_pTextureMgr->GetTexInfo(L"UI", L"HpBar");		// 왼쪽 상단 Hp바
	m_pMpTex		= m_pTextureMgr->GetTexInfo(L"UI", L"MpBar");		// 왼쪽 상단 Mp바
	m_pSpTex		= m_pTextureMgr->GetTexInfo(L"UI", L"SpBar");		// 왼쪽 상단 Sp바
	if (nullptr != m_pMiniInfoTex){
		m_tMiniInfo.vPos	= { 0.f, 0.f, 0.f };
		m_tMiniInfo.vSize	= { float(m_pMiniInfoTex->tImgInfo.Width), float(m_pMiniInfoTex->tImgInfo.Height), 0.f };
		m_tMiniInfo.vOffset = m_pMiniInfoTex->vOffset;
	}
	
	// 상태 버튼
	m_pInfoBarTex	= m_pTextureMgr->GetTexInfo(L"Button", L"InfoBar", 0);	
	if (nullptr != m_pInfoBarTex) {
		m_tInfoBar.vPos = { float(CLIENTCX) - float(m_pInfoBarTex->tImgInfo.Width), 0.f, 0.f };
		m_tInfoBar.vSize = { float(m_pInfoBarTex->tImgInfo.Width), float(m_pInfoBarTex->tImgInfo.Height), 0.f };
		m_tInfoBar.vOffset = m_pInfoBarTex->vOffset;
	}

	// 콤보 버튼
	m_pComboBarTex = m_pTextureMgr->GetTexInfo(L"Button", L"ComboBar", 0);
	if (nullptr != m_pComboBarTex) {
		m_tComboBar.vPos = { float(CLIENTCX) - float(m_pComboBarTex->tImgInfo.Width), m_pComboBarTex->tImgInfo.Height * 1.f, 0.f };
		m_tComboBar.vSize = { float(m_pComboBarTex->tImgInfo.Width), float(m_pComboBarTex->tImgInfo.Height), 0.f };
		m_tComboBar.vOffset = m_pComboBarTex->vOffset;
	}
		
	// 장비 버튼
	m_pEquipBarTex = m_pTextureMgr->GetTexInfo(L"Button", L"EqiupBar", 0);
	if (nullptr != m_pEquipBarTex) {
		m_tEquipBar.vPos = { float(CLIENTCX) - float(m_pInfoBarTex->tImgInfo.Width), m_pEquipBarTex->tImgInfo.Height * 2.f, 0.f };
		m_tEquipBar.vSize = { float(m_pEquipBarTex->tImgInfo.Width), float(m_pEquipBarTex->tImgInfo.Height), 0.f };
		m_tEquipBar.vOffset = m_pEquipBarTex->vOffset;
	}

	// 아이템창 버튼
	m_pInvenBarTex	= m_pTextureMgr->GetTexInfo(L"Button", L"InvenBar", 0);	
	if (nullptr != m_pInvenBarTex) {
		m_tInvenBar.vPos = { float(CLIENTCX) - float(m_pInfoBarTex->tImgInfo.Width), m_pInvenBarTex->tImgInfo.Height * 3.f, 0.f };
		m_tInvenBar.vSize = { float(m_pInvenBarTex->tImgInfo.Width), float(m_pInvenBarTex->tImgInfo.Height), 0.f };
		m_tInvenBar.vOffset = m_pInvenBarTex->vOffset;
	}

	// 스킬 버튼
	m_pSkillBarTex = m_pTextureMgr->GetTexInfo(L"Button", L"SkillBar", 0);
	if (nullptr != m_pEquipBarTex) {
		m_tSkillBar.vPos = { float(CLIENTCX) - float(m_pInfoBarTex->tImgInfo.Width), m_pSkillBarTex->tImgInfo.Height * 4.f, 0.f };
		m_tSkillBar.vSize = { float(m_pSkillBarTex->tImgInfo.Width), float(m_pSkillBarTex->tImgInfo.Height), 0.f };
		m_tSkillBar.vOffset = m_pSkillBarTex->vOffset;
	}

	// 확장 버튼
	m_pExpansionTex = m_pTextureMgr->GetTexInfo(L"Button", L"Expansion", 0);
	if (nullptr != m_pExpansionTex) {
		m_tExpansion.vPos = { float(CLIENTCX) - float(m_pExpansionTex->tImgInfo.Width), 255.f, 0.f };
		m_tExpansion.vSize = { float(m_pExpansionTex->tImgInfo.Width), float(m_pExpansionTex->tImgInfo.Height), 0.f };
		m_tExpansion.vOffset = m_pExpansionTex->vOffset;
	}

	// 아이템 샵
	m_pItemShopTex = m_pTextureMgr->GetTexInfo(L"UI", L"ItemShop");
	if (nullptr != m_pItemShopTex) {
		m_tItemShop.vPos = { 135.f, float(CLIENTCY - m_pItemShopTex->tImgInfo.Height), 0.f };
		m_tItemShop.vSize = { float(m_pItemShopTex->tImgInfo.Width), float(m_pItemShopTex->tImgInfo.Height), 0.f };
		m_tItemShop.vOffset = m_pItemShopTex->vOffset;
	}

	return S_OK;
}

void CUIMgr::Update()
{
	m_tPlayerData		= m_pObserver->GetPlayerUnitData();
	m_tPlayerIdentity	= m_pObserver->GetPlayerIdentity();
	
	if (m_tChatAlarm.CheckAlarm())
	{
		int iChatRand = rand() % 3;
		switch (iChatRand) {
		case 0:InsertChat(L"", L"<System> 건전한 채팅문화를 만듭시다.");
			break;

		case 1:InsertChat(L"???  ", L"  병호로 산다는건 어떤 기분일까..");
			break;

		case 2:InsertChat(L"예찬킹 ", L"  5252 믿고있었다구!!");
			break;
		}
	}

	D3DXVECTOR3 vMouse = m_pMouseMgr->GetMouse();
	if (m_bIsExpansion)
	{
		if (0 == m_tInfoBar.iHover && m_tInfoBar.IsRectInPoint(vMouse))
			m_pSoundMgr->PlaySound(L"barover.MP3", CHANNEL_ID::CHANNEL_WINDOW);
		if (0 == m_tInvenBar.iHover && m_tInvenBar.IsRectInPoint(vMouse))
			m_pSoundMgr->PlaySound(L"barover.MP3", CHANNEL_ID::CHANNEL_WINDOW);
		if (0 == m_tEquipBar.iHover && m_tEquipBar.IsRectInPoint(vMouse))
			m_pSoundMgr->PlaySound(L"barover.MP3", CHANNEL_ID::CHANNEL_WINDOW);
		if (0 == m_tSkillBar.iHover && m_tSkillBar.IsRectInPoint(vMouse))
			m_pSoundMgr->PlaySound(L"barover.MP3", CHANNEL_ID::CHANNEL_WINDOW);
		if (0 == m_tComboBar.iHover && m_tComboBar.IsRectInPoint(vMouse))
			m_pSoundMgr->PlaySound(L"barover.MP3", CHANNEL_ID::CHANNEL_WINDOW);
	}
	if (0 == m_tExpansion.iHover && m_tExpansion.IsRectInPoint(vMouse))
		m_pSoundMgr->PlaySound(L"barover.MP3", CHANNEL_ID::CHANNEL_WINDOW);


	KeyInput();

	m_pInfoWnd->Update();
	m_pEquipWnd->Update();
	m_pInvenWnd->Update();
	m_pSkillWnd->Update();
	m_pShopWnd->Update();
	m_pComboWnd->Update();
	m_pComboSetWnd->Update();
	m_pChatWnd->Update();
	m_pQuickWnd->Update();
	m_pMiniMapWnd->Update();
	m_pCountWnd->Update();
}

void CUIMgr::Render()
{
	m_pDeviceMgr->GetSprite()->End();
	m_pDeviceMgr->GetDevice()->EndScene();

	m_pDeviceMgr->GetDevice()->BeginScene();
	m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
	//////////////////////////////////////////////////////////////////////////////////

	D3DXMATRIX matScale, matTrans, matWorld;
	
	// 왼쪽 상단
	if (nullptr != m_pMiniInfoTex) {
		// 왼쪽 상단
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tMiniInfo.vPos.x, m_tMiniInfo.vPos.y, 0.f);
		matWorld = matScale * matTrans;

		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pMiniInfoTex->pTexture, nullptr,
			&m_tMiniInfo.vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		// 페이스
		const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(L"UI", L"KyokoFaceIcon");
		if (nullptr != pTexInfo)
		{
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
			D3DXMatrixTranslation(&matTrans, 26.f, 17.f, 0.f);
			matWorld = matScale * matTrans;
			m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
			m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &pTexInfo->vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		// 신발
		pTexInfo = m_pTextureMgr->GetTexInfo(L"UI", L"WalkIcon", int(m_tPlayerIdentity.bIsRun));
		if (nullptr != pTexInfo)
		{
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
			D3DXMatrixTranslation(&matTrans, 26.f, 54.f, 0.f);
			matWorld = matScale * matTrans;
			m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
			m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr, &pTexInfo->vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		RECT rt;

		// 왼쪽 상단 Hp바
		if (nullptr != m_pHpTex) {
			D3DXMatrixScaling(&matScale, float(m_tPlayerData.iHp) / m_tPlayerData.iMaxHp, 1.f, 0.f);
			D3DXMatrixTranslation(&matTrans, 93.f, 16.f, 0.f);
			matWorld = matScale * matTrans;

			m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
			m_pDeviceMgr->GetSprite()->Draw(m_pHpTex->pTexture, nullptr,
				&D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

			// HP 폰트
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
			matWorld = matScale;
			m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
			SetRect(&rt, 93, 18, 93 + int(m_pHpTex->tImgInfo.Width), 18 + int(m_pHpTex->tImgInfo.Height));
			m_pDeviceMgr->DrawFont(to_wstring(m_tPlayerData.iHp) + L"/" + to_wstring(m_tPlayerData.iMaxHp), &rt, DT_CENTER);
		}


		// 왼쪽 상단 Mp바
		if (nullptr != m_pMpTex) {
			D3DXMatrixScaling(&matScale, float(m_tPlayerData.iMp) / m_tPlayerData.iMaxMp, 1.f, 0.f);
			D3DXMatrixTranslation(&matTrans, 93.f, 31.f, 0.f);
			matWorld = matScale * matTrans;

			m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
			m_pDeviceMgr->GetSprite()->Draw(m_pMpTex->pTexture, nullptr,
				&D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

			// MP 폰트
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
			matWorld = matScale;
			m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
			SetRect(&rt, 93, 33, 93 + int(m_pMpTex->tImgInfo.Width), 33 + int(m_pMpTex->tImgInfo.Height));
			m_pDeviceMgr->DrawFont(to_wstring(m_tPlayerData.iMp) + L"/" + to_wstring(m_tPlayerData.iMaxMp), &rt, DT_CENTER);
		}


		// 왼쪽 상단 Sp바
		if (nullptr != m_pSpTex) {
			D3DXMatrixScaling(&matScale, float(m_tPlayerData.iSp) / m_tPlayerData.iMaxSp, 1.f, 0.f);
			D3DXMatrixTranslation(&matTrans, 93.f, 46.f, 0.f);
			matWorld = matScale * matTrans;

			m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
			m_pDeviceMgr->GetSprite()->Draw(m_pSpTex->pTexture, nullptr,
				&D3DXVECTOR3(0.f, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

			// SP 폰트
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
			matWorld = matScale;
			m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
			SetRect(&rt, 93, 48, 93 + int(m_pSpTex->tImgInfo.Width), 48 + int(m_pSpTex->tImgInfo.Height));
			m_pDeviceMgr->DrawFont(to_wstring(m_tPlayerData.iSp) + L"/" + to_wstring(m_tPlayerData.iMaxSp), &rt, DT_CENTER);
		}
		//DrawRect(m_tMiniInfo);
	}

	// 채널
	{
		RECT rt;
		// 우측 상단 서버명
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		matWorld = matScale;
		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		SetRect(&rt, CLIENTCX / 4 * 3, 0, CLIENTCX - 18, 12);
		m_pDeviceMgr->DrawFont(L"Client v3.94 트라바체스(Travaches)", &rt, DT_LEFT);

		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		SetRect(&rt, CLIENTCX / 4 * 3, 12, CLIENTCX - 18, 24);
		m_pDeviceMgr->DrawFont(L"[10월22일15시46분]", &rt, DT_LEFT);
	}


	// 우측 상단 버튼
	if (m_bIsExpansion)
	{
		// 스탯창 버튼
		if (nullptr != m_pInfoBarTex)
		{
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
			D3DXMatrixTranslation(&matTrans, m_tInfoBar.vPos.x, m_tInfoBar.vPos.y, 0.f);
			matWorld = matScale * matTrans;

			m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
			m_pDeviceMgr->GetSprite()->Draw(m_pInfoBarTex->pTexture, nullptr,
				&m_tInfoBar.vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

			//DrawRect(m_tInfoBar);
		}

		// 콤보창 버튼
		if (nullptr != m_pComboBarTex)
		{
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
			D3DXMatrixTranslation(&matTrans, m_tComboBar.vPos.x, m_tComboBar.vPos.y, 0.f);
			matWorld = matScale * matTrans;

			m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
			m_pDeviceMgr->GetSprite()->Draw(m_pComboBarTex->pTexture, nullptr,
				&m_tComboBar.vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

			//DrawRect(m_tEquipBar);
		}

		// 장비 버튼
		if (nullptr != m_pEquipBarTex)
		{
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
			D3DXMatrixTranslation(&matTrans, m_tEquipBar.vPos.x, m_tEquipBar.vPos.y, 0.f);
			matWorld = matScale * matTrans;

			m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
			m_pDeviceMgr->GetSprite()->Draw(m_pEquipBarTex->pTexture, nullptr,
				&m_tEquipBar.vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

			//DrawRect(m_tEquipBar);
		}

		// 아이템창 버튼
		if (nullptr != m_pInvenBarTex)
		{
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
			D3DXMatrixTranslation(&matTrans, m_tInvenBar.vPos.x, m_tInvenBar.vPos.y, 0.f);
			matWorld = matScale * matTrans;

			m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
			m_pDeviceMgr->GetSprite()->Draw(m_pInvenBarTex->pTexture, nullptr,
				&m_tInvenBar.vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

			//DrawRect(m_tInvenBar);
		}

		// 스킬창 버튼
		if (nullptr != m_pSkillBarTex)
		{
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
			D3DXMatrixTranslation(&matTrans, m_tSkillBar.vPos.x, m_tSkillBar.vPos.y, 0.f);
			matWorld = matScale * matTrans;

			m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
			m_pDeviceMgr->GetSprite()->Draw(m_pSkillBarTex->pTexture, nullptr,
				&m_tSkillBar.vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

			//DrawRect(m_tInvenBar);
		}
	}

	// 확장 버튼
	if (nullptr != m_pExpansionTex)
	{
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tExpansion.vPos.x, m_tExpansion.vPos.y, 0.f);
		matWorld = matScale * matTrans;

		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pExpansionTex->pTexture, nullptr,
			&m_tExpansion.vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		//DrawRect(m_tInvenBar);
	}

	// 좌측상단 아이템 샵
	if (nullptr != m_pItemShopTex)
	{
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tItemShop.vPos.x, m_tItemShop.vPos.y, 0.f);
		matWorld = matScale * matTrans;

		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pItemShopTex->pTexture, nullptr, &m_tItemShop.vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		//DrawRect(m_tInvenBar);
	}

	if (nullptr != m_pQuickWnd)
		m_pQuickWnd->Render();

	if (nullptr != m_pChatWnd)
		m_pChatWnd->Render();

	if (nullptr != m_pMiniMapWnd)
		m_pMiniMapWnd->Render();

	auto iter_find = m_vecWndRender.begin();
	for (; iter_find != m_vecWndRender.end();)
	{
		bool bIsFind = false;
		switch (*iter_find)
		{
		case WND_ID::WNDID_INFO:
			if (!m_pInfoWnd->IsShow())
			{
				iter_find = m_vecWndRender.erase(iter_find);
				bIsFind = true;
			}
			break;

		case WND_ID::WNDID_EQUIP:
			if (!m_pEquipWnd->IsShow())
			{
				iter_find = m_vecWndRender.erase(iter_find);
				bIsFind = true;
			}
			break;

		case WND_ID::WNDID_INVEN:
			if (!m_pInvenWnd->IsShow()) 
			{
				iter_find = m_vecWndRender.erase(iter_find);
				bIsFind = true;
			}
			break;

		case WND_ID::WNDID_SKILL:
			if (!m_pSkillWnd->IsShow()) 
			{
				iter_find = m_vecWndRender.erase(iter_find);
				bIsFind = true;
			}
			break;

		case WND_ID::WNDID_SHOP:
			if (!m_pShopWnd->IsShow())
			{
				iter_find = m_vecWndRender.erase(iter_find);
				bIsFind = true;
			}
			break;

		case WND_ID::WNDID_COMBO:
			if (!m_pComboWnd->IsShow())
			{
				iter_find = m_vecWndRender.erase(iter_find);
				bIsFind = true;
			}
			break;
		}

		if (!bIsFind)
			++iter_find;
	}

	for (auto& Wnd : m_vecWndRender)
	{
		switch (Wnd)
		{
		case WND_ID::WNDID_INFO:	m_pInfoWnd->Render();	break;
		case WND_ID::WNDID_EQUIP:	m_pEquipWnd->Render();	break;
		case WND_ID::WNDID_INVEN:	m_pInvenWnd->Render();	break;
		case WND_ID::WNDID_SKILL:	m_pSkillWnd->Render();	break;
		case WND_ID::WNDID_SHOP:	m_pShopWnd->Render();	break;
		case WND_ID::WNDID_COMBO:	m_pComboWnd->Render();	break;
		}
	}

	if (nullptr != m_pComboSetWnd)
		m_pComboSetWnd->Render();

	if (nullptr != m_pCountWnd)
		m_pCountWnd->Render();

	//////////////////////////////////////////////////////////////////////////////////
	m_pDeviceMgr->GetSprite()->End();
	m_pDeviceMgr->GetDevice()->EndScene();

	m_pDeviceMgr->GetDevice()->BeginScene();
	m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
}

void CUIMgr::Release()
{
	SafeDelete(m_pInfoWnd);
	SafeDelete(m_pEquipWnd);
	SafeDelete(m_pInvenWnd);
	SafeDelete(m_pSkillWnd);
	SafeDelete(m_pChatWnd);
	SafeDelete(m_pQuickWnd);
	SafeDelete(m_pShopWnd);
	SafeDelete(m_pComboWnd);
	SafeDelete(m_pComboSetWnd);
	SafeDelete(m_pMiniMapWnd);
	SafeDelete(m_pCountWnd);

	for (auto& ExpData : m_vecExpData)
		SafeDelete(ExpData);
	m_vecExpData.clear();
	m_vecExpData.shrink_to_fit();

	m_pSubject->UnSubscribe(m_pObserver);
	SafeDelete(m_pObserver);
}

HRESULT CUIMgr::LoadExpData(const wstring & wstrFilePath)
{
	wifstream LoadFile;

	LoadFile.open(wstrFilePath.c_str(), ios::in);

	if (!LoadFile.good())
		return E_FAIL;

	for (auto& ExpData : m_vecExpData)
		SafeDelete(ExpData);
	m_vecExpData.clear();
	m_vecExpData.shrink_to_fit();

	TCHAR szMinExp[MAX_STR] = L"";
	TCHAR szMaxExp[MAX_STR] = L"";
	TCHAR szAccumulateExp[MAX_STR] = L"";
	TCHAR szSkillPoint[MAX_STR] = L"";

	EXP_DATA*	pExpData = nullptr;

	while (!LoadFile.eof()) {
		LoadFile.getline(szMinExp, MAX_STR, '|');
		LoadFile.getline(szMaxExp, MAX_STR, '|');
		LoadFile.getline(szAccumulateExp, MAX_STR, '|');
		LoadFile.getline(szSkillPoint, MAX_STR);

		pExpData = new EXP_DATA;
		pExpData->iMinExp = int(_wtoi(szMinExp));
		pExpData->iMaxExp = int(_wtoi(szMaxExp));
		pExpData->iAccumulateExp = int(_wtoi(szAccumulateExp));
		pExpData->iSkillPoint = int(_wtoi(szSkillPoint));

		m_vecExpData.push_back(pExpData);
	}

	LoadFile.close();

	return S_OK;
}


void CUIMgr::ShowShop()
{
	int iRand = rand() % 2;
	wstring wstrSound = (0 == iRand) ? L"BenyaClick0.wav" : L"BenyaClick1.wav";
	m_pSoundMgr->PlaySound(wstrSound.c_str(), CHANNEL_ID::CHANNEL_WINDOW);

	m_pShopWnd->ShowWindow(true);
	m_vecWndRender.push_back(WND_ID::WNDID_SHOP);
}

void CUIMgr::SetDead(bool bIsDead)
{
	m_bIsDead = bIsDead;
}

void CUIMgr::AddEquipStat(STAT_INFO* pStatInfo)
{
	NULL_CHECK(pStatInfo);
	pStatInfo->Reset();
	dynamic_cast<CEquipWnd*>(m_pEquipWnd)->AddStat(pStatInfo);
}

// 경험치 상승
void CUIMgr::AddExp(int iAddExp)
{
	m_tPlayerData		= m_pObserver->GetPlayerUnitData();
	m_tPlayerIdentity	= m_pObserver->GetPlayerIdentity();

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObjMgr::GetInstance()->GetPlayer());

	int iLevelUp = (m_tPlayerData.iExp + iAddExp) - m_tPlayerIdentity.iMaxExp;
	dynamic_cast<CChatWnd*>(m_pChatWnd)->InsertChat(new CHAT_DATA(L"", wstring(L"경험치가") + to_wstring(iAddExp) + L" 올랐습니다.", D3DCOLOR_ARGB(255, 238, 239, 163)));

	if (0 <= iLevelUp)
	{
		if (m_tPlayerData.iLevel + 1 >= int(m_vecExpData.size()))
			return;

		m_tPlayerData.iExp = m_vecExpData[m_tPlayerData.iLevel]->iMinExp;

		m_pSoundMgr->PlaySound(L"lvup.MP3", CHANNEL_ID::CHANNEL_EFFECT);

		++m_tPlayerData.iLevel;
		
		m_tPlayerData.iHp = m_tPlayerData.iMaxHp;
		m_tPlayerData.iMp = m_tPlayerData.iMaxMp;
		m_tPlayerData.iSp = m_tPlayerData.iMaxSp;

		m_tPlayerIdentity.iMinExp	= m_vecExpData[m_tPlayerData.iLevel]->iMinExp;
		m_tPlayerIdentity.iMaxExp	= m_vecExpData[m_tPlayerData.iLevel]->iMaxExp;
		m_tPlayerIdentity.iPoint	+= m_vecExpData[m_tPlayerData.iLevel]->iSkillPoint;

		dynamic_cast<CInfoWnd*>(m_pInfoWnd)->UpdateState();
		dynamic_cast<CChatWnd*>(m_pChatWnd)->InsertChat(new CHAT_DATA(L"", L"레벨이 올랐습니다!", D3DCOLOR_ARGB(255, 238, 239, 163)));

		pPlayer->CreateEffect(pPlayer->GetInfo().vPos, D3DXVECTOR3{ 0.f, 0.f, 0.f }, L"Effect", L"LevelUpEffect", false, OBJ_EFFECT, pPlayer, nullptr, FRAME_ONCE, true);
		pPlayer->CreateEffect(pPlayer->GetInfo().vPos, D3DXVECTOR3{ 0.f, 0.f, 0.f }, L"Effect", L"LevelUp", false, OBJ_EFFECT, pPlayer, nullptr, FRAME_ONCE, true);
	}
	else
		m_tPlayerData.iExp += iAddExp;

	pPlayer->UpdateData();
}

void CUIMgr::AddHealth(int iHp, int iMp, int iSp)
{
	m_tPlayerData = m_pObserver->GetPlayerUnitData();
	m_tPlayerIdentity = m_pObserver->GetPlayerIdentity();
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObjMgr::GetInstance()->GetPlayer());

	m_tPlayerData.iHp += iHp;
	if (m_tPlayerData.iMaxHp < m_tPlayerData.iHp)
		m_tPlayerData.iHp = m_tPlayerData.iMaxHp;

	m_tPlayerData.iMp += iMp;
	if (m_tPlayerData.iMaxMp < m_tPlayerData.iMp)
		m_tPlayerData.iMp = m_tPlayerData.iMaxMp;

	m_tPlayerData.iSp += iSp;
	if (m_tPlayerData.iMaxSp < m_tPlayerData.iSp)
		m_tPlayerData.iSp = m_tPlayerData.iMaxSp;

	pPlayer->UpdateData();
}



void CUIMgr::DrawRect(CWndInfo& tInfo)
{
	m_pDeviceMgr->GetSprite()->End();
	m_pDeviceMgr->GetDevice()->EndScene();

	m_pDeviceMgr->GetDevice()->BeginScene();
	m_pDeviceMgr->GetLine()->Begin();

	//m_pViewMgr->Render();

	D3DXVECTOR2 pLine[5] = {
		{ tInfo.vParentPos.x + tInfo.vPos.x - tInfo.vOffset.x, tInfo.vParentPos.y + tInfo.vPos.y - tInfo.vOffset.y},
		{ tInfo.vParentPos.x + tInfo.vPos.x + (tInfo.vSize.x - tInfo.vOffset.x), tInfo.vParentPos.y + tInfo.vPos.y - tInfo.vOffset.y},
		{ tInfo.vParentPos.x + tInfo.vPos.x + (tInfo.vSize.x - tInfo.vOffset.x), tInfo.vParentPos.y + tInfo.vPos.y + (tInfo.vSize.y - tInfo.vOffset.y)},
		{ tInfo.vParentPos.x + tInfo.vPos.x - tInfo.vOffset.x, tInfo.vParentPos.y + tInfo.vPos.y + (tInfo.vSize.y - tInfo.vOffset.y) },
		{ tInfo.vParentPos.x + tInfo.vPos.x - tInfo.vOffset.x, tInfo.vParentPos.y + tInfo.vPos.y - tInfo.vOffset.y}
	};

	m_pDeviceMgr->GetLine()->Draw(pLine, 5, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_pDeviceMgr->GetLine()->End();
	m_pDeviceMgr->GetDevice()->EndScene();

	m_pDeviceMgr->GetDevice()->BeginScene();
	m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
}


void CUIMgr::AddItem(WND_ID eWndID, CItemData* pData)
{
	NULL_CHECK(pData);

	switch (eWndID)
	{
	case WNDID_EQUIP:
		if (nullptr != m_pEquipWnd)
			dynamic_cast<CEquipWnd*>(m_pEquipWnd)->SetItem(reinterpret_cast<CItemData*>(pData)->eItemID, reinterpret_cast<CItemData*>(pData));
		break;

	case WNDID_INVEN:
		if (nullptr != m_pInvenWnd)
			dynamic_cast<CInvenWnd*>(m_pInvenWnd)->AddItem(reinterpret_cast<CItemData*>(pData));

	case WNDID_SHOP:
		if (nullptr != m_pShopWnd)
			dynamic_cast<CShopWnd*>(m_pShopWnd)->AddItem(reinterpret_cast<CItemData*>(pData));
		break;
	}

}

void CUIMgr::AddItem(WND_ID eWndID, wstring wstrName, int iPrice)
{
	CItemData* pItem = CObjMgr::GetInstance()->GetItemData(wstrName);

	if (nullptr == pItem)
		return;

	CItemData* pItemData = nullptr;
	switch (pItem->eItemType)
	{
	case ITEM_TYPE::ITEM_EQUIP:
		pItemData = new CItemEquipData(*(dynamic_cast<CItemEquipData*>(pItem)));
		break;

	case ITEM_TYPE::ITEM_USE:
		pItemData = new CItemUsableData(*(dynamic_cast<CItemUsableData*>(pItem)));
		break;

	case ITEM_TYPE::ITEM_ETC:
		pItemData = new CItemData(*(dynamic_cast<CItemData*>(pItem)));
		break;
	}

	if (nullptr != pItemData)
	{
		if (0 <= iPrice)
			pItemData->SetPrice(iPrice);

		switch (eWndID)
		{
		case WNDID_EQUIP:
			if (nullptr != m_pEquipWnd)
				dynamic_cast<CEquipWnd*>(m_pEquipWnd)->SetItem(pItemData->eItemID, pItemData);
			break;

		case WNDID_INVEN:
			if (nullptr != m_pInvenWnd)
				dynamic_cast<CInvenWnd*>(m_pInvenWnd)->AddItem(pItemData);

		case WNDID_SHOP:
			if (nullptr != m_pShopWnd)
				dynamic_cast<CShopWnd*>(m_pShopWnd)->AddItem(pItemData);
			break;
		}
	}
}

void CUIMgr::InsertChat(const wstring & wstrName, const wstring & wstrChat, DWORD dwFontColor)
{
	dynamic_cast<CChatWnd*>(m_pChatWnd)->InsertChat(new CHAT_DATA(wstrName, wstrChat, dwFontColor));
}

void CUIMgr::UpdatePlayerNotify()
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObjMgr::GetInstance()->GetPlayer());
	if (nullptr != pPlayer)
	{
		pPlayer->UpdateData();
	}
}

CUIWnd * CUIMgr::GetWindow(WND_ID eWndID)
{
	switch (eWndID)
	{
		case WND_ID::WNDID_QUICK:		return m_pQuickWnd;
		case WND_ID::WNDID_INFO:		return m_pInfoWnd;
		case WND_ID::WNDID_EQUIP:		return m_pEquipWnd;
		case WND_ID::WNDID_INVEN:		return m_pInvenWnd;
		case WND_ID::WNDID_SKILL:		return m_pSkillWnd;
		case WND_ID::WNDID_CHAT:		return m_pChatWnd;
		case WND_ID::WNDID_SHOP:		return m_pShopWnd;
		case WND_ID::WNDID_COMBO:		return m_pComboWnd;
		case WND_ID::WNDID_COMBOSET:	return m_pComboSetWnd;
		case WND_ID::WNDID_COUNT:		return m_pCountWnd;
	}

	return nullptr;
}

void CUIMgr::MoveItem(WND_ID eFrom, WND_ID eTo, int iItemSlotIndex, int iCount)
{
	if (eFrom == WND_ID::WNDID_QUICK)
	{
		dynamic_cast<CQuickWnd*>(m_pQuickWnd)->DeleteItem(iItemSlotIndex);
		m_pMouseMgr->SetItemData(WND_ID::WNDID_END, -1, nullptr);
	}

	else if (eFrom == WND_ID::WNDID_INVEN || eFrom == WND_ID::WNDID_EQUIP)
	{
		CItemData* pItem = nullptr;
		if (eFrom == WND_ID::WNDID_INVEN)
			pItem = dynamic_cast<CInvenWnd*>(m_pInvenWnd)->GetItem(iItemSlotIndex);
		else if (eFrom == WND_ID::WNDID_EQUIP)
			pItem = dynamic_cast<CEquipWnd*>(m_pEquipWnd)->GetItem(iItemSlotIndex);

		if (nullptr != pItem)
		{
			switch (eTo)
			{
			case WND_ID::WNDID_INVEN:
			{
				CItemData* pItemData = nullptr;
				switch (pItem->eItemType)
				{
				case ITEM_TYPE::ITEM_EQUIP:
					pItemData = new CItemEquipData(*(dynamic_cast<CItemEquipData*>(pItem)));
					break;

				case ITEM_TYPE::ITEM_USE:
					pItemData = new CItemUsableData(*(dynamic_cast<CItemUsableData*>(pItem)));
					break;

				case ITEM_TYPE::ITEM_ETC:
					pItemData = new CItemData(*(dynamic_cast<CItemData*>(pItem)));
					break;
				}

				if (nullptr != pItemData)
				{
					pItemData->SetPrice(pItem->GetPrice());
					dynamic_cast<CInvenWnd*>(m_pInvenWnd)->AddItem(pItemData);
				}
				m_pMouseMgr->SetItemData(WND_ID::WNDID_END, -1, nullptr);
			}
				break;

			case WND_ID::WNDID_EQUIP:
				if (ITEM_TYPE::ITEM_EQUIP != pItem->eItemType
					|| ITEM_TYPE::ITEM_EQUIP == pItem->eItemType && ITEMID::ITEMID_ACCESSOAY4 < pItem->eItemID)
				{
					m_pMouseMgr->SetItemData(WND_ID::WNDID_END, -1, nullptr);
					return;
				}
				else if (ITEM_TYPE::ITEM_EQUIP == pItem->eItemType)
				{
					CItemData* pItemData = new CItemEquipData(*(dynamic_cast<CItemEquipData*>(pItem)));
					pItemData->SetPrice(pItem->GetPrice());
					dynamic_cast<CEquipWnd*>(m_pEquipWnd)->SetItem(pItem->eItemID, pItemData);
					m_pMouseMgr->SetItemData(WND_ID::WNDID_END, -1, nullptr);
				}
				
				break;
			}

			if (eFrom == WND_ID::WNDID_INVEN)
				dynamic_cast<CInvenWnd*>(m_pInvenWnd)->DeleteItem(iItemSlotIndex, iCount);
			else if (eFrom == WND_ID::WNDID_EQUIP)
				dynamic_cast<CEquipWnd*>(m_pEquipWnd)->DeleteItem(iItemSlotIndex);
		}
	}
}

void CUIMgr::KeyInput()
{
	D3DXVECTOR3 vMouse = m_pMouseMgr->GetMouse();
	m_bIsMouseHover = false;

	if (m_tMiniInfo.IsRectInPoint(vMouse))
		m_bIsMouseHover = true;

	// 채팅 중이 아닐 때만
	if (!m_bIsChat)
	{
		if (m_pKeyMgr->KeyDown(KEY_WND_INFO))
		{
			m_pInfoWnd->ShowWindow(!m_pInfoWnd->IsShow());
			if (m_pInfoWnd->IsShow())
				m_pSoundMgr->PlaySound(L"wopen.MP3", CHANNEL_ID::CHANNEL_WINDOW);
			else
				m_pSoundMgr->PlaySound(L"CloseWnd.wav", CHANNEL_ID::CHANNEL_WINDOW);
			m_vecWndRender.push_back(WND_ID::WNDID_INFO);
		}

		if (m_pKeyMgr->KeyDown(KEY_WND_INVEN))
		{
			m_pInvenWnd->ShowWindow(!m_pInvenWnd->IsShow());
			if (m_pInvenWnd->IsShow())
				m_pSoundMgr->PlaySound(L"wopen.MP3", CHANNEL_ID::CHANNEL_WINDOW);
			else
				m_pSoundMgr->PlaySound(L"CloseWnd.wav", CHANNEL_ID::CHANNEL_WINDOW);
			m_vecWndRender.push_back(WND_ID::WNDID_INVEN);
		}

		if (m_pKeyMgr->KeyDown(KEY_WND_EQUIP))
		{
			m_pEquipWnd->ShowWindow(!m_pEquipWnd->IsShow());
			if (m_pEquipWnd->IsShow())
				m_pSoundMgr->PlaySound(L"wopen.MP3", CHANNEL_ID::CHANNEL_WINDOW);
			else
				m_pSoundMgr->PlaySound(L"CloseWnd.wav", CHANNEL_ID::CHANNEL_WINDOW);
			m_vecWndRender.push_back(WND_ID::WNDID_EQUIP);
		}

		if (m_pKeyMgr->KeyDown(KEY_WND_SKILL))
		{
			m_pSkillWnd->ShowWindow(!m_pSkillWnd->IsShow());
			if (m_pSkillWnd->IsShow())
				m_pSoundMgr->PlaySound(L"wopen.MP3", CHANNEL_ID::CHANNEL_WINDOW);
			else
				m_pSoundMgr->PlaySound(L"CloseWnd.wav", CHANNEL_ID::CHANNEL_WINDOW);
			m_vecWndRender.push_back(WND_ID::WNDID_SKILL);
		}

		if (m_pKeyMgr->KeyDown(KEY_WND_COMBO))
		{
			m_pComboWnd->ShowWindow(!m_pComboWnd->IsShow());
			if (m_pComboWnd->IsShow())
				m_pSoundMgr->PlaySound(L"wopen.MP3", CHANNEL_ID::CHANNEL_WINDOW);
			else
				m_pSoundMgr->PlaySound(L"CloseWnd.wav", CHANNEL_ID::CHANNEL_WINDOW);
			m_vecWndRender.push_back(WND_ID::WNDID_COMBO);
		}
	}

	if (nullptr != m_pExpansionTex)
	{
		if (m_tExpansion.IsRectInPoint(vMouse)) {
			m_bIsMouseHover = true;
			if (m_pKeyMgr->KeyDown(VK_LBUTTON))
			{
				m_pSoundMgr->PlaySound(L"barover.MP3", CHANNEL_ID::CHANNEL_WINDOW);
				m_bIsExpansion ^= true;
			}
		}

		m_pExpansionTex = (m_bIsExpansion) ?
			m_pTextureMgr->GetTexInfo(L"Button", L"Expansion", 0)
			: m_pTextureMgr->GetTexInfo(L"Button", L"Expansion", 1);
		
		m_tExpansion.vPos.y = (m_bIsExpansion) ? 255.f : 0.f;
	}

	if (nullptr != m_pItemShopTex)
	{
		if (m_tItemShop.IsRectInPoint(vMouse)) {
			if (m_pKeyMgr->KeyDown(VK_LBUTTON))
			{
				CWordMgr::GetInstance()->StartWord(true, L"Word3");
			}
		}
	}

	if (m_bIsExpansion)
	{
		if (nullptr != m_pInfoWnd)
		{
			if (m_tInfoBar.IsRectInPoint(vMouse)) {
				m_bIsMouseHover = true;
				if (m_pKeyMgr->KeyDown(VK_LBUTTON))
				{
					m_pInfoWnd->ShowWindow(!m_pInfoWnd->IsShow());
					if (m_pInfoWnd->IsShow())
						m_pSoundMgr->PlaySound(L"wopen.MP3", CHANNEL_ID::CHANNEL_WINDOW);
					else
						m_pSoundMgr->PlaySound(L"CloseWnd.wav", CHANNEL_ID::CHANNEL_WINDOW);
					m_vecWndRender.push_back(WND_ID::WNDID_INFO);
				}
			}

			m_pInfoBarTex = m_pTextureMgr->GetTexInfo(L"Button", L"InfoBar", m_tInfoBar.iHover);
		}

		if (nullptr != m_pComboWnd)
		{
			if (m_tComboBar.IsRectInPoint(vMouse)) {
				m_bIsMouseHover = true;
				if (m_pKeyMgr->KeyDown(VK_LBUTTON))
				{
					m_pComboWnd->ShowWindow(!m_pComboWnd->IsShow());
					if (m_pComboWnd->IsShow())
						m_pSoundMgr->PlaySound(L"wopen.MP3", CHANNEL_ID::CHANNEL_WINDOW);
					else
						m_pSoundMgr->PlaySound(L"CloseWnd.wav", CHANNEL_ID::CHANNEL_WINDOW);
					m_vecWndRender.push_back(WND_ID::WNDID_COMBO);
				}
			}

			m_pComboBarTex = m_pTextureMgr->GetTexInfo(L"Button", L"ComboBar", m_tComboBar.iHover);
		}

		if (nullptr != m_pEquipWnd)
		{
			if (m_tEquipBar.IsRectInPoint(vMouse)) {
				m_bIsMouseHover = true;
				if (m_pKeyMgr->KeyDown(VK_LBUTTON))
				{
					m_pEquipWnd->ShowWindow(!m_pEquipWnd->IsShow());
					if (m_pEquipWnd->IsShow())
						m_pSoundMgr->PlaySound(L"wopen.MP3", CHANNEL_ID::CHANNEL_WINDOW);
					else
						m_pSoundMgr->PlaySound(L"CloseWnd.wav", CHANNEL_ID::CHANNEL_WINDOW);
					m_vecWndRender.push_back(WND_ID::WNDID_EQUIP);
				}
			}

			m_pEquipBarTex = m_pTextureMgr->GetTexInfo(L"Button", L"EqiupBar", m_tEquipBar.iHover);
		}

		if (nullptr != m_pInvenWnd)
		{
			if (m_tInvenBar.IsRectInPoint(vMouse)) {
				m_bIsMouseHover = true;
				if (m_pKeyMgr->KeyDown(VK_LBUTTON))
				{
					m_pInvenWnd->ShowWindow(!m_pInvenWnd->IsShow());
					if (m_pInvenWnd->IsShow())
						m_pSoundMgr->PlaySound(L"wopen.MP3", CHANNEL_ID::CHANNEL_WINDOW);
					else
						m_pSoundMgr->PlaySound(L"CloseWnd.wav", CHANNEL_ID::CHANNEL_WINDOW);
					m_vecWndRender.push_back(WND_ID::WNDID_INVEN);
				}
			}

			m_pInvenBarTex = m_pTextureMgr->GetTexInfo(L"Button", L"InvenBar", m_tInvenBar.iHover);
		}

		if (nullptr != m_pSkillWnd)
		{
			if (m_tSkillBar.IsRectInPoint(vMouse)) {
				m_bIsMouseHover = true;
				if (m_pKeyMgr->KeyDown(VK_LBUTTON))
				{
					m_pSkillWnd->ShowWindow(!m_pSkillWnd->IsShow());

					if(m_pSkillWnd->IsShow())
						m_pSoundMgr->PlaySound(L"wopen.MP3", CHANNEL_ID::CHANNEL_WINDOW);
					else
						m_pSoundMgr->PlaySound(L"CloseWnd.wav", CHANNEL_ID::CHANNEL_WINDOW);

					m_vecWndRender.push_back(WND_ID::WNDID_SKILL);
				}
			}

			m_pSkillBarTex = m_pTextureMgr->GetTexInfo(L"Button", L"SkillBar", m_tSkillBar.iHover);
		}
	}

	if (m_pKeyMgr->KeyDown(VK_LBUTTON))
	{
		m_pInfoWnd->WindowLClickDown();
		m_pEquipWnd->WindowLClickDown();
		m_pInvenWnd->WindowLClickDown();
		m_pSkillWnd->WindowLClickDown();
		m_pComboWnd->WindowLClickDown();
		m_pComboSetWnd->WindowLClickDown();
		m_pChatWnd->WindowLClickDown();
		m_pQuickWnd->WindowLClickDown();
		m_pShopWnd->WindowLClickDown();
		m_pMiniMapWnd->WindowLClickDown();
		m_pCountWnd->WindowLClickDown();
	}

	if (m_pKeyMgr->KeyUp(VK_LBUTTON))
	{
		m_pInfoWnd->WindowLClickUp();
		m_pEquipWnd->WindowLClickUp();
		m_pInvenWnd->WindowLClickUp();
		m_pSkillWnd->WindowLClickUp();
		m_pComboWnd->WindowLClickUp();
		m_pComboSetWnd->WindowLClickUp();
		m_pChatWnd->WindowLClickUp();
		m_pQuickWnd->WindowLClickUp();
		m_pShopWnd->WindowLClickUp();
		m_pMiniMapWnd->WindowLClickUp();
		m_pCountWnd->WindowLClickUp();
	}

	if (m_pKeyMgr->KeyDown(VK_RBUTTON))
	{
		m_pInfoWnd->WindowRClickDown();
		m_pEquipWnd->WindowRClickDown();
		m_pInvenWnd->WindowRClickDown();
		m_pSkillWnd->WindowRClickDown();
		m_pComboWnd->WindowRClickDown();
		m_pComboSetWnd->WindowRClickDown();
		m_pChatWnd->WindowRClickDown();
		m_pQuickWnd->WindowRClickDown();
		m_pShopWnd->WindowRClickDown();
		m_pMiniMapWnd->WindowRClickDown();
		m_pCountWnd->WindowRClickDown();
	}
}
