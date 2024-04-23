#pragma once
#include <d3d11_2.h>
#include <wrl.h>
#include <DirectXMath.h>

namespace RocketCore::Graphics
{
	class DeferredBuffers;
	class VertexShader;
	class GeometryShader;
	class PixelShader;

	class ShadowMapPass
	{
	public:
		ShadowMapPass();
		~ShadowMapPass();

		void Render();

	public:
		static constexpr UINT SHADOW_MAP_SIZE = 2048;
		static constexpr UINT CASCADE_COUNT = 3;

	private:
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> _shadowMapTexture;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> _shadowMapDSV[CASCADE_COUNT];
		//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _shadowMapSRV;
		
		VertexShader* _vertexShader;
		GeometryShader* _geometryShader;
		PixelShader* _pixelShader;
	};
}
