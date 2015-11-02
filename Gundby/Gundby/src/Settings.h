// ------------------------------------------------------------------
// Settings.h
// Lukas Beinlich, 02.11.2015
// Wichtige Einstellungen sind hier gespeichert
// ------------------------------------------------------------------

#pragma once

class Settings
{
private:
public:
	bool Initialize(void) { return true; }
	bool Destroy(void) {}

	int GetScreenWidth(void) { return 1024; }
	int GetScreenHeight(void) { return 768; }
	bool GetVSync(void) { return true; }
	bool GetFullscreen(void) { return false; }
	float GetScreenNear(void) { return 0.1f; }
	float GetScreenDepth(void) { return 1000.0f; }
};

