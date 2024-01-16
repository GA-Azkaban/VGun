#include <cassert>

#include "RocketDX11.h"
#include "Grid.h"
#include "Axis.h"
#include "CubeMesh.h"
#include "TextRenderer.h"
#include "ImageRenderer.h"
#include "VertexShader.h"
#include "PixelShader.h"

#include "RocketMacroDX11.h"
#include "DeviceBuilderDX11.h"

#include "ResourceManager.h"
#include "ObjectManager.h"

#include "HelperObject.h"
#include "StaticMeshObject.h"
#include "SkinningMeshObject.h"
#include "ImageRenderer.h"
#include "LineRenderer.h"

#include "Cubemap.h"

#include "LightStruct.h"
using namespace DirectX;

namespace HDEngine
{
	I3DRenderer* CreateGraphicsInstance()
	{
		return new RocketCore::Graphics::RocketDX11();
	}

	void ReleaseGraphicsInstance(I3DRenderer* instance)
	{
		delete instance;
	}
}

/// IRocketGraphics.h 에 있는 dllexport 함수들.
/// USE_DX11 를 전처리기에 넣어주면 DX11용 dll이 만들어진다.
/// 
/// 23.06.16 강석원 인재원.
namespace RocketCore::Graphics
{
	RocketDX11::RocketDX11()
		: _hWnd(), _screenWidth(), _screenHeight(), _vSyncEnabled(),
		_device(), _deviceContext(),
		_featureLevel(),_m4xMsaaQuality(),
		_swapChain(), _backBuffer(),
		_renderTargetView(), _depthStencilBuffer(), _depthStencilView(),
		_viewport(),
		_resourceManager(ResourceManager::Instance())
	{

	}

	RocketDX11::~RocketDX11()
	{

	}

	void RocketDX11::Initialize(void* hWnd, int screenWidth, int screenHeight)
	{
		// 매크로로 변경하려고 작업중
		HRESULT hr = S_OK;

		_hWnd = static_cast<HWND>(hWnd);
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;

		// device 빌더 클래스를 이용해 device와 deviceContext 생성.
		DeviceBuilderDX11 deviceBuilder;
		deviceBuilder.SetDevice(_device.GetAddressOf());
		deviceBuilder.SetLevelHolder(&_featureLevel);
		deviceBuilder.SetDeviceContext(_deviceContext.GetAddressOf());
		HR(deviceBuilder.Build());

		/// 아래 if문은 용책에서의 코드
		/// 버전이 11_0이 아닐때 false를 리턴한다.
		/// 내 코드에서는 11_1을 사용하는데 이래도 되는건가?!
		/// 23.04.07 강석원 인재원
	// 	if (_featureLevel != D3D_FEATURE_LEVEL_11_0)
	// 	{
	// 		MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
	// 		return false;
	// 	}

		/// 멀티 샘플링 품질레벨 체크
		/// Direct11 에서는 항상 지원되므로, 반환된 품질 수준 값은 반드시 0보다 커야 한다.
		/// 23.04.07 강석원 인재원
		hr = _device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &_m4xMsaaQuality);
		assert(_m4xMsaaQuality > 0);

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.SampleDesc.Count = 4;      //multisampling setting	// 나는 4x를 사용하므로 4
		swapChainDesc.SampleDesc.Quality = _m4xMsaaQuality - 1;	//vendor-specific flag	// 위에서 받아온 퀄리티 레벨을 넣어준다.	// -1 을 왜해줄까?
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;		// 이걸 2로 하게되면 렌더타겟 뷰를 각각의 버퍼에 대해 가지고 있어야하나?
		swapChainDesc.OutputWindow = _hWnd;
		swapChainDesc.Windowed = TRUE; // Sets the initial state of full-screen mode.
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;	// 교환효과. DXGI_SWAP_EFFECT_DISCARD는 디스플레이 구동기가 가장 효율적인 제시 방법을 선택하게 함
		//desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;	// MSDN 그대로 따라 친 것.
		swapChainDesc.Flags = 0;

