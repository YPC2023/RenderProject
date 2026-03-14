#pragma once

#include "CRender.h"

class CRenderSelect : public CRender
{
	friend class CRenderManager;
protected:
	CRenderSelect(IShaderInterface* shader, ICameraInterface* camera);
protected:
	void RenderModel(IModelInterface* model) override;
	void RenderMesh(IMeshInterface* mesh) override;
private:
	void SetRenderId(IModelInterface* model);
};

