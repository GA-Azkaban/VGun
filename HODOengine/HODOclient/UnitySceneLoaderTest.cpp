#include "UnitySceneLoaderTest.h"
#include "CameraMove.h"

UnitySceneLoaderTest::UnitySceneLoaderTest()
{

}

UnitySceneLoaderTest::~UnitySceneLoaderTest()
{

}

void UnitySceneLoaderTest::Start()
{
	_scene = API::CreateScene("Test");

	auto mainCam = API::GetMainCamera()->GetGameObject();
	mainCam->AddComponent<CameraMove>();

	auto skybox = API::CreateObject(_scene);
	auto skyboxComp = skybox->AddComponent<HDData::CubeMapRenderer>();
	skyboxComp->LoadCubeMapTexture("Day Sun Peak Clear.dds");

	//auto building = API::CreateObject(_scene);
	//building->AddComponent<HDData::MeshRenderer>();
	//auto comp = building->GetComponent<HDData::MeshRenderer>();
	//comp->LoadMesh("meshModel.fbx");

	//API::LoadSceneFromData("transformData.json");

	API::LoadScene(_scene);
}
