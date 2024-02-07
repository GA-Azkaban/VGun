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

		// Call the update function of all static objects
		for (const auto& staticObj : _runningStaticObjectList)
		{
			for (const auto& comp : staticObj->GetAllComponents())
			{
				comp->Update();
			}
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
	}

	void ObjectSystem::UpdateDisableList()
	{
		for (auto& obj : _onDisableList)
		{
			obj->FlushDisable();
		}
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
