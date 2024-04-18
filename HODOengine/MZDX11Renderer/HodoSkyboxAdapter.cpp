#include "HodoSkyboxAdapter.h"

namespace hodoGIAdapter
{

	SkyboxAdapter::SkyboxAdapter(Skybox* skybox)
		: _skybox(skybox)
	{

	}

	SkyboxAdapter::~SkyboxAdapter()
	{
		_skybox.release();
	}

	void SkyboxAdapter::LoadCubeMapTexture(const std::string& fileName)
	{
		_skybox->SetCubeMapTexture(fileName);
	}

	void SkyboxAdapter::SetActive(bool isActive)
	{
		_skybox->SetActive(isActive);
	}

}
