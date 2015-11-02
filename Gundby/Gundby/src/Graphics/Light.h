// ------------------------------------------------------------------
// Lukas Beinlich, 26.07.2014
// Light.h
// Lichtquellen werden hier gespeichert
// ------------------------------------------------------------------

#ifndef __LIGHT_H__
#define __LIGHT_H__

#include <DirectXMath.h>
using namespace DirectX;

class CLight
{
private:
	XMFLOAT4 m_AmbientColor;
	XMFLOAT4 m_DiffuseColor;
	XMFLOAT3 m_Direction;
	XMFLOAT4 m_SpecularColor;
	float m_SpecularPower;
public:
	CLight();
	~CLight();
	
	void SetAmbientColor(float r,float g,float b,float a);
	void SetDiffuseColor(float r,float g,float b,float a);
	void SetDirection(float x,float y,float z);
	void SetSpecularColor(float x,float y,float z,float a);
	void SetSpecularPower(float pow);


	XMFLOAT3 GetDirection(void){return m_Direction;}
	XMFLOAT4 GetDiffuseColor(void){return m_DiffuseColor;}
	XMFLOAT4 GetAmbientColor(void){return m_AmbientColor;}
	XMFLOAT4 GetSpecularColor(void){return m_SpecularColor;}
	float GetSpecularPower(void){return m_SpecularPower;}
};

#endif
