// ------------------------------------------------------------------
// Lukas Beinlich, 04.08.2014
// TextureArray.cpp
// Eine Klasse, die mehr als eine Textur läd und verwaltet
// ------------------------------------------------------------------

#include "src\Graphics\TextureArray.h"
#include "DDSTextureLoader.h"
using namespace DirectX;

CTextureArray::CTextureArray(void)
{
	m_Texture=NULL;
}

CTextureArray::~CTextureArray(void)
{
}

bool CTextureArray::Init(ID3D11Device *device,int texcnt,WCHAR *filename1,WCHAR *filename2,WCHAR *filename3,WCHAR *filename4)
{
	m_Texturecnt=texcnt;
	m_Texture=new ID3D11ShaderResourceView*[m_Texturecnt];
	if(!m_Texture)
		return false;
	
	if(FAILED(CreateDDSTextureFromFile(device, filename1, NULL, &m_Texture[0])))
		return false;
	if(m_Texturecnt>1)
		if(FAILED(CreateDDSTextureFromFile(device, filename2, NULL, &m_Texture[1])))
			return false;
	if(m_Texturecnt>2)
		if(FAILED(CreateDDSTextureFromFile(device, filename3, NULL, &m_Texture[2])))
			return false;
	if(m_Texturecnt>3)
		if(FAILED(CreateDDSTextureFromFile(device, filename4, NULL, &m_Texture[3])))
			return false;
	return true;
}

void CTextureArray::Shutdown(void)
{
	for(int i=0;i<m_Texturecnt;i++)
	{
		if(m_Texture[i])
		{
			m_Texture[i]->Release();
			m_Texture[i]=NULL;
		}
	}
	if(m_Texture)
	{
		delete m_Texture;
		m_Texture=NULL;
	}
}
