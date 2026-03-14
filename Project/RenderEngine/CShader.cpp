#include "pch.h"
#include "CShader.h"
#include <glad/glad.h>

CShader::CShader(const char* vertex, const char* fragment)
{
	m_strVertex = vertex;
	m_strFragment = fragment;
	m_VertexId = -1;
	m_FragmentId = -1;
}

bool CShader::Initialize()
{
	std::string strContentVertex = m_strVertex;
	if (CUtils::FileExists(m_strVertex.c_str())) {
		strContentVertex = CUtils::RendFile(m_strVertex.c_str());
	}
	std::string strContentFragment = m_strFragment;
	if (CUtils::FileExists(m_strFragment.c_str())) {
		strContentFragment = CUtils::RendFile(m_strFragment.c_str());
	}
	if (!CompileVertex(strContentVertex)) {
		PRINTLOG("Fail to compile vertex");
		return false;
	}
	if (!CompileFragment(strContentFragment)) {
		PRINTLOG("Fail to compile fragment");
		return false;
	}
	if (!LinkProgram()) {
		PRINTLOG("Fail to link program");
		return false;
	}
	return true;
}

void CShader::UnInitialize()
{
	if (-1 != m_VertexId) {
		glDeleteShader(m_VertexId);
		m_VertexId = -1;
	}
	if (-1 != m_FragmentId) {
		glDeleteShader(m_FragmentId);
		m_FragmentId = -1;
	}
}

void CShader::use() const
{
	glUseProgram(m_ID);
}

void CShader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value);
}

void CShader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void CShader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}

void CShader::setVec2(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
}

void CShader::setVec2(const std::string& name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(m_ID, name.c_str()), x, y);
}

void CShader::setVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
}

void CShader::setVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(m_ID, name.c_str()), x, y, z);
}

void CShader::setVec4(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
}

void CShader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(m_ID, name.c_str()), x, y, z, w);
}

void CShader::setMat2(const std::string& name, const glm::mat2& mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void CShader::setMat3(const std::string& name, const glm::mat3& mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void CShader::setMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

bool CShader::CheckCompileResult(unsigned int id)
{
	GLint success;
	GLchar infoLog[1024] = { 0 };
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (success) {
		return true;
	}
	glGetShaderInfoLog(id, sizeof(infoLog), NULL, infoLog);
	PRINTLOG("ERROR: Fail to compile [%s]:\n%s", m_VertexId == id ? m_strVertex.c_str() : m_strFragment.c_str(), infoLog);
	return true;
}

bool CShader::CheckLinkResult(unsigned int id)
{
	GLint success;
	GLchar infoLog[1024] = { 0 };
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (success) {
		return true;
	}
	glGetProgramInfoLog(id, sizeof(infoLog), NULL, infoLog);
	PRINTLOG("ERROR: Fail to link Shader\n%s", infoLog);
	return true;
}

bool CShader::CompileVertex(const std::string& strCode)
{
	const char* buf = strCode.c_str();
	// vertex shader
	m_VertexId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(m_VertexId, 1, &buf, NULL);
	glCompileShader(m_VertexId);
	return CheckCompileResult(m_VertexId);
}

bool CShader::CompileFragment(const std::string& strCode)
{
	const char* buf = strCode.c_str();
	// vertex shader
	m_FragmentId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_FragmentId, 1, &buf, NULL);
	glCompileShader(m_FragmentId);
	return CheckCompileResult(m_FragmentId);
}

bool CShader::LinkProgram()
{
	m_ID = glCreateProgram();
	glAttachShader(m_ID, m_VertexId);
	glAttachShader(m_ID, m_FragmentId);
	glLinkProgram(m_ID);
	return CheckLinkResult(m_ID);
}