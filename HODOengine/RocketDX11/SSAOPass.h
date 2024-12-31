#pragma once
#include <d3d11_2.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

namespace RocketCore::Graphics
{
	class DeferredBuffers;
	class VertexShader;
	class PixelShader;

	class SSAOPass
	{
	public:
		SSAOPass(DeferredBuffers* deferredBuffers);
		~SSAOPass();
		void Render();
		void CreateTexture(UINT screenWidth, UINT screenHeight);

	private:
		DeferredBuffers* _deferredBuffers;
		VertexShader* _vertexShader;
		PixelShader* _pixelShader;
		DirectX::XMFLOAT4 _ssaoSamples[64];
		DirectX::XMFLOAT2 _windowSize;

		Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> _noiseTexture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _noiseSRV;
	};
}
