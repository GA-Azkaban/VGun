#include "TestScene.h"
#include "../HODOengine/ObjectSystem.h"
#include "../HODOengine/GameObject.h"
#include "../HODOengine/Component.h"
#include "../HODOengine/HodoDebugCube.h"
#include "MovableCamera.h"

TestScene::TestScene()
{
	_scene = hodoEngine::CreateScene("Test");

	auto camera = hodoEngine::CreateObject(_scene);
	camera->AddComponent<MovableCamera>()->SetMainCamera();
	camera->GetTransform()->SetWorldPosition(HDMaths::HDFLOAT3{ 0.0f, 0.0f, -5.0f });

	auto gameObject = hodoEngine::CreateObject(_scene);
	auto comp = gameObject->AddComponent<hodoEngine::DebugCube>();
	comp->Get()->SetFillModeWireframe();
	comp->Get()->SetColor(HDMaths::HDFLOAT4{ 1.0f, 0.0f, 0.0f, 0.0f });

	auto debugtest = gameObject = hodoEngine::CreateObject(_scene);

	hodoEngine::LoadScene(_scene);
}

TestScene::~TestScene()
{

}

void TestScene::Start()
{
	
}