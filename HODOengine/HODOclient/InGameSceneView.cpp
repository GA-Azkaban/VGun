﻿#include "InGameSceneView.h"
#include "CameraMove.h"
#include "PlayerMove.h"
#include "FSMtestScript.h"
#include "RoundManager.h"
#include "../HODOEngine/CollisionCallback.h"
#include "MeshTransformController.h"
#include "FPAniScript.h"
#include "PlayerInfo.h"
#include "Crosshair.h"
#include "Ammo.h"
#include "TPScript.h"
#include "OthersAnim.h"

InGameSceneView::InGameSceneView()
{

}

InGameSceneView::~InGameSceneView()
{

}

void InGameSceneView::Initialize()
{
	_scene = API::CreateScene("InGame");

	RoundManager::Instance()->SetRoundScene(_scene);

	// 플레이어 여섯 명 렌더링
	float posX = 0;
	float posT = 165;

	HDEngine::MaterialDesc red;
	red.materialName = "TP_Red";
	red.albedo = "TP_Red_B.png";

	HDData::Material* M_Red = API::CreateMaterial(red);

	// 그래픽스 디버깅용 카메라 생성
	auto freeRoamingCamObj = API::CreateObject(_scene, "freeRoamingCam");
	auto freeRoamingCam = freeRoamingCamObj->AddComponent<HDData::Camera>();
	freeRoamingCamObj->GetTransform()->SetPosition(-5.0f, 2.0f, -10.0f);
	freeRoamingCamObj->AddComponent<CameraMove>();

	// 내 캐릭터 생성
	HDData::GameObject* player = API::CreateObject(_scene, "playerSelf");
	player->LoadFBXFile("SKM_TP_X_Default.fbx");
	player->GetTransform()->SetPosition(-10, 0, 0);

	auto meshComp = player->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
	meshComp->LoadMaterial(M_Red, 0);
	meshComp->LoadMaterial(M_Red, 1);
	meshComp->LoadMaterial(M_Red, 2);
	meshComp->LoadMaterial(M_Red, 3);
	meshComp->LoadMaterial(M_Red, 4);
	meshComp->SetMeshActive(false, 0);
	meshComp->SetMeshActive(false, 1);
	meshComp->SetMeshActive(false, 2);
	meshComp->SetMeshActive(false, 3);
	meshComp->SetMeshActive(false, 4);

	meshComp->PlayAnimation("AR_aim", true);
	RoundManager::Instance()->_myObj = player;

	// 애니메이션 전달용 더미 캐릭터 생성
	HDData::GameObject* dummy = API::CreateObject(_scene, "dummy");
	dummy->LoadFBXFile("SKM_TP_X_Default.fbx");
	dummy->GetTransform()->SetPosition(0, -10, 0);
	
	dummy->AddComponent<HDData::Animator>();
	API::LoadFPAnimationFromData(dummy, "TP_animation.json");
	dummy->AddComponent<TPScript>();

	RoundManager::Instance()->SetAnimationDummy(dummy);

	auto playerCollider = player->AddComponent<HDData::DynamicCapsuleCollider>(0.26f, 0.6f);
	playerCollider->SetPositionOffset({ 0.0f, 0.43f, 0.0f });
	playerCollider->SetFreezeRotation(true);
	auto playerHead = API::CreateObject(_scene, "head", player);
	playerHead->GetTransform()->SetLocalPosition(Vector3(0.0f, 1.65f, 0.0f));
	auto headCollider = playerHead->AddComponent<HDData::DynamicSphereCollider>(0.15f);
	headCollider->SetParentCollider(playerCollider);
	headCollider->SetPositionOffset(Vector3(0.0f, -1.1f, 0.0f));
	//headCollider->SetScaleOffset(Vector3(0.4f, 0.4f, 0.4f));

	// 메인 카메라를 1인칭 캐릭터 머리에 붙은 카메라로 사용한다.
	// 메인 카메라에 오디오 리스너 컴포넌트가 붙기 때문
	auto mainCam = _scene->GetMainCamera();
	mainCam->GetGameObject()->SetParentObject(player);
	mainCam->GetGameObject()->GetTransform()->SetLocalPosition(Vector3{ 0.0f, 1.65f, 0.175f });
	//mainCam->GetGameObject()->AddComponent<HDData::StaticBoxCollider>();
	
	auto playerMove = player->AddComponent<PlayerMove>();
	playerMove->SetPlayerCamera(freeRoamingCam);
	playerMove->SetHeadCam(mainCam);
	//playerMove->SetPlayerColliders(playerColliderStanding, playerColliderSitting);
	headCollider->SetParentCollider(playerCollider);

	// 1인칭 메쉬 달 오브젝트
	// 카메라에 달려고 했으나 카메라에 달았을 때 이상하게 동작해 메쉬를 카메라와 분리한다.
	auto meshObjShell = API::CreateObject(_scene, "meshShell", player);
	meshObjShell->GetTransform()->SetLocalPosition(Vector3{ 0.0f, 1.65f, 0.170f });


	auto fpMeshObj = API::CreateObject(_scene, "FPMesh", meshObjShell);
	fpMeshObj->LoadFBXFile("SKM_TP_X_Default.fbx");
	fpMeshObj->AddComponent<HDData::Animator>();
	API::LoadFPAnimationFromData(fpMeshObj, "FP_animation.json");
	fpMeshObj->AddComponent<FPAniScript>();

	fpMeshObj->GetTransform()->SetLocalPosition(0.05f, -1.7f, 0.45f);
	auto fpMeshComp = fpMeshObj->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
	fpMeshComp->GetTransform()->SetLocalRotation(Quaternion::CreateFromYawPitchRoll(2.8f, 0.4f, 0.0f));
	fpMeshComp->LoadMaterial(M_Red, 0);
	fpMeshComp->LoadMaterial(M_Red, 1);
	fpMeshComp->LoadMaterial(M_Red, 2);
	fpMeshComp->LoadMaterial(M_Red, 3);
	fpMeshComp->LoadMaterial(M_Red, 4);
	fpMeshComp->SetMeshActive(false, 0);
	fpMeshComp->SetMeshActive(false, 1);
	fpMeshComp->SetMeshActive(false, 3);
	fpMeshComp->SetMeshActive(false, 4);

	//fpMeshComp->PlayAnimation("AR_aim", true);

	// 총 생성
	auto hand = fpMeshObj->GetGameObjectByNameInChildren("hand_r");
	auto weaponTest = API::CreateObject(_scene, "weapon", hand);
	weaponTest->AddComponent<MeshTransformController>();
	weaponTest->GetComponent<HDData::Transform>()->SetLocalPosition(-17.4141f, -5.2570f, -1.595f);
	weaponTest->GetComponent<HDData::Transform>()->SetLocalRotation({ -0.5467f, 0.5239f, -0.4370f, 0.4849f });
	// AJY 24.6.3.
	weaponTest->GetTransform()->SetLocalPosition(Vector3(38.5f, 4.73f, -17.7f));
	weaponTest->GetTransform()->SetLocalRotation(Quaternion(-0.5289f, 0.4137f, -0.4351f, 0.5997f));

	// weapon
	auto weaponComp = weaponTest->AddComponent<HDData::MeshRenderer>();
	weaponComp->LoadMesh("SM_AR_01.fbx");
	HDEngine::MaterialDesc weaponMatDesc;
	weaponMatDesc.materialName = "M_WEP_Basic_039";
	weaponMatDesc.albedo = "T_WEP_Basic_004_D.png";
	weaponMatDesc.roughness = "T_WEP_Basic_R.png";
	weaponMatDesc.metallicValue = 0.15f;
	HDData::Material* weaponMat1 = API::CreateMaterial(weaponMatDesc);
	HDEngine::MaterialDesc weaponMatDesc2;
	weaponMatDesc2.materialName = "M_WEP_Camo_001";
	weaponMatDesc2.albedo = "T_WEP_Basic_004_D.png";
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
	weaponComp->LoadMaterial(weaponMat3, 4);

	auto playerInfo = player->AddComponent<PlayerInfo>();

	std::vector<HDData::ParticleSphereCollider*> particleVec;
	for (int i = 0; i < 30; ++i)
	{
		auto particleObj = API::CreateObject(_scene);
		auto particle = particleObj->AddComponent<HDData::ParticleSphereCollider>();
		particle->SetScaleOffset(Vector3(0.01f, 0.01f, 0.01f));
		particleVec.push_back(particle);
	}
	playerMove->SetHitParticle(particleVec);

	// sound 추가
	HDData::AudioSource* playerSound = player->AddComponent<HDData::AudioSource>();
	playerSound->AddAudio("shoot", "./Resources/Sound/Shoot/Gun_sound6.wav", HDData::SoundGroup::EffectSound);
	playerSound->AddAudio("empty", "./Resources/Sound/Shoot/Gun_sound_empty.wav", HDData::SoundGroup::EffectSound);
	playerSound->AddAudio("hit", "./Resources/Sound/Hit/Hit.wav", HDData::SoundGroup::EffectSound);
	playerSound->AddAudio("jump", "./Resources/Sound/Walk/footfall_01.wav", HDData::SoundGroup::EffectSound);
	playerSound->AddAudio("walk", "./Resources/Sound/Walk/footfall_02.wav", HDData::SoundGroup::EffectSound);
	playerSound->AddAudio("run", "./Resources/Sound/Walk/footfall_02_run.wav", HDData::SoundGroup::EffectSound);
	playerSound->AddAudio("reload", "./Resources/Sound/GunReload/Reload.wav", HDData::SoundGroup::EffectSound);

	posX += 1;
	posT += 315;

	// 상대방 캐릭터 생성
	for (int i = 1; i < 6; ++i)
	{
		std::string otherObjName = "otherPlayer" + std::to_string(i);
		HDData::GameObject* otherPlayer = API::CreateObject(_scene, otherObjName);
		otherPlayer->LoadFBXFile("SKM_TP_X_Default.fbx");
		otherPlayer->GetTransform()->SetPosition(posX, 0, 0);
		auto otherPlayerCollider = otherPlayer->AddComponent<HDData::DynamicCapsuleCollider>(0.26f, 0.6f);
		otherPlayerCollider->SetPositionOffset({ 0.0f, 0.43f, 0.0f });
		otherPlayerCollider->SetFreezeRotation(true);
		auto otherPlayerHead = API::CreateObject(_scene, otherObjName + "Head", otherPlayer);
		otherPlayerHead->GetTransform()->SetLocalPosition(Vector3(0.0f, 1.65f, 0.0f));
		auto ohterPlayerHeadCollider = otherPlayerHead->AddComponent<HDData::DynamicSphereCollider>(0.15f);
		ohterPlayerHeadCollider->SetParentCollider(otherPlayerCollider);
		ohterPlayerHeadCollider->SetPositionOffset(Vector3(0.0f, -1.1f, 0.0f));
		//ohterPlayerHeadCollider->SetScaleOffset(Vector3(0.4f, 0.4f, 0.4f));

		auto otherMeshComp = otherPlayer->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
		otherMeshComp->GetTransform()->SetLocalPosition(0.0f, -0.1f, 0.0f);
		otherMeshComp->LoadMaterial(M_Red, 0);
		otherMeshComp->LoadMaterial(M_Red, 1);
		otherMeshComp->LoadMaterial(M_Red, 2);
		otherMeshComp->LoadMaterial(M_Red, 3);
		otherMeshComp->LoadMaterial(M_Red, 4);
		otherMeshComp->PlayAnimation("AR_idle", true);

		otherPlayer->AddComponent<OthersAnim>();

		RoundManager::Instance()->_playerObjs.push_back(otherPlayer);

		// sound 추가
		HDData::AudioSource* otherPlayerSound = otherPlayer->AddComponent<HDData::AudioSource>();
		otherPlayerSound->AddAudio("shoot", "./Resources/Sound/Shoot/Gun_sound6.wav", HDData::SoundGroup::EffectSound);
		otherPlayerSound->AddAudio("empty", "./Resources/Sound/Shoot/Gun_sound_empty.wav", HDData::SoundGroup::EffectSound);
		otherPlayerSound->AddAudio("hit", "./Resources/Sound/Hit/Hit.wav", HDData::SoundGroup::EffectSound);
		otherPlayerSound->AddAudio("jump", "./Resources/Sound/Walk/footfall_01.wav", HDData::SoundGroup::EffectSound);
		otherPlayerSound->AddAudio("walk", "./Resources/Sound/Walk/footfall_02.wav", HDData::SoundGroup::EffectSound);
		otherPlayerSound->AddAudio("run", "./Resources/Sound/Walk/footfall_02_run.wav", HDData::SoundGroup::EffectSound);
		otherPlayerSound->AddAudio("reload", "./Resources/Sound/GunReload/Reload.wav", HDData::SoundGroup::EffectSound);

		posX += 2;
		posT += 315;
	}

	// crosshair
	auto crosshairObj = API::CreateObject(_scene, "Crosshair");
	auto crosshairComp = crosshairObj->AddComponent<Crosshair>();
	crosshairComp->playerMove = playerMove;

	// ammo
	auto ammo = API::CreateObject(_scene, "remaingAmmo");
	auto ammoComp = ammo->AddComponent<Ammo>();
	ammoComp->playerMove = playerMove;
	HDData::GameObject* defaultAmmo = API::CreateTextbox(_scene, "Ammo");
	defaultAmmo->GetComponent<HDData::TextUI>()->GetTransform()->SetPosition(2400.0f, 1400.0f, 0.0f);
	defaultAmmo->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_55.spriteFont");
	defaultAmmo->GetComponent<HDData::TextUI>()->SetText("/ 30");

	// HP
	HDData::GameObject* healthPoint = API::CreateTextbox(_scene, "healthPoint");
	healthPoint->GetComponent < HDData::TextUI >()->GetTransform()->SetPosition(1800.0f, 1400.0f, 0.0f);
	healthPoint->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_55.spriteFont");
	healthPoint->GetComponent<HDData::TextUI>()->SetText(std::to_string(playerInfo->GetPlayerCurrentHP()));

	API::LoadSceneFromData("sceneData.json", this->_scene);
}

std::string& InGameSceneView::convertBullet(int bullet)
{
	_tempBullet = std::to_string(bullet);
	return _tempBullet;
}
