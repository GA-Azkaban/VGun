#pragma once

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
		DeferredBuffers* _deferredBuffers;
	};
}
