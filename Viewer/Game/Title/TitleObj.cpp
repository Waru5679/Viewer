//ライブラリファイル
#include "../../Library/LibraryHeader.h"

//ゲームファイル
#include "../GameHeader.h"

//初期化
void CTitleObj::Init()
{
	//位置
	m_ScreenPos=D3DXVECTOR2(120.0f,90.0f);
	
	//移動
	m_Move=D3DXVECTOR2(0.0f,0.0f);
	
	//描画色
	m_Color = CColorData(1.0f, 1.0f, 1.0f, 1.0f);
}

//更新
void CTitleObj::Update()
{
	//シーンメインへ
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

	//移動
	m_ScreenPos.x += m_Move.x;
	m_ScreenPos.y += m_Move.y;

	//移動初期化
	m_Move = D3DXVECTOR2(0.0f, 0.0f);
}

//描画
void CTitleObj::Draw()
{	
	wchar_t wStr[] = L"ライブラリ";
	FONT->DrawStr(wStr, m_ScreenPos.x, m_ScreenPos.y, 64.0f, 0.0f);

	wchar_t wStr2[] = L"Enter";
	FONT->DrawStr(wStr2, 261.0f, 377.0f, 64.0f, 0.0f);

	//切り取り位置
	m_Src = RECT_F(0.0f, 0.0f, 640.0f, 480.0f);

	//描画位置
	m_Dst = RECT_F(0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT);

	//描画
	DRAW->DrawTexture(TexName::TexTitle, &m_Src, &m_Dst, &m_Color, m_fRad);
}
