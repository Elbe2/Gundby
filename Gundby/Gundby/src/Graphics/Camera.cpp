// ------------------------------------------------------------------
// Camera.cpp
// Lukas Beinlich, 05.10.2015
// Eine einfache Kamera
// ------------------------------------------------------------------

#include "Camera.h"


Camera::Camera()
{
	m_PosX = 0;
	m_PosY = 0;
	m_PosZ = 0;
	m_RotX = 0;
	m_RotY = 0;
	m_RotZ = 0;
}


Camera::~Camera()
{
}

void Camera::SetPosition(float x, float y, float z)
{
	m_PosX = x;
	m_PosY = y;
	m_PosZ = z;
}

void Camera::SetRotation(float pitch, float yaw, float roll)
{
	m_RotX = pitch;
	m_RotY = yaw;
	m_RotZ = roll;
}

XMFLOAT3 Camera::GetPosition(void)
{
	return XMFLOAT3(m_PosX, m_PosY, m_PosZ);
}

XMFLOAT3 Camera::GetRotation(void)
{
	return XMFLOAT3(m_RotX, m_RotY, m_RotZ);
}

void Camera::Render(void)
{
	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Load it into a XMVECTOR structure.
	upVector = XMLoadFloat3(&up);

	// Setup the position of the camera in the world.
	position.x = m_PosX;
	position.y = m_PosY;
	position.z = m_PosZ;

	// Load it into a XMVECTOR structure.
	positionVector = XMLoadFloat3(&position);

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Load it into a XMVECTOR structure.
	lookAtVector = XMLoadFloat3(&lookAt);

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_RotX * 0.0174532925f;
	yaw = m_RotY * 0.0174532925f;
	roll = m_RotZ * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	// Finally create the view matrix from the three updated vectors.
	m_ViewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}

void Camera::GetViewMatrix(XMMATRIX & viewMatrix)
{
	viewMatrix = m_ViewMatrix;
}
