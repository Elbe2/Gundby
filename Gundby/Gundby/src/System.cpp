// ------------------------------------------------------------------
// System.cpp
// Lukas Beinlich, 04.10.2015
// Die Hauptklasse für die Anwendung
// ------------------------------------------------------------------

#include "src/System.h"

bool System::Frame(void)
{
	bool result;
	
	bool ret = true;
	int mouseX;
	int mouseY;

	if (!m_pInput->Frame())
		return false;

	m_pInput->GetMouseCoord(mouseX, mouseY);

	m_pTimer->Frame();
	m_pFps->Frame();
	m_pCpu->Frame();
	
	int cpu, fps;
	float ftime;

	ftime = m_pTimer->GetFrameTime();
	cpu = m_pCpu->GetCPUPercentageUsage();
	fps = m_pFps->GetFPS();

	m_pFpsPanel->Update(cpu, fps, ftime);

	result = m_pGraphics->Frame(fps,cpu,ftime);
	if (!result)
	{
		return false;
	}

	return true;
}

void System::InitializeWindows(void)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	m_hInstance = GetModuleHandle(NULL);

	m_Name = L"Gundby";

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_Name;
	wc.cbSize = sizeof(WNDCLASSEX);
	RegisterClassEx(&wc);
	if (m_pSettings->GetFullscreen())
	{
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)m_pSettings->GetScreenWidth();
		dmScreenSettings.dmPelsHeight = (unsigned long)m_pSettings->GetScreenHeight();
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		posX = posY = 0;
	}
	else
	{
		posX = (GetSystemMetrics(SM_CXSCREEN) - m_pSettings->GetScreenWidth()) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - m_pSettings->GetScreenHeight()) / 2;
	}

	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, m_Name, m_Name, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, posX, posY, m_pSettings->GetScreenWidth(), m_pSettings->GetScreenHeight(), NULL, NULL, m_hInstance, NULL);

	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	// Hide the mouse cursor.
	ShowCursor(false);
}

void System::DestroyWindows(void)
{
	ShowCursor(true);

	if (m_pSettings->GetFullscreen())
	{
		ChangeDisplaySettings(NULL, 0);
	}

	DestroyWindow(m_hWnd);
	m_hWnd = NULL;

	UnregisterClass(m_Name, m_hInstance);
	m_hInstance = NULL;
}

System::System(void)
{
	m_pInput = NULL;
	m_pGraphics = NULL;
	m_pFps = NULL;
	m_pTimer = NULL;
	m_pCpu = NULL;
}

System::~System(void)
{
}

bool System::Initialize(void)
{
	g_pSystem = this;

	m_pSettings = new Settings();
	if (!m_pSettings) return false;
	if (!m_pSettings->Initialize()) return false;

	bool result;

	InitializeWindows();

	m_pInput = new CInput;
	if (!m_pInput)
	{
		return false;
	}
	m_pInput->Init(m_hInstance,m_hWnd,m_pSettings->GetScreenWidth(),m_pSettings->GetScreenHeight());

	m_pManager = new CObjektManger;

	m_pGraphics = new Graphics;
	if (!m_pGraphics)
	{
		return false;
	}
	result = m_pGraphics->Initialize(m_pSettings,m_hWnd,m_pManager);
	if (!result)
	{
		return false;
	}

	m_pCpu = new CCpu();
	if (!m_pCpu)
		return false;
	m_pCpu->Init();

	m_pFps = new CFps();
	if (!m_pFps)
		return false;
	m_pFps->Init();

	m_pTimer = new CTimer();
	if (!m_pTimer)
		return false;
	if (!m_pTimer->Init())
		return false;

	m_pFpsPanel = new CFPSPanel;
	if (!m_pFpsPanel)
		return false;
	if (!m_pFpsPanel->Init(m_pGraphics->GetSpriteBatch(), m_pGraphics->GetSpriteFont(),m_pManager))
		return false;
	m_pManager->AddElement(m_pFpsPanel);

	return true;
}

void System::Destroy(void)
{
	if (m_pGraphics)
	{
		m_pGraphics->Destroy();
		delete m_pGraphics;
		m_pGraphics = 0;
	}

	if (m_pInput)
	{
		delete m_pInput;
		m_pInput = 0;
	}

	if (m_pTimer)
	{
		delete m_pTimer;
		m_pTimer = NULL;
	}

	if (m_pFps)
	{
		delete m_pFps;
		m_pFps = NULL;
	}

	if (m_pCpu)
	{
		m_pCpu->Shutdown();
		delete m_pCpu;
		m_pCpu = NULL;
	}

	if (m_pManager)
	{
		m_pManager->Destroy();
		delete m_pManager;
		m_pManager = NULL;
	}

	if (m_pFpsPanel)
	{
		delete m_pFpsPanel;
		m_pFpsPanel = NULL;
	}

	DestroyWindows();
	g_pSystem = NULL;
}

void System::Run(void)
{
	MSG msg;
	bool result;

	ZeroMemory(&msg, sizeof(MSG));

	bool done = false;
	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}
		if (m_pInput->IsEscapePressed())
			done = true;
		Sleep(0);
	}
}

LRESULT System::MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	default:
		return g_pSystem->MessageHandler(hWnd, msg, wParam, lParam);
	}
}
