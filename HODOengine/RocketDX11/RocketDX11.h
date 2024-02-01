#pragma once
#include <windows.h>
#include <d3d11_2.h>
#include <dxgi1_3.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <DXTK/SpriteBatch.h>
#include <DXTK/PrimitiveBatch.h>
#include <DXTK/VertexTypes.h>
#include <DXTK/Effects.h>

#include "..\\HODO3DGraphicsInterface\\I3DRenderer.h"
#include "Camera.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")

using Microsoft::WRL::ComPtr;

namespace RocketCore::Graphics
{
	class Grid;
	class Axis;
	class CubeMesh;
	class VertexShader;
	class PixelShader;
	class ResourceManager;
	class ImageRenderer;
	class DeferredBuffers;
	class QuadBuffer;
	class GBufferPass;
	class DeferredPass;
	class SkyboxPass;
	class BlitPass;
	
	class RocketDX11 final : public HDEngine::I3DRenderer
	{
	public:
		RocketDX11();
		~RocketDX11();

	public:
		//그래픽스 엔진을 초기화한다.
		virtual void Initialize(void* hWnd, int screenWidth, int screenHeight) override;

		virtual void Update(float deltaTime) override;

		virtual void Render() override;

		virtual void Finalize() override;

	private:
		void BeginRender();
		void BeginRender(float r, float g, float b, float a);
		void RenderHelperObject();
		/*void RenderStaticMesh();
		void RenderSkinningMesh();*/
		void RenderText();
		void RenderLine();
		void RenderTexture();
		void RenderDebug();
		void EndRender();

		void CreateDepthStencilStates();

		void SetDepthStencilState(ID3D11DepthStencilState* dss);
		// 임시
		void SetLights();

		/// Initialize Member
	private:
		HWND _hWnd;
		int _screenWidth;
		int _screenHeight;
		bool _vSyncEnabled;

		/// 초기화 관련
	private:
		ComPtr<ID3D11Device> _device;
		ComPtr<ID3D11DeviceContext> _deviceContext;	// immediateContext
		D3D_FEATURE_LEVEL _featureLevel;

		UINT _m4xMsaaQuality;

		ComPtr<IDXGISwapChain> _swapChain;
		ComPtr<ID3D11RenderTargetView> _renderTargetView;
		D3D11_VIEWPORT _viewport;

	private:
		ComPtr<ID3D11DepthStencilState> _depthStencilStateEnable;
		ComPtr<ID3D11DepthStencilState> _depthStencilStateDisable;
		ComPtr<ID3D11DepthStencilState> _cubemapDepthStencilState;

	private:
		DirectX::SpriteBatch* _spriteBatch;
		DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* _lineBatch;
		std::unique_ptr<DirectX::BasicEffect> _basicEffect;
		ComPtr<ID3D11InputLayout> _lineInputLayout;

	private:
		ResourceManager& _resourceManager;

	private:
		DeferredBuffers* _deferredBuffers;
		QuadBuffer* _quadBuffer;
		GBufferPass* _GBufferPass;
		DeferredPass* _deferredPass;
		SkyboxPass* _skyboxPass;
		BlitPass* _blitPass;
	};
}
