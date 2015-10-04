// ------------------------------------------------------------------
// Graphics.cpp
// Lukas Beinlich, 04.10.2015
// Alles was gerendert werden soll wird hier behandelt
// ------------------------------------------------------------------

#include "src\Graphics\Graphics.h"


Graphics::Graphics(void)
{
	m_pD3D = NULL;
}


Graphics::~Graphics(void)
{
}

bool Graphics::Initialize(Settings * pSettings, HWND hWnd)
{
	bool result;

	m_pSettings = pSettings;

	m_pD3D = new D3D;
	if (!m_pD3D)
	{
		return false;
	}
	result = m_pD3D->Initialize(hWnd,m_pSettings);
	if (!result)
	{
		MessageBox(hWnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	return true;
}

void Graphics::Destroy(void)
{
	// Release the Direct3D object.
	if (m_pD3D)
	{
		m_pD3D->Shutdown();
		delete m_pD3D;
		m_pD3D= 0;
	}
}

bool Graphics::Update(void)
{
	bool result;

	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool Graphics::Render(void)
{
	m_pD3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	m_pD3D->EndScene();

	return true;
}
