// ------------------------------------------------------------------
// Lukas Beinlich, 03.08.2014
// FrustrumCulling.cpp
// Die Klasse teste ob ein Objekt gerendert werden muss oder nicht.
// ------------------------------------------------------------------

#include "src\Graphics\FrustrumCulling.h"

CFrustrum::CFrustrum(void)
{
}

CFrustrum::~CFrustrum(void)
{
}

void CFrustrum::ConstructFrustrum(float screendepth,XMMATRIX projectionmatrix,XMMATRIX viewmatrix)
{
	float zMin;
	float r;
	XMMATRIX matrix;
	
	XMFLOAT4X4 projfloat4;
	XMFLOAT4X4 float4matrix;

	XMStoreFloat4x4(&projfloat4, projectionmatrix);

	zMin = -projfloat4._43 / projfloat4._33;
	r = screendepth / (screendepth - zMin);
	projfloat4._33 = r;
	projfloat4._43 = -r * zMin;

	matrix = XMMatrixMultiply(viewmatrix, projectionmatrix);

	XMStoreFloat4x4(&float4matrix, matrix);

	// Near Plane
	m_Planes[0].x = float4matrix._14 + float4matrix._13;
	m_Planes[0].y = float4matrix._24 + float4matrix._23;
	m_Planes[0].z = float4matrix._34 + float4matrix._33;
	m_Planes[0].w = float4matrix._44 + float4matrix._43;
	XMStoreFloat4(&m_Planes[0],XMPlaneNormalize(XMLoadFloat4(&m_Planes[0])));

	// Far Plane
	m_Planes[1].x=float4matrix._14 - float4matrix._13;
	m_Planes[1].y=float4matrix._24 - float4matrix._23;
	m_Planes[1].z=float4matrix._34 - float4matrix._33;
	m_Planes[1].w=float4matrix._44 - float4matrix._43;
	XMStoreFloat4(&m_Planes[1], XMPlaneNormalize(XMLoadFloat4(&m_Planes[1])));

	// Left Plane
	m_Planes[2].x=float4matrix._14 + float4matrix._11;
	m_Planes[2].y=float4matrix._24 + float4matrix._21;
	m_Planes[2].z=float4matrix._34 + float4matrix._31;
	m_Planes[2].w=float4matrix._44 + float4matrix._41;
	XMStoreFloat4(&m_Planes[2], XMPlaneNormalize(XMLoadFloat4(&m_Planes[2])));

	// Right Plane
	m_Planes[3].x=float4matrix._14 - float4matrix._11;
	m_Planes[3].y=float4matrix._24 - float4matrix._21;
	m_Planes[3].z=float4matrix._34 - float4matrix._31;
	m_Planes[3].w=float4matrix._44 - float4matrix._41;
	XMStoreFloat4(&m_Planes[3], XMPlaneNormalize(XMLoadFloat4(&m_Planes[3])));

	// Top Plane
	m_Planes[4].x=float4matrix._14 - float4matrix._12;
	m_Planes[4].y=float4matrix._24 - float4matrix._22;
	m_Planes[4].z=float4matrix._34 - float4matrix._32;
	m_Planes[4].w=float4matrix._44 - float4matrix._42;
	XMStoreFloat4(&m_Planes[4], XMPlaneNormalize(XMLoadFloat4(&m_Planes[4])));

	// Bottom Plane
	m_Planes[5].x=float4matrix._14 + float4matrix._12;
	m_Planes[5].y=float4matrix._24 + float4matrix._22;
	m_Planes[5].z=float4matrix._34 + float4matrix._23;
	m_Planes[5].w=float4matrix._44 + float4matrix._24;
	XMStoreFloat4(&m_Planes[5], XMPlaneNormalize(XMLoadFloat4(&m_Planes[5])));
}

bool CFrustrum::CheckPoint(float x,float y,float z)
{
	XMFLOAT4 f4(x, y, z, 0);
	XMVECTOR plane;
	XMVECTOR point = XMLoadFloat4(&f4);
	for (int i = 0;i < 6;i++)
	{
		plane = XMLoadFloat4(&m_Planes[i]);
		XMStoreFloat4(&f4, XMPlaneDotCoord(plane, point));
		if (f4.x < 0.0f)
			return false;
	}
	return true;
}

