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
			/*for (const auto& comp : )
			{
				
			}*/
		}
	}

	void ObjectSystem::LateUpdate()
	{
		// ���⼭ ���� ���� ����Ǿ� Ȱ��ȭ�� ������Ʈ�� �޶������� üũ�Ѵ�
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

	hodoData::GameObject* ObjectSystem::CreateObject(hodoData::Scene* now, hodoData::GameObject* parent)
	{
		hodoData::GameObject* obj = new hodoData::GameObject();
		ID id = IDSystem::Instance().CreateID();

		if (parent != nullptr)
		{
			obj->SetParentObject(parent);
		}

		_allObjectList.insert({ id, obj });

		return obj;
	}

}
