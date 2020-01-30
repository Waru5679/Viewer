#pragma once

#include <d3dx10.h>
#include <vector>
#include "Singleton.h"
#include "XLoader_Skin.h"

using namespace std;

struct ModelData
{
	X_SKIN_MESH m_SkinMesh;	//���b�V��
	int			m_Id;		//ID
};

//�I�u�W�F�N�g�Ǘ��N���X
class CModelManager :public CSingleton<CModelManager>
{
private:
	//�V���O���g��
	friend CSingleton<CModelManager>;
	CModelManager(){};		//�R���X�g���N�^
	~CModelManager() {};	//�f�X�g���N�^
public:

	//���f���ǂݍ���
	void LoadModel(int Id, const char* Name);

	//���f���擾
	X_SKIN_MESH* GetModelData(int Id);

	//�J��
	void Release();

private:
	vector<ModelData> m_SkinMesh;	//�X�L�����b�V��
};