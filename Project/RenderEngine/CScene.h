#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
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
public:
	typedef enum _E_MOUSE_ACTION_TYPE
	{
		E_MOUSE_ACTION_VALIDATE,	// 无效操作
		E_MOUSE_ACTION_SELECTED,	// 选择
		E_MOUSE_ACTION_MOVE,	// 移动
		E_MOUSE_ACTION_ROTATE,	// 旋转
	}E_MOUSE_ACTION_TYPE;
protected:
	CScene();
public:
	bool Initialize() override;
	void UnInitialize() override;
private:
	unsigned int GetSelectId(int x, int y);
	glm::vec3 GetRayDirection(double mouseX, double mouseY);
	glm::vec3 GetRayDirection2(double mouseX, double mouseY);
	float GetMovementOnAxis(glm::vec3 rayDir, glm::vec3 camPos, glm::vec3 modelPos, glm::vec3 axis);
	bool GetRayPlaneIntersection(
		const glm::vec3& rayOrigin,
		const glm::vec3& rayDir,
		const glm::vec3& planePoint,
		const glm::vec3& planeNormal,
		glm::vec3& outIntersection);
	float CalculateAngleOnPlane(
		const glm::vec3& intersection,
		const glm::vec3& centerPoint,
		const glm::vec3& rotationAxis);
private:
	void OnModelSelectedAction(int originX, int originY);
	void OnModelTranslateActionBegin(int originX, int originY);
	void OnModelTranslateActionEnd(int originX, int originY);
	void OnModelTranslateActionIng(int originX, int originY);

	void OnModelRotateActionBegin(int originX, int originY);
	void OnModelRotateActionEnd(int originX, int originY);
	void OnModelRotateActionIng(int originX, int originY);
protected:
	void OnMouseWheel(float delta) override;
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
	CModel* m_pModelSphere;	// 球体
	CModel* m_pModelCone; // 圆锥体
	CModel* m_pModelTorus; // 圆环

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


	E_MOUSE_ACTION_TYPE	m_MouseActionType;
	float		m_StartRotateAngle;
};

