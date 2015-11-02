// ------------------------------------------------------------------
// Graphics.h
// Lukas Beinlich, 04.10.2015
// Alles was gerendert werden soll wird hier behandelt
// ------------------------------------------------------------------
#pragma once

#include "src\Settings.h"
#include "src\Graphics\D3D.h"
#include "src\Graphics\Camera.h"
#include "src\Model\MeshModel.h"
#include "src\Graphics\Shader\SContainer.h"
#include "src\Graphics\Font\Font2.h"
#include "src\Graphics\Light.h"
#include "src\Graphics\FrustrumCulling.h"
#include "SpriteFont.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

class Graphics
{
private:
	HWND m_hWnd;
	Settings *m_pSettings;
	D3D *m_pD3D;
	Camera* m_pCamera;
	CSContainer *m_pShaderContainer;
	CLight *m_pLight;
	CFrustrum *m_pFrustrum;

	SpriteBatch *m_pSpriteBatch;
	SpriteFont *m_pSpriteFont;

	bool Render(void);
	bool Update(void);
	bool UpdateFps(int fps, int cpu);
public:
	Graphics(void);
	~Graphics(void);

	bool Initialize(Settings *pSettings, HWND hWnd);
	void Destroy(void);
	bool Frame(int fps, int cpu, float frameTime);
};

