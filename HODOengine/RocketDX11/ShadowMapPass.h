#pragma once
#include <d3d11_2.h>
#include <wrl.h>
#include <DirectXMath.h>

namespace RocketCore::Graphics
{
	class DeferredBuffers;
	class VertexShader;
	class PixelShader;
	class Light;

	class ShadowMapPass
	{
	public:
		ShadowMapPass(DeferredBuffers* deferredBuffers);
		~ShadowMapPass();

		void Render();

		DirectX::XMMATRIX GetLightView() { return _view; }
		DirectX::XMMATRIX GetLightProj() { return _proj; }

	private:
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext;
		DeferredBuffers* _deferredBuffers;
		VertexShader* _vertexShader;
		PixelShader* _pixelShader;

		DirectX::XMMATRIX _view;
		DirectX::XMMATRIX _proj;
		Light* _directionalLight;
	};
}
