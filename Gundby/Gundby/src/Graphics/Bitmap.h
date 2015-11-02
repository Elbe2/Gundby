// ------------------------------------------------------------------
// Lukas Beinlich, 27.07.2014
// Bitmap.h
// Eine Klasse, die das Rendern von 2D Sprites ermöglicht.
// ------------------------------------------------------------------

#ifndef __BITMAP_H__
#define __BITMAP_H__

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

#include "src\Graphics\Texture.h"

class CBitmap
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
	CTexture *m_Texture;
	int m_ScreenWidth;
	int m_ScreenHeight;
	int m_BitmapWidth;
	int m_BitmapHeight;
	int m_PrevPosX;
	int m_PrevPosY;


	bool InitBuffers(ID3D11Device *device);
	bool LoadTexture(ID3D11Device *device,WCHAR *filename);
	void ShutdownBuffers(void);
	void ReleaseTexture(void);
	bool UpdateBuffers(ID3D11DeviceContext *devicecontext,int x,int y);
	void RenderBuffers(ID3D11DeviceContext *devicecontext);
public:
	CBitmap(void);
	~CBitmap(void);

	bool Init(ID3D11Device* device,int screenx,int screeny,WCHAR *filename,int bitmapwidth,int bitmapheight);
	void Shutdown(void);
	bool Render(ID3D11DeviceContext* devicecontext,int x,int y);

	int GetIndexCount(void);
	ID3D11ShaderResourceView** GetTexture(void){return m_Texture->GetTexture();}
	int GetWidth(void){return m_BitmapWidth;}
	int GetHeight(void){return m_BitmapHeight;}
};

#endif //__BITMAP_H__