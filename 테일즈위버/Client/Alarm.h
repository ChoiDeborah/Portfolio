#pragma once
class CAlarm
{
public:
	CAlarm() : fOldTime(0.f), fDelayTime(0.f), bIsAlarmOn(true), bIsRoof(false) {};
	~CAlarm();

public:
	void CloseAlarm();							// 알람 종료
	void SetAlarm(float fDelay, bool bRoof);	// 알람 설정
	bool CheckAlarm();							// 알람 확인
	bool GetAlarmOn();

private:
	float	fOldTime;		// 프레임 변화 시간
	float	fDelayTime;		// 프레임 변화 속도
	bool	bIsAlarmOn;		// 알람 확인
	bool	bIsRoof;		// 알람 루프 확인
};

