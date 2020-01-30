#include "LibraryHeader.h"
#include "XLoader_Skin.h"
#include "Release.h"
#include "Math.h"

#include <math.h>

//�C���X�^���X
CX_Skin* CX_Skin::m_pInstance = nullptr;

//�X�L�����b�V���̓ǂݍ���
bool CX_Skin::LoadSkinMesh(const char* FileName, X_SKIN_MESH* pSkinMesh)
{
	//�t�@�C���ǂݍ���
	FILE* fp = nullptr;
	fopen_s(&fp, FileName, "rt");
	
	if (fp == nullptr)
	{
		//�t�@�C���I�[�v�����s
		return false;
	}

	//template���Ȃ����t�@�C���̓ǂݍ��݊J�n�ʒu��ۑ�
	long ReadStartPos = X_LOADER->GetTemplateSkipStartPos(fp);

	X_MESH	Mesh;				//���b�V��
	int		BoneNum			= 0;//�{�[����
	int		SkinWeightNum	= 0;//�X�L���E�F�C�g�̐�
	int		AnimeNum		= 0;//�A�j���[�V�����̐�

	X_BONE*			pBone		= nullptr;	//�{�[�����X�g
	X_SKIN_WEIGHT*	pSkinWeight	= nullptr;	//�X�L���E�F�C�g���X�g
	X_ANIMATION*	pAnime		= nullptr;	//�A�j���V�������X�g

	X_SKIN_MESH_HEADER SkinHeader;	//�X�L�����b�V���w�b�_�[

	//�X�L�����b�V���w�b�_�[�ǂݍ���
	X_LOADER->LoadSkinMeshHeader(fp, &SkinHeader, ReadStartPos);

	//���b�V���̓ǂݍ���
	if (X_LOADER->LoadMesh(fp, &Mesh, &SkinHeader,ReadStartPos) == false)
	{
		//���b�V���ǂݍ��ݎ��s
		return false;
	}
	
	//�{�[�����擾
	BoneNum = X_LOADER->GetBoneNum(fp, ReadStartPos);
	
	//�{�[��������Ƃ��̂�
	if (BoneNum != 0)
	{
		//�{�[�����X�g�������m��
		pBone = new X_BONE[BoneNum];

		//�{�[���ǂݍ���
		if (X_LOADER->LoadBone(fp, pBone, ReadStartPos) == false)
		{
			//�{�[���ǂݍ��ݎ��s
			return false;
		}
	}

	//�X�L���E�F�C�g�̐��𐔂���
	SkinWeightNum = X_LOADER->GetSkinWeightNum(fp, ReadStartPos);

	//�X�L���E�F�C�g������Ƃ��̂�
	if (SkinWeightNum != 0)
	{
		//�X�L���E�F�C�g�������m��
		pSkinWeight = new X_SKIN_WEIGHT[SkinWeightNum];
		
		//�X�L�����̓ǂݍ���
		if (X_LOADER->LoadSkinWeight(fp, pSkinWeight, ReadStartPos) == false)
		{
			//�X�L�����ǂݍ��ݎ��s
			return false;
		}
	}

	//�A�j���[�V�����̐��擾
	AnimeNum = X_LOADER->GetAnimeNum(fp, ReadStartPos);

	//�A�j���[�V����������Ƃ��̂�
	if (AnimeNum != 0)
	{
		//�A�j���[�V�����������m��
		pAnime = new X_ANIMATION[AnimeNum];

		//�A�j���[�V�����ǂݍ���
		if (X_LOADER->LoadAnimation(fp, pAnime, ReadStartPos) == false)
		{
			//�A�j���[�V�����ǂݍ��ݎ��s
			return false;
		}
	}

	//�X�L�����b�V���ɂ܂Ƃ߂�
	X_LOADER->SkinMeshPutTogether(Mesh, pBone, BoneNum,pSkinWeight, SkinWeightNum, pAnime,AnimeNum, pSkinMesh,SkinHeader);

	//�X�L���E�F�C�g�̏������ƂɊe���_�ɑΉ��{�[���ƃE�F�C�g�̏�����������
	X_LOADER->VertexMatchBone(pSkinMesh);

	//�E�F�C�g���Ɋe���_�̃{�[���C���f�b�N�X���\�[�g����
	WeightSort(pSkinMesh);

	//�o�[�e�b�N�X�o�b�t�@�[���쐬
	pSkinMesh->m_Mesh.m_pVertexBuffer = DRAW->BufferCreate(pSkinMesh->m_Mesh.m_pVertex, sizeof(X_VERTEX) * pSkinMesh->m_Mesh.m_VerNum, D3D10_BIND_VERTEX_BUFFER);

	return true;
}

