// ------------------------------------------------------------------
// Lukas Beinlich, 02.11.2015
// Fps.h
// Diese Klasse zeigt die aktuellen FPS an
// ------------------------------------------------------------------

#pragma once
#pragma comment(lib,"winmm.lib")

#include <Windows.h>
#include <MMSystem.h>

class CFps
{
private:
	int m_Fps;
	int m_Count;
	unsigned long m_StartTime;
public:
	CFps(void);
	~CFps(void);

	void Init(void);
	void Frame(void);
	int GetFPS(void){return m_Fps;}
};
