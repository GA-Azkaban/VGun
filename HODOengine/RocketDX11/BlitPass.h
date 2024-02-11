#pragma once
#include <d3d11_2.h>
#include <wrl.h>

namespace RocketCore::Graphics
{
	class QuadBuffer;
	class VertexShader;
	class PixelShader;
	class ResourceManager;

	class BlitPass
	{
	public:
		BlitPass(QuadBuffer* toneMapBuffer, ID3D11RenderTargetView* backBufferRTV);
		~BlitPass();

		void Render();

	private:
		QuadBuffer* _toneMapBuffer;
		VertexShader* _vertexShader;
		PixelShader* _pixelShader;

		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _backBufferRTV;
		ResourceManager& _resourceManager;
	};
}
