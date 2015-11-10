// ------------------------------------------------------------------
// FontPanel.h
// Lukas Beinlich, 02.11.2015
// Ein einfaches Rechteck, indem Text angezeigt wird
// ------------------------------------------------------------------

#include "src\GUI\FontPanel.h"
#include <string>
using namespace std;

bool CFontPanel::Init(SpriteBatch * batch, SpriteFont * font)
{
	m_pSpriteBatch = batch;
	m_pSpriteFont = font;
	m_Updated = true;
	m_Position = XMFLOAT2(100, 100);
	m_Color = XMFLOAT4(1, 0, 1, 1);
	m_Text = L"Default-Text!";
	int size = 15;
	return true;
}

bool CFontPanel::RenderFont(void)
{
	XMVECTOR pos = XMLoadFloat2(&m_Position);
	XMVECTOR col = XMLoadFloat4(&m_Color);
	m_pSpriteFont->DrawString(m_pSpriteBatch, m_Text.c_str(), pos, col, 0, g_XMZero, m_Size/100.0f);
	return true;
}

void CFontPanel::SetPosition(int x, int y)
{
	m_Position.x = x;
	m_Position.y = y;
}

void CFontPanel::SetText(wstring text)
{
	m_Text = text;
}

void CFontPanel::SetColor(XMFLOAT4 * col)
{
	m_Color = *col;
}

void CFontPanel::SetSize(int pixel)
{
	m_Size = pixel;
}

