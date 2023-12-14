#pragma once
#include "Singleton.h"

#include <string>
#include <unordered_map>
#include <unordered_set>
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
		friend class HODOengine;

	private:
		ObjectSystem() = default;

	public:
		void Start();
		void Update();
		HDData::GameObject* CreateObject(HDData::Scene* scene, std::string objectName = "", HDData::GameObject * parent = nullptr);
		void DestroyObject(HDData::Scene* scene, HDData::GameObject* gameObject);

	private:
		//std::unordered_set<HDData::GameObject*> _runningObjectList;
		//std::unordered_map<ID, HDData::GameObject*> _allObjectList;
	};

}


