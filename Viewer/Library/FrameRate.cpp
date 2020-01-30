#include "LibraryHeader.h"
#include "Release.h"

//インスタンス
CFrameRate* CFrameRate::m_pInstance = nullptr;

//初期化
void CFrameRate::Init()
{
	//精度を上げる
	timeBeginPeriod(1);

	//開始時刻取得
	m_StartTime = timeGetTime();

	//精度を戻す
	timeEndPeriod(1);
		
	//描画時刻
	m_DrawTime = m_StartTime;
		
	//カウンタ
	m_Count = 0;

	//FPS
	m_FrameRate = 0;

	//表示用文字列カウンタ
	constexpr int ARRAY_SIZE{ 12 };
	m_pDrawStr = new wchar_t[ARRAY_SIZE];
}

//フレームレートの計算
void CFrameRate::FrameCount()
{
	//カウンタ更新
	m_Count++;
	
	//精度上げる
	timeBeginPeriod(1);

	//現在時刻取得
	m_NowTime = timeGetTime();
	
	//このフレームでかかった時間
	float FrameTime = static_cast<float>(m_NowTime - m_StartTime);

	//時間に余裕があるとき
	if (FrameTime < MIN_FRAME_TIME)
	{
		//余分な時間
		DWORD SleepTime = static_cast<DWORD>((MIN_FRAME_TIME - FrameTime));

		//待機
		Sleep(SleepTime);

		//現在時刻の更新
		m_NowTime = timeGetTime();
	}

	//精度を戻す
	timeEndPeriod(1);

	//開始時刻更新
	m_StartTime = m_NowTime;

	//前回の描画から1秒経過してるとき
	float draw_time = static_cast<float>(m_NowTime - m_DrawTime);
	if ( draw_time >= (FRAME_RATE*MIN_FRAME_TIME))
	{
		//描画時刻更新
		m_DrawTime = m_NowTime;

		//フレームレート更新
		m_FrameRate = m_Count;

		//カウンタ初期化
		m_Count = 0;
	}
}

//描画
void CFrameRate::Draw()
{
	//文字列に変換する
	_itow_s(m_FrameRate, m_pDrawStr,sizeof(m_pDrawStr), 10);

	//描画
	FONT->DrawStr(m_pDrawStr, 0.0f, 0.0f, 32.0f, 0.0f);
}

//解放
void CFrameRate::Release()
{
	//表示用文字列破棄
	PointerRelease(m_pDrawStr);

	//インスタンス破棄
	PointerRelease(m_pInstance);
}