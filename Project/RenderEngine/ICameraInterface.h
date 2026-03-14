#pragma once

class ICameraInterface
{
public:
	virtual const glm::mat4& GetView() = 0;
	virtual const glm::mat4& GetProjection() = 0;
};