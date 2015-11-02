// ------------------------------------------------------------------
// Lukas Beinlich, 25.07.2014
// Texture.h
// Eine Klasse, die eine Textur läd und verwaltet
// ------------------------------------------------------------------

#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <d3d11.h>

class CTexture
{
private:
	ID3D11ShaderResourceView *m_Texture;
public:
	CTexture(void);
	~CTexture(void);

	bool Init(ID3D11Device *device,WCHAR *filename);
	void Shutdown(void);

	ID3D11ShaderResourceView** GetTexture(void){return &m_Texture;}
};

#endif //__TEXTURE_H__

