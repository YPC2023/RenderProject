#pragma once

#include "InitializeInterface.h"

class IWindowInterface : public InitializeInterface
{
public:
	virtual void OnRender() = 0;
	virtual void OnSize(int x, int y) = 0;
	virtual void OnMouseAction(unsigned int key, bool down, int x, int y) = 0;
	virtual void OnKeyboardAction(unsigned int key, const char* name, bool down) = 0;
};