//�E�F�C�g���傫�����Ƀ\�[�g����
void CX_Skin::WeightSort(X_SKIN_MESH* pSkin)
{
	//���_����
	for (int i=0;i<pSkin->m_Mesh.m_VerNum; i++)
	{
		//�E�F�C�g���傫�����Ƀ\�[�g
		for (int j = 0; j<MAX_VER_WEIGH-1; j++)
		{
			for (int k = j + 1; k < MAX_VER_WEIGH; k++)
			{
				if (pSkin->m_Mesh.m_pVertex[i].m_fWeight[j] < pSkin->m_Mesh.m_pVertex[i].m_fWeight[k])
				{
					float fTmp = pSkin->m_Mesh.m_pVertex[i].m_fWeight[j];
					int iTmp = pSkin->m_Mesh.m_pVertex[i].m_BoneIndex[j];

					pSkin->m_Mesh.m_pVertex[i].m_fWeight[j]= pSkin->m_Mesh.m_pVertex[i].m_fWeight[k];
					pSkin->m_Mesh.m_pVertex[i].m_BoneIndex[j] = pSkin->m_Mesh.m_pVertex[i].m_BoneIndex[k];

					pSkin->m_Mesh.m_pVertex[i].m_fWeight[k] = fTmp;
					pSkin->m_Mesh.m_pVertex[i].m_BoneIndex[k] = iTmp;
				}
			}
		}
	}
}

//�t���[���⊮
X_KEY CX_Skin::FrameComplement(int NowFrame, X_ANIMATOIN_KEY AnimKey)
{
	X_KEY out;

	int	 keyNum		= AnimKey.m_KeyNum;
	int* pFrameDiff = new int[keyNum];
	bool bKey = false;

	//�t���[���̊Ԋu����ۑ�
	for (int i = 0; i <keyNum; i++)
	{
		pFrameDiff[i] = AnimKey.m_pKey[i].m_Time - NowFrame;

		//���݂̃t���[�����L�[�t���[���̏ꍇ
		if (pFrameDiff[i] == 0)
		{
			bKey = true;
			out = AnimKey.m_pKey[i];
		}
	}

	//�L�[�t���[���ȊO�Ȃ�⊮����
	if(bKey==false)
	{
		X_KEY before= AnimKey.m_pKey[0];
		X_KEY after= AnimKey.m_pKey[keyNum-1];
		
		//�O�t���[����T��
		for (int i = 0; i < keyNum; i++)
		{
			//�����}�C�i�X�̏ꍇ�O�t���[��
			if (pFrameDiff[i] < 0)
				before = AnimKey.m_pKey[i];
		}

		//��t���[����T��
		for (int i = keyNum - 1; i >= 0; i--)
		{
			//�����v���X�Ȃ��t���[��
			if (pFrameDiff[i] > 0)
				after = AnimKey.m_pKey[i];
		}

		//�t���[���������߂�
		int FrameDiff = after.m_Time - before.m_Time;

		//�ω��̊��������߂�
		float fPercent=	(float)(NowFrame - before.m_Time) / FrameDiff;

		//�����|�[�Y�̃������m��
		float* pfPoseDiff = new float[before.m_ValueNum];

		//�O��t���[���ł̃|�[�Y��(�O��t���[���̃f�[�^���͓����Ɖ��肷��)
		for (int i = 0; i < before.m_ValueNum; i++)
		{			
			//�|�[�Y�̍��������߂�
			pfPoseDiff[i] = after.m_pfValue[i] - before.m_pfValue[i];
		}

		//�l�̐��ۑ�
		out.m_ValueNum = before.m_ValueNum;

		//�������̊m��
		out.m_pfValue = new float[out.m_ValueNum];

		//���݂̃|�[�Y�����߂�
		for (int i = 0; i < out.m_ValueNum ; i++)
		{
			out.m_pfValue[i] = before.m_pfValue[i] + (pfPoseDiff[i] * fPercent);
		}

		//�|�[�Y�̍����j��
		delete[] pfPoseDiff;
	}

	//���݃t���[���ۑ�
	out.m_Time = NowFrame;

	//�t���[�������j��
	delete[] pFrameDiff;
	pFrameDiff = nullptr;

	return out;
}

