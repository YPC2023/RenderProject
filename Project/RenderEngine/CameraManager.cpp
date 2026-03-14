#include "pch.h"
#include "CameraManager.h"

Camera* CameraManager::CreateCamera(const glm::vec3& pos, const glm::vec3& up, const glm::vec3& target, E_CAMERA_TYPE type)
{
	if (0 < m_map_Camera.count(type)) {
		return m_map_Camera[type];
	}
	Camera* pCamera = new Camera(pos, up, target);
	if (0 == pCamera) {
		PRINTLOG("Fail to new camera");
		return 0;
	}
	if (!pCamera->Initialize()) {
		PRINTLOG("Fail to initialize Camera");
		delete pCamera;
		return 0;
	}
	m_map_Camera[type] = pCamera;
	return pCamera;
}