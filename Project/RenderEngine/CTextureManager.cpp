#include "CTextureManager.h"


CTextureManager& CTextureManager::GetInstance()
{
	static CTextureManager Instance;
	return Instance;
}

CTexture* CTextureManager::GetTexture(const std::string& strPath)
{
	// 是否已经存在
	if (0 < m_map_TexturePath.count(strPath)) {
		return m_map_TexturePath[strPath];
	}
	CTexture::S_TEXTURE_CONFIG config;
	config.strPath = strPath;
	// 新建纹理      
	CTexture* pTexture = new CTexture(config);
	if (pTexture) {
		m_map_TexturePath[strPath] = pTexture;
	}
	// 加载文件是否成功
	if (!pTexture->IsOK()) {
		delete pTexture;
		return 0;
	}
	return pTexture;
}

CTexture* CTextureManager::GetTexture(int width, int height, unsigned int Format)
{
	CTexture::S_TEXTURE_CONFIG config;
	config.width = width;
	config.height = height;
	config.Format = Format;
	config.TEX_GL_TEXTURE_MAG_FILTER = GL_LINEAR;
	config.TEX_GL_TEXTURE_MIN_FILTER = GL_LINEAR;
	// 新建纹理      
	CTexture* pTexture = new CTexture(config);
	if (pTexture) {
		m_map_TextureID[pTexture->GetID()] = pTexture;
	}
	// 加载文件是否成功
	if (!pTexture->IsOK()) {
		delete pTexture;
		return 0;
	}
	return pTexture;
}