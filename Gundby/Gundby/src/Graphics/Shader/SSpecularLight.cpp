// ------------------------------------------------------------------
// Lukas Beinlich, 25.07.2014
// SLight.cpp
// Die Klasse für einen einfachen Shader mit Licht(light.vs+light.ps)
// ------------------------------------------------------------------

#include "src\Graphics\Shader\SSpecularLight.h"

CSSpecularLight::CSSpecularLight(void)
{
	m_VertexShader=NULL;
	m_PixelShader=NULL;
	m_Layout=NULL;
	m_MatrixBuffer=NULL;
	m_SampleState=NULL;
	m_LightBuffer=NULL;
	m_CameraBuffer=NULL;
}

CSSpecularLight::~CSSpecularLight(void)
{
}

bool CSSpecularLight::Init(ID3D11Device *device,HWND hWnd)
{
	if(!InitShader(device,hWnd,L"Shaders/specularlight/specularlight.vs",L"Shaders/specularlight/specularlight.ps"))
		return false;
	return true;
}

void CSSpecularLight::Shutdown(void)
{
	ShutdownShader();
}

bool CSSpecularLight::Render(ID3D11DeviceContext *devicecontext,int indexCount,XMMATRIX worldmat,XMMATRIX viewmat,XMMATRIX projmat,ID3D11ShaderResourceView **texture,XMFLOAT3 lightDir,XMFLOAT4 diffuseCol,XMFLOAT4 ambientCol,XMFLOAT3 cameraPosition,XMFLOAT4 specularColor,float specularPower)
{
	if(!SetShaderParameters(devicecontext,worldmat,viewmat,projmat,texture,lightDir,diffuseCol,ambientCol,cameraPosition,specularColor,specularPower))
		return false;
	RenderShader(devicecontext,indexCount);
	return true;
}

bool CSSpecularLight::InitShader(ID3D11Device *device,HWND hWnd,WCHAR *vsFilename,WCHAR *psFilename)
{
	ID3D10Blob *errorMessage;
	ID3D10Blob *vertexShaderBuffer;
	ID3D10Blob *pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc;
	D3D11_BUFFER_DESC cameraBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	
	errorMessage=NULL;
	vertexShaderBuffer=NULL;
	pixelShaderBuffer=NULL;

	if(FAILED(D3DCompileFromFile(vsFilename,NULL,NULL,"LightVertexShader","vs_5_0",D3D10_SHADER_ENABLE_STRICTNESS,0,&vertexShaderBuffer,&errorMessage)))
	{
		if(errorMessage)
			OutputShaderErrorMessage(errorMessage,hWnd,vsFilename);
		else
			MessageBox(hWnd,vsFilename,L"Missing Shader File",MB_OK);
		return false;
	}
	if(FAILED(D3DCompileFromFile(psFilename,NULL,NULL,"LightPixelShader","ps_5_0",D3D10_SHADER_ENABLE_STRICTNESS,0,&pixelShaderBuffer,&errorMessage)))
	{
		if(errorMessage)
			OutputShaderErrorMessage(errorMessage,hWnd,psFilename);
		else
			MessageBox(hWnd,psFilename,L"Missing Shader File",MB_OK);
		return false;
	}
	if(FAILED(device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),vertexShaderBuffer->GetBufferSize(),NULL,&m_VertexShader)))
		return false;
	if(FAILED(device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),pixelShaderBuffer->GetBufferSize(),NULL,&m_PixelShader)))
		return false;

	polygonLayout[0].SemanticName="POSITION";
	polygonLayout[0].SemanticIndex=0;
	polygonLayout[0].Format=DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot=0;
	polygonLayout[0].AlignedByteOffset=0;
	polygonLayout[0].InputSlotClass=D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate=0;

	polygonLayout[1].SemanticName="TEXCOORD";
	polygonLayout[1].SemanticIndex=0;
	polygonLayout[1].Format=DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot=0;
	polygonLayout[1].AlignedByteOffset=D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass=D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate=0;

	polygonLayout[2].SemanticName="NORMAL";
	polygonLayout[2].SemanticIndex=0;
	polygonLayout[2].Format=DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot=0;
	polygonLayout[2].AlignedByteOffset=D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass=D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate=0;

	numElements=sizeof(polygonLayout)/sizeof(polygonLayout[0]);
	HRESULT hr=device->CreateInputLayout(polygonLayout,numElements,vertexShaderBuffer->GetBufferPointer(),vertexShaderBuffer->GetBufferSize(),&m_Layout);
	if(FAILED(hr))
		return false;
	
	vertexShaderBuffer->Release();
	vertexShaderBuffer=NULL;
	pixelShaderBuffer->Release();
	pixelShaderBuffer=NULL;

	matrixBufferDesc.Usage=D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth=sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags=D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags=D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags=0;
	matrixBufferDesc.StructureByteStride=0;
	if(FAILED(device->CreateBuffer(&matrixBufferDesc,NULL,&m_MatrixBuffer)))
		return false;

	lightBufferDesc.Usage=D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth=sizeof(LightBufferType);
	lightBufferDesc.BindFlags=D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags=D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags=0;
	lightBufferDesc.StructureByteStride=0;

	if(FAILED(device->CreateBuffer(&lightBufferDesc,NULL,&m_LightBuffer)))
		return false;

	cameraBufferDesc.Usage=D3D11_USAGE_DYNAMIC;
	cameraBufferDesc.ByteWidth=sizeof(CameraBufferType);
	cameraBufferDesc.BindFlags=D3D11_BIND_CONSTANT_BUFFER;
	cameraBufferDesc.CPUAccessFlags=D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.MiscFlags=0;
	cameraBufferDesc.StructureByteStride=0;
	if(FAILED(device->CreateBuffer(&cameraBufferDesc,NULL,&m_CameraBuffer)))
		return false;

	samplerDesc.Filter=D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU=D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV=D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW=D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias=0.0f;
	samplerDesc.MaxAnisotropy=1;
	samplerDesc.ComparisonFunc=D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0]=0;
	samplerDesc.BorderColor[1]=0;
	samplerDesc.BorderColor[2]=0;
	samplerDesc.BorderColor[3]=0;
	samplerDesc.MinLOD=0;
	samplerDesc.MaxLOD=D3D11_FLOAT32_MAX;
	if(FAILED(device->CreateSamplerState(&samplerDesc,&m_SampleState)))
		return false;
	return true;
}

