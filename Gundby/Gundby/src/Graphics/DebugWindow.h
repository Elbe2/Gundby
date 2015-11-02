// ------------------------------------------------------------------
// Lukas Beinlich, 04.08.2014
// DebugWindow.h
// Eine Klasse, die das Rendern von 2D Sprites ermöglicht, aber ohne Textur.
// ------------------------------------------------------------------

#ifndef __DEBUGWINDOW_H__
#define __DEBUGWINDOW_H__

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

#include "src\Graphics\Texture.h"

class CDebugWindow
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
	int m_ScreenWidth;
	int m_ScreenHeight;
	int m_BitmapWidth;
	int m_BitmapHeight;
	int m_PrevPosX;
	int m_PrevPosY;

	bool InitBuffers(ID3D11Device *device);
	void ShutdownBuffers(void);
	bool UpdateBuffers(ID3D11DeviceContext *devicecontext,int x,int y);
	void RenderBuffers(ID3D11DeviceContext *devicecontext);
public:
	CDebugWindow(void);
	~CDebugWindow(void);

	bool Init(ID3D11Device* device,int screenwidth,int screenheight,int bitmapwidth,int bitmapheight);
	void Shutdown(void);
	bool Render(ID3D11DeviceContext* devicecontext,int x,int y);

	int GetIndexCount(void);
};

#endif //__DEBUGWINDOW_H__