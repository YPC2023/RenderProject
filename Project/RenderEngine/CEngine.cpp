#include "pch.h"
#include "CEngine.h"
#include "CUtils.h"
#include "CSceneManager.h"

CEngine::CEngine()
{
	m_Loader = 0;
}

bool CEngine::Initialize()
{
	if (!InitializeGLAD()) {
		PRINTLOG("Fail to initialize GLAD");
		return false;
	}
	if (!InitializeScene()) {
		PRINTLOG("Fail to initialize Scene");
		return false;
	}
	return true;
}

void CEngine::UnInitialize()
{
	CSceneManager::GetInstance().GetMainScene().UnInitialize();
}

void CEngine::SetLoader(GLADloadproc loader)
{
	m_Loader = loader;
}

void CEngine::Render()
{
	CSceneManager::GetInstance().GetMainScene().Render();
}

void CEngine::Resize(int width, int height)
{
	CSceneManager::GetInstance().GetMainScene().Resize(width, height);
}

void CEngine::MouseAction(unsigned int key, bool down, int x, int y)
{
	CSceneManager::GetInstance().GetMainScene().MouseAction(key, down, x, y);
}

void CEngine::KeyboardAction(unsigned int key, const char* name, bool down)
{
	CSceneManager::GetInstance().GetMainScene().KeyboardAction(key, name, down);
}

unsigned int CEngine::GetRenderTextureId()
{
	return CSceneManager::GetInstance().GetMainScene().GetRenderTextureId();
}

bool CEngine::InitializeGLAD()
{
	if (0 == m_Loader) {
		PRINTLOG("Set glfwGetProcAddress first please");
		return false;
	}
	if (!gladLoadGLLoader((GLADloadproc)m_Loader))
	{
		PRINTLOG("Failed to initialize GLAD");
		return false;
	}
	return true;
}

bool CEngine::InitializeScene()
{
	if (!CSceneManager::GetInstance().GetMainScene().Initialize()) {
		PRINTLOG("Fail to initialize Scene");
		return false;
	}
	return true;
}