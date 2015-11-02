// ------------------------------------------------------------------
// Lukas Beinlich, 04.08.2014
// OrthoWindow.h
// Eine einfache Klasse für eine Fläche parallel zum Bildschirm
// ------------------------------------------------------------------

#include "src\Graphics\OrthoWindow.h"

COrthoWindow::COrthoWindow(void)
{
	m_VertexBuffer=NULL;
	m_IndexBuffer=NULL;
}

COrthoWindow::~COrthoWindow(void)
{
}

bool COrthoWindow::Init(ID3D11Device *device,int windowwidth,int windowheight)
{
	if(!InitBuffers(device,windowwidth,windowheight))
		return false;
	return true;
}

void COrthoWindow::Shutdown(void)
{
	ShutdownBuffers();
}

bool COrthoWindow::Render(ID3D11DeviceContext *devicecontext)
{
	RenderBuffers(devicecontext);
	return true;
}

int COrthoWindow::GetIndexCount(void)
{
	return m_IndexCount;
}

bool COrthoWindow::InitBuffers(ID3D11Device *device,int windowwidth,int windowheight)
{
	float left;
	float right;
	float top;
	float bottom;
	VertexType *vertices;
	unsigned long *indices;
	D3D11_BUFFER_DESC VertexBufferDesc;
	D3D11_BUFFER_DESC IndexBufferDesc;
	D3D11_SUBRESOURCE_DATA VertexData;
	D3D11_SUBRESOURCE_DATA IndexData;

	left=(float)((windowwidth/2)*-1);
	right=left+(float)windowwidth;
	top=(float)((windowheight/2));
	bottom=top-(float)windowheight;

	m_VertexCount=4;
	m_IndexCount=6;
	vertices=new VertexType[m_VertexCount];
	if(!vertices)
		return false;
	indices=new unsigned long[m_IndexCount];
	if(!indices)
		return false;

	// oben rechts
	vertices[0].position=XMFLOAT3(right,top,0.0f);
	vertices[0].texture=XMFLOAT2(1.0f,0.0f);
	// unten rechts
	vertices[1].position=XMFLOAT3(right,bottom,0.0f);
	vertices[1].texture=XMFLOAT2(1.0f,1.0f);
	// oben links
	vertices[2].position=XMFLOAT3(left,top,0.0f);
	vertices[2].texture=XMFLOAT2(0.0f,0.0f);
	// unten links
	vertices[3].position=XMFLOAT3(left,bottom,0.0f);
	vertices[3].texture=XMFLOAT2(0.0f,1.0f);

	indices[0]=0;
	indices[1]=1;
	indices[2]=2;
	indices[3]=2;
	indices[4]=1;
	indices[5]=3;

	// Desc erstellen
	VertexBufferDesc.Usage=D3D11_USAGE_DEFAULT;
	VertexBufferDesc.ByteWidth=sizeof(VertexType)*m_VertexCount;
	VertexBufferDesc.BindFlags=D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags=0;
	VertexBufferDesc.MiscFlags=0;
	VertexBufferDesc.StructureByteStride=0;

	VertexData.pSysMem=vertices;
	VertexData.SysMemPitch=0;
	VertexData.SysMemSlicePitch=0;

	if(FAILED(device->CreateBuffer(&VertexBufferDesc,&VertexData,&m_VertexBuffer)))
		return false;

	IndexBufferDesc.Usage=D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth=sizeof(unsigned long) * m_IndexCount;
	IndexBufferDesc.BindFlags=D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags=0;
	IndexBufferDesc.MiscFlags=0;
	IndexBufferDesc.StructureByteStride=0;

	IndexData.pSysMem=indices;
	IndexData.SysMemPitch=0;
	IndexData.SysMemSlicePitch=0;
	
	if(FAILED(device->CreateBuffer(&IndexBufferDesc,&IndexData,&m_IndexBuffer)))
		return false;
	
	delete[] vertices;
	vertices=NULL;
	
	delete[] indices;
	indices=NULL;
	
	return true;
}

void COrthoWindow::ShutdownBuffers(void)
{
	if(m_IndexBuffer)
	{
		m_IndexBuffer->Release();
		m_IndexBuffer=NULL;
	}
	if(m_VertexBuffer)
	{
		m_VertexBuffer->Release();
		m_VertexBuffer=NULL;
	}
}

void COrthoWindow::RenderBuffers(ID3D11DeviceContext *devicecontext)
{
	unsigned int stride;
	unsigned int offset;
	stride = sizeof(VertexType);
	offset=0;
	devicecontext->IASetVertexBuffers(0,1,&m_VertexBuffer,&stride,&offset);
	devicecontext->IASetIndexBuffer(m_IndexBuffer,DXGI_FORMAT_R32_UINT,0);
	devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
