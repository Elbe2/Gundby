// ------------------------------------------------------------------
// Lukas Beinlich, 27.07.2014
// Timer.h
// Ein Timer für zeitabhängige Events
// ------------------------------------------------------------------

#include "src\Control\Timer.h"

CTimer::CTimer(void)
{
}

CTimer::~CTimer(void)
{
}

bool CTimer::Init(void)
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_Frequency);
	if(m_Frequency==0)
		return false;
	m_TicksPerMs=(float)(m_Frequency/1000);
	QueryPerformanceCounter((LARGE_INTEGER*)&m_StartTime);
	m_LastTime=m_StartTime;
	return true;
}

void CTimer::Frame(void)
{
	INT64 currentTime;
	float timeDiff;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	timeDiff=(float)(currentTime-m_LastTime);
	m_FrameTime=timeDiff/m_TicksPerMs;
	m_LastTime=currentTime;
}

double CTimer::GetTime()
{
	INT64 currentTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	INT64 time=m_StartTime-currentTime;
	return time/m_Frequency;
}

float CTimer::GetFrameTime(void)
{
	return m_FrameTime;
}
