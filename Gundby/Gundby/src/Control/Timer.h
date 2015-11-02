// ------------------------------------------------------------------
// Lukas Beinlich, 02.11.2015
// Timer.h
// Ein Timer für zeitabhängige Events
// ------------------------------------------------------------------

#pragma once

#include <Windows.h>

class CTimer
{
private:
	INT64 m_Frequency;
	float m_TicksPerMs;
	INT64 m_StartTime;
	INT64 m_LastTime;
	float m_FrameTime;
public:
	CTimer(void);
	~CTimer(void);

	bool Init(void);
	void Frame(void);

	// returns time in seconds
	double GetTime(void);
	// returns frametime in milliseconds
	float GetFrameTime(void);
};
