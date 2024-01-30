#pragma once
#include "../HODO3DGraphicsInterface/ISkybox.h"
#include "RendererBase.h"

namespace HDData
{
	class HODO_API SkyboxRenderer : public RendererBase
	{
	public:
		SkyboxRenderer();
		HDEngine::ISkybox& Get();


		virtual void OnEnable() override;
		virtual void OnDisable() override;

	private:
		HDEngine::ISkybox* _skybox;
	};
}