#include "HODO_API.h"

#include "SceneSystem.h"
#include "ObjectSystem.h"

namespace hodoData
{

	HODO_API hodoData::Scene* hodoData::CreateScene(std::string sceneName)
	{
		return hodoEngine::SceneSystem::Instance().CreateScene(sceneName);
	}

	HODO_API GameObject* CreateObject(Scene* now, GameObject* parent /*= nullptr*/)
	{
		return hodoEngine::ObjectSystem::Instance().CreateObject(now, parent);
	}
}

