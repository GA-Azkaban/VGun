#pragma once
#include <Vector>

#include "Singleton.h"

namespace RocketCore::Graphics
{
	class Camera;

	class ResourceManager : public Singleton<ResourceManager>
	{
		friend Singleton;
	private:
		ResourceManager();

	public:
		Camera* CreateCamera();

	private:
		std::vector<Camera*> _cameraList;
		// Camera* _mainCamera;
	};
}
