#pragma once
#include "Singleton.h"
#include <vector>
#include <memory>
using namespace std;

//シーンクラス(継承用)
class CSceneBase
{
public:
	virtual void Init() = 0;	//初期化
	virtual void Update() = 0;	//更新
	virtual void Draw()=0;		//描画
	virtual void Release()=0;	//解放
private:
	virtual void LoadAudio() = 0;		//音楽
	virtual void LoadTexture() = 0;		//テクスチャ
	virtual void LoadMesh() = 0;		//メッシュ
};

//シーン管理用構造体
struct MY_SCENE
{
	//コンストラクタ
	MY_SCENE() {}
	MY_SCENE(CSceneBase* pScene,int Id)
	{
		m_pScene = pScene;
		m_Id = Id;
	}
	
	CSceneBase* m_pScene;	//シーンポインタ
	int m_Id;				//管理用ID
};

using pMyScene=unique_ptr<MY_SCENE>;

//シーンマネージャー
class CSceneManager:public CSingleton<CSceneManager>
{
private:
	//シングルトン
	friend CSingleton<CSceneManager>;
	CSceneManager() {};		//コンストラクタ
	~CSceneManager() {};	//デストラクタ
public:
	void Update();							//更新
	void Draw();							//描画
	void Release();							//解放
	void Insert(CSceneBase* pScene, int Id);//登録
	void SetScene(int n);					//シーンのセット
	CSceneBase* GetScene(int Id);			//シーン取得
private:
	CSceneBase* m_pNowScene;				//現在のシーンのポインタ
	vector<pMyScene> m_Scene;				//シーンリスト
};