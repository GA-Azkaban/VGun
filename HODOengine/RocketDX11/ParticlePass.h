#pragma once
#include <d3d11.h>
#include <wrl.h>

namespace RocketCore::Graphics
{
	class QuadBuffer;
	class BillboardObject;

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
		BillboardObject* _billboard;
		const float _blendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	};
}
