// ------------------------------------------------------------------
// Model.h
// Lukas Beinlich, 07.10.2015
// Ein einfaches Model
// ------------------------------------------------------------------
#pragma once

#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

class Model
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	bool InitializeBuffers(ID3D11Device *device);
	void ShutdownBuffers(void);
	void RenderBuffers(ID3D11DeviceContext *devicecontext);
public:
	Model(void);
	~Model(void);

	bool Initialize(ID3D11Device *device);
	void Shutdown(void);
	void Render(ID3D11DeviceContext *devicecontext);

	int GetIndexCount(void);

};

