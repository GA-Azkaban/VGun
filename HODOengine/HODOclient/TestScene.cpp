#include "TestScene.h"
#include "../HODOengine/ObjectSystem.h"
#include "../HODOengine/GameObject.h"
#include "../HODOengine/Component.h"
#include "../HODOengine/HodoDebugCube.h"
#include "../HODOengine/Camera.h"

TestScene::TestScene()
{
	_scene = hodoData::CreateScene("Test");

	auto camera = hodoEngine::CreateObject(_scene);
	camera->AddComponent<hodoEngine::Camera>()->SetMainCamera();
	camera->GetTransform()->SetWorldPosition(HDMaths::HDFLOAT3{ 0.0f, 0.0f, -5.0f });

	auto gameObject = hodoEngine::CreateObject(_scene);
	auto comp = gameObject->AddComponent<hodoEngine::DebugCube>();
	comp->Get()->SetFillModeWireframe();
	comp->Get()->SetColor(hodoGI::Color{ 1.0f, 0.0f, 0.0f, 0.0f });

	hodoEngine::LoadScene(_scene);
}

TestScene::~TestScene()
{

}

void TestScene::Start()
{
	hodoData::GameObject* test = hodoData::CreateObject(_scene);
test->AddComponent<ScriptTest>();
}