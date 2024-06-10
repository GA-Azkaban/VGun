#include "TestScene.h"
#include "CameraMove.h"
#include "PlayerMove.h"
#include "TestSound.h"
#include "SliderSoundScript.h"
#include "NetworkManager.h"
#include "PlayerTest.h"
#include "MeshTransformController.h"
#include "DecalPool.h"

enum eColliderType
{
	PLAYER = 1,
	BOX = 2
};

TestScene::TestScene()
{
	_scene = API::CreateScene("Test");

	auto mainCam = _scene->GetMainCamera()->GetGameObject();
	mainCam->AddComponent<CameraMove>();
	
	auto testBox1 = API::CreateObject(_scene);
	testBox1->GetComponent<HDData::Transform>()->SetPosition(10.0f, -1.85f, 0.0f);
	testBox1->GetComponent<HDData::Transform>()->SetScale(5.0f, 2.0f, 5.0f);
	//testBox1->GetComponent<HDData::Transform>()->Rotate(90.0f, 0.0f, 0.0f);
	auto boxRender1 = testBox1->AddComponent<HDData::MeshRenderer>();
	boxRender1->LoadMesh("primitiveCube");
	HDEngine::MaterialDesc boxMat1;
	boxMat1.materialName = "boxMat";
	//boxMat1.albedo = "button.png";
	boxMat1.color = { 255,255,255,255 };
	HDData::Material* newBoxMat1 = API::CreateMaterial(boxMat1);
	boxRender1->LoadMaterial(newBoxMat1, 0);
	boxRender1->SetUseLight(false); 

	/*auto particleSystemObj = API::CreateObject(_scene);
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

	auto particleSystemObj2 = API::CreateObject(_scene, "particleSystem2", particleSystemObj);
	//auto particleSystemObj2 = API::CreateObject(_scene);
	auto particleSystem2 = particleSystemObj2->AddComponent<HDData::ParticleSystem>();
	particleSystem2->main.duration = 0.2f;
	particleSystem2->main.loop = true;
	particleSystem2->main.minStartColor = { 255, 113, 36, 255 };
	particleSystem2->main.maxStartColor = { 255, 255, 255, 255 };
	particleSystem2->main.minStartLifetime = 0.001f;
	particleSystem2->main.maxStartLifetime = 0.1f;
	particleSystem2->main.minStartRotation = -250.0f;
	particleSystem2->main.maxStartRotation = 250.0f;
	particleSystem2->main.minStartSize = 0.08f;
	particleSystem2->main.maxStartSize = 0.12f;
	particleSystem2->main.minStartSpeed = 10.0f;
	particleSystem2->main.maxStartSpeed = 15.0f;
	particleSystem2->emission.enabled = true;
	HDData::Burst newBurst2(0.0f, 2, 8);
	particleSystem2->emission.SetBurst(newBurst2);
	particleSystem2->sizeOverLifetime.enabled = true;
	HDData::AnimationCurve curve2;
	curve2.AddKey(0.0f, 1.0f, [](float t) { return -6 * t * t + 6 * t; });
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

	//auto particleSystemObj2 = API::CreateObject(_scene, "particleSystem2", particleSystemObj);
	//auto particleSystemObj2 = API::CreateObject(_scene);
	//particleSystemObj2->GetComponent<HDData::Transform>()->Rotate(-90.0f, 0.0f, 0.0f);
	//auto particleSystem2 = particleSystemObj2->AddComponent<HDData::ParticleSystem>();
	//particleSystem2->main.duration = 0.2f;
	//particleSystem2->main.loop = true;
	//particleSystem2->main.minStartColor = { 255, 93, 36, 255 };
	//particleSystem2->main.maxStartColor = { 255, 255, 255, 255 };
	//particleSystem2->main.minStartLifetime = 0.25f;
	//particleSystem2->main.maxStartLifetime = 0.75f;
	//particleSystem2->main.minStartRotation = -250.0f;
	//particleSystem2->main.maxStartRotation = 250.0f;
	//particleSystem2->main.minStartSize = 0.1f;
	//particleSystem2->main.maxStartSize = 0.2f;
	//particleSystem2->main.minStartSpeed = 10.0f;
	//particleSystem2->main.maxStartSpeed = 15.0f;
	//particleSystem2->emission.enabled = true;
	////HDData::Burst newBurst2(0.0f, 4, 16);
	//HDData::Burst newBurst2(0.0f, 4, 16, 2, 0.1);
	//particleSystem2->emission.SetBurst(newBurst2);
	//particleSystem2->sizeOverLifetime.enabled = true;
	//HDData::AnimationCurve curve2;
	//curve2.AddKey(0.0f, 1.0f, [](float t) { return -0.6 * t * t + 0.6 * t; });
	//particleSystem2->sizeOverLifetime.size = HDData::MinMaxCurve(1.0f, curve2);
	//particleSystem2->rotationOverLifetime.enabled = true;
	//particleSystem2->rotationOverLifetime.angularVelocity = 750.0f;
	//HDEngine::MaterialDesc particleMatDesc;
	//particleMatDesc.materialName = "particleMat";
	////particleMatDesc.albedo = "T_Ember_D.png";
	//particleMatDesc.color = { 255, 90, 0, 255 };
	//HDData::Material* particleMat = API::CreateMaterial(particleMatDesc);
	//particleSystem2->rendererModule.material = particleMat;
	//particleSystem2->rendererModule.mesh = "primitiveSphere";
	//particleSystem2->rendererModule.renderMode = HDEngine::ParticleSystemRenderMode::Mesh;
	//particleSystem2->colorOverLifetime.enabled = true;
	//// colorKey, alphaKey 생성
	//std::vector<HDData::GradientColorKey> ck;
	//std::vector<HDData::GradientAlphaKey> ak;
	//HDData::GradientColorKey colorkey1;
	//colorkey1.color = { 255, 255, 255 };
	//colorkey1.time = 0.556f;
	//ck.push_back(colorkey1);
	//HDData::GradientColorKey colorkey2;
	//colorkey2.color = { 80, 70, 60 };
	//colorkey2.time = 1.0f;
	//ck.push_back(colorkey2);
	//HDData::GradientAlphaKey alphaKey1;
	//alphaKey1.alpha = 255;
	//alphaKey1.time = 0.0f;
	//ak.push_back(alphaKey1);
	//HDData::GradientAlphaKey alphaKey2;
	//alphaKey2.alpha = 255;
	//alphaKey2.time = 1.0f;
	//ak.push_back(alphaKey2);
	//particleSystem2->colorOverLifetime.color.SetKeys(ck, ak);
	//particleSystem2->Play();

	//auto testBox2 = API::CreateObject(_scene);
	//testBox2->GetComponent<HDData::Transform>()->SetPosition(-20.0f, -1.0f, 0.0f);
	//auto boxRender2 = testBox2->AddComponent<HDData::MeshRenderer>();
	//boxRender2->LoadMesh("primitiveCube");
	//
	//auto testBox3 = API::CreateObject(_scene);
	//testBox3->GetComponent<HDData::Transform>()->SetPosition(-50.0f, -1.0f, 0.0f);
	//auto boxRender3 = testBox3->AddComponent<HDData::MeshRenderer>();
	//boxRender3->LoadMesh("primitiveCube");
	//
	//auto testBox4 = API::CreateObject(_scene);
	//testBox4->GetComponent<HDData::Transform>()->SetPosition(-80.0f, -1.0f, 20.0f);
	//auto boxRender4 = testBox4->AddComponent<HDData::MeshRenderer>();
	//boxRender4->LoadMesh("primitiveCube");
	//
	//auto testBox5 = API::CreateObject(_scene);
	//testBox5->GetComponent<HDData::Transform>()->SetPosition(50.0f, -1.0f, -10.0f);
	//auto boxRender5 = testBox5->AddComponent<HDData::MeshRenderer>();
	//boxRender5->LoadMesh("primitiveCube");

	//auto buildingTest1 = API::CreateObject(_scene);
	//buildingTest1->GetComponent<HDData::Transform>()->SetPosition(20.0f, 0.0f, 10.0f);
	//buildingTest1->GetComponent<HDData::Transform>()->Rotate(0.0f, -90.0f, 0.0f);
	//auto buildingRenderer1 = buildingTest1->AddComponent<HDData::MeshRenderer>();
	//buildingRenderer1->LoadMesh("SM_building_town_01.fbx");
	//HDEngine::MaterialDesc buildingDesc1;
	//buildingDesc1.materialName = "colorpalette_standard";
	////buildingDesc1.albedo = "colorpalette_standard.png";
	//buildingDesc1.color = { 178,161,132,255 };
	////buildingDesc1.metallic = 0.0f;
	//buildingDesc1.roughness = 1.0f - 0.057f;
	//HDData::Material* newBuildingMat1 = API::CreateMaterial(buildingDesc1);
	//buildingRenderer1->LoadMaterial(newBuildingMat1, 0);
	//buildingRenderer1->LoadMaterial(newBuildingMat1, 1);
	//buildingRenderer1->LoadMaterial(newBuildingMat1, 2);
	//buildingRenderer1->LoadMaterial(newBuildingMat1, 3);
	//buildingRenderer1->LoadMaterial(newBuildingMat1, 4);
	//buildingRenderer1->LoadMaterial(newBuildingMat1, 5); */

	// 플레이어 테스트
	auto playerTest = API::CreateObject(_scene, "player");
	playerTest->GetComponent<HDData::Transform>()->SetPosition(Vector3{ 0.0f, 0.0f, 0.0f });
	playerTest->AddComponent<PlayerTest>();
	// 확장자 포함한 파일이름을 넣어준다.
	// LoadFBXFile 함수는 노드를 따라 게임오브젝트를 계층구조대로 생성해주고
	// 메쉬와 노드를 불러와 적용시킨다.
	// 그리고 자식오브젝트를 만들어 SkinnedMeshRenderer 컴포넌트를 부착한다.
	playerTest->LoadFBXFile("SKM_TP_X_Default.fbx");

	// SkinnedMeshRenderer 컴포넌트는 자식오브젝트에 생성되므로
	// GetComponentInChildren 함수로 가져와서 사용해야 한다.
	auto meshComp = playerTest->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
	//meshComp->SetActive(false);
	//meshComp->SetFillModeWireFrame(true);
	HDEngine::MaterialDesc desc;
	desc.materialName = "TP_Red";
	desc.albedo = "TP_Red_B.png";
	HDData::Material* newMat = API::CreateMaterial(desc);
	meshComp->LoadMaterial(newMat, 0);
	meshComp->LoadMaterial(newMat, 1);
	meshComp->LoadMaterial(newMat, 2);
	meshComp->LoadMaterial(newMat, 3);
	meshComp->LoadMaterial(newMat, 4);
	meshComp->PlayAnimation("AR_aim", true);
	//meshComp->SetOutlineActive(true);
	//meshComp->SetMeshActive(false, 0);
	//meshComp->SetMeshActive(false, 1);
	//meshComp->SetMeshActive(false, 3);
	//meshComp->SetMeshActive(false, 4);

	// 오른손 노드의 오브젝트를 가져와서
	// 그 오브젝트의 자식 오브젝트를 새로 만들어 총기 메쉬를 부착한다.
	auto hand = playerTest->GetGameObjectByNameInChildren("hand_r");
	//auto hand = playerTest->GetGameObjectByNameInChildren("thumb_01_r");
	auto weaponTest = API::CreateObject(_scene, "weapon", hand);
	//weaponTest->AddComponent<MeshTransformController>();
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

	auto decalManager = API::CreateObject(_scene, "DecalManager");
	decalManager->AddComponent<DecalPool>();
}

TestScene::~TestScene()
{

}


