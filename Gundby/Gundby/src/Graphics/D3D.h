// ------------------------------------------------------------------
// D3D.h
// Lukas Beinlich, 04.10.2015
// Praktisch eine Schnittstelle zu DirectX11
// ------------------------------------------------------------------
#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

#include "src\Settings.h"

class D3D
{
private:
	HWND m_hWnd;
	Settings *m_pSettings;
	int m_VideoCardMemory;
	char m_VideoCardDescription[128];
	IDXGISwapChain *m_SwapChain;
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

	bool CreateDepthBuffer(void);
	bool CreateDepthStencilBuffer(void);
	bool CreateDisabledDepthStencilBuffer(void);
	bool CreateDepthStencilView(void);
	bool CreateRasterizerState(void);
	bool CreateViewport(void);
public:
	D3D(void);
	~D3D(void);

	bool Initialize(HWND hWnd, Settings *pSettings);
	void Shutdown(void);

	void BeginScene(float r,float g,float b,float a);
	void EndScene(void);

	ID3D11Device* GetDevice(void);
	ID3D11DeviceContext* GetDeviceContext(void);
	void GetProjectionMatrix(XMMATRIX& matrix);
	void GetWorldMatrix(XMMATRIX& matrix);
	void GetOrthoMatrix(XMMATRIX& matrix);
	void GetVideoCardInfo(char* CardName,int &memory);

	void TurnZBufferOn(void);
	void TurnZBufferOff(void);

	void TurnAlphaBlendingOn(void);
	void TurnAlphaBlendingOff(void);
	void TurnFullAlphaBlendingOn(void);

	ID3D11DepthStencilView* GetDepthStencilView(void);

	void SetBackBufferRenderTarget(void);
	void ResetViewPort(void);
};

