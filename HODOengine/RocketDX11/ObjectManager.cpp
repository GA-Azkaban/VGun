#include "ObjectManager.h"

#include "..\\HODO3DGraphicsInterface\\IRenderable.h"

#include "Camera.h"
#include "StaticMeshObject.h"

namespace RocketCore::Graphics
{
	ObjectManager::ObjectManager()
	{

	}

	Camera* ObjectManager::CreateCamera()
	{
		Camera* temp = new Camera();
		_cameraList.emplace_back(temp);

		return temp;
	}

	StaticMeshObject* ObjectManager::CreateStaticMeshObject()
	{
		StaticMeshObject* temp = new StaticMeshObject();
		_renderObjectList.emplace_back(temp);

		return temp;
	}
}
