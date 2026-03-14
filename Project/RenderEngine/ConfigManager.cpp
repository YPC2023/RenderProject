#include "pch.h"
#include "ConfigManager.h"



Config* ConfigManager::CreateConfig(E_CONFIG_TYPE type)
{
	if (0 < m_map_Config.count(type)) {
		return m_map_Config[type];
	}
	Config* pConfig = new Config();
	if (0 == pConfig) {
		PRINTLOG("Fail to new Config");
		return 0;
	}
	m_map_Config[type] = pConfig;
	return pConfig;
}

