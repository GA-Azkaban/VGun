#include "ObjectSystem.h"
#include "SceneSystem.h"
#include "Scene.h"
#include "GameObject.h"
#include "IDSystem.h"
using namespace HDData;

namespace HDEngine
{

	void ObjectSystem::Start()
	{
		for (const auto& staticObj : GetStaticObjectList())
		{
			for (const auto& comp : staticObj->GetAllComponents())
			{
				if (!comp->GetIsStarted())
				{
					comp->Start();
				}
			}
		}
	}

	void ObjectSystem::Update()
	{
		for (const auto& staticObj : GetStaticObjectList())
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
}
