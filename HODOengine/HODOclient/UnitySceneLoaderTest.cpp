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
	mainCam->AddComponent<CameraMove>();

	auto skybox = API::CreateObject(_scene);
	auto skyboxComp = skybox->AddComponent<HDData::CubeMapRenderer>();
	skyboxComp->LoadCubeMapTexture("Day Sun Peak Clear.dds");

	auto player = API::CreateObject(_scene, "player");
	auto meshComp = player->AddComponent<HDData::SkinnedMeshRenderer>();
	meshComp->LoadMesh("little_man3.fbx");
	meshComp->LoadDiffuseMap("T_TP_CH_Camo_001_001_D.png");
	//meshComp->PlayAnimation("rundd.fbx");

	auto aniComp = player->AddComponent<HDData::Animator>();
	player->AddComponent<FSMtestScript>();

	// animationController
	auto aniCom = API::CreateAnimationController();
	aniCom->CreateState("IDLE", "idle.fbx");
	aniCom->CreateState("WALK", "run.fbx");
	aniCom->CreateState("JUMP", "jump_real.fbx");
	aniCom->CreateState("CRUNCH", "crunch.fbx");

	aniCom->CreateBoolParam("isWalk", false);
	aniCom->CreateTriggerParam("isJump");
	aniCom->CreateTriggerParam("isCrunch");

	aniComp->SetAnimationController(aniCom);

	aniCom->GetState("IDLE").MakeTransition("WALK").AddCondition("WALK", "isWalk", true);
	aniCom->GetState("WALK").MakeTransition("IDLE").AddCondition("IDLE", "isWalk", false);

	aniCom->GetState("IDLE").MakeTransition("JUMP").AddTrigger("JUMP", "isJump", true);
	aniCom->GetState("JUMP").MakeTransition("IDLE");

	aniCom->GetState("WALK").MakeTransition("JUMP").AddTrigger("JUMP", "isJump", true);
	aniCom->GetState("JUMP").MakeTransition("WALK");

	aniCom->GetState("IDLE").MakeTransition("CRUNCH").AddTrigger("CRUNCH", "isCrunch", true);
	aniCom->GetState("CRUNCH").MakeTransition("IDLE");

	aniCom->SetEntryState("IDLE");

	//AnimationController* anicon = new AnimationController;

	//anicon->CreateState("1");
	//anicon->CreateBoolParam("isWalk", false);
	//anicon->CreateTriggerParam("isJump");

	//anicon->GetState("IDLE")->MakeTransition("WALK").AddCondition("WALK", "isWalk", true);
	//anicon->GetState("IDLE")->MakeTransition("DIE", "isDie", "Less", 0.5f);

	//Animator animator;

	//animator->SetBoolParam("isWalk", true);




	//API::LoadSceneFromData("transformData.json");

	API::LoadScene(_scene);
}
