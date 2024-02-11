#pragma once
namespace RocketCore::Graphics
{
	class QuadBuffer;
	class VertexShader;
	class PixelShader;

	class ToneMapPass
	{
	public:
		ToneMapPass(QuadBuffer* quadBuffer, QuadBuffer* toneMapBuffer);

		void Render();

	private:
		QuadBuffer* _quadBuffer;
		QuadBuffer* _toneMapBuffer;

		VertexShader* _vertexShader;
		PixelShader* _pixelShader;
	};

}
