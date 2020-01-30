#pragma once
#include "Main.h"

constexpr int TRIANGLE_POLYGON	{ 3 };	//�O�p�|���S��
constexpr int QUAD_POLYGON		{ 4 };	//�l�p�|���S��
constexpr int READ_ARRAY_SIZE	{ 200 };//�ǂݍ��ݗp�L�[�z��̃T�C�Y
constexpr int NAME_ARRAY_SIZE	{ 100 };//���O�z��̃T�C�Y
constexpr int MAX_VER_WEIGH		{ 4 };	//���_�̍ő�E�F�C�g

//�A�j���[�V�����̃L�[�^�C�v
enum KEY_TYPE
{
	ROT,	//��]
	SCALE,	//�X�P�[��
	TRANS,	//�ړ�
	MATRIX,	//�s��
};

//���_�\����
struct X_VERTEX
{
	X_VERTEX()
	{
		for (int i = 0; i < MAX_VER_WEIGH; i++)
		{
			m_BoneIndex[i] = -1;
			m_fWeight[i] = 0.0f;
		}
		m_WeightNum = 0;
	}

	D3DXVECTOR3 m_vPos;	//���_�ʒu
	D3DXVECTOR3 m_vNorm;//���_�@��
	D3DXVECTOR2 m_vTex;	//UV���W

	int m_BoneIndex[MAX_VER_WEIGH];	//�e�����󂯂�{�[���̃C���f�b�N�X
	float m_fWeight[MAX_VER_WEIGH];	//�E�F�C�g
	int m_WeightNum;				//�E�F�C�g��
};

//�|���S��
struct X_FACE
{
	X_FACE()
	{
		m_FaceOfVer = -1;
		m_pIndex = nullptr;
		m_UseMaterial = -1;
	}
	int m_FaceOfVer;	//�\�����钸�_�̐�
	int* m_pIndex;		//�\�����钸�_�̃C���f�b�N�X
	int m_UseMaterial;	//�g�p����}�e���A��
};

//�}�e���A���\����
struct X_MATERIAL
{
	X_MATERIAL()
	{
		ZeroMemory(this, sizeof(X_MATERIAL));
		m_pTexture = nullptr;
		m_pFaceIndex = nullptr;
		m_ppIndexBuffer = nullptr;
	}

	char						m_TexName[NAME_ARRAY_SIZE];	//�t�@�C����
	D3DXVECTOR4					m_vFaceColor;				//�ʂ̐F
	D3DXVECTOR3					m_vKs;						//�X�y�L�����[
	float						m_fPower;					//�X�y�L�����[�̃p���[
	D3DXVECTOR3					m_vKe;						//�G�~�b�V�u
	int							m_FaceNum;					//���̃}�e���A�����g�p����ʂ̐�
	int*						m_pFaceIndex;				//���̃}�e���A���Ŏg�p����ʂ̃C���f�b�N�X���X�g
	ID3D10Buffer**				m_ppIndexBuffer;			//�C���f�b�N�X�o�b�t�@
	ID3D10ShaderResourceView*	m_pTexture;					//�e�N�X�`���|�C���^	
};

//�{�[���\����
struct X_BONE
{
	X_BONE()
	{
		m_pChildIndex = nullptr;
		D3DXMatrixIdentity(&m_matBindPose);
		D3DXMatrixIdentity(&m_matNewPose);
		D3DXMatrixIdentity(&m_matOffset);
	}
	char		m_Name[NAME_ARRAY_SIZE];	//�{�[����
	int			m_index;					//���g�̃C���f�b�N�X
	int			m_ChildNum;					//�q�̐�
	int*		m_pChildIndex;				//�����̎q�̃C���f�b�N�X���X�g
	D3DXMATRIX	m_matBindPose;				//�����|�[�Y�i�����ƕς��Ȃ��j
	D3DXMATRIX	m_matNewPose;				//���݂̃|�[�Y�i���̓s�x�ς��j
	D3DXMATRIX  m_matOffset;				//�I�t�Z�b�g�s��
};

//�ǂ̃{�[�����x�̒��_�ɂǂꂾ���̉e����^���邩
struct X_SKIN_WEIGHT
{
	X_SKIN_WEIGHT()
	{
		m_WeightNum = -1;
		m_pIndex = nullptr;
		m_pWeight = nullptr;
		D3DXMatrixIdentity(&m_matOffset);
	}

