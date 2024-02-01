#include "SpritePass.h"
#include "QuadBuffer.h"

namespace RocketCore::Graphics
{

	SpritePass::SpritePass(QuadBuffer* quadBuffer)
		: _quadBuffer(quadBuffer)
	{

	}

	void SpritePass::Render()
	{
		_quadBuffer->SetRenderTargets();


	}

}
