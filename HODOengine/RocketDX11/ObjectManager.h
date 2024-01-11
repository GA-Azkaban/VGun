#pragma once
#include <vector>

#include "Singleton.h"


namespace RocketCore::Graphics
{
	class Camera;
	class StaticMeshObject;
	class TextRenderer;
	class ImageRenderer;

	class ObjectManager : public Singleton<ObjectManager>
	{
		friend Singleton;
	private:
		ObjectManager();

	public:
		std::vector<StaticMeshObject*>& GetStaticMeshObjList();
		std::vector<TextRenderer*>& GetTextList();
		std::vector<ImageRenderer*>& GetImageList();
	public:
		Camera* CreateCamera();
		StaticMeshObject* CreateStaticMeshObject();
		TextRenderer* CreateText();
		ImageRenderer* CreateImage();

	private:
		std::vector<Camera*> _cameraList;
		std::vector<StaticMeshObject*> _staticMeshObjectList;
		std::vector<TextRenderer*> _textList;
		std::vector<ImageRenderer*> _ImageList;
	};
}
