#include "ModelManager.h"
#include "LibraryHeader.h"

//インスタンス
CModelManager* CModelManager::m_pInstance = nullptr;

//モデル読み込み
void CModelManager::LoadModel(int Id, const char* Name)
{
	ModelData data;

	//読み込み
	SKIN->LoadSkinMesh(Name, &data.m_SkinMesh);

	//管理ID
	data.m_Id = Id;

	//登録
	m_SkinMesh.push_back(data);
}

//モデル取得
X_SKIN_MESH* CModelManager::GetModelData(int Id)
{
	for (int i = 0; i < m_SkinMesh.size(); i++)
	{
		//Id確認
		if (m_SkinMesh[i].m_Id == Id)
		{
			return &m_SkinMesh[i].m_SkinMesh;
		}
	}
	return nullptr;
}

//開放
void CModelManager::Release()
{
	//インスタンス破棄
	PointerRelease(m_pInstance);
}