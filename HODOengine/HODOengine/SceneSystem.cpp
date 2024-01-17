#include "SceneSystem.h"
#include "UISystem.h"
#include "GameObject.h"
#include "Scene.h"

#include <cassert>

#include "IDSystem.h"


// TODO) ���ӿ�����Ʈ ������ �ٲ�� ������� ������ �ʿ��� ��

namespace HDEngine
{

	SceneSystem::SceneSystem()
		:_currentScene(nullptr)
	{

	}

	HDData::Scene* SceneSystem::CreateScene(std::string sceneName)
	{
		// �̹� �ִ� �� �̸��̶�� �� ���� ��ȯ���ش�.
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
		// ���ο� ���� ã�´�
		auto sceneIter = _sceneList.find(sceneName);
		if (sceneIter == _sceneList.end())
		{
			return;
		}
		_currentScene = sceneIter->second;

		UISystem::Instance().SetChangedScene(_currentScene);
	}

	void SceneSystem::LoadScene(HDData::Scene* scene)
	{
		_currentScene = scene;
		UISystem::Instance().SetChangedScene(_currentScene);
	}

	std::unordered_map<std::string, HDData::Scene*>& SceneSystem::GetAllScenes()
	{
		return _sceneList;
	}

	HDData::Scene* SceneSystem::GetCurrentScene()
	{
		return _currentScene;
	}
}
