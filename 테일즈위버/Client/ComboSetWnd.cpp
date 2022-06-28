#include "stdafx.h"
#include "ComboSetWnd.h"

#include "SkillWnd.h"
#include "ComboWnd.h"

#include "SkillData.h"
#include "Player.h"

CComboSetWnd::CComboSetWnd()
{
}


CComboSetWnd::~CComboSetWnd()
{
	Release();
}

HRESULT CComboSetWnd::Initialize()
{
	m_pWndTex = m_pTextureMgr->GetTexInfo(L"UI", L"ComboSetWnd");
	if (nullptr != m_pWndTex) 
	{
		m_tWndInfo.vPos = { float(CLIENTCX * 0.5f - m_pWndTex->tImgInfo.Width) - 18.f, 153.f, 0.f };
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

	// 애니메이션 창
	m_tCharacter.vParentPos = m_tWndInfo.vPos;
	m_tCharacter.vPos = { 6.f, 31.f, 0.f };
	m_tCharacter.vSize = { 164.f, 143.f, 0.f };
	m_tCharacter.vOffset = { 0.f, 0.f, 0.f };
	
	
	// 하단 콤보 아이콘
	for (int i = 0; i < COMBO_SLOT_X; ++i)
	{
		m_tSkillIcon[i].vParentPos = m_tWndInfo.vPos;
		m_tSkillIcon[i].vPos = { 182.f + 12.f + 37.f * i, 198.f + 12.f, 0.f };
		m_tSkillIcon[i].vSize = { 24.f, 24.f, 0.f };
		m_tSkillIcon[i].vOffset = { 12.f, 12.f, 0.f };
	}

	// 상단 모든 스킬
	for (int i = 0; i < COMBOSET_SELECT_Y; ++i)
	{
		m_tSelected[i].vParentPos = m_tWndInfo.vPos;
		m_tSelected[i].vPos = { 182.f + 12.f, 31.f + 12.f + 33.f * i, 0.f };
		m_tSelected[i].vSize = { 24.f, 24.f, 0.f };
		m_tSelected[i].vOffset = { 12.f, 12.f, 0.f };
	}

	// 애니메이션 상태 흰색
	m_tAnimState.vParentPos = m_tWndInfo.vPos;
	m_tAnimState.vPos = { 239.f, 172.f, 0.f };
	m_tAnimState.vSize = { 41.f, 20.f, 0.f };
	m_tAnimState.vOffset = { 0.f, 0.f, 0.f };

	// 애니메이션 일시정지
	m_tPauseBtn.vParentPos = m_tWndInfo.vPos;
	m_tPauseBtn.vPos = { 245.f, 175.f, 0.f };
	m_tPauseBtn.vSize = { 31.f, 15.f, 0.f };
	m_tPauseBtn.vOffset = { 0.f, 0.f, 0.f };

	// 애니메이션 재생
	m_tStartBtn.vParentPos = m_tWndInfo.vPos;
	m_tStartBtn.vPos = { 304.f, 175.f, 0.f };
	m_tStartBtn.vSize = { 31.f, 15.f, 0.f };
	m_tStartBtn.vOffset = { 0.f, 0.f, 0.f };

	// 스크롤
	{
		m_pScrollUpTex = m_pTextureMgr->GetTexInfo(L"Button", L"ScrollUp");
		if (nullptr != m_pScrollUpTex)
		{
			// UP
			m_tScrollUp.vParentPos = m_tWndInfo.vPos;
			m_tScrollUp.vPos = { 395.f, 34.f, 0.f };
			m_tScrollUp.vSize = { 12.f, 13.f, 0.f };
			m_tScrollUp.vOffset = { 0.f, 0.f, 0.f };
		}

		m_pScrollDownTex = m_pTextureMgr->GetTexInfo(L"Button", L"ScrollDown");
		if (nullptr != m_pScrollDownTex)
		{
			// Down
			m_tScrollDown.vParentPos = m_tWndInfo.vPos;
			m_tScrollDown.vPos = { 395.f, 144.f, 0.f };
			m_tScrollDown.vSize = { 12.f, 13.f, 0.f };
			m_tScrollDown.vOffset = { 0.f, 0.f, 0.f };
		}

		m_pScrollBarTex = m_pTextureMgr->GetTexInfo(L"Button", L"ScrollBar");
		if (nullptr != m_pScrollBarTex)
		{
			// Bar
			m_tScrollBar.vParentPos = m_tWndInfo.vPos;
			m_tScrollBar.vPos = { 395.f, 47.f, 0.f };
			m_tScrollBar.vSize = { 10.f, 19.f, 0.f };
			m_tScrollBar.vOffset = { 0.f, 0.f, 0.f };
		}
	}

	m_tDelayAlarm.SetAlarm(0.1f, false);
	m_tDelayAlarm.CheckAlarm();
	m_wstrObjKey = L"Kyoko";
	m_wstrStateKey = L"Fight_Idle_RD";
	m_tAnimation.iFrameEnd = m_pTextureMgr->GetTexCount(m_wstrObjKey, m_wstrStateKey);

	return S_OK;
}

void CComboSetWnd::Update()
{
	CUIWnd::WindowMove();

	for (int i = 0; i < COMBO_SLOT_X; ++i)
		m_tSkillIcon[i].vParentPos = m_tWndInfo.vPos;
	
	for (int i = 0; i < COMBO_SLOT_Y; ++i)
		m_tSelected[i].vParentPos = m_tWndInfo.vPos;
	
	m_tAnimState.vParentPos =
	m_tCharacter.vParentPos =
	m_tPauseBtn.vParentPos = 
	m_tStartBtn.vParentPos = 
	m_tScrollUp.vParentPos = 
	m_tScrollDown.vParentPos = 
	m_tScrollBar.vParentPos = m_tWndInfo.vPos;

	D3DXVECTOR3 vMouse = m_pMouseMgr->GetMouse();
	if ((0 == m_tAnimState.iHover && m_tAnimState.IsRectInPoint(vMouse))||
		(0 == m_tPauseBtn.iHover && m_tPauseBtn.IsRectInPoint(vMouse)) ||
		(0 == m_tStartBtn.iHover && m_tStartBtn.IsRectInPoint(vMouse)) ||
		(0 == m_tScrollUp.iHover && m_tScrollUp.IsRectInPoint(vMouse)) ||
		(0 == m_tScrollDown.iHover && m_tScrollDown.IsRectInPoint(vMouse)) ||
		(0 == m_tScrollBar.iHover && m_tScrollBar.IsRectInPoint(vMouse)) ||
		(0 == m_tExitInfo.iHover && m_tExitInfo.IsRectInPoint(vMouse)))
		m_pSoundMgr->PlaySound(L"barover.MP3", CHANNEL_ID::CHANNEL_WINDOW);

	m_tAnimState.vPos.x = (m_bIsAnim) ? 300.f : 239.f;
}

void CComboSetWnd::Render()
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
		D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x + m_tExitInfo.vPos.x, m_tWndInfo.vPos.y + m_tExitInfo.vPos.y, 0.f);
		matWorld = matScale * matTrans;
		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pExitTex->pTexture, nullptr,
			&m_tExitInfo.vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


		//m_pUIMgr->DrawRect(m_tWndInfo);
		//m_pUIMgr->DrawRect(m_tMoveInfo);
	}

	// 좌측 애니메이션
	const TEX_INFO* pCharacterTex = m_pTextureMgr->GetTexInfo(m_wstrObjKey, m_wstrStateKey, m_tAnimation.iFrameCur);
	if (nullptr != pCharacterTex)
	{
		if (m_tCharacter.vSize.x < pCharacterTex->tImgInfo.Width)
			D3DXMatrixScaling(&matScale, m_tCharacter.vSize.x / pCharacterTex->tImgInfo.Width * pCharacterTex->m_iMirror, m_tCharacter.vSize.x / pCharacterTex->tImgInfo.Width, 0.f);
		else if (m_tCharacter.vSize.y < pCharacterTex->tImgInfo.Height)
			D3DXMatrixScaling(&matScale, m_tCharacter.vSize.y / pCharacterTex->tImgInfo.Height * pCharacterTex->m_iMirror, m_tCharacter.vSize.y / pCharacterTex->tImgInfo.Height, 0.f);
		else
			D3DXMatrixScaling(&matScale, 1.f * pCharacterTex->m_iMirror, 1.f, 0.f);
		
		D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x + m_tCharacter.vPos.x + m_tCharacter.vSize.x * 0.5f, m_tWndInfo.vPos.y + m_tCharacter.vPos.y + m_tCharacter.vSize.y * 0.7f, 0.f);
		matWorld = matScale * matTrans;
		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pCharacterTex->pTexture, nullptr, &pCharacterTex->vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	// 상단 모든 스킬
	const TEX_INFO* pSkillTex = nullptr;
	vector<CSkillData*>* pVecAllSkill = dynamic_cast<CSkillWnd*>(m_pUIMgr->GetWindow(WND_ID::WNDID_SKILL))->GetVecSkill();
	if (nullptr != pVecAllSkill)
	{
		for (int i = 0; i < COMBOSET_SELECT_Y; ++i)
		{
			if (0 > i + m_iScrollY || i + m_iScrollY >= int(pVecAllSkill->size()))
				continue;

			if (nullptr != (*pVecAllSkill)[i + m_iScrollY])
			{
				pSkillTex = m_pTextureMgr->GetTexInfo((*pVecAllSkill)[i + m_iScrollY]->wstrObjKey, (*pVecAllSkill)[i + m_iScrollY]->wstrStateKey);
				if (nullptr != pSkillTex)
				{
					D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
					D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x + m_tSelected[i].vPos.x, m_tWndInfo.vPos.y + m_tSelected[i].vPos.y, 0.f);
					matWorld = matScale * matTrans;
					m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
					m_pDeviceMgr->GetSprite()->Draw(pSkillTex->pTexture, nullptr,
						&pSkillTex->vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
				}
			}
		}
	}

	
	// 하단 콤보 스킬
	vector<CSkillData*>* pVecCurCombo = dynamic_cast<CComboWnd*>(m_pUIMgr->GetWindow(WND_ID::WNDID_COMBO))->GetVecCurCombo();
	if (nullptr != pVecCurCombo)
	{
		for (size_t i = 0; i < pVecCurCombo->size(); ++i)
		{
			if (nullptr != (*pVecCurCombo)[i])
			{
				pSkillTex = m_pTextureMgr->GetTexInfo((*pVecCurCombo)[i]->wstrObjKey, (*pVecCurCombo)[i]->wstrStateKey);
				if (nullptr != pSkillTex)
				{
					D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
					D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x + m_tSkillIcon[i].vPos.x, m_tWndInfo.vPos.y + m_tSkillIcon[i].vPos.y, 0.f);
					matWorld = matScale * matTrans;
					m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
					m_pDeviceMgr->GetSprite()->Draw(pSkillTex->pTexture, nullptr,
						&pSkillTex->vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
				}
			}
		}
	}

	// 하단 현재 셀렉트
	const TEX_INFO* pSelect = m_pTextureMgr->GetTexInfo(L"UI", L"SkillSelect");
	if (nullptr != pSelect)
	{
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x + m_tSkillIcon[m_iComboCount].vPos.x, m_tWndInfo.vPos.y + m_tSkillIcon[m_iComboCount].vPos.y, 0.f);
		matWorld = matScale * matTrans;
		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pSelect->pTexture, nullptr,
			&pSelect->vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	// 애니메이션 상태 흰색
	const TEX_INFO* pAnimSelect = m_pTextureMgr->GetTexInfo(L"UI", L"AnimState");
	if (nullptr != pAnimSelect)
	{
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x + m_tAnimState.vPos.x, m_tWndInfo.vPos.y + m_tAnimState.vPos.y, 0.f);
		matWorld = matScale * matTrans;
		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(pAnimSelect->pTexture, nullptr, &m_tAnimState.vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}


	// 스크롤
	// 스크롤 업
	if (nullptr != m_pScrollUpTex)
	{
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x + m_tScrollUp.vPos.x, m_tWndInfo.vPos.y + m_tScrollUp.vPos.y, 0.f);
		m_pDeviceMgr->GetSprite()->SetTransform(&(matScale*matTrans));
		m_pDeviceMgr->GetSprite()->Draw(m_pScrollUpTex->pTexture, nullptr, &m_pScrollUpTex->vOffset, nullptr, m_pScrollUpTex->dwBlend);
	}

	// 스크롤 다운
	if (nullptr != m_pScrollDownTex)
	{
		D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x + m_tScrollDown.vPos.x, m_tWndInfo.vPos.y + m_tScrollDown.vPos.y, 0.f);
		m_pDeviceMgr->GetSprite()->SetTransform(&(matScale*matTrans));
		m_pDeviceMgr->GetSprite()->Draw(m_pScrollDownTex->pTexture, nullptr, &m_pScrollDownTex->vOffset, nullptr, m_pScrollDownTex->dwBlend);
	}

	// 스크롤
	if (COMBOSET_SELECT_Y < pVecAllSkill->size())
	{
		// 스크롤 바
		if (nullptr != m_pScrollBarTex)
		{
			// 62 ~ 328 = 247
			D3DXMatrixScaling(&matScale, m_tScrollDown.vSize.x / m_tScrollBar.vSize.x, 1.f/*((266.f - (25.f * m_vecItem.size() - INVEN_SIZE)) / m_tScrollBar.vSize.y)*/, 0.f);
			D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x + m_tScrollBar.vPos.x, m_tWndInfo.vPos.y + m_tScrollBar.vPos.y + ((m_tScrollDown.vPos.y - m_tScrollUp.vPos.y - m_tScrollUp.vSize.y - m_tScrollBar.vSize.y) / (pVecAllSkill->size() - COMBOSET_SELECT_Y)) * m_iScrollY, 0.f);
			m_pDeviceMgr->GetSprite()->SetTransform(&(matScale*matTrans));
			m_pDeviceMgr->GetSprite()->Draw(m_pScrollBarTex->pTexture, nullptr, &m_pScrollBarTex->vOffset, nullptr, m_pScrollBarTex->dwBlend);
		}
	}


	// 프레임 이동
	if (m_bIsAnim)
	{
		m_tAnimation.fFrameTime += CTimeMgr::GetInstance()->GetDeltaTime();
		if (m_tAnimation.fFrameDelay < m_tAnimation.fFrameTime) {
			m_tAnimation.fFrameTime = 0.f;
			++m_tAnimation.iFrameCur;

			if (m_tAnimation.iFrameCur >= m_tAnimation.iFrameEnd) {
				m_tAnimation.iFrameCur = 0;

				if (ANIM_IDLE != m_eAnimState)
				{
					m_wstrObjKey = L"Kyoko";
					m_wstrStateKey = L"Fight_Idle_RD";
					m_eAnimState = ANIM_IDLE;

					const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrObjKey, m_wstrStateKey);
					if (nullptr != pTexInfo)
					{
						m_tAnimation.iFrameEnd = m_pTextureMgr->GetTexCount(m_wstrObjKey, m_wstrStateKey);
						m_tAnimation.fFrameDelay = pTexInfo->m_fFrameDelay;
					}

					if (m_iComboCount + 1 >= int(pVecCurCombo->size()))
					{
						m_iComboCount = 0;
						m_bIsAnim = false;
					}
					else
					{
						m_tDelayAlarm.SetAlarm((*pVecCurCombo)[m_iComboCount]->fColltime, false);
					}
				}
			}
		}

		if (ANIM_IDLE == m_eAnimState && m_tDelayAlarm.CheckAlarm())
		{
			++m_iComboCount;
			m_eAnimState = ANIM_ATTACK;

			switch ((*pVecCurCombo)[m_iComboCount]->m_eSkillID)
			{
			case SKILLID_VERCUT:			m_wstrObjKey = L"Kyoko"; m_wstrStateKey = L"Attack2_RD"; break;
			case SKILLID_ACTIVE0:			m_wstrObjKey = L"Kyoko"; m_wstrStateKey = L"Attack2_RD"; break;

			case SKILLID_STAB:				m_wstrObjKey = L"Kyoko"; m_wstrStateKey = L"Attack1_RD"; break;
			case SKILLID_ACTIVE1:			m_wstrObjKey = L"Kyoko"; m_wstrStateKey = L"Attack1_RD"; break;


			case SKILLID_ACTIVE2:			m_wstrObjKey = L"Kyoko"; m_wstrStateKey = L"Attack1_RD"; break;
			case SKILLID_ACTIVE3:			m_wstrObjKey = L"Kyoko"; m_wstrStateKey = L"Attack1_RD"; break;
			case SKILLID_HORCUT:			m_wstrObjKey = L"Kyoko"; m_wstrStateKey = L"Attack1_RD"; break;


			case SKILLID_ACTIVE4:			m_wstrObjKey = L"Kyoko"; m_wstrStateKey = L"Attack1_RD"; break;

			case SKILLID_FIREWAVE:			m_wstrObjKey = L"Kyoko"; m_wstrStateKey = L"Attack1_RD"; break;

			case SKILLID_RUSHCRASH:			m_wstrObjKey = L"Kyoko"; m_wstrStateKey = L"Skill_RD"; break;
			case SKILLID_STARBUSTSTREAM:	m_wstrObjKey = L"Kyoko"; m_wstrStateKey = L"Attack10_R"; break;
			}

			const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrObjKey, m_wstrStateKey);
			if (nullptr != pTexInfo)
			{
				m_tAnimation.iFrameEnd = m_pTextureMgr->GetTexCount(m_wstrObjKey, m_wstrStateKey);
				m_tAnimation.fFrameDelay = pTexInfo->m_fFrameDelay;
			}
		}
	}	
}

