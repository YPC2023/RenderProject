#pragma once

#include "ISingletonInterface.h"
#include "CShader.h"
#include <map>

class CShaderManager : public ISingletonInterface<CShaderManager>
{
public:
	typedef enum _E_SHADER_TYPE
	{
		E_SHADER_NORMAL,
		E_SHADER_SELECT,
	}E_SHADER_TYPE;
public:
	CShader* CreateShader(const std::string& vertex, const std::string& fragment, E_SHADER_TYPE type = E_SHADER_NORMAL);

private:
	std::map<E_SHADER_TYPE, CShader*>	m_map_Shader;
};

