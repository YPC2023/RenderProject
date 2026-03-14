#include "pch.h"
#include "Controller.h"


Controller::Controller()
{

}

Config* Controller::GetConfig()
{
	return 0;
}

glm::vec2 Controller::TranToViewport(int x, int y)
{
	Config* pConfig = GetConfig();
	ASSERT(0 != pConfig);
	
	// yÈ¡·´
	y = pConfig->GetViewportHeight() - y;

	return glm::vec2((float)x / pConfig->GetViewportWidth(), (float)y / pConfig->GetViewportHeight());
}

void Controller::MouseAction(unsigned int key, bool down, int x, int y)
{
	if (down) {
		return;
	}
	glm::vec2 point = TranToViewport(x, y);
	if (0.0f > point.x || 0.0f > point.y || 1.0f < point.x || 1.0f < point.y) {
		return;
	}
	OnSelectAction(x, y, point.x, point.y);
	//PRINTLOG("MouseAction(%f,%f)", point.x, point.y);
}

void Controller::KeyboardAction(unsigned int key, const char* name, bool down)
{
	//PRINTLOG("KeyboardAction(%d,%s,%d)", key, name);
}

void Controller::OnSelectAction(int originX, int originY, float x, float y)
{

}