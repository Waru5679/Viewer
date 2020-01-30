#pragma once

#include "Main.h"
#include "Camera.h"
#include "Struct.h"
#include "Singleton.h"

struct VERTEX
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vNorm;
	D3DXVECTOR2 vUv;
};

//�`��N���X
class CDraw:public CSingleton<CDraw>
{
private:
	//�V���O���g��
	friend CSingleton<CDraw>;
	CDraw() {};		//�R���X�g���N�^
	~CDraw() {};	//�f�X�g���N�^
public:	
	void Init();	//������
	void Release();	//���

	//�o�b�t�@�쐬
	ID3D10Buffer* BufferCreate(void* pHead, unsigned int Size, int BufferType);

	//2D�`��(�e�N�X�`���̃T�C�Y����؂���ʒu�̐ݒ肪������̗p���p)
	void DrawTexture(int TexId, RECT_F* pSrc, RECT_F* pOut, CColorData* pColor, float fRad);

	//2D�`��
	void DrawTexture(ID3D10ShaderResourceView* pTex, RECT_F* pSrc, RECT_F* pOut, CColorData* pColor, float fRad);
	
	//�J�����Z�b�g
	void SetCamera(CCameraBase* pCamera) { m_pCamera = pCamera; }

	//�|���S���`��
	void DrawPolygon(int VerNum, ID3D10Buffer* VertexBuffer, ID3D10Buffer* IndexBuffer);
private:
	
	ID3D10Buffer* m_pVertexBuffer;//�o�[�e�b�N�X�o�b�t�@
	  	
	CCameraBase* m_pCamera;	//�J�����|�C���^
};