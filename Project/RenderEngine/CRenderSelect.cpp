#include "pch.h"
#include "CRenderSelect.h"


CRenderSelect::CRenderSelect(IShaderInterface* shader, ICameraInterface* camera): CRender(shader, camera)
{

}

void CRenderSelect::RenderModel(IModelInterface* model)
{
    if (0 == model || 0 == m_pShader || 0 == m_pCamera) {
        return;
    }
    m_pShader->setMat4("model", model->GetModel());
    SetRenderId(model);
    for (size_t index = 0; index < model->GetMeshCount(); ++index) {
        RenderMesh(model->GetMesh(index));
    }
}

void CRenderSelect::RenderMesh(IMeshInterface* mesh)
{
    if (0 == mesh) {
        return;
    }

    // draw mesh
    glBindVertexArray(mesh->GetVAO());
    glDrawElements(mesh->GetType(), static_cast<unsigned int>(mesh->GetIndexSize()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}

void CRenderSelect::SetRenderId(IModelInterface* model)
{
    if (0 == model || 0 == m_pShader) {
        return;
    }
    unsigned int ID = model->GetID();
    float v3 = (float)((ID >> 24) & 0xFF);
    float v2 = (float)((ID >> 16) & 0xFF);
    float v1 = (float)((ID >> 8) & 0xFF);
    float v0 = (float)(ID & 0xFF);
    m_pShader->setVec4("objectID", glm::vec4(v0 / 255, v1 / 255, v2 / 255, v3 / 255));
}