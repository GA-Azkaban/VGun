#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include "MathHeader.h"

namespace HDData
{
	class GameObject;
}

namespace HDEngine
{
	class SceneLoader
	{
		struct InfoStruct
		{
			int id;
			int parentID;
			std::string name;
			std::string meshName;
			Vector3 position;
			Quaternion rotation;
			Vector3 scale;
			int colliderType;		// box = 1, sphere = 2
			Vector3 colliderCenter;
			Vector3 boxColliderSize;
			float sphereColliderRadius;
		};

	public:
		SceneLoader();

		void LoadUnityScene(std::string filePath);

	private:
		void LoadFromJson(std::string filePath);
		void CreateObject();
		void LinkHierachy();

	private:
		std::vector<InfoStruct> _infoList;
		std::vector<HDData::GameObject*> _gameObjectList;
		std::unordered_map<int, HDData::GameObject*> _gameObjectMap;
	};
}
