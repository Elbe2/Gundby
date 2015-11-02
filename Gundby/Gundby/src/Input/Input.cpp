// ------------------------------------------------------------------
// Lukas Beinlich, 22.07.2014
// Input.cpp
// Eine einfache Klasse, die die Tasten, die gedrückt sind verwaltet und speichert
// ------------------------------------------------------------------

#include "src\Input\Input.h"

CInput::CInput(void)
{
	m_DirectInput=NULL;
	m_Keyboard=NULL;
	m_Mouse=NULL;
}

CInput::~CInput(void)
{
}

bool CInput::Init(HINSTANCE hInstance,HWND hWnd,int screenwidth,int screenheight)
{
	m_ScreenHeight=screenheight;
	m_ScreenWidth=screenwidth;
	m_MouseX=0;
	m_MouseY=0;

	if(FAILED(DirectInput8Create(hInstance,DIRECTINPUT_VERSION,IID_IDirectInput8,(void**)&m_DirectInput,NULL)))
		return false;

	if(FAILED(m_DirectInput->CreateDevice(GUID_SysKeyboard,&m_Keyboard,NULL)))
		return false;
	if(FAILED(m_Keyboard->SetDataFormat(&c_dfDIKeyboard)))
		return false;
	if(FAILED(m_Keyboard->SetCooperativeLevel(hWnd,DISCL_FOREGROUND|DISCL_EXCLUSIVE)))
		return false;
	if(FAILED(m_Keyboard->Acquire()))
		return false;

	if(FAILED(m_DirectInput->CreateDevice(GUID_SysMouse,&m_Mouse,NULL)))
		return false;
	if(FAILED(m_Mouse->SetDataFormat(&c_dfDIMouse)))
		return false;
	if(FAILED(m_Mouse->SetCooperativeLevel(hWnd,DISCL_FOREGROUND|DISCL_NONEXCLUSIVE)))
		return false;
	if(FAILED(m_Mouse->Acquire()))
		return false;
	return true;
}

void CInput::Shutdown(void)
{
	if(m_Mouse)
	{
		m_Mouse->Unacquire();
		m_Mouse->Release();
		m_Mouse=NULL;
	}
	if(m_Keyboard)
	{
		m_Keyboard->Unacquire();
		m_Keyboard->Release();
		m_Keyboard=NULL;
	}
	if(m_DirectInput)
	{
		m_DirectInput->Release();
		m_DirectInput=NULL;
	}
}

bool CInput::Frame()
{
	if(!ReadKeyboard())
		return false;
	if(!ReadMouse())
		return false;
	ProcessInput();
	return true;
}

bool CInput::ReadKeyboard(void)
{
	HRESULT result;
	if(FAILED(result=m_Keyboard->GetDeviceState(sizeof(m_KeyboardState),(LPVOID)&m_KeyboardState)))
	{
		if((result==DIERR_INPUTLOST)|(result==DIERR_NOTACQUIRED))
			m_Keyboard->Acquire();
		else
			return false;
	}
	return true;
}

bool CInput::ReadMouse(void)
{
	HRESULT result;
	if(FAILED(result=m_Mouse->GetDeviceState(sizeof(DIMOUSESTATE),(LPVOID)&m_MouseState)))
	{
		if((result==DIERR_INPUTLOST)|(result==DIERR_NOTACQUIRED))
			m_Mouse->Acquire();
		else
			return false;
	}
	return true;
}

void CInput::ProcessInput(void)
{
	m_MouseX+=m_MouseState.lX;
	m_MouseY=m_MouseState.lY;
	if(m_MouseX<0)m_MouseX=0;
	if(m_MouseY<0)m_MouseY=0;
	if(m_MouseX>m_ScreenWidth)m_MouseX=m_ScreenWidth;
	if(m_MouseY>m_ScreenHeight)m_MouseY=m_ScreenHeight;
}

bool CInput::IsEscapePressed(void)
{
	if(m_KeyboardState[DIK_ESCAPE] & 0x80)
		return true;
	return false;
}

bool CInput::IsKeyPressed(int key)
{
	if(m_KeyboardState[key] & 0x80)
		return true;
	return false;
}

void CInput::GetMouseCoord(int &mouseX,int &mouseY)
{
	mouseX=m_MouseX;
	mouseY=m_MouseY;
}
