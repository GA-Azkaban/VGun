#include "ObjectFactory.h"
#include "ObjectManager.h"
#include "ResourceManager.h"

#include "Camera.h"
#include "StaticMeshObject.h"
#include "CubeMesh.h"
#include "TextRenderer.h"
#include "ImageRenderer.h"
#include "LineRenderer.h"

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

		obj->SetModel(rscMgr.GetCubeModel());
		obj->SetVertexShader(rscMgr.GetVertexShader("TextureVS"));
		obj->SetPixelShader(rscMgr.GetPixelShader("TexturePS"));
		obj->SetRenderState(rscMgr.GetRenderState(ResourceManager::eRenderState::SOLID));

		return obj;
	}

	HDEngine::ISketchableText* ObjectFactory::CreateText()
	{
		return ObjectManager::Instance().CreateText();
	}

	HDEngine::ISketchableImage* ObjectFactory::CreateImage()
	{
		return ObjectManager::Instance().CreateImage();
	}

	HDEngine::ILineRenderer* ObjectFactory::CreateLineRenderer()
	{
		return ObjectManager::Instance().CreateLineRenderer();
	}

}
