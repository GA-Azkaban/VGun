#pragma once
#include <d3d11.h>
#include <wrl.h>

namespace RocketCore::Graphics
{
	class DeferredBuffers;
	class QuadBuffer;

	class DecalPass
	{
	public:
		DecalPass(DeferredBuffers* deferredBuffers, QuadBuffer* quadBuffer);
		~DecalPass();

		void Render();

	private:
		DeferredBuffers* _deferredBuffers;
		QuadBuffer* _quadBuffer;
		Microsoft::WRL::ComPtr<ID3D11BlendState> _blendState;
		const float _blendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	};
}
