#include "Scene.h"
#include "GameObject.h"

namespace hodoEngine
{


	Scene::Scene(std::string sceneName)
	{
		// 씬이 생성될 때 반드시 Directional Light와 Camera를 가지고 있어야 한다
	}

	Scene::~Scene()
	{

	}

	void Scene::Start()
	{

	}

	void Scene::Update()
	{

	}

	bool Scene::IsScenePlaying()
	{
		return _isPlaying;
	}

	std::vector<ID>& Scene::GetObjList()
	{
		return _ObjList;
	}

	std::string Scene::GetSceneName()
	{
		return _scneneName;
	}

	void Scene::SetSceneName(std::string sceneName)
	{
		_scneneName = sceneName;
	}

}
