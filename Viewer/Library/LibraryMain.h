#pragma once
#include "LibraryHeader.h"

//���C�u�������C���N���X
class CLibraryMain:public CSingleton<CLibraryMain>
{
private:
	//�V���O���g��
	friend CSingleton<CLibraryMain>;	
	CLibraryMain() {};	//�R���X�g���N�^
	~CLibraryMain() {};	//�f�X�g���N�^
public:
	//������
	bool Init(HINSTANCE hInst);

	//�X�V
	void Update();

	//�������̊J��
	void Release();	
private:	
	//window�����ݒ�
	void WinInit(HINSTANCE hInst);

	//�E�C���h�E�n���h��
	HWND m_hWnd;
};