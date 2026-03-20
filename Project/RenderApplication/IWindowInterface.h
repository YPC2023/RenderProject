#pragma once

#include "InitializeInterface.h"

class IWindowInterface : public InitializeInterface
{
public:
	typedef enum _E_MOUSE_ACTION
	{
		E_MOUSE_DOWN,
		E_MOUSE_UP,
		E_MOUSE_MOVE,
		E_MOUSE_WHEEL
	}E_MOUSE_ACTION;
public:
	virtual void OnRender() = 0;
	virtual void OnSize(int x, int y) = 0;
	virtual void OnMouseAction(unsigned int key, E_MOUSE_ACTION action, int x, int y) = 0;
	virtual void OnKeyboardAction(unsigned int key, const char* name, bool down) = 0;
};

