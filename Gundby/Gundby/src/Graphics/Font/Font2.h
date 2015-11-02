// ------------------------------------------------------------------
// Lukas Beinlich, 22.09.2014
// Font2.h
// Meine eigene Font-Engine
// ------------------------------------------------------------------
#pragma once

#include "src\Graphics\Texture.h"
#include "src\Graphics\Shader\SRecolorTexture.h"
#include <DirectXMath.h>
using namespace DirectX;

enum FONT2_FORMAT
{
	FONT2_FORMAT_LEFT=1,
	FONT2_FORMAT_CENTER=2,
	FONT2_FORMAT_RIGHT=3,
	FONT2_FORMAT_TOP=10,
	FONT2_FORMAT_BOTTOM=20,
	FONT2_FORMAT_VCENTER=30,
};

struct SSentence
{
	ID3D11Buffer *vertexBuffer;
	ID3D11Buffer *indexBuffer;
	ID3D11Buffer *PositionArrayBuffer;
	int vertexCount;
	int indexCount;
	int maxLength;
	int formatlr; // Rechts,links,Center
	int formatou; // Format oben unten
	float red;
	float green;
	float blue;
	int posx;
	int posy;
	int sizex; // Wird nur benutzt, wenn formatlr = center oder right ist 
	int sizey; // Wird nur benutzt, wenn formatou = vcenter oder bottom ist
	int height;
	int instanceCount;
	string text;
};

class CFont2
{
	struct PositionBufferType
	{
		XMFLOAT4 Pos[2048];
	};

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

private:
	struct FontType
	{
		float left;
		float right;
		int size;
	};

	CSRecolorTexture *m_pShader;

	CTexture *m_pTexture;
	FontType *m_pFont;

	XMMATRIX m_BaseViewMatrix;

	ID3D11Device *m_pDevice;
	ID3D11DeviceContext *m_pDeviceContext;

	bool LoadFontData(char *fontfilename);
	bool LoadTexture(ID3D11Device *device,WCHAR *texturefilename);

	void ReleaseFontData(void);
	void ReleaseTexture(void);

	// Hilfsfunktionen für Create-/UpdateSentence
	int GetChar(char c);
public:
	CFont2(void);
	~CFont2(void);

	bool Init(ID3D11Device *device,ID3D11DeviceContext *devicecontext,HWND hWnd,XMMATRIX baseviewmatrix,char *fontfilename,WCHAR *texturefilename);
	void Shutdown(void);


	bool CreateSentence(SSentence *sentence,int posx,int posy,float r,float g,float b,float height,const char* text,int format=11,int sizex=-1,int sizey=-1);
	bool UpdateSentence(SSentence *sentence,int posx,int posy,float r,float g,float b,float height,const char* text,int format=11,int sizex=-1,int sizey=-1);
	void SetSentenceColor(SSentence *sentence,float r,float g,float b);
	bool RenderSentence(SSentence *sentence,XMMATRIX worldmat,XMMATRIX projmat);
	void FreeSentence(SSentence *sentence);
};
