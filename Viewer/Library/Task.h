#pragma once
#include <d3dx10.h>
#include <D3D10.h>
#include <vector>
#include <windef.h>

#include "CObj.h"
#include "Singleton.h"
using namespace std;

//管理用　クラス
class CTask:public CSingleton<CTask>
{
private:
	//シングルトン
	friend CSingleton<CTask>;
	CTask() {};
	~CTask() {};
public:
	void Update();		//更新
	void Draw();		//描画
	void Release();		//解放
	void ClearTask();	//タスク情報クリア
		
	//登録
	void InsertObj(CObjBase* pObj, int Id);
	
	//取得	
	CObjBase* GetObj(int Id);
	
private:
	//ソート
	void Sort();

	vector<CObjBase*> m_Obj;			//オブジェクト
};