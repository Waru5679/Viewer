//���C�u�����t�@�C��
#include "../../Library/LibraryHeader.h"

//�Q�[���t�@�C��
#include "../GameHeader.h"

//������
void CTitleObj::Init()
{
	//�ʒu
	m_ScreenPos=D3DXVECTOR2(120.0f,90.0f);
	
	//�ړ�
	m_Move=D3DXVECTOR2(0.0f,0.0f);
	
	//�`��F
	m_Color = CColorData(1.0f, 1.0f, 1.0f, 1.0f);
}

//�X�V
void CTitleObj::Update()
{
	//�V�[�����C����
	if (INPUT->GetKeyPush(VK_RETURN) == true)
	{
		SCENE->SetScene(SceneName::SceneMain);
	}

	if (INPUT->GetKeyPush('W') == true)
	{
		m_Move.y += -1.0f;
	}
	
	if (INPUT->GetKeyPush('S') == true)
	{
		m_Move.y += 1.0f;
	}

	if (INPUT->GetKeyPush('A') == true)
	{
		m_Move.x += -1.0f;
	}
	if (INPUT->GetKeyPush('D') == true)
	{
		m_Move.x += 1.0f;
	}

	//�ړ�
	m_ScreenPos.x += m_Move.x;
	m_ScreenPos.y += m_Move.y;

	//�ړ�������
	m_Move = D3DXVECTOR2(0.0f, 0.0f);
}

//�`��
void CTitleObj::Draw()
{	
	wchar_t wStr[] = L"���C�u����";
	FONT->DrawStr(wStr, m_ScreenPos.x, m_ScreenPos.y, 64.0f, 0.0f);

	wchar_t wStr2[] = L"Enter";
	FONT->DrawStr(wStr2, 261.0f, 377.0f, 64.0f, 0.0f);

	//�؂���ʒu
	m_Src = RECT_F(0.0f, 0.0f, 640.0f, 480.0f);

	//�`��ʒu
	m_Dst = RECT_F(0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT);

	//�`��
	DRAW->DrawTexture(TexName::TexTitle, &m_Src, &m_Dst, &m_Color, m_fRad);
}
