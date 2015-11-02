// ------------------------------------------------------------------
// Graphics.cpp
// Lukas Beinlich, 04.10.2015
// Alles was gerendert werden soll wird hier behandelt
// ------------------------------------------------------------------

#include "src\Graphics\Graphics.h"




Graphics::Graphics(void)
{
	m_hWnd = NULL;
	m_pSettings = NULL;
	m_pD3D = NULL;
	m_pCamera = NULL;
	m_pShaderContainer = NULL;
	m_pLight = NULL;
	m_pFrustrum = NULL;

	m_pSpriteBatch = NULL;
	m_pSpriteFont = NULL;
}


Graphics::~Graphics(void)
{
}

bool Graphics::Initialize(Settings * pSettings, HWND hWnd)
{
	bool result;

	m_hWnd = hWnd;
	m_pSettings = pSettings;

	m_pD3D = new D3D;
	if (!m_pD3D)
	{
		return false;
	}
	result = m_pD3D->Initialize(m_hWnd,m_pSettings);
	if (!result)
	{
		MessageBox(m_hWnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_pCamera = new Camera;
	if (!m_pCamera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_pCamera->SetPosition(0.0f, 0.0f, -5.0f);

	m_pShaderContainer = new CSContainer;
	if(!m_pShaderContainer)
	{
		return false;
	}
	if(!m_pShaderContainer->Init(m_pD3D->GetDevice(),hWnd))
	{
		return false;
	}

	m_pLight = new CLight;
	if (!m_pLight)
		return false;
	m_pLight->SetAmbientColor(1, 1, 1, 1);
	m_pLight->SetDiffuseColor(1, 1, 1, 1);
	m_pLight->SetDirection(0, 0, 1);
	m_pLight->SetSpecularColor(0, 0, 1, 1);
	m_pLight->SetSpecularPower(0.5);

	m_pFrustrum = new CFrustrum;
	if (!m_pFrustrum)
		return false;
	XMMATRIX proj,view;
	m_pD3D->GetProjectionMatrix(proj);
	m_pCamera->GetViewMatrix(view);
	m_pFrustrum->ConstructFrustrum(m_pSettings->GetScreenDepth(), proj, view);

	m_pSpriteBatch = new SpriteBatch(m_pD3D->GetDeviceContext());
	m_pSpriteFont = new SpriteFont(m_pD3D->GetDevice(), L"Arial.spritefont");

	return true;
}

void Graphics::Destroy(void)
{
	// Release the color shader object.
	if (m_pShaderContainer)
	{
		m_pShaderContainer->Shutdown();
		delete m_pShaderContainer;
		m_pShaderContainer = 0;
	}

	// Release the camera object.
	if (m_pCamera)
	{
		delete m_pCamera;
		m_pCamera = 0;
	}

	if (m_pLight)
	{
		delete m_pLight;
		m_pLight = NULL;
	}

	if (m_pFrustrum)
	{
		delete m_pFrustrum;
		m_pFrustrum = NULL;
	}

	// Release the Direct3D object.
	if (m_pD3D)
	{
		m_pD3D->Shutdown();
		delete m_pD3D;
		m_pD3D= 0;
	}
}

bool Graphics::Frame(int fps, int cpu, float frameTime)
{
	if (!UpdateFps(fps, cpu))
		return false;

	if (!Update())
		return false;

	if (!Render())
		return false;
	return true;
}

bool Graphics::UpdateFps(int fps,int cpu)
{
	return true;
}

bool Graphics::Update(void)
{
	return true;
}

bool Graphics::Render(void)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Clear the buffers to begin the scene.
	m_pD3D->BeginScene(0.0f, 0.5f, 0.5f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_pCamera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_pD3D->GetWorldMatrix(worldMatrix);
	m_pCamera->GetViewMatrix(viewMatrix);
	m_pD3D->GetProjectionMatrix(projectionMatrix);

	m_pSpriteBatch->Begin();
	m_pSpriteFont->DrawString(m_pSpriteBatch, L"Hello World", XMFLOAT2(100, 100));
	m_pSpriteBatch->End();

	m_pD3D->EndScene();

	return true;
}
