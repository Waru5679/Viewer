#pragma once

//ライブラリファイル
#include "../../Library/CObj.h"
#include "../../Library/LibraryHeader.h"
#include "../../Library/XLoader_Skin.h"

//手クラス
class CHand :public CObj3DBase
{
public:
	//コンストラクタ
	CHand(D3DXVECTOR3 vPos, D3DXVECTOR3 vAngle, D3DXVECTOR3 vScale);
	void Init();	//初期化
	void Update();	//更新
	void Draw();	//描画
	void Release();	//開放

private:

	X_SKIN_MESH* m_pSkinMesh;
	X_MESH m_Mesh;
	int m_Frame;
};