#pragma once
#include <d3dx10.h>
#include <D3D10.h>
#include <list>
#include "Singleton.h"
using namespace std;

//�e�N�X�`���f�[�^
class CTextureData
{
public:
	//�R���X�g���N�^
	CTextureData(ID3D10ShaderResourceView* pTex,int Width,int Height,int Id)
	{
		m_pTex = pTex;
		m_Width = Width;
		m_Height = Height;
		m_Id = Id;
	}

	//�J��
	void Release()
	{
		m_pTex->Release();
		m_pTex = nullptr;
	}

	//�e�N�X�`���擾
	ID3D10ShaderResourceView* GetTexture() { return m_pTex; }

	//�����擾
	int GetWidth() { return m_Width; }

	//���擾
	int GetHeight() { return m_Height; }

	//ID�擾
	int GetId() { return m_Id; }
private:
	//�e�N�X�`��
	ID3D10ShaderResourceView* m_pTex;
	int m_Width;	//��
	int m_Height;	//����
	int m_Id;		//�Ǘ��pID
};

//�e�N�X�`���Ǘ��N���X
class CTextureManager:public CSingleton<CTextureManager>
{
private:
	//�V���O���g��
	friend CSingleton<CTextureManager>;
	CTextureManager() {};	//�R���X�g���N�^
	~CTextureManager() {};	//�f�X�g���N�^
public:
	//�ǂݍ���
	void Load(int id,const char* name,int width,int height);
	
	//�e�N�X�`���擾
	CTextureData* GetTexData(int Id);

	//�J��
	void Release();
private:
	list<CTextureData*>m_list;	//�e�N�X�`�����X�g
};