#include "pch.h"
#include "CScene.h"
#include "CModelManager.h"

CScene::CScene()
{
    m_pNormalShader = 0;
    m_pSelectShader = 0;

    m_pModelFile = 0;
    m_pModelCoord = 0;
    m_pModelChess = 0;
    m_pModelColumn = 0;
    m_pModelCone = 0;
    m_pModelSphere = 0;
    m_pModelTorus = 0;

    m_pModelSelected = 0;

    m_pNormalFramebuffer = 0;
    m_pSelectFramebuffer = 0;

    m_pCamera = 0;

    m_AxisTransform = glm::vec3(1.0f, 0.0f, 0.0f);
    m_bLeftMouseMoved = false;
    m_MouseActionType = E_MOUSE_ACTION_VALIDATE;
}

bool CScene::Initialize()
{
    if (!InitializeConfig()) {
        PRINTLOG("Fail to initialize Config");
        return false;
    }
    if (!InitializeShader()) {
        PRINTLOG("Fail to initialize Shader");
        return false;
    }
    if (!InitializeModel()) {
        PRINTLOG("Fail to initialize Model");
        return false;
    }
    if (!InitializeFramebuffer()) {
        PRINTLOG("Fail to initialize Framebuffer");
        return false;
    }
    if (!InitializeCamera()) {
        PRINTLOG("Fail to initialize Camera");
        return false;
    }
    if (!InitializeRender()) {
        PRINTLOG("Fail to initialize Render");
        return false;
    }
	return true;
}

void CScene::UnInitialize()
{
	if (0 != m_pNormalShader) {
        m_pNormalShader->UnInitialize();
	}
}

unsigned int CScene::GetSelectId(int x, int y)
{
    unsigned int nSelectId = 0;  // RGB
    if (0 != m_pSelectFramebuffer && 0 != m_pConfig) {

        glBindFramebuffer(GL_FRAMEBUFFER, m_pSelectFramebuffer->GetFBO());

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D, m_pSelectFramebuffer->GetColorTexture()->GetID(), 0);
        // 读取像素
        glReadPixels((GLint)x, (GLint)(m_pConfig->GetViewportHeight() - y),
            1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &nSelectId);
        // 恢复默认帧缓冲区
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        /*PRINTLOG("Select Object (%f,%f)[%d,%d,%d,%d](%u)", x, y,
            (nSelectId >> 24) & 0xFF, (nSelectId >> 16) & 0xFF,
            (nSelectId >> 8) & 0xFF, nSelectId & 0xFF, nSelectId);*/
    }
    return nSelectId;
}

glm::vec3 CScene::GetRayDirection(double mouseX, double mouseY) 
{
    int width = m_pConfig->GetViewportWidth();
    int height = m_pConfig->GetViewportHeight();
    const glm::mat4& viewMatrix = m_pCamera->GetView();
    const glm::mat4& projectionMatrix = m_pCamera->GetProjection();

    // 1. 将鼠标坐标转换为 NDC 坐标 (-1.0 到 1.0)
    // 假设 mouseX, mouseY 来自 GLFW 回调 (左上角为 0,0)
    float x = (2.0f * (float)mouseX) / (float)width - 1.0f;
    float y = 1.0f - (2.0f * (float)mouseY) / (float)height;

    // 2. 构造投影空间下的近裁剪面点 (z = -1.0)
    // 注意 w 为 1.0，表示这是一个点
    glm::vec4 ray_clip = glm::vec4(x, y, -1.0f, 1.0f);

    // 3. 变换到观察空间 (Camera Space)
    glm::vec4 ray_eye = glm::inverse(projectionMatrix) * ray_clip;

    // 关键点：将 z 设为 -1 (向前看)，w 设为 0 (我们要的是方向向量，不是点)
    // 这样在乘以 View 矩阵逆时，平移部分会被忽略
    ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);

    // 4. 变换到世界空间 (World Space)
    glm::vec3 ray_world = glm::vec3(glm::inverse(viewMatrix) * ray_eye);

    // 5. 归一化，得到最终的射线方向
    return glm::normalize(ray_world);
}

