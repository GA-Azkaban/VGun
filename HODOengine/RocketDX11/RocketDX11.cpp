#include <cassert>
#include <windows.h>

#include "RocketDX11.h"
#include "CubeMesh.h"
#include "TextRenderer.h"
#include "ImageRenderer.h"
#include "VertexShader.h"
#include "PixelShader.h"

#include "RocketMacroDX11.h"
#include "DeviceBuilderDX11.h"

#include "ResourceManager.h"
#include "ObjectManager.h"
#include "Light.h"

#include "HelperObject.h"
#include "StaticMeshObject.h"
#include "SkinningMeshObject.h"
#include "ImageRenderer.h"
#include "LineRenderer.h"
#include "Cubemap.h"

#include "DeferredBuffers.h"
#include "QuadBuffer.h"
#include "ShadowMapPass.h"
#include "GBufferPass.h"
#include "SSAOPass.h"
#include "DeferredPass.h"
#include "DebugMeshPass.h"
#include "OutlinePass.h"
#include "SkyboxPass.h"
#include "ToneMapPass.h"
#include "SpritePass.h"
#include "ParticlePass.h"
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
		_featureLevel(), _m4xMsaaQuality(),
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

		ZeroMemory(&_viewport, sizeof(D3D11_VIEWPORT));
		_viewport.Height = static_cast<float>(_screenHeight);
		_viewport.Width = static_cast<float>(_screenWidth);
		_viewport.MinDepth = 0;
		_viewport.MaxDepth = 1;

		_resourceManager.Initialize(_device.Get(), _deviceContext.Get());

		/// Load resources
		/// FBX파일보다 Texture 파일들이 먼저 로드되어야 한다.

		/// tex

		// load all static texture
		const auto& SMtextures = GetEveryTextureFileNamesInFolder("Textures/StaticMesh");
		for (int i = 0; i < SMtextures.size(); ++i)
		{
			_resourceManager.LoadTextureFile("StaticMesh/" + SMtextures[i]);
		}

		// load all skinning texture
		const auto& SKMtextures = GetEveryTextureFileNamesInFolder("Textures/Character");
		for (int i = 0; i < SKMtextures.size(); ++i)
		{
			_resourceManager.LoadTextureFile("Character/" + SKMtextures[i]);
		}

		// load all weapon texture
		const auto& WPtextures = GetEveryTextureFileNamesInFolder("Textures/Weapons");
		for (int i = 0; i < WPtextures.size(); ++i)
		{
			_resourceManager.LoadTextureFile("Weapons/" + WPtextures[i]);
		}

		// load all particle texture
		const auto& PCtextures = GetEveryTextureFileNamesInFolder("Textures/Particles");
		for (int i = 0; i < PCtextures.size(); ++i)
		{
			_resourceManager.LoadTextureFile("Particles/" + PCtextures[i]);
		}

		// load all skybox texture
		_resourceManager.LoadCubeMapTextureFile("sunsetcube1024.dds");
		_resourceManager.LoadCubeMapTextureFile("Day Sun Peak Clear.dds");

		/// fbx

		// load all static mesh
		const auto& SMfiles = GetEveryMeshFileNamesInFolder("Models/SM");
		for (int i = 0; i < SMfiles.size(); ++i)
		{
			_resourceManager.LoadFBXFile("SM/" + SMfiles[i]);
		}

		// load all character mesh
		const auto& TPfiles = GetEveryMeshFileNamesInFolder("Models/TP");
		for (int i = 0; i < TPfiles.size(); ++i)
		{
			_resourceManager.LoadFBXFile("TP/" + TPfiles[i]);
		}
		const auto& FPfiles = GetEveryMeshFileNamesInFolder("Models/FP");
		for (int i = 0; i < FPfiles.size(); ++i)
		{
			_resourceManager.LoadFBXFile("FP/" + FPfiles[i]);
		}

		CreateDepthStencilStates();

		LightManager::Instance().SetGlobalAmbient(XMFLOAT4(0.1, 0.1, 0.1, 1));

		_deferredBuffers = new DeferredBuffers(_device.Get(), _deviceContext.Get());
		_quadBuffer = new QuadBuffer(_device.Get(), _deviceContext.Get());
		_stencilEnableBuffer = new QuadBuffer(_device.Get(), _deviceContext.Get());
		_toneMapBuffer = new QuadBuffer(_device.Get(), _deviceContext.Get());
		_deferredBuffers->Initialize(_screenWidth, _screenHeight);
		_quadBuffer->Initialize(_screenWidth, _screenHeight);
		_stencilEnableBuffer->Initialize(_screenWidth, _screenHeight);
		_toneMapBuffer->Initialize(_screenWidth, _screenHeight);

		_shadowMapPass = new ShadowMapPass(_deferredBuffers);
		_GBufferPass = new GBufferPass(_deferredBuffers);
		_SSAOPass = new SSAOPass(_deferredBuffers);
		_deferredPass = new DeferredPass(_deferredBuffers, _quadBuffer);
		_debugMeshPass = new DebugMeshPass(_deferredBuffers, _quadBuffer);
		_outlinePass = new OutlinePass(_deferredBuffers, _quadBuffer, _stencilEnableBuffer);
		_skyboxPass = new SkyboxPass(_deferredBuffers, _quadBuffer);
		_toneMapPass = new ToneMapPass(_quadBuffer, _toneMapBuffer);
		_spritePass = new SpritePass(_toneMapBuffer);
		_particlePass = new ParticlePass(_deferredBuffers, _quadBuffer);
		_blitPass = new BlitPass(_toneMapBuffer, _renderTargetView.Get());

		Cubemap::Instance()._deferredBuffers = _deferredBuffers;
		Cubemap::Instance().LoadCubeMapTexture("Day Sun Peak Clear.dds");
		Cubemap::Instance().SetEnvLightIntensity(0.8f);

		/// DEBUG Obejct
		HelperObject* grid = ObjectManager::Instance().CreateHelperObject();
		grid->LoadMesh("grid");
		HelperObject* axis = ObjectManager::Instance().CreateHelperObject();
		axis->LoadMesh("axis");

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

	void RocketDX11::Finalize()
	{

	}

	void RocketDX11::OnResize(int screenWidth, int screenHeight)
	{
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;

		// 투영 행렬 재계산
		Camera::GetMainCamera()->SetAspect(static_cast<float>(_screenWidth / _screenHeight));
		Camera::GetMainCamera()->UpdateProjectionMatrix();

		_renderTargetView.Reset();

		_swapChain->ResizeBuffers(0, _screenWidth, _screenHeight, DXGI_FORMAT_UNKNOWN, 0);

		ID3D11Texture2D* backBuffer;
		HR(_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
		HR(_device->CreateRenderTargetView(backBuffer, 0, _renderTargetView.GetAddressOf()));
		backBuffer->Release();

		_deferredBuffers->Initialize(_screenWidth, _screenHeight);
		_quadBuffer->Initialize(_screenWidth, _screenHeight);
		_stencilEnableBuffer->Initialize(_screenWidth, _screenHeight);
		_toneMapBuffer->Initialize(_screenWidth, _screenHeight);
		_SSAOPass->CreateTexture(_screenWidth, _screenHeight);

		// set the viewport transform
		_viewport.TopLeftX = 0;
		_viewport.TopLeftY = 0;
		_viewport.Width = static_cast<float>(_screenWidth);
		_viewport.Height = static_cast<float>(_screenHeight);
		_viewport.MinDepth = 0.0f;
		_viewport.MaxDepth = 1.0f;

		//_deviceContext->RSSetViewports(1, &_viewport);
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
		LightManager::Instance().UpdateViewProj();

		for (auto skinningMeshObj : ObjectManager::Instance().GetSkinningMeshObjList())
		{
			skinningMeshObj->Update(deltaTime);
		}

	}

	void RocketDX11::Render()
	{
		SetDepthStencilState(_shadowmapDepthStencilState.Get());
		_shadowMapPass->Render();

		SetDepthStencilState(_depthStencilStateEnable.Get());
		_GBufferPass->Render();
		SetDepthStencilState(_depthStencilStateEnable.Get());
		_SSAOPass->Render();
		_deferredPass->Render();
		//_particlePass->Render();
		_outlinePass->Render();

#ifdef _DEBUG
		_debugMeshPass->Render();
		RenderLine();
#endif

		SetDepthStencilState(_cubemapDepthStencilState.Get());
		_skyboxPass->Render();

		SetDepthStencilState(_depthStencilStateEnable.Get());
		_particlePass->Render();

		SetDepthStencilState(_depthStencilStateDisable.Get());
		_toneMapPass->Render();
		_spritePass->Render();

		_deviceContext->RSSetViewports(1, &_viewport);
		_blitPass->Render();

		EndRender();
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
		//enableDepthStencilDescription.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		//enableDepthStencilDescription.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;
		//enableDepthStencilDescription.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		//// Stencil operations if pixel is back-facing.
		//enableDepthStencilDescription.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		//enableDepthStencilDescription.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		//enableDepthStencilDescription.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		//enableDepthStencilDescription.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;

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

		D3D11_DEPTH_STENCIL_DESC shadowDepthStencilDescription;
		ZeroMemory(&shadowDepthStencilDescription, sizeof(shadowDepthStencilDescription));

		shadowDepthStencilDescription.DepthEnable = true;
		shadowDepthStencilDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		shadowDepthStencilDescription.DepthFunc = D3D11_COMPARISON_LESS;
		shadowDepthStencilDescription.StencilEnable = false;

		// Create the depth stencil state for enabling Z buffering
		_device->CreateDepthStencilState(&shadowDepthStencilDescription, &_shadowmapDepthStencilState);
	}

	void RocketDX11::SetDepthStencilState(ID3D11DepthStencilState* dss)
	{
		_deviceContext->OMSetDepthStencilState(dss, 0);
	}

	std::vector<std::string>& RocketDX11::GetEveryMeshFileNamesInFolder(const std::string filePath)
	{
		_fileNames.clear();

		std::string searchPath = "Resources/" + filePath + "/*.*";
		WIN32_FIND_DATAA fileData;
		HANDLE hFind = FindFirstFileA(searchPath.c_str(), &fileData);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (!(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					std::string name = fileData.cFileName;
					if (name.find("SM_") == 0 ||
						name.find("SKM_") == 0)
					{
						_fileNames.push_back(fileData.cFileName);
					}
				}
			} while (FindNextFileA(hFind, &fileData));
			FindClose(hFind);
		}

		return _fileNames;
	}

	std::vector<std::string>& RocketDX11::GetEveryTextureFileNamesInFolder(const std::string filePath)
	{
		std::string searchPath = "Resources/" + filePath + "/*.*";
		WIN32_FIND_DATAA fileData;
		HANDLE hFind = FindFirstFileA(searchPath.c_str(), &fileData);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (!(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					_fileNames.push_back(fileData.cFileName);
				}
			} while (FindNextFileA(hFind, &fileData));
			FindClose(hFind);
		}

		return _fileNames;
	}

}
