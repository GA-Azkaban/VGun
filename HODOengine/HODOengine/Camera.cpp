#include "Camera.h"
#include "Transform.h"
using namespace HDData;

Camera* HDData::Camera::_mainCamera = nullptr;

HDData::Camera::Camera()
{
	if (_mainCamera == nullptr)
	{
		SetMainCamera();
	}
}

void Camera::SetMainCamera()
{
	_mainCamera = this;
	//_camera->SetAsMainCamera();
}

HDData::Camera* Camera::GetMainCamera()
{
	return _mainCamera;
}

void Camera::Update()
{
	//_camera->SetWorldTM(GetTransform()->GetWorldTM());
}