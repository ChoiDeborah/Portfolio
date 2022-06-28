#include "stdafx.h"
#include "SkillWnd.h"

#include "SkillData.h"
#include "ComboWnd.h"
#include "Player.h"

CSkillWnd::CSkillWnd()
{
}


CSkillWnd::~CSkillWnd()
{
	Release();
}

HRESULT CSkillWnd::Initialize()
{
	m_pWndTex = m_pTextureMgr->GetTexInfo(L"UI", L"SkillWnd");
	if (nullptr != m_pWndTex) {
		m_tWndInfo.vPos = { float(CLIENTCX - m_pWndTex->tImgInfo.Width) - 18.f, 204.f, 0.f };
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

	for (int i = 0; i < SKILL_SLOT_Y; ++i)
	{
		for (int j = 0; j < SKILL_SLOT_X; ++j)
		{
			m_tSkillIcon[i*SKILL_SLOT_X + j].vParentPos = m_tWndInfo.vPos;
			m_tSkillIcon[i*SKILL_SLOT_X + j].vPos		= { 21.f + 12.f + 31.f * j, 33.f + 12.f + 31.f * i, 0.f };
			m_tSkillIcon[i*SKILL_SLOT_X + j].vSize		= { 24.f, 24.f, 0.f };
			m_tSkillIcon[i*SKILL_SLOT_X + j].vOffset	= { 12.f, 12.f, 0.f };
		}
	}

	for (auto& Skill : m_vecSkillData)
		SafeDelete(Skill);
	m_vecSkillData.clear();
	m_vecSkillData.shrink_to_fit();

	m_vecSkillData.push_back(new CSkillData(ICON_TYPE::ICON_SKILL, SKILL_TYPE::SKILL_PASSIVE, SKILLID::SKILLID_STAB, L"Skill", L"Stab"));
	
	m_vecSkillData.push_back(new CSkillData(ICON_TYPE::ICON_SKILL, SKILL_TYPE::SKILL_PASSIVE, SKILLID::SKILLID_VERCUT, L"Skill", L"VerCut"));
	m_vecSkillData.push_back(new CSkillData(ICON_TYPE::ICON_SKILL, SKILL_TYPE::SKILL_PASSIVE, SKILLID::SKILLID_HORCUT, L"Skill", L"HorCut"));
	
	m_vecSkillData.push_back(new CSkillData(ICON_TYPE::ICON_SKILL, SKILL_TYPE::SKILL_ACTIVE, SKILLID::SKILLID_ACTIVE0, L"Skill", L"Active0"));
	m_vecSkillData.push_back(new CSkillData(ICON_TYPE::ICON_SKILL, SKILL_TYPE::SKILL_ACTIVE, SKILLID::SKILLID_ACTIVE1, L"Skill", L"Active1"));
	m_vecSkillData.push_back(new CSkillData(ICON_TYPE::ICON_SKILL, SKILL_TYPE::SKILL_ACTIVE, SKILLID::SKILLID_ACTIVE2, L"Skill", L"Active2"));
	m_vecSkillData.push_back(new CSkillData(ICON_TYPE::ICON_SKILL, SKILL_TYPE::SKILL_ACTIVE, SKILLID::SKILLID_ACTIVE3, L"Skill", L"Active3"));
	m_vecSkillData.push_back(new CSkillData(ICON_TYPE::ICON_SKILL, SKILL_TYPE::SKILL_ACTIVE, SKILLID::SKILLID_ACTIVE4, L"Skill", L"Active4"));
	
	m_vecSkillData.push_back(new CSkillData(ICON_TYPE::ICON_SKILL, SKILL_TYPE::SKILL_ACTIVE, SKILLID::SKILLID_FIREWAVE, L"Skill", L"FireWaveIcon"));
	m_vecSkillData.push_back(new CSkillData(ICON_TYPE::ICON_SKILL, SKILL_TYPE::SKILL_ACTIVE, SKILLID::SKILLID_RUSHCRASH, L"Skill", L"RushCrashIcon"));
	m_vecSkillData.push_back(new CSkillData(ICON_TYPE::ICON_SKILL, SKILL_TYPE::SKILL_ACTIVE, SKILLID::SKILLID_STARBUSTSTREAM, L"Skill", L"StarBustStreamIcon"));
	
	return S_OK;
}

void CSkillWnd::Update()
{
	CUIWnd::WindowMove();

	D3DXVECTOR3 vMouse = m_pMouseMgr->GetMouse();
	if (0 == m_tExitInfo.iHover && m_tExitInfo.IsRectInPoint(vMouse))
		m_pSoundMgr->PlaySound(L"barover.MP3", CHANNEL_ID::CHANNEL_WINDOW);
}

void CSkillWnd::Render()
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

		// ´Ý±â ¹öÆ°
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x + m_tExitInfo.vPos.x, m_tWndInfo.vPos.y + m_tExitInfo.vPos.y, 0.f);
		matWorld = matScale * matTrans;
		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pExitTex->pTexture, nullptr,
			&m_tExitInfo.vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


		//m_pUIMgr->DrawRect(m_tWndInfo);
		//m_pUIMgr->DrawRect(m_tMoveInfo);
	}


	const TEX_INFO* pSkillTex = nullptr;

	for (int i = 0; i < SKILL_SLOT_Y; ++i)
	{
		for (int j = 0; j < SKILL_SLOT_X; ++j)
		{
			if (size_t(i * SKILL_SLOT_X + j) >= m_vecSkillData.size())
				break;

			if (nullptr != m_vecSkillData[i * SKILL_SLOT_X + j])
			{
				pSkillTex = m_pTextureMgr->GetTexInfo(m_vecSkillData[i * SKILL_SLOT_X + j]->wstrObjKey, m_vecSkillData[i * SKILL_SLOT_X + j]->wstrStateKey);
				if (nullptr != pSkillTex)
				{
					D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
					D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x + m_tSkillIcon[i * SKILL_SLOT_X + j].vPos.x, m_tWndInfo.vPos.y + m_tSkillIcon[i * SKILL_SLOT_X + j].vPos.y, 0.f);
					matWorld = matScale * matTrans;
					m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
					m_pDeviceMgr->GetSprite()->Draw(pSkillTex->pTexture, nullptr,
						&pSkillTex->vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
				}
			}
		}
	}
}

