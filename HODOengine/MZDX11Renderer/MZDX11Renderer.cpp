#include "MZDX11Renderer.h"
#include "MZCamera.h"
#include "DeferredBuffers.h"
#include "ResourceManager.h"
#include "RasterizerState.h"
#include "SamplerState.h"
#include "MathHelper.h"
#include "MZMacro.h"

#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "dxguid.lib" )	
#pragma comment( lib, "d3dcompiler.lib" )	
#pragma comment( lib, "dxgi.lib" )	// adapter info	

MZDX11Renderer* MZDX11Renderer::instance = nullptr;

MZDX11Renderer::MZDX11Renderer()
	: m_hWnd(0), m_screenWidth(1920), m_screenHeight(1080)
{
	m_deferredBuffers = new DeferredBuffers();
}

MZDX11Renderer::~MZDX11Renderer()
{
	RasterizerState::Instance.Get().DestroyRenderStates();
	SamplerState::Instance.Get().DestroySamplerStates();

	if (m_d3dDeviceContext)
	{
		m_d3dDeviceContext->ClearState();
	}

	m_d3dDeviceContext.Reset();
	m_d3dDevice.Reset();
}

bool MZDX11Renderer::Initialize()
{
	// 디버그 모드인 경우 DX11 디버그 모드
	UINT createDeviceFlags = 0;
	D3D_FEATURE_LEVEL featureLevel;
#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// 디바이스와 디바이스 컨텍스트 생성
	HRESULT hr = D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, 0, createDeviceFlags, 0, 0,
		D3D11_SDK_VERSION, &m_d3dDevice, &featureLevel, &m_d3dDeviceContext);
	if (FAILED(hr))
	{
		MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
		return false;
	}
	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
		return false;
	}

	ResourceManager::Instance.Get().Initialize(m_d3dDevice.Get(), m_d3dDeviceContext.Get());
	RasterizerState::Instance.Get().CreateRenderStates(m_d3dDevice.Get());
	SamplerState::Instance.Get().CreateSamplerStates(m_d3dDevice.Get());

	return true;
}

void MZDX11Renderer::SetOutputWindow(unsigned int hWnd)
{
	m_hWnd = (HWND)hWnd;
	RECT rect;
	GetClientRect(m_hWnd, &rect);
	m_screenWidth = rect.right - rect.left;
	m_screenHeight = rect.bottom - rect.top;

	// Check multi sampling
	UINT _4xMsaaQuality;
	HR(m_d3dDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &_4xMsaaQuality));

	// Fill out a DXGI_SWAP_CHAIN_DESC
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = m_screenWidth;
	sd.BufferDesc.Height = m_screenHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	if (_4xMsaaQuality > 0)
	{
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = _4xMsaaQuality - 1;
	}
	else
	{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = m_hWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	IDXGIDevice* dxgiDevice = 0;
	HR(m_d3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

	IDXGIAdapter* dxgiAdapter = 0;
	HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

	IDXGIFactory* dxgiFactory = 0;
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

	HR(dxgiFactory->CreateSwapChain(m_d3dDevice.Get(), &sd, &m_swapChain));

	dxgiDevice->Release();
	dxgiAdapter->Release();
	dxgiFactory->Release();

	ResizeBuffers();
}

void MZDX11Renderer::Update(float deltaTime)
{
	// TODO
}


void MZDX11Renderer::BeginRender()
{
	assert(m_d3dDeviceContext);

	m_d3dDeviceContext->OMSetRenderTargets(1, m_backBufferRTV.GetAddressOf(), m_depthStencilView.Get());
	m_d3dDeviceContext->ClearRenderTargetView(m_backBufferRTV.Get(), reinterpret_cast<const float*>(&DirectX::Colors::Black));
	m_d3dDeviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}


void MZDX11Renderer::Render()
{
	BeginRender();

	assert(m_swapChain);
	m_swapChain->Present(0, 0);
}

void MZDX11Renderer::Finalize()
{

}

void MZDX11Renderer::ResizeResolution(unsigned int width, unsigned int height)
{
	m_screenWidth = width;
	m_screenHeight = height;

	// 투영 행렬 재계산
	MZCamera::GetMainCamera()->SetFrustum(0.25f * MathHelper::Pi, GetAspectRatio(), 
		MZCamera::GetMainCamera()->GetNearZ(), MZCamera::GetMainCamera()->GetFarZ());
	
	ResizeBuffers();
}

void MZDX11Renderer::ResizeBuffers()
{
	m_backBufferRTV.Reset();
	m_quadRTV.Reset();
	m_quadSRV.Reset();
	m_quadTexture.Reset();

	m_swapChain->ResizeBuffers(0, m_screenWidth, m_screenHeight, DXGI_FORMAT_UNKNOWN, 0);

	ID3D11Texture2D* backBuffer;
	HR(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	HR(m_d3dDevice->CreateRenderTargetView(backBuffer, 0, &m_backBufferRTV));
	backBuffer->Release();

	// Quad
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = m_screenWidth;
	textureDesc.Height = m_screenHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	m_d3dDevice->CreateTexture2D(&textureDesc, nullptr, m_quadTexture.GetAddressOf());

	D3D11_RENDER_TARGET_VIEW_DESC quadRTVDesc;
	quadRTVDesc.Format = textureDesc.Format;
	quadRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	quadRTVDesc.Texture2D.MipSlice = 0;

	m_d3dDevice->CreateRenderTargetView(m_quadTexture.Get(), &quadRTVDesc, m_quadRTV.GetAddressOf());

	D3D11_SHADER_RESOURCE_VIEW_DESC quadSRVDesc;
	quadSRVDesc.Format = textureDesc.Format;
	quadSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	quadSRVDesc.Texture2D.MostDetailedMip = 0;
	quadSRVDesc.Texture2D.MipLevels = 1;

	m_d3dDevice->CreateShaderResourceView(m_quadTexture.Get(), &quadSRVDesc, m_quadSRV.GetAddressOf());

	// set the viewport transform
	m_viewPort.TopLeftX = 0;
	m_viewPort.TopLeftY = 0;
	m_viewPort.Width = static_cast<float>(m_screenWidth);
	m_viewPort.Height = static_cast<float>(m_screenHeight);
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.MaxDepth = 1.0f;

	m_d3dDeviceContext->RSSetViewports(1, &m_viewPort);

	m_deferredBuffers->Initialize(m_d3dDevice.Get(), m_screenWidth, m_screenHeight);
}

float MZDX11Renderer::GetAspectRatio() const
{
	return static_cast<float>(m_screenWidth) / m_screenHeight;
}
