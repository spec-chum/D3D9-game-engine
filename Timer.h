#ifndef _ctimer_h
#define _ctimer_h

#include "base.h"

class CTimer
{	
	__int64 m_CurrentTime;
	__int64 m_OldTime;
	__int64 m_Ticks;
	float m_fFPS;
	float m_pFPSBuffer[50];
	float m_fUpdateInterval;
	int m_FPSIndex;

public:
	__int64 m_StartTime;
	__int64 m_EndTime;
	float m_TimeDelta;
	int m_FrameCounter;

	void StartTimer();
	void StopTimer() const;
	void SetTrigger(const float seconds) { m_fUpdateInterval = seconds; };
	BOOL CheckTrigger();
	__int64 GetTime();
	float GetTimeDelta();
	float GetFPS() const { return m_fFPS; };
	float CalcFPS();
	float GetTimerDuration() { return (((float)m_EndTime - (float)m_StartTime)/(float)m_Ticks); };
	CTimer();
	~CTimer();
};

#endif