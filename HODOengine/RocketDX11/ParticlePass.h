#pragma once
#include <d3d11.h>
#include <wrl.h>

namespace RocketCore::Graphics
{
	class DeferredBuffers;
	class QuadBuffer;
	class Particle;

	class ParticlePass
	{
	public:
		ParticlePass(DeferredBuffers* deferredBuffers, QuadBuffer* quadBuffer);
		~ParticlePass();

		void Render();

	private:
		DeferredBuffers* _deferredBuffers;
		QuadBuffer* _quadBuffer;
		Microsoft::WRL::ComPtr<ID3D11BlendState> _defaultBlendState;
		Microsoft::WRL::ComPtr<ID3D11BlendState> _additiveBlendState;
		const float _blendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	};
}
