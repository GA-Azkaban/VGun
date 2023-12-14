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
		// 매니저 뒤에 objectsystem을 한번 돌려주고...
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
