#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include "Singleton.h"
#include "MathHeader.h"

namespace HDData
{
	class GameObject;
	class Scene;
}

namespace HDEngine
{
	class SceneLoader : public Singleton<SceneLoader>
	{
		friend Singleton;
	private:
		SceneLoader();

		struct InfoStruct
		{
			int id;
			int parentID;
			std::string name;
			std::string meshName;
			Vector3 position;
			Quaternion rotation;
			Vector3 scale;
			int colliderType;		// box static = 1, sphere dynamic = 2
			Vector3 colliderCenter;
			Vector3 boxColliderSize;
			float sphereColliderRadius;
			std::vector<std::string> materials;
		};

	public:
		void LoadUnityScene(std::string fileName, HDData::Scene* scene);

	private:
		void LoadFromJson(std::string filePath);
		void CreateObject(HDData::Scene* scene);
		void LinkHierachy();
		void SetTransform();

	private:
		HDData::Scene * now;
		std::vector<InfoStruct> _infoList;
		std::unordered_map<int, HDData::GameObject*> _gameObjectMap;
	};
}
