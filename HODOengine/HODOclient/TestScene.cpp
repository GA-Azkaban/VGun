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

	auto mainCam = API::GetMainCamera()->GetGameObject();
	mainCam->AddComponent<CameraMove>();

	auto skybox = API::CreateObject(_scene);
	auto skyboxComp = skybox->AddComponent<HDData::CubeMapRenderer>();
	skyboxComp->LoadCubeMapTexture("Day Sun Peak Clear.dds");

	auto testBox1 = API::CreateObject(_scene);
	testBox1->GetComponent<HDData::Transform>()->SetPosition(0.0f, -1.0f, 0.0f);
	auto boxRender1 = testBox1->AddComponent<HDData::MeshRenderer>();
	//boxRender1->LoadMesh("SM_Plane.fbx");
	boxRender1->LoadMesh("primitiveCube");

	auto testBox2 = API::CreateObject(_scene);
	testBox2->GetComponent<HDData::Transform>()->SetPosition(-20.0f, -1.0f, 0.0f);
	auto boxRender2 = testBox2->AddComponent<HDData::MeshRenderer>();
	boxRender2->LoadMesh("primitiveCube");

	auto testBox3 = API::CreateObject(_scene);
	testBox3->GetComponent<HDData::Transform>()->SetPosition(-50.0f, -1.0f, 0.0f);
	auto boxRender3 = testBox3->AddComponent<HDData::MeshRenderer>();
	boxRender3->LoadMesh("primitiveCube");

	auto testBox4 = API::CreateObject(_scene);
	testBox4->GetComponent<HDData::Transform>()->SetPosition(-80.0f, -1.0f, 20.0f);
	auto boxRender4 = testBox4->AddComponent<HDData::MeshRenderer>();
	boxRender4->LoadMesh("primitiveCube");

	auto testBox5 = API::CreateObject(_scene);
	testBox5->GetComponent<HDData::Transform>()->SetPosition(50.0f, -1.0f, -10.0f);
	auto boxRender5 = testBox5->AddComponent<HDData::MeshRenderer>();
	boxRender5->LoadMesh("primitiveCube");

	// 플레이어 테스트
	auto playerTest = API::CreateObject(_scene, "player");
	playerTest->GetComponent<HDData::Transform>()->SetPosition(Vector3{ 0.0f, 0.0f, 0.0f });
	playerTest->AddComponent<Player>();
	// 확장자 포함한 파일이름을 넣어준다. 
	// LoadFBXFile 함수는 노드를 따라 게임오브젝트를 계층구조대로 생성해주고
	// 메쉬와 노드를 불러와 적용시킨다.
	// 그리고 자식오브젝트를 만들어 SkinnedMeshRenderer 컴포넌트를 부착한다.
	playerTest->LoadFBXFile("SKM_TP_X_idle.fbx");

	// SkinnedMeshRenderer 컴포넌트는 자식오브젝트에 생성되므로
	// GetComponentInChildren 함수로 가져와서 사용해야 한다.
	auto meshComp = playerTest->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
	HDEngine::MaterialDesc desc;
	desc.materialName = "TP_Red";
	desc.albedo = "TP_Red_B.png";
	HDData::Material* newMat = API::CreateMaterial(desc);
	meshComp->LoadMaterial(newMat, 0);
	meshComp->LoadMaterial(newMat, 1);
	meshComp->LoadMaterial(newMat, 2);
	meshComp->LoadMaterial(newMat, 3);
	meshComp->LoadMaterial(newMat, 4);
	meshComp->PlayAnimationUpper("HG_shoot", true);
	meshComp->PlayAnimationLower("X_crouch", true);
	meshComp->SetOutlineActive(true);

	// 오른손 노드의 오브젝트를 가져와서
	// 그 오브젝트의 자식 오브젝트를 새로 만들어 총기 메쉬를 부착한다.
	auto hand = playerTest->GetGameObjectByNameInChildren("ik_hand_r");
	auto weaponTest = API::CreateObject(_scene, "weapon", hand);
	weaponTest->GetComponent<HDData::Transform>()->SetLocalPosition(-16.0f, -10.0f, 3.0f);
	weaponTest->GetComponent<HDData::Transform>()->Rotate(200.0f, 270.0f, 95.0f);
	auto weaponComp = weaponTest->AddComponent<HDData::MeshRenderer>();
	weaponComp->LoadMesh("SM_AR_01.fbx");
	HDEngine::MaterialDesc weaponMatDesc;
	weaponMatDesc.materialName = "M_WEP_Basic_039";
	weaponMatDesc.albedo = "T_WEP_Basic_039_D.png";
	weaponMatDesc.roughness = "T_WEP_Basic_R.png";
	weaponMatDesc.metallicValue = 0.15;
	HDData::Material* weaponMat1 = API::CreateMaterial(weaponMatDesc);
	HDEngine::MaterialDesc weaponMatDesc2;
	weaponMatDesc2.materialName = "M_WEP_Camo_001";
	weaponMatDesc2.albedo = "T_WEP_Camo_001_D.png";
	weaponMatDesc2.normalMap = "T_WEP_Camo_N.png";
	weaponMatDesc2.roughness = "T_WEP_Camo_001_R.png";
	weaponMatDesc2.metallicValue = 0.1f;
	HDData::Material* weaponMat2 = API::CreateMaterial(weaponMatDesc2);
	HDEngine::MaterialDesc weaponMatDesc3;
	weaponMatDesc3.materialName = "M_WEP_CarbonFibre_001";
	weaponMatDesc3.albedo = "T_WEP_CarbonFibre_001_D.png";
	weaponMatDesc3.normalMap = "T_WEP_CarbonFibre_N.png";
	weaponMatDesc3.roughness = "T_WEP_CarbonFibre_R.png";
	weaponMatDesc3.metallicValue = 0.1f;
	HDData::Material* weaponMat3 = API::CreateMaterial(weaponMatDesc3);
	weaponComp->LoadMaterial(weaponMat1, 0);
	weaponComp->LoadMaterial(weaponMat2, 1);
	weaponComp->LoadMaterial(weaponMat2, 3);
	weaponComp->LoadMaterial(weaponMat2, 5);
	weaponComp->LoadMaterial(weaponMat3, 2);
	weaponComp->LoadMaterial(weaponMat3, 4);

	//-----------------------------------------
	auto playerTest2 = API::CreateObject(_scene, "player2");
	playerTest2->GetComponent<HDData::Transform>()->SetPosition(Vector3{ -20.0f, 0.5f, 0.0f });
	playerTest2->LoadFBXFile("SKM_TP_X_idle.fbx");

	auto meshComp2 = playerTest2->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
	HDEngine::MaterialDesc desc2;
	desc2.materialName = "TP_Green";
	desc2.albedo = "TP_Green_B.png";
	HDData::Material* newMat2 = API::CreateMaterial(desc2);
	meshComp2->LoadMaterial(newMat2, 0);
	meshComp2->LoadMaterial(newMat2, 1);
	meshComp2->LoadMaterial(newMat2, 2);
	meshComp2->LoadMaterial(newMat2, 3);
	meshComp2->LoadMaterial(newMat2, 4);
	meshComp2->PlayAnimation("X_idle", true);
	meshComp2->SetOutlineActive(true);

	auto playerTest3 = API::CreateObject(_scene, "player3");
	playerTest3->GetComponent<HDData::Transform>()->SetPosition(Vector3{ -50.0f, 0.5f, 0.0f });
	playerTest3->LoadFBXFile("SKM_TP_X_idle.fbx");

	auto meshComp3 = playerTest3->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
	HDEngine::MaterialDesc desc3;
	desc3.materialName = "TP_Blue";
	desc3.albedo = "TP_Blue_B.png";
	HDData::Material* newMat3 = API::CreateMaterial(desc3);
	meshComp3->LoadMaterial(newMat3, 0);
	meshComp3->LoadMaterial(newMat3, 1);
	meshComp3->LoadMaterial(newMat3, 2);
	meshComp3->LoadMaterial(newMat3, 3);
	meshComp3->LoadMaterial(newMat3, 4);
	meshComp3->PlayAnimation("X_idle", true);
	meshComp3->SetOutlineActive(true);

	auto playerTest4 = API::CreateObject(_scene, "player4");
	playerTest4->GetComponent<HDData::Transform>()->SetPosition(Vector3{ -80.0f, 0.5f, 20.0f });
	playerTest4->LoadFBXFile("SKM_TP_X_idle.fbx");

	auto meshComp4 = playerTest4->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
	//HDEngine::MaterialDesc desc4;
	//desc4.materialName = "TP_Blue";
	//desc4.albedo = "TP_Blue_B.png";
	//HDData::Material* newMat4 = API::CreateMaterial(desc4);
	meshComp4->LoadMaterial(newMat2, 0);
	meshComp4->LoadMaterial(newMat2, 1);
	meshComp4->LoadMaterial(newMat2, 2);
	meshComp4->LoadMaterial(newMat2, 3);
	meshComp4->LoadMaterial(newMat2, 4);
	meshComp4->PlayAnimation("X_idle", true);
	meshComp4->SetOutlineActive(true);

	auto playerTest5 = API::CreateObject(_scene, "player5");
	playerTest5->GetComponent<HDData::Transform>()->SetPosition(Vector3{ 50.0f, 0.5f, -10.0f });
	playerTest5->LoadFBXFile("SKM_TP_X_idle.fbx");

	auto meshComp5 = playerTest5->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
	//HDEngine::MaterialDesc desc5;
	//desc5.materialName = "TP_Blue";
	//desc5.albedo = "TP_Blue_B.png";
	//HDData::Material* newMat5 = API::CreateMaterial(desc4);
	meshComp5->LoadMaterial(newMat3, 0);
	meshComp5->LoadMaterial(newMat3, 1);
	meshComp5->LoadMaterial(newMat3, 2);
	meshComp5->LoadMaterial(newMat3, 3);
	meshComp5->LoadMaterial(newMat3, 4);
	meshComp5->PlayAnimation("X_idle", true);
	meshComp5->SetOutlineActive(true);

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

