#pragma once
namespace RocketCore::Graphics
{
	class QuadBuffer;
	class VertexShader;
	class PixelShader;

	class PostProcessPass
	{
	public:
		PostProcessPass(QuadBuffer* quadBuffer);
		~PostProcessPass();
		void Render();

	private:
		QuadBuffer* _quadBuffer;

		VertexShader* _vertexShader;
		PixelShader* _pixelShader;
	};

}
