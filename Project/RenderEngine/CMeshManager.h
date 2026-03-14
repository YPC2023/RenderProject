#ifndef __CMESHMANAGER_H__
#define __CMESHMANAGER_H__

#include <map>
#include "CUtils.h"
#include "CMesh.h"

class CMeshManager
{
public:
	static CMeshManager& GetInstance();

public:
	CMesh* GetMesh(std::vector<CMesh::S_VERTEX> vertices, 
		std::vector<unsigned int> indices, 
		unsigned int type = GL_TRIANGLES);
	CMesh* GetMesh(std::vector<CMesh::S_VERTEX> vertices, 
		std::vector<unsigned int> indices, 
		std::vector<CMesh::S_TEXTURE> textures,
		unsigned int type = GL_TRIANGLES);


private:
	std::map<unsigned int, CMesh*>			m_map_Mesh;
};



#endif // __CMESHMANAGER_H__