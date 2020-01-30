#include "LibraryHeader.h"

//インスタンス
CFont* CFont::m_pInstance = nullptr;

//文字テクスチャ作成
void CCharClass::CreateCharTex(wchar_t c, HDC hdc, TEXTMETRIC TM)
{
	//識別文字用
	UINT code = 0;	//作成する文字コード

	BYTE* ptr;			//文字のビットマップを入れる
	DWORD size;			//メモリサイズ
	GLYPHMETRICS GM;	//象形文字の情報が格納

	//Font書き込みの向き
	const MAT2 Mat = { {0,1}, {0,0},{0,0},{0,1} };	

	//テクスチャ書き込み用ポインタ
	D3D10_MAPPED_TEXTURE2D mapped;
	
	//テクスチャのピクセル情報を入れるポインタ
	BYTE* pBits;

	//識別文字コード登録
	m_pc.reset(new wchar_t(c));
	code = (UINT)*m_pc.get();
	
	//フォント情報から文字のビットマップ取得
	//文字のビットマップの大きさ取得
	size = GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &GM, 0, NULL, &Mat);
	ptr = new BYTE[size];
	
	//文字のビットマップ情報をptrに入れる
	GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &GM, size, ptr, &Mat);

	//空テクスチャの設定
	ID3D10Texture2D* pTexture;
	pTexture = CreateTexture();
	
	//ShaderResourceViewを作成
	m_pTexResView = CreateResourceView(pTexture);

	//テクスチャロック
	pTexture->Map(D3D10CalcSubresource(0, 0, 1),
		D3D10_MAP_WRITE_DISCARD,0,&mapped);
	pBits = (BYTE*)mapped.pData;

	//書き出し位置（左上)
	unsigned int iOfs_x = GM.gmptGlyphOrigin.x;
	unsigned int iOfs_y = TM.tmAscent - GM.gmptGlyphOrigin.y;
	
	//フォントビットマップのサイズ
	unsigned int iBmp_w = GM.gmBlackBoxX + (4 - (GM.gmBlackBoxX % 4)) % 4;
	unsigned int iBmp_h = GM.gmBlackBoxY;
	
	//α値の段階
	int Level = 17; //GGO_GRAY4＿BITMAPなので17

	DWORD dwColor,dwFontColor,dwBackColor;
	
	//背景色
	dwBackColor = 0x00ffffff;//透過(黒	
	
	//フォント色
	dwFontColor = 0xffffffff;//白

	//1ピクセル単位にフォントの色情報(32bit)をテクスチャに書き込み
	memset(pBits, 0x00, sizeof(pBits)*32*32);

	for(unsigned int y=0;y<32;y++)
	{
		for (unsigned int x = 0; x < 32; x++)
		{
			//文字の範囲
			if (iOfs_y <= y && y < iOfs_y + iBmp_h
				&& iOfs_x<=x && x< iOfs_x + GM.gmBlackBoxX)
			{
				dwColor = (255 * ptr[x - iOfs_x + iBmp_w * (y - iOfs_y)]) / (Level - 1);

				//色指定
				if (dwColor == 0)
				{
					//空白部
					dwColor = dwBackColor;
				}
				else
				{
					//文字部
					dwColor = dwFontColor;
				}
			}
			else
			{
				//淵
				dwColor = dwBackColor;
			}
			memcpy((BYTE*)pBits + (y << 7) + (x << 2), &dwColor, sizeof(DWORD));
		}
	}
		
	//テクスチャアンロック
	pTexture->Unmap(D3D10CalcSubresource(0, 0, 1));
	
	//データ削除 
	delete[] ptr;
	 
	pTexture->Release();
}

