#include "ObjectSystem.h"
#include "SceneSystem.h"
#include "Scene.h"
#include "GameObject.h"
#include <algorithm>
using namespace HDData;

namespace HDEngine
{

	HDData::GameObject* ObjectSystem::CreateStaticObject(std::string objectName /*= ""*/, HDData::GameObject* parent /*= nullptr*/)
	{
		GameObject* newObject = new GameObject(objectName);

		if (parent != nullptr)
		{
			newObject->SetParentObject(parent);
		}

		_staticObjectList.push_back(newObject);

		return newObject;
	}

	void ObjectSystem::CreateStaticComponent(HDData::Component* component)
	{
		for (auto& one : _staticComponentList)
		{
			if (one == component) return;
		}

		_staticComponentList.push_back(component);
	}

	GameObject* ObjectSystem::CreateObject(Scene* scene, std::string objectName, GameObject* parent)
	{
		GameObject* newObject = new GameObject(objectName);

		if (parent != nullptr)
		{
			newObject->SetParentObject(parent);
		}

		scene->GetGameObjectList().push_back(newObject);

		return newObject;
	}

	void ObjectSystem::DestroyObject(Scene* scene, GameObject* gameObject)
	{
		if (!gameObject)
			return;

		scene->GetDestroyObjectList().push_back(gameObject);
		for (auto child : gameObject->GetChildGameObjects())
		{
			DestroyObject(scene, child);
		}
	}

	void ObjectSystem::DestroyStaticObject(HDData::GameObject* gameObject)
	{
		if (!gameObject)
			return;

		_destroyStaticObjectList.push_back(gameObject);
	}

	void ObjectSystem::FlushDestroyObjectList()
	{
		for (auto& destroyStaticObj : _destroyStaticObjectList)
		{
			for (auto& component : destroyStaticObj->GetAllComponents())
			{
				component->OnDestroy();
			}
			_staticObjectList.erase(std::remove_if(_destroyStaticObjectList.begin(), _destroyStaticObjectList.end(), [&](HDData::GameObject* gameObject) { return gameObject == destroyStaticObj; }));
		}
		_destroyStaticObjectList.clear();

		HDData::Scene* currentScene = HDEngine::SceneSystem::Instance().GetCurrentScene();
		currentScene->FlushDestroyObjectList();
	}

	void ObjectSystem::UpdateCurrentSceneObjects()
	{
		// Call the start function of the static objects that the update function was first called
		if (!_staticObjectList.empty())
		{
			for (auto staticobj = _staticObjectList.begin(); staticobj != _staticObjectList.end();)
			{
				if ((*staticobj)->IsActive())
				{
					(*staticobj)->Start();
					_runningStaticObjectList.push_back(*staticobj);
					staticobj = _staticObjectList.erase(staticobj);
				}
				else
				{
					++staticobj;
				}
			}

			for (const auto& staticObj : _staticObjectList)
			{
				for (const auto& comp : staticObj->GetAllComponents())
				{
					comp->Start();
				}
				_runningStaticObjectList.push_back(staticObj);
			}
			_staticObjectList.clear();
		}

		if (!_staticComponentList.empty())
		{
			for (const auto& staticComp : _staticComponentList)
			{
				staticComp->Start();
				_runningStaticComponentList.push_back(staticComp);
			}
			_staticComponentList.clear();
		}

		// Call the update function of all static objects
		for (const auto& staticObj : _runningStaticObjectList)
		{
			for (const auto& comp : staticObj->GetAllComponents())
			{
				comp->Update();
			}
		}

		for (const auto& staticComp : _runningStaticComponentList)
		{
			staticComp->Update();
		}

		// Call the update function of all gameobjects
		HDData::Scene* currentScene = HDEngine::SceneSystem::Instance().GetCurrentScene();
		if (currentScene == nullptr)
			return;
		currentScene->Update();
	}

	void ObjectSystem::LateUpdateCurrentSceneObjects()
	{
		for (const auto& staticObj : _runningStaticObjectList)
		{
			for (const auto& comp : staticObj->GetAllComponents())
			{
				comp->LateUpdate();
			}
		}

		for (const auto& staticComp : _runningStaticComponentList)
		{
			staticComp->LateUpdate();
		}

		HDData::Scene* currentScene = HDEngine::SceneSystem::Instance().GetCurrentScene();
		if (currentScene == nullptr)
			return;

		currentScene->LateUpdate();
	}

	void ObjectSystem::FixedUpdateCurrentSceneObjects()
	{
		for (const auto& staticObj : _runningStaticObjectList)
		{
			for (const auto& comp : staticObj->GetAllComponents())
			{
				comp->FixedUpdate();
			}
		}

		for (const auto& staticComp : _runningStaticComponentList)
		{
			staticComp->FixedUpdate();
		}

		HDData::Scene* currentScene = HDEngine::SceneSystem::Instance().GetCurrentScene();
		if (currentScene == nullptr)
			return;

		currentScene->FixedUpdate();
	}

	std::vector<HDData::GameObject*>& ObjectSystem::GetStaticObjectList()
	{
		return _staticObjectList;
	}

	std::vector<HDData::GameObject*>& ObjectSystem::GetRunningStaticObjectList()
	{
		return _runningStaticObjectList;
	}

	std::vector<HDData::GameObject*>& ObjectSystem::GetDestroyStaticObjectList()
	{
		return _destroyStaticObjectList;
	}

	void ObjectSystem::UpdateEnableList()
	{
		for (auto& obj : _onEnableList)
		{
			obj->FlushEnable();
		}

		_onEnableList.clear();
	}

	void ObjectSystem::UpdateDisableList()
	{
		for (auto& obj : _onDisableList)
		{
			obj->FlushDisable();
		}

		_onDisableList.clear();
	}

	void ObjectSystem::AddOnEnableList(HDData::GameObject* obj)
	{
		_onEnableList.push_back(obj);
	}

	void ObjectSystem::AddOnDisableList(HDData::GameObject* obj)
	{
		_onDisableList.push_back(obj);
	}

}
