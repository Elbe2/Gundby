// ------------------------------------------------------------------
// Input.h
// Lukas Beinlich, 04.10.2015
// Hier wird der Input bearbeitet
// ------------------------------------------------------------------
#pragma once

class Input
{
private:
	bool m_Keys[256];
public:
	Input();
	~Input();

	void Initialize(void);
	
	void KeyDown(unsigned int key);
	void KeyUp(unsigned int key);
	
	bool IsKeyDown(unsigned int key);
};
