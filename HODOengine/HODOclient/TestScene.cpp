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
	//test->GetComponent<hodoData::Transform>()->SetWorldPosition(3.f, 2.f, 3.f);


	test->AddComponent<ScriptTest>();
}