#pragma once
class CAlarm
{
public:
	CAlarm() : fOldTime(0.f), fDelayTime(0.f), bIsAlarmOn(true), bIsRoof(false) {};
	~CAlarm();

public:
	void CloseAlarm();							// �˶� ����
	void SetAlarm(float fDelay, bool bRoof);	// �˶� ����
	bool CheckAlarm();							// �˶� Ȯ��
	bool GetAlarmOn();

private:
	float	fOldTime;		// ������ ��ȭ �ð�
	float	fDelayTime;		// ������ ��ȭ �ӵ�
	bool	bIsAlarmOn;		// �˶� Ȯ��
	bool	bIsRoof;		// �˶� ���� Ȯ��
};

