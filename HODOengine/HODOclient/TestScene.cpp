#include "TestScene.h"
#include "../HODOengine/ObjectSystem.h"
#include "../HODOengine/GameObject.h"
#include "../HODOengine/Component.h"
#include "CameraMove.h"
#include "Player.h"
#include "PlayerMove.h"

enum eColliderType
{
	PLAYER = 1,
	BOX = 2
};

TestScene::TestScene()
{
	_scene = API::CreateScene("Test");
	auto mainCam = _scene->GetMainCamera()->GetGameObject();
	mainCam->AddComponent<CameraMove>();

	//auto camera = API::CreateObject(_scene);
	//camera->AddComponent<MovableCamera>()->SetMainCamera();
	//camera->GetTransform()->SetWorldPosition(HDMath::HDFLOAT3{ 0.0f, 0.0f, -10.0f });

	//auto gameObject = hodoEngine::CreateObject(_scene);
	//auto comp = gameObject->AddComponent<hodoEngine::DebugCube>();
	////auto test = comp->GetGameObject();
	//comp->Get()->SetFillModeWireframe();
	//comp->Get()->SetColor(HDMaths::HDFLOAT4{ 1.0f, 0.0f, 0.0f, 0.0f });

	auto debugtest = API::CreateObject(_scene);
	debugtest->GetComponent<HDData::Transform>()->SetWorldScale(HDMath::HDFLOAT3{5.f, 5.f, 5.f});
	debugtest->GetComponent<HDData::Transform>()->SetWorldPosition(HDMath::HDFLOAT3{1.f, 10.f, 1.f});
	auto colli = debugtest->AddComponent<HDData::DynamicBoxCollider>();

	colli->Setflag(eColliderType::PLAYER);
	
	
	// 플레이어 테스트
	auto playerTest = API::CreateObject(_scene);
	playerTest->AddComponent<Player>();
	playerTest->AddComponent<PlayerMove>();
	//auto playerColli = playerTest->AddComponent<HDData::>

	
	// ?붾쾭洹?紐⑤뱶 ?쒖꽦??(?쒖꽦?뷀븯怨??띠? ?뚮옒洹몃? ?몄옄濡??ｌ쓬)
	API::DebugModeOn(eColliderType::PLAYER);

	API::LoadScene(_scene);

	// 디버그 모드를 시작하는 함수
	API::DebugModeOn(eColliderType::PLAYER);
}

TestScene::~TestScene()
{

}

void TestScene::Start()
{
	
}