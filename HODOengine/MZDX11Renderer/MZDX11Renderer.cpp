#include "MZDX11Renderer.h"
#include "MZCamera.h"
#include "DeferredRenderer.h"
#include "ShaderManager.h"
#include "RasterizerState.h"
#include "MathHelper.h"
#include "MZMacro.h"

#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "dxguid.lib" )	
#pragma comment( lib, "d3dcompiler.lib" )	
#pragma comment( lib, "dxgi.lib" )	// adapter info
#pragma comment( lib, "assimp-vc143-mt.lib" )	

MZDX11Renderer* MZDX11Renderer::instance = nullptr;

MZDX11Renderer::MZDX11Renderer()
	: m_hWnd(0), m_screenWidth(1920), m_screenHeight(1080)
{
	m_camera = new MZCamera();
	m_camera->SetMain();
	m_camera->LookAt(XMFLOAT3(8.0f, 8.0f, -8.0f), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 1.0f, 0));
}

MZDX11Renderer::~MZDX11Renderer()
{
	RasterizerState::Instance.Get().DestroyRenderStates();

	delete m_camera;

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

	ShaderManager::Instance.Get().LoadShaders(m_d3dDevice.Get(), m_d3dDeviceContext.Get());
	RasterizerState::Instance.Get().CreateRenderStates(m_d3dDevice.Get());

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

	DeferredRenderer::Instance.Get().Initialize(m_d3dDevice.Get(), m_d3dDeviceContext.Get(), m_swapChain.Get(), m_screenWidth, m_screenHeight);
}

void MZDX11Renderer::Update(float deltaTime)
{
	/*m_cameraSpeed = 1.0f;
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		m_cameraSpeed = 2.0f;
	}*/

	DeferredRenderer::Instance.Get().Update(deltaTime);
}


//void MZDX11Renderer::BeginRender()
//{
//	assert(m_d3dDeviceContext);
//
//	m_d3dDeviceContext->OMSetRenderTargets(1, m_backBufferRTV.GetAddressOf(), m_depthStencilView.Get());
//	m_d3dDeviceContext->ClearRenderTargetView(m_backBufferRTV.Get(), reinterpret_cast<const float*>(&DirectX::Colors::Black));
//	m_d3dDeviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
//}


void MZDX11Renderer::Render()
{
	DeferredRenderer::Instance.Get().RenderToBackBuffer();

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
	m_camera->SetFrustum(0.25f * MathHelper::Pi, GetAspectRatio(), 1.0f, 1000.0f);
	
	DeferredRenderer::Instance.Get().ResizeResolution(width, height);
}

void MZDX11Renderer::OnMouseDown(int btnState, int x, int y)
{
	if ((btnState & MK_RBUTTON) != 0)
	{
		m_lastMousePos.x = x;
		m_lastMousePos.y = y;

		SetCapture(m_hWnd);
	}
	else if ((btnState & MK_LBUTTON) != 0)
	{
		Pick(x, y);
	}
}

void MZDX11Renderer::OnMouseUp(int x, int y)
{
	ReleaseCapture();
}

void MZDX11Renderer::OnMouseMove(int btnState, int x, int y)
{
	if ((btnState & MK_RBUTTON) != 0)
	{
		float dx = XMConvertToRadians(0.25f * static_cast<float>(x - m_lastMousePos.x));
		float dy = XMConvertToRadians(0.25f * static_cast<float>(y - m_lastMousePos.y));

		m_camera->Pitch(dy);
		m_camera->RotateY(dx);
	}

	m_lastMousePos.x = x;
	m_lastMousePos.y = y;
}

IRenderableObject* MZDX11Renderer::Pick(float normalizedX, float normalizedY)
{
	return DeferredRenderer::Instance.Get().Pick(normalizedX, normalizedY);
}

float MZDX11Renderer::GetAspectRatio() const
{
	return static_cast<float>(m_screenWidth) / m_screenHeight;
}
