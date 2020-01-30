#include "Scene.h"
#include "Task.h"
#include "Release.h"

//�C���X�^���X
CSceneManager* CSceneManager::m_pInstance=nullptr;

//�o�^
void CSceneManager::Insert(CSceneBase* pScene, int Id)
{
	pMyScene my_scene;
	my_scene.reset( new MY_SCENE(pScene, Id));

	m_Scene.push_back(move(my_scene));
}

//�V�[���擾
CSceneBase* CSceneManager::GetScene(int Id)
{
	for (unsigned int i = 0; i < m_Scene.size(); i++)
	{
		if (m_Scene[i]->m_Id == Id)
			return m_Scene[i]->m_pScene;
	}
	return nullptr;
}

//�V�[���Z�b�g
void CSceneManager::SetScene(int n)
{
	//���݂̃V�[���̊J��
	if(m_pNowScene!=nullptr)
		m_pNowScene->Release();

	//���̃V�[���̌Ăяo��
	m_pNowScene = GetScene(n);

	//���̃V�[���̏�����
	m_pNowScene->Init();
}

//�V�[���̍X�V
void CSceneManager::Update()
{
	m_pNowScene->Update();
}

//�V�[���̕`��
void CSceneManager::Draw()
{
	m_pNowScene->Draw();
}

//���
void CSceneManager::Release()
{
	//���݂̃V�[���j��
	if (m_pNowScene != nullptr)
	{
		m_pNowScene->Release();
		PointerRelease(m_pNowScene);
	}

	//�V�[�����X�g�j��
	VectorRelease(m_Scene);

	//�C���X�^���X�j��
	PointerRelease(m_pInstance);
}

