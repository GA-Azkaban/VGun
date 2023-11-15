#include "HODO_API.h"

#include "SceneSystem.h"
#include "ObjectSystem.h"

namespace hodoEngine
{

	HODO_API Scene* hodoEngine::CreateScene(std::string sceneName)
	{
		return SceneSystem::Instance().CreateScene(sceneName);
	}

	HODO_API Scene* GetCurrentScene()
	{
		return SceneSystem::Instance().GetCurrentScene();
	}

	HODO_API GameObject* CreateObject(Scene* now, GameObject* parent /*= nullptr*/)
	{
		return ObjectSystem::Instance().CreateObject(now, parent);
	}

}

