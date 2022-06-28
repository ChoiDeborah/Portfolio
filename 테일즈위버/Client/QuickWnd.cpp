#include "stdafx.h"
#include "QuickWnd.h"

#include "ItemEquipData.h"
#include "ItemUsableData.h"

#include "InvenWnd.h"

#include "SkillData.h"
#include "Player.h"

CQuickWnd::CQuickWnd()
{
}


CQuickWnd::~CQuickWnd()
{
	Release();
}

HRESULT CQuickWnd::Initialize()
{
	m_bIsShow = true;

	// 왼쪽 하단 스킬/아이템 창
	m_pWndTex = m_pTextureMgr->GetTexInfo(L"UI", L"QuickSlot");
	if (nullptr != m_pWndTex) {
		m_tWndInfo.vPos = { 0.f, float(CLIENTCY - m_pWndTex->tImgInfo.Height), 0.f };
		m_tWndInfo.vSize = { float(m_pWndTex->tImgInfo.Width), float(m_pWndTex->tImgInfo.Height), 0.f };
		m_tWndInfo.vOffset = { 0.f, 0.f, 0.f }; //m_pWndTex->vOffset;
				
		for (int i = 0; i < QUICK_SLOT_SIZEY; ++i)
		{
			for (int j = 0; j < QUICK_SLOT_SIZEX; ++j)
			{
				m_tIcon[i*QUICK_SLOT_SIZEX + j].vParentPos = m_tWndInfo.vPos;
				m_tIcon[i*QUICK_SLOT_SIZEX + j].vPos = { 10.f + 30.f * j, 28.f + 41.f * i, 0.f };
				m_tIcon[i*QUICK_SLOT_SIZEX + j].vSize = { 26.f, 26.f, 0.f };
				m_tIcon[i*QUICK_SLOT_SIZEX + j].vOffset = { 0.f, 0.f, 0.f };
			}
		}
	}

	m_iQuickKey[0] = KEY_QUICK1;
	m_iQuickKey[1] = KEY_QUICK2;
	m_iQuickKey[2] = KEY_QUICK3;
	m_iQuickKey[3] = KEY_QUICK4;
	m_iQuickKey[4] = KEY_QUICK5;
	m_iQuickKey[5] = KEY_QUICK6;
	m_iQuickKey[6] = KEY_QUICK7;
	m_iQuickKey[7] = KEY_QUICK8;

	return S_OK;
}

void CQuickWnd::Update()
{
	CUIWnd::WindowMove();

	for (int i = 0; i < QUICK_SLOT_SIZEX*QUICK_SLOT_SIZEY; ++i)
	{
		if (m_pKeyMgr->KeyDown(m_iQuickKey[i])) 
		{
			if (nullptr != m_pIcon[m_iQuickIndex][i])
			{
				m_pIcon[m_iQuickIndex][i]->UseEvent();
			}
		}
	}


	for (int i = 0; i < QUICK_SLOT_SIZEY; ++i)
	{
		for (int j = 0; j < QUICK_SLOT_SIZEX; ++j)
		{
			m_tIcon[i*QUICK_SLOT_SIZEX + j].vParentPos = m_tWndInfo.vPos;

			for (int k = 0; k < 2; ++k)
			{
				if (nullptr != m_pIcon[k][i*QUICK_SLOT_SIZEX + j] && ICON_TYPE::ICON_ITEM == m_pIcon[k][i*QUICK_SLOT_SIZEX + j]->eIcon)
				{
					if (0 >= dynamic_cast<CItemData*>(m_pIcon[k][i*QUICK_SLOT_SIZEX + j])->iCount)
						m_pIcon[k][i*QUICK_SLOT_SIZEX + j] = nullptr;
				}
			}
		}
	}
}

