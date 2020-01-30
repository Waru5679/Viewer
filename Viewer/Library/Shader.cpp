#include "Shader.h"
#include "Main.h"

//インスタンス
CShader* CShader::m_pInstance = nullptr;

//初期化
bool CShader::Init(ID3D10Device* pDevice)
{
	//エフェクトを作成
	D3DX10CreateEffectFromFile(L"Geometry_Texture.fx", NULL, NULL, "fx_4_0",
		D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_DEBUG, 0,
		pDevice, NULL, NULL, &m_pEffect, NULL, NULL);

	//テクニックを得る
	m_pTechnique = m_pEffect->GetTechniqueByName("Render");

	//インプットレイアウト作成
	if (CreateLayOut(pDevice) == false)
		return false;

	//アプリ←→シェーダー架け橋
	m_pShaderWorldViewProjection = m_pEffect->GetVariableByName("g_mWVP")->AsMatrix();
	m_pShaderTexture = m_pEffect->GetVariableByName("g_texDecal")->AsShaderResource();
	m_pShaderSrc = m_pEffect->GetVariableByName("g_UvSrc")->AsVector();
	m_pShaderColor = m_pEffect->GetVariableByName("g_Color")->AsVector();

	return true;
}

//インプットレイアウトの作成
bool CShader::CreateLayOut(ID3D10Device* pDevice)
{
	//頂点インプットレイアウトを定義	
	D3D10_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//頂点インプットレイアウトを作成
	D3D10_PASS_DESC PassDesc;
	m_pTechnique->GetPassByIndex(0)->GetDesc(&PassDesc);
	if (FAILED(pDevice->CreateInputLayout(layout, numElements, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &m_pVertexLayout)))
	{
		return false;
	}
	return true;
}

//シェーダーセット
void CShader::SetShader(ID3D10ShaderResourceView* pTexture, RECT_F* pSrc,CColorData* pColor, D3DXMATRIX matWorld)
{	
	//頂点インプットレイアウトをセット
	DX->GetDevice()->IASetInputLayout(m_pVertexLayout);

	//カメラがないとき
	if (m_pCamera == nullptr)
		return;

	//ワールド＊ビュー*プロジェクション
	D3DXMATRIX objWVP = matWorld *m_pCamera->GetViewMatrix() *m_pCamera->GetProjMatrix();
	m_pShaderWorldViewProjection->SetMatrix((float*)&(objWVP));
	
	//切り取り位置
	D3DXVECTOR4 vSrc;
	if (pSrc == NULL)
	{
		//切り取り位置の指定がないとき
		vSrc = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	}
	else
	{
		vSrc = D3DXVECTOR4(pSrc->m_Top, pSrc->m_Left, pSrc->m_Right, pSrc->m_Bottom);
	}
	m_pShaderSrc->SetFloatVector(vSrc);

	//描画色
	D3DXVECTOR4 vColor;
	if (pColor == NULL)
	{
		//色データの指定がないとき
		vColor = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else
	{
		vColor = D3DXVECTOR4(pColor->m_Red, pColor->m_Green, pColor->m_Blue, pColor->m_Alpha);
	}
	m_pShaderColor->SetFloatVector(vColor);

	//テクスチャ情報があれば
	if (pTexture != NULL)
	{
		m_pShaderTexture->SetResource(pTexture);
	}
}

//解放
void CShader::Release()
{
	m_pVertexLayout->Release();
	m_pEffect->Release();

	//カメラ破棄
	if(m_pCamera!=nullptr)
		PointerRelease(m_pCamera);

	//インスタンス破棄
	PointerRelease(m_pInstance);
}