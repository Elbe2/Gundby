// ------------------------------------------------------------------
// Lukas Beinlich, 04.08.2014
// RenderTexture.cpp
// Mithilfe dieser Klasse kann man eine Szene auf eine Textur rendern, anstatt in den Backbuffer
// ------------------------------------------------------------------

#include "src\Graphics\RenderTexture.h"

#include <DirectXMath.h>
using namespace DirectX;

CRenderTexture::CRenderTexture(void)
{
	m_RenderTargetTexture=NULL;
	m_RenderTargetView=NULL;
	m_ShaderResourceView=NULL;
}

CRenderTexture::~CRenderTexture(void)
{
}

bool CRenderTexture::Init(ID3D11Device *device,int texturewidth,int textureheight,float screennear,float screendepth)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	m_TextureWidth=texturewidth;
	m_TextureHeight=textureheight;

	ZeroMemory(&textureDesc,sizeof(textureDesc));

	textureDesc.Width=texturewidth;
	textureDesc.Height=textureheight;
	textureDesc.MipLevels=1;
	textureDesc.ArraySize=1;
	textureDesc.Format=DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count=1;
	textureDesc.Usage=D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags=D3D11_BIND_RENDER_TARGET|D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags=0;
	textureDesc.MiscFlags=0;

	if(FAILED(device->CreateTexture2D(&textureDesc,NULL,&m_RenderTargetTexture)))
		return false;

	renderTargetViewDesc.Format=textureDesc.Format;
	renderTargetViewDesc.ViewDimension=D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice=0;

	if(FAILED(device->CreateRenderTargetView(m_RenderTargetTexture,&renderTargetViewDesc,&m_RenderTargetView)))
		return false;

	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	if (FAILED(device->CreateShaderResourceView(m_RenderTargetTexture, &shaderResourceViewDesc, &m_ShaderResourceView)))
		return false;
	m_OrthoMatrix = XMMatrixOrthographicLH(texturewidth, textureheight, screennear, screendepth);
	return true;
}

void CRenderTexture::Shutdown(void)
{
	if(m_ShaderResourceView)
	{
		m_ShaderResourceView->Release();
		m_ShaderResourceView=NULL;
	}
	if(m_RenderTargetView)
	{
		m_RenderTargetView->Release();
		m_RenderTargetView=NULL;
	}
	if(m_RenderTargetTexture)
	{
		m_RenderTargetTexture->Release();
		m_RenderTargetTexture=NULL;
	}
}

void CRenderTexture::SetRenderTarget(ID3D11DeviceContext *devicecontext,ID3D11DepthStencilView *depthstencilview)
{
	devicecontext->OMSetRenderTargets(1,&m_RenderTargetView,depthstencilview);
}

void CRenderTexture::ClearRenderTarget(ID3D11DeviceContext *devicecontext,ID3D11DepthStencilView *depthstencilview,float r,float g,float b,float a)
{
	float color[4];
	
	color[0]=r;
	color[1]=g;
	color[2]=b;
	color[3]=a;
	devicecontext->ClearRenderTargetView(m_RenderTargetView,color);
	devicecontext->ClearDepthStencilView(depthstencilview,D3D11_CLEAR_DEPTH,1.0f,0);
}

ID3D11ShaderResourceView** CRenderTexture::GetShaderResourceView(void)
{
	return &m_ShaderResourceView;
}
