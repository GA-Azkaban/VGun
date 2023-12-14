#include "DebugSystem.h"
//#include "HodoDebugCube.h"
#include "GameObject.h"

// TODO) 디버그 큐브 갈아끼우기

namespace HDEngine
{


	void DebugSystem::Initialize()
	{

	}

	void DebugSystem::Update()
	{
		// 콜라이더 데이터 출력
		for (const auto& colli : _colliderDebugData)
		{
			if (colli->flag == _index)
			{
				DrawColliderDebug(colli->obj, colli->color);
			}
		}
	}

	void DebugSystem::AddDebugData(int flag = 0, HDData::GameObject* obj = nullptr, HDMath::HDFLOAT4 color = HDMath::HDFLOAT4{ 1.f, 0.f, 0.f, 0.f })
	{
		debugData* one = new debugData();
		one->flag = flag;
		one->color = color;
		one->obj = obj;
		_colliderDebugData.push_back(one);
	}

	void DebugSystem::DrawColliderDebug(HDData::GameObject* obj, HDMath::HDFLOAT4 color)
	{
		/*auto component = obj->AddComponent<HDEngine::DebugCube>();
		component->Get()->SetFillModeWireframe();
		component->Get()->SetColor(color);*/
	}

	void DebugSystem::SetDebugOn(int index)
	{
		_index = index;
	}

	void DebugSystem::SetDebugOff()
	{
		_index = -1;
	}

}
