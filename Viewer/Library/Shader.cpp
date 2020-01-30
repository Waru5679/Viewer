#include "Shader.h"
#include "Main.h"

//�C���X�^���X
CShader* CShader::m_pInstance = nullptr;

//������
bool CShader::Init(ID3D10Device* pDevice)
{
	//�G�t�F�N�g���쐬
	D3DX10CreateEffectFromFile(L"Geometry_Texture.fx", NULL, NULL, "fx_4_0",
		D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_DEBUG, 0,
		pDevice, NULL, NULL, &m_pEffect, NULL, NULL);

	//�e�N�j�b�N�𓾂�
	m_pTechnique = m_pEffect->GetTechniqueByName("Render");

	//�C���v�b�g���C�A�E�g�쐬
	if (CreateLayOut(pDevice) == false)
		return false;

	//�A�v�������V�F�[�_�[�˂���
	m_pShaderWorldViewProjection = m_pEffect->GetVariableByName("g_mWVP")->AsMatrix();
	m_pShaderTexture = m_pEffect->GetVariableByName("g_texDecal")->AsShaderResource();
	m_pShaderSrc = m_pEffect->GetVariableByName("g_UvSrc")->AsVector();
	m_pShaderColor = m_pEffect->GetVariableByName("g_Color")->AsVector();

	return true;
}

//�C���v�b�g���C�A�E�g�̍쐬
bool CShader::CreateLayOut(ID3D10Device* pDevice)
{
	//���_�C���v�b�g���C�A�E�g���`	
	D3D10_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//���_�C���v�b�g���C�A�E�g���쐬
	D3D10_PASS_DESC PassDesc;
	m_pTechnique->GetPassByIndex(0)->GetDesc(&PassDesc);
	if (FAILED(pDevice->CreateInputLayout(layout, numElements, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &m_pVertexLayout)))
	{
		return false;
	}
	return true;
}

//�V�F�[�_�[�Z�b�g
void CShader::SetShader(ID3D10ShaderResourceView* pTexture, RECT_F* pSrc,CColorData* pColor, D3DXMATRIX matWorld)
{	
	//���_�C���v�b�g���C�A�E�g���Z�b�g
	DX->GetDevice()->IASetInputLayout(m_pVertexLayout);

	//�J�������Ȃ��Ƃ�
	if (m_pCamera == nullptr)
		return;

	//���[���h���r���[*�v���W�F�N�V����
	D3DXMATRIX objWVP = matWorld *m_pCamera->GetViewMatrix() *m_pCamera->GetProjMatrix();
	m_pShaderWorldViewProjection->SetMatrix((float*)&(objWVP));
	
	//�؂���ʒu
	D3DXVECTOR4 vSrc;
	if (pSrc == NULL)
	{
		//�؂���ʒu�̎w�肪�Ȃ��Ƃ�
		vSrc = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	}
	else
	{
		vSrc = D3DXVECTOR4(pSrc->m_Top, pSrc->m_Left, pSrc->m_Right, pSrc->m_Bottom);
	}
	m_pShaderSrc->SetFloatVector(vSrc);

	//�`��F
	D3DXVECTOR4 vColor;
	if (pColor == NULL)
	{
		//�F�f�[�^�̎w�肪�Ȃ��Ƃ�
		vColor = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else
	{
		vColor = D3DXVECTOR4(pColor->m_Red, pColor->m_Green, pColor->m_Blue, pColor->m_Alpha);
	}
	m_pShaderColor->SetFloatVector(vColor);

	//�e�N�X�`����񂪂����
	if (pTexture != NULL)
	{
		m_pShaderTexture->SetResource(pTexture);
	}
}

//���
void CShader::Release()
{
	m_pVertexLayout->Release();
	m_pEffect->Release();

	//�J�����j��
	if(m_pCamera!=nullptr)
		PointerRelease(m_pCamera);

	//�C���X�^���X�j��
	PointerRelease(m_pInstance);
}