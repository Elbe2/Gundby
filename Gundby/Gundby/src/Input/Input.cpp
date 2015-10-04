// ------------------------------------------------------------------
// Input.cpp
// Lukas Beinlich, 04.10.2015
// Hier wird der Input bearbeitet
// ------------------------------------------------------------------

#include "src\Input\Input.h"

Input::Input()
{
}


Input::~Input()
{
}

void Input::Initialize(void)
{
	for (int i = 0; i < 256; i++)
	{
		m_Keys[i] = 0;
	}
}

void Input::KeyDown(unsigned int key)
{
	m_Keys[key] = true;
}

void Input::KeyUp(unsigned int key)
{
	m_Keys[key] = false;
}

bool Input::IsKeyDown(unsigned int key)
{
	return m_Keys[key];
}
