#ifndef __CMODELMANAGER_H__
#define __CMODELMANAGER_H__

#include <map>
#include "CUtils.h"
#include "CModel.h"

class CModelManager
{
public:
	static CModelManager& GetInstance();

public:
	CModel* GetModel();
	CModel* GetModelFromFile(const std::string& strPath);
public:
	void EnableRenderId(bool bEnable = true);
	void SelectedModel(unsigned int id);
	CModel* FindModel(unsigned int id);
private:
	std::map<unsigned int, CModel*>			m_map_Model;
};




#endif // __CMODELMANAGER_H__