glm::vec3 CScene::GetRayDirection2(double mouseX, double mouseY)
{
    int width = m_pConfig->GetViewportWidth();
    int height = m_pConfig->GetViewportHeight();
    const glm::mat4& view = m_pCamera->GetView();
    const glm::mat4& projection = m_pCamera->GetProjection();

    // 1. 获取视口定义 [x, y, width, height]
    glm::vec4 viewport = glm::vec4(0, 0, width, height);

    // 2. 转换鼠标坐标
    // GLFW 等库的 y 是自顶向下的，而 OpenGL 的 unProject 预期 y 是自底向上的
    float winX = (float)mouseX;
    float winY = (float)height - (float)mouseY;

    // 3. 计算近裁剪面上的点 (z = 0.0 在 unProject 中映射到 NDC 的 -1)
    glm::vec3 nearPos = glm::unProject(
        glm::vec3(winX, winY, 0.0f),
        view,
        projection,
        viewport
    );

    // 4. 计算远裁剪面上的点 (z = 1.0 在 unProject 中映射到 NDC 的 1)
    glm::vec3 farPos = glm::unProject(
        glm::vec3(winX, winY, 1.0f),
        view,
        projection,
        viewport
    );

    //m_pModelColumn = CObjectManager::GetInstance().GetColumnModel(farPos, nearPos, 0.01f);
    // 5. 计算方向向量并归一化
    return glm::normalize(farPos - nearPos);
}

// 输入：鼠标射线向量 v, 相机位置 O, 模型当前位置 P0, 轴向量 u
// 输出：模型沿轴移动后的距离
float CScene::GetMovementOnAxis(glm::vec3 rayDir, glm::vec3 camPos, glm::vec3 modelPos, glm::vec3 axis)
{
    // 确保方向向量都是单位向量
    glm::vec3 u = glm::normalize(axis);
    glm::vec3 v = glm::normalize(rayDir);
    glm::vec3 r = camPos - modelPos; // 注意符号，这里对应推导中的 O - P0

    float b = glm::dot(u, v);
    float d = glm::dot(u, r);
    float e = glm::dot(v, r);

    float denominator = 1.0f - b * b;

    // 如果分母接近 0，说明射线与轴平行，无法产生有效位移
    if (glm::abs(denominator) < 0.0001f) {
        return 0.0f;
    }

    return (b * e - d) / denominator;
}

bool CScene::GetRayPlaneIntersection(
    const glm::vec3& rayOrigin,
    const glm::vec3& rayDir,
    const glm::vec3& planePoint,
    const glm::vec3& planeNormal,
    glm::vec3& outIntersection)
{
    float denom = glm::dot(planeNormal, rayDir);
    // 确保射线不与平面平行（阈值可微调）
    if (std::abs(denom) > 1e-6f) {
        float t = glm::dot(planePoint - rayOrigin, planeNormal) / denom;
        if (t >= 0) {
            outIntersection = rayOrigin + t * rayDir;
            return true;
        }
    }
    return false;
}

float CScene::CalculateAngleOnPlane(
    const glm::vec3& intersection,
    const glm::vec3& centerPoint,
    const glm::vec3& rotationAxis)
{
    glm::vec3 normAxis = glm::normalize(rotationAxis);

    // 1. 构建平面局部坐标系的基向量 (Tangent 和 Bitangent)
    glm::vec3 tangentX;
    if (std::abs(normAxis.y) < 0.9f)
        tangentX = glm::normalize(glm::cross(normAxis, glm::vec3(0, 1, 0)));
    else
        tangentX = glm::normalize(glm::cross(normAxis, glm::vec3(1, 0, 0)));

    glm::vec3 tangentY = glm::cross(normAxis, tangentX);

    // 2. 计算交点相对于中心点的偏移向量
    glm::vec3 localVec = intersection - centerPoint;

    // 3. 投影到 X 和 Y 轴得到 2D 坐标
    float x = glm::dot(localVec, tangentX);
    float y = glm::dot(localVec, tangentY);

    // 4. 返回方位角 (范围 -PI 到 PI)
    return std::atan2(y, x);
}

void CScene::OnModelSelectedAction(int originX, int originY)
{
    if (0 == m_pNormalRender) {
        return;
    }
    RenderSelect();
    unsigned int nSelectId = GetSelectId(originX, originY);
    m_pNormalRender->SetSelectId(nSelectId);
    m_pModelSelected = CModelManager::GetInstance().FindModel(nSelectId);
}

