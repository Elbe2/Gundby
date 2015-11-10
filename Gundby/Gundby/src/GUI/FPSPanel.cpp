//-------------------------------------------------------------------
// FPSPanel.h
// Lukas Beinlich, 04.11.2015
// Zeigt verschiedene Informationen wie FPS, Cpu-Nutzung, Ping und Kram an
// ------------------------------------------------------------------

#include "src\GUI\FPSPanel.h"
#include <sstream>
using namespace std;

bool CFPSPanel::Init(SpriteBatch * pSpriteBatch, SpriteFont * pSpriteFont, CObjektManger *pManager)
{
	m_pFps = new CFontPanel;
	if (!m_pFps)
		return false;
	if (!m_pFps->Init(pSpriteBatch, pSpriteFont))
		return false;
	m_pFps->SetText(L"Fps: -15");
	m_pFps->SetPosition(10, 10);
	m_pFps->SetSize(10);
	m_pFps->SetVisible();
	pManager->AddElement(m_pFps);

	m_pCpu = new CFontPanel;
	if (!m_pCpu)
		return false;
	if (!m_pCpu->Init(pSpriteBatch, pSpriteFont))
		return false;
	m_pCpu->SetText(L"Cpu: 120%");
	m_pCpu->SetPosition(10, 22);
	m_pCpu->SetSize(10);
	m_pCpu->SetVisible();
	pManager->AddElement(m_pCpu);

	m_pTime = new CFontPanel;
	if (!m_pTime)
		return false;
	if (!m_pTime->Init(pSpriteBatch, pSpriteFont))
		return false;
	m_pTime->SetText(L"Zeit: 0");
	m_pTime->SetPosition(10, 34);
	m_pTime->SetSize(10);
	m_pTime->SetVisible();
	pManager->AddElement(m_pTime);

	return true;
}

void CFPSPanel::Destroy(void)
{
	if (m_pFps)
	{
		delete m_pFps;
		m_pFps = NULL;
	}
	if (m_pCpu)
	{
		delete m_pCpu;
		m_pCpu = NULL;
	}
	if (m_pTime)
	{
		delete m_pTime;
		m_pTime = NULL;
	}
}

void CFPSPanel::Update(int cpu, int fps, float time)
{
	wstringstream wss;
	wss << L"Fps: " << fps;
	m_pFps->SetText(wss.str());
	wss.str(L"");
	wss << L"Cpu: " << cpu;
	m_pCpu->SetText(wss.str());
	wss.str(L"");
	wss << L"Time: " << time;
	m_pTime->SetText(wss.str());
}


