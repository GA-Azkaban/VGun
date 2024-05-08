#pragma once

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

		// 임시
		BillboardObject* _billboard;
	};
}