void CQuickWnd::Render()
{
	D3DXMATRIX matScale, matTrans, matWorld;

	if (nullptr != m_pWndTex) {
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tWndInfo.vPos.x, m_tWndInfo.vPos.y, 0.f);
		matWorld = matScale * matTrans;

		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Draw(m_pWndTex->pTexture, nullptr,
			&m_tWndInfo.vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		//m_pUIMgr->DrawRect(m_tWndInfo);

		// 아이콘 정보
		for (int i = 0; i < QUICK_SLOT_SIZEY; ++i)
		{
			for (int j = 0; j < QUICK_SLOT_SIZEX; ++j)
			{
				//m_pUIMgr->DrawRect(m_tIcon[i*QUICK_SLOT_SIZEX + j]);

				if (nullptr == m_pIcon[m_iQuickIndex][i*QUICK_SLOT_SIZEX + j])
					continue;
				
				const TEX_INFO*	pTexInfo = m_pTextureMgr->GetTexInfo(m_pIcon[m_iQuickIndex][i*QUICK_SLOT_SIZEX + j]->wstrObjKey, m_pIcon[m_iQuickIndex][i*QUICK_SLOT_SIZEX + j]->wstrStateKey);
				if (nullptr != pTexInfo)
				{
					// 아이템 그림
					D3DXMatrixTranslation(&matTrans, 
						m_tWndInfo.vPos.x + m_tIcon[i*QUICK_SLOT_SIZEX + j].vPos.x + m_tIcon[i*QUICK_SLOT_SIZEX + j].vSize.x * 0.5f, 
						m_tWndInfo.vPos.y + m_tIcon[i*QUICK_SLOT_SIZEX + j].vPos.y + m_tIcon[i*QUICK_SLOT_SIZEX + j].vSize.y * 0.5f, 0.f);
					m_pDeviceMgr->GetSprite()->SetTransform(&matTrans);
					m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
						&pTexInfo->vOffset, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

					if (ICON_TYPE::ICON_ITEM == m_pIcon[m_iQuickIndex][i*QUICK_SLOT_SIZEX + j]->eIcon)
					{
						D3DXMatrixTranslation(&matTrans,
							m_tWndInfo.vPos.x + m_tIcon[i*QUICK_SLOT_SIZEX + j].vPos.x + m_tIcon[i*QUICK_SLOT_SIZEX + j].vSize.x * 0.7f,
							m_tWndInfo.vPos.y + m_tIcon[i*QUICK_SLOT_SIZEX + j].vPos.y + m_tIcon[i*QUICK_SLOT_SIZEX + j].vSize.y * 0.7f, 0.f);
						m_pDeviceMgr->GetSprite()->SetTransform(&matTrans);
						m_pDeviceMgr->DrawFont(to_wstring(dynamic_cast<CItemData*>(m_pIcon[m_iQuickIndex][i*QUICK_SLOT_SIZEX + j])->iCount), nullptr, DT_LEFT);
					}
				}
			}
		}
	}
}

void CQuickWnd::Release()
{
}

