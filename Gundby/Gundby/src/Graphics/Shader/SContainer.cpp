// ------------------------------------------------------------------
// Lukas Beinlich, 28.07.2014
// SContainer.h
// Eine Klasse die die Shader kapselt
// ------------------------------------------------------------------

#include "src\Graphics\Shader\SContainer.h"

CSContainer::CSContainer(void)
{
	m_Shader[SHADER_NR_COLOR]=NULL;
	m_Shader[SHADER_NR_LIGHT]=NULL;
	m_Shader[SHADER_NR_TEXTURE]=NULL;
	m_Shader[SHADER_NR_SPECULARLIGHT]=NULL;
	m_Shader[SHADER_NR_MULTITEXTURE]=NULL;
	m_Shader[SHADER_NR_HORIZONTALBLUR]=NULL;
	m_Shader[SHADER_NR_VERTICALBLUR]=NULL;
}

CSContainer::~CSContainer(void)
{
}

bool CSContainer::Init(ID3D11Device *device,HWND hWnd)
{
	m_Shader[SHADER_NR_COLOR]=new CSColor();
	if(!m_Shader[SHADER_NR_COLOR])
		return false;
	if(!m_Shader[SHADER_NR_COLOR]->Init(device,hWnd))
		return false;

	m_Shader[SHADER_NR_TEXTURE]=new CSTexture();
	if(!m_Shader[SHADER_NR_TEXTURE])
		return false;
	if(!m_Shader[SHADER_NR_TEXTURE]->Init(device,hWnd))
		return false;

	m_Shader[SHADER_NR_LIGHT]=new CSLightD();
	if(!m_Shader[SHADER_NR_LIGHT])
		return false;
	if(!m_Shader[SHADER_NR_LIGHT]->Init(device,hWnd))
		return false;

	m_Shader[SHADER_NR_SPECULARLIGHT]=new CSSpecularLight();
	if(!m_Shader[SHADER_NR_SPECULARLIGHT])
		return false;
	if(!m_Shader[SHADER_NR_SPECULARLIGHT]->Init(device,hWnd))
		return false;

	m_Shader[SHADER_NR_MULTITEXTURE]=new CSMultiTexture();
	if(!m_Shader[SHADER_NR_MULTITEXTURE])
		return false;
	if(!m_Shader[SHADER_NR_MULTITEXTURE]->Init(device,hWnd))
		return false;

	m_Shader[SHADER_NR_HORIZONTALBLUR]=new CSHorizontalBlur();
	if(!m_Shader[SHADER_NR_HORIZONTALBLUR])
		return false;
	if(!m_Shader[SHADER_NR_HORIZONTALBLUR]->Init(device,hWnd))
		return false;

	m_Shader[SHADER_NR_VERTICALBLUR]=new CSVerticalBlur();
	if(!m_Shader[SHADER_NR_VERTICALBLUR])
		return false;
	if(!m_Shader[SHADER_NR_VERTICALBLUR]->Init(device,hWnd))
		return false;

	return true;
}

void CSContainer::Shutdown(void)
{
	for(int i=0;i<SHADER_NR_COUNT;i++)
	{
		if(m_Shader[i])
		{
			m_Shader[i]->Shutdown();
			delete m_Shader[i];
			m_Shader[i]=NULL;
		}
	}
}
