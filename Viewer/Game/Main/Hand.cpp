#include "Hand.h"
#include "../GameHeader.h"

//�R���X�g���N�^
CHand::CHand(D3DXVECTOR3 vPos, D3DXVECTOR3 vAngle, D3DXVECTOR3 vScale)
{
	m_vPos = vPos;
	m_vAngle = vAngle;
	m_vScale = vScale;
}

//������
void CHand::Init()
{
	//���f���擾
	m_pSkinMesh = MODEL->GetModelData(ModelName::ModelHand);

	//���[���h�s��쐬
	m_matWorld=MakeMatWorld(m_vPos, m_vAngle, m_vScale);
	
	//�t���[��������
	m_Frame=0;
}

//�X�V
void CHand::Update()
{
	//�t���[���X�V
	m_Frame+=15;
	if (m_Frame >= 9000)
		m_Frame = 0;

	//�A�j���[�V����
	for (int i = 0; i < m_pSkinMesh->m_AnimeNum; i++)
	{
		SKIN->Animation(i, m_Frame, m_pSkinMesh);
	}

	//�}�E�X�N���b�N�e�X�g�I
	if (INPUT->MousLeftCrick())
	{
		int a = 0;
	}
	if (INPUT->MousRightCrick())
	{
		int b = 0;
	}
}

//�`��
void CHand::Draw()
{
	//��
	SKIN->DrawMesh(m_matWorld, m_pSkinMesh, nullptr);	
}

//�J��
void CHand::Release()
{
	m_pSkinMesh = nullptr;
}