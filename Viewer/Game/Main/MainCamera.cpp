//���C�u�����t�@�C��
#include "../../Library/LibraryHeader.h"

//�Q�[���t�@�C��
#include "../GameHeader.h"

//������
void CMainCamera::Init()
{
	//�����J�����̈ʒu
	m_vEye = D3DXVECTOR3(0.0f, 3.0f, -20.0f);
	m_vLook = D3DXVECTOR3(0.0f, 2.0f, 1.0f);
	m_vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//�s�񏉊���
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);

	//�J��������
	m_fCameraDis = 3.0f;
	
	//�����_����
	m_fLookDis = 3.0f;

	//�p�x
	m_vAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//�X�V�֐�
void CMainCamera::Update()
{
	//��]
	if (INPUT->GetKeyPush(VK_LEFT) == true)
	{
		m_vAngle.y -= 0.05f;
	}

	if (INPUT->GetKeyPush(VK_RIGHT) == true)
	{
		m_vAngle.y += 0.05f;
	}

	//���_�V�F�[�_�p�̃}�g���b�N�X�쐬
	CreateVSMatrix(&m_vEye, &m_vLook, &m_vUp, &m_matView, &m_matProj, WINDOW_WIDTH, WINDOW_HEIGHT);
}
