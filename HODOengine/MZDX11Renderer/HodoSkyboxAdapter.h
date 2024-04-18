#pragma once
#include "../HODO3DGraphicsInterface/ICubeMap.h"
#include "Skybox.h"
#include <memory>

namespace hodoGIAdapter 
{
	class SkyboxAdapter : public HDEngine::ICubeMap
	{
	public:
		SkyboxAdapter(Skybox* skybox);
		~SkyboxAdapter();

		virtual void LoadCubeMapTexture(const std::string& fileName) override;
		virtual void SetActive(bool isActive) override;

	private:
		std::unique_ptr<Skybox> _skybox;
	};
}
