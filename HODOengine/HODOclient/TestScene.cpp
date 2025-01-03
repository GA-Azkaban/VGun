﻿#include "TestScene.h"
#include "CameraMove.h"
#include "PlayerMove.h"
#include "TestSound.h"
#include "SliderSoundScript.h"
#include "NetworkManager.h"
#include "PlayerTest.h"
#include "MeshTransformController.h"
#include "UIEffect.h"

TestScene::TestScene()
{
	_scene = API::CreateScene("Test");

	auto mainCam = _scene->GetMainCamera()->GetGameObject();
	mainCam->AddComponent<CameraMove>();
	
	//auto testBox1 = API::CreateObject(_scene);
	//testBox1->GetComponent<HDData::Transform>()->SetPosition(0.0f, 0.0f, 10.0f);
	//testBox1->GetComponent<HDData::Transform>()->SetScale(20.0f, 20.0f, 0.0f);
	////testBox1->GetComponent<HDData::Transform>()->Rotate(90.0f, 0.0f, 0.0f);
	//auto boxRender1 = testBox1->AddComponent<HDData::MeshRenderer>();
	//boxRender1->LoadMesh("primitiveQuad");
	//HDEngine::MaterialDesc boxMat1;
	//boxMat1.materialName = "boxMat";
	////boxMat1.albedo = "button.png";
	//boxMat1.color = { 10,10,10,255 };
	//HDData::Material* newBoxMat1 = API::CreateMaterial(boxMat1);
	//boxRender1->LoadMaterial(newBoxMat1, 0);
	//boxRender1->SetUseLight(false);

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

	particleSystem->Play(); */

	/*auto particleSystemObj2 = API::CreateObject(_scene, "SmokeParticle");
	auto particleSystem2 = particleSystemObj2->AddComponent<HDData::ParticleSystem>();
	particleSystem2->main.duration = 2.5f;
	particleSystem2->main.loop = true;
	particleSystem2->main.minStartColor = { 255, 255, 255, 255 };
	particleSystem2->main.maxStartColor = { 255, 255, 255, 255 };
	particleSystem2->main.minStartLifetime = 2.0f;
	particleSystem2->main.maxStartLifetime = 2.0f;
	particleSystem2->main.minStartRotation = 0.0f;
	particleSystem2->main.maxStartRotation = 360.0f;
	particleSystem2->main.minStartSize = 0.1f;
	particleSystem2->main.maxStartSize = 0.2f;
	particleSystem2->main.minStartSpeed = 1.0f;
	particleSystem2->main.maxStartSpeed = 1.0f;
	particleSystem2->emission.enabled = true;
	HDData::Burst newBurst2(0.0f, 4);
	particleSystem2->emission.SetBurst(newBurst2);
	particleSystem2->sizeOverLifetime.enabled = true;
	HDData::AnimationCurve curve2;
	curve2.AddKey(0.0f, 0.2f, [](float t) { return 3.75f * t; });
	curve2.AddKey(0.2f, 1.0f, [](float t) { return 0.3125f * t + 0.6875f; });
	particleSystem2->sizeOverLifetime.size = HDData::MinMaxCurve(1.0f, curve2);
	particleSystem2->rotationOverLifetime.enabled = true;
	particleSystem2->rotationOverLifetime.angularVelocity = 100.0f;
	HDEngine::MaterialDesc particleMatDesc;
	particleMatDesc.materialName = "particleMat";
	particleMatDesc.albedo = "PolygonParticles_Circle_01.png";
	HDData::Material* particleMat = API::CreateMaterial(particleMatDesc);
	particleSystem2->rendererModule.material = particleMat;
	particleSystem2->colorOverLifetime.enabled = true;
	// colorKey, alphaKey 생성
	std::vector<HDData::GradientColorKey> ck;
	std::vector<HDData::GradientAlphaKey> ak;
	HDData::GradientColorKey colorkey1;
	colorkey1.color = { 255, 255, 255 };
	colorkey1.time = 0.0f;
	ck.push_back(colorkey1);
	HDData::GradientColorKey colorkey2;
	colorkey2.color = { 63, 63, 63 };
	colorkey2.time = 1.0f;
	ck.push_back(colorkey2);
	HDData::GradientAlphaKey alphaKey1;
	alphaKey1.alpha = 150;
	alphaKey1.time = 0.0f;
	ak.push_back(alphaKey1);
	HDData::GradientAlphaKey alphaKey2;
	alphaKey2.alpha = 64;
	alphaKey2.time = 0.703f;
	ak.push_back(alphaKey2);
	HDData::GradientAlphaKey alphaKey3;
	alphaKey3.alpha = 0;
	alphaKey3.time = 1.0f;
	ak.push_back(alphaKey3);
	particleSystem2->colorOverLifetime.color.SetKeys(ck, ak);
	particleSystem2->Play(); */

	auto buildingTest1 = API::CreateObject(_scene);
	buildingTest1->GetComponent<HDData::Transform>()->SetPosition(20.0f, 0.0f, 10.0f);
	//buildingTest1->GetComponent<HDData::Transform>()->Rotate(0.0f, -90.0f, 0.0f);
	auto buildingRenderer1 = buildingTest1->AddComponent<HDData::MeshRenderer>();
	buildingRenderer1->LoadMesh("SM_Bld_Saloon_01.fbx");
	HDEngine::MaterialDesc buildingDesc1;
	buildingDesc1.materialName = "PolygonWestern_Texture_02";
	buildingDesc1.albedo = "PolygonWestern_Texture_02.png";
	buildingDesc1.metallic = 0.0f;
	HDData::Material* newBuildingMat1 = API::CreateMaterial(buildingDesc1);
	for (int i = 0; i < buildingRenderer1->GetMeshCount(); ++i)
	{
		buildingRenderer1->LoadMaterial(newBuildingMat1, i);
	}

	// 플레이어 테스트
	auto playerTest = API::CreateObject(_scene, "player");
	playerTest->GetComponent<HDData::Transform>()->SetPosition(Vector3{ 20.0f, 0.0f, 0.0f });
	playerTest->GetComponent<HDData::Transform>()->Rotate(0.0f, 0.0f, 0.0f);
	playerTest->AddComponent<PlayerTest>();
	// 확장자 포함한 파일이름을 넣어준다.
	// LoadFBXFile 함수는 노드를 따라 게임오브젝트를 계층구조대로 생성해주고
	// 메쉬와 노드를 불러와 적용시킨다.
	// 그리고 자식오브젝트를 만들어 SkinnedMeshRenderer 컴포넌트를 부착한다.
	playerTest->LoadFBXFile("SKM_CowboyTP_X_default.fbx");

	////SkinnedMeshRenderer 컴포넌트는 자식오브젝트에 생성되므로
	////GetComponentInChildren 함수로 가져와서 사용해야 한다.
	auto meshComp = playerTest->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
	meshComp->LoadAnimation("TP");
	////meshComp->SetMeshActive(false, 0);
	////meshComp->SetFillModeWireFrame(true);

	HDEngine::MaterialDesc desc;
	desc.materialName = "PolygonWestern_Texture_01_A";
	desc.albedo = "PolygonWestern_Texture_01_A.png";
	HDData::Material* newMat = API::CreateMaterial(desc);
	meshComp->LoadMaterial(newMat, 0);

	meshComp->PlayAnimation("RV_none", true);

	//playerTest->AddComponent<HDData::Animator>();
	//API::LoadFPAnimationFromData(playerTest, "TP_animation.json");
	//playerTest->AddComponent<TPScript>();

	//// 오른손 노드의 오브젝트를 가져와서
	//// 그 오브젝트의 자식 오브젝트를 새로 만들어 총기 메쉬를 부착한다.
	/*auto hand = playerTest->GetGameObjectByNameInChildren("Thumb_01.001");
	auto weaponTest = API::CreateObject(_scene, "weapon", hand);
	weaponTest->AddComponent<MeshTransformController>();
	weaponTest->GetComponent<HDData::Transform>()->SetLocalPosition(-2.1510f, 8.9123f, 7.9229f);
	weaponTest->GetComponent<HDData::Transform>()->SetLocalRotation({ 0.0398f, -0.6211f, 0.0026f, 0.7820f });
	auto weaponComp = weaponTest->AddComponent<HDData::MeshRenderer>();
	weaponComp->LoadMesh("SM_Wep_Revolver_01.fbx");

	weaponComp->LoadMaterial(newMat, 0);
	weaponComp->LoadMaterial(newMat, 1);
	weaponComp->LoadMaterial(newMat, 2);
	weaponComp->LoadMaterial(newMat, 3);*/

	//playerTest->AddComponent<HDData::Animator>();
	//API::LoadFPAnimationFromData(playerTest, "TP_animation.json");

	//playerTest->AddComponent<TPScript>(); */

	auto particleSystemObj3 = API::CreateObject(_scene, "BloodParticle");
	particleSystemObj3->GetTransform()->SetPosition(10.0f, 0.0f, 0.0f);
	particleSystemObj3->GetTransform()->Rotate(0.0f, 45.0f, 0.0f);
	particleSystemObj3->GetTransform()->SetLocalScale({ 0.01f, 0.01f, 0.01f });
	auto particleSystem3 = particleSystemObj3->AddComponent<HDData::ParticleSystem>();
	particleSystem3->main.duration = 1.0f;
	particleSystem3->main.loop = true;
	particleSystem3->main.minStartColor = { 180, 0, 0, 255 };
	particleSystem3->main.maxStartColor = { 180, 0, 0, 255 };
	particleSystem3->emission.enabled = true;
	HDData::Burst newBurst3(0.0f, 12);
	particleSystem3->emission.SetBurst(newBurst3);
	particleSystem3->limitVelocityOverLifetime.enabled = true;
	particleSystem3->limitVelocityOverLifetime.drag = true;
	particleSystem3->sizeOverLifetime.enabled = true;
	HDData::AnimationCurve curve3;
	curve3.AddKey(0.0f, 0.2f, [](float t) { return 2.875f * t * t * t + (-4.3625f) * t * t + 0.6075f * t + 0.98f; });
	particleSystem3->sizeOverLifetime.size = HDData::MinMaxCurve(1.0f, curve3);
	particleSystem3->rotationOverLifetime.enabled = true;
	particleSystem3->rotationOverLifetime.angularVelocity = 500.0f;
	HDEngine::MaterialDesc bloodMatDesc;
	bloodMatDesc.materialName = "bloodMat";
	bloodMatDesc.color = { 180, 0, 0, 255 };
	HDData::Material* bloodMat = API::CreateMaterial(bloodMatDesc);
	particleSystem3->rendererModule.material = bloodMat;
	particleSystem3->rendererModule.renderMode = HDEngine::ParticleSystemRenderMode::Mesh;
	particleSystem3->rendererModule.mesh = "SM_FX_Sphere_01.fbx";
	particleSystem3->rendererModule.alphaBlending = true;
	particleSystem3->colorOverLifetime.enabled = true;
	//// colorKey, alphaKey 생성
	//std::vector<HDData::GradientColorKey> ck3;
	//std::vector<HDData::GradientAlphaKey> ak3;
	//HDData::GradientColorKey colorkey5;
	//colorkey5.color = { 180, 0, 0 };
	//colorkey5.time = 0.0f;
	//ck3.push_back(colorkey5);
	//HDData::GradientColorKey colorkey6;
	//colorkey6.color = { 255, 0, 0 };
	//colorkey6.time = 0.144f;
	//ck3.push_back(colorkey6);
	//HDData::GradientColorKey colorkey7;
	//colorkey7.color = { 220, 0, 0 };
	//colorkey7.time = 0.403f;
	//ck3.push_back(colorkey7);
	//HDData::GradientAlphaKey alphaKey6;
	//alphaKey6.alpha = 255;
	//alphaKey6.time = 0.0f;
	//ak3.push_back(alphaKey6);
	//HDData::GradientAlphaKey alphaKey7;
	//alphaKey7.alpha = 255;
	//alphaKey7.time = 1.0f;
	//ak3.push_back(alphaKey7);
	//particleSystem3->colorOverLifetime.color.SetKeys(ck3, ak3);
	//particleSystem3->Play();
}

TestScene::~TestScene()
{

}


