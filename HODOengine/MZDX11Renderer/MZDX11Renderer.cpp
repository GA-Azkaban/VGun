#include "MZDX11Renderer.h"
#include "MZCamera.h"
#include "DeferredBuffers.h"
#include "QuadBuffer.h"
#include "ResourceManager.h"
#include "RasterizerState.h"
#include "SamplerState.h"
#include "PixelShader.h"
#include "GBufferPass.h"
#include "DeferredPass.h"
#include "SkyboxPass.h"
#include "BlitPass.h"
#include "Lights.h"
#include "MathHelper.h"
#include "MZMacro.h"
#include "StaticMeshObject.h"
#include "SkinningMeshObject.h"
#include "HelperObject.h"
#include "Skybox.h"

#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "dxguid.lib" )	
#pragma comment( lib, "d3dcompiler.lib" )	
#pragma comment( lib, "dxgi.lib" )	// adapter info	

MZDX11Renderer* MZDX11Renderer::instance = nullptr;

MZDX11Renderer::MZDX11Renderer()
	: m_hWnd(0), m_screenWidth(1920), m_screenHeight(1080)
{
	
}

MZDX11Renderer::~MZDX11Renderer()
{
	RasterizerState::Instance.Get().DestroyRenderStates();
	m_samplerState->DestroySamplerStates();
	delete m_samplerState;
	delete m_deferredBuffers;

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

	m_samplerState = new SamplerState();
	m_samplerState->CreateSamplerStates(m_d3dDevice.Get(), m_d3dDeviceContext.Get());

	m_deferredBuffers = new DeferredBuffers(m_d3dDevice.Get(), m_d3dDeviceContext.Get());
	m_quadBuffer = new QuadBuffer(m_d3dDevice.Get(), m_d3dDeviceContext.Get());

	_GBufferPass = new GBufferPass(m_deferredBuffers);
	_deferredPass = new DeferredPass(m_deferredBuffers, m_quadBuffer);
	_skyboxPass = new SkyboxPass(m_deferredBuffers, m_quadBuffer);
	_blitPass = new BlitPass(m_quadBuffer);
	
	CreateDepthStecilStates();
	SetLights();
	SetObjects();

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
	EnableZBuffering();
}

void MZDX11Renderer::Update(float deltaTime)
{
	for (auto& meshObject : IMeshObject::meshObjects)
	{
		meshObject->Update(deltaTime);
	}
}

