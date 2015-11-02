// ------------------------------------------------------------------
// Lukas Beinlich, 22.07.2014
// Grafik.cpp
// Die Hauptklasse, die die Grafik verwaltet (DirectX 11 initialisierung usw...)
// ------------------------------------------------------------------
/*
#include "src/Grafik/Grafik2.h"

// Für die DIK_ - Keys
#include <dinput.h>
#include <string>
#include <sstream>

CGrafik::CGrafik(void)
{
	m_D3D=NULL;
	m_Camera=NULL;
	m_Light=NULL;
	m_ShaderContainer=NULL;
	m_Frustrum=NULL;
	m_gui=NULL;
	m_Font2=NULL;
}

CGrafik::~CGrafik(void)
{
}

bool CGrafik::Frame(int fps, int cpu, float frameTime)
{
	if(!UpdateFps(fps,cpu))
		return false;

	if(!Update())
		return false;

	if(!Render())
		return false;
	return true;
}

bool CGrafik::Init(int screenwidth,int screenheight,HWND hWnd,CInput *input)
{
	XMMATRIX baseViewMatrix;
	int downSampleWidth=screenwidth/2;
	int downSampleHeight=screenheight/2;
	m_hWnd=hWnd;
	m_Input=input;

	m_D3D=new CD3D;
	if(!m_D3D)
		return false;
	if(!m_D3D->Init(screenwidth,screenheight,g_Options.GetVSync(),hWnd,g_Options.GetFullscreen(),g_Options.GetScreenDepth(),g_Options.GetScreenNear()))
	{
		MessageBox(hWnd,L"Fehler beim Initialisieren von D3D.",L"Fehler",MB_OK | MB_ICONERROR);
		return false;
	}
	m_Camera=new CCamera();
	if(!m_Camera)
		return false;
	m_Camera->SetPosition(0.0f,0.0f,-1.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);
	m_Camera->SetPosition(0.0f,0.0f,-5.0f);

	m_Light=new CLight;
	if(!m_Light)
		return false;
	m_Light->SetDiffuseColor(1.0f,1.0f,1.0f,1.0f);
	m_Light->SetAmbientColor(0.15f,0.15f,0.15f,0.15f);
	m_Light->SetDirection(1.0f,0.1f,0.0f);
	m_Light->SetSpecularColor(1.0f,1.0f,1.0f,1.0f);
	m_Light->SetSpecularPower(32.0f);


	m_ShaderContainer=new CSContainer();
	if(!m_ShaderContainer)
		return false;
	if(!m_ShaderContainer->Init(m_D3D->GetDevice(),hWnd))
	{
		MessageBox(hWnd, L"Could not initialize the shader.", L"Error", MB_OK);
		return false;
	}

	m_Frustrum=new CFrustrum();
	if(!m_Frustrum)
		return false;

	m_Font2 = new CFont2;
	if(!m_Font2)
		return false;
	if(!m_Font2->Init(m_D3D->GetDevice(),m_D3D->GetDeviceContext(),hWnd,baseViewMatrix,"Font/Font2.txt",L"Font/comic_sans_ms_26pt.dds"))
		return false;

	m_gui=new CGuiMain();
	if(!m_gui)
		return false;
	XMMATRIX ortho;
	m_D3D->GetOrthoMatrix(ortho);
	if(!m_gui->Init(m_D3D,m_ShaderContainer,m_Input,m_Font2,ortho))
		return false;
	m_gui->Show();

	m_SFPS = new SSentence();
	m_SCpu = new SSentence();

	if(!m_Font2->CreateSentence(m_SFPS,20,20,1.0f,0.0f,0.0f,20,"FPS: "))
		return false;
	if(!m_Font2->CreateSentence(m_SCpu,20,40,1.0f,0.0f,0.0f,20,"Cpu not availeble"))
		return false;

	return true;
}

void CGrafik::Shutdown(void)
{
	if(m_gui)
	{
		m_gui->Destroy();
		delete m_gui;
		m_gui=NULL;
	}
	m_Font2->FreeSentence(m_SFPS);
	m_Font2->FreeSentence(m_SCpu);
	if(m_Font2)
	{
		m_Font2->Shutdown();
		delete m_Font2;
		m_Font2=NULL;
	}
	if(m_Frustrum)
	{
		delete m_Frustrum;
		m_Frustrum=NULL;
	}
	if(m_Light)
	{
		delete m_Light;
		m_Light=NULL;
	}
	if(m_ShaderContainer)
	{
		m_ShaderContainer->Shutdown();
		delete m_ShaderContainer;
		m_ShaderContainer=NULL;
	}
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera=NULL;
	}
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D=NULL;
	}
}

bool CGrafik::UpdateFps(int fps,int cpu)
{
	static int lastfps=0;
	static int lastcpu=0;
	if((cpu != 0) && (cpu != lastcpu))
	{
		lastcpu = cpu;
	}
	if(fps != lastfps)
	{
		string sfps("FPS: ");
		stringstream sstream;
		sstream<<fps;
		sfps+=sstream.str();
		float r,g,b;
		if(fps > 30)
		{
			g = 1.0f;
			r = b = 0.0f;
		}
		else // 30 <= fps < 60
		{
			g = r = 1.0f;
			b = 0.0f;
		}
		if(fps < 30)
		{
			r = 1.0f;
			g = b = 0.0f;
		}
		if(!m_Font2->UpdateSentence(m_SFPS,20,20,r,g,b,20,sfps.c_str()))
			return false;
		lastfps = fps;
	}
	return true;
}

bool CGrafik::Update(void)
{
	if(!m_gui->Update())
		return false;
	return true;
}

bool CGrafik::Render(void)
{
	XMMATRIX viewmat;
	XMMATRIX projmat;
	XMMATRIX worldmat;
	XMMATRIX orthomat;
	
	m_D3D->BeginScene(0.5f,0.5f,0.5f,1.0f);

	m_D3D->GetProjectionMatrix(projmat);
	m_Camera->GetViewMatrix(viewmat);
	m_D3D->GetWorldMatrix(worldmat);
	m_D3D->GetOrthoMatrix(orthomat);

	if(!m_gui->Render(viewmat))
		return false;

	m_D3D->TurnZBufferOff();
	
	m_D3D->TurnFullAlphaBlendingOn();

	if(!m_Font2->RenderSentence(m_SFPS,worldmat,orthomat))
		return false;
	if(!m_Font2->RenderSentence(m_SCpu,worldmat,orthomat))
		return false;

	m_D3D->TurnZBufferOn();

	m_D3D->EndScene();
	return true;
}

bool CGrafik::Resize(int screenwidth,int screenheight)
{
	m_gui->Destroy();
	XMMATRIX ortho;
	m_D3D->GetOrthoMatrix(ortho);
	if(!m_gui->Init(m_D3D,m_ShaderContainer,m_Input,m_Font2,ortho))
		return false;
	return true;
}
*/
