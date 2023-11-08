#include "HODO_API.h"


namespace hodoEngine
{

	HODO_API Scene* hodoEngine::CreateScene(std::string sceneName)
	{
		return SceneSystem::Instance().CreateScene(sceneName);
	}
}

