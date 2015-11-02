// ------------------------------------------------------------------
// Lukas Beinlich, 28.07.2014
// SBasic.h
// Eine Basisklasse für Shader (Dummyklassse)
// ------------------------------------------------------------------
#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <fstream>
using namespace std;
using namespace DirectX;

class CSBasic
{
protected:
	void OutputShaderErrorMessage(ID3D10Blob * errorMessage, HWND hwnd, WCHAR * shaderFilename);
public:
	virtual bool Init(ID3D11Device *device,HWND hWnd)=0;
	virtual void Shutdown(void)=0;
};