void CQuickWnd::LClickDown()
{
	D3DXVECTOR3	vMouse = m_pMouseMgr->GetMouse();

	if (!m_tWndInfo.IsRectInPoint(vMouse))
		return;

	// 마우스에 아이템 없는 상태
	CItemData* pMouseItemData = m_pMouseMgr->GetItemData();
	CSkillData* pMouseSkillData = m_pMouseMgr->GetSkillData();
	if (nullptr == pMouseItemData && nullptr == pMouseSkillData)
	{
		bool bIsFind = false;
		for (int i = 0; i < QUICK_SLOT_SIZEY; ++i)
		{
			for (int j = 0; j < QUICK_SLOT_SIZEX; ++j)
			{
				m_tIcon[i*QUICK_SLOT_SIZEX + j].vParentPos = m_tWndInfo.vPos;

				if (m_tIcon[i*QUICK_SLOT_SIZEX + j].IsRectInPoint(vMouse))
				{
					m_pSoundMgr->PlaySound(L"Drag.wav", CHANNEL_ID::CHANNEL_WINDOW);
					bIsFind = true;
					if (nullptr == m_pIcon[m_iQuickIndex][i*QUICK_SLOT_SIZEX + j])
					{
						break;
					}
					else
					{
						switch (m_pIcon[m_iQuickIndex][i*QUICK_SLOT_SIZEX + j]->eIcon)
						{
						case ICON_TYPE::ICON_ITEM:
							m_pSoundMgr->PlaySound(L"PotionDrug.wav", CHANNEL_ID::CHANNEL_USEITEM);

							m_pMouseMgr->SetItemData(WND_ID::WNDID_QUICK, i*QUICK_SLOT_SIZEX + j, dynamic_cast<CItemData*>(m_pIcon[m_iQuickIndex][i*QUICK_SLOT_SIZEX + j]));
							m_pMouseMgr->SetSkillData(WND_ID::WNDID_END, -1, nullptr);
							break;

						case ICON_TYPE::ICON_SKILL:
							m_pMouseMgr->SetSkillData(WND_ID::WNDID_QUICK, i*QUICK_SLOT_SIZEX + j, dynamic_cast<CSkillData*>(m_pIcon[m_iQuickIndex][i*QUICK_SLOT_SIZEX + j]));
							m_pMouseMgr->SetItemData(WND_ID::WNDID_END, -1, nullptr);
							break;
						}
					}
				}
			}
			if (bIsFind)
				break;
		}
	}
	else
	{
		if (nullptr != pMouseItemData)
		{
			if (ITEM_TYPE::ITEM_USE == pMouseItemData->eItemType)
			{
				int iY = QUICK_SLOT_SIZEY;
				int iX = QUICK_SLOT_SIZEX;

				bool bIsFind = false;
				for (int i = 0; i < QUICK_SLOT_SIZEY; ++i)
				{
					for (int j = 0; j < QUICK_SLOT_SIZEX; ++j)
					{
						if (m_tIcon[i*QUICK_SLOT_SIZEX + j].IsRectInPoint(vMouse))
						{
							bIsFind = true;

							iY = i;
							iX = j;

							if (nullptr == m_pIcon[m_iQuickIndex][i*QUICK_SLOT_SIZEX + j])
							{
								m_pIcon[m_iQuickIndex][i*QUICK_SLOT_SIZEX + j] = pMouseItemData;

								if (WND_ID::WNDID_QUICK == m_pMouseMgr->GetFromWndID())
								{
									m_pIcon[m_iQuickIndex][m_pMouseMgr->GetFromItemIndex()] = nullptr;
								}
							}
							else
							{
								if (m_pMouseMgr->GetFromItemIndex() != i*QUICK_SLOT_SIZEX + j)
								{
									CIconData* pTmp = m_pIcon[m_iQuickIndex][i*QUICK_SLOT_SIZEX + j];

									m_pIcon[m_iQuickIndex][i*QUICK_SLOT_SIZEX + j] = pMouseItemData;

									if (WND_ID::WNDID_QUICK == m_pMouseMgr->GetFromWndID())
									{
										m_pIcon[m_iQuickIndex][m_pMouseMgr->GetFromItemIndex()] = pTmp;
									}
								}
							}

							break;
						}
					}
					if (bIsFind)
						break;
				}

				if (bIsFind)
				{
					for (int i = 0; i < QUICK_SLOT_SIZEY; ++i)
					{
						for (int j = 0; j < QUICK_SLOT_SIZEX; ++j)
						{
							if (i == iY && j == iX)
								continue;

							if (nullptr != m_pIcon[m_iQuickIndex][i*QUICK_SLOT_SIZEX + j])
							{
								if (!lstrcmp(m_pIcon[m_iQuickIndex][i*QUICK_SLOT_SIZEX + j]->wstrStateKey.c_str(),
									m_pIcon[m_iQuickIndex][iY*QUICK_SLOT_SIZEX + iX]->wstrStateKey.c_str()))
								{
									m_pIcon[m_iQuickIndex][i*QUICK_SLOT_SIZEX + j] = nullptr;
								}
							}
						}
					}
				}
			}
		}
		else if (nullptr != pMouseSkillData)
		{
			if (SKILL_TYPE::SKILL_ACTIVE == pMouseSkillData->m_eSkillType)
			{
				int iY = QUICK_SLOT_SIZEY;
				int iX = QUICK_SLOT_SIZEX;

				bool bIsFind = false;
				for (int i = 0; i < QUICK_SLOT_SIZEY; ++i)
				{
					for (int j = 0; j < QUICK_SLOT_SIZEX; ++j)
					{
						if (m_tIcon[i*QUICK_SLOT_SIZEX + j].IsRectInPoint(vMouse))
						{
							bIsFind = true;

							iY = i;
							iX = j;

							if (nullptr == m_pIcon[m_iQuickIndex][i*QUICK_SLOT_SIZEX + j])
							{
								m_pIcon[m_iQuickIndex][i*QUICK_SLOT_SIZEX + j] = pMouseSkillData;

								if (WND_ID::WNDID_QUICK == m_pMouseMgr->GetFromWndID())
								{
									m_pIcon[m_iQuickIndex][m_pMouseMgr->GetFromItemIndex()] = nullptr;
								}
							}
							else
							{
								if (m_pMouseMgr->GetFromItemIndex() != i*QUICK_SLOT_SIZEX + j)
								{
									CIconData* pTmp = m_pIcon[m_iQuickIndex][i*QUICK_SLOT_SIZEX + j];

									m_pIcon[m_iQuickIndex][i*QUICK_SLOT_SIZEX + j] = pMouseSkillData;

									if (WND_ID::WNDID_QUICK == m_pMouseMgr->GetFromWndID())
									{
										m_pIcon[m_iQuickIndex][m_pMouseMgr->GetFromItemIndex()] = pTmp;
									}
								}
							}

							break;
						}
					}
					if (bIsFind)
						break;
				}

				if (bIsFind)
				{
					for (int i = 0; i < QUICK_SLOT_SIZEY; ++i)
					{
						for (int j = 0; j < QUICK_SLOT_SIZEX; ++j)
						{
							if (i == iY && j == iX)
								continue;

							if (nullptr != m_pIcon[m_iQuickIndex][i*QUICK_SLOT_SIZEX + j])
							{
								if (!lstrcmp(m_pIcon[m_iQuickIndex][i*QUICK_SLOT_SIZEX + j]->wstrStateKey.c_str(),
									m_pIcon[m_iQuickIndex][iY*QUICK_SLOT_SIZEX + iX]->wstrStateKey.c_str()))
								{
									m_pIcon[m_iQuickIndex][i*QUICK_SLOT_SIZEX + j] = nullptr;
								}
							}
						}
					}
				}
			}
		}

		m_pMouseMgr->SetItemData(WND_ID::WNDID_END, -1, nullptr);
		m_pMouseMgr->SetSkillData(WND_ID::WNDID_END, -1, nullptr);
	}
}

