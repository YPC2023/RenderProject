#pragma once

#include "Define.h"
#include "ISingletonInterface.h"
#include "CEngine.h"

class ENGINE_API_ACTION CEngineManager : public ISingletonInterface<CEngineManager>
{
public:
	CEngine& GetEngine();
};

