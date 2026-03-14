#ifndef __CFRAMEBUFFERMANAGER_H__
#define __CFRAMEBUFFERMANAGER_H__

#include <map>
#include "CFrameBuffer.h"

class CFrameBufferManager
{
public:
	static CFrameBufferManager& GetInstance();
public:
	CFramebuffer* CreateFrameBuffer(int width, int height);

private:
	std::map<unsigned int, CFramebuffer*>		m_map_Framebuffer;
};





#endif // __CFRAMEBUFFERMANAGER_H__