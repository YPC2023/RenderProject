#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "InitializeInterface.h"
#include "CShaderManager.h"
#include "CObjectManager.h"
#include "CFrameBufferManager.h"
#include "ConfigManager.h"
#include "CameraManager.h"
#include "CRenderManager.h"
#include "Controller.h"

class CScene : public InitializeInterface,
	public Controller
{
	friend class CSceneManager;
protected:
	CScene();
public:
	bool Initialize() override;
	void UnInitialize() override;
private:
	unsigned int GetSelectId(int x, int y);
protected:
	void OnSelectAction(int originX, int originY, float x, float y) override;
	Config* GetConfig() override;
private:
	void OnRender(CRender* pRender);
public:
	void RenderNormal();
	void RenderSelect();
	void Render();
private:
	void ResizeConfig(int width, int height);
	void ResizeCamera(int width, int height);
	void ResizeNormalFramebuffer(int width, int height);
	void ResizeSelectFramebuffer(int width, int height);
public:
	void Resize(int width, int height);
public:
	unsigned int GetRenderTextureId();
private:
	bool InitializeShader();
	bool InitializeModel();
	bool InitializeFramebuffer();
	bool InitializeConfig();
	bool InitializeCamera();
	bool InitializeRender();

private:
	CShader* m_pNormalShader;
	CShader* m_pSelectShader;

	CModel* m_pModelFile;
	CModel* m_pModelCoord;
	CModel* m_pModelChess;

	CFramebuffer* m_pNormalFramebuffer;
	CFramebuffer* m_pSelectFramebuffer;

	Config* m_pConfig;

	Camera* m_pCamera;

	CRender* m_pNormalRender;
	CRender* m_pSelectRender;

};

