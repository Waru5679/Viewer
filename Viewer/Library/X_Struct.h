#pragma once
#include "Main.h"

constexpr int TRIANGLE_POLYGON	{ 3 };	//三角ポリゴン
constexpr int QUAD_POLYGON		{ 4 };	//四角ポリゴン
constexpr int READ_ARRAY_SIZE	{ 200 };//読み込み用キー配列のサイズ
constexpr int NAME_ARRAY_SIZE	{ 100 };//名前配列のサイズ
constexpr int MAX_VER_WEIGH		{ 4 };	//頂点の最大ウェイト

//アニメーションのキータイプ
enum KEY_TYPE
{
	ROT,	//回転
	SCALE,	//スケール
	TRANS,	//移動
	MATRIX,	//行列
};

//頂点構造体
struct X_VERTEX
{
	X_VERTEX()
	{
		for (int i = 0; i < MAX_VER_WEIGH; i++)
		{
			m_BoneIndex[i] = -1;
			m_fWeight[i] = 0.0f;
		}
		m_WeightNum = 0;
	}

	D3DXVECTOR3 m_vPos;	//頂点位置
	D3DXVECTOR3 m_vNorm;//頂点法線
	D3DXVECTOR2 m_vTex;	//UV座標

	int m_BoneIndex[MAX_VER_WEIGH];	//影響を受けるボーンのインデックス
	float m_fWeight[MAX_VER_WEIGH];	//ウェイト
	int m_WeightNum;				//ウェイト数
};

//ポリゴン
struct X_FACE
{
	X_FACE()
	{
		m_FaceOfVer = -1;
		m_pIndex = nullptr;
		m_UseMaterial = -1;
	}
	int m_FaceOfVer;	//構成する頂点の数
	int* m_pIndex;		//構成する頂点のインデックス
	int m_UseMaterial;	//使用するマテリアル
};

//マテリアル構造体
struct X_MATERIAL
{
	X_MATERIAL()
	{
		ZeroMemory(this, sizeof(X_MATERIAL));
		m_pTexture = nullptr;
		m_pFaceIndex = nullptr;
		m_ppIndexBuffer = nullptr;
	}

	char						m_TexName[NAME_ARRAY_SIZE];	//ファイル名
	D3DXVECTOR4					m_vFaceColor;				//面の色
	D3DXVECTOR3					m_vKs;						//スペキュラー
	float						m_fPower;					//スペキュラーのパワー
	D3DXVECTOR3					m_vKe;						//エミッシブ
	int							m_FaceNum;					//このマテリアルを使用する面の数
	int*						m_pFaceIndex;				//このマテリアルで使用する面のインデックスリスト
	ID3D10Buffer**				m_ppIndexBuffer;			//インデックスバッファ
	ID3D10ShaderResourceView*	m_pTexture;					//テクスチャポインタ	
};

//ボーン構造体
struct X_BONE
{
	X_BONE()
	{
		m_pChildIndex = nullptr;
		D3DXMatrixIdentity(&m_matBindPose);
		D3DXMatrixIdentity(&m_matNewPose);
		D3DXMatrixIdentity(&m_matOffset);
	}
	char		m_Name[NAME_ARRAY_SIZE];	//ボーン名
	int			m_index;					//自身のインデックス
	int			m_ChildNum;					//子の数
	int*		m_pChildIndex;				//自分の子のインデックスリスト
	D3DXMATRIX	m_matBindPose;				//初期ポーズ（ずっと変わらない）
	D3DXMATRIX	m_matNewPose;				//現在のポーズ（その都度変わる）
	D3DXMATRIX  m_matOffset;				//オフセット行列
};

//どのボーンが度の頂点にどれだけの影響を与えるか
struct X_SKIN_WEIGHT
{
	X_SKIN_WEIGHT()
	{
		m_WeightNum = -1;
		m_pIndex = nullptr;
		m_pWeight = nullptr;
		D3DXMatrixIdentity(&m_matOffset);
	}

