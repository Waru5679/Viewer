#include "Input.h"
#include "Release.h"
#include "LibraryHeader.h"

//インスタンス
CInput* CInput::m_pInstance = nullptr;

//初期化
void CInput::Init()
{
	//マウス位置
	m_Mous_x = 0;
	m_Mous_y = 0;

	//マウス左右ボタン
	m_bMousLeft = false;
	m_bMousRight = false;

	//キー情報初期化
	for (int i = 0; i < KEY_BOARD_ARRAY_SIZE; i++)
		m_Key[i] = false;
}

//更新
void CInput::Update(UINT* uMsg, LPARAM* lParam)
{
	switch (*uMsg)
	{
		//マウス操作したとき
		case WM_MOUSEMOVE:
		{
			POINT point = { LOWORD(*lParam),HIWORD((*lParam)) };

			m_Mous_x = point.x;	//カーソルのx座標
			m_Mous_y = point.y;	//カーソルのy座標

			break;
		}
		//左クリック
		case WM_LBUTTONDOWN:
		{
			m_bMousLeft = true;
			break;
		}
		//左クリックしていない時
		case WM_LBUTTONUP:
		{
			m_bMousLeft = false;
			break;
		}
		//右クリック
		case WM_RBUTTONDOWN:
		{
			m_bMousRight = true;
			break;
		}
		//右クリックしていない時
		case WM_RBUTTONUP:
		{
			m_bMousRight = false;
			break;
		}
		default:
			break;
	}
}

//描画
void CInput::Draw()
{	
}

//キー・マウスボタンのプッシュ確認
//ボタンが押されていればtrue,押されていなければfalseを返す
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

//キー・マウスボタンのプッシュ確認 (トリガー式)
//新しき押されたならtrue,
//すでに押されていた　または　押されていないなら false　
bool CInput::GetTriggerKeyPush(int key)
{
	//キーが押されていてる場合
	if ( (GetAsyncKeyState(key) & 0x8000) ) 
	{
		//キーのフラグがオフなら
		if (m_Key[key] == false)
		{
			m_Key[key] = true;
			return true;
		}
	}
	//キーが押されていない場合
	else
	{
		m_Key[key] = false;
	}
	return false;
}



//解放
void CInput::Release()
{
	//インスタンス破棄
	PointerRelease(m_pInstance);
}