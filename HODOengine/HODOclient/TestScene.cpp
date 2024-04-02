#include "TestScene.h"
#include "../HODOengine/ObjectSystem.h"
#include "../HODOengine/GameObject.h"
#include "../HODOengine/Component.h"
#include "../HODOengine/AudioSource.h"
#include "CameraMove.h"
#include "Player.h"
#include "PlayerMove.h"
#include "TestSound.h"
#include "SliderSoundScript.h"
#include "NetworkManager.h"

enum eColliderType
{
	PLAYER = 1,
	BOX = 2
};

TestScene::TestScene()
{
	_scene = API::CreateScene("Test");

	//API::LoadSceneFromData("transformData.json");

	//API::LoadScene(_scene);

	auto mainCam = API::GetMainCamera()->GetGameObject();
	mainCam->AddComponent<CameraMove>();

	auto skybox = API::CreateObject(_scene);
	auto skyboxComp = skybox->AddComponent<HDData::CubeMapRenderer>();
	skyboxComp->LoadCubeMapTexture("Day Sun Peak Clear.dds");
	//skyboxComp->LoadCubeMapTexture("sunsetcube1024.dds");
	//skyboxComp->LoadCubeMapTexture("skyboxmap.png");

	//auto testBox1 = API::CreateObject(_scene);
	//testBox1->GetComponent<HDData::Transform>()->SetPosition(0.0f, -0.5f, 0.0f);
	//auto boxRender1 = testBox1->AddComponent<HDData::MeshRenderer>();
	//boxRender1->LoadMesh("Plane.fbx");
	//boxRender1->SetMetallicValue(0.0f);

	// 플레이어 테스트
	auto playerTest = API::CreateObject(_scene, "player");
	playerTest->GetComponent<HDData::Transform>()->SetPosition(Vector3{ -5.0f, 0.5f, 0.0f });
	playerTest->AddComponent<Player>();
	// 확장자 포함한 파일이름을 넣어준다. 
	// LoadNodeFromFBXFile 함수는 노드를 따라 게임오브젝트를 계층구조대로 생성해주고
	// 메쉬와 노드를 불러와 적용시킨다.
	// 그리고 자식오브젝트를 만들어 SkinnedMeshRenderer 컴포넌트를 부착한다.
	playerTest->LoadNodeFromFBXFile("SKM_TP_X_idle.fbx");

	// SkinnedMeshRenderer 컴포넌트는 자식오브젝트에 생성되므로
	// GetComponentInChildren 함수로 가져와서 사용해야 한다.
	auto meshComp = playerTest->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
	meshComp->LoadAlbedoMap("TP_Red_B.png");
	meshComp->PlayAnimation("X_idle", true);
	meshComp->SetOutlineActive(true);

	// 오른손 노드의 오브젝트를 가져와서
	// 그 오브젝트의 자식 오브젝트를 새로 만들어 총기 메쉬를 부착한다.
	auto hand = playerTest->GetGameObjectByNameInChildren("ik_hand_r");
	auto weaponTest = API::CreateObject(_scene, "weapon", hand);
	weaponTest->GetComponent<HDData::Transform>()->SetLocalPosition(-16.0f, -10.0f, 3.0f);
	weaponTest->GetComponent<HDData::Transform>()->Rotate(200.0f, 270.0f, 95.0f);
	auto weaponComp = weaponTest->AddComponent<HDData::MeshRenderer>();
	weaponComp->LoadMesh("SM_AR1.fbx");
	weaponComp->LoadAlbedoMap("T_WEP_Camo_001_D.png");
	weaponComp->LoadNormalMap("T_WEP_Camo_N.png");

	API::LoadScene(_scene);
}

TestScene::~TestScene()
{

}

void TestScene::Start()
{

}

void TestScene::ClickEvent()
{
}

