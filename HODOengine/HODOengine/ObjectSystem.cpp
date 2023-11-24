#include "ObjectSystem.h"
#include "SceneSystem.h"
#include "Scene.h"
#include "GameObject.h"
#include "IDSystem.h"


namespace hodoEngine
{


	void ObjectSystem::Initialize()
	{

	}

	void ObjectSystem::Update()
	{
		for (const auto& obj : _runningObjectList)
		{
			/*for (const auto& comp : )
			{
				
			}*/
		}
	}

	void ObjectSystem::LateUpdate()
	{
		// 여기서 현재 씬이 변경되어 활성화된 오브젝트가 달라졌는지 체크한다
		//if (SceneSystem::Instance().GetIsCurrentSceneChange())
		//{
		//	for (auto& nowobj : _runningObjectList)
		//	{
		//		nowobj->SetSelfActive(false);
		//	}
		//
		//	_runningObjectList.clear();
		//
		//	auto objs = SceneSystem::Instance().GetCurrentScene()->GetObjList();
		//
		//	for (const auto& obj : objs)
		//	{
		//		auto one = _allObjectList.find(obj)->second;
		//		one->SetSelfActive(true);
		//		_runningObjectList.insert(one);
		//	}
		//}	
	}

	hodoEngine::GameObject* ObjectSystem::CreateObject(Scene* scene, std::string objectName, GameObject* parent)
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
