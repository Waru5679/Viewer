#pragma once
#include <d3dx10.h>
#include <D3D10.h>
#include <list>
#include "Singleton.h"
using namespace std;

//テクスチャデータ
class CTextureData
{
public:
	//コンストラクタ
	CTextureData(ID3D10ShaderResourceView* pTex,int Width,int Height,int Id)
	{
		m_pTex = pTex;
		m_Width = Width;
		m_Height = Height;
		m_Id = Id;
	}

	//開放
	void Release()
	{
		m_pTex->Release();
		m_pTex = nullptr;
	}

	//テクスチャ取得
	ID3D10ShaderResourceView* GetTexture() { return m_pTex; }

	//高さ取得
	int GetWidth() { return m_Width; }

	//幅取得
	int GetHeight() { return m_Height; }

	//ID取得
	int GetId() { return m_Id; }
private:
	//テクスチャ
	ID3D10ShaderResourceView* m_pTex;
	int m_Width;	//幅
	int m_Height;	//高さ
	int m_Id;		//管理用ID
};

//テクスチャ管理クラス
class CTextureManager:public CSingleton<CTextureManager>
{
private:
	//シングルトン
	friend CSingleton<CTextureManager>;
	CTextureManager() {};	//コンストラクタ
	~CTextureManager() {};	//デストラクタ
public:
	//読み込み
	void Load(int id,const char* name,int width,int height);
	
	//テクスチャ取得
	CTextureData* GetTexData(int Id);

	//開放
	void Release();
private:
	list<CTextureData*>m_list;	//テクスチャリスト
};