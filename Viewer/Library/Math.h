#pragma once
#include "Main.h"
#include <math.h>

//�x�N�g���̊|���Z
D3DXVECTOR3 operator* (D3DXVECTOR3 Vec1,D3DXVECTOR3 Vec2);

//�x�N�g���̊���Z
D3DXVECTOR3 operator/ (D3DXVECTOR3 Vec1, D3DXVECTOR3 Vec2);

//Pos����Trans�s������
D3DXMATRIX MakeMatTrans(D3DXVECTOR3 Pos);

//Angle����Rot�s������
D3DXMATRIX MakeMatRot(D3DXVECTOR3 Angle);

//Scale����Scale�s������
D3DXMATRIX MakeMatScale(D3DXVECTOR3 Scale);

//Pos,Angle,Scale����World�s������
D3DXMATRIX MakeMatWorld(D3DXVECTOR3 Pos, D3DXVECTOR3 Angle, D3DXVECTOR3 Scale);

//��Βl�x�N�g�������
D3DXVECTOR3 MakeAbsVector3(D3DXVECTOR3 Vec);

//�ł����������𒲂ׂ�
float MostLongComponent(D3DXVECTOR3 Vec);
float MostLongComponent(float f1, float f2, float f3);

//���_�V�F�[�_�p�̃}�g���b�N�X�̍쐬
void CreateVSMatrix(D3DXVECTOR3* vEye, D3DXVECTOR3* vLook, D3DXVECTOR3* vUp, D3DXMATRIX* matView, D3DXMATRIX* matProj, int width, int height);