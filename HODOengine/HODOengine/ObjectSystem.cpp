#include "ObjectSystem.h"
#include "SceneSystem.h"
#include "Scene.h"
#include "GameObject.h"
#include "IDSystem.h"
using namespace hodoData;

namespace hodoEngine
{
	GameObject* ObjectSystem::CreateObject(Scene* scene, std::string objectName, GameObject* parent)
	{
		GameObject* newObject = new GameObject(objectName);
		//ID id = IDSystem::Instance().CreateID();

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