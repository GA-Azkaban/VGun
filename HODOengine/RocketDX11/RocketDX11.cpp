#include "RocketDX11.h"
#include "Grid.h"
#include "Axis.h"
#include "VertexShader.h"
#include "PixelShader.h"

#include "RocketMacroDX11.h"
#include "DeviceBuilderDX11.h"

#include <cassert>

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
		_viewport(), _wireframeRenderState(), _solidRenderState(), _NormalDepthStencilState()
	{

	}

	RocketDX11::~RocketDX11()
	{

	}

	void RocketDX11::Initialize(void* hWnd, int screenWidth, int screenHeight, bool isEditor /*= false*/)
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

		// Render State
		CreateRenderStates();

		_axis = new Axis(_device.Get(), _deviceContext.Get(), _wireframeRenderState.Get());
		_axis->Initialize();
		
		_vertexShader = new VertexShader();
		_vertexShader->CreateShader(_device.Get(), "../x64/Debug/VertexShader.cso");

		_pixelShader = new PixelShader();
		_pixelShader->CreateShader(_device.Get(), "../x64/Debug/PixelShader.cso");

		_grid = new Grid();
		_grid->Initialize(_device.Get());
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

	void RocketDX11::RenderMesh()
	{
		// 관리하는 RenderableObject를 그리는 것

	}

	void RocketDX11::RenderText()
	{

	}

	void RocketDX11::RenderTexture()
	{

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



	void RocketDX11::CreateRenderStates()
	{
		// Render State 중 Rasterizer State
		D3D11_RASTERIZER_DESC solidDesc;
		ZeroMemory(&solidDesc, sizeof(D3D11_RASTERIZER_DESC));
		solidDesc.FillMode = D3D11_FILL_SOLID;
		solidDesc.CullMode = D3D11_CULL_BACK;
		solidDesc.FrontCounterClockwise = false;
		solidDesc.DepthClipEnable = true;

		HR(_device->CreateRasterizerState(&solidDesc, &_solidRenderState));

		D3D11_RASTERIZER_DESC wireframeDesc;
		ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
		wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
		wireframeDesc.CullMode = D3D11_CULL_BACK;
		wireframeDesc.FrontCounterClockwise = false;
		wireframeDesc.DepthClipEnable = true;

		HR(_device->CreateRasterizerState(&wireframeDesc, &_wireframeRenderState));

		//
		// 폰트용 DSS
		//
		D3D11_DEPTH_STENCIL_DESC equalsDesc;
		ZeroMemory(&equalsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		equalsDesc.DepthEnable = true;
		equalsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;		// 깊이버퍼에 쓰기는 한다
		equalsDesc.DepthFunc = D3D11_COMPARISON_LESS;

		HR(_device->CreateDepthStencilState(&equalsDesc, &_NormalDepthStencilState));

	}

	void RocketDX11::UpdateCamera(const HDEngine::CameraData & cameraData)
	{
		_camera.SetPosition(cameraData.position.x, cameraData.position.y, cameraData.position.z);
		_camera.SetRotation(cameraData.rotation.w, cameraData.rotation.x, cameraData.rotation.y, cameraData.rotation.z);
		_camera.SetFrustum(cameraData.fovY, cameraData.aspect, cameraData.nearZ, cameraData.farZ);
		_camera.UpdateViewMatrix();
	}

	void RocketDX11::Render()
	{
		BeginRender(0.0f, 0.0f, 0.0f, 1.0f);
		_grid->Update(DirectX::XMMatrixIdentity(), _camera.GetViewMatrix(), _camera.GetProjectionMatrix());
		_grid->Render(_deviceContext.Get(), _vertexShader->GetVertexShader(), _pixelShader->GetPixelShader(), _vertexShader->GetMatrixBuffer(), _vertexShader->GetInputLayout(), _wireframeRenderState.Get());
		_axis->Update(DirectX::XMMatrixIdentity(), _camera.GetViewMatrix(), _camera.GetProjectionMatrix());
		_axis->Render();
		RenderMesh();
		EndRender();
	}

	void RocketDX11::Finalize()
	{
		delete _grid;
		delete _axis;
	}
}
