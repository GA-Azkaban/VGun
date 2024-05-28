#include "InGameSceneView.h"
#include "CameraMove.h"
#include "PlayerMove.h"
#include "FSMtestScript.h"
#include "RoundManager.h"
#include "../HODOEngine/CollisionCallback.h"

InGameSceneView::InGameSceneView()
{

}

InGameSceneView::~InGameSceneView()
{

}

void InGameSceneView::Initialize()
{
	_scene = API::CreateScene("InGame");

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
	std::string objName1 = "player" + std::to_string(1);
	HDData::GameObject* player = API::CreateObject(_scene, objName1);
	player->LoadFBXFile("SKM_TP_X_Default.fbx");
	player->GetTransform()->SetPosition(-10, 3, 0);

	auto meshComp = player->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
	meshComp->LoadMaterial(M_Red, 0);
	meshComp->LoadMaterial(M_Red, 1);
	meshComp->LoadMaterial(M_Red, 2);
	meshComp->LoadMaterial(M_Red, 3);
	meshComp->LoadMaterial(M_Red, 4);
	//meshComp->SetMeshActive(false, 0);
	//meshComp->SetMeshActive(false, 1);
	//meshComp->SetMeshActive(false, 3);
	//meshComp->SetMeshActive(false, 4);

	meshComp->PlayAnimation("AR_aim", true);

	// 총 생성
	auto hand = player->GetGameObjectByNameInChildren("hand_r");
	auto weaponTest = API::CreateObject(_scene, "weapon", hand);
	weaponTest->GetComponent<HDData::Transform>()->SetLocalPosition(-15.2774f, -5.1681f, -1.1526f);
	weaponTest->GetComponent<HDData::Transform>()->SetLocalRotation({ -0.5260f, 0.5268f, -0.4282f, 0.5123f });
	auto weaponComp = weaponTest->AddComponent<HDData::MeshRenderer>();
	weaponComp->LoadMesh("SM_AR_01.fbx");
	HDEngine::MaterialDesc weaponMatDesc;
	weaponMatDesc.materialName = "M_WEP_Basic_039";
	weaponMatDesc.albedo = "T_WEP_Basic_004_D.png";
	weaponMatDesc.roughness = "T_WEP_Basic_R.png";
	weaponMatDesc.metallicValue = 0.15;
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

	RoundManager::Instance()->GetPlayerObjs().push_back(player);

	// 메인 카메라를 1인칭 캐릭터 머리에 붙은 카메라로 사용한다.
	// 메인 카메라에 오디오 리스너 컴포넌트가 붙기 때문
	auto mainCam = _scene->GetMainCamera();
	mainCam->GetGameObject()->SetParentObject(player);
	mainCam->GetGameObject()->GetTransform()->SetLocalPosition(Vector3{ -0.1f, 1.65f, -0.175f });

	auto playerMove = player->AddComponent<PlayerMove>();
	playerMove->SetPlayerCamera(freeRoamingCam);
	playerMove->SetHeadCam(mainCam);

	//player->AddComponent<HDData::DynamicBoxCollider>(0.5f, 1.2f, 0.25f, 1);
	auto playerCollider = player->AddComponent<HDData::DynamicCapsuleCollider>(0.4f, 0.875f);
	playerCollider->SetPositionOffset({ 0.0f, 0.875f, 0.0f });

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
		otherPlayer->GetTransform()->SetPosition(posX, 0, 0);

		auto otherMeshComp = otherPlayer->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
		otherMeshComp->LoadMaterial(M_Red, 0);
		otherMeshComp->LoadMaterial(M_Red, 1);
		otherMeshComp->LoadMaterial(M_Red, 2);
		otherMeshComp->LoadMaterial(M_Red, 3);
		otherMeshComp->LoadMaterial(M_Red, 4);
		
		otherMeshComp->PlayAnimation("AR_idle", true);
		
		auto boxCol = otherPlayer->AddComponent<HDData::DynamicBoxCollider>(0.5f, 1.2f, 0.25f, 1);
		boxCol->SetPositionOffset(Vector3(0.0f, 0.6f, 0.0f));

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
