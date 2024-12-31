#pragma once

namespace RocketCore::Graphics
{
	class DeferredBuffers;
	class QuadBuffer;

	class DebugMeshPass
	{
	public:
		DebugMeshPass(DeferredBuffers* deferredBuffers, QuadBuffer* quadBuffer);
		~DebugMeshPass();
		void Render();

	private:
		DeferredBuffers* _deferredBuffers;
		QuadBuffer* _quadBuffer;
	};
}
