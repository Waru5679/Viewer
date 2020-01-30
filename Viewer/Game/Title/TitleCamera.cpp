//ライブラリファイル
#include "../../Library/LibraryHeader.h"

//ゲームファイル
#include "../GameHeader.h"

//初期化
void CTitleCamera::Init()
{
	//初期カメラの位置
	m_vEye = D3DXVECTOR3(0.0f, 5.0f, -2.0f);
	m_vLook = D3DXVECTOR3(0.0f, 2.0f, 1.0f);
	m_vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//行列初期化
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);

	//角度
	m_vAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//頂点シェーダ用のマトリックス作成
	CreateVSMatrix(&m_vEye, &m_vLook, &m_vUp, &m_matView, &m_matProj, WINDOW_WIDTH, WINDOW_HEIGHT);
}
