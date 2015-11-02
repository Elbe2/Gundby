// ------------------------------------------------------------------
// SColor.h
// Lukas Beinlich, 06.10.2015
// Ein einfacher Shader ohne Textur nur mit Farbe
// ------------------------------------------------------------------
#pragma once

#include "src\Graphics\Shader\SBasic.h"

class CSColor:public CSBasic
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
		XMMATRIX worldviewprojection;
	};

	ID3D11VertexShader *m_VertexShader;
	ID3D11PixelShader *m_PixelShader;
	ID3D11InputLayout *m_Layout;
	ID3D11Buffer *m_MatrixBuffer;

	bool InitShader(ID3D11Device *device,HWND hWnd,WCHAR* vsFilename,WCHAR* psFilename);
	void ShutdownShader(void);

	bool SetShaderParameters(ID3D11DeviceContext* devicecontext,XMMATRIX worldmat,XMMATRIX viewmat,XMMATRIX projmat);
	void RenderShader(ID3D11DeviceContext* devicecontext,int indexCount);
public:
	CSColor(void);
	~CSColor(void);
	bool Init(ID3D11Device *device,HWND hWnd);
	void Shutdown(void);
	bool Render(ID3D11DeviceContext *devicecontext,int indexCount,XMMATRIX worldmat,XMMATRIX viewmat,XMMATRIX projmat);
};

