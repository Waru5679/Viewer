#include "LibraryHeader.h"

//�C���X�^���X
CDraw* CDraw::m_pInstance = nullptr;

//������
void CDraw::Init()
{
	//���_���
	VERTEX ver[]=
	{
		D3DXVECTOR3(-1.0f,  1.0f, 0.0f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR2(0.0f,0.0f),	//����
		D3DXVECTOR3( 1.0f,  1.0f, 0.0f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR2(1.0f,0.0f),	//�E��
		D3DXVECTOR3(-1.0f, -1.0f, 0.0f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR2(0.0f,1.0f),	//����
		D3DXVECTOR3( 1.0f, -1.0f, 0.0f),D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR2(1.0f,1.0f)	//�E��
	};
	
	//�o�[�e�b�N�X�o�b�t�@�쐬
	m_pVertexBuffer= BufferCreate(ver,sizeof(VERTEX)*4, D3D10_BIND_VERTEX_BUFFER);
}

//2D�`��(�e�N�X�`���̃T�C�Y����؂���ʒu�̐ݒ肪������̗p���p)
void CDraw::DrawTexture(int TexId, RECT_F* pSrc,RECT_F* pOut,CColorData* pColor,float fRad)
{
	//�e�N�X�`�����
	CTextureData* pData=nullptr;
	pData = TEXTURE->GetTexData(TexId);

	//�؂���ʒu�̐ݒ�
	RECT_F Src;
	Src.m_Top	= pSrc->m_Left	/ pData->GetWidth();
	Src.m_Left	= pSrc->m_Top	/ pData->GetHeight();
	Src.m_Right = pSrc->m_Right	/ pData->GetWidth();
	Src.m_Bottom= pSrc->m_Bottom/ pData->GetHeight();

	DrawTexture(pData->GetTexture(), &Src, pOut, pColor, fRad);
}

//2D�`��
void CDraw::DrawTexture(ID3D10ShaderResourceView* pTex, RECT_F* pSrc, RECT_F* pOut, CColorData* pColor, float fRad)
{
	if (m_pCamera == nullptr)
		return;

	//�t�r���[�s��
	D3DXMATRIX matInvView;
	D3DXMatrixIdentity(&matInvView);
	D3DXMatrixInverse(&matInvView, NULL, &m_pCamera->GetViewMatrix());

	//�t�v���W�F�N�V�����s��
	D3DXMATRIX matInvProj;
	D3DXMatrixIdentity(&matInvProj);
	D3DXMatrixInverse(&matInvProj, NULL, &m_pCamera->GetProjMatrix());

	//���[���h�s��
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);

	//�T�C�Y�����߂�
	D3DXVECTOR2 OutSize;
	OutSize.x = pOut->m_Right - pOut->m_Left;
	OutSize.y = pOut->m_Bottom - pOut->m_Top;
	matWorld._11 = OutSize.x / WINDOW_WIDTH;
	matWorld._22 = OutSize.y / WINDOW_HEIGHT;

	//Z����]�s��
	D3DXMATRIX matRot;
	D3DXMatrixRotationZ(&matRot, (float)D3DXToRadian(fRad));
	
	//���[���h�ɉ�]��������
	matWorld *= matRot;

	//1�s�N�Z���̑傫�������߂�
	D3DXVECTOR2 PixcelSize;
	PixcelSize.x = (2.0f / (float)WINDOW_WIDTH);
	PixcelSize.y = (2.0f / (float)WINDOW_HEIGHT);

	//�}�C�i�X����ɗ���悤�ɏ㉺���]����
	pOut->m_Top = WINDOW_HEIGHT - pOut->m_Bottom;
	pOut->m_Bottom = WINDOW_HEIGHT - pOut->m_Top;

	//���s�ړ��ʂ����߂�
	matWorld._41 = PixcelSize.x * pOut->m_Left + PixcelSize.x * (OutSize.x / 2.0f) - 1.0f;
	matWorld._42 = PixcelSize.y * pOut->m_Top + PixcelSize.y * (OutSize.y / 2.0f) - 1.0f;

	//�V�F�[�_�[�̃Z�b�g
	SHADER->SetShader(pTex, pSrc, pColor, matWorld *matInvProj*matInvView);

	//�|���S���̕`��
	DrawPolygon(4,m_pVertexBuffer,NULL);
}

//�o�b�t�@�[�쐬
ID3D10Buffer* CDraw::BufferCreate(void* pHead, unsigned int Size, int BufferType)
{
	D3D10_BUFFER_DESC bd;
	bd.Usage = D3D10_USAGE_DEFAULT;
	bd.ByteWidth = Size;
	bd.BindFlags = BufferType;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pHead;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	//�o�b�t�@�쐬
	ID3D10Buffer* pOut;
	if (FAILED(DX->GetDevice()->CreateBuffer(&bd, &InitData, &pOut)))
		return nullptr;

	return pOut;
}

//�|���S���̕`��
void CDraw::DrawPolygon(int VerNum, ID3D10Buffer* VertexBuffer, ID3D10Buffer* IndexBuffer)
{
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	DX->GetDevice()->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);

	//�C���f�b�N�X�o�b�t�@�[���Z�b�g
	if (IndexBuffer != NULL)
	{
		stride = sizeof(int);
		offset = 0;
		DX->GetDevice()->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	}

	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	if (VerNum == 3)
	{
		//�O�p�`
		DX->GetDevice()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
	if (VerNum == 4)
	{
		//�l�p�`
		DX->GetDevice()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	}

	//�v���~�e�B�u�������_�����O
	D3D10_TECHNIQUE_DESC dc;
	SHADER->GetTechnique()->GetDesc(&dc);
	for (UINT p = 0; p < dc.Passes; ++p)
	{
		SHADER->GetTechnique()->GetPassByIndex(p)->Apply(0);
		if (IndexBuffer != NULL)
		{
			DX->GetDevice()->DrawIndexed(VerNum, 0, 0);
		}
		else
		{
			DX->GetDevice()->Draw(VerNum, 0);
		}
	}
}

//���
void CDraw::Release()
{
	m_pVertexBuffer->Release();
	
	//�C���X�^���X�j��
	PointerRelease(m_pInstance);
}