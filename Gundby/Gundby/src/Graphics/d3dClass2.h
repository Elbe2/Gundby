// ------------------------------------------------------------------
// Lukas Beinlich, 27.07.2014
// d3dClass.h
// Verwaltet den ganzen DirectX Kram
// ------------------------------------------------------------------
#pragma once

#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dx10.lib")

#include <dxgi.h>
#include <D3Dcommon.h>
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

class CD3D
{
private:
	bool m_VSync;
	int m_VidMem;
	char m_VidCardDesc[128];
	IDXGISwapChain* m_SwapChain;
	ID3D11Device *m_Device;
	ID3D11DeviceContext *m_DeviceContext;
	ID3D11RenderTargetView *m_RenderTargetView;
	ID3D11Texture2D *m_DepthStencilBuffer;
	ID3D11DepthStencilState *m_DepthStencilState;
	ID3D11DepthStencilState *m_DepthDisabledStencilState;
	ID3D11DepthStencilView *m_DepthStencilView;
	ID3D11BlendState *m_AlphaEnableBlendingState;
	ID3D11BlendState *m_AlphaDisableBlendingState;
	ID3D11BlendState *m_AlphaFullBlending;
	ID3D11RasterizerState *m_RasterState;
	XMMATRIX m_ProjectionMatrix;
	XMMATRIX m_WorldMatrix;
	XMMATRIX m_OrthoMatrix;
	HWND m_Hwnd;
	unsigned int m_Denominator;
	unsigned int m_Numerator;
	DXGI_MODE_DESC *m_DisplayModeList;

	bool CreateDepthBuffer(void);
	bool CreateDepthStencilBuffer(void);
	bool CreateDisabledDepthStencilBuffer(void);
	bool CreateDepthStencilView(void);
	bool CreateRasterizerState(void);
	bool CreateViewport(void);


public:
	CD3D(void);
	~CD3D(void);

	bool Init(int screenWidth,int screenHeight,bool vsync,HWND hwnd,bool fullscreen,float screenDepth,float screenNear);
	void Shutdown(void);

	void BeginScene(float red,float green,float blue,float alpha);
	void EndScene(void);

	ID3D11Device* GetDevice(void){return m_Device;}
	ID3D11DeviceContext* GetDeviceContext(void){return m_DeviceContext;}

	void GetProjectionMatrix(XMMATRIX& promat){promat=m_ProjectionMatrix;}
	void GetWorldMatrix(XMMATRIX& worldmat){worldmat=m_WorldMatrix;}
	void GetOrthoMatrix(XMMATRIX& orthomat){orthomat=m_OrthoMatrix;}

	void GetVideoCardInfo(char* cardname,int& memory);

	void TurnZBufferOn(void);
	void TurnZBufferOff(void);

	void TurnAlphaBlendingOn(void);
	void TurnAlphaBlendingOff(void);
	void TurnFullAlphaBlendingOn(void);

	ID3D11DepthStencilView* GetDepthStencilView(void);
	
	void SetBackBufferRenderTarget(void);
	void ResetViewPort(void);

	void SetFullscreenOn(void);
	void SetFullscreenOff(void);

	bool ChangeRes(int screenwidth,int screenheight);
};