void CComboSetWnd::Release()
{
}

void CComboSetWnd::LClickDown()
{
	if (!m_bIsShow)
		return;

	D3DXVECTOR3	vMouse = m_pMouseMgr->GetMouse();

	if (!m_tWndInfo.IsRectInPoint(vMouse))
		return;

	m_pMouseMgr->SetItemData(WND_ID::WNDID_END, -1, nullptr);
	m_pMouseMgr->SetSkillData(WND_ID::WNDID_END, -1, nullptr);

	// 상단 모든 메뉴 클릭
	vector<CSkillData*>* pVecAllSkill = dynamic_cast<CSkillWnd*>(m_pUIMgr->GetWindow(WND_ID::WNDID_SKILL))->GetVecSkill();
	vector<CSkillData*>* pVecCurCombo = dynamic_cast<CComboWnd*>(m_pUIMgr->GetWindow(WND_ID::WNDID_COMBO))->GetVecCurCombo();
	for (int i = 0; i < COMBO_SLOT_Y; ++i)
	{
		if (m_tSelected[i].IsRectInPoint(vMouse))
		{
			m_pSoundMgr->PlaySound(L"Drag.wav", CHANNEL_ID::CHANNEL_WINDOW);

			if (0 > i + m_iScrollY || i + m_iScrollY >= int(pVecAllSkill->size()))
				continue;

			if (m_iComboCount < int(pVecCurCombo->size()))
				(*pVecCurCombo)[m_iComboCount] = (*pVecAllSkill)[i + m_iScrollY];

			++m_iComboCount;
			if (m_iComboCount >= COMBOSET_SLOT_X)
				m_iComboCount = 0;
		}
	}

	// 하단 콤보 클릭
	for (size_t i = 0; i < pVecCurCombo->size(); ++i)
	{
		if (m_tSkillIcon[i].IsRectInPoint(vMouse))
		{
			m_pSoundMgr->PlaySound(L"Drag.wav", CHANNEL_ID::CHANNEL_WINDOW);

			if (0 > i || i >= int(pVecCurCombo->size()))
				continue;

			m_iComboCount = i;
		}
	}

	// 취소 클릭
	if (m_tPauseBtn.IsRectInPoint(vMouse))
	{
		CSoundMgr::GetInstance()->PlaySound(L"BtnClick.wav", CHANNEL_ID::CHANNEL_WINDOW);

		m_bIsAnim = false;
		m_tAnimation.iFrameCur = 0;

		m_wstrObjKey = L"Kyoko";
		m_wstrStateKey = L"Fight_Idle_RD";
		const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_pUIMgr->GetPlayerData().wstrObjKey, m_wstrStateKey);
		if (nullptr != pTexInfo)
		{
			m_tAnimation.iFrameEnd = m_pTextureMgr->GetTexCount(m_wstrObjKey, m_wstrStateKey);
			m_tAnimation.fFrameDelay = pTexInfo->m_fFrameDelay;
		}
	}

	// 재생 클릭
	if (m_tStartBtn.IsRectInPoint(vMouse))
	{
		CSoundMgr::GetInstance()->PlaySound(L"BtnClick.wav", CHANNEL_ID::CHANNEL_WINDOW);

		m_bIsAnim = true;
		m_eAnimState = ANIM_ATTACK;
		switch ((*pVecCurCombo)[m_iComboCount]->m_eSkillID)
		{
		case SKILLID_VERCUT:			m_wstrObjKey = L"Kyoko"; m_wstrStateKey = L"Attack2_RD"; break;
		case SKILLID_ACTIVE0:			m_wstrObjKey = L"Kyoko"; m_wstrStateKey = L"Attack2_RD"; break;

		case SKILLID_STAB:				m_wstrObjKey = L"Kyoko"; m_wstrStateKey = L"Attack1_RD"; break;
		case SKILLID_ACTIVE1:			m_wstrObjKey = L"Kyoko"; m_wstrStateKey = L"Attack1_RD"; break;


		case SKILLID_ACTIVE2:			m_wstrObjKey = L"Creature"; m_wstrStateKey = L"Attack0_R"; break;
		case SKILLID_ACTIVE3:			m_wstrObjKey = L"Creature"; m_wstrStateKey = L"Attack1_R"; break;
		case SKILLID_HORCUT:			m_wstrObjKey = L"Creature"; m_wstrStateKey = L"Attack2_R"; break;


		case SKILLID_ACTIVE4:			m_wstrObjKey = L"Creature"; m_wstrStateKey = L"Attack4"; break;

		case SKILLID_FIREWAVE:			m_wstrObjKey = L"Creature"; m_wstrStateKey = L"Attack11"; break;

		case SKILLID_RUSHCRASH:			m_wstrObjKey = L"Kyoko"; m_wstrStateKey = L"Skill_RD"; break;
		case SKILLID_STARBUSTSTREAM:	m_wstrObjKey = L"Kyoko"; m_wstrStateKey = L"Attack10_R"; break;
		}

		const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_wstrObjKey, m_wstrStateKey);
		if (nullptr != pTexInfo)
		{
			m_tAnimation.iFrameEnd = m_pTextureMgr->GetTexCount(m_wstrObjKey, m_wstrStateKey);
			m_tAnimation.fFrameDelay = pTexInfo->m_fFrameDelay;
		}
	}
}

void CComboSetWnd::LClickUp()
{
}

void CComboSetWnd::RClickDown()
{
	if (!m_bIsShow)
		return;

	D3DXVECTOR3	vMouse = m_pMouseMgr->GetMouse();

	if (!m_tWndInfo.IsRectInPoint(vMouse))
		return;

	m_pMouseMgr->SetItemData(WND_ID::WNDID_END, -1, nullptr);
	m_pMouseMgr->SetSkillData(WND_ID::WNDID_END, -1, nullptr);
}

CComboSetWnd* CComboSetWnd::Create()
{
	CComboSetWnd* pInstance = new CComboSetWnd;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}