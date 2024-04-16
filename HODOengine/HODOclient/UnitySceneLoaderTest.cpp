#include "UnitySceneLoaderTest.h"
#include "CameraMove.h"
#include "FSMtestScript.h"

UnitySceneLoaderTest::UnitySceneLoaderTest()
{

}

UnitySceneLoaderTest::~UnitySceneLoaderTest()
{

}

void UnitySceneLoaderTest::Start()
{
	_scene = API::CreateScene("Test");

	auto mainCam = API::GetMainCamera()->GetGameObject();
	//mainCam->AddComponent<CameraMove>();

	//auto playerFP = API::CreateObject(_scene, "playerFP");
	//playerFP->LoadNodeFromFBXFile("SKM_FP_X_idle.fbx");
	//
	//auto meshCompFP = playerFP->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
	//meshCompFP->LoadAlbedoMap("FP_Yellow_A.png");
	//meshCompFP->PlayAnimation("X_idle", true);

	auto playerTP = API::CreateObject(_scene, "playerTP");
	playerTP->GetTransform()->Translate(5.f, 0.f, 0.f);
	playerTP->LoadNodeFromFBXFile("SKM_TP_X_idle.fbx");

	auto meshComp = playerTP->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
	meshComp->LoadAlbedoMap("TP_Red_B.png");
	playerTP->AddComponent<FSMtestScript>();
	API::LoadAnimationFromData(playerTP, "data.json");

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

	//API::LoadSceneFromData("transformData.json");

	API::LoadScene(_scene);
}
