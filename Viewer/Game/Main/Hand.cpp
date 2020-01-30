#include "Hand.h"
#include "../GameHeader.h"

//コンストラクタ
CHand::CHand(D3DXVECTOR3 vPos, D3DXVECTOR3 vAngle, D3DXVECTOR3 vScale)
{
	m_vPos = vPos;
	m_vAngle = vAngle;
	m_vScale = vScale;
}

//初期化
void CHand::Init()
{
	//モデル取得
	m_pSkinMesh = MODEL->GetModelData(ModelName::ModelHand);

	//ワールド行列作成
	m_matWorld=MakeMatWorld(m_vPos, m_vAngle, m_vScale);
	
	//フレーム初期化
	m_Frame=0;
}

//更新
void CHand::Update()
{
	//フレーム更新
	m_Frame+=15;
	if (m_Frame >= 9000)
		m_Frame = 0;

	//アニメーション
	for (int i = 0; i < m_pSkinMesh->m_AnimeNum; i++)
	{
		SKIN->Animation(i, m_Frame, m_pSkinMesh);
	}

	//マウスクリックテスト！
	if (INPUT->MousLeftCrick())
	{
		int a = 0;
	}
	if (INPUT->MousRightCrick())
	{
		int b = 0;
	}
}

//描画
void CHand::Draw()
{
	//手
	SKIN->DrawMesh(m_matWorld, m_pSkinMesh, nullptr);	
}

//開放
void CHand::Release()
{
	m_pSkinMesh = nullptr;
}