//空テクスチャ作成
ID3D10Texture2D* CCharClass::CreateTexture()
{
	D3D10_TEXTURE2D_DESC desc;
	memset(&desc, 0, sizeof(desc));

	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		//テクスチャフォーマットR8G8B8の24Bit
	desc.SampleDesc.Count = 1;						//サンプリングは1ピクセルのみ
	desc.Usage = D3D10_USAGE_DYNAMIC;				//CPU書き込み可能
	desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;	//シェーダリソース
	desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;	//CPUから書き込みアクセス可
	desc.Height = 32;
	desc.Width = 32;
	
	//設定を元に空テクスチャを作成
	ID3D10Texture2D* pOut;
	if (FAILED(DX->GetDevice()->CreateTexture2D(&desc, 0, &pOut)))
		return nullptr;

	return pOut;
}

//ShaderResourceView作成
ID3D10ShaderResourceView* CCharClass::CreateResourceView(ID3D10Texture2D* pTex)
{
	//テクスチャ情報を取得する
	D3D10_TEXTURE2D_DESC texDesc;
	pTex->GetDesc(&texDesc);

	D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;

	//設定を元にShaderResourceView作成
	ID3D10ShaderResourceView* pOut;
	if (FAILED(DX->GetDevice()->CreateShaderResourceView(pTex, &srvDesc, &pOut)))
		return nullptr;

	return pOut;
}

//初期化
void CFont::Init()
{
	//リスト確保＆初期化
	m_Char = new list<  unique_ptr < CCharClass >>;
	m_Char->clear();

	//unicodeの文字を日本コードにする
	setlocale(LC_CTYPE, "jpn");

	//論理フォント設定
	HFONT hFont = CreateFont(
		32,
		0, 0, 0, 0,
		FALSE, FALSE, FALSE,
		SHIFTJIS_CHARSET,
		OUT_TT_ONLY_PRECIS,
		CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY,
		FIXED_PITCH | FF_MODERN,
		L"ＭＳ ゴシック"
	);

	//ディスプレイデバイスコンテキストのハンドルを取得
	m_hdc = GetDC(NULL);

	//デバイスコンテキストにフォントをセットする
	hFont = (HFONT)SelectObject(m_hdc, hFont);

	//フォントの情報をバッファに格納
	GetTextMetrics(m_hdc, &m_TM);
	
	//論理フォント削除
	DeleteObject(hFont);
}

//文字列をもとにテクスチャを作成
void CFont::CreateStrTex(const wchar_t* str)
{
	bool entry;

	//リストに登録済か調べる
	for (unsigned int i = 0; i < wcslen(str); i++)
	{
		entry = false;
		
		//リストから検索
		for (auto itr = m_Char->begin(); itr != m_Char->end(); itr++)
		{
			//登録された文字とstrの文字を比較
			if (*itr->get()->GetChar()==str[i])
			{
				//登録されている
				entry = true;
			}
		}

		//登録されてなければ、作成する
		if (entry == false)
		{
			//文字テクスチャ作成
			unique_ptr<CCharClass> obj(new CCharClass());
			obj->CreateCharTex(str[i], m_hdc, m_TM);

			//リストに登録
			m_Char->push_back(move(obj));
		}
	}
}

//フォント描画
void CFont::DrawStr(const wchar_t* Str, float Pos_x, float Pos_y, float FontSize,float Rad)
{
	//文字列登録
	CreateStrTex(Str);

	for (unsigned int i = 0; i < wcslen(Str); i++)
	{
		//リストから検索
		for (auto itr = m_Char->begin(); itr != m_Char->end(); itr++)
		{
			//登録された文字とstrの文字を比較
			if (*itr->get()->GetChar() == Str[i])
			{			
				//テクスチャポインタ
				ID3D10ShaderResourceView* pTex = (*itr)->GetTex();
				
				//描画位置
				RECT_F Out( Pos_y, Pos_x + FontSize * i, FontSize, FontSize);
			
				//描画
				DRAW->DrawTexture(pTex,NULL, &Out,NULL,Rad);
			}
		}
	}	
}

//解放
void CFont::Release()
{
	m_Char->clear();
	ReleaseDC(NULL, m_hdc);

	//インスタンス破棄
	PointerRelease(m_pInstance);
}