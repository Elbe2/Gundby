// ------------------------------------------------------------------
// Graphics.h
// Lukas Beinlich, 04.10.2015
// Alles was gerendert werden soll wird hier behandelt
// ------------------------------------------------------------------
#pragma once

#include "src\Settings.h"
#include "src\Graphics\D3D.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

class Graphics
{
private:
	Settings *m_pSettings;
	D3D *m_pD3D;
public:
	Graphics(void);
	~Graphics(void);

	bool Initialize(Settings *pSettings, HWND hWnd);
	void Destroy(void);
	bool Update(void);
	bool Render(void);
};

