#include "InGameSceneView.h"
#include "CameraMove.h"
#include "PlayerMove.h"
#include "FSMtestScript.h"
#include "RoundManager.h"

InGameSceneView::InGameSceneView()
{

}

InGameSceneView::~InGameSceneView()
{

}

void InGameSceneView::Initialize()
{
	_scene = API::CreateScene("InGame");

	auto skybox = API::CreateObject(_scene);
	auto skyboxComp = skybox->AddComponent<HDData::CubeMapRenderer>();
	skyboxComp->LoadCubeMapTexture("Day Sun Peak Clear.dds");

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
	freeRoamingCamObj->GetTransform()->SetPosition(0.0f, 2.0f, -10.0f);
	freeRoamingCamObj->AddComponent<CameraMove>();

	// 내 캐릭터 생성
	std::string objName1 = "player" + std::to_string(1);
	HDData::GameObject* player = API::CreateObject(_scene, objName1);
	player->LoadFBXFile("SKM_TP_X_Default.fbx");
	player->GetTransform()->SetPosition(posX, 3, 0);

	auto meshComp = player->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
	meshComp->LoadMaterial(M_Red, 0);
	meshComp->LoadMaterial(M_Red, 1);
	meshComp->LoadMaterial(M_Red, 2);
	meshComp->LoadMaterial(M_Red, 3);
	meshComp->LoadMaterial(M_Red, 4);
	meshComp->GetGameObject()->GetTransform()->Rotate(0.0f, 180.0f, 0.0f);

	meshComp->PlayAnimation("AR_idle", true);

	RoundManager::Instance()->GetPlayerObjs().push_back(player);

	// 메인 카메라를 1인칭 캐릭터 머리에 붙은 카메라로 사용한다.
	// 메인 카메라에 오디오 리스너 컴포넌트가 붙기 때문
	auto mainCam = _scene->GetMainCamera();
	mainCam->GetGameObject()->SetParentObject(player);
	mainCam->GetGameObject()->GetTransform()->SetLocalPosition(Vector3{ 0.0f, 0.12f, 0.2f });

	auto playerMove = player->AddComponent<PlayerMove>();
	playerMove->SetPlayerCamera(freeRoamingCam);
	playerMove->SetHeadCam(mainCam);

	player->AddComponent<HDData::DynamicBoxCollider>(0.5f, 1.2f, 0.25f, 1);

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
		std::string otherObjName = "player" + std::to_string(i + 1);
		HDData::GameObject* otherPlayer = API::CreateObject(_scene, otherObjName);
		otherPlayer->LoadFBXFile("SKM_TP_X_Default.fbx");
		otherPlayer->GetTransform()->SetPosition(posX, 3, 0);

		auto otherMeshComp = otherPlayer->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
		otherMeshComp->LoadMaterial(M_Red, 0);
		otherMeshComp->LoadMaterial(M_Red, 1);
		otherMeshComp->LoadMaterial(M_Red, 2);
		otherMeshComp->LoadMaterial(M_Red, 3);
		otherMeshComp->LoadMaterial(M_Red, 4);
		otherMeshComp->GetGameObject()->GetTransform()->Rotate(0.0f, 180.0f, 0.0f);
		
		otherMeshComp->PlayAnimation("AR_idle", true);
		
		otherPlayer->AddComponent<HDData::DynamicBoxCollider>(0.5f, 1.2f, 0.25f, 1);

		RoundManager::Instance()->GetPlayerObjs().push_back(otherPlayer);

		// sound 추가
		HDData::AudioSource* otherPlayerSound = otherPlayer->AddComponent<HDData::AudioSource>();
		otherPlayerSound->AddAudio("shoot", "./Resources/Sound/Shoot/Gun_sound6.wav", HDData::SoundGroup::EffectSound);
		otherPlayerSound->AddAudio("empty", "./Resources/Sound/Shoot/Gun_sound_empty.wav", HDData::SoundGroup::EffectSound);
		otherPlayerSound->AddAudio("hit", "./Resources/Sound/Hit/Hit.wav", HDData::SoundGroup::EffectSound);
		otherPlayerSound->AddAudio("jump", "./Resources/Sound/Walk/footfall_01.wav", HDData::SoundGroup::EffectSound);
		otherPlayerSound->AddAudio("walk", "./Resources/Sound/Walk/footfall_02.wav", HDData::SoundGroup::EffectSound);
		otherPlayerSound->AddAudio("run", "./Resources/Sound/Walk/footfall_02_run.wav", HDData::SoundGroup::EffectSound);
		otherPlayerSound->AddAudio("reload", "./Resources/Sound/GunReload/Reload.wav", HDData::SoundGroup::EffectSound);

		posX += 1;
		posT += 315;
	} 
	
	API::LoadSceneFromData("sceneData.json", this->_scene);
}
