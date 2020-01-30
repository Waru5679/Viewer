#pragma once
template <class CLASS>

//シングルトンクラス
class CSingleton
{
public:
	//インスタンス取得
	static CLASS* GetInstance()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new CLASS();
		}
		return m_pInstance;
	}
protected:
	//インスタンス
	static CLASS* m_pInstance;
};