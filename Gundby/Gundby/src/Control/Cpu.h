// ------------------------------------------------------------------
// Lukas Beinlich, 27.07.2014
// Cpu.h
// Diese Klasse zeigt die momentane CPU-Auslastung an
// ------------------------------------------------------------------

#ifndef __CPU_H__
#define __CPU_H__

#pragma comment(lib,"pdh.lib")

#include <Pdh.h>

class CCpu
{
private:
	bool m_CanReadCPU;
	HQUERY m_QueryHandle;
	HCOUNTER m_CounterHandle;
	unsigned long m_LastSampleTime;
	long m_CPUUsage;
public:
	CCpu(void);
	~CCpu(void);

	void Init(void);
	void Shutdown(void);
	void Frame(void);
	int GetCPUPercentageUsage(void);
};

#endif //__CPU_H__
