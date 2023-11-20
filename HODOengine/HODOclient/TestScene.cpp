#include "TestScene.h"

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
	test->GetComponent<hodoData::Transform>();
	test->AddComponent<hodoData::Script>();
}