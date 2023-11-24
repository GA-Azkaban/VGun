#pragma once
#include "Singleton.h"

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <set>

namespace hodoEngine
{
	class GameObject;
	class Scene;

	using ID = std::string;

	class ObjectSystem : public Singleton<ObjectSystem>
	{
		friend Singleton;
		friend class HODOengine;

	private:
		ObjectSystem() = default;

	public:
		void Initialize();
		void Update();
		void LateUpdate();
		void FlushEnable();

	public:
		GameObject* CreateObject(Scene* scene, std::string objectName = "", GameObject * parent = nullptr);
		void DestroyObject(Scene* scene, GameObject* gameObject);

	private:
		std::unordered_set<GameObject*> _runningObjectList;
		std::unordered_map<ID, GameObject*> _allObjectList;
	};

}


