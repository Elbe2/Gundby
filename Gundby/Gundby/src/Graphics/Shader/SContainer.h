// ------------------------------------------------------------------
// SContainer.h
// Lukas Beinlich, 08.10.2015
// Eine Klasse die die Shader kapselt
// ------------------------------------------------------------------
#pragma once

#include "src\Graphics\Shader\SColor.h"
#include "src\Graphics\Shader\SSpecularLight.h"
#include "src\Graphics\Shader\STexture.h"
#include "src\Graphics\Shader\SLight.h"
#include "src\Graphics\Shader\SMultiTexture.h"
#include "src\Graphics\Shader\SHorizontalBlur.h"
#include "src\Graphics\Shader\SVerticalBlur.h"

enum SHADER_NR
{
	SHADER_NR_COLOR=0,
	SHADER_NR_TEXTURE,
	SHADER_NR_LIGHT,
	SHADER_NR_SPECULARLIGHT,
	SHADER_NR_MULTITEXTURE,
	SHADER_NR_HORIZONTALBLUR,
	SHADER_NR_VERTICALBLUR,
	SHADER_NR_COUNT,
};

class CSContainer
{
private:
	CSBasic *m_Shader[SHADER_NR_COUNT];
public:
	CSContainer(void);
	~CSContainer(void);

	bool Init(ID3D11Device *device,HWND hWnd);
	void Shutdown(void);

	CSBasic* GetShader(SHADER_NR nr){return m_Shader[nr];}

	CSLightD* GetLightDShader(void){return (CSLightD*)m_Shader[SHADER_NR_LIGHT];}
	CSTexture* GetTextureShader(void){return (CSTexture*)m_Shader[SHADER_NR_TEXTURE];}
	CSColor* GetColorShader(void){return (CSColor*)m_Shader[SHADER_NR_COLOR];}
	CSSpecularLight* GetSpecularLightShader(void){return (CSSpecularLight*)m_Shader[SHADER_NR_SPECULARLIGHT];}
	CSMultiTexture* GetMultiTextureShader(void){return (CSMultiTexture*)m_Shader[SHADER_NR_MULTITEXTURE];}
	CSHorizontalBlur* GetHorizontalBlurShader(void){return (CSHorizontalBlur*)m_Shader[SHADER_NR_HORIZONTALBLUR];}
	CSVerticalBlur* GetVerticalBlurShader(void){return (CSVerticalBlur*)m_Shader[SHADER_NR_VERTICALBLUR];}
};

