#pragma once

namespace hodoEngine
{
	class Scene;
	class SceneSystem;
}

class TestScene
{
public:
	TestScene();
	~TestScene();

	void Start();
	void Update();

private:
	hodoEngine::Scene* _scene;
	hodoEngine::SceneSystem* _system;

};

