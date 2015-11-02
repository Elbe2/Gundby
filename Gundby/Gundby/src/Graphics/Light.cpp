// ------------------------------------------------------------------
// Lukas Beinlich, 26.07.2014
// Light.cpp
// Lichtquellen werden hier gespeichert
// ------------------------------------------------------------------

#include "src\Graphics\Light.h"

CLight::CLight(void)
{
}

CLight::~CLight(void)
{
}

void CLight::SetSpecularColor(float r,float g,float b,float a)
{
	m_SpecularColor=XMFLOAT4(r,g,b,a);
}

void CLight::SetAmbientColor(float r,float g,float b,float a)
{
	m_AmbientColor=XMFLOAT4(r,g,b,a);
}

void CLight::SetDiffuseColor(float r,float g,float b,float a)
{
	m_DiffuseColor=XMFLOAT4(r,g,b,a);
}

void CLight::SetDirection(float x,float y,float z)
{
	m_Direction=XMFLOAT3(x,y,z);
}

void CLight::SetSpecularPower(float pow)
{
	m_SpecularPower=pow;
}
