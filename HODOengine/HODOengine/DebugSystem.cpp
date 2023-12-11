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
		}
	}

	void DebugSystem::DrawColliderDebug(hodoData::GameObject* obj, HDMaths::HDFLOAT4& color)
	{
		auto component = obj->AddComponent<hodoEngine::DebugCube>();
		component->Get()->SetFillModeWireframe();
		component->Get()->SetColor(color);
	}

	void DebugSystem::SetDebugOn(int index)
	{
		_isDebugMode = true;
		_index = index;
		// TODO) 인덱스가 일치하는 오브젝트만 렌더링되도록.......
	}

	void DebugSystem::SetDebugOff()
	{
		_isDebugMode = false;
	}

}
