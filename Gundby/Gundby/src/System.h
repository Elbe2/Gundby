// ------------------------------------------------------------------
// System.h
// Lukas Beinlich, 04.10.2015
// Die Hauptklasse für die Anwendung
// ------------------------------------------------------------------
#pragma once

// I dont need everything, so we can speed up the build process
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "src\Control\Cpu.h"
#include "src\Control\Timer.h"
#include "src\Control\Fps.h"

#include "src\Settings.h"
#include "src\Input\Input.h"
#include "src\Graphics\Graphics.h"
#include "src\Manager\Objektmanager.h"
#include "src\GUI\FontPanel.h"

class System
{
private:
	Settings *m_pSettings;
	LPCWSTR m_Name;
	HINSTANCE m_hInstance;
	HWND m_hWnd;

	CInput* m_pInput;
	Graphics* m_pGraphics;
	CObjektManger *m_pManager;

	CFps *m_pFps;
	CCpu *m_pCpu;
	CTimer *m_pTimer;

	CFontPanel *m_pCpuPanel;
	CFontPanel *m_pTimerPanel;
	CFontPanel *m_pFpsPanel;

	bool Frame(void);
	void InitializeWindows(void);
	void DestroyWindows(void);

public:
	System(void);
	~System(void);

	bool Initialize(void);
	void Destroy(void);
	void Run(void);

	LRESULT CALLBACK MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

// Important Callback, because you cant set the member of a class as a callback, so we actually need to call it manually
static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// The global Handle
static System* g_pSystem = 0;