void CScene::OnModelTranslateActionBegin(int originX, int originY)
{
    if (0 == m_pModelSelected) {
        return;
    }
    m_ModelPosition = m_pModelSelected->GetTranslation();
    m_AxisTransform = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 rayDir = GetRayDirection2(originX, originY);
    m_StartPosition = GetMovementOnAxis(rayDir, m_pCamera->GetPosition(), m_ModelPosition, m_AxisTransform);
}

void CScene::OnModelTranslateActionEnd(int originX, int originY)
{
    if (!m_bLeftMouseMoved || 0 == m_pModelSelected) {
        return;
    }
    m_pModelSelected->ActionTransform();
    m_pModelSelected->ResetTranslation();
}

void CScene::OnModelTranslateActionIng(int originX, int originY)
{
    if (0 == m_pModelSelected) {
        return;
    }
    m_bLeftMouseMoved = true;
    glm::vec3 rayDir = GetRayDirection2(originX, originY);
    float current = GetMovementOnAxis(rayDir, m_pCamera->GetPosition(), m_ModelPosition, m_AxisTransform);
    m_pModelSelected->SetTranslation(m_ModelPosition + m_AxisTransform * (m_StartPosition - current));
}

void CScene::OnModelRotateActionBegin(int originX, int originY)
{
    if (0 == m_pModelSelected) {
        return;
    }
    glm::vec3 rayDir = GetRayDirection(originX, originY);
    glm::vec3 intersection;
    m_ModelPosition = m_pModelSelected->GetTranslation();

    if (GetRayPlaneIntersection(m_pConfig->GetCameraPos(), rayDir, m_ModelPosition, m_AxisTransform, intersection)) {
        // A. 保存点击时的初始方位角
        m_StartRotateAngle = CalculateAngleOnPlane(intersection, m_ModelPosition, m_AxisTransform);
    }
}

void CScene::OnModelRotateActionEnd(int originX, int originY)
{
    if (0 == m_pModelSelected) {
        return;
    }
    m_pModelSelected->ActionTransform();
    m_pModelSelected->ResetRotation();
}

void CScene::OnModelRotateActionIng(int originX, int originY)
{
    if (0 == m_pModelSelected) {
        return;
    }
    // 1. 获取当前射线
    glm::vec3 rayDir = GetRayDirection(originX, originY);
    glm::vec3 intersection;

    if (GetRayPlaneIntersection(m_pConfig->GetCameraPos(), rayDir, m_ModelPosition, m_AxisTransform, intersection)) {
        // C. 计算当前的方位角
        float currentAngle = CalculateAngleOnPlane(intersection, m_ModelPosition, m_AxisTransform);

        // D. 计算角度差值
        float deltaAngle = currentAngle - m_StartRotateAngle;
        // E. 构造增量四元数，并叠加到初始姿态上
        glm::quat deltaQuat = glm::angleAxis(deltaAngle, glm::normalize(m_AxisTransform));
        m_pModelSelected->SetRotation(deltaQuat);
        m_bLeftMouseMoved = true;
    }
}


void CScene::OnMouseWheel(float delta)
{
	if (0 == m_pModelSelected) {
		return;
	}
	// 前滚放大(delta>0)，后滚缩小(delta<0)
	float scaleDelta = delta * 0.1f;
	glm::vec3 currentScale = m_pModelSelected->GetScale();
	float newScale = currentScale.x + scaleDelta;
	if (newScale < 0.1f) newScale = 0.1f;
	if (newScale > 10.0f) newScale = 10.0f;
	m_pModelSelected->SetScale(glm::vec3(newScale, newScale, newScale));
}
void CScene::OnMouseLeftDown(int originX, int originY, float x, float y)
{
    m_MouseActionType = E_MOUSE_ACTION_MOVE;
    if (E_MOUSE_ACTION_MOVE == m_MouseActionType) {
        OnModelTranslateActionBegin(originX, originY);
    }
    else if (E_MOUSE_ACTION_ROTATE == m_MouseActionType) {
        OnModelRotateActionBegin(originX, originY);
    }
}

void CScene::OnMouseLeftUp(int originX, int originY, float x, float y)
{
    if (!m_bLeftMouseMoved) {
        OnModelSelectedAction(originX, originY);
        return;
    }
    if (E_MOUSE_ACTION_MOVE == m_MouseActionType) {
        OnModelTranslateActionEnd(originX, originY);
    }
    else if (E_MOUSE_ACTION_ROTATE == m_MouseActionType) {
        OnModelRotateActionEnd(originX, originY);
    }
    
    m_bLeftMouseMoved = false;
}

