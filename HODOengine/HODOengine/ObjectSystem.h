#pragma once
#include "Singleton.h"

#include <string>
#include <vector>
#include <set>

namespace HDData
{
	class GameObject;
	class Scene;
}

namespace HDEngine
{
	using ID = std::string;

	class ObjectSystem : public Singleton<ObjectSystem>
	{
		friend Singleton;

	private:
		ObjectSystem() = default;

	public:
		void Update();

		std::vector<HDData::GameObject*> GetStaticObjectList();
		std::vector<HDData::GameObject*> GetRunningStaticObjectList();
		HDData::GameObject* CreateStaticObject(std::string objectName = "", HDData::GameObject* parent = nullptr);
		HDData::GameObject* CreateObject(HDData::Scene* scene, std::string objectName = "", HDData::GameObject * parent = nullptr);
		void DestroyObject(HDData::Scene* scene, HDData::GameObject* gameObject);

	private:
		std::vector<HDData::GameObject*> _staticObjectList;
		std::vector<HDData::GameObject*> _runningStaticObjectList;
	};

}


