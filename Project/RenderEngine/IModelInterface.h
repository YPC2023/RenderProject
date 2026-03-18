#pragma once

#include <glm/glm.hpp>
#include "IMeshInterface.h"

class IModelInterface
{
public:
	virtual glm::mat4 GetModel() = 0;
	virtual size_t GetMeshCount() = 0;
	virtual IMeshInterface* GetMesh(size_t index) = 0;
	virtual unsigned int GetID() = 0;
};