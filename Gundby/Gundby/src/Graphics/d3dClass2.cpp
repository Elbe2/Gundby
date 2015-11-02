// ------------------------------------------------------------------
// Lukas Beinlich, 22.07.2014
// d3dClass.cpp
// Verwaltet den ganzen DirectX Kram
// ------------------------------------------------------------------

#include "src\Graphics\d3dClass2.h"

CD3D::CD3D(void)
{
	m_SwapChain=NULL;
	m_Device=NULL;
	m_DeviceContext=NULL;
	m_RenderTargetView=NULL;
	m_DepthStencilBuffer=NULL;
	m_DepthStencilState=NULL;
	m_DepthDisabledStencilState=NULL;
	m_DepthStencilView=NULL;
	m_RasterState=NULL;
	m_AlphaDisableBlendingState=NULL;
	m_AlphaEnableBlendingState=NULL;
	m_DisplayModeList=NULL;

	m_Hwnd=NULL;
	m_Numerator=0;
	m_Denominator=0;
}

CD3D::~CD3D(void)
{
}

bool CD3D::Init(int screenWidth,int screenHeight,bool vsync,HWND hwnd,bool fullscreen,float screenDepth,float screenNear)
{
	IDXGIFactory *Factory;
	IDXGIAdapter *Adapter;
	IDXGIOutput *AdapterOutput;
	unsigned int NumModes;
	unsigned int i;
	unsigned int StringLen;
	DXGI_ADAPTER_DESC AdapterDesc;
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	D3D_FEATURE_LEVEL FeatureLevel;
	ID3D11Texture2D *BackBufferPtr;
	D3D11_BLEND_DESC BlendStateDesc;

	m_VSync=vsync;
	m_Hwnd=hwnd;
	// Die DirectX Factory erstellen
	if(FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory),(void**)&Factory)))
	{
		MessageBox(hwnd,L"Fehler beim erstellen der Factory.",L"Error",MB_OK | MB_ICONERROR);
		return false;
	}
	// Damit können wir die Grafikkarten auflisten
	if(FAILED(Factory->EnumAdapters(0,&Adapter)))
	{
		MessageBox(hwnd,L"Fehler beim auflisten der Adapter.",L"Error",MB_OK | MB_ICONERROR);
		return false;
	}
	// und damit dann auch die Monitore
	if(FAILED(Adapter->EnumOutputs(0,&AdapterOutput)))
	{
		MessageBox(hwnd,L"Fehler beim auflisten der Monitore.",L"Error",MB_OK | MB_ICONERROR);
		return false;
	}
	// Anschließend listen wir die Modi auf, die mit dem Monitor funktionieren
	if(FAILED(AdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,DXGI_ENUM_MODES_INTERLACED,&NumModes,NULL)))
	{
		MessageBox(hwnd,L"Fehler beim herausfinden der Anzahl der Display Modes.",L"Error",MB_OK | MB_ICONERROR);
		return false;
	}
	// Display Mode List erstellen
	if(!(m_DisplayModeList = new DXGI_MODE_DESC[NumModes]))
	{
		MessageBox(hwnd,L"Fehler beim erstellen der Anzahl der Display Modes.",L"Error",MB_OK | MB_ICONERROR);
		return false;
	}
	// Jetzt die Liste füllen
	if(FAILED(AdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,DXGI_ENUM_MODES_INTERLACED,&NumModes,m_DisplayModeList)))
	{
		MessageBox(hwnd,L"Fehler beim erstellen der Display Modes.",L"Error",MB_OK | MB_ICONERROR);
		return false;
	}
	// Jetzt alle Display Modes durchsuchen, nach den richtigen und die Frequenzen merken
	for(i=0;i<NumModes;i++)
	{
		if(m_DisplayModeList[i].Width == (unsigned int) screenWidth)
		{
			if(m_DisplayModeList[i].Height == (unsigned int) screenHeight)
			{
				m_Numerator=m_DisplayModeList[i].RefreshRate.Numerator;
				m_Denominator=m_DisplayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// Video Card Information
	if(FAILED(Adapter->GetDesc(&AdapterDesc)))
	{
		MessageBox(hwnd,L"Fehler beim Herausfinden der VideoCardDesc.",L"Error",MB_OK | MB_ICONERROR);
		return false;
	}
	m_VidMem=(int)(AdapterDesc.DedicatedVideoMemory / 1024); // in Kb schätzungsweise
	if(wcstombs_s(&StringLen,m_VidCardDesc,128,AdapterDesc.Description,128) != 0)
	{
		MessageBox(hwnd,L"Zu doof zum konvertieren...",L"Error",MB_OK | MB_ICONERROR);
		return false;
	}

	// Aufräumen
	AdapterOutput->Release();
	AdapterOutput=NULL;
	Adapter->Release();
	Adapter=NULL;
	Factory->Release();
	Factory=NULL;

	// Hier beginnt die DX initialisierung
	// Swapchain und Buffer initialisieren
	ZeroMemory(&SwapChainDesc,sizeof(SwapChainDesc));
	SwapChainDesc.BufferCount=1;
	SwapChainDesc.BufferDesc.Height=screenHeight;
	SwapChainDesc.BufferDesc.Width=screenWidth;
	SwapChainDesc.BufferDesc.Format=DXGI_FORMAT_R8G8B8A8_UNORM;
	// Wenn Vsynch an -> refreshrate vom Monitor (wie oben herausgefunden), sonst so oft wie möglich
	if(m_VSync)
	{
		SwapChainDesc.BufferDesc.RefreshRate.Denominator=m_Denominator;
		SwapChainDesc.BufferDesc.RefreshRate.Numerator=m_Numerator;
	}
	else
	{
		SwapChainDesc.BufferDesc.RefreshRate.Denominator=1;
		SwapChainDesc.BufferDesc.RefreshRate.Numerator=0;
	}
	SwapChainDesc.BufferUsage=DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.OutputWindow=hwnd;
	// Kein Multisampling...
	SwapChainDesc.SampleDesc.Count=1;
	SwapChainDesc.SampleDesc.Quality=0;
	if(fullscreen)
		SwapChainDesc.Windowed=false;
	else
		SwapChainDesc.Windowed=true;
	SwapChainDesc.BufferDesc.ScanlineOrdering=DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChainDesc.BufferDesc.Scaling=DXGI_MODE_SCALING_UNSPECIFIED;
	SwapChainDesc.SwapEffect=DXGI_SWAP_EFFECT_DISCARD;
	SwapChainDesc.Flags=DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	FeatureLevel=D3D_FEATURE_LEVEL_11_0;
	
	// Und jetzt das Device und den Device Context erstellen
	// Und natürlich die SwapChain
	if(FAILED(D3D11CreateDeviceAndSwapChain(NULL,D3D_DRIVER_TYPE_HARDWARE,NULL,0,&FeatureLevel,1,D3D11_SDK_VERSION,&SwapChainDesc,&m_SwapChain,&m_Device,NULL,&m_DeviceContext)))
	{
		MessageBox(hwnd,L"Fehler beim erstellen des Device/DeviceContext/SwapChain. Ist DirectX11 unterstützt?",L"Fehler",MB_OK | MB_ICONERROR);
		return false;
	}

	// Jetzt den BackBuffer erstellen
	if(FAILED(m_SwapChain->GetBuffer(0,__uuidof(ID3D11Texture2D),(LPVOID*)&BackBufferPtr)))
	{
		MessageBox(hwnd,L"Fehler beim erstellen des Backbuffers.",L"Error",MB_OK | MB_ICONERROR);
		return false;
	}
	if(FAILED(m_Device->CreateRenderTargetView(BackBufferPtr,NULL,&m_RenderTargetView)))
	{
		MessageBox(hwnd,L"Fehler beim erstellen des RTV.",L"Error",MB_OK | MB_ICONERROR);
		return false;
	}
	BackBufferPtr->Release();
	BackBufferPtr=NULL;

	if(!CreateDepthBuffer())
		return false;

	if(!CreateDepthStencilBuffer())
		return false;

	if(!CreateDisabledDepthStencilBuffer())
		return false;

	if(!CreateDepthStencilView())
		return false;
	
	if(!CreateRasterizerState())
		return false;
	
	ZeroMemory(&BlendStateDesc,sizeof(BlendStateDesc));

	BlendStateDesc.RenderTarget[0].BlendEnable=TRUE;
	BlendStateDesc.RenderTarget[0].SrcBlend=D3D11_BLEND_ONE;
	BlendStateDesc.RenderTarget[0].DestBlend=D3D11_BLEND_INV_SRC_ALPHA;
	BlendStateDesc.RenderTarget[0].BlendOp=D3D11_BLEND_OP_ADD;
	BlendStateDesc.RenderTarget[0].SrcBlendAlpha=D3D11_BLEND_ONE;
	BlendStateDesc.RenderTarget[0].DestBlendAlpha=D3D11_BLEND_ZERO;
	BlendStateDesc.RenderTarget[0].BlendOpAlpha=D3D11_BLEND_OP_ADD;
	BlendStateDesc.RenderTarget[0].RenderTargetWriteMask=0x0f;
	if(FAILED(m_Device->CreateBlendState(&BlendStateDesc,&m_AlphaEnableBlendingState)))
		return false;
	BlendStateDesc.RenderTarget[0].BlendEnable=TRUE;
	BlendStateDesc.RenderTarget[0].SrcBlend=D3D11_BLEND_SRC_ALPHA;
	BlendStateDesc.RenderTarget[0].DestBlend=D3D11_BLEND_INV_SRC_ALPHA;
	BlendStateDesc.RenderTarget[0].BlendOp=D3D11_BLEND_OP_ADD;
	BlendStateDesc.RenderTarget[0].SrcBlendAlpha=D3D11_BLEND_ZERO;
	BlendStateDesc.RenderTarget[0].DestBlendAlpha=D3D11_BLEND_ZERO;
	BlendStateDesc.RenderTarget[0].BlendOpAlpha=D3D11_BLEND_OP_ADD;
	BlendStateDesc.RenderTarget[0].RenderTargetWriteMask=0x0f;
	if(FAILED(m_Device->CreateBlendState(&BlendStateDesc,&m_AlphaFullBlending)))
		return false;
	BlendStateDesc.RenderTarget[0].BlendEnable=FALSE;
	if(FAILED(m_Device->CreateBlendState(&BlendStateDesc,&m_AlphaDisableBlendingState)))
		return false;

	if(!CreateViewport())
		return false;

	return true;
}

bool CD3D::CreateDepthBuffer(void)
{
	D3D11_TEXTURE2D_DESC DepthBufferDesc;
	// Jetzt kommt der DepthBuffer und der Depthstencil-Buffer
	ZeroMemory(&DepthBufferDesc,sizeof(DepthBufferDesc));
	DepthBufferDesc.Width=g_Options.GetScreenWidth();
	DepthBufferDesc.Height=g_Options.GetScreenHeight();
	DepthBufferDesc.MipLevels=1;
	DepthBufferDesc.ArraySize=1;
	DepthBufferDesc.Format=DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthBufferDesc.SampleDesc.Count=1;
	DepthBufferDesc.SampleDesc.Quality=0;
	DepthBufferDesc.Usage=D3D11_USAGE_DEFAULT;
	DepthBufferDesc.BindFlags=D3D11_BIND_DEPTH_STENCIL;
	DepthBufferDesc.CPUAccessFlags=0;
	DepthBufferDesc.MiscFlags=0;
	if(FAILED(m_Device->CreateTexture2D(&DepthBufferDesc,NULL,&m_DepthStencilBuffer)))
	{
		MessageBox(m_Hwnd,L"Fehler beim erstellen des DepthBuffer.",L"Error",MB_OK | MB_ICONERROR);
		return false;
	}
	return true;
}

bool CD3D::CreateDepthStencilBuffer(void)
{
	D3D11_DEPTH_STENCIL_DESC DepthStencilDesc;
	// Als nächstes den DepthStencil State initialisieren
	ZeroMemory(&DepthStencilDesc,sizeof(DepthStencilDesc));
	DepthStencilDesc.DepthEnable=true;
	DepthStencilDesc.DepthWriteMask=D3D11_DEPTH_WRITE_MASK_ALL;
	DepthStencilDesc.DepthFunc=D3D11_COMPARISON_LESS;
	DepthStencilDesc.StencilEnable=true;
	DepthStencilDesc.StencilReadMask=0xff;
	DepthStencilDesc.StencilWriteMask=0xff;

	DepthStencilDesc.FrontFace.StencilFailOp=D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilDepthFailOp=D3D11_STENCIL_OP_INCR;
	DepthStencilDesc.FrontFace.StencilPassOp=D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilFunc=D3D11_COMPARISON_ALWAYS;
	DepthStencilDesc.BackFace.StencilFailOp=D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilDepthFailOp=D3D11_STENCIL_OP_DECR;
	DepthStencilDesc.BackFace.StencilPassOp=D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilFunc=D3D11_COMPARISON_ALWAYS;
	if(FAILED(m_Device->CreateDepthStencilState(&DepthStencilDesc,&m_DepthStencilState)))
	{
		MessageBox(m_Hwnd,L"Fehler beim erstellen des DepthStencilState.",L"Error",MB_OK | MB_ICONERROR);
		return false;
	}
	// Und benutzen
	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState,1);
	return true;
}

bool CD3D::CreateDisabledDepthStencilBuffer(void)
{
	D3D11_DEPTH_STENCIL_DESC DepthDisabledStencilDesc;
	// DepthStencilState ohne Depth erstellen
	ZeroMemory(&DepthDisabledStencilDesc,sizeof(DepthDisabledStencilDesc));
	DepthDisabledStencilDesc.DepthEnable=false;
	DepthDisabledStencilDesc.DepthWriteMask=D3D11_DEPTH_WRITE_MASK_ALL;
	DepthDisabledStencilDesc.DepthFunc=D3D11_COMPARISON_LESS;
	DepthDisabledStencilDesc.StencilEnable=true;
	DepthDisabledStencilDesc.StencilReadMask=0xff;
	DepthDisabledStencilDesc.StencilWriteMask=0xff;
	DepthDisabledStencilDesc.FrontFace.StencilFailOp=D3D11_STENCIL_OP_KEEP;
	DepthDisabledStencilDesc.FrontFace.StencilDepthFailOp=D3D11_STENCIL_OP_INCR;
	DepthDisabledStencilDesc.FrontFace.StencilPassOp=D3D11_STENCIL_OP_KEEP;
	DepthDisabledStencilDesc.FrontFace.StencilFunc=D3D11_COMPARISON_ALWAYS;
	DepthDisabledStencilDesc.BackFace.StencilFailOp=D3D11_STENCIL_OP_KEEP;
	DepthDisabledStencilDesc.BackFace.StencilDepthFailOp=D3D11_STENCIL_OP_DECR;
	DepthDisabledStencilDesc.BackFace.StencilPassOp=D3D11_STENCIL_OP_KEEP;
	DepthDisabledStencilDesc.BackFace.StencilFunc=D3D11_COMPARISON_ALWAYS;
	if(FAILED(m_Device->CreateDepthStencilState(&DepthDisabledStencilDesc,&m_DepthDisabledStencilState)))
		return false;
	return true;
}

bool CD3D::CreateDepthStencilView(void)
{
	D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc;
	// Jetzt den DepthStencilView erstellen
	ZeroMemory(&DepthStencilViewDesc,sizeof(DepthStencilViewDesc));
	DepthStencilViewDesc.Format=DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthStencilViewDesc.ViewDimension=D3D11_DSV_DIMENSION_TEXTURE2D;
	DepthStencilViewDesc.Texture2D.MipSlice=0;
	if(FAILED(m_Device->CreateDepthStencilView(m_DepthStencilBuffer,&DepthStencilViewDesc,&m_DepthStencilView)))
	{
		MessageBox(m_Hwnd,L"Fehler beim erstellen des DepthStencilView.",L"Error",MB_OK | MB_ICONERROR);
		return false;
	}
	m_DeviceContext->OMSetRenderTargets(1,&m_RenderTargetView,m_DepthStencilView);
	return true;
}

bool CD3D::CreateRasterizerState(void)
{
	D3D11_RASTERIZER_DESC RasterDesc;
	// Einen StandardRasterizer State erstellen
	RasterDesc.AntialiasedLineEnable=true;
	RasterDesc.CullMode=D3D11_CULL_BACK;
	RasterDesc.DepthBias=0;
	RasterDesc.DepthBiasClamp=0.0f;
	RasterDesc.DepthClipEnable=true;
	RasterDesc.FillMode=D3D11_FILL_SOLID;
	RasterDesc.FrontCounterClockwise=false;
	RasterDesc.MultisampleEnable=false;
	RasterDesc.ScissorEnable=false;
	RasterDesc.SlopeScaledDepthBias=0.0f;
	if(FAILED(m_Device->CreateRasterizerState(&RasterDesc,&m_RasterState)))
	{
		MessageBox(m_Hwnd,L"Fehler beim erstellen des RasterizerState.",L"Error",MB_OK | MB_ICONERROR);
		return false;
	}
	m_DeviceContext->RSSetState(m_RasterState);
	return true;
}

bool CD3D::CreateViewport(void)
{
	D3D11_VIEWPORT Viewport;
	float FOV;
	float ScreenAspect;
	// Viewport initialisieren
	Viewport.Width=(float)g_Options.GetScreenWidth();
	Viewport.Height=(float)g_Options.GetScreenHeight();
	Viewport.MinDepth=0.0f;
	Viewport.MaxDepth=1.0f;
	Viewport.TopLeftX=0.0f;
	Viewport.TopLeftY=0.0f;
	m_DeviceContext->RSSetViewports(1,&Viewport);
	// ProjektionsMatrix erstellen
	FOV=(float)XM_PI/4.0f;
	ScreenAspect=(float)g_Options.GetScreenWidth()/(float)g_Options.GetScreenHeight();
	XMMatrixPerspectiveFovLH(&m_ProjectionMatrix,FOV,ScreenAspect,g_Options.GetScreenNear(),g_Options.GetScreenDepth());
	// WorldMatrix
	XMMatrixIdentity(&m_WorldMatrix);
	// OrthoMatrix für 2D-Rendering
	XMMatrixOrthographicLH(&m_OrthoMatrix,(float)g_Options.GetScreenWidth(),(float)g_Options.GetScreenHeight(),g_Options.GetScreenNear(),g_Options.GetScreenDepth());

	return true;
}

void CD3D::ResetViewPort(void)
{
	D3D11_VIEWPORT Viewport;
	Viewport.Width=(float)g_Options.GetScreenWidth();
	Viewport.Height=(float)g_Options.GetScreenHeight();
	Viewport.MinDepth=0.0f;
	Viewport.MaxDepth=1.0f;
	Viewport.TopLeftX=0.0f;
	Viewport.TopLeftY=0.0f;
	m_DeviceContext->RSSetViewports(1,&Viewport);

}

void CD3D::Shutdown(void)
{
	// Erstmal in Windowmode gehen, damit keine Exceptions auftreten
	if(m_SwapChain)
		m_SwapChain->SetFullscreenState(false,NULL);
	if(m_DisplayModeList)
	{
		delete[] m_DisplayModeList;
		m_DisplayModeList=NULL;
	}
	if(m_RasterState)
	{
		m_RasterState->Release();
		m_RasterState=NULL;
	}
	if(m_AlphaDisableBlendingState)
	{
		m_AlphaDisableBlendingState->Release();
		m_AlphaDisableBlendingState=NULL;
	}
	if(m_AlphaEnableBlendingState)
	{
		m_AlphaEnableBlendingState->Release();
		m_AlphaEnableBlendingState=NULL;
	}
	if(m_DepthDisabledStencilState)
	{
		m_DepthDisabledStencilState->Release();
		m_DepthDisabledStencilState=NULL;
	}
	if(m_DepthStencilView)
	{
		m_DepthStencilView->Release();
		m_DepthStencilView=NULL;
	}
	if(m_DepthStencilState)
	{
		m_DepthStencilState->Release();
		m_DepthStencilState=NULL;
	}
	if(m_DepthStencilBuffer)
	{
		m_DepthStencilBuffer->Release();
		m_DepthStencilBuffer=NULL;
	}
	if(m_RenderTargetView)
	{
		m_RenderTargetView->Release();
		m_RenderTargetView=NULL;
	}
	if(m_DeviceContext)
	{
		m_DeviceContext->Release();
		m_DeviceContext=NULL;
	}
	if(m_Device)
	{
		m_Device->Release();
		m_Device=NULL;
	}
	if(m_SwapChain)
	{
		m_SwapChain->Release();
		m_SwapChain=NULL;
	}
}

void CD3D::BeginScene(float red,float green,float blue,float alpha)
{
	float color[4];
	color[0]=red;
	color[1]=green;
	color[2]=blue;
	color[3]=alpha;
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView,color);
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView,D3D11_CLEAR_DEPTH,1.0f,0);
}

