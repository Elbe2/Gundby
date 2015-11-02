// ------------------------------------------------------------------
// Lukas Beinlich, 25.07.2014
// SLight.cpp
// Die Klasse für einen einfachen Shader mit Licht(light.vs+light.ps)
// ------------------------------------------------------------------

#include "src\Graphics\Shader\SLight.h"

CSLightD::CSLightD(void)
{
	m_VertexShader=NULL;
	m_PixelShader=NULL;
	m_Layout=NULL;
	m_MatrixBuffer=NULL;
	m_SampleState=NULL;
	m_LightBuffer=NULL;
}

CSLightD::~CSLightD(void)
{
}

bool CSLightD::Init(ID3D11Device *device,HWND hWnd)
{
	if(!InitShader(device,hWnd,L"Shaders/light/light.vs",L"Shaders/light/light.ps"))
		return false;
	return true;
}

void CSLightD::Shutdown(void)
{
	ShutdownShader();
}

bool CSLightD::Render(ID3D11DeviceContext *devicecontext,int indexCount,XMMATRIX worldmat,XMMATRIX viewmat,XMMATRIX projmat,ID3D11ShaderResourceView *texture,XMFLOAT3 lightDir,XMFLOAT4 diffuseCol,XMFLOAT4 ambientCol)
{
	if(!SetShaderParameters(devicecontext,worldmat,viewmat,projmat,texture,lightDir,diffuseCol,ambientCol))
		return false;
	RenderShader(devicecontext,indexCount);
	return true;
}

bool CSLightD::InitShader(ID3D11Device *device,HWND hWnd,WCHAR *vsFilename,WCHAR *psFilename)
{
	ID3D10Blob *errorMessage;
	ID3D10Blob *vertexShaderBuffer;
	ID3D10Blob *pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc;
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

void CSLightD::ShutdownShader(void)
{
	if(m_SampleState)
	{
		m_SampleState->Release();
		m_SampleState=NULL;
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


bool CSLightD::SetShaderParameters(ID3D11DeviceContext *devicecontext,XMMATRIX worldmat,XMMATRIX viewmat,XMMATRIX projmat,ID3D11ShaderResourceView *texture,XMFLOAT3 lightDir,XMFLOAT4 diffuseCol,XMFLOAT4 ambientCol)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType *dataPtr;
	LightBufferType *dataPtr2;
	unsigned int bufferNr;

	worldmat = XMMatrixTranspose(worldmat);
	viewmat = XMMatrixTranspose(viewmat);
	projmat = XMMatrixTranspose(projmat);

	if(FAILED(devicecontext->Map(m_MatrixBuffer,0,D3D11_MAP_WRITE_DISCARD,0,&mappedResource)))
		return false;
	dataPtr=(MatrixBufferType*)mappedResource.pData;

	dataPtr->world=worldmat;
	dataPtr->view=viewmat;
	dataPtr->projection=projmat;

	devicecontext->Unmap(m_MatrixBuffer,0);

	bufferNr=0;
	devicecontext->VSSetConstantBuffers(bufferNr,1,&m_MatrixBuffer);
	devicecontext->PSSetShaderResources(0,1,&texture);

	if(FAILED(devicecontext->Map(m_LightBuffer,0,D3D11_MAP_WRITE_DISCARD,0,&mappedResource)))
		return false;
	dataPtr2=(LightBufferType*)mappedResource.pData;

	dataPtr2->ambientColor=ambientCol;
	dataPtr2->diffuseColor=diffuseCol;
	dataPtr2->lightDirection=lightDir;
	dataPtr2->padding=0;

	devicecontext->Unmap(m_LightBuffer,0);

	bufferNr=0;
	devicecontext->PSSetConstantBuffers(bufferNr,1,&m_LightBuffer);

	return true;
}

void CSLightD::RenderShader(ID3D11DeviceContext *devicecontext,int indexCount)
{
	devicecontext->IASetInputLayout(m_Layout);
	devicecontext->VSSetShader(m_VertexShader,NULL,0);
	devicecontext->PSSetShader(m_PixelShader,NULL,0);
	devicecontext->PSSetSamplers(0,1,&m_SampleState);
	devicecontext->DrawIndexed(indexCount,NULL,0);
}
