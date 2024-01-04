#include "ObjectManager.h"

#include "..\\HODO3DGraphicsInterface\\IRenderable.h"

#include "Camera.h"
#include "StaticMeshObject.h"
#include "TextRenderer.h"

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
		_staticMeshObjectList.emplace_back(temp);

		return temp;
	}

	std::vector<StaticMeshObject*>& ObjectManager::GetStaticMeshObjList()
	{
		return _staticMeshObjectList;
	}

	RocketCore::Graphics::TextRenderer* ObjectManager::CreateText()
	{
		TextRenderer* TextObject = new TextRenderer();
		_textList.emplace_back(TextObject);
		return TextObject;
	}

	std::vector<TextRenderer*>& ObjectManager::GetTextList()
	{
		return _textList;
	}

}
