#pragma once
#include <d3d11.h>
#include <wrl.h>

namespace RocketCore::Graphics
{
	class DeferredBuffers;

	class GBufferPass
	{
	public:
		GBufferPass(DeferredBuffers* deferredBuffers);
		~GBufferPass();

		void Render();

	private:
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext;
		DeferredBuffers* _deferredBuffers;
	};
}
