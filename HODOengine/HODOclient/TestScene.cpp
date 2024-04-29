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
	_scene = API::CreateScene("Test Scene");

	auto mainCam = API::GetCurrenSceneMainCamera()->GetGameObject();
	mainCam->AddComponent<CameraMove>();

	auto skybox = API::CreateObject(_scene);
	auto skyboxComp = skybox->AddComponent<HDData::CubeMapRenderer>();
	skyboxComp->LoadCubeMapTexture("Day Sun Peak Clear.dds");

	//auto testBox1 = API::CreateObject(_scene);
	//testBox1->GetComponent<HDData::Transform>()->SetPosition(0.0f, -1.0f, 0.0f);
	//auto boxRender1 = testBox1->AddComponent<HDData::MeshRenderer>();
	//boxRender1->LoadMesh("primitiveCube");
	//
	//auto testBox2 = API::CreateObject(_scene);
	//testBox2->GetComponent<HDData::Transform>()->SetPosition(-20.0f, -1.0f, 0.0f);
	//auto boxRender2 = testBox2->AddComponent<HDData::MeshRenderer>();
	//boxRender2->LoadMesh("primitiveCube");
	//
	//auto testBox3 = API::CreateObject(_scene);
	//testBox3->GetComponent<HDData::Transform>()->SetPosition(-50.0f, -1.0f, 0.0f);
	//auto boxRender3 = testBox3->AddComponent<HDData::MeshRenderer>();
	//boxRender3->LoadMesh("primitiveCube");
	//
	//auto testBox4 = API::CreateObject(_scene);
	//testBox4->GetComponent<HDData::Transform>()->SetPosition(-80.0f, -1.0f, 20.0f);
	//auto boxRender4 = testBox4->AddComponent<HDData::MeshRenderer>();
	//boxRender4->LoadMesh("primitiveCube");
	//
	//auto testBox5 = API::CreateObject(_scene);
	//testBox5->GetComponent<HDData::Transform>()->SetPosition(50.0f, -1.0f, -10.0f);
	//auto boxRender5 = testBox5->AddComponent<HDData::MeshRenderer>();
	//boxRender5->LoadMesh("primitiveCube");	

	//auto buildingTest1 = API::CreateObject(_scene);
	//buildingTest1->GetComponent<HDData::Transform>()->SetPosition(20.0f, 0.0f, 10.0f);
	//buildingTest1->GetComponent<HDData::Transform>()->Rotate(0.0f, -90.0f, 0.0f);
	//auto buildingRenderer1 = buildingTest1->AddComponent<HDData::MeshRenderer>();
	//buildingRenderer1->LoadMesh("SM_building_town_01.fbx");
	//HDEngine::MaterialDesc buildingDesc1;
	//buildingDesc1.materialName = "colorpalette_standard";
	////buildingDesc1.albedo = "colorpalette_standard.png";
	//buildingDesc1.color = { 178,161,132,255 };
	////buildingDesc1.metallic = 0.0f;
	//buildingDesc1.roughness = 1.0f - 0.057f;
	//HDData::Material* newBuildingMat1 = API::CreateMaterial(buildingDesc1);
	//buildingRenderer1->LoadMaterial(newBuildingMat1, 0);
	//buildingRenderer1->LoadMaterial(newBuildingMat1, 1);
	//buildingRenderer1->LoadMaterial(newBuildingMat1, 2);
	//buildingRenderer1->LoadMaterial(newBuildingMat1, 3);
	//buildingRenderer1->LoadMaterial(newBuildingMat1, 4);
	//buildingRenderer1->LoadMaterial(newBuildingMat1, 5);

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
	//meshComp->SetActive(false);
	meshComp->SetFillModeWireFrame(true);
	HDEngine::MaterialDesc desc;
	desc.materialName = "TP_Red";
	desc.albedo = "TP_Red_B.png";
	HDData::Material* newMat = API::CreateMaterial(desc);
	meshComp->LoadMaterial(newMat, 0);
	meshComp->LoadMaterial(newMat, 1);
	meshComp->LoadMaterial(newMat, 2);
	meshComp->LoadMaterial(newMat, 3);
	meshComp->LoadMaterial(newMat, 4);
	//meshComp->PlayAnimationUpper("AR_idle", true, true, 0.1f);
	//meshComp->PlayAnimationLower("AR_idle", true, true, 0.1f);
	meshComp->PlayAnimationUpper("AR_idle", true);
	meshComp->PlayAnimationLower("AR_idle", true);
	//meshComp->SetOutlineActive(true);

	// 오른손 노드의 오브젝트를 가져와서
	// 그 오브젝트의 자식 오브젝트를 새로 만들어 총기 메쉬를 부착한다.
	/*auto hand = playerTest->GetGameObjectByNameInChildren("hand_r");
	auto weaponTest = API::CreateObject(_scene, "weapon", hand);
	weaponTest->GetComponent<HDData::Transform>()->SetLocalPosition(-18.0f, -10.0f, 0.0f);
	weaponTest->GetComponent<HDData::Transform>()->Rotate(180.0f, 255.0f, 90.0f);
	auto weaponComp = weaponTest->AddComponent<HDData::MeshRenderer>();
	weaponComp->LoadMesh("SM_AR_01.fbx");
	HDEngine::MaterialDesc weaponMatDesc;
	weaponMatDesc.materialName = "M_WEP_Basic_039";
	//weaponMatDesc.albedo = "T_WEP_Basic_039_D.png";
	weaponMatDesc.albedo = "T_WEP_Basic_004_D.png";
	weaponMatDesc.roughness = "T_WEP_Basic_R.png";
	weaponMatDesc.metallicValue = 0.15;
	HDData::Material* weaponMat1 = API::CreateMaterial(weaponMatDesc);
	HDEngine::MaterialDesc weaponMatDesc2;
	weaponMatDesc2.materialName = "M_WEP_Camo_001";
	//weaponMatDesc2.albedo = "T_WEP_Camo_001_D.png";
	weaponMatDesc2.albedo = "T_WEP_Basic_004_D.png";
	//weaponMatDesc2.normalMap = "T_WEP_Camo_N.png";
	//weaponMatDesc2.roughness = "T_WEP_Camo_001_R.png";
	weaponMatDesc2.roughness = "T_WEP_Basic_R.png";
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
	weaponComp->LoadMaterial(weaponMat3, 4); */

	// 바닥
	//auto groundFloor = API::CreateObject(_scene, "ground");
	//groundFloor->GetComponent<HDData::Transform>()->SetPosition(0.f, 0.f, 0.f);
	//auto groundCollier = groundFloor->AddComponent<HDData::StaticPlaneCollider>();

	//// 플레이어 collider들을 추가해주는 부분
	//auto playerCollider = playerTest->AddComponent<HDData::DynamicBoxCollider>(1.0f, 1.2f, 0.5f, 1);

	auto playerTestHead = playerTest->GetGameObjectByNameInChildren("head");
	auto headColliderObj = API::CreateObject(_scene, "HeadCollider", playerTestHead);
	headColliderObj->GetTransform()->SetLocalScale(25.0f, 25.0f, 19.0f);
	headColliderObj->GetTransform()->SetLocalPosition(Vector3{ 5.0f, -3.0f, 0.0f });
	//headColliderObj->GetTransform()->SetLocalPosition(Vector3{ 500.0f, 0.0f, 0.0f });
	//auto playerHeadCollider = headColliderObj->AddComponent<HDData::DynamicSphereCollider>(1.0f, true);
	//auto playerHeadCollider = headColliderObj->AddComponent<HDData::DynamicBoxCollider>(1.0f, 1.0f, 1.0f, true);
	auto playerHeadCollider = headColliderObj->AddComponent<HDData::StaticSphereCollider>();
	//playerHeadCollider->SetParentCollider(playerCollider);

	auto plLeftUpperArm = playerTest->GetGameObjectByNameInChildren("upperarm_l");
	auto leftUpperArmObj = API::CreateObject(_scene, "LeftUpperArmCollider", plLeftUpperArm);
	leftUpperArmObj->GetTransform()->SetLocalPosition(Vector3{ 5.0f, -2.0f, 0.0f });
	//leftUpperArmObj->GetTransform()->SetLocalRotation(0.0f, 0.52f, 0.0f, 1.0f);
	leftUpperArmObj->GetTransform()->SetLocalScale(20.0f, 15.0f, 10.0f);
	auto leftUpperCollider = leftUpperArmObj->AddComponent<HDData::StaticBoxCollider>();
	//plLeftUpperArm->SetParentObject(playerTest);
	//plLeftUpperArm->GetTransform()->SetLocalPosition(Vector3{ -0.65f, 0.2f, 0.0f });
	//auto LUArmCollider = plLeftUpperArm->AddComponent<HDData::DynamicBoxCollider>(0.25f, 0.7f, 0.25f, 2);
	//LUArmCollider->SetParentCollider(playerCollider);

	//auto plLeftForeArm = playerTest->GetGameObjectByNameInChildren("lowerarm_l");
	//plLeftForeArm->SetParentObject(plLeftUpperArm);
	//plLeftForeArm->GetTransform()->SetLocalPosition(Vector3{ 0.0f, -0.8f, 0.0f });
	//auto LFArmCollider = plLeftForeArm->AddComponent<HDData::DynamicBoxCollider>(0.2f, 0.9f, 0.2f, 2);
	//LFArmCollider->SetParentCollider(LUArmCollider);

	//auto plRightUpperArm = playerTest->GetGameObjectByNameInChildren("upperarm_r");
	//plRightUpperArm->SetParentObject(playerTest);
	//plRightUpperArm->GetTransform()->SetLocalPosition(Vector3{ 0.65f, 0.2f, 0.0f });
	//auto RUArmCollider = plRightUpperArm->AddComponent<HDData::DynamicBoxCollider>(0.25f, 0.7f, 0.25f, 2);
	//RUArmCollider->SetParentCollider(playerCollider);

	//auto plRightForeArm = playerTest->GetGameObjectByNameInChildren("lowerarm_r");
	//plRightForeArm->SetParentObject(plRightUpperArm);
	//plRightForeArm->GetTransform()->SetLocalPosition(Vector3{ 0.0f, -0.8f, 0.0f });
	//auto RFArmCollider = plRightForeArm->AddComponent<HDData::DynamicBoxCollider>(0.2f, 0.9f, 0.2f, 2);
	//RFArmCollider->SetParentCollider(RUArmCollider);

	//auto plLeftThigh = playerTest->GetGameObjectByNameInChildren("thigh_l");
	//plLeftThigh->SetParentObject(playerTest);
	//plLeftThigh->GetTransform()->SetLocalPosition(Vector3{ -0.3f, -0.95f, 0.0f });
	//auto LThighCollider = plLeftThigh->AddComponent<HDData::DynamicBoxCollider>(0.4f, 0.7f, 0.4f, 2);
	//LThighCollider->SetParentCollider(playerCollider);

	//auto plLeftLowerLeg = playerTest->GetGameObjectByNameInChildren("calf_l");
	//plLeftLowerLeg->SetParentObject(plLeftThigh);
	//plLeftLowerLeg->GetTransform()->SetLocalPosition(Vector3{ 0.0f, -0.7f, 0.0f });
	//auto LLLegCollider = plLeftLowerLeg->AddComponent<HDData::DynamicBoxCollider>(0.4f, 0.7f, 0.4f, 2);
	//LLLegCollider->SetParentCollider(LThighCollider);

	//auto plLeftFoot = playerTest->GetGameObjectByNameInChildren("foot_l");
	//plLeftFoot->SetParentObject(plLeftLowerLeg);
	//plLeftFoot->GetTransform()->SetLocalPosition(Vector3{ 0.0f, -0.5f, 0.1f });
	//auto LFootCollider = plLeftFoot->AddComponent<HDData::DynamicBoxCollider>(0.4f, 0.3f, 0.6f, 2);
	//LFootCollider->SetParentCollider(LLLegCollider);

	//auto plRightThigh = playerTest->GetGameObjectByNameInChildren("thigh_r");
	//plRightThigh->SetParentObject(playerTest);
	//plRightThigh->GetTransform()->SetLocalPosition(Vector3{ 0.3f, -0.95f, 0.0f });
	//auto RThighCollider = plRightThigh->AddComponent<HDData::DynamicBoxCollider>(0.4f, 0.7f, 0.4f, 2);
	//RThighCollider->SetParentCollider(playerCollider);

	//auto plRightLowerLeg = playerTest->GetGameObjectByNameInChildren("calf_r");
	//plRightLowerLeg->SetParentObject(plRightThigh);
	//plRightLowerLeg->GetTransform()->SetLocalPosition(Vector3{ 0.0f, -0.7f, 0.0f });
	//auto RLLegCollider = plRightLowerLeg->AddComponent<HDData::DynamicBoxCollider>(0.4f, 0.7f, 0.4f, 2);
	//RLLegCollider->SetParentCollider(RThighCollider);

	//auto plRightFoot = playerTest->GetGameObjectByNameInChildren("foot_r");
	//plRightFoot->SetParentObject(plRightLowerLeg);
	//plRightFoot->GetTransform()->SetLocalPosition(Vector3{ 0.0f, -0.5f, 0.1f });
	//auto RFootCollider = plRightFoot->AddComponent<HDData::DynamicBoxCollider>(0.4f, 0.3f, 0.6f, 2);
	//RFootCollider->SetParentCollider(RLLegCollider);

	//-----------------------------------------
	/*auto playerTest2 = API::CreateObject(_scene, "player2");
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
	meshComp5->SetOutlineActive(true); */

	//auto btn = API::CreateButton(_scene);
	//btn->GetComponent<HDData::Button>()->SetOnClickEvent([]()
	//	{
	//		API::LoadSceneByName("B");
	//	});

	//API::LoadScene(_scene);
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

