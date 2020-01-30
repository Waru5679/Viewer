#include "X_Loader.h"

CX_Loader* CX_Loader::m_pInstance = nullptr;


//template���΂����ǂݍ��݊J�n�ʒu���擾����
long CX_Loader::GetTemplateSkipStartPos(FILE* fp)
{
	//�t�@�C���̐擪�ɃZ�b�g
	fseek(fp, 0, SEEK_SET);

	long pos;//�t�@�C���̌��݈ʒu

	//�L�[���[�h�ǂݍ���
	char str[READ_ARRAY_SIZE];

	while (!feof(fp))
	{
		//�t�@�C���̌��݈ʒu�ۑ�
		pos = ftell(fp);

		//�L�[���[�h �ǂݍ���
		fscanf_s(fp, "%s ", str, sizeof(str));

		//Frame���o��܂Ŕ�΂�
		if (strcmp(str, "Frame") == 0)
		{
			return pos;
		}
	}
}

//���b�V�����̓ǂݍ���
bool CX_Loader::LoadMesh(FILE* fp, X_MESH* pMesh, X_SKIN_MESH_HEADER* pSkinHeader, long lStartPos)
{
	//�ǂݍ��݂̊J�n�ʒu�ɃZ�b�g
	fseek(fp, lStartPos, SEEK_SET);

	int verNum	= 0;//���_��
	int faceNum = 0;//�ʂ̐�
	int normNum = 0;//�@����
	int uvNum	= 0;//UV��
	int mateNum = 0;//�}�e���A���̐�

	//�ǂݍ��ݗp�i�ǂݍ��݌�j���j
	D3DXVECTOR3* pvPos		= nullptr;//���_���W
	D3DXVECTOR3* pvNormal	= nullptr;//�@��
	D3DXVECTOR2* pvTex		= nullptr;//�e�N�X�`�����W

	//�ǂݍ��݌セ�̂܂܎g������
	X_FACE*		pFace		= nullptr;//�ʂ̃��X�g
	X_MATERIAL* pMaterial	= nullptr;//�}�e���A���̃��X�g
	X_VERTEX*	pVertex		= nullptr;//���_���X�g

	//�ǂݍ��ݗp
	float x, y, z, w;
	int v1, v2, v3, v4;
	v1 = v2 = v3 = v4 = 0;

	//�L�[���[�h�ǂݍ���
	char str[READ_ARRAY_SIZE];

	while (!feof(fp))
	{
		//�L�[���[�h �ǂݍ���
		fscanf_s(fp, "%s ", str, sizeof(str));

		//���_
		if (strcmp(str, "Mesh") == 0)
		{
			//���b�V����
			fgets(str, sizeof(str), fp);

			//���_��
			fgets(str, sizeof(str), fp);
			verNum = atoi(str);

			//���_���W�������m��
			pvPos = new D3DXVECTOR3[verNum];

			//���_���W�ǂݍ���
			for (int i = 0; i < verNum; i++)
			{
				fscanf_s(fp, "%f;%f;%f;,", &x, &y, &z);
				pvPos[i].x = x;
				pvPos[i].y = y;
				pvPos[i].z = z;
			}

			//;����
			fgets(str, sizeof(str), fp);

			//�ʂ̐�
			fgets(str, sizeof(str), fp);
			faceNum = atoi(str);

			//�ʂ̃������m��
			pFace = new X_FACE[faceNum];

			int faceOfVer = 0;//�ʂ��\�����钸�_��

			//�ʓǂݍ���
			for (int i = 0; i < faceNum; i++)
			{
				//�ʂ��\�����钸�_�̐�
				fscanf_s(fp, "%d;", &faceOfVer);
				pFace[i].m_FaceOfVer = faceOfVer;

				//�ʍ\���̃C���f�b�N�X���X�g�̃������m��
				pFace[i].m_pIndex = new int[faceOfVer];

				//�O�p�|���S��
				if (faceOfVer == TRIANGLE_POLYGON)
				{
					fscanf_s(fp, "%d,%d,%d;,", &v1, &v2, &v3);
					pFace[i].m_pIndex[0] = v1;
					pFace[i].m_pIndex[1] = v2;
					pFace[i].m_pIndex[2] = v3;
				}
				//�l�p�|���S��
				if (faceOfVer == QUAD_POLYGON)
				{
					fscanf_s(fp, "%d,%d,%d,%d;,", &v1, &v2, &v3, &v4);
					pFace[i].m_pIndex[0] = v1;
					pFace[i].m_pIndex[1] = v2;
					pFace[i].m_pIndex[2] = v3;
					pFace[i].m_pIndex[3] = v4;
				}
			}
		}
		//�@��
		if (strcmp(str, "MeshNormals") == 0)
		{
			//{����
			fgets(str, sizeof(str), fp);

			//�@����
			fgets(str, sizeof(str), fp);
			normNum = atoi(str);

			//�@���������m��
			pvNormal = new D3DXVECTOR3[normNum];

			//�@���ǂݍ���
			for (int i = 0; i < normNum; i++)
			{
				fscanf_s(fp, "%f;%f;%f;,", &x, &y, &z);
				pvNormal[i].x = x;
				pvNormal[i].y = y;
				pvNormal[i].z = z;
			}
		}

		//�e�N�X�`���[���W �ǂݍ���
		if (strcmp(str, "MeshTextureCoords") == 0)
		{
			//{����
			fgets(str, sizeof(str), fp);

			//UV�̐�
			fgets(str, sizeof(str), fp);
			uvNum = atoi(str);

			//UV�������m��
			pvTex = new D3DXVECTOR2[uvNum];

			//UV�ǂݍ���
			for (int i = 0; i < uvNum; i++)
			{
				fscanf_s(fp, "%f;%f;,", &x, &y);
				pvTex[i].x = x;
				pvTex[i].y = y;
			}
		}

		//�}�e���A�����X�g�ǂݍ���
		if (strcmp(str, "MeshMaterialList") == 0)
		{
			//{����
			fgets(str, sizeof(str), fp);

			//�}�e���A���̐�
			fgets(str, sizeof(str), fp);
			mateNum = atoi(str);

			//�}�e���A���������m��
			pMaterial = new X_MATERIAL[mateNum];

			//�ʂ̐�
			fgets(str, sizeof(str), fp);
			faceNum = atoi(str);

			//�ʂɉ��Ԃ̃}�e���A�����g����
			for (int i = 0; i < faceNum; i++)
			{
				//�ǂݍ���
				fscanf_s(fp, "%d", &pFace[i].m_UseMaterial);

				//,�܂���;�̏���
				fgets(str, sizeof(str), fp);
			}
		}
	}

	//�ǂݍ��݂̊J�n�ʒu�ɖ߂�
	fseek(fp, lStartPos, SEEK_SET);

	while (!feof(fp))
	{
		//�L�[���[�h �ǂݍ���
		fscanf_s(fp, "%s ", str, sizeof(str));

		//�}�e���A���ǂݍ���
		for (int i = 0; i < mateNum; i++)
		{
			if (strcmp(str, "Material") == 0)
			{
				//{����
				fgets(str, sizeof(str), fp);

				//�ʂ̐F
				fscanf_s(fp, "%f;%f;%f;%f;;", &x, &y, &z, &w);
				pMaterial[i].m_vFaceColor.x = x;
				pMaterial[i].m_vFaceColor.y = y;
				pMaterial[i].m_vFaceColor.z = z;
				pMaterial[i].m_vFaceColor.w = w;

				//�X�y�L�����[�̃p���[
				fscanf_s(fp, "%f;", &pMaterial[i].m_fPower);

				//�X�y�L�����[
				fscanf_s(fp, "%f;%f;%f;;", &x, &y, &z);
				pMaterial[i].m_vKs.x = x;
				pMaterial[i].m_vKs.y = y;
				pMaterial[i].m_vKs.z = z;

				//�G�~�b�V�u
				fscanf_s(fp, "%f;%f;%f;;", &x, &y, &z);
				pMaterial[i].m_vKe.x = x;
				pMaterial[i].m_vKe.y = y;
				pMaterial[i].m_vKe.z = z;

				//�e�N�X�`��
				fscanf_s(fp, "%s ", str, sizeof(str));
				if (strcmp(str, "TextureFilename") == 0)
				{
					fgets(str, sizeof(str), fp);//{����

					//�ǂݍ��ݍő�T�C�Y
					int size = sizeof(pMaterial[i].m_TexName);

					//�e�N�X�`����
					fscanf_s(fp, "%s", pMaterial[i].m_TexName, size);

					//"��;����������
					ErasCharFromString(pMaterial->m_TexName, size, '\"');
					ErasCharFromString(pMaterial->m_TexName, size, ';');

					//�e�N�X�`���[���쐬
					if (FAILED(D3DX10CreateShaderResourceViewFromFileA(DX->GetDevice(), pMaterial[i].m_TexName, NULL, NULL, &pMaterial[i].m_pTexture, NULL)))
					{
						//�e�N�X�`���쐬���s
						return false;
					}
				}
			}
		}
	}

	//���_�\���̃������m��
	pVertex = new X_VERTEX[verNum];

	//���_�\���̂ɂ܂Ƃ߂�
	for (int i = 0; i < verNum; i++)
	{
		pVertex[i].m_vPos = pvPos[i];
		pVertex[i].m_vNorm = pvNormal[i];
		pVertex[i].m_vTex = pvTex[i];
	}

	//�}�e���A�����ɏ��𐮗�����
	for (int i = 0; i < mateNum; i++)
	{
		//���̃}�e���A�����g�p����ʂ̐��𐔂���
		int UseFaceNum = 0;
		for (int j = 0; j < faceNum; j++)
		{
			if (pFace[j].m_UseMaterial == i)
			{
				UseFaceNum++;
			}
		}

		//�ʂ̐�
		pMaterial[i].m_FaceNum = UseFaceNum;

		//�ʂ̃��X�g�������m��
		pMaterial[i].m_pFaceIndex = new int[UseFaceNum];

		//�ʂ̃C���f�b�N�X��ۑ�
		int face_count = 0;
		for (int j = 0; j < faceNum; j++)
		{
			//���̃}�e���A�����g�p����ʂ̂�
			if (pFace[j].m_UseMaterial == i)
			{
				pMaterial[i].m_pFaceIndex[face_count] = j;
				face_count++;
			}
		}

		//�ʂ̐������������m��
		pMaterial[i].m_ppIndexBuffer = new ID3D10Buffer * [pMaterial[i].m_FaceNum];

		//�ʂ̐������C���f�b�N�X�o�b�t�@�쐬
		for (int j = 0; j < pMaterial[i].m_FaceNum; j++)
		{
			X_FACE face = pFace[pMaterial[i].m_pFaceIndex[j]];

			//�C���f�b�N�X�o�b�t�@�쐬
			pMaterial[i].m_ppIndexBuffer[j] = DRAW->BufferCreate(face.m_pIndex, sizeof(int) * face.m_FaceOfVer, D3D10_BIND_INDEX_BUFFER);
		}
	}

	//�ꎞ�ۑ����烁�b�V���|�C���^�փf�[�^���ڂ�
	pMesh->m_MaterialNum = mateNum;
	pMesh->m_pMaterial	= pMaterial;
	pMesh->m_FaceNum	= faceNum;
	pMesh->m_pFace		= pFace;
	pMesh->m_pVertex	= pVertex;
	pMesh->m_VerNum		= verNum;

	//�ꎞ�ۑ��͔j��
	PointerRelease(pvPos);
	PointerRelease(pvNormal);
	PointerRelease(pvTex);

	return true;
}

