#pragma once

//���C�u�����t�@�C��
#include "../../Library/Camera.h"

//���C���J�����N���X
class CMainCamera :public CCameraBase
{
public:
	void Init();	//������
	void Update();	//�X�V
	void Draw() {};	//�`��
	void Release() {};	//�J��

private:
	float m_fCameraDis;	//�v���C���[�ƃJ�����̋���
	float m_fLookDis;	//�v���C���[�ƒ����_�̋���

};