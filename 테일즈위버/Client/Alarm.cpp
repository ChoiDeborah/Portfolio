#include "stdafx.h"
#include "Alarm.h"


CAlarm::~CAlarm()
{
}
// �˶� ����
void CAlarm::CloseAlarm()
{
	bIsRoof = false;
	bIsAlarmOn = false;
}

// �˶� ����
void CAlarm::SetAlarm(float fDelay, bool bRoof)
{
	fOldTime = 0.f;
	fDelayTime = fDelay;
	bIsRoof = bRoof;
	bIsAlarmOn = true;
}

// �˶� Ȯ��
bool CAlarm::CheckAlarm()
{
	if (bIsAlarmOn) {
		fOldTime += CTimeMgr::GetInstance()->GetDeltaTime();
		if (fDelayTime <= fOldTime) {
			if (bIsRoof)
				fOldTime = CTimeMgr::GetInstance()->GetDeltaTime();
			else
				bIsAlarmOn = false;

			return true;
		}
	}
	return false;
}

bool CAlarm::GetAlarmOn()
{
	return bIsAlarmOn;
}