//�w�蕶���𕶎��񂩂����
void CX_Loader::ErasCharFromString(char* pSource, int Size, char Erace)
{
	int count = 0;//�����J�E���g

	for (int i = 0; i < Size; i++)
	{
		if (pSource[i] == Erace)
		{
			//���������J�E���g
			count++;
		}
		else
		{
			//�J�E���g�����l�߂ăR�s�[
			pSource[i - count] = pSource[i];
		}
	}
}

//�X�L�����b�V���w�b�_�[�ǂݍ���
void CX_Loader::LoadSkinMeshHeader(FILE* fp, X_SKIN_MESH_HEADER* pSkinHeader, long lStartPos)
{
	//�t�@�C���̐擪�ɃZ�b�g
	fseek(fp, lStartPos, SEEK_SET);

	//�L�[���[�h�ǂݍ��ݗp
	char str[READ_ARRAY_SIZE];

	while (!feof(fp))
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		//�X�L�����b�V���w�b�_�[�ǂݍ���
		if (strcmp(str, "XSkinMeshHeader") == 0)
		{
			//{����
			fscanf_s(fp, "%s", str, sizeof(str));

			//���_�̍ő�E�F�C�g��
			fscanf_s(fp, "%d;", &pSkinHeader->m_MaxVertex);

			//�ʂ̍ő�E�F�C�g��
			fscanf_s(fp, "%d;", &pSkinHeader->m_MaxFace);

			//�{�[����
			fscanf_s(fp, "%d;", &pSkinHeader->m_BoneNum);

			//}����
			fscanf_s(fp, "%s", str, sizeof(str));
		}
	}
}

