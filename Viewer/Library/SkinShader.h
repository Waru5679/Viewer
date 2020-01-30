#pragma once
#include "Main.h"
#include "Camera.h"
#include "LibraryHeader.h"
#include "X_Loader.h"
#include "XLoader_Skin.h"

//�V�F�[�_�N���X
class CSkinShader :public CSingleton<CSkinShader>
{
private:
	//�V���O���g��
	friend CSingleton<CSkinShader>;
	CSkinShader() {};	//�R���X�g���N�^
	~CSkinShader() {};	//�f�X�g���N�^
public:
	bool Init(ID3D10Device* pDevice);	//������
	void Release();						//���

	//�J�����Z�b�g
	void SetCamera(CCameraBase* pCamera) { m_pCamera = pCamera; }

	//�V�F�[�_�[�Z�b�g
	void SetShader(ID3D10ShaderResourceView* pTexture, RECT_F* pSrc, CColorData* pColor, D3DXMATRIX matWorld, X_SKIN_MESH* pSkin);

	//�C���v�b�g���C�A�E�g�̍쐬
	bool CreateLayOut(ID3D10Device* pDevice);

	//�A�N�Z�T
	ID3D10Effect* GetEffect() { return m_pEffect; }
	ID3D10EffectTechnique* GetTechnique() { return m_pTechnique; }
	ID3D10InputLayout* GetVertexLayOut() { return m_pVertexLayout; }
private:
	ID3D10Effect* m_pEffect;				//�G�t�F�N�g
	ID3D10EffectTechnique* m_pTechnique;	//�e�N�j�b�N
	ID3D10InputLayout* m_pVertexLayout;		//���_���C�A�E�g

	//�A�v�������V�F�[�_�[�˂���
	ID3D10EffectMatrixVariable* m_pShaderWorldViewProjection;	//���[���h����ˉe�܂ł̕ϊ��s��
	ID3D10EffectShaderResourceVariable* m_pShaderTexture;		//�e�N�X�`���[
	ID3D10EffectVectorVariable* m_pShaderSrc;					//�؂���ʒu
	ID3D10EffectVectorVariable* m_pShaderColor;					//�`��F
	ID3D10EffectMatrixVariable* m_pBone;						//�{�[���̃|�[�Y�s��

	CCameraBase* m_pCamera;	//�J�����|�C���^
};