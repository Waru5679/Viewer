#include "Input.h"
#include "Release.h"
#include "LibraryHeader.h"

//�C���X�^���X
CInput* CInput::m_pInstance = nullptr;

//������
void CInput::Init()
{
	//�}�E�X�ʒu
	m_Mous_x = 0;
	m_Mous_y = 0;

	//�}�E�X���E�{�^��
	m_bMousLeft = false;
	m_bMousRight = false;

	//�L�[��񏉊���
	for (int i = 0; i < KEY_BOARD_ARRAY_SIZE; i++)
		m_Key[i] = false;
}

//�X�V
void CInput::Update(UINT* uMsg, LPARAM* lParam)
{
	switch (*uMsg)
	{
		//�}�E�X���삵���Ƃ�
		case WM_MOUSEMOVE:
		{
			POINT point = { LOWORD(*lParam),HIWORD((*lParam)) };

			m_Mous_x = point.x;	//�J�[�\����x���W
			m_Mous_y = point.y;	//�J�[�\����y���W

			break;
		}
		//���N���b�N
		case WM_LBUTTONDOWN:
		{
			m_bMousLeft = true;
			break;
		}
		//���N���b�N���Ă��Ȃ���
		case WM_LBUTTONUP:
		{
			m_bMousLeft = false;
			break;
		}
		//�E�N���b�N
		case WM_RBUTTONDOWN:
		{
			m_bMousRight = true;
			break;
		}
		//�E�N���b�N���Ă��Ȃ���
		case WM_RBUTTONUP:
		{
			m_bMousRight = false;
			break;
		}
		default:
			break;
	}
}

//�`��
void CInput::Draw()
{	
}

//�L�[�E�}�E�X�{�^���̃v�b�V���m�F
//�{�^����������Ă����true,������Ă��Ȃ����false��Ԃ�
bool CInput::GetKeyPush(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		m_Key[key] = true;
		return true;
	}
	m_Key[key] = false;
	return false;
}

//�L�[�E�}�E�X�{�^���̃v�b�V���m�F (�g���K�[��)
//�V���������ꂽ�Ȃ�true,
//���łɉ�����Ă����@�܂��́@������Ă��Ȃ��Ȃ� false�@
bool CInput::GetTriggerKeyPush(int key)
{
	//�L�[��������Ă��Ă�ꍇ
	if ( (GetAsyncKeyState(key) & 0x8000) ) 
	{
		//�L�[�̃t���O���I�t�Ȃ�
		if (m_Key[key] == false)
		{
			m_Key[key] = true;
			return true;
		}
	}
	//�L�[��������Ă��Ȃ��ꍇ
	else
	{
		m_Key[key] = false;
	}
	return false;
}



//���
void CInput::Release()
{
	//�C���X�^���X�j��
	PointerRelease(m_pInstance);
}