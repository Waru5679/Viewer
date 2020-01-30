#pragma once
#include "Singleton.h"

constexpr float MIN_FRAME_TIME{ 1000.0f / FRAME_RATE };

//フレームレート計算クラス
class CFrameRate:public CSingleton<CFrameRate>
{
private:
	//シングルトン
	friend CSingleton<CFrameRate>;
	CFrameRate() {};	//コンストラクタ
	~CFrameRate() {};	//デストラクタ
public:
	void Init();		//初期化
	void FrameCount();	//フレーム計算
	void Draw();		//描画
	void Release();		//解放
private:
	DWORD m_NowTime;	//現在時刻
	DWORD m_StartTime;	//開始時刻
	DWORD m_DrawTime;	//描画時刻

	int m_FrameRate;	//フレームレート
	int m_Count;		//カウンタ
		
	wchar_t* m_pDrawStr;//表示用文字列
};