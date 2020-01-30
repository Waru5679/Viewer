#pragma once
#include "Main.h"
#include "Camera.h"
#include "LibraryHeader.h"
#include "X_Loader.h"
#include "XLoader_Skin.h"

//シェーダクラス
class CSkinShader :public CSingleton<CSkinShader>
{
private:
	//シングルトン
	friend CSingleton<CSkinShader>;
	CSkinShader() {};	//コンストラクタ
	~CSkinShader() {};	//デストラクタ
public:
	bool Init(ID3D10Device* pDevice);	//初期化
	void Release();						//解放

	//カメラセット
	void SetCamera(CCameraBase* pCamera) { m_pCamera = pCamera; }

	//シェーダーセット
	void SetShader(ID3D10ShaderResourceView* pTexture, RECT_F* pSrc, CColorData* pColor, D3DXMATRIX matWorld, X_SKIN_MESH* pSkin);

	//インプットレイアウトの作成
	bool CreateLayOut(ID3D10Device* pDevice);

	//アクセサ
	ID3D10Effect* GetEffect() { return m_pEffect; }
	ID3D10EffectTechnique* GetTechnique() { return m_pTechnique; }
	ID3D10InputLayout* GetVertexLayOut() { return m_pVertexLayout; }
private:
	ID3D10Effect* m_pEffect;				//エフェクト
	ID3D10EffectTechnique* m_pTechnique;	//テクニック
	ID3D10InputLayout* m_pVertexLayout;		//頂点レイアウト

	//アプリ←→シェーダー架け橋
	ID3D10EffectMatrixVariable* m_pShaderWorldViewProjection;	//ワールドから射影までの変換行列
	ID3D10EffectShaderResourceVariable* m_pShaderTexture;		//テクスチャー
	ID3D10EffectVectorVariable* m_pShaderSrc;					//切り取り位置
	ID3D10EffectVectorVariable* m_pShaderColor;					//描画色
	ID3D10EffectMatrixVariable* m_pBone;						//ボーンのポーズ行列

	CCameraBase* m_pCamera;	//カメラポインタ
};