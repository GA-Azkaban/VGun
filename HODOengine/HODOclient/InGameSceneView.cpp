﻿#include "InGameSceneView.h"
#include "CameraMove.h"
#include "PlayerMove.h"
#include "RoundManager.h"
#include "../HODOEngine/CollisionCallback.h"
#include "FPAniScript.h"
#include "PlayerInfo.h"
#include "Crosshair.h"
#include "Ammo.h"
#include "TPScript.h"
#include "OthersAnim.h"
#include "LowHPEffect.h"
#include "HitEffect.h"
#include "IndicatorPool.h"

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

	// 게임엔딩 카메라
	auto gameendCam = API::CreateObject(_scene, "endCam");
	gameendCam->GetTransform()->SetPosition(59.25668f, 2.73707f, -34.79806f);
	gameendCam->GetTransform()->SetRotation(0.077f, -0.701f, 0.07f, 0.74f);
	auto gameendCamcomp = gameendCam->AddComponent<HDData::Camera>();

	RoundManager::Instance()->SetEndCam(gameendCam);

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

	player->AddComponent<HDData::Animator>();
	API::LoadFPAnimationFromData(player, "TP_animation.json");
	player->AddComponent<TPScript>();
	RoundManager::Instance()->SetAnimationDummy(player);

	RoundManager::Instance()->_myObj = player;

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

	RoundManager::Instance()->SetStartCam(mainCam);

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
	fpMeshComp->SetShadowActive(false);
	fpMeshComp->PlayAnimation("RV_idle", true);

	// 총 생성
	auto hand = fpMeshObj->GetGameObjectByNameInChildren("Thumb_01.001");
	auto weaponTest = API::CreateObject(_scene, "weapon", hand);
	weaponTest->GetComponent<HDData::Transform>()->SetLocalPosition(-2.1321f, 9.6180f, 7.9218f);
	weaponTest->GetComponent<HDData::Transform>()->SetLocalRotation({ 0.0453f, -0.6043f, -0.1055f, 0.7881f });
	auto weaponComp = weaponTest->AddComponent<HDData::MeshRenderer>();
	weaponComp->LoadMesh("SM_Wep_Revolver_02.fbx");
	weaponComp->LoadMaterial(chMat, 0);
	weaponComp->LoadMaterial(chMat, 1);
	weaponComp->LoadMaterial(chMat, 2);
	weaponComp->LoadMaterial(chMat, 3);
	weaponComp->SetShadowActive(false);

	// 총구 이펙트
	auto particleSystemObj = API::CreateObject(_scene, "effect");
	auto particleSystem = particleSystemObj->AddComponent<HDData::ParticleSystem>();
	particleSystem->main.duration = 0.2f;
	particleSystem->main.loop = true;
	particleSystem->main.minStartColor = { 255, 255, 197, 255 };
	particleSystem->main.maxStartColor = { 255, 255, 255, 255 };
	particleSystem->main.minStartLifetime = 0.025f;
	particleSystem->main.maxStartLifetime = 0.05f;
	particleSystem->main.minStartRotation = -90.0f;
	particleSystem->main.maxStartRotation = -90.0f;
	particleSystem->main.minStartSize = 0.05f;
	particleSystem->main.maxStartSize = 0.075f;
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
	particleSystem->colorOverLifetime.color.SetKeys(ck, ak);

	particleSystem->Play();

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
	//playerSound->AddAudio3D("shoot", "./Resources/Sound/Shoot/Gun_sound6.wav", HDData::SoundGroup::GunSound, 5.0f, 30.0f);
	//playerSound->AddAudio3D("empty", "./Resources/Sound/Shoot/Gun_sound_empty.wav", HDData::SoundGroup::GunSound, 5.0f, 30.0f);
	//playerSound->AddAudio3D("reload", "./Resources/Sound/GunReload/Reload.wav", HDData::SoundGroup::GunSound, 5.0f, 30.0f);
	//playerSound->AddAudio3D("jump", "./Resources/Sound/Walk/footfall_01.wav", HDData::SoundGroup::MoveSound, 5.0f, 30.0f);
	//playerSound->AddAudio3D("walk", "./Resources/Sound/Walk/footfall_02.wav", HDData::SoundGroup::SoundGroupChannel5, 5.0f, 30.0f);
	//playerSound->AddAudio3D("run", "./Resources/Sound/Walk/footfall_02_run.wav", HDData::SoundGroup::SoundGroupChannel5, 5.0f, 30.0f);
	//playerSound->AddAudio3D("tumble", "./Resources/Sound/Tumble/tumble.wav", HDData::SoundGroup::MoveSound, 5.0f, 100.0f);
	//playerSound->AddAudio3D("tumblingMan", "./Resources/Sound/Tumble/tumblingMan.wav", HDData::SoundGroup::ActionSound, 5.0f, 30.0f);
	//playerSound->AddAudio3D("dance", "./Resources/Sound/Dance/danceMusic.wav", HDData::SoundGroup::ActionSound, 5.0f, 30.0f);
	//playerSound->AddAudio3D("hit", "./Resources/Sound/Hit/Hit.wav", HDData::SoundGroup::EffectSound, 5.0f, 30.0f);
	//playerSound->SetSoundGroupVolume(HDData::SoundGroup::SoundGroupChannel5, 1000.0f);
	playerSound->AddAudio("shoot", "./Resources/Sound/Shoot/Gun_sound7-2.wav", HDData::SoundGroup::GunSound);
	playerSound->AddAudio("shoot2", "./Resources/Sound/Shoot/Gun_sound9.wav", HDData::SoundGroup::GunSound);
	playerSound->AddAudio("empty", "./Resources/Sound/Shoot/Gun_sound_empty.wav", HDData::SoundGroup::GunSound);
	playerSound->AddAudio("reload", "./Resources/Sound/GunReload/Reload2.wav", HDData::SoundGroup::GunSound);
	playerSound->AddAudio("jump", "./Resources/Sound/Walk/footfall_01.wav", HDData::SoundGroup::MoveSound);
	playerSound->AddAudio("land", "./Resources/Sound/Jump&Land/landing2.wav", HDData::SoundGroup::MoveSound);
	playerSound->AddAudio("walk", "./Resources/Sound/Walk/footfall_02.wav", HDData::SoundGroup::MoveSound);
	playerSound->AddAudio("run", "./Resources/Sound/Walk/footfall_02_run.wav", HDData::SoundGroup::MoveSound);
	playerSound->AddAudio("tumble", "./Resources/Sound/Tumble/tumble_large.wav", HDData::SoundGroup::MoveSound);
	playerSound->AddAudio("tumblingMan", "./Resources/Sound/Tumble/tumblingMan.wav", HDData::SoundGroup::ActionSound);
	playerSound->AddAudio("dance", "./Resources/Sound/Dance/danceMusic.wav", HDData::SoundGroup::ActionSound);
	playerSound->AddAudio("hitBody", "./Resources/Sound/Hit/hitBody3.wav", HDData::SoundGroup::EffectSound);
	playerSound->AddAudio("hitHead", "./Resources/Sound/Hit/hitHead2.wav", HDData::SoundGroup::EffectSound);
	playerSound->AddAudio("bgm", "./Resources/Sound/BGM/FunnyBGM.wav", HDData::SoundGroup::BackgroundMusic);

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
	auto ammo = API::CreateObject(_scene, "remainAmmo");
	auto ammoComp = ammo->AddComponent<Ammo>();
	HDData::GameObject* defaultAmmo = API::CreateTextbox(_scene, "Ammo");
	defaultAmmo->GetComponent<HDData::TextUI>()->GetTransform()->SetPosition(2400.0f, 1400.0f, 0.0f);
	defaultAmmo->GetComponent<HDData::TextUI>()->SetFont("Resources/Font/KRAFTON_55.spriteFont");
	defaultAmmo->GetComponent<HDData::TextUI>()->SetText("/ 6");

	// HP
	HDData::GameObject* healthPoint = API::CreateTextbox(_scene, "healthPoint");
	auto hpTxt = healthPoint->GetComponent<HDData::TextUI>();
	hpTxt->SetFont("Resources/Font/KRAFTON_55.spriteFont");
	hpTxt->GetTransform()->SetPosition(2100.0f, 1400.0f, 0.0f);
	RoundManager::Instance()->SetHPObject(hpTxt);

	// Timer
	auto timer = API::CreateTextbox(_scene, "timer");
	RoundManager::Instance()->SetRoundTimerObject(timer->GetComponent<HDData::TextUI>());
	
	// 디버그용 state
	HDData::GameObject* state = API::CreateTextbox(_scene, "state");
	HDData::TextUI* stateText = state->GetComponent<HDData::TextUI>();
	stateText->SetFont("Resources/Font/KRAFTON_55.spriteFont");
	stateText->GetTransform()->SetPosition(200.0f, 1400.0f, 0.0f);
	playerMove->_plStateText = stateText;

	// 디버그용 cooltime
	HDData::GameObject* tumbleCooltime = API::CreateTextbox(_scene, "coolTime");
	HDData::TextUI* coolTimeText = tumbleCooltime->GetComponent<HDData::TextUI>();
	coolTimeText->SetFont("Resources/Font/KRAFTON_55.spriteFont");
	coolTimeText->GetTransform()->SetPosition(200.0f, 1300.0f, 0.0f);
	playerMove->_tumbleText = coolTimeText;

	// 디버그용 포지션
	HDData::GameObject* plPos = API::CreateTextbox(_scene, "plPos");
	HDData::TextUI* posText = plPos->GetComponent<HDData::TextUI>();
	posText->SetFont("Resources/Font/KRAFTON_55.spriteFont");
	posText->GetTransform()->SetPosition(2300.0f, 50.0f, 0.0f);
	playerMove->_plPosText = posText;

	/// game end

	auto endButton = API::CreateButton(_scene, "endBtn");
	endButton->GetTransform()->SetPosition(1350.0f, 1200.0f, 0.0f);
	auto endComp = endButton->GetComponent<HDData::Button>();
	endComp->SetImage("start.png");
	endComp->SetOnClickEvent([=]()
		{
			RoundManager::Instance()->ExitGame();
		});

	auto endText = API::CreateTextbox(_scene, "endTXT", endButton);
	endText->GetTransform()->SetPosition(endButton->GetTransform()->GetPosition());
	auto endTXTcomp = endText->GetComponent<HDData::TextUI>();
	endTXTcomp->SetText("EXIT GAME");
	endTXTcomp->SetFont("Resources/Font/KRAFTON_55.spriteFont");

	endButton->SetSelfActive(false);

	RoundManager::Instance()->SetRoundEndButton(endButton);

	// 우승자
	auto winnerObj = API::CreateObject(_scene, "winner");
	winnerObj->LoadFBXFile("SKM_GunManTP_X_default.fbx");
	winnerObj->GetTransform()->SetPosition(53, 0, -35);
	winnerObj->GetTransform()->Rotate(0, 90, 0);
	winnerObj->GetComponentInChildren<HDData::SkinnedMeshRenderer>()->LoadAnimation("TP");
	winnerObj->GetComponentInChildren<HDData::SkinnedMeshRenderer>()->PlayAnimation("RV_sillyDancing");
	winnerObj->GetComponentInChildren<HDData::SkinnedMeshRenderer>()->LoadMaterial(chMat, 0);

	auto winnerName = API::CreateTextbox(_scene, "winner");
	winnerName->GetTransform()->SetPosition(1000.0f, 900.0f, 0.0f);
	auto winnerComp = winnerName->GetComponent<HDData::TextUI>();
	winnerComp->SetFont("Resources/Font/KRAFTON_55.spriteFont");
	winnerComp->SetColor(DirectX::Colors::Red);
	
	RoundManager::Instance()->SetWinnerText(winnerComp);

	winnerName->SetSelfActive(false);

	// 루저들
	float loserX = 100.f;

	for (int i = 0; i < 5; ++i)
	{
		auto loserName = API::CreateTextbox(_scene, "loser" + std::to_string(i));
		loserName->GetTransform()->SetPosition(loserX, 400.0f, 0.0f);

		auto loserComp = loserName->GetComponent<HDData::TextUI>();
		loserComp->SetFont("Resources/Font/KRAFTON_30.spriteFont");
		loserComp->SetColor(DirectX::Colors::Black);

		RoundManager::Instance()->SetLoserText(loserComp, i);

		loserName->SetSelfActive(false);

		loserX += 500;
	}

	// low hp screen effect
	auto hpEffectObj = API::CreateObject(_scene, "LowHPEffect");
	hpEffectObj->AddComponent<LowHPEffect>();

	auto hitEffectObj = API::CreateObject(_scene, "HitEffect");
	hitEffectObj->AddComponent<HitEffect>();

	IndicatorPool::Instance().player = player;

	API::LoadSceneFromData("sceneData.json", this->_scene);
}

std::string& InGameSceneView::convertBullet(int bullet)
{
	_tempBullet = std::to_string(bullet);
	return _tempBullet;
}
