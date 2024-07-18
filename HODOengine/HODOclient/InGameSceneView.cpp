#include "InGameSceneView.h"
#include "CameraMove.h"
#include "PlayerMove.h"
#include "RoundManager.h"
#include "../HODOEngine/CollisionCallback.h"
#include "PlayerInfo.h"
#include "Crosshair.h"
#include "TPScript.h"
#include "OthersAnim.h"
#include "LowHPEffect.h"
#include "HitEffect.h"
#include "IndicatorPool.h"
#include "CloudRotate.h"
#include "UIEffect.h"
#include "GameManager.h"

#include "BtnTextScript.h"
#include "CooldownAlpha.h"
#include "CooldownText.h"
#include "MeshTransformController.h"
#include "UITransformController.h"

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

	auto soundObj = API::CreateObject(_scene, "soundObj");
	soundObj->AddComponent<HDData::AudioSource>();

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
	//playerMove->SetMovable(false);
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
	playerHead->GetTransform()->SetLocalPosition(Vector3(0.0f, 1.6f, 0.05f));
	auto headCollider = playerHead->AddComponent<HDData::DynamicSphereCollider>(0.15f);
	headCollider->SetParentCollider(playerCollider);
	//headCollider->SetPositionOffset(Vector3(0.0f, -1.1f, 0.0f));
	headCollider->SetPositionOffset(Vector3(0.0f, -0.6f, 0.0f));
	auto landingHelper = API::CreateObject(_scene, "landingHelper", player);
	landingHelper->GetTransform()->SetLocalPosition(Vector3(0.0f, -0.1f, 0.0f));
	auto helperBox = landingHelper->AddComponent<HDData::TriggerBoxCollider>(0.26f, 0.14f, 0.26f);
	helperBox->SetParentCollider(playerCollider);

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
	meshObjShell->GetTransform()->SetLocalPosition(Vector3{ 0.20f, 1.65f, 0.1f });

	auto fpMeshObj = API::CreateObject(_scene, "FPMesh", meshObjShell);
	fpMeshObj->LoadFBXFile("SKM_CowboyFP_X_default.fbx");
	fpMeshObj->AddComponent<HDData::Animator>();
	API::LoadFPAnimationFromData(fpMeshObj, "FP_animation.json");

	fpMeshObj->GetTransform()->SetLocalPosition(0.15f, -1.7f, 0.5f);
	fpMeshObj->GetTransform()->SetLocalRotation(-0.0925f, -0.0168f, 0.0014f, 0.9955f);
	auto fpMeshComp = fpMeshObj->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
	fpMeshComp->LoadMesh("SKM_CowgirlFP_X_default.fbx");
	fpMeshComp->LoadAnimation("TP");
	fpMeshComp->LoadMaterial(chMat, 0);
	fpMeshComp->SetShadowActive(false);
	fpMeshComp->PlayAnimation("RV_idle", true);

	// 총 생성
	auto hand = fpMeshObj->GetGameObjectByNameInChildren("Thumb_01.001");
	auto weaponTest = API::CreateObject(_scene, "weapon", hand);
	weaponTest->GetComponent<HDData::Transform>()->SetLocalPosition(-2.892f, 8.082f, 7.923f);
	weaponTest->GetComponent<HDData::Transform>()->SetLocalRotation({ 0.061f, -0.606f, -0.093f, 0.787f });
	auto weaponComp = weaponTest->AddComponent<HDData::MeshRenderer>();
	weaponComp->LoadMesh("SM_Wep_Revolver_02.fbx");
	weaponComp->LoadMaterial(chMat, 0);
	weaponComp->LoadMaterial(chMat, 1);
	weaponComp->LoadMaterial(chMat, 2);
	weaponComp->LoadMaterial(chMat, 3);
	weaponComp->SetShadowActive(false);

	// 총구 이펙트
	auto particleSystemObj = API::CreateObject(_scene, "ParticleEffect", weaponTest);
	particleSystemObj->GetTransform()->SetLocalScale(20.0f, 20.0f, 20.0f);
	particleSystemObj->GetTransform()->SetLocalPosition(0.0f, 40.5722f, 6.8792f);
	particleSystemObj->GetTransform()->SetLocalRotation(-0.6989f, 0.0f, 0.0f, -0.7150f);
	auto particleSystem = particleSystemObj->AddComponent<HDData::ParticleSystem>();
	particleSystem->main.duration = 0.08f;
	particleSystem->main.loop = false;
	particleSystem->main.minStartColor = { 255, 255, 197, 255 };
	particleSystem->main.maxStartColor = { 255, 255, 255, 255 };
	particleSystem->main.minStartLifetime = 0.04f;
	particleSystem->main.maxStartLifetime = 0.08f;
	particleSystem->main.minStartRotation = 90.0f;
	particleSystem->main.maxStartRotation = 90.0f;
	particleSystem->main.minStartSize = 0.05f;
	particleSystem->main.maxStartSize = 0.075f;
	particleSystem->main.minStartSpeed = 0.0f;
	particleSystem->main.maxStartSpeed = 0.0f;
	particleSystem->emission.enabled = true;
	HDData::Burst newBurst(0.0f, 1);
	particleSystem->emission.SetBurst(newBurst);
	particleSystem->sizeOverLifetime.enabled = true;
	HDData::AnimationCurve curve;
	curve.AddKey(0.0f, 1.0f, [](float t) { return -3.8f * t * t + 3.7f * t + 0.1f; });
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
	playerSound->AddAudio("shoot", "./Resources/Sound/Shoot/Gun_sound7-2.wav", HDData::SoundGroup::GunSound);
	playerSound->AddAudio("shoot2", "./Resources/Sound/Shoot/Gun_sound9.wav", HDData::SoundGroup::GunSound);
	playerSound->AddAudio("empty", "./Resources/Sound/Shoot/Gun_sound_empty.wav", HDData::SoundGroup::GunSound);
	playerSound->AddAudio("reload", "./Resources/Sound/GunReload/Reload3.wav", HDData::SoundGroup::GunSound);
	playerSound->AddAudio("jump", "./Resources/Sound/Walk/footfall_01.wav", HDData::SoundGroup::MoveSound);
	playerSound->AddAudio("land", "./Resources/Sound/Jump&Land/landing2.wav", HDData::SoundGroup::MoveSound);
	playerSound->AddAudio("walk", "./Resources/Sound/Walk/footfall_02.wav", HDData::SoundGroup::MoveSound);
	playerSound->AddAudio("run", "./Resources/Sound/Walk/footfall_02_run.wav", HDData::SoundGroup::MoveSound);
	playerSound->AddAudio("tumble", "./Resources/Sound/Tumble/tumble_large.wav", HDData::SoundGroup::MoveSound);
	playerSound->AddAudio("tumblingMan", "./Resources/Sound/Tumble/tumblingMan.wav", HDData::SoundGroup::ActionSound);
	playerSound->AddAudio("dance", "./Resources/Sound/Dance/danceMusic.wav", HDData::SoundGroup::ActionSound);
	playerSound->AddAudio("hitBody", "./Resources/Sound/Hit/hitBody3.wav", HDData::SoundGroup::EffectSound);
	playerSound->AddAudio("hitHead", "./Resources/Sound/Hit/hitHead2.wav", HDData::SoundGroup::EffectSound);

	posX += 1;
	posT += 315;

	// 디버그용 state
	//auto stateInfo = API::CreateTextbox(_scene, "stateInfo");
	//stateInfo->GetTransform()->SetPosition(300, 300, 0);
	//auto stateInfoComp = stateInfo->GetComponent<HDData::TextUI>();
	//stateInfoComp->SetFont("Resources/Font/KRAFTON_30.spriteFont");
	//stateInfoComp->SetColor(DirectX::Colors::Black);
	//stateInfoComp->SetText("");
	//stateInfoComp->SetSortOrder(0.7);
	//playerMove->_plStateText = stateInfoComp;

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

		// 총
		std::string weaponObjectName = "weapon" + std::to_string(i);
		auto enemyHand = otherPlayer->GetGameObjectByNameInChildren("Thumb_01.001");
		auto enemyWeapon = API::CreateObject(_scene, weaponObjectName, enemyHand);
		enemyWeapon->GetComponent<HDData::Transform>()->SetLocalPosition(-2.892f, 8.082f, 7.923f);
		enemyWeapon->GetComponent<HDData::Transform>()->SetLocalRotation({ 0.061f, -0.606f, -0.093f, 0.787f });
		auto enemyWeaponComp = enemyWeapon->AddComponent<HDData::MeshRenderer>();
		enemyWeaponComp->LoadMesh("SM_Wep_Revolver_02.fbx");
		enemyWeaponComp->LoadMaterial(chMat, 0);
		enemyWeaponComp->LoadMaterial(chMat, 1);
		enemyWeaponComp->LoadMaterial(chMat, 2);
		enemyWeaponComp->LoadMaterial(chMat, 3);

		// 파티클
		std::string particleObjName = "EnemyParticleEffect" + std::to_string(i);
		auto enemyParticleSystemObj = API::CreateObject(_scene, particleObjName, enemyWeapon);
		enemyParticleSystemObj->GetTransform()->SetLocalScale(20.0f, 20.0f, 20.0f);
		enemyParticleSystemObj->GetTransform()->SetLocalPosition(0.0f, 40.5722f, 6.8792f);
		enemyParticleSystemObj->GetTransform()->SetLocalRotation(-0.6989f, 0.0f, 0.0f, -0.7150f);
		auto enemyParticleSystem = enemyParticleSystemObj->AddComponent<HDData::ParticleSystem>();
		enemyParticleSystem->main.duration = 0.08f;
		enemyParticleSystem->main.loop = false;
		enemyParticleSystem->main.minStartColor = { 255, 255, 197, 255 };
		enemyParticleSystem->main.maxStartColor = { 255, 255, 255, 255 };
		enemyParticleSystem->main.minStartLifetime = 0.04f;
		enemyParticleSystem->main.maxStartLifetime = 0.08f;
		enemyParticleSystem->main.minStartRotation = 90.0f;
		enemyParticleSystem->main.maxStartRotation = 90.0f;
		enemyParticleSystem->main.minStartSize = 0.05f;
		enemyParticleSystem->main.maxStartSize = 0.075f;
		enemyParticleSystem->main.minStartSpeed = 0.0f;
		enemyParticleSystem->main.maxStartSpeed = 0.0f;
		enemyParticleSystem->emission.enabled = true;
		HDData::Burst newBurst2(0.0f, 1);
		enemyParticleSystem->emission.SetBurst(newBurst2);
		enemyParticleSystem->sizeOverLifetime.enabled = true;
		HDData::AnimationCurve curve2;
		curve.AddKey(0.0f, 1.0f, [](float t) { return -3.8f * t * t + 3.7f * t + 0.1f; });
		enemyParticleSystem->sizeOverLifetime.size = HDData::MinMaxCurve(1.0f, curve2);
		HDEngine::MaterialDesc flashMatDesc2;
		flashMatDesc2.materialName = "muzzleFlash";
		flashMatDesc2.albedo = "T_MuzzleFlash_D.png";
		flashMatDesc2.color = { 255, 140, 85, 255 };
		HDData::Material* flashMat2 = API::CreateMaterial(flashMatDesc2);
		enemyParticleSystem->rendererModule.renderMode = HDEngine::ParticleSystemRenderMode::Mesh;
		enemyParticleSystem->rendererModule.material = flashMat2;
		enemyParticleSystem->rendererModule.mesh = "SM_MuzzleFlash.fbx";

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
		enemyParticleSystem->colorOverLifetime.color.SetKeys(ck, ak);

		otherPlayer->AddComponent<OthersAnim>();

		RoundManager::Instance()->_playerObjs.push_back(otherPlayer);

		// sound 추가
		HDData::AudioSource* otherPlayerSound = otherPlayer->AddComponent<HDData::AudioSource>();
		otherPlayerSound->AddAudio3D("shootother", "./Resources/Sound/Shoot/Gun_sound7.wav", HDData::SoundGroup::EffectSound, 10, 150);
		otherPlayerSound->AddAudio3D("walkother", "./Resources/Sound/Walk/footstep.wav", HDData::SoundGroup::EffectSound, 10, 250);

		posX += 2;
		posT += 315;
	}

	int uiX = 130;
	int uiY = 52.0f;

	for (int i = 0; i < 6; ++i)
	{
		// killCount UI
		//auto uiBack = API::CreateImageBox(_scene, "back" + std::to_string(i));
		//uiBack->GetTransform()->SetPosition(uiX, uiY, 0);
		//uiBack->GetTransform()->SetScale(1, 3, 0);
		//uiBack->GetComponent<HDData::ImageUI>()->SetSortOrder(0.6);
		//uiBack->GetComponent<HDData::ImageUI>()->SetImage("back.png");

		auto nickname = API::CreateTextbox(_scene, "nick" + std::to_string(i));
		nickname->GetTransform()->SetPosition(uiX-40, uiY, 0);
		auto nickComp = nickname->GetComponent<HDData::TextUI>();
		nickComp->SetFont("Resources/Font/KRAFTON_30.spriteFont");
		nickComp->SetText("");
		nickComp->SetSortOrder(0.7);

		auto killcount = API::CreateTextbox(_scene, "count" + std::to_string(i));
		killcount->GetTransform()->SetPosition(uiX + 35, uiY, 0);
		auto countComp = killcount->GetComponent<HDData::TextUI>();
		countComp->SetFont("Resources/Font/KRAFTON_30.spriteFont");
		countComp->SetText("");
		countComp->SetSortOrder(0.7);

		//RoundManager::Instance()->SetKillCountBack(uiBack->GetComponent<HDData::ImageUI>(), i);
		RoundManager::Instance()->SetKillCountUI(nickComp, countComp, i);

		uiY += 60;
	}

	// crosshair
	auto crosshairObj = API::CreateObject(_scene, "Crosshair");
	auto crosshairComp = crosshairObj->AddComponent<Crosshair>();
	crosshairComp->playerMove = playerMove;

	// ammo
	auto ammo = API::CreateTextbox(_scene, "AmmoText");
	auto ammoTXT = ammo->GetComponent<HDData::TextUI>();
	ammoTXT->SetFont("Resources/Font/KRAFTON_55.spriteFont");
	ammoTXT->GetTransform()->SetPosition(2250.0f, 1400.0f, 0.0f);
	RoundManager::Instance()->SetAmmoText(ammoTXT);

	// HP
	HDData::GameObject* healthPoint = API::CreateTextbox(_scene, "healthPoint");
	auto hpTxt = healthPoint->GetComponent<HDData::TextUI>();
	hpTxt->SetFont("Resources/Font/KRAFTON_55.spriteFont");
	hpTxt->GetTransform()->SetPosition(2100.0f, 1400.0f, 0.0f);
	RoundManager::Instance()->SetHPObject(hpTxt);

	// Timer
	auto timer = API::CreateTextbox(_scene, "timer");
	RoundManager::Instance()->SetRoundTimerObject(timer->GetComponent<HDData::TextUI>());

	/// game end

	auto endButton = API::CreateButton(_scene, "endBtn");
	endButton->GetTransform()->SetPosition(2300.0f, 1300.0f, 0.0f);
	endButton->AddComponent<BtnTextScript>();
	auto endComp = endButton->GetComponent<HDData::Button>();
	endComp->SetImage("Button_02.png");
	endComp->SetOnClickEvent([=]()
		{
			RoundManager::Instance()->ExitGame();
		});

	auto endText = API::CreateTextbox(_scene, "endTXT", endButton);
	//endText->GetTransform()->SetPosition(endButton->GetTransform()->GetPosition());
	//endText->GetTransform()->SetPosition(1225.0f, 1285.0f, 0.0f);
	endText->GetTransform()->SetPosition(2175.0f, 1285.0f, 0.0f);	//endbutton.x -125
	auto endTXTcomp = endText->GetComponent<HDData::TextUI>();
	endTXTcomp->SetText("EXIT GAME");
	endTXTcomp->SetFont("Resources/Font/KRAFTON_55.spriteFont");

	endButton->SetSelfActive(false);

	RoundManager::Instance()->SetRoundEndButton(endButton);

	// ESC Menu
	HDData::GameObject* esc_controlObj = API::CreateImageBox(_scene,"escControlObj");
	esc_controlObj->SetSelfActive(false);
	esc_controlObj->GetTransform()->SetPosition(-500.0f, -500.0f, 0.0f);

	auto inGameESCMenuCanvas = API::CreateImageBox(_scene,"ESCMenuCanvas",esc_controlObj);

	// 우승자
	auto winnerObj = API::CreateObject(_scene, "winner");
	winnerObj->LoadFBXFile("SKM_GunManTP_X_default.fbx");
	winnerObj->GetTransform()->SetPosition(53, 0, -35);
	winnerObj->GetTransform()->Rotate(0, 90, 0);
	winnerObj->GetComponentInChildren<HDData::SkinnedMeshRenderer>()->LoadAnimation("TP");
	winnerObj->GetComponentInChildren<HDData::SkinnedMeshRenderer>()->PlayAnimation("RV_sillyDancing");
	winnerObj->GetComponentInChildren<HDData::SkinnedMeshRenderer>()->LoadMaterial(chMat, 0);

	auto winnerTextImg = API::CreateImageBox(_scene, "winnerImg");
	winnerTextImg->SetSelfActive(false);
	winnerTextImg->GetTransform()->SetPosition(API::GetScreenWidth()/2,400.0f,0.0f);
	winnerTextImg->SetSelfActive(false);
	auto winnerTextImgComp = winnerTextImg->GetComponent<HDData::ImageUI>();
	winnerTextImgComp->SetImage("winner.png");
	winnerTextImgComp->SetColor(DirectX::Colors::Gold);	// 노란색 이미지를 가져올것

	auto winnerName = API::CreateTextbox(_scene, "winner");
	winnerName->GetTransform()->SetPosition((API::GetScreenWidth()/2)-30, 1200.0f, 0.0f);
	auto winnerComp = winnerName->GetComponent<HDData::TextUI>();
	winnerComp->SetFont("Resources/Font/KRAFTON_55.spriteFont");
	winnerComp->SetColor(DirectX::Colors::Gold);
	
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

	// 다시 로비 진입을 위한 타이머 UI
	auto resultTimer = API::CreateTextbox(_scene, "resultTimer");
	resultTimer->GetTransform()->SetPosition(200.0f, 1400.0f, 0.0f);
	auto resultTimerComp = resultTimer->GetComponent<HDData::TextUI>();
	resultTimerComp->SetFont("Resources/Font/KRAFTON_55.spriteFont");
	resultTimerComp->SetColor(DirectX::Colors::OrangeRed);

	resultTimer->SetSelfActive(false);

	RoundManager::Instance()->SetResultTimerUI(resultTimerComp);

	// low hp screen effect
	auto hpEffectObj = API::CreateObject(_scene, "LowHPEffect");
	RoundManager::Instance()->lowHPEffect = hpEffectObj->AddComponent<LowHPEffect>();

	auto hitEffectObj = API::CreateObject(_scene, "HitEffect");
	hitEffectObj->AddComponent<HitEffect>();

	auto dirLight = API::GetObjectByName(_scene, "DirectionalLight");
	auto lightComp = dirLight->GetComponent<HDData::Light>();
	lightComp->SetColor({ 251.0f / 255.0f, 209.0f / 255.0f, 129.0f / 255.0f, 1.0f });

	IndicatorPool::Instance().player = player;

	if (RoundManager::Instance()->GetMenuStatus())
	{
		esc_controlObj->SetSelfActive(true);
	}
	else
	{
		esc_controlObj->SetSelfActive(false);
	}

	/// init round
	// 라운드 시작 카운터
	auto initCounter = API::CreateTextbox(_scene, "initCounter");
	initCounter->GetTransform()->SetPosition(API::GetScreenWidth()/2,API::GetScreenHeight()/2,0.0f);
	initCounter->SetSelfActive(false);
	auto countertxt = initCounter->GetComponent<HDData::TextUI>();
	countertxt->SetFont("Resources/Font/KRAFTON_200.spriteFont");
	countertxt->SetColor(DirectX::Colors::LightYellow);
	RoundManager::Instance()->SetInitRoundTimer(countertxt);

	auto gamestarttxt = API::CreateImageBox(_scene);
	gamestarttxt->GetTransform()->SetPosition(API::GetScreenWidth() / 2, API::GetScreenHeight() / 2, 0.0f);
	gamestarttxt->AddComponent<UIEffect>(Vector2{ 1.5, 1.5 }, 0.2, false, 10);
	auto startimg = gamestarttxt->GetComponent<HDData::ImageUI>();
	startimg->SetImage("gamestart.png");
	RoundManager::Instance()->startRoundimg = startimg;

	// 헤드샷 이펙트
	auto killEffect = API::CreateImageBox(_scene);
	killEffect->GetTransform()->SetPosition(API::GetScreenWidth() / 2, API::GetScreenHeight() / 4, 0);
	auto killEffectImg = killEffect->GetComponent<HDData::ImageUI>();
	killEffectImg->ChangeScale(0.5, 0.5);
	killEffect->AddComponent<UIEffect>(Vector2{ 1.5, 1.5 }, 0.2, true, 5);
	GameManager::Instance()->GetMyInfo()->SetKillEffectImg(killEffectImg);

	// round finish
	auto roundfin = API::CreateImageBox(_scene);
	roundfin->GetTransform()->SetPosition(API::GetScreenWidth() / 2, API::GetScreenHeight() / 2, 0);
	roundfin->AddComponent<UIEffect>(Vector2{ 1.5, 1.5 }, 0.2, false, 10);
	auto finimg = roundfin->GetComponent<HDData::ImageUI>();
	finimg->SetImage("finRound2.png");
	RoundManager::Instance()->finRoundimg = finimg;

	// die image
	auto dieblack = API::CreateImageBox(_scene);
	dieblack->GetTransform()->SetPosition(API::GetScreenWidth() / 2, API::GetScreenHeight() / 2, 0);
	dieblack->SetSelfActive(false);
	auto dieblackimg = dieblack->GetComponent<HDData::ImageUI>();
	dieblackimg->SetImage("black.png");
	GameManager::Instance()->GetMyInfo()->SetDieEffectImg(dieblackimg);

	// 구르기 UI
	auto tumbleObj = API::CreateObject(_scene, "Tumble");
	tumbleObj->GetTransform()->SetPosition(1750, 1350, 0);
	auto tumbleComp = tumbleObj->AddComponent<HDData::ImageUI>();
	tumbleComp->SetImage("recoil_rounded.png");
	tumbleComp->SetSortOrder(0.7f);
	RoundManager::Instance()->tumbleImage = tumbleComp;

	auto tumbleAlphaObj = API::CreateObject(_scene, "TumbleAlpha");
	tumbleAlphaObj->GetTransform()->SetPosition(1750, 1350, 0);
	auto tumbleCooldown = tumbleAlphaObj->AddComponent<CooldownAlpha>();
	auto tumbleAlphaImage = tumbleAlphaObj->AddComponent<HDData::ImageUI>();
	tumbleAlphaImage->SetImage("recoil_alpha_rounded.png");
	tumbleAlphaImage->SetSortOrder(0.8f);

	auto tumbleCooldownCountObj = API::CreateObject(_scene, "TumbleCount");
	tumbleCooldownCountObj->GetTransform()->SetPosition(1750 - 5, 1350 + 5, 0);
	auto tumbleCooldownCount = tumbleCooldownCountObj->AddComponent<CooldownText>();
	auto tumbleCooldownText = tumbleCooldownCountObj->AddComponent<HDData::TextUI>();
	tumbleCooldownText->SetFont("Resources/Font/KRAFTON_55.spriteFont");
	tumbleCooldownText->SetText("0");
	tumbleCooldownText->SetSortOrder(0.9f);

	playerMove->recoilCooldown = tumbleCooldown;
	playerMove->cooldownCountText = tumbleCooldownCount;

	//auto cube = API::CreateObject(_scene);
	//cube->LoadFBXFile("SM_Bld_TowerClock_01.fbx");
	//cube->GetTransform()->SetPosition(-10, 3, 0);

	//NetworkManager::Instance().cube = cube;

	API::LoadSceneFromData("sceneData.json", this->_scene);
}

std::string& InGameSceneView::convertBullet(int bullet)
{
	_tempBullet = std::to_string(bullet);
	return _tempBullet;
}
