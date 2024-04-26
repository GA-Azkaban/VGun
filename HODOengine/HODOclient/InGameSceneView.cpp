#include "InGameSceneView.h"
#include "CameraMove.h"

InGameSceneView::InGameSceneView()
{

}

InGameSceneView::~InGameSceneView()
{

}

void InGameSceneView::Initialize()
{
	_scene = API::CreateScene("InGameScene");

	auto mainCam = _scene->GetMainCamera();
	mainCam->GetGameObject()->AddComponent<CameraMove>();

	API::LoadSceneFromData("sceneData.json", this->_scene);
}
