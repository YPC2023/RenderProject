#pragma once

#include "Config.h"
#include "ISingletonInterface.h"
#include <map>

class ConfigManager : public ISingletonInterface<ConfigManager>
{
public:
	typedef enum _E_CONFIG_TYPE
	{
		E_CONFIG_NORMAL,

	}E_CONFIG_TYPE;
public:
	Config* CreateConfig(E_CONFIG_TYPE type = E_CONFIG_NORMAL);
	
private:
	std::map<E_CONFIG_TYPE, Config*>	m_map_Config;
};

