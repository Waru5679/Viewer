#pragma once

#include "Main.h"

#include "LibraryHeader.h"
#include "Singleton.h"
#include "Release.h"
#include "X_Struct.h"
#include <stdio.h>

//X�t�@�C���ǂ݂��݃N���X
class CX_Loader:public CSingleton<CX_Loader>
{
private:
	//�V���O���g��
	friend CSingleton<CX_Loader>;
	CX_Loader() {};
	~CX_Loader() {};
public:

	int GetBoneNum(FILE* fp, long lStartPos);		//�{�[�����̎擾
	int GetSkinWeightNum(FILE* fp, long lStartPos);	//�X�L���E�F�C�g�̐����擾
	int GetAnimeNum(FILE* fp, long lStartPos);		//�A�j���[�V�����̐����擾
	
	//template���΂����ǂݍ��݊J�n�ʒu���擾����
	long GetTemplateSkipStartPos(FILE* fp);

	//���b�V�����̓ǂݍ���
	bool LoadMesh(FILE* fp, X_MESH* pMesh, X_SKIN_MESH_HEADER* pSkinHeader, long lStartPos);

	//�X�L�����b�V���w�b�_�[�ǂݍ���
	void LoadSkinMeshHeader(FILE* fp, X_SKIN_MESH_HEADER* pSkinHeader, long lStartPos);

	//�{�[���ǂݍ���
	bool LoadBone(FILE* fp, X_BONE* pBone, long lStartPos);

	//�{�[�����̓ǂݍ���(�ċN�֐�))
	X_BONE LoadBoneInfo(FILE* fp, int* pBoneIndex, X_BONE* pBone);

	//�X�L���E�F�C�g�̓ǂݍ���
	bool LoadSkinWeight(FILE* fp, X_SKIN_WEIGHT* pSkinWeight, long lStartPos);

	//�A�j���[�V�����ǂݍ���
	bool LoadAnimation(FILE* fp, X_ANIMATION* pAnime, long lStartPos);

	//�X�L�����b�V���ɂ܂Ƃ߂�
	void SkinMeshPutTogether(X_MESH Mesh, X_BONE* pBone, int BoneNum, X_SKIN_WEIGHT* pSkinWeight, int WeightNum, X_ANIMATION* pAnimation, int AnimeNum, X_SKIN_MESH* pSkinMesh, X_SKIN_MESH_HEADER SkinHeader);

	//�{�[�����̃L�[���ǂݍ���
	X_BONE_KEY LoadBoneKey(FILE* fp);

	//�A�j���[�V�����L�[�̐����擾����
	int GetAnimeKeyNum(FILE* fp);

	//�A�j���[�V�����L�[�̓ǂݍ���
	X_ANIMATOIN_KEY LoadAnimationKey(FILE* fp);

	//�X�L���E�F�C�g�̏������ƂɊe���_�ɑΉ��{�[���ƃE�F�C�g�̏�����������
	void VertexMatchBone(X_SKIN_MESH* pSkin);

	//�w�蕶���𕶎��񂩂����
	void ErasCharFromString(char* pSource, int Size, char Erace);
};