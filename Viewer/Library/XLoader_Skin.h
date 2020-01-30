#pragma once

#include "Main.h"
#include "Singleton.h"
#include "Struct.h"
#include "LibraryHeader.h"
#include "X_Struct.h"
#include <stdio.h>

//X�t�@�C���֘A�̃N���X
class CX_Skin : public CSingleton<CX_Skin>
{
private:
	//�V���O���g��
	friend CSingleton<CX_Skin>;
	CX_Skin() {};	//�R���X�g���N�^
	~CX_Skin() {};	//�f�X�g���N�^
public:
	void Release();//�J��

	//�X�L�����b�V���̓ǂݍ���
	bool LoadSkinMesh(const char* FileName, X_SKIN_MESH* pSkinMesh);

	//�A�j���[�V����
	void Animation(int AnimeId, int NowFrame, X_SKIN_MESH* pSkinMesh);

	//���b�V���`��
	void DrawMesh(D3DMATRIX matWorld, X_SKIN_MESH* pSkinMesh, CColorData* pColor);
private:
	
	//�{�[���̍X�V
	void BoneUpdate(X_SKIN_MESH* pSkin, int AnimeId, int NowFrame);

	//�|�[�Y���擾����
	D3DXMATRIX GetPose(bool* bFind, X_SKIN_MESH* pSkin, X_BONE* pBone, X_ANIMATION Anime, int NowFrame, int BoneID);

	//�E�F�C�g���傫�����Ƀ\�[�g����
	void WeightSort(X_SKIN_MESH* pSkin);

	//�t���[���⊮
	X_KEY FrameComplement(int NowFrame, X_ANIMATOIN_KEY AnimKey);

};