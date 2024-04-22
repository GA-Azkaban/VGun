#include "UnitySceneLoaderTest.h"
#include "CameraMove.h"
#include "FSMtestScript.h"
#include "PlayerMove.h"

UnitySceneLoaderTest::UnitySceneLoaderTest()
{

}

UnitySceneLoaderTest::~UnitySceneLoaderTest()
{

}

void UnitySceneLoaderTest::Start()
{
	_scene = API::CreateScene("Scene2");

	auto mainCam = API::GetMainCamera()->GetGameObject();
	mainCam->AddComponent<CameraMove>();

	//auto playerFP = API::CreateObject(_scene, "playerFP");
	//playerFP->LoadNodeFromFBXFile("SKM_FP_X_idle.fbx");
	//
	//auto meshCompFP = playerFP->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
	//meshCompFP->LoadAlbedoMap("FP_Yellow_A.png");
	//meshCompFP->PlayAnimation("X_idle", true);

	auto playerTP = API::CreateObject(_scene, "playerTP");
	playerTP->GetTransform()->Translate(5.f, 3.f, 0.f);
	playerTP->LoadFBXFile("SKM_TP_X_idle.fbx");

	// 바닥
	auto groundFloor = API::CreateObject(_scene, "ground");
	groundFloor->GetComponent<HDData::Transform>()->SetPosition(0.f, 0.f, 0.f);
	auto groundCollier = groundFloor->AddComponent<HDData::StaticPlaneCollider>();

	// 플레이어 collider들을 추가해주는 부분
	auto playerCollider = playerTP->AddComponent<HDData::DynamicBoxCollider>(1.0f, 1.2f, 0.5f, 1);

	auto playerTestHead = playerTP->GetGameObjectByNameInChildren("head");
	playerTestHead->SetParentObject(playerTP);
	playerTestHead->GetTransform()->SetLocalPosition(Vector3{ 0.0f, 0.9f, 0.0f });
	auto playerHeadCollider = playerTestHead->AddComponent<HDData::DynamicSphereCollider>(0.35f, true);
	playerHeadCollider->SetParentCollider(playerCollider);

	auto plLeftUpperArm = playerTP->GetGameObjectByNameInChildren("upperarm_l");
	plLeftUpperArm->SetParentObject(playerTP);
	plLeftUpperArm->GetTransform()->SetLocalPosition(Vector3{ -0.65f, 0.2f, 0.0f });
	auto LUArmCollider = plLeftUpperArm->AddComponent<HDData::DynamicBoxCollider>(0.25f, 0.7f, 0.25f, 2);
	LUArmCollider->SetParentCollider(playerCollider);

	auto plLeftForeArm = playerTP->GetGameObjectByNameInChildren("lowerarm_l");
	plLeftForeArm->SetParentObject(plLeftUpperArm);
	plLeftForeArm->GetTransform()->SetLocalPosition(Vector3{ 0.0f, -0.8f, 0.0f });
	auto LFArmCollider = plLeftForeArm->AddComponent<HDData::DynamicBoxCollider>(0.2f, 0.9f, 0.2f, 2);
	LFArmCollider->SetParentCollider(LUArmCollider);

	auto plRightUpperArm = playerTP->GetGameObjectByNameInChildren("upperarm_r");
	plRightUpperArm->SetParentObject(playerTP);
	plRightUpperArm->GetTransform()->SetLocalPosition(Vector3{ 0.65f, 0.2f, 0.0f });
	auto RUArmCollider = plRightUpperArm->AddComponent<HDData::DynamicBoxCollider>(0.25f, 0.7f, 0.25f, 2);
	RUArmCollider->SetParentCollider(playerCollider);

	auto plRightForeArm = playerTP->GetGameObjectByNameInChildren("lowerarm_r");
	plRightForeArm->SetParentObject(plRightUpperArm);
	plRightForeArm->GetTransform()->SetLocalPosition(Vector3{ 0.0f, -0.8f, 0.0f });
	auto RFArmCollider = plRightForeArm->AddComponent<HDData::DynamicBoxCollider>(0.2f, 0.9f, 0.2f, 2);
	RFArmCollider->SetParentCollider(RUArmCollider);

	auto plLeftThigh = playerTP->GetGameObjectByNameInChildren("thigh_l");
	plLeftThigh->SetParentObject(playerTP);
	plLeftThigh->GetTransform()->SetLocalPosition(Vector3{ -0.3f, -0.95f, 0.0f });
	auto LThighCollider = plLeftThigh->AddComponent<HDData::DynamicBoxCollider>(0.4f, 0.7f, 0.4f, 2);
	LThighCollider->SetParentCollider(playerCollider);

	auto plLeftLowerLeg = playerTP->GetGameObjectByNameInChildren("calf_l");
	plLeftLowerLeg->SetParentObject(plLeftThigh);
	plLeftLowerLeg->GetTransform()->SetLocalPosition(Vector3{ 0.0f, -0.7f, 0.0f });
	auto LLLegCollider = plLeftLowerLeg->AddComponent<HDData::DynamicBoxCollider>(0.4f, 0.7f, 0.4f, 2);
	LLLegCollider->SetParentCollider(LThighCollider);

	auto plLeftFoot = playerTP->GetGameObjectByNameInChildren("foot_l");
	plLeftFoot->SetParentObject(plLeftLowerLeg);
	plLeftFoot->GetTransform()->SetLocalPosition(Vector3{ 0.0f, -0.5f, 0.1f });
	auto LFootCollider = plLeftFoot->AddComponent<HDData::DynamicBoxCollider>(0.4f, 0.3f, 0.6f, 2);
	LFootCollider->SetParentCollider(LLLegCollider);

	auto plRightThigh = playerTP->GetGameObjectByNameInChildren("thigh_r");
	plRightThigh->SetParentObject(playerTP);
	plRightThigh->GetTransform()->SetLocalPosition(Vector3{ 0.3f, -0.95f, 0.0f });
	auto RThighCollider = plRightThigh->AddComponent<HDData::DynamicBoxCollider>(0.4f, 0.7f, 0.4f, 2);
	RThighCollider->SetParentCollider(playerCollider);

	auto plRightLowerLeg = playerTP->GetGameObjectByNameInChildren("calf_r");
	plRightLowerLeg->SetParentObject(plRightThigh);
	plRightLowerLeg->GetTransform()->SetLocalPosition(Vector3{ 0.0f, -0.7f, 0.0f });
	auto RLLegCollider = plRightLowerLeg->AddComponent<HDData::DynamicBoxCollider>(0.4f, 0.7f, 0.4f, 2);
	RLLegCollider->SetParentCollider(RThighCollider);

	auto plRightFoot = playerTP->GetGameObjectByNameInChildren("foot_r");
	plRightFoot->SetParentObject(plRightLowerLeg);
	plRightFoot->GetTransform()->SetLocalPosition(Vector3{ 0.0f, -0.5f, 0.1f });
	auto RFootCollider = plRightFoot->AddComponent<HDData::DynamicBoxCollider>(0.4f, 0.3f, 0.6f, 2);
	RFootCollider->SetParentCollider(RLLegCollider);

	// 텍스트 오브젝트
	auto playerPosText = API::CreateTextbox(_scene);
	playerPosText->GetTransform()->SetPosition(Vector3(1700.0f, 40.0f, 50.0f));
	playerPosText->GetComponent<HDData::TextUI>()->SetColor(DirectX::XMVECTOR{ 1.0f, 0.0f, 0.0f, 1.0f });

	auto aimText = API::CreateTextbox(_scene);
	aimText->GetTransform()->SetPosition(Vector3(950.0f, 520.0f, 50.0f));
	aimText->GetComponent<HDData::TextUI>()->SetColor(DirectX::XMVECTOR{ 1.0f, 0.0f, 0.0f, 1.0f });
	aimText->GetComponent<HDData::TextUI>()->SetText("");

	// 플레이어에 달린 카메라, 움직임
	auto headCamObj = API::CreateObject(_scene, "headCamObj");
	headCamObj->SetParentObject(playerTestHead);
	headCamObj->GetTransform()->SetLocalPosition(Vector3{ 0.0f, 0.1f, 0.3f });
	auto headCam = headCamObj->AddComponent<HDData::Camera>();

	auto playerMove = playerTP->AddComponent<PlayerMove>();
	playerMove->SetPlayerCamera(_scene->GetMainCamera());
	playerMove->SetPlayerText(playerPosText->GetComponent<HDData::TextUI>(), aimText->GetComponent<HDData::TextUI>());
	playerTP->GetComponent<PlayerMove>()->SetPlayerCamera(_scene->GetMainCamera());
	playerMove->SetHeadCam(headCam);

	/*auto meshComp = playerTP->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
	meshComp->LoadAlbedoMap("TP_Red_B.png");*/
	//playerTP->AddComponent<FSMtestScript>();
	//API::LoadAnimationFromData(playerTP, "data.json");

	// animationController
	/*auto aniComp = playerTP->AddComponent<HDData::Animator>();
	auto controller = API::CreateAnimationController();
	controller->CreateState("IDLE", "X_idle");
	controller->CreateState("IDLE_AIM", "idle_aim.fbx");
	controller->CreateState("WALK", "X_run");
	controller->CreateState("JUMP", "jump_real.fbx");
	controller->CreateState("CRUNCH_DOWN", "crunch_down.fbx");
	controller->CreateState("CRUNCH", "crunch.fbx");
	controller->CreateState("CRUNCH_UP", "crunch_up.fbx");
	controller->CreateState("SHOOT", "idle_shoot.fbx");
	controller->CreateState("CRUNCH_SHOOT", "shoot.fbx");
	controller->CreateBoolParam("isAim", false);
	controller->CreateBoolParam("isWalk", false);
	controller->CreateTriggerParam("isJump");
	controller->CreateBoolParam("isCrunch", false);
	controller->CreateTriggerParam("isShoot");
	controller->CreateTriggerParam("isCrunchShoot");
	aniComp->SetAnimationController(controller);
	controller->GetState("IDLE").MakeTransition("WALK").AddCondition("WALK", "isWalk", true);
	controller->GetState("WALK").MakeTransition("IDLE").AddCondition("IDLE", "isWalk", false);
	controller->GetState("IDLE").MakeTransition("JUMP").AddTrigger("JUMP", "isJump", true);
	controller->GetState("JUMP").MakeTransition("IDLE");
	controller->GetState("IDLE").MakeTransition("IDLE_AIM").AddCondition("IDLE_AIM", "isAim", true);
	controller->GetState("IDLE_AIM").MakeTransition("IDLE").AddCondition("IDLE", "isAim", false);
	controller->GetState("IDLE_AIM").MakeTransition("SHOOT").AddTrigger("SHOOT", "isShoot", true);
	controller->GetState("SHOOT").MakeTransition("IDLE_AIM");
	controller->GetState("IDLE_AIM").MakeTransition("CRUNCH").AddCondition("CRUNCH", "isCrunch", true);
	controller->GetState("CRUNCH").MakeTransition("IDLE_AIM").AddCondition("IDLE_AIM", "isCrunch", false);
	controller->GetState("WALK").MakeTransition("JUMP").AddTrigger("JUMP", "isJump", true);
	controller->GetState("JUMP").MakeTransition("WALK");
	controller->GetState("IDLE").MakeTransition("CRUNCH").AddCondition("CRUNCH", "isCrunch", true, "CRUNCH_DOWN");
	controller->GetState("CRUNCH").MakeTransition("IDLE").AddCondition("IDLE", "isCrunch", false, "CRUNCH_UP");
	controller->GetState("CRUNCH").MakeTransition("CRUNCH_SHOOT").AddTrigger("CRUNCH_SHOOT", "isCrunchShoot", true);
	controller->GetState("CRUNCH_SHOOT").MakeTransition("CRUNCH");
	controller->SetEntryState("IDLE");*/

	API::LoadSceneFromData("sceneData.json");

	//API::LoadScene(_scene);
}
