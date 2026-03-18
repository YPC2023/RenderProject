#include "CModelManager.h"

CModelManager& CModelManager::GetInstance()
{
	static CModelManager Instance;
	return Instance;
}

CModel* CModelManager::GetModel()
{
	CModel* pModel = new CModel();
	if (!pModel->IsOK()) {
		delete pModel;
		return 0;
	}
	m_map_Model[pModel->GetID()] = pModel;
	return pModel;
}

CModel* CModelManager::GetModelFromFile(const std::string& strPath)
{
	CModel* pModel = GetModel();
	if (pModel) {
		pModel->SetPath(strPath);
	}
	return pModel;
}

void CModelManager::EnableRenderId(bool bEnable)
{
	CModel::EnableRenderId(bEnable);
}

void CModelManager::SelectedModel(unsigned int id)
{
	CModel::SelectModel(id);
}

CModel* CModelManager::FindModel(unsigned int id)
{
	if (m_map_Model.count(id)) {
		return m_map_Model[id];
	}
	return 0;
}
