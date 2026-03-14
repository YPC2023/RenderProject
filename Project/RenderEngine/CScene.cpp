#include "pch.h"
#include "CScene.h"

CScene::CScene()
{
    m_pNormalShader = 0;
    m_pSelectShader = 0;

    m_pModelFile = 0;
    m_pModelCoord = 0;
    m_pModelChess = 0;

    m_pNormalFramebuffer = 0;
    m_pSelectFramebuffer = 0;

    m_pCamera = 0;
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
        // ¶ÁÈ¡ÏñËØ
        glReadPixels((GLint)x, (GLint)(m_pConfig->GetViewportHeight() - y),
            1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &nSelectId);
        // »Ö¸´Ä¬ÈÏÖ¡»º³åÇø
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        /*PRINTLOG("Select Object (%f,%f)[%d,%d,%d,%d](%u)", x, y,
            (nSelectId >> 24) & 0xFF, (nSelectId >> 16) & 0xFF,
            (nSelectId >> 8) & 0xFF, nSelectId & 0xFF, nSelectId);*/
    }
    return nSelectId;
}

void CScene::OnSelectAction(int originX, int originY, float x, float y)
{
    RenderSelect();

    unsigned int nSelectId = GetSelectId(originX, originY);
    
    if (0 != m_pNormalRender) {
        m_pNormalRender->SetSelectId(nSelectId);
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