void MZDX11Renderer::Render()
{
	//EnableZBuffering();

	_GBufferPass->Render();
	_deferredPass->Render();
	_skyboxPass->Render();
	
	//DisableZBuffering();
	_blitPass->Render();

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

	m_swapChain->ResizeBuffers(0, m_screenWidth, m_screenHeight, DXGI_FORMAT_UNKNOWN, 0);

	ID3D11Texture2D* backBuffer;
	HR(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	HR(m_d3dDevice->CreateRenderTargetView(backBuffer, 0, &m_backBufferRTV));
	backBuffer->Release();

	m_deferredBuffers->Initialize(m_screenWidth, m_screenHeight);
	m_quadBuffer->Initialize(m_screenWidth, m_screenHeight);

	// set the viewport transform
	m_viewPort.TopLeftX = 0;
	m_viewPort.TopLeftY = 0;
	m_viewPort.Width = static_cast<float>(m_screenWidth);
	m_viewPort.Height = static_cast<float>(m_screenHeight);
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.MaxDepth = 1.0f;

	m_d3dDeviceContext->RSSetViewports(1, &m_viewPort);
}

void MZDX11Renderer::CreateDepthStecilStates()
{
	// Initialize the depth stencil states
	D3D11_DEPTH_STENCIL_DESC enableDepthStencilDescription;
	ZeroMemory(&enableDepthStencilDescription, sizeof(enableDepthStencilDescription));

	enableDepthStencilDescription.DepthEnable = true;
	enableDepthStencilDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	enableDepthStencilDescription.DepthFunc = D3D11_COMPARISON_LESS;
	enableDepthStencilDescription.StencilEnable = false;
	//enableDepthStencilDescription.StencilReadMask = 0xFF;
	//enableDepthStencilDescription.StencilWriteMask = 0xFF;
	//// Stencil operations if pixel is front-facing.
	//enableDepthStencilDescription.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	//enableDepthStencilDescription.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	//enableDepthStencilDescription.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	//enableDepthStencilDescription.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	//// Stencil operations if pixel is back-facing.
	//enableDepthStencilDescription.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	//enableDepthStencilDescription.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	//enableDepthStencilDescription.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	//enableDepthStencilDescription.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state for enabling Z buffering
	m_d3dDevice->CreateDepthStencilState(&enableDepthStencilDescription, &m_depthStencilStateEnable);

	// Initialize the depth stencil states
	D3D11_DEPTH_STENCIL_DESC disableDepthStencilDescription;
	ZeroMemory(&disableDepthStencilDescription, sizeof(disableDepthStencilDescription));

	disableDepthStencilDescription.DepthEnable = false;
	disableDepthStencilDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	disableDepthStencilDescription.DepthFunc = D3D11_COMPARISON_LESS;
	disableDepthStencilDescription.StencilEnable = true;
	disableDepthStencilDescription.StencilReadMask = 0xFF;
	disableDepthStencilDescription.StencilWriteMask = 0xFF;
	// Stencil operations if pixel is front-facing.
	disableDepthStencilDescription.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	disableDepthStencilDescription.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	disableDepthStencilDescription.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	disableDepthStencilDescription.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// Stencil operations if pixel is back-facing.
	disableDepthStencilDescription.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	disableDepthStencilDescription.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	disableDepthStencilDescription.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	disableDepthStencilDescription.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state for disabling Z buffering
	m_d3dDevice->CreateDepthStencilState(&disableDepthStencilDescription, &m_depthStencilStateDisable);
}

void MZDX11Renderer::SetLights()
{
	DirectionalLight* dirLight = new DirectionalLight();
	dirLight->Color = XMFLOAT4{ 0.5f, 0.5f, 0.5f, 1.0f };
	dirLight->Direction = XMFLOAT3{ 10.0f, -10.0f, 0.0f };
	ResourceManager::Instance.Get().GetPixelShader("PixelShader.cso")->SetDirectionalLight("dirLight", *dirLight);
	ResourceManager::Instance.Get().GetPixelShader("FullScreenQuadPixelShader.cso")->SetDirectionalLight("dirLight", *dirLight);

	PointLight pointLight[4];
	pointLight[0].Color = XMFLOAT4{ 0.3f, 0.0f, 0.0f, 1.0f };
	pointLight[0].Position = XMFLOAT4{ 5.0f, 5.0f, 0.0f, 1.0f };
	pointLight[1].Color = XMFLOAT4{ 0.0f, 0.3f, 0.0f, 1.0f };
	pointLight[1].Position = XMFLOAT4{ 0.0f, 3.0f, -5.0f, 1.0f };
	pointLight[2].Color = XMFLOAT4{ 0.5f, 0.3f, 0.0f, 1.0f };
	pointLight[2].Position = XMFLOAT4{ -1.0f, 0.0f, 0.0f, 1.0f };
	pointLight[3].Color = XMFLOAT4{ 0.2f, 0.2f, 0.2f, 1.0f };
	pointLight[3].Position = XMFLOAT4{ 0.0f, 3.0f, -10.0f, 1.0f };
	ResourceManager::Instance.Get().GetPixelShader("PixelShader.cso")->SetPointLight("pointLight", pointLight);
	ResourceManager::Instance.Get().GetPixelShader("FullScreenQuadPixelShader.cso")->SetPointLight("pointLight", pointLight);

	SpotLight spotLight[2];
	spotLight[0].Color = XMFLOAT4{ 0.1f, 0.1f, 0.1f, 1.0f };
	spotLight[0].Direction = XMFLOAT3{ 10.0f, -3.0f, 0.0f };
	spotLight[0].Position = XMFLOAT4{ 20.0f, 10.0f, 0.0f, 1.0f };
	spotLight[0].SpotPower = 1.0f;
	spotLight[1].Color = XMFLOAT4{ 0.1f, 0.1f, 0.1f, 1.0f };
	spotLight[1].Direction = XMFLOAT3{ -10.0f, 0.0f, -5.0f };
	spotLight[1].Position = XMFLOAT4{ 10.0f, 20.0f, 5.0f, 1.0f };
	spotLight[1].SpotPower = 1.0f;
	ResourceManager::Instance.Get().GetPixelShader("PixelShader.cso")->SetSpotLight("spotLight", spotLight);
	ResourceManager::Instance.Get().GetPixelShader("FullScreenQuadPixelShader.cso")->SetSpotLight("spotLight", spotLight);
}

void MZDX11Renderer::SetObjects()
{
	/*HelperObject* grid = new HelperObject();
	grid->SetMesh("grid");

	HelperObject* axis = new HelperObject();
	axis->SetMesh("axis");*/

	// FBX Test
	ResourceManager::Instance.Get().LoadFBXFile("Characters/Rob02.fbx");
	ResourceManager::Instance.Get().LoadFBXFile("Characters/4QCharacter_tpose.fbx");
	ResourceManager::Instance.Get().LoadFBXFile("Characters/4QCharacter_idle_ani.fbx");
	ResourceManager::Instance.Get().LoadFBXFile("Characters/A_TP_CH_Sprint_F.fbx");
	ResourceManager::Instance.Get().LoadFBXFile("Characters/untitled.fbx");
	ResourceManager::Instance.Get().LoadFBXFile("Characters/newSprint.fbx");
	ResourceManager::Instance.Get().LoadFBXFile("Characters/model.dae");
	ResourceManager::Instance.Get().LoadTextureFile("sunsetcube1024.dds");

	//StaticMeshObject* test1 = new StaticMeshObject();
	SkinningMeshObject* test1 = new SkinningMeshObject();
	//test1->SetMesh("Rob02.fbx");
	//test1->SetDiffuseTexture("Rob02Yellow_AlbedoTransparency.png");
	//test1->SetNormalTexture("Rob02White_Normal.png");
	//test1->SetMesh("A_TP_CH_Breathing.fbx");
	test1->SetMesh("newSprint.fbx");
	test1->SetDiffuseTexture("T_TP_CH_Basic_001_001_D.png");
	test1->PlayAnimation(0, true);

	Skybox* sky = new Skybox();
	sky->SetCubeMapTexture("sunsetcube1024.dds");
	sky->SetActive(false);
}

void MZDX11Renderer::EnableZBuffering()
{
	m_d3dDeviceContext->OMSetDepthStencilState(m_depthStencilStateEnable.Get(), 1);
}

void MZDX11Renderer::DisableZBuffering()
{
	m_d3dDeviceContext->OMSetDepthStencilState(m_depthStencilStateDisable.Get(), 1);
}

void MZDX11Renderer::SetRenderTarget()
{
	m_d3dDeviceContext->OMSetRenderTargets(1, m_backBufferRTV.GetAddressOf(), nullptr);
}

void MZDX11Renderer::ClearRenderTarget()
{
	m_d3dDeviceContext->ClearRenderTargetView(m_backBufferRTV.Get(), reinterpret_cast<const float*>(&DirectX::Colors::Transparent));
	//m_d3dDeviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

float MZDX11Renderer::GetAspectRatio() const
{
	return static_cast<float>(m_screenWidth) / m_screenHeight;
}
