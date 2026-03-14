#include "pch.h"
#include "CEngineManager.h"

CEngine& CEngineManager::GetEngine()
{
	static CEngine Instance;
	return Instance;
}