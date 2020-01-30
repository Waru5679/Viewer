#include "ModelManager.h"
#include "LibraryHeader.h"

//�C���X�^���X
CModelManager* CModelManager::m_pInstance = nullptr;

//���f���ǂݍ���
void CModelManager::LoadModel(int Id, const char* Name)
{
	ModelData data;

	//�ǂݍ���
	SKIN->LoadSkinMesh(Name, &data.m_SkinMesh);

	//�Ǘ�ID
	data.m_Id = Id;

	//�o�^
	m_SkinMesh.push_back(data);
}

//���f���擾
X_SKIN_MESH* CModelManager::GetModelData(int Id)
{
	for (int i = 0; i < m_SkinMesh.size(); i++)
	{
		//Id�m�F
		if (m_SkinMesh[i].m_Id == Id)
		{
			return &m_SkinMesh[i].m_SkinMesh;
		}
	}
	return nullptr;
}

//�J��
void CModelManager::Release()
{
	//�C���X�^���X�j��
	PointerRelease(m_pInstance);
}