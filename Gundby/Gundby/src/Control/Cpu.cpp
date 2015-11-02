// ------------------------------------------------------------------
// Cpu.cpp
// Lukas Beinlich, 08.10.2015
// Diese Klasse zeigt die momentane CPU-Auslastung an (wenn möglich)
// ------------------------------------------------------------------

#include "src\Control\Cpu.h"

CCpu::CCpu(void)
{
}

CCpu::~CCpu(void)
{
}

void CCpu::Init(void)
{
	m_CanReadCPU=true;
	if(PdhOpenQuery(NULL,0,&m_QueryHandle)!=ERROR_SUCCESS)
	{
		m_CanReadCPU=false;
	}
	if(PdhAddCounter(m_QueryHandle, TEXT("\\Processor(_Total)\\% processor time"), 0, &m_CounterHandle)!=ERROR_SUCCESS)
		m_CanReadCPU=false;
	m_LastSampleTime=GetTickCount();
	m_CPUUsage=0;
}

void CCpu::Shutdown(void)
{
	if(m_CanReadCPU)
		PdhCloseQuery(m_QueryHandle);
}

void CCpu::Frame(void)
{
	PDH_FMT_COUNTERVALUE value;
	if(m_CanReadCPU)
	{
		if((m_LastSampleTime + 1000) < GetTickCount())
		{
			m_LastSampleTime=GetTickCount();
			PdhCollectQueryData(m_QueryHandle);
			PdhGetFormattedCounterValue(m_CounterHandle,PDH_FMT_LONG,NULL,&value);
			m_CPUUsage=value.longValue;
		}
	}
}

int CCpu::GetCPUPercentageUsage(void)
{
	int usage;
	if(m_CanReadCPU)
		usage=(int)m_CPUUsage;
	else
		usage=0;
	return usage;
}
