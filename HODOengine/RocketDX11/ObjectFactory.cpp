#include "ObjectFactory.h"
#include "ObjectManager.h"
#include "ResourceManager.h"

#include "Camera.h"
#include "StaticMeshObject.h"
#include "CubeMesh.h"

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
		ObjectManager& objMgr = ObjectManager::Instance();
		ResourceManager& rscMgr = ResourceManager::Instance();

		StaticMeshObject* obj = objMgr.CreateStaticMeshObject();

		obj->SetMesh(rscMgr.GetCubeMesh());
		obj->SetVertexShader(rscMgr.GetDefaultVertexShader());
		obj->SetPixelShader(rscMgr.GetDefaultPixelShader());

		return obj;
	}

	HDEngine::ISketchableText* ObjectFactory::CreateText()
	{
		return nullptr;
	}

}
