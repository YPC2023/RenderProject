#include "pch.h"
#include "CRenderManager.h"

CRender* CRenderManager::CreateRender(IShaderInterface* shader, ICameraInterface* camera, E_RENDER_TYPE type)
{
	if (0 < m_map_Render.count(type)) {
		return m_map_Render[type];
	}
	CRender* pRender = 0;
	if (E_RENDER_SELECT == type) {
		pRender = new CRenderSelect(shader, camera);
	}
	else {
		pRender = new CRender(shader, camera);
	}
	if (0 == pRender) {
		PRINTLOG("Fail to new Render");
		return 0;
	}
	m_map_Render[type] = pRender;
	return pRender;
}