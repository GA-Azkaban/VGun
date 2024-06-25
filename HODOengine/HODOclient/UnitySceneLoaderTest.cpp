#include "UnitySceneLoaderTest.h"
#include "CameraMove.h"
#include "LobbyManager.h"
#include "PlayerMove.h"
#include "FPAniScript.h"


UnitySceneLoaderTest::UnitySceneLoaderTest()
{

}

UnitySceneLoaderTest::~UnitySceneLoaderTest()
{

}

void UnitySceneLoaderTest::Start()
{
	_scene = API::CreateScene("Scene2");

	HDData::GameObject* mainCam = _scene->GetMainCamera()->GetGameObject();
	mainCam->AddComponent<CameraMove>();
	mainCam->GetTransform()->SetPosition(-20.f, 22.f, 28.f);
	mainCam->GetTransform()->Rotate(30.07, 150.f, 0.f);

/*
	auto playerTP = API::CreateObject(_scene, "playerTP");
	//playerTP->GetComponent<HDData::Transform>()->SetPosition(Vector3{ 0.0f, 3.0f, 0.0f });
	//playerTP->GetTransform()->Translate(5.f, 3.f, 0.f);
	playerTP->GetTransform()->SetLocalPosition(0.0f, -1.0f, 0.0f);
	playerTP->GetTransform()->SetLocalRotationEuler(Vector3(0.0f, 180.0f, 0.0f));
	playerTP->LoadFBXFile("SKM_TP_X_idle.fbx");
*/
	auto playerFP = API::CreateObject(_scene);
	playerFP->GetTransform()->SetPosition(Vector3{ 0, 10, 0 });
	playerFP->LoadFBXFile("SKM_FP_HG_idle");

	auto meshComp = playerFP->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
	HDEngine::MaterialDesc desc;
	desc.materialName = "FP_Red";
	desc.albedo = "FP_Red_B.png";

	HDData::Material* newMat = API::CreateMaterial(desc);
	meshComp->LoadMaterial(newMat, 0);

	playerFP->AddComponent<HDData::Animator>();
	API::LoadFPAnimationFromData(playerFP, "fp_data.json");

	playerFP->AddComponent<FPAniScript>();

	auto hand = playerFP->GetGameObjectByNameInChildren("ik_hand_r");
	auto weaponTest = API::CreateObject(_scene, "weapon", hand);
	weaponTest->GetComponent<HDData::Transform>()->SetLocalPosition(-16.f, -5.0f, -2.f);
	weaponTest->GetComponent<HDData::Transform>()->Rotate(180.0f, 255.0f, 90.0f);
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

	//auto playerTP = API::CreateObject(_scene, "playerTP");
	//playerTP->GetComponent<HDData::Transform>()->SetPosition(Vector3{ 0.0f, -3.0f, 0.0f });
	//playerTP->GetTransform()->Translate(5.f, 3.f, 0.f);
	//playerTP->LoadFBXFile("SKM_TP_X_idle.fbx");

	//auto meshComp = playerTP->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
	//HDEngine::MaterialDesc desc;
	//desc.materialName = "TP_Red";
	//desc.albedo = "TP_Red_B.png";

	//HDData::Material* newMat = API::CreateMaterial(desc);
	//meshComp->LoadMaterial(newMat, 0);
	//meshComp->LoadMaterial(newMat, 1);
	//meshComp->LoadMaterial(newMat, 2);
	//meshComp->LoadMaterial(newMat, 3);
	//meshComp->LoadMaterial(newMat, 4);

	//auto btn = API::CreateButton(_scene);
	//btn->GetComponent<HDData::Button>()->SetOnClickEvent([]() {
	//	LobbyManager::Instance().Test();
	//	});


	//meshComp->PlayAnimationUpper("X_idle", true);
	//meshComp->PlayAnimationLower("X_idle", true);

	//------
	auto playerTest2 = API::CreateObject(_scene, "player2");
	playerTest2->GetComponent<HDData::Transform>()->SetPosition(Vector3{ -10.0f, 0.0f, 0.0f });
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
	playerTest3->GetComponent<HDData::Transform>()->SetPosition(Vector3{ -20.0f, 0.0f, 0.0f });
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
	playerTest4->GetComponent<HDData::Transform>()->SetPosition(Vector3{ -25.0f, 0.0f, -10.0f });
	playerTest4->LoadFBXFile("SKM_TP_X_idle.fbx");

	auto meshComp4 = playerTest4->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
	meshComp4->LoadMaterial(newMat2, 0);
	meshComp4->LoadMaterial(newMat2, 1);
	meshComp4->LoadMaterial(newMat2, 2);
	meshComp4->LoadMaterial(newMat2, 3);
	meshComp4->LoadMaterial(newMat2, 4);
	meshComp4->PlayAnimation("X_idle", true);
	meshComp4->SetOutlineActive(true);

	auto playerTest5 = API::CreateObject(_scene, "player5");
	playerTest5->GetComponent<HDData::Transform>()->SetPosition(Vector3{ 10.0f, 0.f, 0.0f });
	playerTest5->LoadFBXFile("SKM_TP_X_idle.fbx");

	auto meshComp5 = playerTest5->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
	meshComp5->LoadMaterial(newMat3, 0);
	meshComp5->LoadMaterial(newMat3, 1);
	meshComp5->LoadMaterial(newMat3, 2);
	meshComp5->LoadMaterial(newMat3, 3);
	meshComp5->LoadMaterial(newMat3, 4);
	meshComp5->PlayAnimation("X_idle", true);
	meshComp5->SetOutlineActive(true);
	//------
	
	//auto playerFP = API::CreateObject(_scene, "playerFP");
	//playerFP->LoadNodeFromFBXFile("SKM_FP_X_idle.fbx");
	
	//auto meshCompFP = playerFP->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
	//meshCompFP->LoadAlbedoMap("FP_Yellow_A.png");
	//meshCompFP->PlayAnimation("X_idle", true);

	//auto playerTP = API::CreateObject(_scene, "playerTP");
	//playerTP->GetTransform()->Translate(5.f, 3.f, 0.f);
	//playerTP->LoadFBXFile("SKM_TP_X_idle.fbx");
	//playerTP->GetTransform()->Translate(10.f, 0.f, 0.f);

	//// 바닥
	//auto groundFloor = API::CreateObject(_scene, "ground");
	//groundFloor->GetComponent<HDData::Transform>()->SetPosition(0.f, 0.f, 0.f);
	//auto groundCollier = groundFloor->AddComponent<HDData::StaticPlaneCollider>();

	// 플레이어 collider들을 추가해주는 부분
	auto playerCenter = API::CreateObject(_scene, "playerCenter");
	//playerCenter->SetParentObject(playerTP);
	playerFP->SetParentObject(playerCenter);
	playerCenter->GetTransform()->SetPosition(0.0f, 3.0f, 0.0f);
	auto playerCollider = playerCenter->AddComponent<HDData::DynamicBoxCollider>(0.5f, 0.6f, 0.25f, 1);

	auto playerTestHead = playerFP->GetGameObjectByNameInChildren("head");
	playerTestHead->SetParentObject(playerCenter);
	playerTestHead->GetTransform()->SetLocalPosition(Vector3{ 0.0f, 0.4f, 0.0f });
	auto playerHeadCollider = playerTestHead->AddComponent<HDData::DynamicSphereCollider>(0.2f, true);
	playerHeadCollider->SetParentCollider(playerCollider);

	auto plLeftUpperArm = playerFP->GetGameObjectByNameInChildren("upperarm_l");
	plLeftUpperArm->SetParentObject(playerCenter);
	plLeftUpperArm->GetTransform()->SetLocalPosition(Vector3{ -0.3f, 0.1f, 0.0f });
	auto LUArmCollider = plLeftUpperArm->AddComponent<HDData::DynamicBoxCollider>(0.12f, 0.35f, 0.12f, 2);
	LUArmCollider->SetParentCollider(playerCollider);

	auto plLeftForeArm = playerFP->GetGameObjectByNameInChildren("lowerarm_l");
	plLeftForeArm->SetParentObject(plLeftUpperArm);
	plLeftForeArm->GetTransform()->SetLocalPosition(Vector3{ 0.0f, -0.4f, 0.0f });
	auto LFArmCollider = plLeftForeArm->AddComponent<HDData::DynamicBoxCollider>(0.1f, 0.45f, 0.1f, 2);
	LFArmCollider->SetParentCollider(LUArmCollider);

	auto plRightUpperArm = playerFP->GetGameObjectByNameInChildren("upperarm_r");
	plRightUpperArm->SetParentObject(playerCenter);
	plRightUpperArm->GetTransform()->SetLocalPosition(Vector3{ 0.3f, 0.1f, 0.0f });
	auto RUArmCollider = plRightUpperArm->AddComponent<HDData::DynamicBoxCollider>(0.12f, 0.35f, 0.12f, 2);
	RUArmCollider->SetParentCollider(playerCollider);

	auto plRightForeArm = playerFP->GetGameObjectByNameInChildren("lowerarm_r");
	plRightForeArm->SetParentObject(plRightUpperArm);
	plRightForeArm->GetTransform()->SetLocalPosition(Vector3{ 0.0f, -0.4f, 0.0f });
	auto RFArmCollider = plRightForeArm->AddComponent<HDData::DynamicBoxCollider>(0.1f, 0.45f, 0.1f, 2);
	RFArmCollider->SetParentCollider(RUArmCollider);

	auto plLeftThigh = playerFP->GetGameObjectByNameInChildren("thigh_l");
	plLeftThigh->SetParentObject(playerCenter);
	plLeftThigh->GetTransform()->SetLocalPosition(Vector3{ -0.15f, -0.4f, 0.0f });
	auto LThighCollider = plLeftThigh->AddComponent<HDData::DynamicBoxCollider>(0.2f, 0.35f, 0.2f, 2);
	LThighCollider->SetParentCollider(playerCollider);

	auto plLeftLowerLeg = playerFP->GetGameObjectByNameInChildren("calf_l");
	plLeftLowerLeg->SetParentObject(plLeftThigh);
	plLeftLowerLeg->GetTransform()->SetLocalPosition(Vector3{ 0.0f, -0.35f, 0.0f });
	auto LLLegCollider = plLeftLowerLeg->AddComponent<HDData::DynamicBoxCollider>(0.2f, 0.35f, 0.2f, 2);
	LLLegCollider->SetParentCollider(LThighCollider);

	auto plLeftFoot = playerFP->GetGameObjectByNameInChildren("foot_l");
	plLeftFoot->SetParentObject(plLeftLowerLeg);
	plLeftFoot->GetTransform()->SetLocalPosition(Vector3{ 0.0f, -0.25f, 0.05f });
	auto LFootCollider = plLeftFoot->AddComponent<HDData::DynamicBoxCollider>(0.2f, 0.15f, 0.3f, 2);
	LFootCollider->SetParentCollider(LLLegCollider);

	auto plRightThigh = playerFP->GetGameObjectByNameInChildren("thigh_r");
	plRightThigh->SetParentObject(playerCenter);
	plRightThigh->GetTransform()->SetLocalPosition(Vector3{ 0.15f, -0.4f, 0.0f });
	auto RThighCollider = plRightThigh->AddComponent<HDData::DynamicBoxCollider>(0.2f, 0.35f, 0.2f, 2);
	RThighCollider->SetParentCollider(playerCollider);

	auto plRightLowerLeg = playerFP->GetGameObjectByNameInChildren("calf_r");
	plRightLowerLeg->SetParentObject(plRightThigh);
	plRightLowerLeg->GetTransform()->SetLocalPosition(Vector3{ 0.0f, -0.35f, 0.0f });
	auto RLLegCollider = plRightLowerLeg->AddComponent<HDData::DynamicBoxCollider>(0.2f, 0.35f, 0.2f, 2);
	RLLegCollider->SetParentCollider(RThighCollider);

	auto plRightFoot = playerFP->GetGameObjectByNameInChildren("foot_r");
	plRightFoot->SetParentObject(plRightLowerLeg);
	plRightFoot->GetTransform()->SetLocalPosition(Vector3{ 0.0f, -0.25f, 0.05f });
	auto RFootCollider = plRightFoot->AddComponent<HDData::DynamicBoxCollider>(0.2f, 0.15f, 0.3f, 2);
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
	headCamObj->GetTransform()->SetLocalPosition(Vector3{ 0.0f, 0.12f, 0.2f });
	auto headCam = headCamObj->AddComponent<HDData::Camera>();

	auto playerMove = playerCenter->AddComponent<PlayerMove>();
	playerMove->SetPlayerCamera(_scene->GetMainCamera());
	playerMove->SetPlayerText(playerPosText->GetComponent<HDData::TextUI>(), aimText->GetComponent<HDData::TextUI>());
	playerCenter->GetComponent<PlayerMove>()->SetPlayerCamera(_scene->GetMainCamera());
	playerMove->SetHeadCam(headCam);

	auto triggerTest = API::CreateObject(_scene, "triggerTest");
	triggerTest->GetTransform()->SetPosition(Vector3(3.0f, 1.0f, -3.0f));
	auto triggerCol = triggerTest->AddComponent<HDData::TriggerBoxCollider>();


	// 피격 표시 particle
	std::vector<HDData::ParticleSphereCollider*> particleContainer;
	particleContainer.reserve(30);
	for (int i = 0; i < 30; ++i)
	{
		auto particleTest = API::CreateObject(_scene, "particleTest");
		particleTest->GetComponent<HDData::Transform>()->SetPosition(0.f, -5.f, 0.f);
		auto particleCollider = particleTest->AddComponent<HDData::ParticleSphereCollider>();
		particleContainer.push_back(particleCollider);
	}
	playerMove->SetHitParticle(particleContainer);

	// sound 추가
	HDData::AudioSource* playerSound = playerCenter->AddComponent<HDData::AudioSource>();
	playerSound->AddAudio("shoot", "./Resources/Sound/Shoot/Gun_sound6.wav", HDData::SoundGroup::EffectSound);
	playerSound->AddAudio("empty", "./Resources/Sound/Shoot/Gun_sound_empty.wav", HDData::SoundGroup::EffectSound);
	playerSound->AddAudio("hit", "./Resources/Sound/Hit/Hit.wav", HDData::SoundGroup::EffectSound);
	playerSound->AddAudio("jump", "./Resources/Sound/Walk/footfall_01.wav", HDData::SoundGroup::EffectSound);
	playerSound->AddAudio("walk", "./Resources/Sound/Walk/footfall_02.wav", HDData::SoundGroup::EffectSound);
	playerSound->AddAudio("run", "./Resources/Sound/Walk/footfall_02_run.wav", HDData::SoundGroup::EffectSound);
	playerSound->AddAudio("reload", "./Resources/Sound/GunReload/Reload.wav", HDData::SoundGroup::EffectSound);

	// Particle System
	auto particleSystemObj = API::CreateObject(_scene);
	particleSystemObj->GetComponent<HDData::Transform>()->SetPosition(0.0f, 3.0f, 0.0f);
	particleSystemObj->GetComponent<HDData::Transform>()->Rotate(0.0f, 90.0f, 0.0f);
	auto particleSystem = particleSystemObj->AddComponent<HDData::ParticleSystem>();
	particleSystem->main.duration = 0.15f;
	particleSystem->main.loop = true;
	particleSystem->main.minStartColor = { 255, 255, 197, 255 };
	particleSystem->main.maxStartColor = { 255, 255, 255, 255 };
	particleSystem->main.minStartLifetime = 0.025f;
	particleSystem->main.maxStartLifetime = 0.05f;
	particleSystem->main.minStartRotation = -90.0f;
	particleSystem->main.maxStartRotation = -90.0f;
	particleSystem->main.minStartSize = 0.0025f;
	particleSystem->main.maxStartSize = 0.01f;
	particleSystem->main.minStartSpeed = 0.0f;
	particleSystem->main.maxStartSpeed = 0.0f;
	particleSystem->emission.enabled = true;
	HDData::Burst newBurst(0.0f, 1);
	particleSystem->emission.SetBurst(newBurst);
	particleSystem->sizeOverLifetime.enabled = true;
	HDData::AnimationCurve curve;
	curve.AddKey(0.0f, 0.654f, [](float t) { return -9.34 * t * t + 6.11 * t; });
	particleSystem->sizeOverLifetime.size = HDData::MinMaxCurve(1.0f, curve);
	HDEngine::MaterialDesc flashMatDesc;
	flashMatDesc.materialName = "muzzleFlash";
	flashMatDesc.albedo = "T_MuzzleFlash_D.png";
	flashMatDesc.color = { 255, 140, 85, 255 };
	HDData::Material* flashMat = API::CreateMaterial(flashMatDesc);
	particleSystem->rendererModule.renderMode = HDEngine::ParticleSystemRenderMode::Mesh;
	particleSystem->rendererModule.material = flashMat;
	particleSystem->rendererModule.mesh = "SM_MuzzleFlash.fbx";

	auto particleSystemObj2 = API::CreateObject(_scene, "particleSystem2", particleSystemObj);
	auto particleSystem2 = particleSystemObj2->AddComponent<HDData::ParticleSystem>();
	particleSystem2->main.duration = 0.15f;
	particleSystem2->main.loop = true;
	particleSystem2->main.minStartColor = { 255, 113, 36, 255 };
	particleSystem2->main.maxStartColor = { 255, 255, 255, 255 };
	particleSystem2->main.minStartLifetime = 0.001f;
	particleSystem2->main.maxStartLifetime = 0.1f;
	particleSystem2->main.minStartRotation = -250.0f;
	particleSystem2->main.maxStartRotation = 250.0f;
	particleSystem2->main.minStartSize = 0.1f;
	particleSystem2->main.maxStartSize = 0.2f;
	particleSystem2->main.minStartSpeed = 10.0f;
	particleSystem2->main.maxStartSpeed = 15.0f;
	particleSystem2->emission.enabled = true;
	HDData::Burst newBurst2(0.0f, 2, 8);
	particleSystem2->emission.SetBurst(newBurst2);
	particleSystem2->sizeOverLifetime.enabled = true;
	HDData::AnimationCurve curve2;
	curve2.AddKey(0.0f, 1.0f, [](float t) { return -0.6f * t * t + 0.6f * t; });
	particleSystem2->sizeOverLifetime.size = HDData::MinMaxCurve(1.0f, curve2);
	particleSystem2->rotationOverLifetime.enabled = true;
	particleSystem2->rotationOverLifetime.angularVelocity = 750.0f;
	HDEngine::MaterialDesc particleMatDesc;
	particleMatDesc.materialName = "particleMat";
	particleMatDesc.albedo = "T_Sparks_D.png";
	HDData::Material* particleMat = API::CreateMaterial(particleMatDesc);
	particleSystem2->rendererModule.material = particleMat;
	particleSystem2->colorOverLifetime.enabled = true;
	// colorKey, alphaKey 생성
	std::vector<HDData::GradientColorKey> ck;
	std::vector<HDData::GradientAlphaKey> ak;
	HDData::GradientColorKey colorkey1;
	colorkey1.color = { 255, 255, 255 };
	colorkey1.time = 0.556f;
	ck.push_back(colorkey1);
	HDData::GradientColorKey colorkey2;
	colorkey2.color = { 255, 79, 0 };
	colorkey2.time = 1.0f;
	ck.push_back(colorkey2);
	HDData::GradientAlphaKey alphaKey1;
	alphaKey1.alpha = 255;
	alphaKey1.time = 0.0f;
	ak.push_back(alphaKey1);
	HDData::GradientAlphaKey alphaKey2;
	alphaKey2.alpha = 255;
	alphaKey2.time = 1.0f;
	ak.push_back(alphaKey2);
	particleSystem2->colorOverLifetime.color.SetKeys(ck, ak);

	particleSystem->Play();

	//playerTP->AddComponent<HDData::Animator>();
	//API::LoadUpperAnimationFromData(playerTP, "upperdata.json");
	//API::LoadLowerAnimationFromData(playerTP, "lowerdata.json");

	//playerTP->AddComponent<FSMtestScript>();

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


	API::LoadSceneFromData("sceneData.json", this->_scene);

	//API::LoadScene(_scene);
}