void CSSpecularLight::ShutdownShader(void)
{
	if(m_SampleState)
	{
		m_SampleState->Release();
		m_SampleState=NULL;
	}
	if(m_CameraBuffer)
	{
		m_CameraBuffer->Release();
		m_CameraBuffer=NULL;
	}
	if(m_LightBuffer)
	{
		m_LightBuffer->Release();
		m_LightBuffer=NULL;
	}
	if(m_MatrixBuffer)
	{
		m_MatrixBuffer->Release();
		m_MatrixBuffer=NULL;
	}
	if(m_Layout)
	{
		m_Layout->Release();
		m_Layout=NULL;
	}
	if(m_PixelShader)
	{
		m_PixelShader->Release();
		m_PixelShader=NULL;
	}
	if(m_VertexShader)
	{
		m_VertexShader->Release();
		m_VertexShader=NULL;
	}
}

bool CSSpecularLight::SetShaderParameters(ID3D11DeviceContext* devicecontext,XMMATRIX worldMatrix,XMMATRIX viewMatrix,XMMATRIX projectionMatrix, ID3D11ShaderResourceView **texture, XMFLOAT3 lightDirection,XMFLOAT4 ambientColor,XMFLOAT4 diffuseColor,XMFLOAT3 cameraPosition,XMFLOAT4 specularColor,float specularPower)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType *dataPtr;
	LightBufferType *dataPtr2;
	CameraBufferType *dataPtr3;
	unsigned int bufferNr;

	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	if(FAILED(devicecontext->Map(m_MatrixBuffer,0,D3D11_MAP_WRITE_DISCARD,0,&mappedResource)))
		return false;
	dataPtr=(MatrixBufferType*)mappedResource.pData;

	dataPtr->world=worldMatrix;
	dataPtr->view=viewMatrix;
	dataPtr->projection=projectionMatrix;

	devicecontext->Unmap(m_MatrixBuffer,0);

	bufferNr=0;
	devicecontext->VSSetConstantBuffers(bufferNr,1,&m_MatrixBuffer);
	devicecontext->PSSetShaderResources(0,1,texture);

	if(FAILED(devicecontext->Map(m_LightBuffer,0,D3D11_MAP_WRITE_DISCARD,0,&mappedResource)))
		return false;
	dataPtr2=(LightBufferType*)mappedResource.pData;

	dataPtr2->ambientColor=ambientColor;
	dataPtr2->diffuseColor=diffuseColor;
	dataPtr2->lightDirection=lightDirection;
	dataPtr2->specularColor=specularColor;
	dataPtr2->specularPower=specularPower;

	devicecontext->Unmap(m_LightBuffer,0);

	bufferNr=0;
	devicecontext->PSSetConstantBuffers(bufferNr,1,&m_LightBuffer);

	if(FAILED(devicecontext->Map(m_CameraBuffer,0,D3D11_MAP_WRITE_DISCARD,0,&mappedResource)))
		return false;
	dataPtr3=(CameraBufferType*)mappedResource.pData;
	
	dataPtr3->cameraPosition=cameraPosition;
	dataPtr3->padding=0.0f;

	devicecontext->Unmap(m_CameraBuffer,0);
	
	bufferNr=1;
	devicecontext->VSSetConstantBuffers(bufferNr,1,&m_CameraBuffer);

	return true;
}

void CSSpecularLight::RenderShader(ID3D11DeviceContext *devicecontext,int indexCount)
{
	devicecontext->IASetInputLayout(m_Layout);
	devicecontext->VSSetShader(m_VertexShader,NULL,0);
	devicecontext->PSSetShader(m_PixelShader,NULL,0);
	devicecontext->PSSetSamplers(0,1,&m_SampleState);
	devicecontext->DrawIndexed(indexCount,NULL,0);
}
