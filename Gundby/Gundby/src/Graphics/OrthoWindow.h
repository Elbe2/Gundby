// ------------------------------------------------------------------
// Lukas Beinlich, 04.08.2014
// OrthoWindow.h
// Eine einfache Klasse für eine Fläche parallel zum Bildschirm
// ------------------------------------------------------------------

#ifndef __ORTHOWINDOW_H__
#define __ORTHOWINDOW_H__

#include <D3D11.h>
#include <DirectXMath.h>
using namespace DirectX;

class COrthoWindow
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};
	
	int m_VertexCount;
	int m_IndexCount;
	ID3D11Buffer *m_VertexBuffer;
	ID3D11Buffer *m_IndexBuffer;

	bool InitBuffers(ID3D11Device *device,int windowwidth,int windowheight);
	void ShutdownBuffers(void);
	void RenderBuffers(ID3D11DeviceContext *devicecontext);
	void CreateOrthoMatrix(void);
public:
	COrthoWindow(void);
	~COrthoWindow(void);

	bool Init(ID3D11Device* device,int windowwidth,int windowheight);
	void Shutdown(void);
	bool Render(ID3D11DeviceContext* devicecontext);

	int GetIndexCount(void);
};

#endif //__ORTHOWINDOW_H__
