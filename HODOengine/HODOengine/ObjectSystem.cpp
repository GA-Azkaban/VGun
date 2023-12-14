#include "ObjectSystem.h"
#include "SceneSystem.h"
#include "Scene.h"
#include "GameObject.h"
#include "IDSystem.h"
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

		_staticObjectList.insert(newObject);

		return newObject;
	}

	GameObject* ObjectSystem::CreateObject(Scene* scene, std::string objectName, GameObject* parent)
	{
		GameObject* newObject = new GameObject(objectName);

		if (parent != nullptr)
		{
			newObject->SetParentObject(parent);
		}

		scene->GetGameObjectList().insert(newObject);

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

		_destroyStaticObjectList.insert(gameObject);
	}

	void ObjectSystem::FlushDestroyObjectList()
	{
		for (auto& destroyStaticObj : _destroyStaticObjectList)
		{
			for (auto& component : destroyStaticObj->GetAllComponents())
			{
				component->OnDestroy();
			}
			_staticObjectList.erase(destroyStaticObj);
		}
		_destroyStaticObjectList.clear();

		HDData::Scene* currentScene = HDEngine::SceneSystem::Instance().GetCurrentScene();
		currentScene->FlushDestroyObjectList();
	}

	void ObjectSystem::StartCurrentSceneObjects()
	{
		HDData::Scene* currentScene = HDEngine::SceneSystem::Instance().GetCurrentScene();
		if (currentScene == nullptr)
			return;

		currentScene->Start();
	}

	void ObjectSystem::UpdateCurrentSceneObjects()
	{
		if (!GetStaticObjectList().empty())
		{
			for (const auto& staticObj : GetStaticObjectList())
			{
				for (const auto& comp : staticObj->GetAllComponents())
				{
					comp->Start();
				}
			}

			for (const auto& staticObj : GetStaticObjectList())
			{
				GetRunningStaticObjectList().insert(staticObj);
			}

			GetStaticObjectList().clear();
		}

		// static한 오브젝트들의 업데이트를 돌려준다
		for (const auto& staticObj : GetRunningStaticObjectList())
		{
			for (const auto& comp : staticObj->GetAllComponents())
			{
				comp->Update();
			}
		}

		// 현재 씬의 오브젝트들을 업데이트
		HDData::Scene* currentScene = HDEngine::SceneSystem::Instance().GetCurrentScene();
		if (currentScene == nullptr)
			return;
		currentScene->Update();
	}

	void ObjectSystem::LateUpdateCurrentSceneObjects()
	{
		for (const auto& staticObj : GetRunningStaticObjectList())
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
		for (const auto& staticObj : GetRunningStaticObjectList())
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

	std::unordered_set<HDData::GameObject*>& ObjectSystem::GetStaticObjectList()
	{
		return _staticObjectList;
	}

	std::unordered_set<HDData::GameObject*>& ObjectSystem::GetRunningStaticObjectList()
	{
		return _runningStaticObjectList;
	}

	std::unordered_set<HDData::GameObject*>& ObjectSystem::GetDestroyStaticObjectList()
	{
		return _destroyStaticObjectList;
	}

}
