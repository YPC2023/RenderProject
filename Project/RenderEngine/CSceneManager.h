#pragma once

#include "CScene.h"
#include "ISingletonInterface.h"

class CSceneManager : public ISingletonInterface<CSceneManager>
{
public:
	CScene& GetMainScene();
};

