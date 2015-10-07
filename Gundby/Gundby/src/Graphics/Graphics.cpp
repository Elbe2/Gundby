// ------------------------------------------------------------------
// Graphics.cpp
// Lukas Beinlich, 04.10.2015
// Alles was gerendert werden soll wird hier behandelt
// ------------------------------------------------------------------

#include "src\Graphics\Graphics.h"


Graphics::Graphics(void)
{
	m_pD3D = NULL;
	m_pCamera = NULL;
	m_pModel = NULL;
	m_pColorShader = NULL;
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

	// Create the camera object.
	m_pCamera = new Camera;
	if (!m_pCamera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_pCamera->SetPosition(0.0f, 0.0f, -5.0f);

	// Create the model object.
	m_pModel = new Model;
	if (!m_pModel)
	{
		return false;
	}

	// Initialize the model object.
	result = m_pModel->Initialize(m_pD3D->GetDevice());
	if (!result)
	{
		MessageBox(hWnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the color shader object.
	m_pColorShader = new ColorShader;
	if (!m_pColorShader)
	{
		return false;
	}

	// Initialize the color shader object.
	result = m_pColorShader->Initialize(m_pD3D->GetDevice(), hWnd);
	if (!result)
	{
		MessageBox(hWnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void Graphics::Destroy(void)
{
	// Release the color shader object.
	if (m_pColorShader)
	{
		m_pColorShader->Destroy();
		delete m_pColorShader;
		m_pColorShader = 0;
	}

	// Release the model object.
	if (m_pModel)
	{
		m_pModel->Shutdown();
		delete m_pModel;
		m_pModel = 0;
	}

	// Release the camera object.
	if (m_pCamera)
	{
		delete m_pCamera;
		m_pCamera = 0;
	}
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
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;


	// Clear the buffers to begin the scene.
	m_pD3D->BeginScene(0.0f, 0.5f, 0.5f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_pCamera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_pD3D->GetWorldMatrix(worldMatrix);
	m_pCamera->GetViewMatrix(viewMatrix);
	m_pD3D->GetProjectionMatrix(projectionMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_pModel->Render(m_pD3D->GetDeviceContext());

	// Render the model using the color shader.
	result = m_pColorShader->Render(m_pD3D->GetDeviceContext(), m_pModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	m_pD3D->EndScene();

	return true;
}
