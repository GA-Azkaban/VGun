#include "DebugSystem.h"
#include "GameObject.h"

namespace HDEngine
{


	void DebugSystem::Initialize()
	{

	}

	void DebugSystem::Update()
	{
		if (_isDebugMode)
		{

		}
	}

	void DebugSystem::DrawColliderDebug(HDData::GameObject* obj, HDMath::HDFLOAT4& color)
	{
// 		auto component = obj->AddComponent<HDEngine::DebugCube>();
// 		component->Get()->SetFillModeWireframe();
		//component->Get()->SetColor(color);
	}

	void DebugSystem::SetDebugOn(int index)
	{
		_isDebugMode = true;
		_index = index;
		// TODO) �ε����� ���õ� ��ü�� �������ϵ���
	}

	void DebugSystem::SetDebugOff()
	{
		_isDebugMode = false;
	}

}
