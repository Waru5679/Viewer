#pragma once

//ライブラリファイル
#include "../../Library/Scene.h"

//シーンメイン
class CSceneTitle :public CSceneBase
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