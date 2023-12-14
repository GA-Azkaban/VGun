#pragma once
#include "Singleton.h"

#include <string>
#include <unordered_set>

namespace HDData
{
	class GameObject;
	class Scene;
}

namespace HDEngine
{
	class ObjectSystem : public Singleton<ObjectSystem>
	{
		friend Singleton;

	private:
		ObjectSystem() = default;

	public:
		HDData::GameObject* CreateObject(HDData::Scene* scene, std::string objectName = "", HDData::GameObject* parent = nullptr);
		HDData::GameObject* CreateStaticObject(std::string objectName = "", HDData::GameObject* parent = nullptr);
		void DestroyObject(HDData::Scene* scene, HDData::GameObject* gameObject);
		void DestroyStaticObject(HDData::GameObject* gameObject);

		void FlushDestroyObjectList();

		void StartCurrentSceneObjects();
		void UpdateCurrentSceneObjects();
		void LateUpdateCurrentSceneObjects();
		void FixedUpdateCurrentSceneObjects();

		std::unordered_set<HDData::GameObject*>& GetStaticObjectList();
		std::unordered_set<HDData::GameObject*>& GetRunningStaticObjectList();
		std::unordered_set<HDData::GameObject*>& GetDestroyStaticObjectList();

	private:
		std::unordered_set<HDData::GameObject*> _destroyStaticObjectList;
		std::unordered_set<HDData::GameObject*> _staticObjectList;
		std::unordered_set<HDData::GameObject*> _runningStaticObjectList;
	};

}


