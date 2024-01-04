#pragma once
#include <windows.h>
#include <d3d11_2.h>
#include <dxgi1_3.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <DXTK/SpriteBatch.h>

#include "..\\HODO3DGraphicsInterface\\I3DRenderer.h"
#include "Camera.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

using Microsoft::WRL::ComPtr;

namespace RocketCore::Graphics
{
	class Grid;
	class Axis;
	class CubeMesh;
	class VertexShader;
	class PixelShader;
	class ResourceManager;
	
	class RocketDX11 final : public HDEngine::I3DRenderer
	{
	public:
		RocketDX11();
		~RocketDX11();

	public:
		//그래픽스 엔진을 초기화한다.
		virtual void Initialize(void* hWnd, int screenWidth, int screenHeight) override;

		virtual void Render() override;

		virtual void Finalize() override;

	private:
		void Update();

		/// 렌더스테이트 제작 함수
	private:
		void CreateRenderStates();

	private:
		void BeginRender();
		void BeginRender(float r, float g, float b, float a);
		void RenderHelperObject();
		void RenderStaticMesh();
		void RenderText();
		void RenderTexture();
		void EndRender();

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
		ComPtr<ID3D11Texture2D> _backBuffer;
		ComPtr<ID3D11RenderTargetView> _renderTargetView;
		ComPtr<ID3D11Texture2D> _depthStencilBuffer;
		ComPtr<ID3D11DepthStencilView> _depthStencilView;
		D3D11_VIEWPORT _viewport;

		/// Render State
		// 미리 여러 세트를 만들어두고 스위칭한다.
	private:
		ComPtr<ID3D11RasterizerState> _wireframeRenderState;
		ComPtr<ID3D11RasterizerState> _solidRenderState;
		// 폰트때문에 뎁스스탠실 스테이트가 강제가 됐다. 
		ComPtr<ID3D11DepthStencilState> _NormalDepthStencilState;

	private:
		Grid* _grid;
		Axis* _axis;
		DirectX::SpriteBatch* _spriteBatch;

	private:
		ResourceManager& _resourceManager;
	};
}
