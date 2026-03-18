#ifndef __COBJECTMANAGER_H__
#define __COBJECTMANAGER_H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "CUtils.h"
#include "CModel.h"

class CObjectManager
{
public:
	static CObjectManager& GetInstance();
private:
	void GenerateTorusVertex(const glm::vec3& center, const glm::vec3& axisDir, const glm::vec4& color,
		float majorRadius, float minorRadius, int mainSectors, int tubeSectors,
		std::vector<CMesh::S_VERTEX>& vertices, std::vector<unsigned int>& indices);
	void GenerateConeVertex(const glm::vec3& center, const glm::vec3& axisDir,
		float height, float radius, int sectors, const glm::vec4& color,
		std::vector<CMesh::S_VERTEX>& vertices, std::vector<unsigned int>& indices);
	void GenerateSphereVertex(const glm::vec3& center, float radius, int sectors, int stacks,
		const glm::vec4& color, std::vector<CMesh::S_VERTEX>& vertices, std::vector<unsigned int>& indices);
	void GenerateColumnVertex(glm::vec3 start, glm::vec3 end,
		std::vector<CMesh::S_VERTEX>& vertices, std::vector<unsigned int>& indices, float radius = 1.0f, int sectors = 32,
		const glm::vec4& sColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), const glm::vec4& eColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
public:
	CModel* GetModelFromFile(const std::string& strPath);
	CModel* GetCoordModel(float size = 1.0f);
	CModel* GetChessModel(float size = 10.0f);
	CModel* GetScreenModel();
	CModel* GetColumnModel(const glm::vec3& start, const glm::vec3& end, float radius = 1.0f, int sectors = 32);
	CModel* GetSphereModel(const glm::vec3& center, float radius = 1.0f, const glm::vec4& color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), int sectors = 320, int stacks = 640);
	CModel* GetConeModel(const glm::vec3& center, const glm::vec3& axisDir, float height = 1.0f, float radius = 1.0f,
		const glm::vec4& color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), int sectors = 32);
	CModel* GetTorusModel(const glm::vec3& center, const glm::vec3& axisDir, float radius, float width,
		const glm::vec4& color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), int mainSectors = 64, int tubeSectors = 32);
};



#endif // __COBJECTMANAGER_H__