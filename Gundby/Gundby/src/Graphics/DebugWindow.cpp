// ------------------------------------------------------------------
// Lukas Beinlich, 04.08.2014
// DebugWindow.cpp
// Eine Klasse, die das Rendern von 2D Sprites ohne Textur ermöglicht.
// ------------------------------------------------------------------

#include "src\Graphics\DebugWindow.h"

CDebugWindow::CDebugWindow(void)
{
	m_VertexBuffer=NULL;
	m_IndexBuffer=NULL;
}

CDebugWindow::~CDebugWindow(void)
{
}

bool CDebugWindow::Init(ID3D11Device *device,int screenwidth,int screenheight,int bitmapwidth,int bitmapheight)
{
	m_ScreenWidth=screenwidth;
	m_ScreenHeight=screenheight;
	m_BitmapHeight=bitmapheight;
	m_BitmapWidth=bitmapwidth;
	m_PrevPosX=-1;
	m_PrevPosY=-1;
	if(!InitBuffers(device))
		return false;
	return true;
}

void CDebugWindow::Shutdown(void)
{
	ShutdownBuffers();
}

bool CDebugWindow::Render(ID3D11DeviceContext *devicecontext,int posx,int posy)
{
	if(!UpdateBuffers(devicecontext,posx,posy))
		return false;
	RenderBuffers(devicecontext);
	return true;
}

int CDebugWindow::GetIndexCount(void)
{
	return m_IndexCount;
}

bool CDebugWindow::InitBuffers(ID3D11Device *device)
{
	VertexType *vertices;
	unsigned long *indices;
	D3D11_BUFFER_DESC VertexBufferDesc;
	D3D11_BUFFER_DESC IndexBufferDesc;
	D3D11_SUBRESOURCE_DATA VertexData;
	D3D11_SUBRESOURCE_DATA IndexData;

	m_VertexCount=4;
	m_IndexCount=6;
	vertices=new VertexType[m_VertexCount];
	if(!vertices)
		return false;
	indices=new unsigned long[m_IndexCount];
	if(!indices)
		return false;

	// Vertices erstmal auf 0 setzen
	memset(vertices,0,sizeof(VertexType)*m_VertexCount);

	indices[0]=0;
	indices[1]=1;
	indices[2]=2;
	indices[3]=2;
	indices[4]=1;
	indices[5]=3;

	// Desc erstellen
	VertexBufferDesc.Usage=D3D11_USAGE_DYNAMIC;
	VertexBufferDesc.ByteWidth=sizeof(VertexType)*m_VertexCount;
	VertexBufferDesc.BindFlags=D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags=D3D11_CPU_ACCESS_WRITE;
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

void CDebugWindow::ShutdownBuffers(void)
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

bool CDebugWindow::UpdateBuffers(ID3D11DeviceContext *devicecontext,int x,int y)
{
	float left;
	float right;
	float top;
	float bottom;
	VertexType *vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType *vertexPtr;

	if((x==m_PrevPosX)&&(y=m_PrevPosY))
		return true;
	m_PrevPosX=x;
	m_PrevPosY=y;

	left=(float)((m_ScreenWidth/2)*-1)+(float)x;
	right=left+(float)m_BitmapWidth;
	top=(float)((m_ScreenHeight/2))-(float)y;
	bottom=top-(float)m_BitmapHeight;

	vertices=new VertexType[m_VertexCount];
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

	if(FAILED(devicecontext->Map(m_VertexBuffer,0,D3D11_MAP_WRITE_DISCARD,0,&mappedResource)))
		return false;
	vertexPtr=(VertexType*)mappedResource.pData;
	memcpy(vertexPtr,vertices,sizeof(VertexType)*m_VertexCount);
	devicecontext->Unmap(m_VertexBuffer,0);

	delete[] vertices;
	vertices=NULL;
	return true;
}

void CDebugWindow::RenderBuffers(ID3D11DeviceContext *devicecontext)
{
	unsigned int stride;
	unsigned int offset;
	stride = sizeof(VertexType);
	offset=0;
	devicecontext->IASetVertexBuffers(0,1,&m_VertexBuffer,&stride,&offset);
	devicecontext->IASetIndexBuffer(m_IndexBuffer,DXGI_FORMAT_R32G32B32A32_UINT,0);
	devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

