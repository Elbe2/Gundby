// ------------------------------------------------------------------
// Main.cpp
// Lukas Beinlich, 04.10.2015
// Der Einstiegspunkt
// ------------------------------------------------------------------

#define _WINDOWS_LEAN_AND_MEAN_

#include <Windows.h>


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	MessageBox(NULL, L"Hello Wolrd!", L"It works!", 1);
	return 0;
}

