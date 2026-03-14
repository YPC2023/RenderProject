#pragma once

#include "Camera.h"
#include <ISingletonInterface.h>
#include <map>

class CameraManager : public ISingletonInterface<CameraManager>
{
public:
	typedef enum _E_CAMERA_TYPE
	{
		E_CAMERA_NORMAL,
	}E_CAMERA_TYPE;
public:
	Camera* CreateCamera(const glm::vec3& pos, const glm::vec3& up, const glm::vec3& target, E_CAMERA_TYPE type = E_CAMERA_NORMAL);


private:
	std::map< E_CAMERA_TYPE, Camera*>	m_map_Camera;
};

