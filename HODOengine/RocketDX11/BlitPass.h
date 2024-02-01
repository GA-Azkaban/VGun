#pragma once

namespace RocketCore::Graphics
{
	class QuadBuffer;
	class VertexShader;
	class PixelShader;
	class ResourceManager;

	class BlitPass
	{
	public:
		BlitPass(QuadBuffer* quadBuffer);
		~BlitPass();

		void Render();

	private:
		QuadBuffer* _quadBuffer;
		VertexShader* _vertexShader;
		PixelShader* _pixelShader;

		ResourceManager& _resourceManager;
	};
}
