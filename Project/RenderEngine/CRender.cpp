#include "pch.h"
#include "CRender.h"


CRender::CRender(IShaderInterface* shader, ICameraInterface* camera)
{
	m_pShader = shader;
    m_pCamera = camera;
    m_nSelectedId = 0;
}

void CRender::RenderModel(IModelInterface* model)
{
    if (0 == model || 0 == m_pShader || 0 == m_pCamera) {
        return;
    }
    m_pShader->use();
    m_pShader->setMat4("model", model->GetModel());
    for (size_t index = 0; index < model->GetMeshCount(); ++index) {
        RenderMesh(model->GetMesh(index));
    }
}

void CRender::RenderMesh(IMeshInterface* mesh)
{
    if (0 == mesh) {
        return;
    }
    // 激活纹理
    SetTexture(mesh);
    // 设置是能属性
    if (0 < mesh->GetVertexCount()) {
        SetEnable(mesh);
    }

    // draw mesh
    glBindVertexArray(mesh->GetVAO());
    glDrawElements(mesh->GetType(), static_cast<unsigned int>(mesh->GetIndexSize()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}

void CRender::Render(IModelInterface* model)
{
    if (0 != m_pShader && 0 != m_pCamera) {
        m_pShader->use();
        m_pShader->setMat4("projection", m_pCamera->GetProjection());
        m_pShader->setMat4("view", m_pCamera->GetView());
    }
    SetSelected(model);
    RenderModel(model);
}

void CRender::SetSelectId(unsigned int id)
{
    m_nSelectedId = id;
}

void CRender::SetTexture(IMeshInterface* mesh)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    if (0 == mesh || 0 == m_pShader) {
        return;
    }
    for (size_t index = 0; index < mesh->GetTextureSize(); ++index)
    {
        glActiveTexture((GLenum)(GL_TEXTURE0 + index)); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = mesh->GetTextureName(index);
        if (name == "texture_diffuse")
            number = CUtils::UintToString(diffuseNr++);
        else if (name == "texture_specular")
            number = CUtils::UintToString(specularNr++); // transfer unsigned int to string
        else if (name == "texture_normal")
            number = CUtils::UintToString(normalNr++); // transfer unsigned int to string
        else if (name == "texture_height")
            number = CUtils::UintToString(heightNr++); // transfer unsigned int to string

        // now set the sampler to the correct texture unit
        m_pShader->setInt(name.c_str(), (int)index);
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, mesh->GetTextureId(index));
    }
}

void CRender::SetEnable(IMeshInterface* mesh)
{
    if (0 != mesh && 0 != m_pShader) {
        m_pShader->setBool("hasPos", mesh->IsPositionEnable());
        m_pShader->setBool("hasColor", mesh->IsColorEnable());
        m_pShader->setBool("hasNormal", mesh->IsNormalEnable());
        m_pShader->setBool("hasTexCoords", mesh->IsTexCoordsEnable());
        m_pShader->setBool("hasTangent", mesh->IsTangentEnable());
        m_pShader->setBool("hasBitangent", mesh->IsBitangentEnable());
    }
}

void CRender::SetSelected(IModelInterface* model)
{
    if (0 != m_pShader && 0 != model) {
        m_pShader->setBool("hasSelected", model->GetID() == m_nSelectedId);
    }
}