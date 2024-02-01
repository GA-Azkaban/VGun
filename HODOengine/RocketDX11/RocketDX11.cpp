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

#include "DeferredBuffers.h"
#include "QuadBuffer.h"
#include "GBufferPass.h"
#include "DeferredPass.h"
#include "SkyboxPass.h"
#include "BlitPass.h"

#include "../HODO3DGraphicsInterface/PrimitiveHeader.h"

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
		_swapChain(), _renderTargetView(), _viewport(),
		_resourceManager(ResourceManager::Instance())
	{

	}

	RocketDX11::~RocketDX11()
	{

	}

	void RocketDX11::Initialize(void* hWnd, int screenWidth, int screenHeight)
	{
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

		ID3D11Texture2D* backBuffer;
		hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
		hr = _device->CreateRenderTargetView(backBuffer, nullptr, _renderTargetView.GetAddressOf());

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
		_resourceManager.LoadFBXFile("A_TP_CH_Breathing.fbx");
		_resourceManager.LoadTextureFile("sunsetcube1024.dds");

		CreateDepthStencilStates();
		SetLights();

		_deferredBuffers = new DeferredBuffers(_device.Get(), _deviceContext.Get());
		_quadBuffer = new QuadBuffer(_device.Get(), _deviceContext.Get());

		_GBufferPass = new GBufferPass(_deferredBuffers);
		_deferredPass = new DeferredPass(_deferredBuffers, _quadBuffer);
		_skyboxPass = new SkyboxPass(_deferredBuffers, _quadBuffer);
		_blitPass = new BlitPass(_quadBuffer);

		/// DEBUG Obejct
		HelperObject* grid = ObjectManager::Instance().CreateHelperObject();
		grid->SetMesh("grid");
		HelperObject* axis = ObjectManager::Instance().CreateHelperObject();
		axis->SetMesh("axis");

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
		for (auto& helperObject : ObjectManager::Instance().GetHelperObjList())
		{
			helperObject->Render();
		}
	}

	/*void RocketDX11::RenderStaticMesh()
	{
		for (auto staticMeshObj : ObjectManager::Instance().GetStaticMeshObjList())
		{
			staticMeshObj->Render();
		}
	}

	void RocketDX11::RenderSkinningMesh()
	{
		for (auto skinningMeshObj : ObjectManager::Instance().GetSkinningMeshObjList())
		{
			skinningMeshObj->Render();
		}
	}*/

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
		Camera::GetMainCamera()->UpdateViewMatrix();

		for (auto skinningMeshObj : ObjectManager::Instance().GetSkinningMeshObjList())
		{
			skinningMeshObj->Update(deltaTime);
		}
	}

	void RocketDX11::Render()
	{
		SetDepthStencilState(_depthStencilStateEnable.Get());
		_GBufferPass->Render();
		_deferredPass->Render();

		//RenderHelperObject();
		//RenderStaticMesh();
		//RenderSkinningMesh();

		SetDepthStencilState(_cubemapDepthStencilState.Get());
		_skyboxPass->Render();

		RenderTexture();
		RenderLine();
		RenderText();		
		RenderDebug();		

		SetDepthStencilState(_depthStencilStateDisable.Get());
		_blitPass->Render();

		EndRender();
	}

	void RocketDX11::Finalize()
	{
		
	}

	void RocketDX11::CreateDepthStencilStates()
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
		_device->CreateDepthStencilState(&enableDepthStencilDescription, &_depthStencilStateEnable);

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
		_device->CreateDepthStencilState(&disableDepthStencilDescription, &_depthStencilStateDisable);

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

	void RocketDX11::SetDepthStencilState(ID3D11DepthStencilState* dss)
	{
		_deviceContext->OMSetDepthStencilState(dss, 1);
	}

	void RocketDX11::SetLights()
	{
		DirectionalLight* dirLight = new DirectionalLight();
		dirLight->Color = XMFLOAT4{ 0.3f, 0.3f, 0.3f, 1.0f };
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

	void RocketDX11::RenderDebug()
	{
#ifdef _DEBUG
		Camera* cam = Camera::GetMainCamera();

		for (auto e : ObjectManager::Instance().GetCubePrimitiveList())
		{
			e->worldTM.m[0][0] *= e->widthHeightDepth.x;
			e->worldTM.m[1][1] *= e->widthHeightDepth.y;
			e->worldTM.m[2][2] *= e->widthHeightDepth.z;

			ResourceManager::Instance().GetCubePrimitive()->Draw(e->worldTM, cam->GetViewMatrix(), cam->GetProjectionMatrix(), e->color, nullptr, true);
		}

		for (auto e : ObjectManager::Instance().GetSpherePrimitiveList())
		{
			e->worldTM.m[0][0] *= e->diameter;
			e->worldTM.m[1][1] *= e->diameter;
			e->worldTM.m[2][2] *= e->diameter;
			
			ResourceManager::Instance().GetSpherePrimitive()->Draw(e->worldTM, cam->GetViewMatrix(), cam->GetProjectionMatrix(), e->color, nullptr, true);
		}

		for (auto e : ObjectManager::Instance().GetCylinderPrimitiveList())
		{
			e->worldTM.m[0][0] *= e->diameter;
			e->worldTM.m[1][1] *= e->height/2;
			e->worldTM.m[2][2] *= e->diameter;

			ResourceManager::Instance().GetCylinderPrimitive()->Draw(e->worldTM, cam->GetViewMatrix(), cam->GetProjectionMatrix(), e->color, nullptr, true);
		}

		_spriteBatch->Begin();
		for (auto textRenderer : ObjectManager::Instance().GetTextList())
		{
			textRenderer->RenderDebug(_spriteBatch);
		}
		_spriteBatch->End();
	}
#endif
}
