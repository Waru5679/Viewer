#pragma once

//ライブラリファイル
#include "../../Library/Scene.h"

#include "MainCamera.h"

//シーンメイン
class CSceneMain :public CSceneBase
{
public:
	void Init();	//初期化
	void Update();	//更新	
	void Draw();	//描画
	void Release();	//解放

private:
	void LoadAudio();		//音楽
	void LoadTexture();		//テクスチャ
	void LoadMesh();		//メッシュ

};