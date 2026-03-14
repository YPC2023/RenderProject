#include "pch.h"
#include "CShaderManager.h"

CShader* CShaderManager::CreateShader(const std::string& vertex, const std::string& fragment, E_SHADER_TYPE type)
{
	if (0 < m_map_Shader.count(type)) {
		return m_map_Shader[type];
	}
	CShader* pShader = new CShader(vertex.c_str(), fragment.c_str());
	if (0 == pShader) {
		PRINTLOG("Fail to new Shader");
		return 0;
	}
	if (!pShader->Initialize()) {
		PRINTLOG("Fail to initialize Shader");
		return 0;
	}
	m_map_Shader[type] = pShader;
	return pShader;
}