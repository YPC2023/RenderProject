#pragma once

#include <CUtils.h>
#include <glm/glm.hpp>

class Config
{
	friend class ConfigManager;
protected:
	Config();
public:
	void SetSize(int width, int height);
	int GetViewportWidth() { return m_nWidth_Viewport; }
	int GetViewportHeight() { return m_nHeight_Viewport; }
	glm::vec3 GetCameraPos() { return m_CameraPos; }
	glm::vec3 GetCameraTarget() { return m_CameraTarget; }
	glm::vec3 GetCameraUp() { return m_CameraUp; }
private:
	int m_nWidth_Viewport;
	int m_nHeight_Viewport;
private:
	glm::vec3 m_CameraPos;
	glm::vec3 m_CameraTarget;
	glm::vec3 m_CameraUp;
private:
	unsigned int	m_ID;
};

