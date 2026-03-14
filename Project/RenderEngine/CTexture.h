#ifndef ___CTEXTURE_H__
#define ___CTEXTURE_H__
#include <vector>
#include <map>
#include <glad/glad.h>
#include "CUtils.h"

class CTexture
{
public:
	typedef struct _S_TEXTURE_CONFIG
	{
		int width = 0;
		int height = 0;
		std::string strPath = "";
		unsigned int Format = GL_RGB;
		unsigned int TEX_GL_TEXTURE_WRAP_S = GL_REPEAT;
		unsigned int TEX_GL_TEXTURE_WRAP_T = GL_REPEAT;
		unsigned int TEX_GL_TEXTURE_MIN_FILTER = GL_LINEAR_MIPMAP_LINEAR;
		unsigned int TEX_GL_TEXTURE_MAG_FILTER = GL_LINEAR;
	}S_TEXTURE_CONFIG;
public:
	CTexture(const S_TEXTURE_CONFIG& Config);
private:
	bool LoadFromFile();
	bool CreateEmptyTexture();
public:
	unsigned int GetID() { return m_nID; }
	std::string GetPath() { return m_strPath; }
	bool IsOK() { return m_bOK; }
	void Resize(int width, int height);
	bool SaveToFile(const std::string& strPath);
private:
	static std::vector<CTexture*>				m_vec_Texture;
	static std::map<unsigned int, CTexture*>	m_map_Texture;
private:
	S_TEXTURE_CONFIG			m_Config;
private:
	bool						m_bOK;
	unsigned int				m_nID;
	std::string					m_strPath;

};




#endif // ___CTEXTURE_H__