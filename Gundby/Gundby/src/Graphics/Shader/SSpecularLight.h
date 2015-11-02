// ------------------------------------------------------------------
// SSPecularLigtht.h
// Lukas Beinlich, 08.10.2015
// Die Klasse für einen Shader mit "Specular Light"(specularlight.vs+specuularlight.ps)
// ------------------------------------------------------------------
#pragma once

#include "src\Graphics\Shader\SBasic.h"

class CSSpecularLight:public CSBasic
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};
	struct CameraBufferType
	{
		XMFLOAT3 cameraPosition;
		float padding;
	};
	struct LightBufferType
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float specularPower;
		XMFLOAT4 specularColor;
	};

	ID3D11VertexShader *m_VertexShader;
	ID3D11PixelShader *m_PixelShader;
	ID3D11InputLayout *m_Layout;
	ID3D11Buffer *m_MatrixBuffer;
	ID3D11Buffer *m_LightBuffer;
	ID3D11Buffer *m_CameraBuffer;
	ID3D11SamplerState *m_SampleState;

	bool InitShader(ID3D11Device *device, HWND hWnd, WCHAR* vsFilename, WCHAR* psFilename);
	void ShutdownShader(void);

	bool SetShaderParameters(ID3D11DeviceContext* devicecontext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView **texture, XMFLOAT3 lightDirection, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT3 cameraPosition, XMFLOAT4 specularColor, float specularPower);
	void RenderShader(ID3D11DeviceContext* devicecontext, int indexCount);
public:
	CSSpecularLight(void);
	~CSSpecularLight(void);
	bool Init(ID3D11Device *device, HWND hWnd);
	void Shutdown(void);
	bool Render(ID3D11DeviceContext *devicecontext, int indexCount, XMMATRIX worldmat, XMMATRIX viewmat, XMMATRIX projmat, ID3D11ShaderResourceView **texture, XMFLOAT3 lightDir, XMFLOAT4 diffuseCol, XMFLOAT4 ambientCol, XMFLOAT3 cameraPosition, XMFLOAT4 specularColor, float specularPower);
};

