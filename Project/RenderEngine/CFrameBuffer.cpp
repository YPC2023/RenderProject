#include "CFrameBuffer.h"
#include <glad/glad.h>

std::vector<CFramebuffer*>	CFramebuffer::m_vec_Framebuffer;

CFramebuffer::CFramebuffer()
{
    static unsigned int index = 0;
    m_nID = ++index;
    m_vec_Framebuffer.push_back(this);
}
CFramebuffer::CFramebuffer(int width, int height) : CFramebuffer()
{
    m_Width = width;
    m_Height = height;
    m_bOK = SetupFramebuffer();
}

void CFramebuffer::Resize(int width, int height)
{
    if (m_ColorTexture) {
        m_ColorTexture->Resize(width, height);
    }
    // 更新深度模板缓冲
    glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
}

bool CFramebuffer::SetupFramebuffer()
{
    bool bRet = true;
    // 1. 创建帧缓冲
    glGenFramebuffers(1, &m_FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

    // 2. 创建纹理附件
    m_ColorTexture = CTextureManager::GetInstance().GetTexture(m_Width, m_Height, GL_RGBA);
    if (0 == m_ColorTexture) {
        PRINTLOG("Fail to create color texture for framebuffer");
        return false;
    }
    // 将纹理附加到帧缓冲的颜色附件
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTexture->GetID(), 0);

    // 3. 创建渲染缓冲对象（用于深度和模板附件）
    glGenRenderbuffers(1, &m_RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    // 将渲染缓冲附加到帧缓冲
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);

    // 4. 检查帧缓冲完整性
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        PRINTLOG("Fail to crate framebuffer");
        bRet = false;
    }

    // 5. 解绑帧缓冲
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return bRet;
}