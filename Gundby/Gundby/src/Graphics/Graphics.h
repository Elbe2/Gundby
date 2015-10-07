// ------------------------------------------------------------------
// Graphics.h
// Lukas Beinlich, 04.10.2015
// Alles was gerendert werden soll wird hier behandelt
// ------------------------------------------------------------------
#pragma once

#include "src\Settings.h"
#include "src\Graphics\D3D.h"
#include "src\Graphics\Camera.h"
#include "src\Model\Model.h"
#include "src\Graphics\Shader\ColorShader.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

class Graphics
{
private:
	Settings *m_pSettings;
	D3D *m_pD3D;
	Camera* m_pCamera;
	Model* m_pModel;
	ColorShader* m_pColorShader;
public:
	Graphics(void);
	~Graphics(void);

	bool Initialize(Settings *pSettings, HWND hWnd);
	void Destroy(void);
	bool Update(void);
	bool Render(void);
};

