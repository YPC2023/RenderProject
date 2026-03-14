#include "pch.h"
#include "Config.h"

#define SCR_WIDTH   800
#define SCR_HEIGHT  600
static const glm::vec3 cameraPos = glm::vec3(5.0f, 5.0f, 5.0f);
static const glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
static const glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
static const float fov = 90.0f;
static const float zNear = 0.1f;
static const float zFar = 100.0f;

Config::Config()
{
	static unsigned int index = 0;
	m_ID = ++index;

	m_nWidth_Viewport = SCR_WIDTH;
	m_nHeight_Viewport = SCR_HEIGHT;

	m_CameraPos = glm::vec3(5.0f, 5.0f, 5.0f);
	m_CameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Config::SetSize(int width, int height)
{
	m_nWidth_Viewport = width;
	m_nHeight_Viewport = height;
}