#pragma once

#include <imgui.h>
#include "CWindow.h"

class CRenderWindow : public CWindow
{
	friend class CWindowManager;
protected:
	CRenderWindow(const std::string& strName);
public:
	bool Initialize() override;
	void UnInitialize() override;
public:
	void OnRender() override;
	void OnSize(int x, int y) override;
	void OnMouseAction(unsigned int key, E_MOUSE_ACTION action, int x, int y) override;
	void OnKeyboardAction(unsigned int key, const char* name, bool down) override;
private:
	ImVec2	GetRelativePosition(const ImVec2& point);
};