	char		m_BoneName[NAME_ARRAY_SIZE];//�{�[����
	int			m_WeightNum;				//�E�F�C�g�̐�
	int*		m_pIndex;					//�e�����󂯂钸�_�̃C���f�b�N�X���X�g
	float*		m_pWeight;					//�E�F�C�g���X�g
	D3DXMATRIX	m_matOffset;				//�I�t�Z�b�g�s��
};

//�A�j���[�V�����̃L�[
struct X_KEY
{
	X_KEY()
	{
		m_Time = -1;
		m_ValueNum = -1;
		m_pfValue = nullptr;
	}

	int		m_Time;		//�R�}
	int		m_ValueNum;	//�l�̐�
	float*	m_pfValue;	//�l�̃��X�g
};

struct  X_ANIMATOIN_KEY
{
	X_ANIMATOIN_KEY()
	{
		m_KeyNum = -1;
		m_KeyType = -1;
		m_pKey = nullptr;
	}
	int		m_KeyType;	//�L�[�^�C�v
	int		m_KeyNum;	//�L�[�̐�
	X_KEY*	m_pKey;		//�L�[���X�g
};

//�{�[���̃L�[
struct X_BONE_KEY
{
	X_BONE_KEY()
	{
		m_AniKeyNum = -1;
		m_pAniKey = nullptr;
	}

	char	m_AffectBoneName[NAME_ARRAY_SIZE];	//�e�����󂯂�{�[����
	int		m_AniKeyNum;						//�L�[�̐�

	X_ANIMATOIN_KEY*	m_pAniKey;				//�L�[���X�g
};

//�A�j���[�V�����\����
struct X_ANIMATION
{
	X_ANIMATION()
	{
		m_BoneKeyNum = -1;
		m_pBoneKey = nullptr;
	}

	char		m_Name[NAME_ARRAY_SIZE];//�A�j���[�V������
	int			m_BoneKeyNum;			//�Ή��{�[���̐�
	X_BONE_KEY*	m_pBoneKey;				//�{�[���̃L�[���X�g
};

//���b�V��
struct X_MESH
{
	X_MESH()
	{
		m_VerNum = -1;
		m_FaceNum = -1;
		m_MaterialNum = -1;
		m_pFace = nullptr;
		m_pMaterial = nullptr;
		m_pVertexBuffer = nullptr;
		m_pVertex = nullptr;
	}
	int				m_VerNum;		//���_��
	int				m_FaceNum;		//��(�|���S��)��
	X_FACE*			m_pFace;		//�ʃ��X�g
	int				m_MaterialNum;	//�}�e���A���̐�
	X_MATERIAL*		m_pMaterial;	//�}�e���A�����X�g
	ID3D10Buffer*	m_pVertexBuffer;//���_�o�b�t�@
	X_VERTEX*			 m_pVertex;		//���_���̃��X�g
};

//�X�L�����b�V���w�b�_�[
struct X_SKIN_MESH_HEADER
{
	X_SKIN_MESH_HEADER()
	{
		m_MaxVertex = -1;
		m_MaxFace = -1;
		m_BoneNum = -1;
	}
	int	m_MaxVertex;//���_�̍ő�E�F�C�g��
	int m_MaxFace;	//�ʂ̍ő�E�F�C�g��
	int m_BoneNum;	//�{�[����
};

//�X�L�����b�V��
struct X_SKIN_MESH
{
	X_SKIN_MESH()
	{
		m_Mesh = X_MESH();
		m_BoneNum = -1;
		m_pBone = nullptr;
		m_SkinHeader = X_SKIN_MESH_HEADER();
		m_WeightNum = -1;
		m_pWeight = nullptr;
		m_AnimeNum = -1;
		m_pAnimation = nullptr;
		m_pRoot = nullptr;
	}

	X_MESH				m_Mesh;			//���b�V��
	int					m_BoneNum;		//�{�[����
	X_BONE*				m_pBone;		//�{�[�����X�g
	X_SKIN_MESH_HEADER	m_SkinHeader;	//�X�L�����b�V���w�b�_�[
	int					m_WeightNum;	//�E�F�C�g��
	X_SKIN_WEIGHT*		m_pWeight;		//�X�L���E�F�C�g
	int					m_AnimeNum;		//�A�j���[�V������
	X_ANIMATION*		m_pAnimation;	//�A�j���[�V����
	X_BONE*				m_pRoot;		//���[�g�{�[��
};
