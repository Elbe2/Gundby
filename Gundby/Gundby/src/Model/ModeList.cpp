// ------------------------------------------------------------------
// Lukas Beinlich, 03.08.2014
// ModelList.h
// Eine Klasse, die die Modelle/Objekte verwaltet
// ------------------------------------------------------------------

#include "src\Model\ModelList.h"

CModelList::CModelList(void)
{
	m_ModelInfoList=NULL;
}

CModelList::~CModelList(void)
{
}

bool CModelList::Init(int numModels)
{
	m_ModelCount = numModels;
	m_ModelInfoList = new ModelInfoType[m_ModelCount];
	if(!m_ModelInfoList)
		return false;
	srand((unsigned int)time(NULL));
	return true;
}