void CD3D::EndScene(void)
{
	if(m_VSync)
		m_SwapChain->Present(1,0);
	else
		m_SwapChain->Present(0,0);
}

void CD3D::GetVideoCardInfo(char* cardname,int& memory)
{
	strcpy_s(cardname,128,m_VidCardDesc);
	memory=m_VidMem;
}

void CD3D::TurnZBufferOn(void)
{
	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState,1);
}

void CD3D::TurnZBufferOff(void)
{
	m_DeviceContext->OMSetDepthStencilState(m_DepthDisabledStencilState,1);
}

void CD3D::TurnAlphaBlendingOn(void)
{
	float blendFactor[4];
	blendFactor[0]=0.0f;
	blendFactor[1]=0.0f;
	blendFactor[2]=0.0f;
	blendFactor[3]=0.0f;
	m_DeviceContext->OMSetBlendState(m_AlphaEnableBlendingState,blendFactor,0xffffffff);
}

void CD3D::TurnAlphaBlendingOff(void)
{
	float blendFactor[4];
	blendFactor[0]=0.0f;
	blendFactor[1]=0.0f;
	blendFactor[2]=0.0f;
	blendFactor[3]=0.0f;
	m_DeviceContext->OMSetBlendState(m_AlphaDisableBlendingState,blendFactor,0xffffffff);
}

