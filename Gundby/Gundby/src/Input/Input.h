// ------------------------------------------------------------------
// Lukas Beinlich, 22.07.2014
// Input.h
// Eine einfache Klasse, die die Tasten, die gedrückt sind verwaltet und speichert
// ------------------------------------------------------------------

#ifndef __INPUT_H__
#define __INPUT_H__

#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#include <dinput.h>

class CInput
{
private:
	IDirectInput8 *m_DirectInput;
	IDirectInputDevice8 *m_Keyboard;
	IDirectInputDevice8 *m_Mouse;

	unsigned char m_KeyboardState[256];
	DIMOUSESTATE m_MouseState;

	int m_ScreenWidth;
	int m_ScreenHeight;

	int m_MouseX;
	int m_MouseY;

	bool ReadKeyboard(void);
	bool ReadMouse(void);
	void ProcessInput(void);
public:
	CInput(void);
	~CInput(void);

	bool Init(HINSTANCE hInstance,HWND hWnd,int screenwidth,int screenheight);
	void Shutdown(void);
	bool Frame(void);

	void GetMouseCoord(int &x,int &y);
	bool IsEscapePressed(void);
	bool IsKeyPressed(int key);
};

#endif //__INPUT_H__
