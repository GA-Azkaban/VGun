#pragma once
#include <d3d11_2.h>
#include <DirectXMath.h>

namespace RocketCore::Graphics
{
	class DeferredBuffers;
	class QuadBuffer;
	class VertexShader;
	class PixelShader;
	class EnvMapInfo;

	class DeferredPass
	{
	public:
		DeferredPass(DeferredBuffers* deferredBuffers, QuadBuffer* quadBuffer);

		void Render();

	private:
		DeferredBuffers* _deferredBuffers;
		QuadBuffer* _quadBuffer;

		VertexShader* _vertexShader;
		PixelShader* _pixelShader;
	};
}
