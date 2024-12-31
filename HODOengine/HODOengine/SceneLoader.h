#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include "Singleton.h"
#include "MathHeader.h"
#include "MaterialManager.h"

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
		std::unordered_map<int, Vector3>& GetRespawnPoint();
		Vector3* GetCloudPoint();

	private:
		void LoadFromJson(std::string filePath);
		void CreateObject(HDData::Scene* scene);
		void LinkHierachy();
		void SetTransform();

		// 스폰 지점
		//Vector3 _spawnPoint[20];
		std::unordered_map<int, Vector3> _spawnPoint;
		int _spawnIndex = 1;

		// 구름 위치 지정
		Vector3 _cloudPoint[60];
		int _cloudIndex = 0;
	private:
		HDData::Scene * now;
		HDData::Material* _material;
		HDData::Material* _plane;
		std::vector<InfoStruct> _infoList;
		std::unordered_map<int, HDData::GameObject*> _gameObjectMap;
	};
}
