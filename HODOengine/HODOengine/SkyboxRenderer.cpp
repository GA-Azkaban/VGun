#include "SkyboxRenderer.h"
#include "GraphicsObjFactory.h"

namespace HDData
{

	SkyboxRenderer::SkyboxRenderer()
		: _skybox(HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateSkybox())
	{

	}

	HDEngine::ISkybox& SkyboxRenderer::Get()
	{
		return *_skybox;
	}

	void SkyboxRenderer::OnEnable()
	{
		_skybox->SetActive(true);
	}

	void SkyboxRenderer::OnDisable()
	{
		_skybox->SetActive(false);
	}

}