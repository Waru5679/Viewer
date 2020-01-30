#pragma once

//�K�v�ȃw�b�_�[�t�@�C���̃C���N���[�h
#include "LibraryMain.h"
#include "LibraryHeader.h"

//���C�u�������C��
#define LIB_MAIN CLibraryMain::GetInstance()

//�A�v���P�[�V�����̃G���g���[�֐�
INT WINAPI WinMain( HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR szStr,INT iCmdShow)
{
	//���������[�N���o
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//������
	LIB_MAIN->Init(hInst);

	// ���b�Z�[�W���[�v
	MSG msg;
    ZeroMemory( &msg, sizeof(msg) );

	while( msg.message!=WM_QUIT )
	{
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
	    {
			TranslateMessage( &msg );
			DispatchMessage( &msg );
	    }
	    else
		{
			//�X�V
			LIB_MAIN->Update();
		}				
	}
	return (INT)msg.wParam;
}

// �E�B���h�E�v���V�[�W���[�֐�
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
{	
	//�}�E�X���X�V
	INPUT->Update(&iMsg,&lParam);

	switch(iMsg)
	{
		//x��
		case WM_DESTROY:
		{
			LIB_MAIN->Release();	//���������

			PostQuitMessage(0);
			break;
		}
		case WM_KEYDOWN:
		{
			switch (wParam)
			{
				//ESC
				case VK_ESCAPE:
				{
					LIB_MAIN->Release();	//���������

					PostQuitMessage(0);
					break;
				}
			}
		}
		break;		
	}
	return DefWindowProc (hWnd, iMsg, wParam, lParam);	
}
