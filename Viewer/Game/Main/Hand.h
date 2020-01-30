#pragma once

//���C�u�����t�@�C��
#include "../../Library/CObj.h"
#include "../../Library/LibraryHeader.h"
#include "../../Library/XLoader_Skin.h"

//��N���X
class CHand :public CObj3DBase
{
public:
	//�R���X�g���N�^
	CHand(D3DXVECTOR3 vPos, D3DXVECTOR3 vAngle, D3DXVECTOR3 vScale);
	void Init();	//������
	void Update();	//�X�V
	void Draw();	//�`��
	void Release();	//�J��

private:

	X_SKIN_MESH* m_pSkinMesh;
	X_MESH m_Mesh;
	int m_Frame;
};