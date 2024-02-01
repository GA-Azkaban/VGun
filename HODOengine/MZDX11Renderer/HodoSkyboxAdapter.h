#pragma once
#include "../HODO3DGraphicsInterface/ISkybox.h"
#include "Skybox.h"
#include <memory>

namespace hodoGIAdapter 
{
	class SkyboxAdapter : public HDEngine::ISkybox
	{
	public:
		SkyboxAdapter(Skybox* skybox);
		~SkyboxAdapter();

		virtual void SetCubeMapTexture(const std::string& fileName) override;
		virtual void SetActive(bool isActive) override;

	private:
		std::unique_ptr<Skybox> _skybox;
	};
}