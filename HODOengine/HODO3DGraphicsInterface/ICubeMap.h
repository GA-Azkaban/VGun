#pragma once
#include <string>

namespace HDEngine
{
	class ICubeMap
	{
	public:
		virtual ~ICubeMap() {}
		virtual void LoadCubeMapTexture(const std::string& fileName) = 0;
		virtual void SetActive(bool isActive) = 0;
		virtual void SetEnvLightIntensity(float value) = 0;
	};
}
