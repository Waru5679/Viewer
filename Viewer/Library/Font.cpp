#include "LibraryHeader.h"

//�C���X�^���X
CFont* CFont::m_pInstance = nullptr;

//�����e�N�X�`���쐬
void CCharClass::CreateCharTex(wchar_t c, HDC hdc, TEXTMETRIC TM)
{
	//���ʕ����p
	UINT code = 0;	//�쐬���镶���R�[�h

	BYTE* ptr;			//�����̃r�b�g�}�b�v������
	DWORD size;			//�������T�C�Y
	GLYPHMETRICS GM;	//�ی`�����̏�񂪊i�[

	//Font�������݂̌���
	const MAT2 Mat = { {0,1}, {0,0},{0,0},{0,1} };	

	//�e�N�X�`���������ݗp�|�C���^
	D3D10_MAPPED_TEXTURE2D mapped;
	
	//�e�N�X�`���̃s�N�Z����������|�C���^
	BYTE* pBits;

	//���ʕ����R�[�h�o�^
	m_pc.reset(new wchar_t(c));
	code = (UINT)*m_pc.get();
	
	//�t�H���g��񂩂當���̃r�b�g�}�b�v�擾
	//�����̃r�b�g�}�b�v�̑傫���擾
	size = GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &GM, 0, NULL, &Mat);
	ptr = new BYTE[size];
	
	//�����̃r�b�g�}�b�v����ptr�ɓ����
	GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &GM, size, ptr, &Mat);

	//��e�N�X�`���̐ݒ�
	ID3D10Texture2D* pTexture;
	pTexture = CreateTexture();
	
	//ShaderResourceView���쐬
	m_pTexResView = CreateResourceView(pTexture);

	//�e�N�X�`�����b�N
	pTexture->Map(D3D10CalcSubresource(0, 0, 1),
		D3D10_MAP_WRITE_DISCARD,0,&mapped);
	pBits = (BYTE*)mapped.pData;

	//�����o���ʒu�i����)
	unsigned int iOfs_x = GM.gmptGlyphOrigin.x;
	unsigned int iOfs_y = TM.tmAscent - GM.gmptGlyphOrigin.y;
	
	//�t�H���g�r�b�g�}�b�v�̃T�C�Y
	unsigned int iBmp_w = GM.gmBlackBoxX + (4 - (GM.gmBlackBoxX % 4)) % 4;
	unsigned int iBmp_h = GM.gmBlackBoxY;
	
	//���l�̒i�K
	int Level = 17; //GGO_GRAY4�QBITMAP�Ȃ̂�17

	DWORD dwColor,dwFontColor,dwBackColor;
	
	//�w�i�F
	dwBackColor = 0x00ffffff;//����(��	
	
	//�t�H���g�F
	dwFontColor = 0xffffffff;//��

	//1�s�N�Z���P�ʂɃt�H���g�̐F���(32bit)���e�N�X�`���ɏ�������
	memset(pBits, 0x00, sizeof(pBits)*32*32);

	for(unsigned int y=0;y<32;y++)
	{
		for (unsigned int x = 0; x < 32; x++)
		{
			//�����͈̔�
			if (iOfs_y <= y && y < iOfs_y + iBmp_h
				&& iOfs_x<=x && x< iOfs_x + GM.gmBlackBoxX)
			{
				dwColor = (255 * ptr[x - iOfs_x + iBmp_w * (y - iOfs_y)]) / (Level - 1);

				//�F�w��
				if (dwColor == 0)
				{
					//�󔒕�
					dwColor = dwBackColor;
				}
				else
				{
					//������
					dwColor = dwFontColor;
				}
			}
			else
			{
				//��
				dwColor = dwBackColor;
			}
			memcpy((BYTE*)pBits + (y << 7) + (x << 2), &dwColor, sizeof(DWORD));
		}
	}
		
	//�e�N�X�`���A�����b�N
	pTexture->Unmap(D3D10CalcSubresource(0, 0, 1));
	
	//�f�[�^�폜 
	delete[] ptr;
	 
	pTexture->Release();
}

