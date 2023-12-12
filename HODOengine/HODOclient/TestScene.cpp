#include "TestScene.h"
#include "../HODOengine/ObjectSystem.h"
#include "../HODOengine/GameObject.h"
#include "../HODOengine/Component.h"
#include "MovableCamera.h"

TestScene::TestScene()
{
	_scene = HDEngine::CreateScene("Test");

	auto camera = HDEngine::CreateObject(_scene);
	camera->AddComponent<MovableCamera>()->SetMainCamera();
	camera->GetTransform()->SetWorldPosition(HDMath::HDFLOAT3{ 0.0f, 0.0f, -10.0f });

	auto gameObject = HDEngine::CreateObject(_scene);
// 	auto comp = gameObject->AddComponent<HDEngine::DebugCube>();
// 	comp->Get()->SetFillModeWireframe();
// 	comp->Get()->SetColor(HDMath::HDFLOAT4{ 1.0f, 0.0f, 0.0f, 0.0f });

	auto debugtest = gameObject = HDEngine::CreateObject(_scene);

	HDEngine::LoadScene(_scene);
}

TestScene::~TestScene()
{

}

void TestScene::Start()
{
	
}