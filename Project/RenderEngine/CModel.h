#ifndef __MODEL_H__
#define __MODEL_H__

#define GLM_ENABLE_EXPERIMENTAL
#include <vector>
#include "CUtils.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
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
	bool SetPath(const std::string& strPath);
	bool IsOK() { return m_bOK; }
	void AppendMesh(CMesh* pMesh);
	CMesh* GetFirstMesh();
	void SetModel(const glm::mat4& model);
	void SetTranslation(const glm::vec3& translation);
	const glm::vec3& GetTranslation();
	void ResetTranslation();

	void SetRotation(const glm::quat& rotation);
	const glm::quat& GetRotation();
	void ResetRotation();

	void SetScale(const glm::vec3& scale);
	const glm::vec3& GetScale();
	void ResetScale();

	void ActionTransform();

	void SetCoordinate(bool bYes = true) { m_IsCoordinate = bYes; }
	bool IsCoordinate() { return m_IsCoordinate; }
	glm::vec3 GetCoordValue() { return m_AxisValue; }
public:
	glm::mat4 GetModel() override;
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
private:
	glm::vec3				m_Translation;
	glm::quat				m_Rotation;
	glm::vec3				m_Scale;
private:
	bool					m_IsCoordinate;
	glm::vec3				m_AxisValue;
};




#endif // __MODEL_H__