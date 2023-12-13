#include "TestScene.h"
#include "../HODOengine/ObjectSystem.h"
#include "../HODOengine/GameObject.h"
#include "../HODOengine/Component.h"
#include "MovableCamera.h"

enum eColliderType
{
	PLAYER = 1,
	BOX = 2
};

TestScene::TestScene()
{
	_scene = API::CreateScene("Test");

	auto camera = API::CreateObject(_scene);
	camera->AddComponent<MovableCamera>()->SetMainCamera();
	camera->GetTransform()->SetWorldPosition(HDMaths::HDFLOAT3{ 0.0f, 0.0f, -10.0f });

	//auto gameObject = hodoEngine::CreateObject(_scene);
	//auto comp = gameObject->AddComponent<hodoEngine::DebugCube>();
	////auto test = comp->GetGameObject();
	//comp->Get()->SetFillModeWireframe();
	//comp->Get()->SetColor(HDMaths::HDFLOAT4{ 1.0f, 0.0f, 0.0f, 0.0f });

	auto debugtest = API::CreateObject(_scene);
	debugtest->GetComponent<hodoData::Transform>()->SetWorldScale(HDMaths::HDFLOAT3{5.f, 5.f, 5.f});
	auto colli = debugtest->AddComponent<hodoData::StaticBoxCollider>();
	colli->Setflag(eColliderType::PLAYER);

	
	// 디버그 모드 활성화 (활성화하고 싶은 플래그를 인자로 넣음)
	API::DebugModeOn(eColliderType::PLAYER);

	API::LoadScene(_scene);
}

TestScene::~TestScene()
{

}

void TestScene::Start()
{
	
}