#include "ObjectSystem.h"
#include "SceneSystem.h"
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
			// 여기서 활성화된 오브젝트가 업데이트 된다
		}
	}

	void ObjectSystem::LateUpdate()
	{
		// 여기서 현재 씬이 변경되어 활성화된 오브젝트가 달라졌는지 체크한다
		if (SceneSystem::Instance().GetIsCurrentSceneChange())
		{
			for (auto& nowobj : _runningObjectList)
			{
				nowobj->SetSelfActive(false);
			}

			_runningObjectList.clear();

			auto objs = SceneSystem::Instance().GetCurrentScene()->GetObjList();

			for (const auto& obj : objs)
			{
				auto one = _allObjectList.find(obj)->second;
				one->SetSelfActive(true);
				_runningObjectList.push_back(one);
			}
		}
		
	}

	hodoEngine::GameObject* ObjectSystem::CreateObject()
	{
		GameObject* obj = new GameObject();
		ID id = IDSystem::Instance().CreateID();

		_allObjectList.insert({id, obj});

		return obj;
	}

	hodoEngine::GameObject* ObjectSystem::CreateObject(std::string objName)
	{
		GameObject* obj = new GameObject(objName);
		ID id = IDSystem::Instance().CreateID();

		_allObjectList.insert({ id, obj });

		return obj;
	}

}
