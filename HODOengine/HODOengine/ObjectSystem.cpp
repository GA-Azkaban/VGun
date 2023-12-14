#include "ObjectSystem.h"
#include "SceneSystem.h"
#include "Scene.h"
#include "GameObject.h"
#include "IDSystem.h"
using namespace HDData;

namespace HDEngine
{
	void ObjectSystem::Update()
	{
		// 중간에 static 오브젝트가 추가된다면.... 
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
				GetRunningStaticObjectList().push_back(staticObj);
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
	}

	std::vector<HDData::GameObject*> ObjectSystem::GetStaticObjectList()
{
		return _staticObjectList;
	}

	std::vector<HDData::GameObject*> ObjectSystem::GetRunningStaticObjectList()
	{
		return _runningStaticObjectList;
	}

	HDData::GameObject* ObjectSystem::CreateStaticObject(std::string objectName /*= ""*/, HDData::GameObject* parent /*= nullptr*/)
	{
		GameObject* newObject = new GameObject(objectName);

		if (parent != nullptr)
		{
			newObject->SetParentObject(parent);
		}
		
		GetStaticObjectList().push_back(newObject);

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

	HDData::GameObject* ObjectSystem::CreateStaticObject(std::string objectName /*= ""*/)
	{
		GameObject* newObject = new GameObject(objectName);
		_staticObjectList.insert(newObject);
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

		for (auto& staticObject : _staticObjectList)
		{
			staticObject->Start();
		}

		currentScene->Start();
	}

	void ObjectSystem::UpdateCurrentSceneObjects()
	{

	}

	void ObjectSystem::LateUpdateCurrentSceneObjects()
	{

	}

	void ObjectSystem::FixedUpdateCurrentSceneObjects()
	{

	}

	std::unordered_set<HDData::GameObject*>& ObjectSystem::GetStaticObjectList()
	{
		return _staticObjectList;
	}

	std::unordered_set<HDData::GameObject*>& ObjectSystem::GetDestroyStaticObjectList()
	{
		return _destroyStaticObjectList;
	}

}
