#include "Scene.h"
#include "Task.h"
#include "Release.h"

//インスタンス
CSceneManager* CSceneManager::m_pInstance=nullptr;

//登録
void CSceneManager::Insert(CSceneBase* pScene, int Id)
{
	pMyScene my_scene;
	my_scene.reset( new MY_SCENE(pScene, Id));

	m_Scene.push_back(move(my_scene));
}

//シーン取得
CSceneBase* CSceneManager::GetScene(int Id)
{
	for (unsigned int i = 0; i < m_Scene.size(); i++)
	{
		if (m_Scene[i]->m_Id == Id)
			return m_Scene[i]->m_pScene;
	}
	return nullptr;
}

//シーンセット
void CSceneManager::SetScene(int n)
{
	//現在のシーンの開放
	if(m_pNowScene!=nullptr)
		m_pNowScene->Release();

	//次のシーンの呼び出し
	m_pNowScene = GetScene(n);

	//次のシーンの初期化
	m_pNowScene->Init();
}

//シーンの更新
void CSceneManager::Update()
{
	m_pNowScene->Update();
}

//シーンの描画
void CSceneManager::Draw()
{
	m_pNowScene->Draw();
}

//解放
void CSceneManager::Release()
{
	//現在のシーン破棄
	if (m_pNowScene != nullptr)
	{
		m_pNowScene->Release();
		PointerRelease(m_pNowScene);
	}

	//シーンリスト破棄
	VectorRelease(m_Scene);

	//インスタンス破棄
	PointerRelease(m_pInstance);
}

