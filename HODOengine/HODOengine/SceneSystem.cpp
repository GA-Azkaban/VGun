#include "SceneSystem.h"

namespace hodoEngine
{
	hodoEngine::Scene* SceneSystem::CreateScene(std::string sceneName)
	{
		if (_sceneList.find(sceneName) == _sceneList.end()) 
		{
			_sceneList[sceneName] = new Scene(sceneName);
			return _sceneList[sceneName];
		}
		return _sceneList[sceneName];
	}
}
