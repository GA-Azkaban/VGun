#pragma once
#include "Singleton.h"

#include <string>
#include <vector>

namespace HDData
{
	class GameObject;
	class Scene;
	class Component;
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
		void CreateStaticComponent(HDData::Component* component);
		void DestroyObject(HDData::Scene* scene, HDData::GameObject* gameObject);
		void DestroyStaticObject(HDData::GameObject* gameObject);

		void FlushDestroyObjectList();

		void UpdateCurrentSceneObjects();
		void LateUpdateCurrentSceneObjects();
		void FixedUpdateCurrentSceneObjects();

		std::vector<HDData::GameObject*>& GetStaticObjectList();
		std::vector<HDData::GameObject*>& GetRunningStaticObjectList();
		std::vector<HDData::GameObject*>& GetDestroyStaticObjectList();

	private:
		std::vector<HDData::GameObject*> _destroyStaticObjectList;
		std::vector<HDData::GameObject*> _staticObjectList;
		std::vector<HDData::GameObject*> _runningStaticObjectList;
		std::vector<HDData::Component*> _staticComponentList;
		std::vector<HDData::Component*> _runningStaticComponentList;

	public:
		void UpdateEnableList();
		void UpdateDisableList();
		void AddOnEnableList(HDData::GameObject* obj);
		void AddOnDisableList(HDData::GameObject* obj);

	private:
		std::vector<HDData::GameObject*> _onEnableList;
		std::vector<HDData::GameObject*> _onDisableList;
	};

}