bool CFrustrum::CheckCube(float x,float y,float z,float r)
{
	XMFLOAT4 f4;
	XMVECTOR plane;
	for(int i=0;i<6;i++)
	{
		plane = XMLoadFloat4(&m_Planes[i]);
		XMStoreFloat4(&f4, XMPlaneDotCoord(plane, XMLoadFloat4(&XMFLOAT4(x - r, y - r, z - r, 0))));
		if (f4.x >= 0.0f)
			continue;
		XMStoreFloat4(&f4, XMPlaneDotCoord(plane, XMLoadFloat4(&XMFLOAT4(x + r, y - r, z - r, 0))));
		if (f4.x >= 0.0f)
			continue;
		XMStoreFloat4(&f4, XMPlaneDotCoord(plane, XMLoadFloat4(&XMFLOAT4(x - r, y + r, z - r, 0))));
		if (f4.x >= 0.0f)
			continue;
		XMStoreFloat4(&f4, XMPlaneDotCoord(plane, XMLoadFloat4(&XMFLOAT4(x + r, y + r, z - r, 0))));
		if (f4.x >= 0.0f)
			continue;
		XMStoreFloat4(&f4, XMPlaneDotCoord(plane, XMLoadFloat4(&XMFLOAT4(x - r, y - r, z + r, 0))));
		if (f4.x >= 0.0f)
			continue;
		XMStoreFloat4(&f4, XMPlaneDotCoord(plane, XMLoadFloat4(&XMFLOAT4(x + r, y - r, z + r, 0))));
		if (f4.x >= 0.0f)
			continue;
		XMStoreFloat4(&f4, XMPlaneDotCoord(plane, XMLoadFloat4(&XMFLOAT4(x - r, y + r, z + r, 0))));
		if (f4.x >= 0.0f)
			continue;
		XMStoreFloat4(&f4, XMPlaneDotCoord(plane, XMLoadFloat4(&XMFLOAT4(x + r, y + r, z + r, 0))));
		if (f4.x >= 0.0f)
			continue;
		return false;
	}
	return true;
}

bool CFrustrum::CheckSphere(float x, float y, float z, float r)
{
	XMFLOAT4 f4;
	XMVECTOR plane;
	for (int i = 0;i < 6;i++)
	{
		XMStoreFloat4(&f4, XMPlaneDotCoord(plane, XMLoadFloat4(&XMFLOAT4(x, y, z, 0))));
		if (f4.x < -r)
			return false;
	}
	return true;
}

bool CFrustrum::CheckRectangle(float x,float y,float z,float xs,float ys,float zs)
{
	XMFLOAT4 f4;
	XMVECTOR plane;
	for (int i = 0;i<6;i++)
	{
		plane = XMLoadFloat4(&m_Planes[i]);
		XMStoreFloat4(&f4, XMPlaneDotCoord(plane, XMLoadFloat4(&XMFLOAT4(x - xs, y - ys, z - zs, 0))));
		if (f4.x >= 0.0f)
			continue;
		XMStoreFloat4(&f4, XMPlaneDotCoord(plane, XMLoadFloat4(&XMFLOAT4(x + xs, y - ys, z - zs, 0))));
		if (f4.x >= 0.0f)
			continue;
		XMStoreFloat4(&f4, XMPlaneDotCoord(plane, XMLoadFloat4(&XMFLOAT4(x - xs, y + ys, z - zs, 0))));
		if (f4.x >= 0.0f)
			continue;
		XMStoreFloat4(&f4, XMPlaneDotCoord(plane, XMLoadFloat4(&XMFLOAT4(x + xs, y + ys, z - zs, 0))));
		if (f4.x >= 0.0f)
			continue;
		XMStoreFloat4(&f4, XMPlaneDotCoord(plane, XMLoadFloat4(&XMFLOAT4(x - xs, y - ys, z + zs, 0))));
		if (f4.x >= 0.0f)
			continue;
		XMStoreFloat4(&f4, XMPlaneDotCoord(plane, XMLoadFloat4(&XMFLOAT4(x + xs, y - ys, z + zs, 0))));
		if (f4.x >= 0.0f)
			continue;
		XMStoreFloat4(&f4, XMPlaneDotCoord(plane, XMLoadFloat4(&XMFLOAT4(x - xs, y + ys, z + zs, 0))));
		if (f4.x >= 0.0f)
			continue;
		XMStoreFloat4(&f4, XMPlaneDotCoord(plane, XMLoadFloat4(&XMFLOAT4(x + xs, y + ys, z + zs, 0))));
		if (f4.x >= 0.0f)
			continue;
		return false;
	}
	return true;
}