		/// DXGIDevice로 DXGIAdapter를 만들고
		/// DXGIAdapter로 DXGIFactory를 만들고
		/// DXGIFactory로 SwapChain을 만든다!
		/// 위의 swapchain 명세를 이용해 명세에 적힌대로 swapchain을 만드는 것!
		/// 
		/// 23.04.10 강석원 인재원
		// Create the DXGI device object to use in other factories, such as Direct2D.
		Microsoft::WRL::ComPtr<IDXGIDevice3> dxgiDevice;
		_device.As(&dxgiDevice);

		// Create swap chain.
		Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
		Microsoft::WRL::ComPtr<IDXGIFactory> factory;

		hr = dxgiDevice->GetAdapter(&adapter);

		if (SUCCEEDED(hr))
		{
			adapter->GetParent(IID_PPV_ARGS(&factory));

			hr = factory->CreateSwapChain(
				_device.Get(),
				//_device.Get(),
				&swapChainDesc,
				&_swapChain
			);
		}

		hr = _swapChain->GetBuffer(
			0,
			__uuidof(ID3D11Texture2D),
			(void**)&_backBuffer);

		hr = _device->CreateRenderTargetView(
			_backBuffer.Get(),
			nullptr,
			_renderTargetView.GetAddressOf()
		);

		D3D11_TEXTURE2D_DESC backBufferDesc;

		_backBuffer->GetDesc(&backBufferDesc);

		CD3D11_TEXTURE2D_DESC depthStencilDesc(
			DXGI_FORMAT_D24_UNORM_S8_UINT,
			static_cast<UINT> (backBufferDesc.Width),
			static_cast<UINT> (backBufferDesc.Height),
			1, // This depth stencil view has only one texture.
			1, // Use a single mipmap level.
			D3D11_BIND_DEPTH_STENCIL
		);

		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = _m4xMsaaQuality - 1;

		hr = _device->CreateTexture2D(
			&depthStencilDesc,
			nullptr,
			&_depthStencilBuffer
		);

		CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);

		// 	hr = d3dDevice_->CreateDepthStencilView(
		// 		depthStencilBuffer_.Get(),
		// 		&depthStencilViewDesc,
		// 		&depthStencilView_
		// 	);

		hr = _device->CreateDepthStencilView(
			_depthStencilBuffer.Get(),
			nullptr,
			&_depthStencilView
		);

		/// RenderTargetView 와 DepthStencilBuffer를 출력 병합 단계(Output Merger Stage)에 바인딩
		_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), _depthStencilView.Get());

		ZeroMemory(&_viewport, sizeof(D3D11_VIEWPORT));
		_viewport.Height = (float)backBufferDesc.Height;
		_viewport.Width = (float)backBufferDesc.Width;
		_viewport.MinDepth = 0;
		_viewport.MaxDepth = 1;

		_deviceContext->RSSetViewports(
			1,
			&_viewport
		);

		_resourceManager.Initialize(_device.Get(), _deviceContext.Get());

		/// Load resources
		_resourceManager.LoadFBXFile("Rob02.fbx");
		_resourceManager.LoadTextureFile("sunsetcube1024.dds");

		CreateDepthStencilStates();
		SetLights();

		/*_axis = new Axis();
		_axis->Initialize(_device.Get());

		_grid = new Grid();
		_grid->Initialize(_device.Get());*/

		/// DEBUG Obejct
		HelperObject* grid = ObjectManager::Instance().CreateHelperObject();
		grid->SetMesh("grid");
		HelperObject* axis = ObjectManager::Instance().CreateHelperObject();
		axis->SetMesh("axis");

		/// CUBEMAP
		_cubemap = new Cubemap();
		_cubemap->LoadMesh("skySphere");
		_cubemap->LoadCubeMapTexture("sunsetcube1024.dds");

		_spriteBatch = new DirectX::SpriteBatch(_deviceContext.Get());
		_lineBatch = new DirectX::PrimitiveBatch<DirectX::VertexPositionColor>(_deviceContext.Get());
		_basicEffect = std::make_unique<DirectX::BasicEffect>(_device.Get());
		_basicEffect->SetVertexColorEnabled(true);

		void const* shaderByteCode;
		size_t byteCodeLength;

		_basicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

		HR(_device->CreateInputLayout(DirectX::VertexPositionColor::InputElements,
			DirectX::VertexPositionColor::InputElementCount,
			shaderByteCode, byteCodeLength,
			&_lineInputLayout));

		SkinningMeshObject* test1 = ObjectManager::Instance().CreateSkinningMeshObject();
		test1->LoadMesh("Rob02.fbx");
		test1->LoadDiffuseMap("Rob02Yellow_AlbedoTransparency.png");
		test1->LoadNormalMap("Rob02White_Normal.png");
		test1->PlayAnimation(0, true);
	}

	void RocketDX11::BeginRender()
	{
		float color[4];

		// Setup the color to clear the buffer to.
		color[0] = 0.1f;	// r
		color[1] = 0.1f;	// g
		color[2] = 0.1f;	// b
		color[3] = 0.1f;	// a
		// Clear the back buffer.
		_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), color);
		// Clear the depth buffer.
		_deviceContext->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
		//d3dDeviceContext_->OMSetRenderTargets(1, renderTargetView_.GetAddressOf(), depthStencilView_.Get());

		return;
	}

	void RocketDX11::BeginRender(float r, float g, float b, float a)
	{
		float color[4];

		// Setup the color to clear the buffer to.
		color[0] = r;	// r
		color[1] = g;	// g
		color[2] = b;	// b
		color[3] = a;	// a
		// Clear the back buffer.
		_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), color);
		// Clear the depth buffer.
		_deviceContext->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
		//d3dDeviceContext_->OMSetRenderTargets(1, renderTargetView_.GetAddressOf(), depthStencilView_.Get());

		return;
	}

	void RocketDX11::RenderHelperObject()
	{
		/*auto vs = _resourceManager.GetVertexShader("ColorVS");
		auto ps = _resourceManager.GetPixelShader("ColorPS");

		_grid->Update(DirectX::XMMatrixIdentity(), Camera::GetMainCamera()->GetViewMatrix(), Camera::GetMainCamera()->GetProjectionMatrix());
		_grid->Render(_deviceContext.Get(), vs->GetVertexShader(), ps->GetPixelShader(), vs->GetMatrixBuffer(), vs->GetInputLayout());
		_axis->Update(DirectX::XMMatrixIdentity(), Camera::GetMainCamera()->GetViewMatrix(), Camera::GetMainCamera()->GetProjectionMatrix());
		_axis->Render(_deviceContext.Get(), vs->GetVertexShader(), ps->GetPixelShader(), vs->GetMatrixBuffer(), vs->GetInputLayout());*/

		for (auto& helperObject : ObjectManager::Instance().GetHelperObjList())
		{
			helperObject->Render();
		}
	}

	void RocketDX11::RenderStaticMesh()
	{
		Camera* mainCam = Camera::GetMainCamera();

		for (auto staticMeshObj : ObjectManager::Instance().GetStaticMeshObjList())
		{
			//staticMeshObj->Render(_deviceContext.Get(), mainCam->GetViewMatrix(), mainCam->GetProjectionMatrix());
			staticMeshObj->Render();
		}
	}

	void RocketDX11::RenderSkinningMesh()
	{
		for (auto skinningMeshObj : ObjectManager::Instance().GetSkinningMeshObjList())
		{
			skinningMeshObj->Render();
		}
	}

	void RocketDX11::RenderText()
	{
		_spriteBatch->Begin();
		for (auto textRenderer : ObjectManager::Instance().GetTextList())
		{
			textRenderer->Render(_spriteBatch);
		}
		_spriteBatch->End();
	}

	void RocketDX11::RenderTexture()
	{
		// 이미지(UI)를 그리기 위한 함수
		for (auto imageRenderer : ObjectManager::Instance().GetImageList())
		{
			imageRenderer->Render(_spriteBatch);
		}		
	}

	void RocketDX11::RenderLine()
	{
		_basicEffect->SetWorld(DirectX::XMMatrixIdentity());
		_basicEffect->SetView(Camera::GetMainCamera()->GetViewMatrix());
		_basicEffect->SetProjection(Camera::GetMainCamera()->GetProjectionMatrix());

		_basicEffect->Apply(_deviceContext.Get());

		_deviceContext->IASetInputLayout(_lineInputLayout.Get());

		_lineBatch->Begin();

		for (const auto& line : ObjectManager::Instance().GetLineRenderer()->GetLines())
		{
			_lineBatch->DrawLine(DirectX::VertexPositionColor(line.startPos, line.color), DirectX::VertexPositionColor(line.endPos, line.color));
		}
		_lineBatch->End();
		ObjectManager::Instance().GetLineRenderer()->Flush();
	}

	void RocketDX11::EndRender()
	{
		// Present the back buffer to the screen since rendering is complete.
		if (_vSyncEnabled)
		{
			// Lock to screen refresh rate.
			_swapChain->Present(1, 0);
		}
		else
		{
			// Present as fast as possible.
			_swapChain->Present(0, 0);
		}

		return;
	}

	void RocketDX11::Update(float deltaTime)
	{
		//Camera::GetMainCamera()->UpdateViewMatrix();
		//Camera::GetMainCamera()->UpdateProjectionMatrix();

		for (auto skinningMeshObj : ObjectManager::Instance().GetSkinningMeshObjList())
		{
			skinningMeshObj->Update(deltaTime);
		}
	}

	void RocketDX11::Render()
	{
		BeginRender(0.0f, 0.0f, 0.0f, 1.0f);

		EnableZBuffering();

		RenderHelperObject();
		RenderStaticMesh();
		RenderSkinningMesh();

		SetCubemapDSS();
		_cubemap->Render();

		RenderText();
		RenderTexture();
		RenderLine();
		EndRender();
	}

	void RocketDX11::Finalize()
	{
		//delete _grid;
		//delete _axis;
		delete _cubemap;
	}

	void RocketDX11::CreateDepthStencilStates()
	{
		// Initialize the depth stencil states
		D3D11_DEPTH_STENCIL_DESC enableDepthStencilDescription;
		ZeroMemory(&enableDepthStencilDescription, sizeof(enableDepthStencilDescription));

		enableDepthStencilDescription.DepthEnable = true;
		enableDepthStencilDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		enableDepthStencilDescription.DepthFunc = D3D11_COMPARISON_LESS;
		enableDepthStencilDescription.StencilEnable = true;
		enableDepthStencilDescription.StencilReadMask = 0xFF;
		enableDepthStencilDescription.StencilWriteMask = 0xFF;
		// Stencil operations if pixel is front-facing.
		enableDepthStencilDescription.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		enableDepthStencilDescription.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		enableDepthStencilDescription.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		enableDepthStencilDescription.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		// Stencil operations if pixel is back-facing.
		enableDepthStencilDescription.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		enableDepthStencilDescription.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		enableDepthStencilDescription.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		enableDepthStencilDescription.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Create the depth stencil state for enabling Z buffering
		_device->CreateDepthStencilState(&enableDepthStencilDescription, &_normalDepthStencilState);

		// Initialize the depth stencil states
		D3D11_DEPTH_STENCIL_DESC cubemapDepthStencilDescription;
		ZeroMemory(&cubemapDepthStencilDescription, sizeof(cubemapDepthStencilDescription));

		cubemapDepthStencilDescription.DepthEnable = true;
		cubemapDepthStencilDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		cubemapDepthStencilDescription.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		cubemapDepthStencilDescription.StencilEnable = true;
		cubemapDepthStencilDescription.StencilReadMask = 0xFF;
		cubemapDepthStencilDescription.StencilWriteMask = 0xFF;
		// Stencil operations if pixel is front-facing.
		cubemapDepthStencilDescription.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		cubemapDepthStencilDescription.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		cubemapDepthStencilDescription.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		cubemapDepthStencilDescription.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		// Stencil operations if pixel is back-facing.
		cubemapDepthStencilDescription.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		cubemapDepthStencilDescription.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		cubemapDepthStencilDescription.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		cubemapDepthStencilDescription.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Create the depth stencil state for cube mapping
		_device->CreateDepthStencilState(&cubemapDepthStencilDescription, &_cubemapDepthStencilState);
	}

	void RocketDX11::EnableZBuffering()
	{
		_deviceContext->OMSetDepthStencilState(_normalDepthStencilState.Get(), 1);
	}

	void RocketDX11::SetCubemapDSS()
	{
		_deviceContext->OMSetDepthStencilState(_cubemapDepthStencilState.Get(), 1);
	}

	void RocketDX11::SetLights()
	{
		DirectionalLight* dirLight = new DirectionalLight();
		dirLight->Color = XMFLOAT4{ 0.5f, 0.5f, 0.5f, 1.0f };
		dirLight->Direction = XMFLOAT3{ 10.0f, -10.0f, 0.0f };
		ResourceManager::Instance().GetPixelShader("PixelShader.cso")->SetDirectionalLight("dirLight", *dirLight);
		ResourceManager::Instance().GetPixelShader("SkeletonPixelShader.cso")->SetDirectionalLight("dirLight", *dirLight);

		PointLight pointLight[4];
		pointLight[0].Color = XMFLOAT4{ 0.3f, 0.0f, 0.0f, 1.0f };
		pointLight[0].Position = XMFLOAT4{ 5.0f, 5.0f, 0.0f, 1.0f };
		pointLight[1].Color = XMFLOAT4{ 0.0f, 0.3f, 0.0f, 1.0f };
		pointLight[1].Position = XMFLOAT4{ 0.0f, 3.0f, -5.0f, 1.0f };
		pointLight[2].Color = XMFLOAT4{ 0.5f, 0.3f, 0.0f, 1.0f };
		pointLight[2].Position = XMFLOAT4{ -1.0f, 0.0f, 0.0f, 1.0f };
		pointLight[3].Color = XMFLOAT4{ 0.2f, 0.2f, 0.2f, 1.0f };
		pointLight[3].Position = XMFLOAT4{ 0.0f, 3.0f, -10.0f, 1.0f };
		ResourceManager::Instance().GetPixelShader("PixelShader.cso")->SetPointLight("pointLight", pointLight);
		ResourceManager::Instance().GetPixelShader("SkeletonPixelShader.cso")->SetPointLight("pointLight", pointLight);

		SpotLight spotLight[2];
		spotLight[0].Color = XMFLOAT4{ 0.1f, 0.1f, 0.1f, 1.0f };
		spotLight[0].Direction = XMFLOAT3{ 10.0f, -3.0f, 0.0f };
		spotLight[0].Position = XMFLOAT4{ 20.0f, 10.0f, 0.0f, 1.0f };
		spotLight[0].SpotPower = 1.0f;
		spotLight[1].Color = XMFLOAT4{ 0.1f, 0.1f, 0.1f, 1.0f };
		spotLight[1].Direction = XMFLOAT3{ -10.0f, 0.0f, -5.0f };
		spotLight[1].Position = XMFLOAT4{ 10.0f, 20.0f, 5.0f, 1.0f };
		spotLight[1].SpotPower = 1.0f;
		ResourceManager::Instance().GetPixelShader("PixelShader.cso")->SetSpotLight("spotLight", spotLight);
		ResourceManager::Instance().GetPixelShader("SkeletonPixelShader.cso")->SetSpotLight("spotLight", spotLight);
	}

}
