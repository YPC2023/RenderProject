#ifndef __CMESH_H__
#define __CMESH_H__

#include <vector>
#include <glm/glm.hpp>
#include <CUtils.h>
#include "CTexture.h"
#include "CShader.h"
#include "IMeshInterface.h"


class Vec2
{
    friend class CMesh;
public:
    Vec2()
    {
        m_enable = false;
        m_value = glm::vec3(0.0f, 0.0f, 0.0f);
    }
public:
    Vec2 operator = (glm::vec2 v)
    {
        m_enable = true;
        m_value = v;
        return *this;
    }
private:
    glm::vec2   m_value;
protected:
    bool        m_enable;
};

class Vec3
{
    friend class CMesh;
public:
    Vec3()
    {
        m_enable = false;
        m_value = glm::vec3(0.0f, 0.0f, 0.0f);
    }
public:
    Vec3 operator = (glm::vec3 v)
    {
        m_enable = true;
        m_value = v;
        return *this;
    }
private:
    glm::vec3   m_value;
protected:
    bool        m_enable;
};

class Vec4
{
    friend class CMesh;
public:
    Vec4()
    {
        m_enable = false;
        m_value = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    }
public:
    Vec4 operator = (glm::vec4 v)
    {
        m_enable = true;
        m_value = v;
        return *this;
    }
private:
    glm::vec4   m_value;
protected:
    bool        m_enable;
};

class CMesh : IMeshInterface
{
public:
    typedef struct _S_VERTEX {
        // position
        Vec3 Position;
        // Color
        Vec4 Color;
        // normal
        Vec3 Normal;
        // texCoords
        Vec2 TexCoords;
        // tangent
        Vec3 Tangent;
        // bitangent
        Vec3 Bitangent;
    }S_VERTEX;
    typedef struct _S_TEXTURE {
        CTexture*   pTexture;
        std::string strType;
        std::string strPath;
    }S_TEXTURE;
public:
    CMesh(std::vector<S_VERTEX> vertices, 
        std::vector<unsigned int> indices, 
        unsigned int type = GL_TRIANGLES);
    CMesh(std::vector<S_VERTEX> vertices, 
        std::vector<unsigned int> indices, 
        std::vector<S_TEXTURE> textures, 
        unsigned int type = GL_TRIANGLES);
public:
    size_t GetVertexCount() override;
    unsigned int GetVAO() override;
    size_t GetIndexSize() override;
    unsigned int GetType() override;
    size_t GetTextureSize() override;
    const char* GetTextureName(size_t index) override;
    unsigned int GetTextureId(size_t index) override;
    bool IsPositionEnable() override;
    bool IsColorEnable() override;
    bool IsNormalEnable() override;
    bool IsTexCoordsEnable() override;
    bool IsTangentEnable() override;
    bool IsBitangentEnable() override;
public:
    unsigned int GetID() { return m_nID; }
    void AppendTexture(const S_TEXTURE& texture);
    void AppendTexture(CTexture* pTexture, const std::string& strType, const std::string& strPath = "");
    //void Render(const CShader& shader);
private:
    void SetupMesh();
    //void SetTexture(const CShader& shader);
    //void SetEnable(const CShader& shader, const S_VERTEX& vertex);
private:
    unsigned int                m_nID;
    std::vector<S_VERTEX>       m_vec_Vertices;
    std::vector<unsigned int>   m_vec_Indices;
    std::vector<S_TEXTURE>      m_vec_Textures;
    unsigned int                m_nType;
    unsigned int                m_VAO;
    unsigned int                m_VBO;
    unsigned int                m_EBO;
};



#endif // __CMESH_H__