//�{�[�����̎擾
int CX_Loader::GetBoneNum(FILE* fp, long lStartPos)
{
	//�t�@�C���̐擪�ɃZ�b�g
	fseek(fp, lStartPos, SEEK_SET);

	int boneNum = 0;	//�{�[���̐�

	//�L�[���[�h�ǂݍ��ݗp
	char str[READ_ARRAY_SIZE];

	//�{�[���̐��𐔂���
	while (!feof(fp))
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		if (strcmp(str, "Frame") == 0)
		{
			boneNum++;
		}
	}

	return boneNum;
}

//�{�[���ǂݍ���
bool CX_Loader::LoadBone(FILE* fp, X_BONE* pBone, long lStartPos)
{
	//�t�@�C���̐擪�ɃZ�b�g
	fseek(fp, lStartPos, SEEK_SET);

	int boneNum = 0;	//�{�[���̐�

	//�L�[���[�h�ǂݍ��ݗp
	char str[READ_ARRAY_SIZE];

	int start_count = 0;//{�𐔂���
	int end_count	= 0;//}�𐔂���

	int boneIndex = 0;//�C���f�b�N�X�J�E���^�[

	//�{�[���ǂݍ���
	while (!feof(fp))
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		//�{�[��
		if (strcmp(str, "Frame") == 0)
		{
			//�{�[�������X�g�ɕۑ�
			X_BONE bone = LoadBoneInfo(fp, &boneIndex, pBone);
			pBone[bone.m_index] = bone;
		}
	}
	return true;
}

