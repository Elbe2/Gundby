// ------------------------------------------------------------------
// Lukas Beinlich, 03.08.2014
// ModelList.h
// Eine Klasse, die die Modelle/Objekte verwaltet
// ------------------------------------------------------------------
#pragma once


#include <DirectXMath.h>
#include <stdlib.h>
#include <time.h>
using namespace DirectX;

class CModelList
{
private:
	struct ModelInfoType
	{
		XMFLOAT4 color;
		float x,y,z;
	};

	int m_ModelCount;
	ModelInfoType *m_ModelInfoList;
public:
	CModelList(void);
	~CModelList(void);

	bool Init(int numModels);
	void Shutdown(void);

	int GetModelCount(void);
	void GetData(int,float&,float&,float&,XMFLOAT4&);
};
