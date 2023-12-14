#include "SceneSystem.h"
#include "GameObject.h"
#include "Scene.h"

#include <cassert>

#include "IDSystem.h"


// TODO) 게임오브젝트 구조가 바뀌면 대대적인 수정이 필요할 듯

namespace HDEngine
{

	SceneSystem::SceneSystem()
		:_currentScene(nullptr)
	{

	}

	HDData::Scene* SceneSystem::CreateScene(std::string sceneName)
	{
		// 이미 있는 씬 이름이라면 그 씬을 반환해준다.
		auto iter = _sceneList.find(sceneName);
		if (iter != _sceneList.end())
		{
			return _sceneList[sceneName];
		}

		HDData::Scene* scene = new HDData::Scene(sceneName);
		_sceneList.insert({ sceneName, scene });
		return scene;
	}

	void SceneSystem::LoadScene(std::string sceneName)
	{
		// 새로운 씬을 찾는다
		auto sceneIter = _sceneList.find(sceneName);
		if (sceneIter == _sceneList.end())
		{
			return;
		}
		_currentScene = sceneIter->second;
	}

	void SceneSystem::LoadScene(HDData::Scene* scene)
	{
		_currentScene = scene;
	}

	HDData::Scene* SceneSystem::GetCurrentScene()
	{
		return _currentScene;
	}
}
