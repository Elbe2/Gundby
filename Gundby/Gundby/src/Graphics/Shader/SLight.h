// ------------------------------------------------------------------
// Lukas Beinlich, 25.07.2014
// SLight.h
// Die Klasse für einen einfachen Shader mit Licht(light.vs+light.ps)
// ------------------------------------------------------------------
#pragma once

#include "src\Graphics\Shader\SBasic.h"

class CSLightD:public CSBasic
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};
	struct LightBufferType
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float padding;
	};

	ID3D11VertexShader *m_VertexShader;
	ID3D11PixelShader *m_PixelShader;
	ID3D11InputLayout *m_Layout;
	ID3D11Buffer *m_MatrixBuffer;
	ID3D11Buffer *m_LightBuffer;
	ID3D11SamplerState *m_SampleState;

	bool InitShader(ID3D11Device *device,HWND hWnd,WCHAR* vsFilename,WCHAR* psFilename);
	void ShutdownShader(void);

	bool SetShaderParameters(ID3D11DeviceContext* devicecontext,XMMATRIX worldmat,XMMATRIX viewmat,XMMATRIX projmat,ID3D11ShaderResourceView *texture,XMFLOAT3 lightDir,XMFLOAT4 diffuseCol,XMFLOAT4 ambientCol);
	void RenderShader(ID3D11DeviceContext* devicecontext,int indexCount);
public:
	CSLightD(void);
	~CSLightD(void);
	bool Init(ID3D11Device *device,HWND hWnd);
	void Shutdown(void);
	bool Render(ID3D11DeviceContext *devicecontext,int indexCount,XMMATRIX worldmat,XMMATRIX viewmat,XMMATRIX projmat,ID3D11ShaderResourceView *texture,XMFLOAT3 lightDir,XMFLOAT4 diffuseCol,XMFLOAT4 ambientCol);
};

