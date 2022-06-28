#include "stdafx.h"
#include "EventObj.h"


CEventObj::CEventObj(EVENT_NOTIFY* pNotify) 
	: m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pTimeMgr(CTimeMgr::GetInstance())
{
	if (nullptr == pNotify) {
		Release();
		delete this;
		return;
	}

	m_tCurState = *pNotify;
	m_vecNotify.push_back(pNotify);

	m_tFrame.iFrameCur = 0;
	m_iCurCount = 0;

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_tCurState.wstrObjKey, m_tCurState.wstrStateKey);
	m_tFrame.iFrameEnd = m_pTextureMgr->GetTexCount(m_tCurState.wstrObjKey, m_tCurState.wstrStateKey);
	NULL_CHECK(pTexInfo);
	m_tFrame.fFrameDelay = pTexInfo->m_fFrameDelay;
}

CEventObj::~CEventObj()
{
	Release();
}

void CEventObj::Release()
{
	for (auto& Notify : m_vecNotify) {
		if (Notify) {
			delete Notify;
			Notify = nullptr;
		}
	}
	m_vecNotify.clear();
	m_vecNotify.shrink_to_fit();
}

void CEventObj::InsertNotify(EVENT_NOTIFY* pNotify)
{
	NULL_CHECK(pNotify);
	m_vecNotify.push_back(pNotify);
}

void CEventObj::ResetNotify() {
	if (m_vecNotify.empty())
		return;

	m_tCurState = *m_vecNotify[0];

	m_pNextNotify = nullptr;

	m_tFrame.iFrameCur = 0;
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_tCurState.wstrObjKey, m_tCurState.wstrStateKey);
	m_tFrame.iFrameEnd = m_pTextureMgr->GetTexCount(m_tCurState.wstrObjKey, m_tCurState.wstrStateKey);
	if (nullptr != pTexInfo)
		m_tFrame.fFrameDelay = pTexInfo->m_fFrameDelay;

	m_iCurCount = 0;
	m_iNextCount = 0;
}

void CEventObj::UpdateNotify(float fTime) {
	if (nullptr == m_pNextNotify)
		FindNextNotify(fTime);

	// 시간이 지나면 다음 노드 찾음
	if (m_iCurCount != m_iNextCount && nullptr != m_pNextNotify)
	{
		// 갱신
		if ( fTime > m_pNextNotify->fTime)
		{
			if (m_iCurCount != m_vecNotify.size())
				FindNextNotify(fTime);
		}

		// 이동
		else if (m_tCurState.bIsMove)
		{
			float fEndTime = m_pNextNotify->fTime - m_vecNotify[m_iNextCount-1]->fTime;

			if (0 != int(fEndTime)) {
				m_vDir = (m_pNextNotify->vPos - m_vecNotify[m_iNextCount-1]->vPos);

				// 도착시간, 현재시간
				float fCurTime = fTime - m_vecNotify[m_iNextCount-1]->fTime;

				// 현재 위치 보간
				m_tCurState.vPos = m_pNextNotify->vPos - m_vDir * (1.f - (fCurTime / fEndTime));
			}
			else {
				m_tCurState.vPos = m_pNextNotify->vPos;
			}
		}
	}

	/*if (m_iViewObj)
		CViewMgr::GetInstance()->SetViewObj(m_tCurState.vPos);*/

	if (!m_tCurState.bIsActive || 0.f > m_tFrame.fFrameDelay)
		return;

	m_tFrame.fFrameTime += m_pTimeMgr->GetDeltaTime();
	if (m_tFrame.fFrameDelay < m_tFrame.fFrameTime) {
		m_tFrame.fFrameTime = 0.f;
		++m_tFrame.iFrameCur;

		if (m_tFrame.iFrameCur >= m_tFrame.iFrameEnd) {
			m_tFrame.iFrameCur = 0;
		}
	}
}

void CEventObj::FindNextNotify(float fTime)
{
	bool bIsFind = false;

	// 다음 노드
	for (size_t i = m_iCurCount + 1; i < m_vecNotify.size(); ++i)
	{
		if (nullptr == m_vecNotify[i])
			continue;

		if (fTime <= m_vecNotify[i]->fTime)
		{
			m_pNextNotify = m_vecNotify[i];
			m_iNextCount = i;

			bIsFind = true;

			break;
		}
	}

	if (bIsFind)
	{
		m_iCurCount = m_iNextCount - 1;
		m_tCurState = *m_vecNotify[m_iNextCount - 1];
	}
	else
	{
		m_iCurCount		= m_iNextCount = m_vecNotify.size() - 1;
		m_pNextNotify	= m_vecNotify[m_iNextCount];
		m_tCurState		= *m_pNextNotify;
	}

	m_tFrame.iFrameCur = 0;
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(m_tCurState.wstrObjKey, m_tCurState.wstrStateKey);
	m_tFrame.iFrameEnd = m_pTextureMgr->GetTexCount(m_tCurState.wstrObjKey, m_tCurState.wstrStateKey);
	if (nullptr != pTexInfo)
		m_tFrame.fFrameDelay = pTexInfo->m_fFrameDelay;
}


