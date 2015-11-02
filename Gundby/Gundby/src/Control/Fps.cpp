// ------------------------------------------------------------------
// Lukas Beinlich, 27.07.2014
// Fps.h
// Diese Klasse zeigt die aktuellen FPS an
// ------------------------------------------------------------------

#include "src\Control\Fps.h"

CFps::CFps(void)
{
}

CFps::~CFps(void)
{
}

void CFps::Init(void)
{
	m_Fps=0;
	m_Count=0;
	m_StartTime=timeGetTime();
}

void CFps::Frame(void)
{
	m_Count++;
	if(timeGetTime()>=(m_StartTime + 1000))
	{
		m_Fps=m_Count;
		m_Count=0;
		m_StartTime=timeGetTime();
	}
}
