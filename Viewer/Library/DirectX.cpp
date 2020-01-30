#include "DirectX.h"
#include "Release.h"

//�C���X�^���X
CDirectX* CDirectX::m_pInstance = nullptr;

//������
bool CDirectX::Init(HWND hWnd)
{
	// �f�o�C�X�ƃX���b�v�`�F�[���̍쐬
	if (FAILED(CreateSwapChain(hWnd)))
	{
		return false;
	}	

	//�����_�[�^�[�Q�b�g�r���[�̍쐬
	CreateRenderTarget();

	//�r���[�|�[�g�̐ݒ�
	SetViewport();
	
	//�[�x�X�e���V���r���[�̍쐬
	CreateStencil();		

	//�u�����h�X�e�[�g�쐬
	CreateBlendState();

	//���X�^���C�U�ݒ�
	SetRasterizer();

	return true;
}

//�X���b�v�`�F�C���쐬
HRESULT CDirectX::CreateSwapChain(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = WINDOW_WIDTH;
	sd.BufferDesc.Height = WINDOW_HEIGHT;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	if (FAILED(D3D10CreateDeviceAndSwapChain(NULL, D3D10_DRIVER_TYPE_HARDWARE, NULL,
		0, D3D10_SDK_VERSION, &sd, &m_pSwapChain, &m_pDevice)))
	{
		return FALSE;
	}

	return S_OK;
}

//�r���[�|�[�g�ݒ�
void CDirectX::SetViewport()
{
	D3D10_VIEWPORT vp;
	vp.Width = WINDOW_WIDTH;
	vp.Height = WINDOW_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pDevice->RSSetViewports(1, &vp);
}

//�����_�[�^�[�Q�b�g�r���[�̍쐬
void CDirectX::CreateRenderTarget()
{
	ID3D10Texture2D *pBackBuffer;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&pBackBuffer);
	m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
	pBackBuffer->Release();
}

//�[�x�X�e���V���r���[�̍쐬
void CDirectX::CreateStencil()
{
	D3D10_TEXTURE2D_DESC descDepth;
	descDepth.Width = WINDOW_WIDTH;
	descDepth.Height = WINDOW_HEIGHT;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D10_USAGE_DEFAULT;
	descDepth.BindFlags = D3D10_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	m_pDevice->CreateTexture2D(&descDepth, NULL, &m_pDepthStencil);

	D3D10_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	m_pDevice->CreateDepthStencilView(m_pDepthStencil, &descDSV, &m_pDepthStencilView);

	//�����_�[�^�[�Q�b�g�r���[�Ɛ[�x�X�e���V���r���[���p�C�v���C���Ƀo�C���h
	m_pDevice->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
}

//�u�����h�X�e�[�g�쐬
void CDirectX::CreateBlendState()
{
	D3D10_BLEND_DESC BlendStateDesc;
	memset(&BlendStateDesc, 0, sizeof(BlendStateDesc));

	BlendStateDesc.BlendEnable[0] = TRUE;
	BlendStateDesc.SrcBlend = D3D10_BLEND_SRC_ALPHA;
	BlendStateDesc.DestBlend = D3D10_BLEND_INV_SRC_ALPHA;
	BlendStateDesc.BlendOp = D3D10_BLEND_OP_ADD;
	BlendStateDesc.SrcBlendAlpha = D3D10_BLEND_SRC_ALPHA;
	BlendStateDesc.DestBlendAlpha = D3D10_BLEND_INV_SRC_ALPHA;
	BlendStateDesc.BlendOpAlpha = D3D10_BLEND_OP_ADD;
	BlendStateDesc.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;

	m_pBlendState = 0;
	m_pDevice->CreateBlendState(&BlendStateDesc, &m_pBlendState);
	m_pDevice->OMSetBlendState(m_pBlendState, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
}

//���X�^���C�Y�ݒ�
void CDirectX::SetRasterizer()
{
	D3D10_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));
	rdc.CullMode = D3D10_CULL_BACK;		//�������͕`�悵�Ȃ�
	rdc.FillMode = D3D10_FILL_SOLID;

	ID3D10RasterizerState* pIr = NULL;
	m_pDevice->CreateRasterizerState(&rdc, &pIr);
	m_pDevice->RSSetState(pIr);
}

//�������J��
void CDirectX::Release()
{
	m_pBlendState->Release();
	m_pDepthStencilView->Release();
	m_pDepthStencil->Release();
	m_pRenderTargetView->Release();
	m_pSwapChain->Release();
	m_pDevice->Release();

	//�C���X�^���X�j��
	PointerRelease(m_pInstance);
}