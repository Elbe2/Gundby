//-------------------------------------------------------------------
// FPSPanel.h
// Lukas Beinlich, 04.11.2015
// Zeigt verschiedene Informationen wie FPS, Cpu-Nutzung, Ping und Kram an
// ------------------------------------------------------------------

#pragma once

#include "src\GUI\FontPanel.h"
#include "src\Manager\Objektmanager.h"
#include "SpriteFont.h"
using namespace DirectX;

class CFPSPanel : public CObjekt
{
protected:
	CFontPanel *m_pFps;
	CFontPanel *m_pCpu;
	CFontPanel *m_pTime;
public:
	bool Init(SpriteBatch *pSpriteBatch, SpriteFont *pSpriteFont,CObjektManger *pManager);
	bool Render(void) { return true; }
	bool RenderFont(void) { return true; }
	void Destroy(void);

	void Update(int cpu, int fps, float time);
};
