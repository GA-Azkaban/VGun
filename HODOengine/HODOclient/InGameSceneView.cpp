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

	auto mainCam = _scene->GetMainCamera();
	mainCam->GetGameObject()->AddComponent<CameraMove>();

	// 플레이어 여섯 명 렌더링

	float posX = 0;
	float posT = 165;

	HDEngine::MaterialDesc red;
	red.materialName = "TP_Red";
	red.albedo = "TP_Red_B.png";

	HDData::Material* M_Red = API::CreateMaterial(red);

	for (int i = 0; i < 6; ++i)
	{
		std::string objName1 = "player" + std::to_string(i + 1);
		HDData::GameObject* player = API::CreateObject(_scene, objName1);
		player->LoadFBXFile("SKM_TP_X_Default.fbx");
		player->GetTransform()->SetPosition(posX, 0, 0);
		player->GetTransform()->SetLocalRotationEuler(Vector3(0.0f, 180.0f, 0.0f));

		auto meshComp = player->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
		meshComp->LoadMaterial(M_Red, 0);
		meshComp->LoadMaterial(M_Red, 1);
		meshComp->LoadMaterial(M_Red, 2);
		meshComp->LoadMaterial(M_Red, 3);
		meshComp->LoadMaterial(M_Red, 4);

		meshComp->PlayAnimation("AR_idle", true);

		RoundManager::Instance()->GetPlayerObjs().push_back(player);

		player->SetSelfActive(false);

		

		// playerMove 장착을 위한
		std::string objName2 = "playerCenter" + std::to_string(i + 1);
		auto playerCenter = API::CreateObject(_scene, objName2);
		player->SetParentObject(playerCenter);
		playerCenter->GetTransform()->SetPosition(0.0f, 3.0f, 0.0f);
		auto playerBodyCollider = playerCenter->AddComponent<HDData::DynamicBoxCollider>(0.5f, 0.6f, 0.25f, 1);

		auto playerTestHead = player->GetGameObjectByNameInChildren("head");
		playerTestHead->SetParentObject(playerCenter);
		//playerTestHead->GetTransform()->SetLocalPosition(Vector3{ 0.0f, 0.4f, 0.0f });
		auto playerHeadCollider = playerTestHead->AddComponent<HDData::DynamicSphereCollider>(0.2f, true);
		playerHeadCollider->SetParentCollider(playerBodyCollider);
		playerHeadCollider->SetPositionOffset(Vector3(0.0f, 0.4f, 0.0f));

		std::string objName3 = "headCamObj" + std::to_string(i + 1);
		auto headCamObj = API::CreateObject(_scene, objName3);
		headCamObj->SetParentObject(playerTestHead);
		headCamObj->GetTransform()->SetLocalPosition(Vector3{ 0.0f, 0.12f, 0.2f });
		auto headCam = headCamObj->AddComponent<HDData::Camera>();

		auto playerMove = playerCenter->AddComponent<PlayerMove>();
		playerMove->SetPlayerCamera(_scene->GetMainCamera());
		playerCenter->GetComponent<PlayerMove>()->SetPlayerCamera(_scene->GetMainCamera());
		playerMove->SetHeadCam(headCam);

		// sound 추가
		HDData::AudioSource* playerSound = playerCenter->AddComponent<HDData::AudioSource>();
		playerSound->AddAudio("shoot", "./Resources/Sound/Shoot/Gun_sound6.wav", HDData::SoundGroup::EffectSound);
		playerSound->AddAudio("empty", "./Resources/Sound/Shoot/Gun_sound_empty.wav", HDData::SoundGroup::EffectSound);
		playerSound->AddAudio("hit", "./Resources/Sound/Hit/Hit.wav", HDData::SoundGroup::EffectSound);
		playerSound->AddAudio("jump", "./Resources/Sound/Walk/footfall_01.wav", HDData::SoundGroup::EffectSound);
		playerSound->AddAudio("walk", "./Resources/Sound/Walk/footfall_02.wav", HDData::SoundGroup::EffectSound);
		playerSound->AddAudio("run", "./Resources/Sound/Walk/footfall_02_run.wav", HDData::SoundGroup::EffectSound);
		playerSound->AddAudio("reload", "./Resources/Sound/GunReload/Reload.wav", HDData::SoundGroup::EffectSound);

		posX += 1;
		posT += 315;
	}

	API::LoadSceneFromData("sceneData.json", this->_scene);
}
