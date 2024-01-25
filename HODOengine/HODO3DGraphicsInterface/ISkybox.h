#pragma once
#include <string>

namespace HDEngine
{
	class ISkybox
	{
	public:
		virtual ~ISkybox() {}
		virtual void SetCubeMapTexture(const std::string& fileName) = 0;
		virtual void SetActive(bool isActive) = 0;
	};
}