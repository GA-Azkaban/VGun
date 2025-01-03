﻿#pragma once
#include <d3d11_2.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <string>

namespace RocketCore::Graphics
{
	// Render Target Type
	enum class BUFFERTYPE
	{
		BUFFER_POSITION,
		BUFFER_DIFFUSE,
		BUFFER_NORNAL,
		BUFFER_METALROUGHOCCLUSION,

		GBUFFER_COUNT,
	};
	class DeferredBuffers
	{
	public:
		DeferredBuffers(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
		~DeferredBuffers();

		void Initialize(UINT textureWidth, UINT textureHeight);

		void SetRenderTargets();
		// 임시
		void SetSSAORenderTarget();
		void SetShadowMapRenderTarget();

		void SetEnvironmentMap(std::string fileName);
		
		UINT GetScreenWidth() { return _textureWidth; }
		UINT GetScreenHeight() { return _textureHeight; }
		
		ID3D11ShaderResourceView* GetShaderResourceView(UINT index);
		ID3D11DepthStencilView* GetDepthStencilView();
		ID3D11ShaderResourceView* GetDepthSRV();
		ID3D11ShaderResourceView* GetEnvMap();
		ID3D11ShaderResourceView* GetEnvPreFilterMap();
		ID3D11ShaderResourceView* GetBRDFLut();
		ID3D11ShaderResourceView* GetSSAOMap();
		ID3D11ShaderResourceView* GetShadowMap();

		void ClearRenderTargets(DirectX::XMVECTOR color = DirectX::XMVECTOR{ 0.0f, 0.0f, 0.0f, 0.0f });
		void ClearDepthStencil();
		// 임시
		void ClearSSAORenderTarget(DirectX::XMVECTOR color = DirectX::XMVECTOR{ 0.0f, 0.0f, 0.0f, 0.0f });
		void ClearShadowMapRenderTarget(DirectX::XMVECTOR color = DirectX::XMVECTOR{ 0.0f, 0.0f, 0.0f, 0.0f });
		
		void FlushShaderResourceViews();

	private:
		UINT _textureWidth;
		UINT _textureHeight;

		Microsoft::WRL::ComPtr<ID3D11Device> _device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext;

		// G-Buffer
		Microsoft::WRL::ComPtr<ID3D11Texture2D> _renderTargetTextures[(int)BUFFERTYPE::GBUFFER_COUNT];
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _renderTargetViews[(int)BUFFERTYPE::GBUFFER_COUNT];
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _shaderResourceViews[(int)BUFFERTYPE::GBUFFER_COUNT];

		Microsoft::WRL::ComPtr<ID3D11Texture2D> _depthTexture;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> _depthStencilView;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _depthShaderResourceView;

		// Env map
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _envMap;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _envPreFilter;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _brdfLUT;
		
		// AO map
		Microsoft::WRL::ComPtr<ID3D11Texture2D> _ssaoTexture;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _ssaoRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _ssaoShaderResourceView;

		// Shadow map
		Microsoft::WRL::ComPtr<ID3D11Texture2D> _shadowMapTexture;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _shadowMapRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _shadowMapShaderResourceView;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> _shadowMapDepthTexture;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> _shadowMapDSV;
	};
}
