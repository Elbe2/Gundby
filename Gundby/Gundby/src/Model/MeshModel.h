// ------------------------------------------------------------------
// Lukas Beinlich, 03.08.2014
// MeshModel.h
// Eine Modelklasse, die ein Model aus einem .mesh-file läd.
// ------------------------------------------------------------------

#ifndef __MESHMODEL_H__
#define __MESHMODEL_H__

#include "src\Model\BaseModel.h"

class CMeshModel:public CBaseModel
{
private:
	virtual bool LoadModel(char* modelfile);
};

#endif //__MESHMODEL_H__
