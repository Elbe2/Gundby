// ------------------------------------------------------------------
// Lukas Beinlich, 03.08.2014
// FrustrumCulling.h
// Die Klasse teste ob ein Objekt gerendert werden muss oder nicht.
// ------------------------------------------------------------------

#ifndef __FRUSTRUMCULLING_H__
#define __FRUSTRUMCULLING_H__

#include <DirectXMath.h>
using namespace DirectX;

class CFrustrum
{
private:
	XMFLOAT4 m_Planes[6];
public:
	CFrustrum(void);
	~CFrustrum(void);
	
	void ConstructFrustrum(float screendepth,XMMATRIX projectionmatrix,XMMATRIX viewmatrix);
	
	bool CheckPoint(float x,float y,float z);
	bool CheckCube(float, float, float, float);
	bool CheckSphere(float, float, float, float);
	bool CheckRectangle(float, float, float, float, float, float);
};

#endif //__FRUSTRUMCULLING_H__
