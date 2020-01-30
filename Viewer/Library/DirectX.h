#pragma once
#include "Main.h"
#include "Singleton.h"

//DirectXクラス
class CDirectX :public CSingleton<CDirectX>
{
private:
	//シングルトン
	friend CSingleton<CDirectX>;
	CDirectX() {};	//コンストラクタ
	~CDirectX() {};	//デストラクタ
public:
	//初期化
	bool Init(HWND hWnd);

	//メモリの開放
	void Release();

	//アクセサ
	ID3D10Device*			GetDevice()				{ return m_pDevice; }	
	IDXGISwapChain*			GetSwapChain()			{ return m_pSwapChain; }
	ID3D10RenderTargetView* GetRenderTargetView()	{ return m_pRenderTargetView; }
	ID3D10DepthStencilView* GetDepthStencilView()	{ return m_pDepthStencilView; }
	ID3D10Texture2D*		GetDepthStencil()		{ return m_pDepthStencil; }
	ID3D10BlendState*		GetBlendState()			{ return m_pBlendState; }
private:
	//スワップチェイン作成
	HRESULT CreateSwapChain(HWND hWnd);

	//レンダーターゲットビューの作成
	void CreateRenderTarget();

	//ビューポート設定
	void SetViewport();

	//深度ステンシルビューの作成
	void CreateStencil();

	//ブレンドステートの作成
	void CreateBlendState();

	//ラスタライザ設定
	void SetRasterizer();	

	ID3D10Device* m_pDevice;					//デバイス
	IDXGISwapChain* m_pSwapChain;				//スワップチェイン
	ID3D10RenderTargetView* m_pRenderTargetView;//レンダーターゲットビュー
	ID3D10DepthStencilView* m_pDepthStencilView;//深度ステンシルビュー
	ID3D10Texture2D* m_pDepthStencil;			//深度ステンシル
	ID3D10BlendState* m_pBlendState;			//ブレンドステート
};