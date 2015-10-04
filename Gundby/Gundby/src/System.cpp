// ------------------------------------------------------------------
// System.cpp
// Lukas Beinlich, 04.10.2015
// Die Hauptklasse für die Anwendung
// ------------------------------------------------------------------

#include "src/System.h"

bool System::Frame(void)
{
	bool result;
	
	if (m_pInput->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	result = m_pGraphics->Update();
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

	m_pInput = new Input;
	if (!m_pInput)
	{
		return false;
	}
	m_pInput->Initialize();

	m_pGraphics = new Graphics;
	if (!m_pGraphics)
	{
		return false;
	}
	result = m_pGraphics->Initialize(m_pSettings,m_hWnd);
	if (!result)
	{
		return false;
	}

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

	}
}

LRESULT System::MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		m_pInput->KeyDown((unsigned int)wParam);
		return 0;
	case WM_KEYUP:
		m_pInput->KeyUp((unsigned int)wParam);
		return 0;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
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