//�{�[�����̓ǂݍ���
X_BONE CX_Loader::LoadBoneInfo(FILE* fp, int* pBoneIndex, X_BONE* pBone)
{
	//�֐��Ăяo�����̃t�@�C���̈ʒu��ۑ�
	long ReadStartPos = ftell(fp);

	int start_count = 0;//{�J�E���g
	int end_count	= 0;//}�J�E���g
	int childNum	= 0;//�q�{�[���̐�

	//�ǂݍ��ݗp
	char str[READ_ARRAY_SIZE];

	//�{�[���ǂݍ��ݗp
	X_BONE bone;

	//���g�̃C���f�b�N�X
	bone.m_index = *(pBoneIndex);

	char test[READ_ARRAY_SIZE];

	fscanf_s(fp, "%s ", test, sizeof(test));

	//��Ɏq�{�[���̐��𐔂���
	while (start_count != end_count || start_count == 0 || end_count == 0)
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		//{�J�E���g
		if (strcmp(str, "{") == 0)
		{
			start_count++;
		}

		//}�J�E���g
		if (strcmp(str, "}") == 0)
		{
			end_count++;
		}
		//�q�{�[��
		if (strcmp(str, "Frame") == 0)
		{
			//���̃{�[���̒����̎q
			if (start_count - end_count == 1)
			{
				childNum++;
			}
			else
			{
				;//���{�[���Ȃǂ̓J�E���g���Ȃ�
			}
		}
	}

	//���̊֐����Ă΂ꂽ�Ǝ��̈ʒu�ɖ߂�
	fseek(fp, ReadStartPos, SEEK_SET);

	//�q�̐���ۑ�
	bone.m_ChildNum = childNum;

	//�q�{�[���̃C���f�b�N�X���X�g�������m��
	bone.m_pChildIndex = new int[childNum];

	//�J�E���^��������
	start_count = 0;
	end_count	= 0;
	childNum	= 0;

	fscanf_s(fp, "%s", bone.m_Name, sizeof(bone.m_Name));

	//�{�ǂݍ���
	while (start_count != end_count || start_count == 0 || end_count == 0)
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		//{�J�E���g
		if (strcmp(str, "{") == 0)
		{
			start_count++;
		}

		//}�J�E���g
		if (strcmp(str, "}") == 0)
		{
			end_count++;
		}

		//�����|�[�Y
		if (strcmp(str, "FrameTransformMatrix") == 0)
		{
			//{����
			fgets(str, sizeof(str), fp);

			//�s��ǂݍ���
			D3DXMATRIX mat;
			fgets(str, sizeof(str), fp);
			sscanf_s(str, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f;;",
				&mat._11, &mat._12, &mat._13, &mat._14,
				&mat._21, &mat._22, &mat._23, &mat._24,
				&mat._31, &mat._32, &mat._33, &mat._34,
				&mat._41, &mat._42, &mat._43, &mat._44);

			//�ۑ�
			bone.m_matBindPose = mat;

			//}����
			fgets(str, sizeof(str), fp);
		}

		//�q�{�[��
		if (strcmp(str, "Frame") == 0)
		{
			//�{�[���̃C���f�b�N�X�X�V
			*pBoneIndex = *(pBoneIndex)+1;

			//�q�{�[���̃C���f�b�N�X��ۑ�
			bone.m_pChildIndex[childNum++] = *pBoneIndex;

			//�{�[�������X�g�ɕۑ�
			X_BONE read = LoadBoneInfo(fp, pBoneIndex, pBone);
			pBone[read.m_index] = read;
		}
	}

	return bone;
}


