#pragma once

namespace RocketCore::Graphics
{
	class QuadBuffer;

	class SpritePass
	{
	public:
		SpritePass(QuadBuffer* quadBuffer);

		void Render();

	private:
		QuadBuffer* _quadBuffer;
	};
}
