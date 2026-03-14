#include "CModel.h"
#include "CTextureManager.h"
#include "CMeshManager.h"

bool CModel::m_bRenderId = false;
unsigned int CModel::m_nSelectedId = 0;
CModel::CModel()
{
    static unsigned int index = 0;
    m_nID = ++index;
    m_bOK = true;
    m_Model = glm::mat4(1.0f);
}

CModel::CModel(const std::string& strPath) : CModel()
{
    m_bOK = LoadModel(strPath);
}

/*
void CModel::Render(const CShader& shader)
{
    shader.setMat4("model", m_Model);
    SetRenderId(shader);
    SetSelected(shader);
    for (size_t index = 0; index < m_vec_Mesh.size(); ++index) {
        if (m_vec_Mesh[index]) {
            m_vec_Mesh[index]->Render(shader);
        }
    }
    
}
*/

bool CModel::SetPath(const std::string& strPath)
{
    m_bOK = LoadModel(strPath);
    return m_bOK;
}

void CModel::AppendMesh(CMesh* pMesh)
{
    if (pMesh) {
        m_vec_Mesh.push_back(pMesh);
    }
}

CMesh* CModel::GetFirstMesh()
{
    if (0 < m_vec_Mesh.size()) {
        return m_vec_Mesh[0];
    }
    return 0;
}

void CModel::SetModel(const glm::mat4& model)
{
    m_Model = model;
};

const glm::mat4& CModel::GetModel()
{
    return m_Model;
}

size_t CModel::GetMeshCount()
{
    return m_vec_Mesh.size();
}

IMeshInterface* CModel::GetMesh(size_t index)
{
    if (m_vec_Mesh.size() > index) {
        return (IMeshInterface*)m_vec_Mesh[index];
    }
    return 0;
}

unsigned int CModel::GetID()
{
    return m_nID;
}

void CModel::EnableRenderId(bool bEnable)
{
    m_bRenderId = bEnable;
}

void CModel::SelectModel(unsigned int id)
{
    m_nSelectedId = id;
}

bool CModel::LoadModel(const std::string& strPath)
{
    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(strPath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        PRINTLOG("ERROR::ASSIMP:: %s", importer.GetErrorString());
        return false;
    }
    size_t nPos = strPath.find_last_of("/\\");
    if (strPath.npos != nPos) {
        m_strRootPath = strPath.substr(0, nPos);
    }
    m_strPath = strPath;
    // process ASSIMP's root node recursively
    return ProcessNode(scene->mRootNode, scene);
}

bool CModel::ProcessNode(aiNode* node, const aiScene* scene)
{
    // process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene. 
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        CMesh* pMesh = ProcessMesh(mesh, scene);
        if (0 == pMesh) {
            return false;
        }
        m_vec_Mesh.push_back(pMesh);
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        if (!ProcessNode(node->mChildren[i], scene)) {
            return false;
        }
    }
    return true;
}

CMesh* CModel::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    // data to fill
    std::vector<CMesh::S_VERTEX>    vertices;
    std::vector<unsigned int>       indices;
    std::vector<CMesh::S_TEXTURE>   textures;

    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        CMesh::S_VERTEX vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    // 1. diffuse maps
    std::vector<CMesh::S_TEXTURE> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    std::vector<CMesh::S_TEXTURE> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<CMesh::S_TEXTURE> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<CMesh::S_TEXTURE> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    // return a mesh object created from the extracted mesh data
    return CMeshManager::GetInstance().GetMesh(vertices, indices, textures);
}

std::vector<CMesh::S_TEXTURE> CModel::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<CMesh::S_TEXTURE> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        if (aiReturn_SUCCESS == mat->GetTexture(type, i, &str)) {
            CMesh::S_TEXTURE texture;
            texture.pTexture = CTextureManager::GetInstance().GetTexture(m_strRootPath + "/" + str.C_Str());
            if (0 == texture.pTexture) {
                PRINTLOG("Fail to load texture(%s)", str.C_Str());
                continue;
            }
            texture.strType = typeName;
            texture.strPath = str.C_Str();
            textures.push_back(texture);
        }
    }
    return textures;
}
/*
void CModel::SetRenderId(const CShader& shader)
{
    shader.use();
    shader.setBool("renderID", m_bRenderId);
    if (m_bRenderId) {
        float v3 = (float)((m_nID >> 24) & 0xFF);
        float v2 = (float)((m_nID >> 16) & 0xFF);
        float v1 = (float)((m_nID >> 8) & 0xFF);
        float v0 = (float)(m_nID & 0xFF);
        shader.setVec4("objectID", glm::vec4(v0 / 255, v1 / 255, v2 / 255, v3 / 255));
    }
}

void CModel::SetSelected(const CShader& shader)
{
    shader.use();
    shader.setBool("hasSelected", m_nID == m_nSelectedId);
}
*/