//�X�L���E�F�C�g�̐����擾
int CX_Loader::GetSkinWeightNum(FILE* fp, long lStartPos)
{
	//�ǂݍ��݊J�n�ʒu�ɃZ�b�g����
	fseek(fp, lStartPos, SEEK_SET);

	//�X�L���E�F�C�g�̐�
	int SkinWeightNum = 0;

	//�L�[���[�h�ǂݍ��ݗp
	char str[READ_ARRAY_SIZE];

	//�X�L���E�F�C�g�̐����J�E���g����
	while (!feof(fp))
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		if (strcmp(str, "SkinWeights") == 0)
		{
			SkinWeightNum++;
		}
	}

	return SkinWeightNum;
}

//�X�L���E�F�C�g�̓ǂݍ���
bool CX_Loader::LoadSkinWeight(FILE* fp, X_SKIN_WEIGHT* pSkinWeight, long lStartPos)
{
	//�ǂݍ��݊J�n�ʒu�ɃZ�b�g����
	fseek(fp, lStartPos, SEEK_SET);

	//�L�[���[�h�ǂݍ��ݗp
	char str[READ_ARRAY_SIZE];

	//�ǂݍ��ݗp
	char boneName[NAME_ARRAY_SIZE];	//�{�[����
	int count		= 0;			//�J�E���^�[		
	int weightNum	= 0;			//�E�F�C�g��
	D3DXMATRIX mat;					//�s��

	//�ǂݍ���
	while (!feof(fp))
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		//�X�L���E�F�C�g
		if (strcmp(str, "SkinWeights") == 0)
		{
			fgets(str, sizeof(str), fp);//{����

			//�{�[����
			fscanf_s(fp, "%s", boneName, sizeof(boneName));

			//"��;����������
			ErasCharFromString(boneName, sizeof(boneName), '\"');
			ErasCharFromString(boneName, sizeof(boneName), ';');

			//�ۑ�
			strcpy_s(pSkinWeight[count].m_BoneName, boneName);

			//�E�F�C�g�̐�
			fscanf_s(fp, "%d;", &weightNum);
			pSkinWeight[count].m_WeightNum = weightNum;

			//�C���f�b�N�X�ƃE�F�C�g�̃������m��
			pSkinWeight[count].m_pIndex = new int[weightNum];
			pSkinWeight[count].m_pWeight = new float[weightNum];

			//�C���f�b�N�X�ǂݍ���
			for (int i = 0; i < weightNum; i++)
			{
				fscanf_s(fp, "%d", &pSkinWeight[count].m_pIndex[i]);

				//,�܂���;�̏���
				fgets(str, sizeof(str), fp);
			}

			//�E�F�C�g�ǂݍ���
			for (int i = 0; i < weightNum; i++)
			{
				fscanf_s(fp, "%f", &pSkinWeight[count].m_pWeight[i]);
				//,�܂���;�̏���
				fgets(str, sizeof(str), fp);
			}
			//�I�t�Z�b�g�s��
			fscanf_s(fp, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f;;",
				&mat._11, &mat._12, &mat._13, &mat._14,
				&mat._21, &mat._22, &mat._23, &mat._24,
				&mat._31, &mat._32, &mat._33, &mat._34,
				&mat._41, &mat._42, &mat._43, &mat._44);
			pSkinWeight[count].m_matOffset = mat;

			//�J�E���^�[�X�V
			count++;
		}
	}
	return true;
}

