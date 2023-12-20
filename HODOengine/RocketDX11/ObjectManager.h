#pragma once
#include <vector>
#include "Singleton.h"

namespace HDEngine
{
	class IRenderable;
}

namespace RocketCore::Graphics
{
	class Camera;
	class StaticMeshObject;

	class ObjectManager : public Singleton<ObjectManager>
	{
		friend Singleton;
	private:
		ObjectManager();

	public:
		Camera* CreateCamera();
		StaticMeshObject* CreateStaticMeshObject();

	private:
		std::vector<Camera*> _cameraList;
		std::vector<HDEngine::IRenderable*> _renderObjectList;
	};
}
