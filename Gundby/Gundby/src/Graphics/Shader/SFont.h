// ------------------------------------------------------------------
// Lukas Beinlich, 27.07.2014
// SFont.h
// Die Klasse für einen Shader für Schrift (font.vs + font.ps)
// ------------------------------------------------------------------

#ifndef __FONT_SHADER_H__
#define __FONT_SHADER_H__


#include <d3d11.h>

#include "src\Graphics\Shader\SBasic.h"

class CSFont:public CSBasic
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct PixelBufferType
	{
		XMFLOAT4 pixelColor;
	};

	ID3D11VertexShader *m_VertexShader;
	ID3D11PixelShader *m_PixelShader;
	ID3D11InputLayout *m_Layout;
	ID3D11Buffer *m_MatrixBuffer;
	ID3D11Buffer *m_PixelBuffer;
	ID3D11SamplerState *m_SampleState;

	bool InitShader(ID3D11Device *device,HWND hWnd,WCHAR* vsFilename,WCHAR* psFilename);
	void ShutdownShader(void);

	bool SetShaderParameters(ID3D11DeviceContext* devicecontext,XMMATRIX worldmat,XMMATRIX viewmat,XMMATRIX projmat,ID3D11ShaderResourceView **texture,XMFLOAT4 pixelColor);
	void RenderShader(ID3D11DeviceContext* devicecontext,int indexCount);
public:
	CSFont(void);
	~CSFont(void);
	bool Init(ID3D11Device *device,HWND hWnd);
	void Shutdown(void);
	bool Render(ID3D11DeviceContext *devicecontext,int indexCount,XMMATRIX worldmat,XMMATRIX viewmat,XMMATRIX projmat,ID3D11ShaderResourceView **texture,XMFLOAT4 pixelColor);
};

#endif //__FONT_SHADER_H__