//�A�j���[�V�����̐����擾
int CX_Loader::GetAnimeNum(FILE* fp, long lStartPos)
{
	//�ǂݍ��݊J�n�ʒu�ɃZ�b�g����
	fseek(fp, lStartPos, SEEK_SET);

	//�A�j���[�V�����̐�
	int animeNum = 0;

	//�L�[���[�h�ǂݍ��ݗp
	char str[READ_ARRAY_SIZE];

	//�A�j���[�V�����̐��𐔂���
	while (!feof(fp))
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		if (strcmp(str, "AnimationSet") == 0)
		{
			animeNum++;
		}
	}
	return animeNum;
}

//�X�L���E�F�C�g�̏������ƂɊe���_�ɑΉ��{�[���ƃE�F�C�g�̏�����������
void CX_Loader::VertexMatchBone(X_SKIN_MESH* pSkin)
{
	//�Ή��{�[����
	char boneName[NAME_ARRAY_SIZE];

	for (int i = 0; i < pSkin->m_WeightNum; i++)
	{
		//�Ή��{�[����
		strcpy_s(boneName, pSkin->m_pWeight[i].m_BoneName);

		//�Ή��{�[����T��
		bool bFind = false;
		int boneID = -1;
		for (int j = 0; j < pSkin->m_BoneNum && bFind == false; j++)
		{
			//�Ή��{�[������
			if (strcmp(pSkin->m_pBone[j].m_Name, boneName) == 0)
			{
				bFind = true;
				boneID = j;
			}
		}

		//�Ή��{�[���ɃI�t�Z�b�g�s���n��
		pSkin->m_pBone[boneID].m_matOffset = pSkin->m_pWeight[i].m_matOffset;

		//�Ή����_�Ƀ{�[��ID�ƃE�F�C�g��n��
		for (int j = 0; j < pSkin->m_pWeight[i].m_WeightNum; j++)
		{
			X_VERTEX ver = pSkin->m_Mesh.m_pVertex[pSkin->m_pWeight[i].m_pIndex[j]];

			//�{�[��ID
			ver.m_BoneIndex[ver.m_WeightNum] = boneID;

			//�E�F�C�g
			ver.m_fWeight[ver.m_WeightNum] = pSkin->m_pWeight[i].m_pWeight[j];

			//�E�F�C�g���X�V
			ver.m_WeightNum++;

			//�ۑ�
			pSkin->m_Mesh.m_pVertex[pSkin->m_pWeight[i].m_pIndex[j]] = ver;
		}
	}
}

