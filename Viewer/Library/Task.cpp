#include "Task.h"
#include "Release.h"

//�C���X�^���X
CTask* CTask::m_pInstance = nullptr;

//�I�u�W�F�N�g�o�^
void CTask::InsertObj(CObjBase* pObj, int Id)
{
	m_Obj.push_back(pObj);
	pObj->SetId(Id);
	pObj->SetDelete(false);
	pObj->Init();
}

//�X�V
void CTask::Update()
{
	//�������Ƀ\�[�g
	void Sort();

	//�I�u�W�F�N�g�X�V
	for (unsigned int i=0;i < m_Obj.size(); i++)
	{
		m_Obj[i]->Update();

		//�폜
		if (m_Obj[i]->GetDelete() == true)
		{
			//�J������
			m_Obj[i]->Release();

			m_Obj.erase(m_Obj.begin()+i);
			i--;
		}
	}
}

//�`��
void CTask::Draw()
{
	//OBJ�`��
	for (auto itr = m_Obj.begin(); itr != m_Obj.end(); itr++)
	{
		(*itr)->Draw();
	}
}

//�I�u�W�F�N�g�擾
CObjBase* CTask::GetObj(int Id)
{
	for(auto itr = m_Obj.begin(); itr != m_Obj.end(); itr++)
	{
		if ((*itr)->GetId() == Id)
			return (*itr);
	}
	return nullptr;
}

//�������Ƀ\�[�g����
void CTask::Sort()
{
	for (unsigned int i = 0; i < m_Obj.size() - 1; i++)
	{
		for (unsigned int j = i + 1; j < m_Obj.size(); j++)
		{
			//���Ԃ��Ⴄ�Ƃ�����ւ�
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

//�^�X�N���N���A
void CTask::ClearTask()
{
	for (unsigned int i = 0; i < m_Obj.size(); i++)
	{
		m_Obj[i]->Release();
	}
	VectorRelease(m_Obj);
}

//�������̊J��
void CTask::Release()
{
	//�C���X�^���X�j��
	PointerRelease(m_pInstance);
}