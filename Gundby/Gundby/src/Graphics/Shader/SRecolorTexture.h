// ------------------------------------------------------------------
// Lukas Beinlich, 27.07.2014
// SRecolorShader.h
// Die Klasse für einen Shader der eine Textur mit einer Farbe multipliziert (recolortexture.vs + recolortexture.ps)
// ------------------------------------------------------------------

#ifndef __RECOLORTEXTURE_SHADER_H__
#define __RECOLORTEXTURE_SHADER_H__


#include <d3d11.h>

#include "src\Graphics\Shader\SBasic.h"

class CSRecolorTexture:public CSBasic
{
private:
	struct PixelBufferType
	{
		XMFLOAT4 pixelColor;
	};

	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX proj;
	};

	ID3D11VertexShader *m_VertexShader;
	ID3D11PixelShader *m_PixelShader;
	ID3D11InputLayout *m_Layout;
//	ID3D11Buffer *m_PositionArrayBuffer; // Sollte als Parameter übergeben werden
	ID3D11Buffer *m_MatrixBuffer;
	ID3D11Buffer *m_PixelBuffer;
	ID3D11SamplerState *m_SampleState;

	bool InitShader(ID3D11Device *device,HWND hWnd,WCHAR* vsFilename,WCHAR* psFilename);
	void ShutdownShader(void);

	bool SetShaderParameters(ID3D11DeviceContext* devicecontext,XMMATRIX worldmat,XMMATRIX viewmat,XMMATRIX projmat,ID3D11ShaderResourceView **texture,XMFLOAT4 pixelColor,ID3D11Buffer *positionBuffer);
	void RenderShader(ID3D11DeviceContext* devicecontext,int indexCount,unsigned int instancecnt);
public:
	CSRecolorTexture(void);
	~CSRecolorTexture(void);
	bool Init(ID3D11Device *device,HWND hWnd);
	void Shutdown(void);
	bool Render(ID3D11DeviceContext *devicecontext,int indexCount,XMMATRIX worldmat,XMMATRIX viewmat,XMMATRIX projmat,ID3D11ShaderResourceView **texture,XMFLOAT4 pixelColor,ID3D11Buffer *positionBuffer,unsigned int instancecnt);
};

#endif //__RECOLORTEXTURE_SHADER_H__
