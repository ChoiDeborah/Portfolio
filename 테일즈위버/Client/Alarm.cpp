#include "stdafx.h"
#include "Alarm.h"


CAlarm::~CAlarm()
{
}
// 알람 종료
void CAlarm::CloseAlarm()
{
	bIsRoof = false;
	bIsAlarmOn = false;
}

// 알람 설정
void CAlarm::SetAlarm(float fDelay, bool bRoof)
{
	fOldTime = 0.f;
	fDelayTime = fDelay;
	bIsRoof = bRoof;
	bIsAlarmOn = true;
}

// 알람 확인
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
