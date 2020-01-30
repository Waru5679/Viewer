#include "Task.h"
#include "Release.h"

//インスタンス
CTask* CTask::m_pInstance = nullptr;

//オブジェクト登録
void CTask::InsertObj(CObjBase* pObj, int Id)
{
	m_Obj.push_back(pObj);
	pObj->SetId(Id);
	pObj->SetDelete(false);
	pObj->Init();
}

//更新
void CTask::Update()
{
	//処理順にソート
	void Sort();

	//オブジェクト更新
	for (unsigned int i=0;i < m_Obj.size(); i++)
	{
		m_Obj[i]->Update();

		//削除
		if (m_Obj[i]->GetDelete() == true)
		{
			//開放処理
			m_Obj[i]->Release();

			m_Obj.erase(m_Obj.begin()+i);
			i--;
		}
	}
}

//描画
void CTask::Draw()
{
	//OBJ描画
	for (auto itr = m_Obj.begin(); itr != m_Obj.end(); itr++)
	{
		(*itr)->Draw();
	}
}

//オブジェクト取得
CObjBase* CTask::GetObj(int Id)
{
	for(auto itr = m_Obj.begin(); itr != m_Obj.end(); itr++)
	{
		if ((*itr)->GetId() == Id)
			return (*itr);
	}
	return nullptr;
}

//処理順にソートする
void CTask::Sort()
{
	for (unsigned int i = 0; i < m_Obj.size() - 1; i++)
	{
		for (unsigned int j = i + 1; j < m_Obj.size(); j++)
		{
			//順番が違うとき入れ替え
			if (m_Obj[i]->GetId() > m_Obj[j]->GetId())
			{
				CObjBase* tmp = m_Obj[i];
				m_Obj[i] = m_Obj[j];
				m_Obj[j] = tmp;
			
				tmp = nullptr;
			}
		}
	}
}

//タスク情報クリア
void CTask::ClearTask()
{
	for (unsigned int i = 0; i < m_Obj.size(); i++)
	{
		m_Obj[i]->Release();
	}
	VectorRelease(m_Obj);
}

//メモリの開放
void CTask::Release()
{
	//インスタンス破棄
	PointerRelease(m_pInstance);
}