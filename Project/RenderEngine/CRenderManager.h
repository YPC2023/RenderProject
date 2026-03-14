#pragma once

#include <CUtils.h>
#include "CRender.h"
#include "CRenderSelect.h"
#include <ISingletonInterface.h>
#include <map>

class CRenderManager : public ISingletonInterface<CRenderManager>
{
public:
	typedef enum _E_RENDER_TYPE
	{
		E_RENDER_NORMAL,
		E_RENDER_SELECT,
	}E_RENDER_TYPE;
public:
	CRender* CreateRender(IShaderInterface* shader, ICameraInterface* camera, E_RENDER_TYPE type = E_RENDER_NORMAL);

private:
	std::map<E_RENDER_TYPE, CRender*>	m_map_Render;
};

