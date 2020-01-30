#pragma once
#include "Main.h"
#include "Struct.h"

//�I�u�W�F�N�g�@�p���p�N���X(�Q�[���I�u�W�F�N�g�S�ĂɌp��������)
class CObjBase
{
public:
	virtual void Init() = 0;	//������
	virtual void Update() = 0;	//�X�V
	virtual void Draw() = 0;	//�`��
	virtual void Release() = 0;	//�J��

	void SetId(int Id)				{ m_Id = Id; }
	void SetDelete(bool isDelete)	{ m_bDelete = isDelete; }
	int	 GetId()					{ return m_Id; }
	bool GetDelete()				{ return m_bDelete; }
private:
	int m_Id;		//�I�u�W�F�N�gID
	bool m_bDelete;	//�폜�t���O
};

//2D�I�u�W�F�N�g�@�p���p�N���X
class CObj2DBase :public CObjBase
{
protected:
	D3DXVECTOR2 m_ScreenPos;	//�`��ʒu(����)
	D3DXVECTOR2 m_Move;			//�ړ��x�N�g��
	float		m_fRad;			//��]

	CColorData m_Color;	//�`��F

	//�e�N�X�`��
	RECT_F	m_Src;//�؂���
	RECT_F	m_Dst;//�`��
};

//3D���f�����g�p������́@�p���p�N���X
class CObj3DBase :public CObjBase
{
public :
	D3DXVECTOR3 GetPos()	{ return m_vPos;	}
	D3DXVECTOR3 GetAngle()	{ return m_vAngle;	}
	D3DXVECTOR3 GetScale()	{ return m_vScale;	}
	D3DXMATRIX  GetWorld()	{ return m_matWorld;}

protected:

	//�x�N�g��
	D3DXVECTOR3 m_vPos;		//�ʒu
	D3DXVECTOR3 m_vAngle;	//��]
	D3DXVECTOR3 m_vScale;	//�傫��

	//�s��
	D3DXMATRIX m_matWorld;	//���[���h
	D3DXMATRIX m_matTrans;	//�ړ�
	D3DXMATRIX m_matRot;	//��]
	D3DXMATRIX m_matScale;	//�g��E�k��
};