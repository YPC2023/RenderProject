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

void Controller::MouseAction(unsigned int key, int action, int x, int y)
{
	glm::vec2 point = TranToViewport(x, y);
	if (0.0f > point.x || 0.0f > point.y || 1.0f < point.x || 1.0f < point.y) {
		return;
	}
	if (0 == key) {
		if (0 == action) {
			OnMouseLeftDown(x, y, point.x, point.y);
		}
		else if (1 == action) {
			OnMouseLeftUp(x, y, point.x, point.y);
		}
		else if (2 == action) {
			OnMouseLeftMove(x, y, point.x, point.y);
		}
	}
}

void Controller::KeyboardAction(unsigned int key, const char* name, bool down)
{
	//PRINTLOG("KeyboardAction(%d,%s,%d)", key, name);
}

void Controller::OnMouseLeftDown(int originX, int originY, float x, float y)
{

}

void Controller::OnMouseLeftUp(int originX, int originY, float x, float y)
{

}

void Controller::OnMouseLeftMove(int originX, int originY, float x, float y)
{

}
