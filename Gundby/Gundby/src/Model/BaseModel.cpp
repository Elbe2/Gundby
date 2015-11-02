// ------------------------------------------------------------------
// Lukas Beinlich, 03.08.2014
// BaseModel.cpp
// Eine Basisklasse für Objekte mit Vertices
// ------------------------------------------------------------------

#include "src\Model\BaseModel.h"

CBaseModel::CBaseModel(void)
{
	m_VertexBuffer=NULL;
	m_IndexBuffer=NULL;
	m_Texture=NULL;
	m_Model=NULL;
}

CBaseModel::~CBaseModel(void)
{
}

bool CBaseModel::Init(ID3D11Device *device,char *modelfile,int texcnt,WCHAR *filename1,WCHAR *filename2,WCHAR *filename3,WCHAR *filename4)
{
	if(!LoadModel(modelfile))
		return false;
	if(!InitBuffers(device))
		return false;
	if(!LoadTexture(device,texcnt,filename1,filename2,filename3,filename4))
		return false;
	return true;
}

void CBaseModel::Shutdown(void)
{
	ReleaseTexture();
	ShutdownBuffers();
	ReleaseModel();
}

void CBaseModel::Render(ID3D11DeviceContext *devicecontext)
{
	RenderBuffers(devicecontext);
}

int CBaseModel::GetIndexCount(void)
{
	return m_IndexCount;
}

bool CBaseModel::InitBuffers(ID3D11Device *device)
{
	VertexType *vertices;
	unsigned long *indices;
	D3D11_BUFFER_DESC VertexBufferDesc;
	D3D11_BUFFER_DESC IndexBufferDesc;
	D3D11_SUBRESOURCE_DATA VertexData;
	D3D11_SUBRESOURCE_DATA IndexData;

	vertices=new VertexType[m_VertexCount];
	if(!vertices)
		return false;
	indices=new unsigned long[m_IndexCount];
	if(!indices)
		return false;

	for(int i=0;i<m_VertexCount;i++)
	{
		vertices[i].position=XMFLOAT3(m_Model[i].x,m_Model[i].y,m_Model[i].z);
		vertices[i].texture=XMFLOAT2(m_Model[i].tu,m_Model[i].tv);
		vertices[i].normal=XMFLOAT3(m_Model[i].nx,m_Model[i].ny,m_Model[i].nz);
	}

	for(int i=0;i<m_IndexCount;i++)
		indices[i]=m_Indices[i];

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

bool CBaseModel::LoadTexture(ID3D11Device *device,int texcnt,WCHAR *filename1,WCHAR *filename2,WCHAR *filename3,WCHAR *filename4)
{
	m_Texture=new CTextureArray();
	if(!m_Texture)
		return false;
	if(!m_Texture->Init(device,texcnt,filename1,filename2,filename3,filename4))
		return false;
	return true;
}

/*bool CBaseModel::LoadModel(char *filename)
{
	ifstream fin;
	char input;

	fin.open(filename);
	if(fin.fail())
		return false;
	fin.get(input);
	while(input!=':')
		fin.get(input);

	fin>>m_VertexCount;
	m_IndexCount=m_VertexCount;
	m_Model = new ModelType[m_VertexCount];
	if(!m_Model)
		return false;
	fin.get(input);
	while(input!=':')
		fin.get(input);
	fin.get(input);
	fin.get(input);
	for(int i=0;i<m_VertexCount;i++)
	{
		fin>>m_Model[i].x>>m_Model[i].y>>m_Model[i].z;
		fin>>m_Model[i].tu>>m_Model[i].tv;
		fin>>m_Model[i].nx>>m_Model[i].ny>>m_Model[i].nz;
	}
	fin.close();
	return true;
}*/

void CBaseModel::ShutdownBuffers(void)
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

void CBaseModel::ReleaseTexture(void)
{
	if(m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture=NULL;
	}
}

void CBaseModel::ReleaseModel(void)
{
	if(m_Model)
	{
		delete[] m_Model;
		m_Model=NULL;
	}
}

void CBaseModel::RenderBuffers(ID3D11DeviceContext *devicecontext)
{
	unsigned int stride;
	unsigned int offset;
	stride = sizeof(VertexType);
	offset=0;
	devicecontext->IASetVertexBuffers(0,1,&m_VertexBuffer,&stride,&offset);
	devicecontext->IASetIndexBuffer(m_IndexBuffer,DXGI_FORMAT_R32G32B32A32_UINT,0);
	devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

