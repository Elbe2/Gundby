// ------------------------------------------------------------------
// Lukas Beinlich, 22.07.2014
// Grafik.h
// Die Hauptklasse, die die Grafik verwaltet (DirectX 11 initialisierung usw...)
// ------------------------------------------------------------------

#ifndef __GRAFIK_H__
#define __GRAFIK_H__

#include "src\Graphics\d3dClass.h"
#include "Engine\Input\Input.h"
#include "src\Graphics\Camera.h"
#include "src\Graphics\Model\BaseModel.h"
#include "src\Graphics\Model\MeshModel.h"
#include "src\Graphics\Light.h"
#include "src\Graphics\Bitmap.h"	
#include "src\Graphics\Shader\SContainer.h"
#include "src\Graphics\FrustrumCulling.h"
#include "src\Graphics\DebugWindow.h"
#include "src\Graphics\RenderTexture.h"
#include "src\Graphics\Font\Font2.h"

#include "src\Graphics\OrthoWindow.h"
#include "src\Graphics\RenderTexture.h"

#include "Game\GUI\GUIs\GuiMain.h"

#include "Game\Options.h"
/*const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;*/

class CGrafik
{
private:
	CD3D *m_D3D;
	CInput *m_Input;

	CCamera *m_Camera;
	CSContainer *m_ShaderContainer;
	CLight *m_Light;
	CFrustrum *m_Frustrum;

	CFont2 *m_Font2;

	SSentence *m_SFPS;
	SSentence *m_SCpu;

	CGuiMain *m_gui;
	HWND m_hWnd;

	bool Render(void);
	bool Update(void);
	bool UpdateFps(int fps,int cpu);
public:
	CGrafik(void);
	~CGrafik(void);

	bool Init(int screenwidth,int screenheight,HWND hWnd,CInput *input);
	void Shutdown(void);
	bool Frame(int fps, int cpu, float frameTime);

	bool Resize(int screenwidth,int screenheight);
};

#endif //__GRAFIK_H__
