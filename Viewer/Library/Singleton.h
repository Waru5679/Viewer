#pragma once
template <class CLASS>

//�V���O���g���N���X
class CSingleton
{
public:
	//�C���X�^���X�擾
	static CLASS* GetInstance()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new CLASS();
		}
		return m_pInstance;
	}
protected:
	//�C���X�^���X
	static CLASS* m_pInstance;
};