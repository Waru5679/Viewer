#pragma once

//ライブラリファイル
#include "../../Library/Camera.h"

//メインカメラクラス
class CMainCamera :public CCameraBase
{
public:
	void Init();	//初期化
	void Update();	//更新
	void Draw() {};	//描画
	void Release() {};	//開放

private:
	float m_fCameraDis;	//プレイヤーとカメラの距離
	float m_fLookDis;	//プレイヤーと注視点の距離

};