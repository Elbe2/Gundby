// ------------------------------------------------------------------
// FontPanel.h
// Lukas Beinlich, 02.11.2015
// Ein einfaches Rechteck, indem Text angezeigt wird
// ------------------------------------------------------------------

#pragma once

#include "src\Manager\Objektmanager.h"
#include "SpriteFont.h"
using namespace DirectX;

class CFontPanel : public CObjekt
{
protected:
	WCHAR *m_Text;
	int m_Size;
	XMFLOAT4 m_Color;
	SpriteBatch *m_pSpriteBatch;
	SpriteFont *m_pSpriteFont;
public:
	bool Init(SpriteBatch *batch, SpriteFont *font);
	bool Render(void) { return true; }
	bool RenderFont(void);
	void Destroy(void) { m_pSpriteBatch = NULL;m_pSpriteFont = NULL; }

	void SetPosition(int x, int y);
	void SetText(WCHAR *text);
	void SetColor(XMFLOAT4 *col);
	void SetSize(int pixel);
};
