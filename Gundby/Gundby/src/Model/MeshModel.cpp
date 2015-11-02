// ------------------------------------------------------------------
// Lukas Beinlich, 03.08.2014
// MeshModel.cpp
// Eine Modelklasse, die ein Model aus einem .mesh-file läd.
// ------------------------------------------------------------------

#include "src\Model\MeshModel.h"
#include <string>
#include <fstream>
using namespace std;

bool CMeshModel::LoadModel(char *filename)
{
	ifstream fin;
	string input;
	
	fin.open(filename);
	if(fin.fail())
		return false;
	
	fin>>input;
	if(input != "vertices:")
		return false;
	fin>>m_VertexCount;
	fin>>input;
	if(input != "indices:")
		return false;
	fin>>m_IndexCount;

	m_Model = new ModelType[m_VertexCount];
	if(!m_Model)
		return false;
	m_Indices = new unsigned int[m_IndexCount];
	if(!m_Indices)
		return false;

	for(int i=0;i<m_VertexCount;i++)
	{
		fin>>m_Model[i].x>>m_Model[i].y>>m_Model[i].z;
		fin>>m_Model[i].nx>>m_Model[i].ny>>m_Model[i].nz;
		fin>>m_Model[i].tu>>m_Model[i].tv;
	}

	for(int i=0;i<m_IndexCount;i++)
	{
		fin>>m_Indices[i];
	}
	fin.close();
	return true;
}