void CD3D::TurnFullAlphaBlendingOn(void)
{
	float blendFactor[4];
	blendFactor[0]=0.0f;
	blendFactor[1]=0.0f;
	blendFactor[2]=0.0f;
	blendFactor[3]=0.0f;
	m_DeviceContext->OMSetBlendState(m_AlphaFullBlending,blendFactor,0xffffffff);
}

ID3D11DepthStencilView* CD3D::GetDepthStencilView(void)
{
	return m_DepthStencilView;
}

void CD3D::SetBackBufferRenderTarget(void)
{
	m_DeviceContext->OMSetRenderTargets(1,&m_RenderTargetView,m_DepthStencilView);
}

void CD3D::SetFullscreenOn(void)
{
	m_SwapChain->SetFullscreenState(true,NULL); // Fullscreen Yea
}

void CD3D::SetFullscreenOff(void)
{
	m_SwapChain->SetFullscreenState(false,NULL); // Fullscreen brauch keiner
}

bool CD3D::ChangeRes(int screenwidth,int screenheight)
{
	HRESULT hr;
	ID3D11Texture2D *BackBufferPtr;

	m_RenderTargetView->Release();

	hr=m_SwapChain->ResizeBuffers(0,screenwidth,screenheight,DXGI_FORMAT_R8G8B8A8_UNORM,DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
	if(FAILED(hr))
		return false;

	// Jetzt den BackBuffer neu erstellen
	if(FAILED(m_SwapChain->GetBuffer(0,__uuidof(ID3D11Texture2D),(LPVOID*)&BackBufferPtr)))
	{
		MessageBox(m_Hwnd,L"Fehler beim erstellen des Backbuffers.",L"Error",MB_OK | MB_ICONERROR);
		return false;
	}
	if(FAILED(m_Device->CreateRenderTargetView(BackBufferPtr,NULL,&m_RenderTargetView)))
	{
		MessageBox(m_Hwnd,L"Fehler beim erstellen des RTV.",L"Error",MB_OK | MB_ICONERROR);
		return false;
	}
	BackBufferPtr->Release();
	BackBufferPtr=NULL;

	DXGI_MODE_DESC modedesc;
	modedesc.Format=DXGI_FORMAT_R8G8B8A8_UNORM;
	modedesc.Height=screenheight;
	modedesc.Width=screenwidth;
	modedesc.Scaling=DXGI_MODE_SCALING_UNSPECIFIED;
	modedesc.ScanlineOrdering=DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	modedesc.RefreshRate.Denominator=m_Denominator;
	modedesc.RefreshRate.Numerator=m_Numerator;
	if(FAILED(m_SwapChain->ResizeTarget(&modedesc)))
		return false;

	m_DepthStencilBuffer->Release();
	m_DepthStencilBuffer=NULL;
	if(!CreateDepthBuffer())
		return false;

	m_DepthStencilState->Release();
	m_DepthStencilState=NULL;
	if(!CreateDepthStencilBuffer())
		return false;
	m_DepthDisabledStencilState->Release();
	m_DepthDisabledStencilState=NULL;
	if(!CreateDisabledDepthStencilBuffer())
		return false;
	
	m_RasterState->Release();
	m_RasterState=NULL;
	if(!CreateRasterizerState())
		return false;
	if(!CreateViewport())
		return false;
	SetBackBufferRenderTarget();
	
	if(!g_pAppHandle->GetGrafikHandle()->Resize(screenwidth,screenheight))
		return false;
	
	return true;
}
