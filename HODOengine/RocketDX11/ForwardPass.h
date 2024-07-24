#pragma once
#include <d3d11.h>
#include <wrl.h>

namespace RocketCore::Graphics
{
	class DeferredBuffers;
	class QuadBuffer;

	class ForwardPass
	{
	public:
		ForwardPass(DeferredBuffers* deferredBuffers, QuadBuffer* quadBuffer);
		~ForwardPass();

		void Render();

	private:
		DeferredBuffers* _deferredBuffers;
		QuadBuffer* _quadBuffer;
	};
}
