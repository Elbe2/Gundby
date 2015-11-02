// ------------------------------------------------------------------
// Lukas Beinlich, 03.08.2014
// BaseModel.h
// Eine Basisklasse für Objekte mit Vertices
// ------------------------------------------------------------------

#ifndef __BASEMODEL_H__
#define __BASEMODEL_H__

#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>
using namespace std;
using namespace DirectX;

#include "src\Graphics\TextureArray.h"

class CBaseModel
{
protected:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};
	
	struct ModelType
	{
		float x,y,z;
		float tu,tv;
		float nx,ny,nz;
	};

	int m_VertexCount;
	ModelType *m_Model;
	ID3D11Buffer *m_VertexBuffer;

	int m_IndexCount;
	unsigned int *m_Indices;
	ID3D11Buffer *m_IndexBuffer;
	
	CTextureArray *m_Texture;


	bool InitBuffers(ID3D11Device *device);
	bool LoadTexture(ID3D11Device *device,int texcnt,WCHAR *filename1,WCHAR *filename2,WCHAR *filename3,WCHAR *filename4);
	void ShutdownBuffers(void);
	void ReleaseTexture(void);
	void RenderBuffers(ID3D11DeviceContext *devicecontext);

	virtual bool LoadModel(char* modelfile)=0;
	void ReleaseModel(void);
public:
	CBaseModel(void);
	~CBaseModel(void);

	bool Init(ID3D11Device* device,char* modelfile,int texcnt,WCHAR *filename1,WCHAR *filename2=L"",WCHAR *filename3=L"",WCHAR *filename4=L"");
	void Shutdown(void);
	void Render(ID3D11DeviceContext* devicecontext);

	int GetIndexCount(void);
	ID3D11ShaderResourceView** GetTexture(void){return m_Texture->GetTexture();}
	int GetTextureCnt(void){return m_Texture->GetTextureCnt();}
};

#endif //__BASEMODEL_H__
