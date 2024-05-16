#pragma once
#include <d3d11.h>
#include <wrl.h>

namespace RocketCore::Graphics
{
	class QuadBuffer;
	class BillboardParticle;

	class ParticlePass
	{
	public:
		ParticlePass(QuadBuffer* toneMapBuffer);
		~ParticlePass();

		void Render();

	private:
		QuadBuffer* _toneMapBuffer;
		Microsoft::WRL::ComPtr<ID3D11BlendState> _blendState;
		// 임시
		BillboardParticle* _billboard;
		const float _blendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	};
}
