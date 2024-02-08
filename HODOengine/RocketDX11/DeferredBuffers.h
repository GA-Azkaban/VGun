#pragma once
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
		void SetEnvironmentMap(std::string fileName);
		
		ID3D11ShaderResourceView* GetShaderResourceView(UINT index);
		ID3D11DepthStencilView* GetDepthStencilView();
		ID3D11ShaderResourceView* GetDepthSRV();
		ID3D11ShaderResourceView* GetEnvMap();
		ID3D11ShaderResourceView* GetEnvPreFilterMap();
		ID3D11ShaderResourceView* GetBRDFLut();

		void ClearRenderTargets(DirectX::XMVECTOR color = DirectX::XMVECTOR{ 0.0f, 0.0f, 0.0f, 0.0f });
		void FlushShaderResourceViews();

	private:
		UINT m_textureWidth;
		UINT m_textureHeight;

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
		//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _envMap;
		//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _envPreFilter;
		//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _brdfLUT; 
		ID3D11ShaderResourceView* _envMap;
		ID3D11ShaderResourceView* _envPreFilter;
		ID3D11ShaderResourceView* _brdfLUT;
	};
}
