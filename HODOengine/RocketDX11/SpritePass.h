#pragma once
#include <DXTK/SpriteBatch.h>
#include <DXTK/CommonStates.h>

namespace RocketCore::Graphics
{
	class QuadBuffer;

	class SpritePass
	{
	public:
		SpritePass(QuadBuffer* toneMapBuffer);
		~SpritePass();
		void Render();

	private:
		void RenderImage();
		void RenderText();

	private:
		DirectX::SpriteBatch* _spriteBatch;
		DirectX::CommonStates* _states;
		QuadBuffer* _toneMapBuffer;
	};
}