	char		m_BoneName[NAME_ARRAY_SIZE];//ボーン名
	int			m_WeightNum;				//ウェイトの数
	int*		m_pIndex;					//影響を受ける頂点のインデックスリスト
	float*		m_pWeight;					//ウェイトリスト
	D3DXMATRIX	m_matOffset;				//オフセット行列
};

//アニメーションのキー
struct X_KEY
{
	X_KEY()
	{
		m_Time = -1;
		m_ValueNum = -1;
		m_pfValue = nullptr;
	}

	int		m_Time;		//コマ
	int		m_ValueNum;	//値の数
	float*	m_pfValue;	//値のリスト
};

struct  X_ANIMATOIN_KEY
{
	X_ANIMATOIN_KEY()
	{
		m_KeyNum = -1;
		m_KeyType = -1;
		m_pKey = nullptr;
	}
	int		m_KeyType;	//キータイプ
	int		m_KeyNum;	//キーの数
	X_KEY*	m_pKey;		//キーリスト
};

//ボーンのキー
struct X_BONE_KEY
{
	X_BONE_KEY()
	{
		m_AniKeyNum = -1;
		m_pAniKey = nullptr;
	}

	char	m_AffectBoneName[NAME_ARRAY_SIZE];	//影響を受けるボーン名
	int		m_AniKeyNum;						//キーの数

	X_ANIMATOIN_KEY*	m_pAniKey;				//キーリスト
};

//アニメーション構造体
struct X_ANIMATION
{
	X_ANIMATION()
	{
		m_BoneKeyNum = -1;
		m_pBoneKey = nullptr;
	}

	char		m_Name[NAME_ARRAY_SIZE];//アニメーション名
	int			m_BoneKeyNum;			//対応ボーンの数
	X_BONE_KEY*	m_pBoneKey;				//ボーンのキーリスト
};

//メッシュ
struct X_MESH
{
	X_MESH()
	{
		m_VerNum = -1;
		m_FaceNum = -1;
		m_MaterialNum = -1;
		m_pFace = nullptr;
		m_pMaterial = nullptr;
		m_pVertexBuffer = nullptr;
		m_pVertex = nullptr;
	}
	int				m_VerNum;		//頂点数
	int				m_FaceNum;		//面(ポリゴン)数
	X_FACE*			m_pFace;		//面リスト
	int				m_MaterialNum;	//マテリアルの数
	X_MATERIAL*		m_pMaterial;	//マテリアルリスト
	ID3D10Buffer*	m_pVertexBuffer;//頂点バッファ
	X_VERTEX*			 m_pVertex;		//頂点情報のリスト
};

//スキンメッシュヘッダー
struct X_SKIN_MESH_HEADER
{
	X_SKIN_MESH_HEADER()
	{
		m_MaxVertex = -1;
		m_MaxFace = -1;
		m_BoneNum = -1;
	}
	int	m_MaxVertex;//頂点の最大ウェイト数
	int m_MaxFace;	//面の最大ウェイト数
	int m_BoneNum;	//ボーン数
};

//スキンメッシュ
struct X_SKIN_MESH
{
	X_SKIN_MESH()
	{
		m_Mesh = X_MESH();
		m_BoneNum = -1;
		m_pBone = nullptr;
		m_SkinHeader = X_SKIN_MESH_HEADER();
		m_WeightNum = -1;
		m_pWeight = nullptr;
		m_AnimeNum = -1;
		m_pAnimation = nullptr;
		m_pRoot = nullptr;
	}

	X_MESH				m_Mesh;			//メッシュ
	int					m_BoneNum;		//ボーン数
	X_BONE*				m_pBone;		//ボーンリスト
	X_SKIN_MESH_HEADER	m_SkinHeader;	//スキンメッシュヘッダー
	int					m_WeightNum;	//ウェイト数
	X_SKIN_WEIGHT*		m_pWeight;		//スキンウェイト
	int					m_AnimeNum;		//アニメーション数
	X_ANIMATION*		m_pAnimation;	//アニメーション
	X_BONE*				m_pRoot;		//ルートボーン
};
