#include "CMeshManager.h"

CMeshManager& CMeshManager::GetInstance()
{
	static CMeshManager Instance;
	return Instance;
}

CMesh* CMeshManager::GetMesh(std::vector<CMesh::S_VERTEX> vertices, 
	std::vector<unsigned int> indices, 
	unsigned int type)
{
	CMesh* pMesh = new CMesh(vertices, indices, type);
	if (pMesh) {
		m_map_Mesh[pMesh->GetID()] = pMesh;
	}
	return pMesh;
}

CMesh* CMeshManager::GetMesh(std::vector<CMesh::S_VERTEX> vertices, 
	std::vector<unsigned int> indices, 
	std::vector<CMesh::S_TEXTURE> textures,
	unsigned int type)
{
	CMesh* pMesh = new CMesh(vertices, indices, textures, type);
	if (pMesh) {
		m_map_Mesh[pMesh->GetID()] = pMesh;
	}
	return pMesh;
}