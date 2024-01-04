#pragma once
#include <vector>

#include "Singleton.h"


namespace RocketCore::Graphics
{
	class Camera;
	class StaticMeshObject;
	class TextRenderer;

	class ObjectManager : public Singleton<ObjectManager>
	{
		friend Singleton;
	private:
		ObjectManager();

	public:
		std::vector<StaticMeshObject*>& GetStaticMeshObjList();
		std::vector<TextRenderer*>& GetTextList();
	public:
		Camera* CreateCamera();
		StaticMeshObject* CreateStaticMeshObject();
		TextRenderer* CreateText();

	private:
		std::vector<Camera*> _cameraList;
		std::vector<StaticMeshObject*> _staticMeshObjectList;
		std::vector<TextRenderer*> _textList;
	};
}
