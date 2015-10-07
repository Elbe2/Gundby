// ------------------------------------------------------------------
// Camera.h
// Lukas Beinlich, 05.10.2015
// Eine einfache Kamera
// ------------------------------------------------------------------
#pragma once

#include <DirectXMath.h>
using namespace DirectX;

class Camera
{
private:
	float m_PosX;
	float m_PosY;
	float m_PosZ;
	float m_RotX;
	float m_RotY;
	float m_RotZ;
	XMMATRIX m_ViewMatrix;
public:
	Camera();
	~Camera();

	void SetPosition(float x, float y, float z);
	void SetRotation(float pitch, float yaw, float roll);

	XMFLOAT3 GetPosition(void);
	XMFLOAT3 GetRotation(void);

	void Render(void);
	void GetViewMatrix(XMMATRIX &viewMatrix);
};

