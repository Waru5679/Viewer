#pragma once

//ライブラリファイル
#include "../../Library/CObj.h"
#include "../../Library/LibraryHeader.h"

//TitleObj
class CTitleObj :public CObj2DBase
{
public:
	void Init();		//初期化
	void Update();		//更新
	void Draw();		//描画
	void Release() {};	//開放
private:

	wchar_t m_wPos_x[100];
	wchar_t m_wPos_y[100];
};