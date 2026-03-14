#ifndef __COBJECTMANAGER_H__
#define __COBJECTMANAGER_H__

#include "CUtils.h"
#include "CModel.h"

class CObjectManager
{
public:
	static CObjectManager& GetInstance();

public:
	CModel* GetModelFromFile(const std::string& strPath);
	CModel* GetCoordModel(float size = 1.0f);
	CModel* GetChessModel(float size = 10.0f);
	CModel* GetScreenModel();
};



#endif // __COBJECTMANAGER_H__