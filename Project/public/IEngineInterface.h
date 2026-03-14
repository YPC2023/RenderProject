#pragma once

#include <glad/glad.h>
#include "InitializeInterface.h"

class IEngineInterface : public InitializeInterface
{
public:
	virtual void Render() = 0;
	virtual void SetLoader(GLADloadproc loader) = 0;
};