//�{�[���̍X�V
void CX_Skin::BoneUpdate(X_SKIN_MESH* pSkin, int AnimeId, int NowFrame)
{
	//���s����A�j���[�V�����f�[�^
	X_ANIMATION anime = pSkin->m_pAnimation[AnimeId];
	
	//�{�[�����ɍX�V����
	bool bBoneFind = false;
	for (int i = 0; i < pSkin->m_BoneNum; i++)
	{
		bBoneFind = false;
		pSkin->m_pBone[i].m_matNewPose=GetPose(&bBoneFind,pSkin, pSkin->m_pRoot, anime, NowFrame,i);
	}
}

//�|�[�Y���擾����
D3DXMATRIX CX_Skin::GetPose(bool* bBoneFind, X_SKIN_MESH* pSkin, X_BONE* pBone, X_ANIMATION Anime, int NowFrame, int BoneID)
{
	//�|�[�Y
	D3DXMATRIX matOut;
	D3DXMatrixIdentity(&matOut);

	//�V�����|�[�Y
	D3DXMATRIX matNewPose;
	D3DXMatrixIdentity(&matNewPose);

	//�{�[�����ƈ�v����A�j���[�V�����f�[�^��T��
	bool bAnimFind = false;
	for (int i = 0; i < Anime.m_BoneKeyNum && bAnimFind ==false; i++)
	{
		//�{�[�����ƈ�v����A�j���[�V�����f�[�^����
		if (strcmp(pBone->m_Name, Anime.m_pBoneKey[i].m_AffectBoneName) == 0)
		{
			bAnimFind = true;

			//�t���[���⊮�p
			D3DXMATRIX matRot, matTrans, matScale,matMatrix;
			D3DXMatrixIdentity(&matRot);
			D3DXMatrixIdentity(&matTrans);
			D3DXMatrixIdentity(&matScale);
			D3DXMatrixIdentity(&matMatrix);

			//���̃{�[���̃|�[�Y���X�V����@
			for (int j = 0; j < Anime.m_pBoneKey[i].m_AniKeyNum; j++)
			{
				//��]
				if (Anime.m_pBoneKey[i].m_pAniKey[j].m_KeyType==KEY_TYPE::ROT)
				{
					//�t���[���⊮
					X_KEY newPose = FrameComplement(NowFrame, Anime.m_pBoneKey[i].m_pAniKey[j]);

					//�⊮�����f�[�^���x�N�g���ɂ���
					D3DXVECTOR3 vRot(newPose.m_pfValue[0], newPose.m_pfValue[1], newPose.m_pfValue[2]);

					//�s��ɕϊ�
					matRot=MakeMatRot(vRot);
				}
				//�X�P�[��
				if (Anime.m_pBoneKey[i].m_pAniKey[j].m_KeyType == KEY_TYPE::SCALE)
				{
					//�t���[���⊮
					X_KEY newPose = FrameComplement(NowFrame, Anime.m_pBoneKey[i].m_pAniKey[j]);

					//�⊮�����f�[�^���x�N�g���ɂ���
					D3DXVECTOR3 vScale(newPose.m_pfValue[0], newPose.m_pfValue[1], newPose.m_pfValue[2]);

					//�s��ɕϊ�
					matScale = MakeMatScale(vScale);
				}
				//�ړ�
				if (Anime.m_pBoneKey[i].m_pAniKey[j].m_KeyType == KEY_TYPE::TRANS)
				{
					//�t���[���⊮
					X_KEY newPose = FrameComplement(NowFrame, Anime.m_pBoneKey[i].m_pAniKey[j]);

					//�⊮�����f�[�^���x�N�g���ɂ���
					D3DXVECTOR3 vTrans(newPose.m_pfValue[0], newPose.m_pfValue[1], newPose.m_pfValue[2]);

					//�s��ɕϊ�
					matTrans = MakeMatTrans(vTrans);
				}
				//�s��
				if (Anime.m_pBoneKey[i].m_pAniKey[j].m_KeyType >= KEY_TYPE::MATRIX)
				{
					//�t���[���⊮
					X_KEY newPose = FrameComplement(NowFrame, Anime.m_pBoneKey[i].m_pAniKey[j]);

					//�s��ɕϊ�
					matMatrix = D3DXMATRIX(newPose.m_pfValue);
				}
			}

			//�V�����|�[�Y
			matNewPose = matScale * matRot * matTrans * matMatrix;

			//�ق����{�[��ID�̃{�[���Ȃ�
			if (pBone->m_index == BoneID)
			{
				//�{�[���̃|�[�Y�X�V
				*bBoneFind = true;
				matOut = pBone->m_matOffset * matNewPose;
				return matOut;
			}
		}
	}

	//�q�{�[���̃|�[�Y�����߂�
	bool bFind = false;
	for (int i = 0; i < pBone->m_ChildNum && bFind == false; i++)
	{
		matOut=GetPose(&bFind,pSkin, &pSkin->m_pBone[pBone->m_pChildIndex[i]], Anime, NowFrame,BoneID)*matNewPose;
		
		//�ړ��Ẵ{�[�����������Ă����
		if (bFind == true)
		{
			*bBoneFind = true;
			return matOut;
		}
	}

	D3DXMatrixIdentity(&matOut);
	return matOut;
}

