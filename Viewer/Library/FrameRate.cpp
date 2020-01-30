#include "LibraryHeader.h"
#include "Release.h"

//�C���X�^���X
CFrameRate* CFrameRate::m_pInstance = nullptr;

//������
void CFrameRate::Init()
{
	//���x���グ��
	timeBeginPeriod(1);

	//�J�n�����擾
	m_StartTime = timeGetTime();

	//���x��߂�
	timeEndPeriod(1);
		
	//�`�掞��
	m_DrawTime = m_StartTime;
		
	//�J�E���^
	m_Count = 0;

	//FPS
	m_FrameRate = 0;

	//�\���p������J�E���^
	constexpr int ARRAY_SIZE{ 12 };
	m_pDrawStr = new wchar_t[ARRAY_SIZE];
}

//�t���[�����[�g�̌v�Z
void CFrameRate::FrameCount()
{
	//�J�E���^�X�V
	m_Count++;
	
	//���x�グ��
	timeBeginPeriod(1);

	//���ݎ����擾
	m_NowTime = timeGetTime();
	
	//���̃t���[���ł�����������
	float FrameTime = static_cast<float>(m_NowTime - m_StartTime);

	//���Ԃɗ]�T������Ƃ�
	if (FrameTime < MIN_FRAME_TIME)
	{
		//�]���Ȏ���
		DWORD SleepTime = static_cast<DWORD>((MIN_FRAME_TIME - FrameTime));

		//�ҋ@
		Sleep(SleepTime);

		//���ݎ����̍X�V
		m_NowTime = timeGetTime();
	}

	//���x��߂�
	timeEndPeriod(1);

	//�J�n�����X�V
	m_StartTime = m_NowTime;

	//�O��̕`�悩��1�b�o�߂��Ă�Ƃ�
	float draw_time = static_cast<float>(m_NowTime - m_DrawTime);
	if ( draw_time >= (FRAME_RATE*MIN_FRAME_TIME))
	{
		//�`�掞���X�V
		m_DrawTime = m_NowTime;

		//�t���[�����[�g�X�V
		m_FrameRate = m_Count;

		//�J�E���^������
		m_Count = 0;
	}
}

//�`��
void CFrameRate::Draw()
{
	//������ɕϊ�����
	_itow_s(m_FrameRate, m_pDrawStr,sizeof(m_pDrawStr), 10);

	//�`��
	FONT->DrawStr(m_pDrawStr, 0.0f, 0.0f, 32.0f, 0.0f);
}

//���
void CFrameRate::Release()
{
	//�\���p������j��
	PointerRelease(m_pDrawStr);

	//�C���X�^���X�j��
	PointerRelease(m_pInstance);
}