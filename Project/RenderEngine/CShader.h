#pragma once

#include "CUtils.h"
#include "InitializeInterface.h"
#include "IShaderInterface.h"

class CShader : public InitializeInterface, public IShaderInterface
{
	friend class CShaderManager;
protected:
	CShader(const char* vertex, const char* fragment);

public:
	bool Initialize() override;
	void UnInitialize() override;
public:
	void use() const override;
	void setBool(const std::string& name, bool value) const override;
	void setInt(const std::string& name, int value) const override;
	void setFloat(const std::string& name, float value) const override;
	void setVec2(const std::string& name, const glm::vec2& value) const override;
	void setVec2(const std::string& name, float x, float y) const override;
	void setVec3(const std::string& name, const glm::vec3& value) const override;
	void setVec3(const std::string& name, float x, float y, float z) const override;
	void setVec4(const std::string& name, const glm::vec4& value) const override;
	void setVec4(const std::string& name, float x, float y, float z, float w) const override;
	void setMat2(const std::string& name, const glm::mat2& mat) const override;
	void setMat3(const std::string& name, const glm::mat3& mat) const override;
	void setMat4(const std::string& name, const glm::mat4& mat) const override;
private:
	bool CheckCompileResult(unsigned int id);
	bool CheckLinkResult(unsigned int id);
	bool CompileVertex(const std::string& strCode);
	bool CompileFragment(const std::string& strCode);
	bool LinkProgram();
private:
	std::string		m_strVertex;
	std::string		m_strFragment;
	unsigned int	m_VertexId;
	unsigned int	m_FragmentId;
	unsigned int	m_ID;
};

