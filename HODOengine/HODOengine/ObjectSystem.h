#pragma once
#include "Singleton.h"

#include <string>
#include <unordered_map>

namespace hodoData
{
	class GameObject;
	class Scene;
}

namespace hodoEngine
{
	using ID = std::string;
	using hodoData::GameObject;


	class ObjectSystem : public Singleton<ObjectSystem>
	{
		friend Singleton;

	private:
		ObjectSystem() = default;

	public:
		void Initialize();
		void Update();
		void LateUpdate();
		void FlushEnable();

	public:
		GameObject* CreateObject(hodoData::Scene* now, GameObject* parent = nullptr);

	private:
		std::vector<GameObject*> _runningObjectList;
		std::unordered_map<ID, GameObject*> _allObjectList;

	};

}


