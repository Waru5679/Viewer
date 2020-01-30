//ライブラリファイル
#include "LibraryHeader.h"

//ゲームで使うシーンファイル
#include "../Game/GameHeader.h"

//シーンの作成
void CSceneInclude::LoadScene()
{
	//メイン
	CSceneMain* pMain = new CSceneMain();
	SCENE->Insert(pMain, SceneName::SceneMain);

	//タイトル
	CSceneTitle* pTitle = new CSceneTitle();
	SCENE->Insert(pTitle, SceneName::SceneTitle);

}