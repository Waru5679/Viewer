#include "TextureManager.h"
#include "LibraryHeader.h"
#include "Release.h"

//�C���X�^���X
CTextureManager* CTextureManager::m_pInstance = nullptr;

//�ǂݍ���
void CTextureManager::Load(int id, const char* name, int width, int height)
{
	//�e�N�X�`���[���쐬
	ID3D10ShaderResourceView* pTex;
	D3DX10CreateShaderResourceViewFromFileA(DX->GetDevice(), name, NULL, NULL, &pTex, NULL);

	//���X�g�o�^
	m_list.push_back(new CTextureData(pTex, width, height, id));
}

//�e�N�X�`���擾
CTextureData* CTextureManager::GetTexData(int Id)
{
	for (auto itr = m_list.begin(); itr != m_list.end(); itr++)
	{
		if ((*itr)->GetId()== Id)
			return (*itr);
	}
	return nullptr;
}

//�J��
void CTextureManager::Release()
{
	for (auto itr = m_list.begin(); itr != m_list.end(); itr++)
		(*itr)->Release();
	m_list.clear();
}