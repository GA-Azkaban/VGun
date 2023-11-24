#pragma once
#include "Singleton.h"

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <set>

namespace hodoData
{
	class GameObject;
	class Scene;
}

namespace hodoEngine
{
	using ID = std::string;

	class ObjectSystem : public Singleton<ObjectSystem>
	{
		friend Singleton;
		friend class HODOengine;

	private:
		ObjectSystem() = default;

	public:
		hodoData::GameObject* CreateObject(hodoData::Scene* scene, std::string objectName = "", hodoData::GameObject * parent = nullptr);
		void DestroyObject(hodoData::Scene* scene, hodoData::GameObject* gameObject);

	private:
		//std::unordered_set<hodoData::GameObject*> _runningObjectList;
		//std::unordered_map<ID, hodoData::GameObject*> _allObjectList;
	};

}


