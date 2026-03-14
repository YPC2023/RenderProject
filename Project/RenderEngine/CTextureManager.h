#ifndef __CTEXTUREMANAGER_H__
#define __CTEXTUREMANAGER_H__
#include <map>
#include "CUtils.h"
#include "CTexture.h"

class CTextureManager
{
public:
	static CTextureManager& GetInstance();
public:
	CTexture* GetTexture(const std::string& strPath);
	CTexture* GetTexture(int width, int height, unsigned int Format = GL_RGB);
private:
	std::map<std::string, CTexture*>			m_map_TexturePath;
	std::map<unsigned int, CTexture*>			m_map_TextureID;
};





#endif // __CTEXTUREMANAGER_H__