#pragma once

#include <d3dx10.h>
#include <vector>
#include "Singleton.h"
#include "XLoader_Skin.h"

using namespace std;

struct ModelData
{
	X_SKIN_MESH m_SkinMesh;	//メッシュ
	int			m_Id;		//ID
};

//オブジェクト管理クラス
class CModelManager :public CSingleton<CModelManager>
{
private:
	//シングルトン
	friend CSingleton<CModelManager>;
	CModelManager(){};		//コンストラクタ
	~CModelManager() {};	//デストラクタ
public:

	//モデル読み込み
	void LoadModel(int Id, const char* Name);

	//モデル取得
	X_SKIN_MESH* GetModelData(int Id);

	//開放
	void Release();

private:
	vector<ModelData> m_SkinMesh;	//スキンメッシュ
};