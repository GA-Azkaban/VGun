#include "Camera.h"
#include "HodoGIObject.h"
#include "Transform.h"
using namespace hodoData;

Camera* hodoData::Camera::_mainCamera = nullptr;

hodoData::Camera::Camera()
{
	_camera = hodoEngine::HodoGIObject::Instance().factory->CreateCamera();
	if (_mainCamera == nullptr)
	{
		SetMainCamera();
	}
}

void Camera::SetMainCamera()
{
	_mainCamera = this;
	_camera->SetAsMainCamera();
}

hodoData::Camera* Camera::GetMainCamera()
{
	return _mainCamera;
}

void Camera::Update()
{
	_camera->SetWorldTM(GetTransform()->GetWorldTM());
}