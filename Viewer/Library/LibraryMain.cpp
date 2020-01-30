#include "LibraryMain.h"
#include "../Game/GameHeader.h"

//�C���X�^���X
CLibraryMain* CLibraryMain::m_pInstance = nullptr;

//������
bool CLibraryMain::Init(HINSTANCE hInst)
{
	//�E�C���h�E�n���h��
	m_hWnd = nullptr;

	//window������
	WinInit(hInst);

	// DirectX�̏������֐����Ă�
	if (DX->Init(m_hWnd)==false)
	{
		return false;
	}

	//�V�F�[�_�[������
	if (SHADER->Init(DX->GetDevice()) == false)
	{
		return false;
	}

	//�X�L�����b�V���p�V�F�[�_������
	if (SKIN_SHADER->Init(DX->GetDevice()) == false)
	{
		return false;
	}

	//�`��
	DRAW->Init();

	//�t�H���g
	FONT->Init();

	//���y
	AUDIO->Init();

	//�t���[�����[�g
	FRAME->Init();

	//���͏��
	INPUT->Init();

	//�V�[�����[�h
	CSceneInclude::LoadScene();

	//�X�^�[�g�V�[���Z�b�g
	SCENE->SetScene(START_SCENE);

	return true;
}

//�X�V
void CLibraryMain::Update()
{
	//��ʃN���A (RGBA)
	float ClearColor[4] = { 0.0f,0.0f,0.0f,1.0f };
	DX->GetDevice()->ClearRenderTargetView(DX->GetRenderTargetView(), ClearColor);

	//�[�x�o�b�t�@�N���A
	DX->GetDevice()->ClearDepthStencilView(DX->GetDepthStencilView(), D3D10_CLEAR_DEPTH, 1.0f, 0);

	//�t���[�����[�g�v�Z
	FRAME->FrameCount();

	//�X�V
	SCENE->Update();

	//�`��
	SCENE->Draw();
	
	//�t���[�����[�g���`��
	FRAME->Draw();

	//��ʍX�V�i�o�b�N�o�b�t�@���t�����g�o�b�t�@�Ɂj
	DX->GetSwapChain()->Present(0, 0);
}

//�E�C���h�E�̏����ݒ�
void CLibraryMain::WinInit(HINSTANCE hInst)
{
	// �E�B���h�E�̐ݒ�
	WNDCLASSEX wndclass;

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInst;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
	wndclass.hIconSm = LoadIcon(NULL, IDI_ASTERISK);
	RegisterClassEx(&wndclass);

	//�E�C���h�E�쐬
	m_hWnd = CreateWindow(szAppName, szAppName, WS_OVERLAPPEDWINDOW,
		0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInst, NULL);

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);
}

//�������̊J��
void  CLibraryMain::Release()
{
	TASK->Release();
	INPUT->Release();
	FRAME->Release();
	SCENE->Release();
	AUDIO->Release();
	FONT->Release();
	DRAW->Release();
	SKIN_SHADER->Release();
	SHADER->Release();
	DX->Release();
	
	//window�n���h��
	m_hWnd = nullptr;

	//�C���X�^���X�j��
	PointerRelease(m_pInstance);
}