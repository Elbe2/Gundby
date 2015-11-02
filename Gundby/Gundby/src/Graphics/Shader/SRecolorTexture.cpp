// ------------------------------------------------------------------
// Lukas Beinlich, 23.09.2014
// SRecolorShader.cpp
// Die Klasse für einen Shader der eine Textur mit einer Farbe multipliziert (recolortexture.vs + recolortexture.ps)
// ------------------------------------------------------------------

#include "src\Graphics\Shader\SRecolorTexture.h"


CSRecolorTexture::CSRecolorTexture(void)
{
	m_VertexShader=NULL;
	m_PixelShader=NULL;
	m_Layout=NULL;
	m_MatrixBuffer=NULL;
	m_PixelBuffer=NULL;
	m_SampleState=NULL;
}

CSRecolorTexture::~CSRecolorTexture(void)
{
}

bool CSRecolorTexture::Init(ID3D11Device *device,HWND hWnd)
{
	if(!InitShader(device,hWnd,L"Shaders/font/recolortexture.vs",L"Shaders/font/recolortexture.ps"))
		return false;
	return true;
}

void CSRecolorTexture::Shutdown(void)
{
	ShutdownShader();
}

bool CSRecolorTexture::Render(ID3D11DeviceContext *devicecontext,int indexCount,XMMATRIX worldmat,XMMATRIX viewmat,XMMATRIX projmat,ID3D11ShaderResourceView **texture,XMFLOAT4 pixelColor,ID3D11Buffer *positionBuffer,unsigned int instancecnt)
{
	if(!SetShaderParameters(devicecontext,worldmat,viewmat,projmat,texture,pixelColor,positionBuffer))
		return false;
	RenderShader(devicecontext,indexCount,instancecnt);
	return true;
}

bool CSRecolorTexture::InitShader(ID3D11Device *device,HWND hWnd,WCHAR *vsFilename,WCHAR *psFilename)
{
	ID3D10Blob *errorMessage;
	ID3D10Blob *vertexShaderBuffer;
	ID3D10Blob *pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC pixelBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	
	errorMessage=NULL;
	vertexShaderBuffer=NULL;
	pixelShaderBuffer=NULL;

	if(FAILED(D3DCompileFromFile(vsFilename,NULL,NULL,"FontVertexShader","vs_5_0",D3D10_SHADER_ENABLE_STRICTNESS,0,&vertexShaderBuffer,&errorMessage)))
	{
		if(errorMessage)
			OutputShaderErrorMessage(errorMessage,hWnd,vsFilename);
		else
			MessageBox(hWnd,vsFilename,L"Missing Shader File",MB_OK);
		return false;
	}
	if(FAILED(D3DCompileFromFile(psFilename,NULL,NULL,"FontPixelShader","ps_5_0",D3D10_SHADER_ENABLE_STRICTNESS,0,&pixelShaderBuffer,&errorMessage)))
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

	numElements=sizeof(polygonLayout)/sizeof(polygonLayout[0]);
	if(FAILED(device->CreateInputLayout(polygonLayout,numElements,vertexShaderBuffer->GetBufferPointer(),vertexShaderBuffer->GetBufferSize(),&m_Layout)))
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

	pixelBufferDesc.Usage=D3D11_USAGE_DYNAMIC;
	pixelBufferDesc.ByteWidth=sizeof(PixelBufferType);
	pixelBufferDesc.BindFlags=D3D11_BIND_CONSTANT_BUFFER;
	pixelBufferDesc.CPUAccessFlags=D3D11_CPU_ACCESS_WRITE;
	pixelBufferDesc.MiscFlags=0;
	pixelBufferDesc.StructureByteStride=0;
	if(FAILED(device->CreateBuffer(&pixelBufferDesc,NULL,&m_PixelBuffer)))
		return false;

	samplerDesc.Filter=D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU=D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV=D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW=D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MaxAnisotropy=0;
	samplerDesc.MipLODBias=0.0f;
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

void CSRecolorTexture::ShutdownShader(void)
{
	if(m_SampleState)
	{
		m_SampleState->Release();
		m_SampleState=NULL;
	}
	if(m_PixelBuffer)
	{
		m_PixelBuffer->Release();
		m_PixelBuffer=NULL;
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

bool CSRecolorTexture::SetShaderParameters(ID3D11DeviceContext *devicecontext,XMMATRIX worldmat,XMMATRIX viewmat,XMMATRIX projmat,ID3D11ShaderResourceView **texture,XMFLOAT4 pixelColor,ID3D11Buffer *positionBuffer)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType *dataPtr;
	PixelBufferType *dataPtr2;
	unsigned int bufferNr;

	worldmat = XMMatrixTranspose(worldmat);
	viewmat = XMMatrixTranspose(viewmat);
	projmat = XMMatrixTranspose(projmat);

	if(FAILED(devicecontext->Map(m_MatrixBuffer,0,D3D11_MAP_WRITE_DISCARD,0,&mappedResource)))
		return false;
	dataPtr=(MatrixBufferType*)mappedResource.pData;
	dataPtr->world=worldmat;
	dataPtr->view=viewmat;
	dataPtr->proj=projmat;
	devicecontext->Unmap(m_MatrixBuffer,0);

	bufferNr=0;
	// Hier den Buffer als Paramter übergeben
	devicecontext->VSSetConstantBuffers(bufferNr,1,&positionBuffer);
	bufferNr++;
	devicecontext->VSSetConstantBuffers(bufferNr,1,&m_MatrixBuffer);
	devicecontext->PSSetShaderResources(0,1,texture);

	if(FAILED(devicecontext->Map(m_PixelBuffer,0,D3D11_MAP_WRITE_DISCARD,0,&mappedResource)))
		return false;
	dataPtr2=(PixelBufferType*)mappedResource.pData;
	dataPtr2->pixelColor=pixelColor;
	devicecontext->Unmap(m_PixelBuffer,0);
	bufferNr=0;
	devicecontext->PSSetConstantBuffers(bufferNr,1,&m_PixelBuffer);
	return true;
}

void CSRecolorTexture::RenderShader(ID3D11DeviceContext *devicecontext,int indexCount,unsigned int instancecnt)
{
	devicecontext->IASetInputLayout(m_Layout);
	devicecontext->VSSetShader(m_VertexShader,NULL,0);
	devicecontext->PSSetShader(m_PixelShader,NULL,0);
	devicecontext->PSSetSamplers(0,1,&m_SampleState);
	devicecontext->DrawIndexedInstanced(indexCount,instancecnt,0,0,0);
}
