#pragma once
#include "CObj.h"

//継承用　カメラクラス
class CCameraBase:public CObjBase
{
public:
	virtual void Init()=0;		//初期化
	virtual void Update()=0;	//更新

	D3DXMATRIX GetWorldMatrix() { return m_matWorld; }
	D3DXMATRIX GetViewMatrix() { return m_matView; }
	D3DXMATRIX GetProjMatrix() { return m_matProj; }
	D3DXVECTOR3 GetEye() { return m_vEye; }
	D3DXVECTOR3 GetAngle() { return m_vAngle; }	
protected:
	D3DXVECTOR3 m_vEye;		//視点
	D3DXVECTOR3 m_vLook;	//注視点
	D3DXVECTOR3 m_vUp;		//カメラの向き
	D3DXVECTOR3 m_vAngle;	//角度

	D3DXMATRIX m_matView;	//ビューマトリックス
	D3DXMATRIX m_matProj;	//プロジェクションマトリックス
	D3DXMATRIX m_matWorld;	//ワールドマトリックス
};