void CScene::OnMouseLeftMove(int originX, int originY, float x, float y)
{
    if (E_MOUSE_ACTION_MOVE == m_MouseActionType) {
        OnModelTranslateActionIng(originX, originY);
    }
    else if (E_MOUSE_ACTION_ROTATE == m_MouseActionType) {
        OnModelRotateActionIng(originX, originY);
    }
}

Config* CScene::GetConfig()
{
    return m_pConfig;
}

void CScene::OnRender(CRender* pRender)
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (0 != pRender) {
        if (0 != m_pModelFile) {
            pRender->Render(m_pModelFile);
        }
        if (0 != m_pModelCoord) {
            pRender->Render(m_pModelCoord);
        }
        if (0 != m_pModelChess) {
            pRender->Render(m_pModelChess);
        }
        if (0 != m_pModelColumn) {
            pRender->Render(m_pModelColumn);
        }
        if (0 != m_pModelCone) {
            pRender->Render(m_pModelCone);
        }
        if (0 != m_pModelSphere) {
            pRender->Render(m_pModelSphere);
        }
        if (0 != m_pModelTorus) {
            pRender->Render(m_pModelTorus);
        }
    }
}

void CScene::RenderNormal()
{
    if (0 != m_pNormalFramebuffer && 0 != m_pConfig) {
        glViewport(0, 0, m_pConfig->GetViewportWidth(), m_pConfig->GetViewportHeight());

        glBindFramebuffer(GL_FRAMEBUFFER, m_pNormalFramebuffer->GetFBO());

        OnRender(m_pNormalRender);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

void CScene::RenderSelect()
{
    if (0 != m_pSelectFramebuffer && 0 != m_pConfig) {
        glViewport(0, 0, m_pConfig->GetViewportWidth(), m_pConfig->GetViewportHeight());

        glBindFramebuffer(GL_FRAMEBUFFER, m_pSelectFramebuffer->GetFBO());

        OnRender(m_pSelectRender);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

void CScene::Render()
{
    RenderNormal();
    //RenderSelect();
}

void CScene::ResizeConfig(int width, int height)
{
    if (0 == m_pConfig) {
        PRINTLOG("m_pConfig is null");
        return;
    }
    m_pConfig->SetSize(width, height);
}

void CScene::ResizeCamera(int width, int height)
{
    if (0 == m_pCamera) {
        PRINTLOG("m_pCamera is null");
        return;
    }
    m_pCamera->Resize(width, height);
}

void CScene::ResizeNormalFramebuffer(int width, int height)
{
    if (0 == m_pNormalFramebuffer) {
        PRINTLOG("m_pNormalFramebuffer is null");
        return;
    }
    m_pNormalFramebuffer->Resize(width, height);
}

void CScene::ResizeSelectFramebuffer(int width, int height)
{
    if (0 == m_pSelectFramebuffer) {
        PRINTLOG("m_pSelectFramebuffer is null");
        return;
    }
    m_pSelectFramebuffer->Resize(width, height);
}

void CScene::Resize(int width, int height)
{
    ResizeConfig(width, height);
    ResizeCamera(width, height);
    ResizeNormalFramebuffer(width, height);
    ResizeSelectFramebuffer(width, height);
}

unsigned int CScene::GetRenderTextureId()
{
    if (0 == m_pNormalFramebuffer) {
        PRINTLOG("MainFramebuffer is empty");
        return 0;
    }
    return m_pNormalFramebuffer->GetColorTexture()->GetID();
}

bool CScene::InitializeShader()
{
    m_pNormalShader = CShaderManager::GetInstance().
		CreateShader(CUtils::GetShaderPathFile("model.vs"), CUtils::GetShaderPathFile("model.fs"));
	if (0 == m_pNormalShader) {
		PRINTLOG("Fail to create shader of normal");
		return false;
	}
    m_pSelectShader = CShaderManager::GetInstance().
        CreateShader(CUtils::GetShaderPathFile("select.vs"), CUtils::GetShaderPathFile("select.fs"),
            CShaderManager::E_SHADER_SELECT);
    if (0 == m_pSelectShader) {
        PRINTLOG("Fail to create shader of select");
        return false;
    }
	return true;
}

bool CScene::InitializeModel()
{
    m_pModelFile = CObjectManager::GetInstance().GetModelFromFile((CUtils::GetModelPathFile("cyborg/cyborg.obj")));
    //m_pFileModel = CObjectManager::GetInstance().GetModelFromFile((CUtils::GetModelPathFile("udk0xohj4k-cityislands/City_Islands/City_Islands.obj")));
    //m_pFileModel = CObjectManager::GetInstance().GetModelFromFile((CUtils::GetModelPathFile("mountain/mount.blend1.obj")));
    if (0 == m_pModelFile) {
        PRINTLOG("Fail to create model");
        return false;
    }

    m_pModelCoord = CObjectManager::GetInstance().GetCoordModel(5.0f);
    if (0 == m_pModelCoord) {
        PRINTLOG("Fail to create coord model");
        return false;
    }

    m_pModelChess = CObjectManager::GetInstance().GetChessModel(50.0f);
    if (0 == m_pModelChess) {
        PRINTLOG("Fail to create chess model");
        return false;
    }
    m_pModelChess->SetModel(glm::scale(glm::mat4(1.0f), glm::vec3(100.0f, 100.0f, 100.0f)));

    glm::vec3 start(0.0f, 0.0f, 0.0f);
    glm::vec3 end(0.0f, 3.0f, 0.0f);
    float radius = 0.1f;
    m_pModelColumn = CObjectManager::GetInstance().GetColumnModel(start, end, radius);
    if (0 == m_pModelColumn) {
        PRINTLOG("Fail to create column model");
        return false;
    }
    m_pModelCone = CObjectManager::GetInstance().GetConeModel(end, end - start, 0.5f, radius * 5);
    if (0 == m_pModelCone) {
        PRINTLOG("Fail to create cone model");
        return false;
    }
    m_pModelSphere = CObjectManager::GetInstance().GetSphereModel(glm::vec3(0.0f, 0.0f, 0.0f));
    if (0 == m_pModelSphere) {
        PRINTLOG("Fail to create sphere model");
        return false;
    }
    m_pModelTorus = CObjectManager::GetInstance().GetTorusModel(start, glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, 0.05f);
    if (0 == m_pModelTorus) {
        PRINTLOG("Fail to create torus model");
        return false;
    }
    return true;
}

bool CScene::InitializeFramebuffer()
{
    m_pNormalFramebuffer = CFrameBufferManager::GetInstance().CreateFrameBuffer(
        m_pConfig->GetViewportWidth(), m_pConfig->GetViewportHeight()
    );
    if (0 == m_pNormalFramebuffer) {
        PRINTLOG("Fail to create MainFramebuffer for Normal");
        return false;
    }
    m_pSelectFramebuffer = CFrameBufferManager::GetInstance().CreateFrameBuffer(
        m_pConfig->GetViewportWidth(), m_pConfig->GetViewportHeight()
    );
    if (0 == m_pSelectFramebuffer) {
        PRINTLOG("Fail to create MainFramebuffer for Select");
        return false;
    }
    return true;
}

bool CScene::InitializeConfig()
{
    m_pConfig = ConfigManager::GetInstance().CreateConfig();
    if (0 == m_pConfig) {
        PRINTLOG("Fail to create config instance");
        return false;
    }
    return true;
}

bool CScene::InitializeCamera()
{
    m_pCamera = CameraManager::GetInstance().CreateCamera(
        m_pConfig->GetCameraPos(), m_pConfig->GetCameraUp(), m_pConfig->GetCameraTarget()
    );
    if (0 == m_pCamera) {
        PRINTLOG("Fail to create camera");
        return false;
    }
    return true;
}

bool CScene::InitializeRender()
{
    m_pNormalRender = CRenderManager::GetInstance().CreateRender(m_pNormalShader, m_pCamera);
    if (0 == m_pNormalRender) {
        PRINTLOG("Fail to create Render for Normal");
        return false;
    }
    m_pSelectRender = CRenderManager::GetInstance().CreateRender(m_pSelectShader, m_pCamera, CRenderManager::E_RENDER_SELECT);
    if (0 == m_pSelectRender) {
        PRINTLOG("Fail to create Render for Select");
        return false;
    }
    return true;
}