//�A�j���[�V����
void CX_Skin::Animation(int AnimeId,int NowFrame, X_SKIN_MESH* pSkinMesh)
{
	//�{�[���X�V
	BoneUpdate(pSkinMesh, AnimeId, NowFrame);
}

//���b�V���`��
void CX_Skin::DrawMesh(D3DMATRIX matWorld, X_SKIN_MESH* pSkinMesh, CColorData* pColor)
{
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g�j
	UINT stride = sizeof(X_VERTEX);
	UINT offset = 0;
	DX->GetDevice()->IASetVertexBuffers(0, 1, &pSkinMesh->m_Mesh.m_pVertexBuffer, &stride, &offset);

	//�}�e���A���̐������A���ꂼ��̃}�e���A���̃C���f�b�N�X�o�b�t�@�|��`��
	for (int i = 0; i < pSkinMesh->m_Mesh.m_MaterialNum; i++)
	{
		//�ʂ��Ƃɕ`��
		for (int j = 0; j < pSkinMesh->m_Mesh.m_pMaterial[i].m_FaceNum; j++)
		{
			X_FACE face = pSkinMesh->m_Mesh.m_pFace[pSkinMesh->m_Mesh.m_pMaterial[i].m_pFaceIndex[j]];

			//�C���f�b�N�X�o�b�t�@�[���Z�b�g
			stride = sizeof(int);
			offset = 0;
			DX->GetDevice()->IASetIndexBuffer(pSkinMesh->m_Mesh.m_pMaterial[i].m_ppIndexBuffer[j], DXGI_FORMAT_R32_UINT, 0);

			//�v���~�e�B�u�E�g�|���W�[���Z�b�g
			if (face.m_FaceOfVer == TRIANGLE_POLYGON)
			{
				//�O�p�|���S��
				DX->GetDevice()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			}
			if (face.m_FaceOfVer == QUAD_POLYGON)
			{
				//�l�p�|���S��
				DX->GetDevice()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			}

			//�V�F�[�_�Z�b�g
			SKIN_SHADER->SetShader(pSkinMesh->m_Mesh.m_pMaterial[i].m_pTexture, NULL, pColor, matWorld,pSkinMesh);

			D3D10_TECHNIQUE_DESC dc;
			SKIN_SHADER->GetTechnique()->GetDesc(&dc);
			for (UINT p = 0; p < dc.Passes; ++p)
			{
				SKIN_SHADER->GetTechnique()->GetPassByIndex(p)->Apply(0);
				DX->GetDevice()->DrawIndexed(face.m_FaceOfVer, 0, 0);
			}
		}
	}
}

//�J��
void CX_Skin::Release()
{
	//�C���X�^���X�j��
	PointerRelease(m_pInstance);
}