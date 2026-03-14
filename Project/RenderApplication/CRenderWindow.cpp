#include "CRenderWindow.h"
#include "CEngineManager.h"
#include <GLFW/glfw3.h>


CRenderWindow::CRenderWindow(const std::string& strName): CWindow(strName)
{

}

bool CRenderWindow::Initialize()
{
	CEngineManager::GetInstance().GetEngine().SetLoader((GLADloadproc)glfwGetProcAddress);
	if (!CEngineManager::GetInstance().GetEngine().Initialize()) {
		PRINTLOG("Fail to initialize Engine");
		return false;
	}
	return true;
}

void CRenderWindow::UnInitialize()
{
	CEngineManager::GetInstance().GetEngine().UnInitialize();
}

void CRenderWindow::OnRender()
{
	CEngineManager::GetInstance().GetEngine().Render();
	ImTextureID texID = (ImTextureID)CEngineManager::GetInstance().GetEngine().GetRenderTextureId();
	ImVec2 viewportSize = ImGui::GetContentRegionAvail();
	// 注意：OpenGL 坐标系 y 轴向上，ImGui 向下，可能需要翻转 UV
	ImGui::Image(texID, viewportSize, ImVec2(0, 1), ImVec2(1, 0));
}

void CRenderWindow::OnSize(int x, int y)
{
	//PRINTLOG("%s(%d,%d)",m_strName.c_str(), x, y);
	ImVec2 viewportSize = ImGui::GetContentRegionAvail();
	CEngineManager::GetInstance().GetEngine().Resize((int)viewportSize.x, (int)viewportSize.y);
}

void CRenderWindow::OnMouseAction(unsigned int key, bool down, int x, int y)
{
	//PRINTLOG("%s[%d:%s](%d,%d)", m_strName.c_str(), key, down ? "DOWN" : "UP", x, y);
	ImVec2 relativePos = GetRelativePosition(ImGui::GetIO().MousePos);
	CEngineManager::GetInstance().GetEngine().MouseAction(key, down, (int)relativePos.x, (int)relativePos.y);
}

void CRenderWindow::OnKeyboardAction(unsigned int key, const char* name, bool down)
{
	//PRINTLOG("%s[%s(%d):%s]", m_strName.c_str(), name, key, down ? "DOWN" : "UP");
	CEngineManager::GetInstance().GetEngine().KeyboardAction(key, name, down);
}

ImVec2	CRenderWindow::GetRelativePosition(const ImVec2& point)
{
	// 1. 获取当前内容区域的起始屏幕坐标 (左上角)
	// 注意：如果是紧接着 UI 元素后渲染，这个坐标会随着布局下移
	ImVec2 vMin = ImGui::GetCursorScreenPos();
	// 2. 计算相对坐标
	return ImVec2(point.x - vMin.x, point.y - vMin.y);
}