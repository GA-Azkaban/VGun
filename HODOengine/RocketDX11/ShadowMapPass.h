#pragma once
#include <d3d11_2.h>
#include <wrl.h>
#include <DirectXMath.h>

namespace RocketCore::Graphics
{
	class DeferredBuffers;
	class VertexShader;
	class PixelShader;

	class ShadowMapPass
	{
	public:
		ShadowMapPass(DeferredBuffers* deferredBuffers);
		~ShadowMapPass();

		void Render();

	public:
		static constexpr UINT SHADOW_MAP_SIZE = 2048;

	private:
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext;
		DeferredBuffers* _deferredBuffers;
		VertexShader* _vertexShader;
		PixelShader* _pixelShader;
	};
}
