#include "timer.h"

CTimer::CTimer()
{
	Log.LogFile << "Constructing Timer object...OK\n\n";
	
	m_CurrentTime = 0;
	m_TimeDelta = 0;
	m_OldTime = 0;
	m_FrameCounter = 0;
	m_fFPS = 0;
	m_FPSIndex = 0;
	m_fUpdateInterval = 1.0f;
	
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_Ticks);
	StartTimer();
}

CTimer::~CTimer()
{
	StopTimer();
	Log.LogFile << "Releasing Timer...OK.\n\n";
}

void CTimer::StartTimer()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&m_CurrentTime);
	m_StartTime = m_OldTime = m_CurrentTime;
}

void CTimer::StopTimer() const
{
	QueryPerformanceCounter((LARGE_INTEGER*)&m_EndTime);
}

BOOL CTimer::CheckTrigger()
{
	if(static_cast<float>(GetTimeDelta()) < m_fUpdateInterval)
		return false;
	return true;
}
__int64 CTimer::GetTime()
{
	m_OldTime = m_CurrentTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&m_CurrentTime);
	
	return m_CurrentTime;
}

float CTimer::GetTimeDelta()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&m_CurrentTime);
	m_TimeDelta = (static_cast<float>(m_CurrentTime) - static_cast<float>(m_OldTime))/static_cast<float>(m_Ticks);

	m_OldTime = m_CurrentTime;

	return m_TimeDelta;
}

float CTimer::CalcFPS()
{
	m_FrameCounter++;
	GetTimeDelta();
  
	if(m_FPSIndex++ > 49)
		m_FPSIndex = 0;
	m_pFPSBuffer[m_FPSIndex] = static_cast<float>(m_FrameCounter) / static_cast<float>(m_TimeDelta);
	
	if(m_TimeDelta > m_fUpdateInterval)
	{
		m_fFPS = (m_pFPSBuffer[10] + m_pFPSBuffer[20] + m_pFPSBuffer[30] + m_pFPSBuffer[40]) * 0.25f;
		m_FrameCounter = 0;
		m_OldTime = m_CurrentTime;
	}
	return m_fFPS;
	
}