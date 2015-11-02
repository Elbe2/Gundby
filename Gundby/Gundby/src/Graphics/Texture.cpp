// ------------------------------------------------------------------
// Lukas Beinlich, 25.07.2014
// Texture.cpp
// Eine Klasse, die eine Textur läd und verwaltet
// ------------------------------------------------------------------

#include "src\Graphics\Texture.h"
#include "DDSTextureLoader.h"
using namespace DirectX;

CTexture::CTexture(void)
{
	m_Texture=NULL;
}

CTexture::~CTexture(void)
{
}

bool CTexture::Init(ID3D11Device *device,WCHAR *filename)
{
	if(FAILED(CreateDDSTextureFromFile(device, filename, NULL, &m_Texture)))
		return false;
	return true;
}

void CTexture::Shutdown(void)
{
	if(m_Texture)
	{
		m_Texture->Release();
		m_Texture=NULL;
	}
}
