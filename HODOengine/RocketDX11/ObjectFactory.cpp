#include "ObjectFactory.h"
#include "ObjectManager.h"
#include "Camera.h"
#include "StaticMeshObject.h"

namespace HDEngine
{
	IFactory* CreateGraphicsObjectFactory()
	{
		return new RocketCore::Graphics::ObjectFactory();
	}

	void ReleaseFactory(IFactory* instance)
	{
		delete instance;
	}
}

namespace RocketCore::Graphics
{
	HDEngine::ICamera* ObjectFactory::CreateCamera()
	{
		return ObjectManager::Instance().CreateCamera();
	}

	HDEngine::IStaticMesh* ObjectFactory::CreateStaticMeshObject()
	{
		return ObjectManager::Instance().CreateStaticMeshObject();
	}

}
