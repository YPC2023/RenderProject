#include "CFrameBufferManager.h"


CFrameBufferManager& CFrameBufferManager::GetInstance()
{
	static CFrameBufferManager Instance;

	return Instance;
}

CFramebuffer* CFrameBufferManager::CreateFrameBuffer(int width, int height)
{
	CFramebuffer* pFramebuffer = new CFramebuffer(width, height);
	if (pFramebuffer) {
		m_map_Framebuffer[pFramebuffer->GetFBO()] = pFramebuffer;
	}
	return pFramebuffer;
}