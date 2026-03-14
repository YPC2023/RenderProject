#ifndef __CFRAMEBUFFER_H__
#define __CFRAMEBUFFER_H__

#include <vector>
#include "CUtils.h"
#include "CTextureManager.h"

class CFramebuffer
{
public:
	typedef struct _S_FRAMEBUFFERCONFIG
	{

	}S_FRAMEBUFFERCONFIG;
public:
	CFramebuffer();
	CFramebuffer(int width, int height);
public:
	unsigned int GetID() { return m_nID; }
	bool IsOK() { return m_bOK; }
	unsigned int GetFBO() { return m_FBO; }
	CTexture* GetColorTexture() { return m_ColorTexture; }
	unsigned int GetRBO() { return m_RBO; }
	int GetWidth() { return m_Width; }
	int GetHeight() { return m_Height; }
	void Resize(int width, int height);
private:
	bool SetupFramebuffer();
private:
	static std::vector<CFramebuffer*>	m_vec_Framebuffer;
private:
	unsigned int	m_nID;			// 非OpenGL的ID，而是示例的ID（自己生成）
	bool			m_bOK;
	unsigned int	m_FBO;          // 帧缓冲对象
	CTexture*		m_ColorTexture; // 颜色附件纹理
	unsigned int	m_RBO;          // 渲染缓冲对象（用于深度和模板测试）
	int				m_Width;
	int				m_Height;
};





#endif // __CFRAMEBUFFER_H__