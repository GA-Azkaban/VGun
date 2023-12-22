#pragma once
#include <vector>

#include "Singleton.h"


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
		std::vector<StaticMeshObject*>& GetStaticMeshObjList();

	public:
		Camera* CreateCamera();
		StaticMeshObject* CreateStaticMeshObject();

	private:
		std::vector<Camera*> _cameraList;
		std::vector<StaticMeshObject*> _staticMeshObjectList;
	};
}