//�{�[�����̃L�[���̓ǂݍ���
X_BONE_KEY CX_Loader::LoadBoneKey(FILE* fp)
{
	X_BONE_KEY Out;

	//�L�[���[�h�ǂݍ��ݗp
	char str[READ_ARRAY_SIZE];

	//�Ή��{�[����
	char boneName[NAME_ARRAY_SIZE];

	//�ǂݍ��ݗp������
	int keyNum = 0;
	int keyCount = 0;
	int valueNum = 0;
	int count = 0;//�J�E���^�[

	int start_count = 0;	//{�J�E���g
	int end_count = 0;		//}�J�E���g

	//�֐��Ăяo�����̃t�@�C���̈ʒu��ۑ�
	long ReadStartPos = ftell(fp);

	//�A�j���[�V�����L�[�̐����擾
	int AnimeKeyNum = GetAnimeKeyNum(fp);

	//�A�j���[�V�����L�[�̃������m��
	Out.m_pAniKey = new X_ANIMATOIN_KEY[AnimeKeyNum];

	//�A�j���[�V�����L�[�̐��ۑ�
	Out.m_AniKeyNum = AnimeKeyNum;

	//���̊֐����Ă΂ꂽ�Ǝ��̈ʒu�ɖ߂�
	fseek(fp, ReadStartPos, SEEK_SET);

	//�A�j���[�V������
	fscanf_s(fp, "%s ", str, sizeof(str));

	//�A�j���[�V���������Ȃ��ꍇ
	if (strcmp(str, "{") == 0)
	{
		start_count++;
	}

	while (start_count != end_count || start_count == 0 || end_count == 0)
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		//{�J�E���g
		if (strcmp(str, "{") == 0)
		{
			start_count++;

			//�Ή��{�[����
			if (start_count - end_count >= 2)
			{
				fscanf_s(fp, "%s ", boneName, sizeof(boneName));

				//�ۑ�
				strcpy_s(Out.m_AffectBoneName, boneName);
			}						
		}
		
		//}�J�E���g
		if (strcmp(str, "}") == 0)
			end_count++;


		//�A�j���[�V�����L�[�ǂݍ���
		if (strcmp(str, "AnimationKey") == 0)
		{
			//{����
			fgets(str, sizeof(str), fp);

			//�ǂݍ���
			Out.m_pAniKey[count++]=LoadAnimationKey(fp);

			//}����
			fgets(str, sizeof(str), fp);
		}
	}
	return Out;
}

//�A�j���[�V�����L�[�̐����擾����
int CX_Loader::GetAnimeKeyNum(FILE* fp)
{
	int Out = 0;

	//�L�[���[�h�ǂݍ��ݗp
	char str[READ_ARRAY_SIZE];

	int start_count = 0;	//{�J�E���g
	int end_count = 0;		//}�J�E���g

	while (start_count != end_count || start_count == 0 || end_count == 0)
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		//{�J�E���g
		if (strcmp(str, "{") == 0)
			start_count++;
		
		//}�J�E���g
		if (strcmp(str, "}") == 0)
			end_count++;
		
		//�A�j���[�V�����L�[���J�E���g
		if (strcmp(str, "AnimationKey") == 0)
			Out++;
	}
	return Out;
}

//�A�j���[�V�����L�[�̓ǂݍ���
X_ANIMATOIN_KEY CX_Loader::LoadAnimationKey(FILE* fp)
{
	X_ANIMATOIN_KEY Out;

	//�L�[���[�h�ǂݍ��ݗp
	char str[READ_ARRAY_SIZE];

	int keyNum = -1;//�L�[�̐�
	int valueNum = -1;//�l�̐�
	int keyCount = 0;//�L�[�J�E���^�[

	//�L�[�^�C�v
	fscanf_s(fp, "%d;", &Out.m_KeyType);

	//�L�[�̐�
	fscanf_s(fp, "%d;", &keyNum);
	Out.m_KeyNum = keyNum;

	//�L�[�̃������m��
	Out.m_pKey = new X_KEY[keyNum];

	//�L�[�̓ǂݍ���
	for (int i = 0; i < keyNum; i++)
	{
		//�R�}
		fscanf_s(fp, "%d;", &Out.m_pKey[i].m_Time);

		//�f�[�^�̐�
		fscanf_s(fp, "%d;", &valueNum);
		Out.m_pKey[i].m_ValueNum = valueNum;

		//�f�[�^�̃������m��
		Out.m_pKey[i].m_pfValue = new float[valueNum];

		//�J�E���^�[������
		keyCount = 0;

		//�ŏ��̈��
		fscanf_s(fp, "%f", &Out.m_pKey[i].m_pfValue[keyCount++]);

		//2�ڈȍ~��,���܂߂ēǂ�
		while (keyCount < Out.m_pKey[i].m_ValueNum)
		{
			fscanf_s(fp, ",%f", &Out.m_pKey[i].m_pfValue[keyCount++]);
		}

		//;;,������
		fgets(str, sizeof(str), fp);
	}

	return Out;
}


