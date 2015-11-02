// ------------------------------------------------------------------
// Lukas Beinlich, 04.08.2014
// SHorizontalBlur.h
// Die Klasse für einen Shader mit horizontalem Blur(horizontalblur.vs+horizontalblur.ps)
// ------------------------------------------------------------------
#pragma once

#include "src\Graphics\Shader\SBasic.h"

class CSHorizontalBlur:public CSBasic
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct ScreenSizeBufferType
	{
		float screenWidth;
		float padding[3];
	};

	ID3D11VertexShader *m_VertexShader;
	ID3D11PixelShader *m_PixelShader;
	ID3D11InputLayout *m_Layout;
	ID3D11Buffer *m_MatrixBuffer;
	ID3D11Buffer *m_ScreenSizeBuffer;
	ID3D11SamplerState *m_SampleState;

	bool InitShader(ID3D11Device *device,HWND hWnd,WCHAR* vsFilename,WCHAR* psFilename);
	void ShutdownShader(void);

	bool SetShaderParameters(ID3D11DeviceContext* devicecontext,XMMATRIX worldmat,XMMATRIX viewmat,XMMATRIX projmat,ID3D11ShaderResourceView **texture,float screenwidth);
	void RenderShader(ID3D11DeviceContext* devicecontext,int indexCount);
public:
	CSHorizontalBlur(void);
	~CSHorizontalBlur(void);
	bool Init(ID3D11Device *device,HWND hWnd);
	void Shutdown(void);
	bool Render(ID3D11DeviceContext *devicecontext,int indexCount,XMMATRIX worldmat,XMMATRIX viewmat,XMMATRIX projmat,ID3D11ShaderResourceView **texture,float screenwidth);
};
