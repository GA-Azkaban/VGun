#include "TestScene.h"
#include "ScriptTest.h"

TestScene::TestScene()
{
	_scene = hodoData::CreateScene("Test");
}

TestScene::~TestScene()
{

}

void TestScene::Start()
{
	hodoData::GameObject* test = hodoData::CreateObject(_scene);
test->AddComponent<ScriptTest>();
}