//��e�N�X�`���쐬
ID3D10Texture2D* CCharClass::CreateTexture()
{
	D3D10_TEXTURE2D_DESC desc;
	memset(&desc, 0, sizeof(desc));

	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		//�e�N�X�`���t�H�[�}�b�gR8G8B8��24Bit
	desc.SampleDesc.Count = 1;						//�T���v�����O��1�s�N�Z���̂�
	desc.Usage = D3D10_USAGE_DYNAMIC;				//CPU�������݉\
	desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;	//�V�F�[�_���\�[�X
	desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;	//CPU���珑�����݃A�N�Z�X��
	desc.Height = 32;
	desc.Width = 32;
	
	//�ݒ�����ɋ�e�N�X�`�����쐬
	ID3D10Texture2D* pOut;
	if (FAILED(DX->GetDevice()->CreateTexture2D(&desc, 0, &pOut)))
		return nullptr;

	return pOut;
}

//ShaderResourceView�쐬
ID3D10ShaderResourceView* CCharClass::CreateResourceView(ID3D10Texture2D* pTex)
{
	//�e�N�X�`�������擾����
	D3D10_TEXTURE2D_DESC texDesc;
	pTex->GetDesc(&texDesc);

	D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;

	//�ݒ������ShaderResourceView�쐬
	ID3D10ShaderResourceView* pOut;
	if (FAILED(DX->GetDevice()->CreateShaderResourceView(pTex, &srvDesc, &pOut)))
		return nullptr;

	return pOut;
}

//������
void CFont::Init()
{
	//���X�g�m�ہ�������
	m_Char = new list<  unique_ptr < CCharClass >>;
	m_Char->clear();

	//unicode�̕�������{�R�[�h�ɂ���
	setlocale(LC_CTYPE, "jpn");

	//�_���t�H���g�ݒ�
	HFONT hFont = CreateFont(
		32,
		0, 0, 0, 0,
		FALSE, FALSE, FALSE,
		SHIFTJIS_CHARSET,
		OUT_TT_ONLY_PRECIS,
		CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY,
		FIXED_PITCH | FF_MODERN,
		L"�l�r �S�V�b�N"
	);

	//�f�B�X�v���C�f�o�C�X�R���e�L�X�g�̃n���h�����擾
	m_hdc = GetDC(NULL);

	//�f�o�C�X�R���e�L�X�g�Ƀt�H���g���Z�b�g����
	hFont = (HFONT)SelectObject(m_hdc, hFont);

	//�t�H���g�̏����o�b�t�@�Ɋi�[
	GetTextMetrics(m_hdc, &m_TM);
	
	//�_���t�H���g�폜
	DeleteObject(hFont);
}

//����������ƂɃe�N�X�`�����쐬
void CFont::CreateStrTex(const wchar_t* str)
{
	bool entry;

	//���X�g�ɓo�^�ς����ׂ�
	for (unsigned int i = 0; i < wcslen(str); i++)
	{
		entry = false;
		
		//���X�g���猟��
		for (auto itr = m_Char->begin(); itr != m_Char->end(); itr++)
		{
			//�o�^���ꂽ������str�̕������r
			if (*itr->get()->GetChar()==str[i])
			{
				//�o�^����Ă���
				entry = true;
			}
		}

		//�o�^����ĂȂ���΁A�쐬����
		if (entry == false)
		{
			//�����e�N�X�`���쐬
			unique_ptr<CCharClass> obj(new CCharClass());
			obj->CreateCharTex(str[i], m_hdc, m_TM);

			//���X�g�ɓo�^
			m_Char->push_back(move(obj));
		}
	}
}

//�t�H���g�`��
void CFont::DrawStr(const wchar_t* Str, float Pos_x, float Pos_y, float FontSize,float Rad)
{
	//������o�^
	CreateStrTex(Str);

	for (unsigned int i = 0; i < wcslen(Str); i++)
	{
		//���X�g���猟��
		for (auto itr = m_Char->begin(); itr != m_Char->end(); itr++)
		{
			//�o�^���ꂽ������str�̕������r
			if (*itr->get()->GetChar() == Str[i])
			{			
				//�e�N�X�`���|�C���^
				ID3D10ShaderResourceView* pTex = (*itr)->GetTex();
				
				//�`��ʒu
				RECT_F Out( Pos_y, Pos_x + FontSize * i, FontSize, FontSize);
			
				//�`��
				DRAW->DrawTexture(pTex,NULL, &Out,NULL,Rad);
			}
		}
	}	
}

//���
void CFont::Release()
{
	m_Char->clear();
	ReleaseDC(NULL, m_hdc);

	//�C���X�^���X�j��
	PointerRelease(m_pInstance);
}