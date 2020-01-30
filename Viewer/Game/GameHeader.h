#pragma once

//初めのシーン
#define START_SCENE SceneName::SceneMain


//ゲームで使うオブジェクトファイル
#include "../Game/Main/MainCamera.h"
#include "../Game/Title/TitleCamera.h"
#include "../Game/Title/TitleObj.h"

//ゲームで使うシーンファイル
#include "../Game/Main/SceneMain.h"
#include "../Game/Title/SceneTitle.h"

//シーン
enum SceneName
{
	SceneMain,	//メイン
	SceneTitle,	//タイトル
};

//オブジェクトID(処理順)
enum ObjName
{
	ObjMainCamera,	//メインカメラ
	ObjDraw2D,		//2D描画
	ObjTitleCamera,	//タイトルカメラ
	ObjTitle,		//タイトル
	ObjHand,		//手
};

//モデル
enum ModelName
{
	ModelHand,	//手
};

//テクスチャ
enum TexName
{
	TexTitle,	//タイトルテクスチャ
};

