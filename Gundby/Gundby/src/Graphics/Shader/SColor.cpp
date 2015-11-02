// ------------------------------------------------------------------
// Lukas Beinlic, 24.07.2014
// Color.h
// Die Klasse für einen einfachen Shader (color.vs+color.ps)
// ------------------------------------------------------------------

#include "src\Graphics\Shader\SColor.h"

CSColor::CSColor(void)
{
	m_VertexShader=NULL;
	m_PixelShader=NULL;
	m_Layout=NULL;
	m_MatrixBuffer=NULL;
}

CSColor::~CSColor(void)
{
}

bool CSColor::Init(ID3D11Device *device,HWND hWnd)
{
	if(!InitShader(device,hWnd,L"Shaders/color/color.vs",L"Shaders/color/color.ps"))
		return false;
	return true;
}

void CSColor::Shutdown(void)
{
	ShutdownShader();
}

bool CSColor::Render(ID3D11DeviceContext *devicecontext,int indexCount, XMMATRIX worldmat, XMMATRIX viewmat,XMMATRIX projmat)
{
	if(!SetShaderParameters(devicecontext,worldmat,viewmat,projmat))
		return false;
	RenderShader(devicecontext,indexCount);
	return true;
}

bool CSColor::InitShader(ID3D11Device *device,HWND hWnd,WCHAR *vsFilename,WCHAR *psFilename)
{
	ID3D10Blob *errorMessage;
	ID3D10Blob *vertexShaderBuffer;
	ID3D10Blob *pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	
	errorMessage = NULL;
	vertexShaderBuffer = NULL;
	pixelShaderBuffer = NULL;

	if (FAILED(D3DCompileFromFile(vsFilename, NULL, NULL, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage)))
	{
		if (errorMessage)
			OutputShaderErrorMessage(errorMessage, hWnd, vsFilename);
		else
			MessageBox(hWnd, vsFilename, L"Missing Shader File", MB_OK);
		return false;
	}
	if (FAILED(D3DCompileFromFile(psFilename, NULL, NULL, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage)))
	{
		if (errorMessage)
			OutputShaderErrorMessage(errorMessage, hWnd, psFilename);
		else
			MessageBox(hWnd, psFilename, L"Missing Shader File", MB_OK);
		return false;
	}
	if (FAILED(device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_VertexShader)))
		return false;
	if (FAILED(device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_PixelShader)))
		return false;

	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);
	if (FAILED(device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_Layout)))
		return false;
	
	vertexShaderBuffer->Release();
	vertexShaderBuffer = NULL;
	pixelShaderBuffer->Release();
	pixelShaderBuffer = NULL;

	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;
	if (FAILED(device->CreateBuffer(&matrixBufferDesc, NULL, &m_MatrixBuffer)))
		return false;
	return true;
}

void CSColor::ShutdownShader(void)
{
	if(m_MatrixBuffer)
	{
		m_MatrixBuffer->Release();
		m_MatrixBuffer = NULL;
	}
	if(m_Layout)
	{
		m_Layout->Release();
		m_Layout = NULL;
	}
	if(m_PixelShader)
	{
		m_PixelShader->Release();
		m_PixelShader = NULL;
	}
	if(m_VertexShader)
	{
		m_VertexShader->Release();
		m_VertexShader = NULL;
	}
}

bool CSColor::SetShaderParameters(ID3D11DeviceContext *devicecontext,XMMATRIX worldmat,XMMATRIX viewmat, XMMATRIX projmat)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType *dataPtr;
	unsigned int bufferNr;

	XMMATRIX worldviewprojmat = XMMatrixIdentity();
	worldviewprojmat = worldmat;
	worldviewprojmat = XMMatrixMultiply(worldviewprojmat, viewmat);
	worldviewprojmat = XMMatrixMultiply(worldviewprojmat, projmat);
	worldmat = XMMatrixTranspose(worldmat);
	viewmat = XMMatrixTranspose(viewmat);
	projmat = XMMatrixTranspose(projmat);
	worldviewprojmat = XMMatrixTranspose(worldviewprojmat);

	if (FAILED(devicecontext->Map(m_MatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->world = worldmat;
	dataPtr->view = viewmat;
	dataPtr->projection = projmat;
	dataPtr->worldviewprojection = worldviewprojmat;
	devicecontext->Unmap(m_MatrixBuffer,0);

	bufferNr=0;
	devicecontext->VSSetConstantBuffers(bufferNr,1,&m_MatrixBuffer);
	return true;
}

void CSColor::RenderShader(ID3D11DeviceContext *devicecontext,int indexCount)
{
	devicecontext->IASetInputLayout(m_Layout);
	devicecontext->VSSetShader(m_VertexShader,NULL,0);
	devicecontext->PSSetShader(m_PixelShader,NULL,0);
	devicecontext->DrawIndexed(indexCount,NULL,0);
}
