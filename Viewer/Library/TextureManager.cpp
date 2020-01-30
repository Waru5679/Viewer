#include "TextureManager.h"
#include "LibraryHeader.h"
#include "Release.h"

//インスタンス
CTextureManager* CTextureManager::m_pInstance = nullptr;

//読み込み
void CTextureManager::Load(int id, const char* name, int width, int height)
{
	//テクスチャーを作成
	ID3D10ShaderResourceView* pTex;
	D3DX10CreateShaderResourceViewFromFileA(DX->GetDevice(), name, NULL, NULL, &pTex, NULL);

	//リスト登録
	m_list.push_back(new CTextureData(pTex, width, height, id));
}

//テクスチャ取得
CTextureData* CTextureManager::GetTexData(int Id)
{
	for (auto itr = m_list.begin(); itr != m_list.end(); itr++)
	{
		if ((*itr)->GetId()== Id)
			return (*itr);
	}
	return nullptr;
}

//開放
void CTextureManager::Release()
{
	for (auto itr = m_list.begin(); itr != m_list.end(); itr++)
		(*itr)->Release();
	m_list.clear();
}