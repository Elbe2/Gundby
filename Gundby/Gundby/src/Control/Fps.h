// ------------------------------------------------------------------
// Lukas Beinlich, 27.07.2014
// Fps.h
// Diese Klasse zeigt die aktuellen FPS an
// ------------------------------------------------------------------

#ifndef __FPS_H__
#define __FPS_H__

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

#endif //__FPS_H__
