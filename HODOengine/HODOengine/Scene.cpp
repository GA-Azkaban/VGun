#include "Scene.h"
#include "GameObject.h"

namespace hodoEngine
{


	Scene::Scene(std::string sceneName)
	{
		// ���� ������ �� �ݵ�� Directional Light�� Camera�� ������ �־�� �Ѵ�
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