void CQuickWnd::LClickUp()
{
}

void CQuickWnd::RClickDown()
{
	D3DXVECTOR3	vMouse = m_pMouseMgr->GetMouse();

	if (!m_tWndInfo.IsRectInPoint(vMouse))
		return;

	bool bIsFind = false;
	for (int i = 0; i < QUICK_SLOT_SIZEY; ++i)
	{
		for (int j = 0; j < QUICK_SLOT_SIZEX; ++j)
		{
			m_tIcon[i*QUICK_SLOT_SIZEX + j].vParentPos = m_tWndInfo.vPos;

			if (m_tIcon[i*QUICK_SLOT_SIZEX + j].IsRectInPoint(vMouse))
			{
				bIsFind = true;
				if (nullptr == m_pIcon[m_iQuickIndex][i*QUICK_SLOT_SIZEX + j])
				{
					break;
				}
				else
				{
					switch (m_pIcon[m_iQuickIndex][i*QUICK_SLOT_SIZEX + j]->eIcon)
					{
					case ICON_TYPE::ICON_ITEM:
						if (ITEM_TYPE::ITEM_USE == dynamic_cast<CItemData*>(m_pIcon[m_iQuickIndex][i*QUICK_SLOT_SIZEX + j])->eItemType)
						{
							dynamic_cast<CItemUsableData*>(m_pIcon[m_iQuickIndex][i*QUICK_SLOT_SIZEX + j])->UseEvent();
							dynamic_cast<CInvenWnd*>(m_pUIMgr->GetWindow(WND_ID::WNDID_INVEN))->UpdateSlot();
						}
						break;

					case ICON_TYPE::ICON_SKILL:
					{
						m_pIcon[m_iQuickIndex][i*QUICK_SLOT_SIZEX + j]->UseEvent();
					}
						break;
					}
				}
			}
		}
		if (bIsFind)
			break;
	}

	//m_pMouseMgr->SetItemData(WND_ID::WNDID_END, -1, nullptr);
	//m_pMouseMgr->SetSkillData(WND_ID::WNDID_END, -1, nullptr);
}


void CQuickWnd::SetIcon(int iIndex, CIconData* pIconData)
{
	if (0 > iIndex || iIndex >= QUICK_SLOT_SIZEX * QUICK_SLOT_SIZEY)
		return;

	NULL_CHECK(pIconData);

	if (nullptr != m_pIcon[m_iQuickIndex][iIndex])
		SafeDelete(m_pIcon[m_iQuickIndex][iIndex]);

	m_pIcon[m_iQuickIndex][iIndex] = pIconData;
}

void CQuickWnd::DeleteItem(int iIndex)
{
	if (0 > iIndex || iIndex >= QUICK_SLOT_SIZEX * QUICK_SLOT_SIZEY)
		return;

	m_pIcon[m_iQuickIndex][iIndex] = nullptr;
}

void CQuickWnd::UpdateSlot()
{
	for (int i = 0; i < QUICK_SLOT_SIZEY; ++i)
	{
		for (int j = 0; j < QUICK_SLOT_SIZEX; ++j)
		{
			m_tIcon[i*QUICK_SLOT_SIZEX + j].vParentPos = m_tWndInfo.vPos;

			for (int k = 0; k < 2; ++k)
			{
				if (nullptr != m_pIcon[k][i*QUICK_SLOT_SIZEX + j] && ICON_TYPE::ICON_ITEM == m_pIcon[k][i*QUICK_SLOT_SIZEX + j]->eIcon)
				{
					if (0 >= dynamic_cast<CItemData*>(m_pIcon[k][i*QUICK_SLOT_SIZEX + j])->iCount)
						m_pIcon[k][i*QUICK_SLOT_SIZEX + j] = nullptr;
				}
			}
		}
	}
}

CQuickWnd* CQuickWnd::Create()
{
	CQuickWnd* pInstance = new CQuickWnd;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}