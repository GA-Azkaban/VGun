#include "AnimationLoaderTest.h"
#include "CameraMove.h"
#include "FSMtestScript.h"
#include "LobbyManager.h"

AnimationLoaderTest::AnimationLoaderTest()
{

}

AnimationLoaderTest::~AnimationLoaderTest()
{

}

void AnimationLoaderTest::Start()
{
	_scene = API::CreateScene("ALT");

	HDData::GameObject* mainCam = _scene->GetMainCamera()->GetGameObject();
	mainCam->AddComponent<CameraMove>();

	auto playerTP = API::CreateObject(_scene, "playerTP");
	playerTP->GetComponent<HDData::Transform>()->SetPosition(Vector3{ 0.0f, 0.0f, 0.0f });
	playerTP->LoadFBXFile("SKM_TP_X_idle.fbx");

	auto meshComp = playerTP->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
	HDEngine::MaterialDesc desc;
	desc.materialName = "TP_Red";
	desc.albedo = "TP_Red_B.png";

	HDData::Material* newMat = API::CreateMaterial(desc);
	meshComp->LoadMaterial(newMat, 0);
	meshComp->LoadMaterial(newMat, 1);
	meshComp->LoadMaterial(newMat, 2);
	meshComp->LoadMaterial(newMat, 3);
	meshComp->LoadMaterial(newMat, 4);

	auto name = playerTP->GetThisObjectScene()->GetSceneName();

	playerTP->AddComponent<HDData::Animator>();

	playerTP->AddComponent<FSMtestScript>();

	HDData::GameObject* test2 = API::CreateButton(_scene);
	//test2->GetComponent<HDData::Button>()->SetSortOrder(0.5f);
	test2->GetComponent<HDData::Button>()->SetOnClickEvent(
		[]()
		{
			//LobbyManager::Instance().Test2();
		}
	);
}
