#include "ResourceManager.h"
#include "Camera.h"

namespace RocketCore::Graphics
{
	ResourceManager::ResourceManager()
	{

	}

	Camera* ResourceManager::CreateCamera()
	{
		Camera* temp = new Camera();
		_cameraList.emplace_back(temp);
		
// 		if (!_mainCamera)
// 		{
// 			_mainCamera = temp;
// 		}

		return temp;
	}

}
