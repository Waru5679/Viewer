#pragma once
#include "Main.h"
#include <math.h>

//ベクトルの掛け算
D3DXVECTOR3 operator* (D3DXVECTOR3 Vec1,D3DXVECTOR3 Vec2);

//ベクトルの割り算
D3DXVECTOR3 operator/ (D3DXVECTOR3 Vec1, D3DXVECTOR3 Vec2);

//PosからTrans行列を作る
D3DXMATRIX MakeMatTrans(D3DXVECTOR3 Pos);

//AngleからRot行列を作る
D3DXMATRIX MakeMatRot(D3DXVECTOR3 Angle);

//ScaleからScale行列を作る
D3DXMATRIX MakeMatScale(D3DXVECTOR3 Scale);

//Pos,Angle,ScaleからWorld行列を作る
D3DXMATRIX MakeMatWorld(D3DXVECTOR3 Pos, D3DXVECTOR3 Angle, D3DXVECTOR3 Scale);

//絶対値ベクトルを作る
D3DXVECTOR3 MakeAbsVector3(D3DXVECTOR3 Vec);

//最も長い成分を調べる
float MostLongComponent(D3DXVECTOR3 Vec);
float MostLongComponent(float f1, float f2, float f3);

//頂点シェーダ用のマトリックスの作成
void CreateVSMatrix(D3DXVECTOR3* vEye, D3DXVECTOR3* vLook, D3DXVECTOR3* vUp, D3DXMATRIX* matView, D3DXMATRIX* matProj, int width, int height);