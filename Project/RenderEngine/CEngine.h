#pragma once

#include <glad/glad.h>
#include "Define.h"
#include "IEngineInterface.h"
#include "CScene.h"

class ENGINE_API_ACTION CEngine : public IEngineInterface
{
	friend class CEngineManager;
protected:
	CEngine();
public:
	bool Initialize() override;
	void UnInitialize() override;
	void SetLoader(GLADloadproc loader) override;
	void Render() override;
public:
	void Resize(int width, int height);
	void MouseAction(unsigned int key, bool down, int x, int y);
	void KeyboardAction(unsigned int key, const char* name, bool down);
public:
	unsigned int GetRenderTextureId();
private:
	bool InitializeGLAD();
	bool InitializeScene();
private:
	GLADloadproc	m_Loader;
};

