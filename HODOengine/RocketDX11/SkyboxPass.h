﻿#pragma once

namespace RocketCore::Graphics
{
	class DeferredBuffers;
	class QuadBuffer;

	class SkyboxPass
	{
	public:
		SkyboxPass(DeferredBuffers* deferredBuffers, QuadBuffer* quadBuffer);
		~SkyboxPass();
		void Render();

	private:
		DeferredBuffers* _deferredBuffers;
		QuadBuffer* _quadBuffer;
	};
}
