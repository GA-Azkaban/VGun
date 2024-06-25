#include "InGameSceneView.h"
#include "CameraMove.h"
#include "PlayerMove.h"
#include "RoundManager.h"
#include "../HODOEngine/CollisionCallback.h"
#include "MeshTransformController.h"
#include "FPAniScript.h"
#include "PlayerInfo.h"
#include "Crosshair.h"
#include "Ammo.h"
#include "TPScript.h"
#include "OthersAnim.h"
#include "Health.h"

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

	HDEngine::MaterialDesc characterMat;  
	characterMat.materialName = "PolygonWestern_Texture_01_A";
	characterMat.albedo = "PolygonWestern_Texture_01_A.png";
	characterMat.metallic = "PolygonWestern_Texture_Metallic.png";

	HDData::Material* chMat = API::CreateMaterial(characterMat);

	// 그래픽스 디버깅용 카메라 생성
	auto freeRoamingCamObj = API::CreateObject(_scene, "freeRoamingCam");
	auto freeRoamingCam = freeRoamingCamObj->AddComponent<HDData::Camera>();
	freeRoamingCamObj->GetTransform()->SetPosition(-5.0f, 2.0f, -10.0f);
	freeRoamingCamObj->AddComponent<CameraMove>();

	// 내 캐릭터 생성
	std::string objName1 = "playerSelf";
	HDData::GameObject* player = API::CreateObject(_scene, objName1);
	auto playerMove = player->AddComponent<PlayerMove>();
	player->LoadFBXFile("SKM_CowboyTP_X_default.fbx");
	player->GetTransform()->SetPosition(-10, 3, 0);

	auto meshComp = player->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
	meshComp->LoadAnimation("TP");
	meshComp->LoadMaterial(chMat, 0);
	meshComp->PlayAnimation("RV_idle");
	meshComp->SetMeshActive(false, 0);

	RoundManager::Instance()->_myObj = player;

	// 애니메이션 전달용 더미 캐릭터 생성
	HDData::GameObject* dummy = API::CreateObject(_scene, "dummy");
	dummy->LoadFBXFile("SKM_BadguyTP_X_default.fbx");
	dummy->GetTransform()->SetPosition(0, -10, 0);
	dummy->GetComponentInChildren<HDData::SkinnedMeshRenderer>()->LoadAnimation("TP");
	
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

	// 메인 카메라를 1인칭 캐릭터 머리에 붙은 카메라로 사용한다.
	// 메인 카메라에 오디오 리스너 컴포넌트가 붙기 때문
	auto mainCam = _scene->GetMainCamera();
	mainCam->GetGameObject()->SetParentObject(player);
	mainCam->GetGameObject()->GetTransform()->SetLocalPosition(Vector3{ 0.0f, 1.65f, 0.175f });
	playerMove->SetHeadCam(mainCam);
	playerMove->SetPlayerCamera(freeRoamingCam);

	// 1인칭 메쉬 달 오브젝트
	// 카메라에 달려고 했으나 카메라에 달았을 때 이상하게 동작해 메쉬를 카메라와 분리한다.
	auto meshObjShell = API::CreateObject(_scene, "meshShell", player);
	meshObjShell->GetTransform()->SetLocalPosition(Vector3{ 0.0f, 1.65f, 0.170f });


	auto fpMeshObj = API::CreateObject(_scene, "FPMesh", meshObjShell);
	fpMeshObj->LoadFBXFile("SKM_CowboyFP_X_default.fbx");
	fpMeshObj->AddComponent<HDData::Animator>();
	API::LoadFPAnimationFromData(fpMeshObj, "FP_animation.json");
	fpMeshObj->AddComponent<FPAniScript>();

	fpMeshObj->GetTransform()->SetLocalPosition(0.15f, -1.7f, 0.5f);
	auto fpMeshComp = fpMeshObj->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
	fpMeshComp->LoadMesh("SKM_CowgirlFP_X_default.fbx");
	fpMeshComp->LoadAnimation("TP");
	//fpMeshComp->GetTransform()->SetLocalRotation(Quaternion::CreateFromYawPitchRoll(2.8f, 0.4f, 0.0f));
	fpMeshComp->LoadMaterial(chMat, 0);
	fpMeshComp->PlayAnimation("RV_idle", true);

	// 총 생성
	auto hand = fpMeshObj->GetGameObjectByNameInChildren("Thumb_01.001");
	auto weaponTest = API::CreateObject(_scene, "weapon", hand);
	weaponTest->AddComponent<MeshTransformController>();
	weaponTest->GetComponent<HDData::Transform>()->SetLocalPosition(-2.1321f, 9.6180f, 7.9218f);
	weaponTest->GetComponent<HDData::Transform>()->SetLocalRotation({ 0.0453f, -0.6043f, -0.1055f, 0.7881f });
	auto weaponComp = weaponTest->AddComponent<HDData::MeshRenderer>();
	weaponComp->LoadMesh("SM_Wep_Revolver_02.fbx");
	weaponComp->LoadMaterial(chMat, 0);
	weaponComp->LoadMaterial(chMat, 1);
	weaponComp->LoadMaterial(chMat, 2);
	weaponComp->LoadMaterial(chMat, 3);

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
		otherPlayer->LoadFBXFile("SKM_GunManTP_X_default.fbx");
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
		otherMeshComp->LoadAnimation("TP");
		otherMeshComp->LoadMaterial(chMat, 0);
		otherMeshComp->PlayAnimation("RV_idle", true);

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

	int uiX = 130;
	int uiY = 50.0f;

	for (int i = 0; i < 6; ++i)
	{
		// killCount UI
		auto uiBack = API::CreateImageBox(_scene, "back" + std::to_string(i));
		uiBack->GetTransform()->SetPosition(uiX, uiY, 0);
		uiBack->GetTransform()->SetScale(1, 3, 0);
		uiBack->GetComponent<HDData::ImageUI>()->SetSortOrder(0.6);
		uiBack->GetComponent<HDData::ImageUI>()->SetImage("back.png");

		auto nickname = API::CreateTextbox(_scene, "nick" + std::to_string(i));
		nickname->GetTransform()->SetPosition(uiX-40, uiY, 0);
		auto nickComp = nickname->GetComponent<HDData::TextUI>();
		nickComp->SetColor(DirectX::Colors::Black);
		nickComp->SetText("");
		nickComp->SetSortOrder(0.7);

		auto killcount = API::CreateTextbox(_scene, "count" + std::to_string(i));
		killcount->GetTransform()->SetPosition(uiX + 25, uiY, 0);
		auto countComp = killcount->GetComponent<HDData::TextUI>();
		countComp->SetColor(DirectX::Colors::Black);
		countComp->SetText("");
		countComp->SetSortOrder(0.7);

		RoundManager::Instance()->SetKillCountUI(nickComp, countComp, i);

		uiY += 60;
	}

	// crosshair
	auto crosshairObj = API::CreateObject(_scene, "Crosshair");
	auto crosshairComp = crosshairObj->AddComponent<Crosshair>();
	crosshairComp->playerMove = playerMove;

	// ammo
	auto ammo = API::CreateObject(_scene, "remaingAmmo");
	auto ammoComp = ammo->AddComponent<Ammo>();
	HDData::GameObject* defaultAmmo = API::CreateTextbox(_scene, "Ammo");
	defaultAmmo->GetComponent<HDData::TextUI>()->GetTransform()->SetPosition(2400.0f, 1400.0f, 0.0f);
	defaultAmmo->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_55.spriteFont");
	defaultAmmo->GetComponent<HDData::TextUI>()->SetText("/ 6");

	// HP
	HDData::GameObject* healthPoint = API::CreateObject(_scene, "healthPoint");
	healthPoint->AddComponent<Health>();

	// Timer
	auto timer = API::CreateTextbox(_scene, "timer");
	RoundManager::Instance()->SetRoundTimerObject(timer->GetComponent<HDData::TextUI>());

	API::LoadSceneFromData("sceneData.json", this->_scene);
}

std::string& InGameSceneView::convertBullet(int bullet)
{
	_tempBullet = std::to_string(bullet);
	return _tempBullet;
}
