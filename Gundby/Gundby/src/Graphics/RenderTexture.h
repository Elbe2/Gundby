// ------------------------------------------------------------------
// Lukas Beinlich, 04.08.2014
// RenderTexture.h
// Mithilfe dieser Klasse kann man eine Szene auf eine Textur rendern, anstatt in den Backbuffer
// ------------------------------------------------------------------

#ifndef __RENDERTEXTURE_H__
#define __RENDERTEXTURE_H__

#include <D3D11.h>
#include <DirectXMath.h>
using namespace DirectX;

class CRenderTexture
{
private:
	ID3D11Texture2D *m_RenderTargetTexture;
	ID3D11RenderTargetView *m_RenderTargetView;
	ID3D11ShaderResourceView *m_ShaderResourceView;
	XMMATRIX m_OrthoMatrix;
	int m_TextureWidth;
	int m_TextureHeight;
public:
	CRenderTexture(void);
	~CRenderTexture(void);

	bool Init(ID3D11Device *device,int texturewidth,int textureheight,float screennear,float screendepth);
	void Shutdown(void);

	void SetRenderTarget(ID3D11DeviceContext *devicecontext,ID3D11DepthStencilView *depthstencilview);
	void ClearRenderTarget(ID3D11DeviceContext *devicecontext,ID3D11DepthStencilView *depthstencilview,float r,float g,float b,float a);

	ID3D11ShaderResourceView** GetShaderResourceView(void);
	void GetOrthoMatrix(XMMATRIX &ortho){ortho=m_OrthoMatrix;}
	int GetTextureWidth(void){return m_TextureWidth;}
	int GetTextureHeight(void){return m_TextureHeight;}
};

#endif //__RENDERTEXTURE_H__
