#include "CObjectManager.h"
#include "CModelManager.h"
#include "CMeshManager.h"
#include "CTextureManager.h"

CObjectManager& CObjectManager::GetInstance()
{
	static CObjectManager Instance;
	return Instance;
}

CModel* CObjectManager::GetModelFromFile(const std::string& strPath)
{
	return CModelManager::GetInstance().GetModelFromFile(strPath);
}

CModel* CObjectManager::GetCoordModel(float size)
{
	CModel* pModel = CModelManager::GetInstance().GetModel();
	if (pModel) {
		std::vector<CMesh::S_VERTEX>	vertices;
		std::vector<unsigned int>	indices = { 0, 1 };

		CMesh::S_VERTEX	O; 
		O.Position = glm::vec3(0.0f, 0.0f, 0.0f);

		CMesh::S_VERTEX	X;
		X.Position = glm::vec3(size, 0.0f, 0.0f);
		X.Color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		O.Color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		vertices.push_back(O);
		vertices.push_back(X);
		CMesh* pMeshX = CMeshManager::GetInstance().GetMesh(vertices, indices, GL_LINES);

		CMesh::S_VERTEX	Y;
		Y.Position = glm::vec3(0.0f, size, 0.0f);
		Y.Color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		O.Color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		vertices.clear();
		vertices.push_back(O);
		vertices.push_back(Y);
		CMesh* pMeshY = CMeshManager::GetInstance().GetMesh(vertices, indices, GL_LINES);

		CMesh::S_VERTEX	Z;
		Z.Position = glm::vec3(0.0f, 0.0f, size);
		Z.Color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		O.Color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		vertices.clear();
		vertices.push_back(O);
		vertices.push_back(Z);
		CMesh* pMeshZ = CMeshManager::GetInstance().GetMesh(vertices, indices, GL_LINES);

		pModel->AppendMesh(pMeshX);
		pModel->AppendMesh(pMeshY);
		pModel->AppendMesh(pMeshZ);

		pModel->SetCoordinate();
	}
	return pModel;
}

CModel* CObjectManager::GetChessModel(float size)
{
	CModel* pModel = CModelManager::GetInstance().GetModel();
	if (pModel) {
		std::vector<CMesh::S_VERTEX>	vertices;
		std::vector<CMesh::S_TEXTURE>	textures;
		std::vector<unsigned int>	indices = { 
			0, 1, 2,  
			2, 3, 0
		};

		CMesh::S_TEXTURE texture;
		texture.strPath = CUtils::GetImagePathFile("chess.jpg");
		texture.strType = "texture";
		texture.pTexture = CTextureManager::GetInstance().GetTexture(texture.strPath);
		if (0 == texture.pTexture) {
			PRINTLOG("Fail to create chess texture");
		}
		textures.push_back(texture);

		CMesh::S_VERTEX	LeftTop, RightTop, RightBottom, LeftBottom;
		LeftTop.Position = glm::vec3(-1.0f, 0.0f, -1.0f);
		LeftTop.TexCoords = glm::vec2(0.0f, size);

		RightTop.Position = glm::vec3(1.0f, 0.0f, -1.0f);
		RightTop.TexCoords = glm::vec2(size, size);

		RightBottom.Position = glm::vec3(1.0f, 0.0f, 1.0f);
		RightBottom.TexCoords = glm::vec2(size, 0.0f);

		LeftBottom.Position = glm::vec3(-1.0f, 0.0f, 1.0f);
		LeftBottom.TexCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(LeftTop);
		vertices.push_back(RightTop);
		vertices.push_back(RightBottom);
		vertices.push_back(LeftBottom);

		CMesh* pMesh = CMeshManager::GetInstance().GetMesh(vertices, indices, textures);

		pModel->AppendMesh(pMesh);
	}
	return pModel;
}

CModel* CObjectManager::GetScreenModel()
{
	CModel* pModel = CModelManager::GetInstance().GetModel();
	if (pModel) {
		std::vector<CMesh::S_VERTEX>	vertices;
		std::vector<CMesh::S_TEXTURE>	textures;
		std::vector<unsigned int>	indices = {
			0, 1, 2,
			2, 3, 0
		};
		CMesh::S_VERTEX	LeftTop, RightTop, RightBottom, LeftBottom;
		LeftTop.Position = glm::vec3(-1.0f, 1.0f, 0.0f);
		LeftTop.TexCoords = glm::vec2(0.0f, 1.0f);

		RightTop.Position = glm::vec3(1.0f, 1.0f, 0.0f);
		RightTop.TexCoords = glm::vec2(1.0f, 1.0f);

		RightBottom.Position = glm::vec3(1.0f, -1.0f, 0.0f);
		RightBottom.TexCoords = glm::vec2(1.0f, 0.0f);

		LeftBottom.Position = glm::vec3(-1.0f, -1.0f, 0.0f);
		LeftBottom.TexCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(LeftTop);
		vertices.push_back(RightTop);
		vertices.push_back(RightBottom);
		vertices.push_back(LeftBottom);
		CMesh* pMesh = CMeshManager::GetInstance().GetMesh(vertices, indices, textures);

		pModel->AppendMesh(pMesh);
	}
	return pModel;
}