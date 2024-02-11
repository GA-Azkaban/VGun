#pragma once
#include <DXTK/SpriteBatch.h>

namespace RocketCore::Graphics
{
	class QuadBuffer;

	class SpritePass
	{
	public:
		SpritePass(QuadBuffer* toneMapBuffer);

		void Render();

	private:
		void RenderImage();
		void RenderText();

	private:
		DirectX::SpriteBatch* _spriteBatch;
		QuadBuffer* _toneMapBuffer;
	};
}
