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
	void SelectAction(int originX, int originY);
	glm::vec3 GetRayDirection(double mouseX, double mouseY);
	glm::vec3 GetRayDirection2(double mouseX, double mouseY);
	float GetMovementOnAxis(glm::vec3 rayDir, glm::vec3 camPos, glm::vec3 modelPos, glm::vec3 axis);

protected:
	void OnMouseLeftDown(int originX, int originY, float x, float y) override;
	void OnMouseLeftUp(int originX, int originY, float x, float y) override;
	void OnMouseLeftMove(int originX, int originY, float x, float y) override;
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
	CModel* m_pModelColumn;
	CModel* m_pModelSphere;	// ÇòÌå
	CModel* m_pModelCone; // Ô²×¶Ìå
	CModel* m_pModelTorus; // Ô²»·

	CModel* m_pModelSelected;

	CFramebuffer* m_pNormalFramebuffer;
	CFramebuffer* m_pSelectFramebuffer;

	Config* m_pConfig;

	Camera* m_pCamera;

	CRender* m_pNormalRender;
	CRender* m_pSelectRender;

private:
	bool		m_bLeftMouseMoved;
	float		m_StartPosition;
	glm::vec3	m_ModelPosition;
	glm::vec3	m_AxisTransform;

};

