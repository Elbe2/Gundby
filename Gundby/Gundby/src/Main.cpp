// ------------------------------------------------------------------
// Main.cpp
// Lukas Beinlich, 02.11.2015
// Der Einstiegspunkt
// ------------------------------------------------------------------

#include "src\System.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	System* pSystem;
	bool result;

	pSystem = new System;
	if (!pSystem)
	{
		return 0;
	}

	result = pSystem->Initialize();
	if (result)
	{
		pSystem->Run();
	}

	pSystem->Destroy();
	delete pSystem;
	pSystem = 0;

	return 0;
}