void CSkillWnd::Release()
{
	for (auto& Skill : m_vecSkillData)
		SafeDelete(Skill);

	m_vecSkillData.clear();
	m_vecSkillData.shrink_to_fit();
}

void CSkillWnd::LClickDown()
{
	if (!m_bIsShow)
		return;

	D3DXVECTOR3	vMouse = m_pMouseMgr->GetMouse();

	if (!m_tWndInfo.IsRectInPoint(vMouse))
		return;

	m_pMouseMgr->SetItemData(WND_ID::WNDID_END, -1, nullptr);
	m_pMouseMgr->SetSkillData(WND_ID::WNDID_END, -1, nullptr);

	for (int i = 0; i < SKILL_SLOT_Y; ++i)
	{
		for (int j = 0; j < SKILL_SLOT_X; ++j)
		{
			m_tSkillIcon[i*SKILL_SLOT_X + j].vParentPos = m_tWndInfo.vPos;
			if (m_tSkillIcon[i*SKILL_SLOT_X + j].IsRectInPoint(vMouse))
			{
				m_pSoundMgr->PlaySound(L"BtnClick.wav", CHANNEL_ID::CHANNEL_WINDOW);

				if (0 > i*SKILL_SLOT_X + j || i*SKILL_SLOT_X + j >= int(m_vecSkillData.size()))
					break;

				m_pMouseMgr->SetSkillData(WND_ID::WNDID_SKILL, i*SKILL_SLOT_X + j, m_vecSkillData[i*SKILL_SLOT_X + j]);
			}
		}
	}
}

void CSkillWnd::LClickUp()
{
}

void CSkillWnd::RClickDown()
{
	if (!m_bIsShow)
		return;

	D3DXVECTOR3	vMouse = m_pMouseMgr->GetMouse();

	if (!m_tWndInfo.IsRectInPoint(vMouse))
		return;

	m_pMouseMgr->SetItemData(WND_ID::WNDID_END, -1, nullptr);

	for (int i = 0; i < SKILL_SLOT_Y; ++i)
	{
		for (int j = 0; j < SKILL_SLOT_X; ++j)
		{
			if (m_tSkillIcon[i*SKILL_SLOT_X + j].IsRectInPoint(vMouse))
			{
				m_pSoundMgr->PlaySound(L"BtnClick.wav", CHANNEL_ID::CHANNEL_WINDOW);

				if (0 > i*SKILL_SLOT_X + j || i*SKILL_SLOT_X + j >= int(m_vecSkillData.size()))
					break;

				m_vecSkillData[i*SKILL_SLOT_X + j]->UseEvent();
			}
		}
	}
	m_pMouseMgr->SetSkillData(WND_ID::WNDID_END, -1, nullptr);
}

CSkillData * CSkillWnd::GetSkillData(SKILLID eSkillID)
{
	CSkillData* pSkillData = nullptr;

	for (auto& Skill : m_vecSkillData)
	{
		if (nullptr != Skill)
		{
			if (Skill->m_eSkillID == eSkillID)
			{
				pSkillData = Skill;
				break;
			}
		}
	}

	return pSkillData;
}

CSkillWnd* CSkillWnd::Create()
{
	CSkillWnd* pInstance = new CSkillWnd;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}