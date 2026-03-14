#pragma once

#include <CUtils.h>
#include <glm/glm.hpp>
#include "ConfigManager.h"

class Controller
{
public:
	Controller();
protected:
	virtual Config* GetConfig();
private:
	glm::vec2 TranToViewport(int x, int y);
public:
	void MouseAction(unsigned int key, bool down, int x, int y);
	void KeyboardAction(unsigned int key, const char* name, bool down);
protected:
	virtual void OnSelectAction(int originX, int originY, float x, float y);
};

