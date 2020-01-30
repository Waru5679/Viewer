#pragma once

#include "Main.h"
#include "Singleton.h"
#include "Struct.h"
#include "LibraryHeader.h"
#include "X_Struct.h"
#include <stdio.h>

//Xファイル関連のクラス
class CX_Skin : public CSingleton<CX_Skin>
{
private:
	//シングルトン
	friend CSingleton<CX_Skin>;
	CX_Skin() {};	//コンストラクタ
	~CX_Skin() {};	//デストラクタ
public:
	void Release();//開放

	//スキンメッシュの読み込み
	bool LoadSkinMesh(const char* FileName, X_SKIN_MESH* pSkinMesh);

	//アニメーション
	void Animation(int AnimeId, int NowFrame, X_SKIN_MESH* pSkinMesh);

	//メッシュ描画
	void DrawMesh(D3DMATRIX matWorld, X_SKIN_MESH* pSkinMesh, CColorData* pColor);
private:
	
	//ボーンの更新
	void BoneUpdate(X_SKIN_MESH* pSkin, int AnimeId, int NowFrame);

	//ポーズを取得する
	D3DXMATRIX GetPose(bool* bFind, X_SKIN_MESH* pSkin, X_BONE* pBone, X_ANIMATION Anime, int NowFrame, int BoneID);

	//ウェイトが大きい順にソートする
	void WeightSort(X_SKIN_MESH* pSkin);

	//フレーム補完
	X_KEY FrameComplement(int NowFrame, X_ANIMATOIN_KEY AnimKey);

};