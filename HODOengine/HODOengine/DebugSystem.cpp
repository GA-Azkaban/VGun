#include "DebugSystem.h"
#include "HodoDebugCube.h"
#include "GameObject.h"

namespace hodoEngine
{


	void DebugSystem::Initialize()
	{

	}

	void DebugSystem::Update()
	{
		if (_isDebugMode)
		{
			int test = 0;
		}
	}

	void DebugSystem::DrawColliderDebug(hodoData::GameObject* obj, HDMaths::HDFLOAT4& color)
	{
		auto component = obj->AddComponent<hodoEngine::DebugCube>();
		component->Get()->SetFillModeWireframe();
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
