#include "pch.h"
#include "CSceneManager.h"

CScene& CSceneManager::GetMainScene()
{
	static CScene Instance;
	return Instance;
}