//�A�j���[�V�����ǂݍ���
bool CX_Loader::LoadAnimation(FILE* fp, X_ANIMATION* pAnime, long lStartPos)
{
	//�ǂݍ��݊J�n�ʒu�ɃZ�b�g����
	fseek(fp, lStartPos, SEEK_SET);

	//�L�[���[�h�ǂݍ��ݗp
	char str[READ_ARRAY_SIZE];

	//�ǂݍ��ݗp
	char animeName[NAME_ARRAY_SIZE];	//�A�j���[�V������
	int animeCount = 0;					//�A�j���[�V�����J�E���^�[

	while (!feof(fp))
	{
		fscanf_s(fp, "%s ", str, sizeof(str));

		//�A�j���[�V����
		if (strcmp(str, "AnimationSet") == 0)
		{
			//�A�j���[�V������
			fscanf_s(fp, "%s ", animeName, sizeof(animeName));
			strcpy_s(pAnime[animeCount].m_Name, animeName);

			//�t�@�C���̈ʒu��ۑ�
			long ReadStartPos = ftell(fp);

			int boneNum = 0;
			int start_count = 0;
			int end_count = 0;

			//��ɑΉ��{�[���̐��𐔂���
			while (start_count != end_count || start_count == 0 || end_count == 0)
			{
				fscanf_s(fp, "%s ", str, sizeof(str));

				//{�J�E���g
				if (strcmp(str, "{") == 0)
				{
					start_count++;
				}

				//}�J�E���g
				if (strcmp(str, "}") == 0)
				{
					end_count++;
				}

				//�Ή��{�[��
				if (strcmp(str, "Animation") == 0)
				{
					boneNum++;
				}
			}

			//�Ή��{�[�����ۑ�
			pAnime[animeCount].m_BoneKeyNum = boneNum;

			//�������m��
			pAnime[animeCount].m_pBoneKey = new X_BONE_KEY[boneNum];

			//�A�j���[�V�����ǂݍ��݈ʒu�ɃZ�b�g����
			fseek(fp, ReadStartPos, SEEK_SET);

			//�Ή��{�[�����ǂݍ���
			int boneCount = 0;
			while (!feof(fp) && (boneCount < boneNum))
			{
				fscanf_s(fp, "%s ", str, sizeof(str));

				if (strcmp(str, "Animation") == 0)
				{
					//�{�[�����̃L�[�ǂݍ���
					pAnime[animeCount].m_pBoneKey[boneCount] = LoadBoneKey(fp);
					boneCount++;
				}
			}
			//�J�E���^�[�X�V
			animeCount++;
		}
	}
	return true;
}

//�X�L�����b�V���ɂ܂Ƃ߂�
void CX_Loader::SkinMeshPutTogether(X_MESH Mesh, X_BONE* pBone, int BoneNum, X_SKIN_WEIGHT* pSkinWeight, int WeightNum, X_ANIMATION* pAnimation, int AnimeNum, X_SKIN_MESH* pSkinMesh, X_SKIN_MESH_HEADER SkinHeader)
{
	pSkinMesh->m_Mesh = Mesh;						//���b�V��
	pSkinMesh->m_BoneNum = BoneNum;					//�{�[����
	pSkinMesh->m_pBone = pBone;						//�{�[��
	pSkinMesh->m_WeightNum = WeightNum;				//�E�F�C�g��	
	pSkinMesh->m_pWeight = pSkinWeight;				//�E�F�C�g���X�g
	pSkinMesh->m_AnimeNum = AnimeNum;				//�A�j���[�V�����̐�
	pSkinMesh->m_pAnimation = pAnimation;			//�A�j���[�V����
	pSkinMesh->m_SkinHeader = SkinHeader;			//�X�L���w�b�_�[
	pSkinMesh->m_pRoot = &pSkinMesh->m_pBone[0];	//���[�g�{�[��
}