#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "CUtils.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "CMesh.h"
#include "IModelInterface.h"

class CModel : public IModelInterface
{
public:
	CModel();
	CModel(const std::string& strPth);
public:
	//void Render(const CShader& shader);
public:
	bool SetPath(const std::string& strPath);
	bool IsOK() { return m_bOK; }
	void AppendMesh(CMesh* pMesh);
	CMesh* GetFirstMesh();
	void SetModel(const glm::mat4& model);
public:
	const glm::mat4& GetModel() override;
	size_t GetMeshCount() override;
	IMeshInterface* GetMesh(size_t index) override;
	unsigned int GetID() override;
public:
	static void EnableRenderId(bool bEnable = true);
	static void SelectModel(unsigned int id);
private:
	bool LoadModel(const std::string& path);
	bool ProcessNode(aiNode* node, const aiScene* scene);
	CMesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<CMesh::S_TEXTURE> LoadMaterialTextures(aiMaterial* mat, 
		aiTextureType type, std::string typeName);
	//void SetRenderId(const CShader& shader);
	//void SetSelected(const CShader& shader);
private:
	static bool				m_bRenderId;
	static unsigned int		m_nSelectedId;
private:
	unsigned int			m_nID;
	bool					m_bOK;
	std::string				m_strPath;
	std::vector<CMesh*>		m_vec_Mesh;
	std::string				m_strRootPath;
	glm::mat4				m_Model;
};




#endif // __MODEL_H__