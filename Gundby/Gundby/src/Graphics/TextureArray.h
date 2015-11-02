// ------------------------------------------------------------------
// Lukas Beinlich, 04.08.2014
// TextureArray.h
// Eine Klasse, die mehr als eine Textur läd und verwaltet
// ------------------------------------------------------------------

#ifndef __TEXTUREARRAY_H__
#define __TEXTUREARRAY_H__

#include <d3d11.h>

class CTextureArray
{
private:
	ID3D11ShaderResourceView **m_Texture;
	int m_Texturecnt;
public:
	CTextureArray(void);
	~CTextureArray(void);

	bool Init(ID3D11Device *device,int texcnt,WCHAR *filename1,WCHAR *filename2=L"",WCHAR *filename3=L"",WCHAR *filename4=L"");
	void Shutdown(void);

	ID3D11ShaderResourceView** GetTexture(void){return m_Texture;}
	int GetTextureCnt(void){return m_Texturecnt;}
};

#endif //__TEXTUREARRAY_H__

