#include "TestScene.h"
#include "SceneSystem.h"

TestScene::TestScene()
{
	_system = new hodoEngine::SceneSystem();
	_scene = _system->CreateScene("TestScene");
}

TestScene::~TestScene()
{

}

void TestScene::Start()
{

}

void TestScene::Update()
{

}
