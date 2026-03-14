#pragma once

#include <glad/glad.h>
#include <CUtils.h>
#include "IShaderInterface.h"
#include "IModelInterface.h"
#include "IMeshInterface.h"
#include "ICameraInterface.h"


class CRender
{
	friend class CRenderManager;
protected:
	CRender(IShaderInterface* shader, ICameraInterface* camera);
private:
	virtual void RenderModel(IModelInterface* model);
	virtual void RenderMesh(IMeshInterface* mesh);
public:
	void Render(IModelInterface* model);
public:
	void SetSelectId(unsigned int id);
private:
	void SetTexture(IMeshInterface* mesh);
	void SetEnable(IMeshInterface* mesh);
	void SetSelected(IModelInterface* model);
protected:
	IShaderInterface* m_pShader;
	ICameraInterface* m_pCamera;
private:
	unsigned int	m_nSelectedId;
};

