#pragma once

#include "Main.h"
#include "Camera.h"
#include "Struct.h"
#include "Singleton.h"

struct VERTEX
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vNorm;
	D3DXVECTOR2 vUv;
};

//描画クラス
class CDraw:public CSingleton<CDraw>
{
private:
	//シングルトン
	friend CSingleton<CDraw>;
	CDraw() {};		//コンストラクタ
	~CDraw() {};	//デストラクタ
public:	
	void Init();	//初期化
	void Release();	//解放

	//バッファ作成
	ID3D10Buffer* BufferCreate(void* pHead, unsigned int Size, int BufferType);

	//2D描画(テクスチャのサイズから切り取り位置の設定がいるもの用中継)
	void DrawTexture(int TexId, RECT_F* pSrc, RECT_F* pOut, CColorData* pColor, float fRad);

	//2D描画
	void DrawTexture(ID3D10ShaderResourceView* pTex, RECT_F* pSrc, RECT_F* pOut, CColorData* pColor, float fRad);
	
	//カメラセット
	void SetCamera(CCameraBase* pCamera) { m_pCamera = pCamera; }

	//ポリゴン描画
	void DrawPolygon(int VerNum, ID3D10Buffer* VertexBuffer, ID3D10Buffer* IndexBuffer);
private:
	
	ID3D10Buffer* m_pVertexBuffer;//バーテックスバッファ
	  	
	CCameraBase* m_pCamera;	//カメラポインタ
};