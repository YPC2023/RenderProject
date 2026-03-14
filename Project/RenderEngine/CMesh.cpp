#include "CMesh.h"
#include <glad/glad.h>

CMesh::CMesh(std::vector<S_VERTEX> vertices,
    std::vector<unsigned int> indices, unsigned int type)
{
    static unsigned int index = 0;
    m_nID = ++index;

    m_nType = type;
    m_vec_Vertices = vertices;
    m_vec_Indices = indices;

    SetupMesh();
}

CMesh::CMesh(std::vector<S_VERTEX> vertices,
    std::vector<unsigned int> indices,
    std::vector<S_TEXTURE> textures, unsigned int type) :CMesh(vertices, indices, type)
{
    this->m_vec_Textures = textures;
}

size_t CMesh::GetVertexCount()
{
    return m_vec_Vertices.size();
}

unsigned int CMesh::GetVAO()
{
    return m_VAO;
}

size_t CMesh::GetIndexSize()
{
    return m_vec_Indices.size();
}

unsigned int CMesh::GetType()
{
    return m_nType;
}

size_t CMesh::GetTextureSize()
{
    return m_vec_Textures.size();
}

const char* CMesh::GetTextureName(size_t index)
{
    ASSERT(index < m_vec_Textures.size());
    if (index < m_vec_Textures.size()) {
        return m_vec_Textures[index].strPath.c_str();
    }
    return 0;
}

unsigned int CMesh::GetTextureId(size_t index)
{
    ASSERT(index < m_vec_Textures.size());
    if (index < m_vec_Textures.size()) {
        CTexture* pTexture = m_vec_Textures[index].pTexture;
        ASSERT(0 != pTexture);
        return pTexture->GetID();
    }
    return 0;
}

bool CMesh::IsPositionEnable()
{
    if (0 < m_vec_Vertices.size()) {
        return m_vec_Vertices[0].Position.m_enable;
    }
    return false;
}

bool CMesh::IsColorEnable()
{
    if (0 < m_vec_Vertices.size()) {
        return m_vec_Vertices[0].Color.m_enable;
    }
    return false;
}

bool CMesh::IsNormalEnable()
{
    if (0 < m_vec_Vertices.size()) {
        return m_vec_Vertices[0].Normal.m_enable;
    }
    return false;
}

bool CMesh::IsTexCoordsEnable()
{
    if (0 < m_vec_Vertices.size()) {
        return m_vec_Vertices[0].TexCoords.m_enable;
    }
    return false;
}

bool CMesh::IsTangentEnable()
{
    if (0 < m_vec_Vertices.size()) {
        return m_vec_Vertices[0].Tangent.m_enable;
    }
    return false;
}

bool CMesh::IsBitangentEnable()
{
    if (0 < m_vec_Vertices.size()) {
        return m_vec_Vertices[0].Bitangent.m_enable;
    }
    return false;
}

void CMesh::AppendTexture(const S_TEXTURE& texture)
{
    m_vec_Textures.push_back(texture);
}

void CMesh::AppendTexture(CTexture* pTexture, const std::string& strType, const std::string& strPath)
{
    if (0 != pTexture) {
        S_TEXTURE texture;
        texture.pTexture = pTexture;
        texture.strType = strType;
        texture.strPath = strPath;
        AppendTexture(texture);
    }
}
/*
void CMesh::Render(const CShader& shader)
{
    // 激活纹理
    SetTexture(shader);
    // 设置是能属性
    if (0 < m_vec_Vertices.size()) {
        SetEnable(shader, m_vec_Vertices[0]);
    }

    // draw mesh
    glBindVertexArray(m_VAO);
    glDrawElements(m_nType, static_cast<unsigned int>(m_vec_Indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}
*/
void CMesh::SetupMesh()
{
    // create buffers/arrays
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, m_vec_Vertices.size() * sizeof(S_VERTEX), &m_vec_Vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vec_Indices.size() * sizeof(unsigned int), &m_vec_Indices[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(S_VERTEX), (void*)0);
    // vertex color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(S_VERTEX), (void*)offsetof(S_VERTEX, Color));
    // vertex normals
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(S_VERTEX), (void*)offsetof(S_VERTEX, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(S_VERTEX), (void*)offsetof(S_VERTEX, TexCoords));
    // vertex tangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(S_VERTEX), (void*)offsetof(S_VERTEX, Tangent));
    // vertex bitangent
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(S_VERTEX), (void*)offsetof(S_VERTEX, Bitangent));

    glEnableVertexAttribArray(0);

}
/*
void CMesh::SetTexture(const CShader& shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    for (unsigned int i = 0; i < m_vec_Textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = m_vec_Textures[i].strType;
        if (name == "texture_diffuse")
            number = CUtils::UintToString(diffuseNr++);
        else if (name == "texture_specular")
            number = CUtils::UintToString(specularNr++); // transfer unsigned int to string
        else if (name == "texture_normal")
            number = CUtils::UintToString(normalNr++); // transfer unsigned int to string
        else if (name == "texture_height")
            number = CUtils::UintToString(heightNr++); // transfer unsigned int to string

        // now set the sampler to the correct texture unit
        shader.setInt(name.c_str(), i);
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, m_vec_Textures[i].pTexture->GetID());
    }
}

void CMesh::SetEnable(const CShader& shader, const S_VERTEX& vertex)
{
    shader.setBool("hasPos", vertex.Position.m_enable);
    shader.setBool("hasColor", vertex.Color.m_enable);
    shader.setBool("hasNormal", vertex.Normal.m_enable);
    shader.setBool("hasTexCoords", vertex.TexCoords.m_enable);
    shader.setBool("hasTangent", vertex.Tangent.m_enable);
    shader.setBool("hasBitangent", vertex.Bitangent.m_enable);
}
*/