#include "ParticlePass.h"
#include "QuadBuffer.h"
#include "BillboardObject.h"

namespace RocketCore::Graphics
{

	ParticlePass::ParticlePass(QuadBuffer* toneMapBuffer)
		: _toneMapBuffer(toneMapBuffer)
	{
		_billboard = new BillboardObject();
	}

	ParticlePass::~ParticlePass()
	{
		_toneMapBuffer = nullptr;
	}

	void ParticlePass::Render()
	{
		_toneMapBuffer->SetRenderTargets();

		_billboard->Render();
	}

}
