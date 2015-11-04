// ------------------------------------------------------------------
// Objektmanager.h
// Lukas Beinlich, 02.11.2015
// Eine Liste aller Objekte
// ------------------------------------------------------------------

#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
using namespace std;
using namespace DirectX;

class CObjekt
{
protected:
	bool m_Visible;
	bool m_Updated;
	XMFLOAT2 m_Position;
	ID3D11Device *m_pDevice;
	ID3D11DeviceContext *m_pDeviceContext;
public:
	CObjekt(void) { m_Visible = false; }
	~CObjekt(void) {}
	void SetNotVisible(void) { m_Visible = false; }
	void SetVisible(bool vis = true) { m_Visible = vis; }
	bool Visible(void) { return m_Visible; }
	void ToggleVisible(void) { m_Visible = !m_Visible; }

//	virtual bool Init(void) = 0; // Es gibt keine allgemeinen Init für verschiedene Objekte
	virtual bool Render(void) = 0;
	virtual bool RenderFont(void) { return true; }
	virtual bool Upadte(void) { return true; } // Eigentlich gibt es auch kein allgemeines Update, es sei denn alles wird durch get und set methoden gemacht
	virtual void Destroy(void) {}
};

class CObjektManger
{
private:
	vector<CObjekt*> m_Elements;
public:
	bool Render(void);
	bool RenderFont(void);
//	bool Update(void);
	void Destroy(void) { m_Elements.clear(); } // Maybe i should delete the contents as well
	void AddElement(CObjekt* pelem) { m_Elements.push_back(pelem); }
	void RemoveElement(int nr) {  } // You dont remove Elements, thats it
};
