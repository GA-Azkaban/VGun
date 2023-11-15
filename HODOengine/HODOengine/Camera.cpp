#include "Camera.h"
#include "HodoGIObject.h"
#include "Transform.h"
using namespace hodoEngine;

Camera* hodoEngine::Camera::_mainCamera = nullptr;

hodoEngine::Camera::Camera()
{
	_camera = HodoGIObject::Instance().factory->CreateCamera();
	if (_mainCamera == nullptr)
	{
		SetMainCamera();
	}
}

hodoEngine::Camera* Camera::GetMainCamera()
{
	return _mainCamera;
}

void Camera::SetMainCamera()
{
	_mainCamera = this;
	_camera->SetAsMainCamera();
}

void Camera::Update()
{
	_camera->SetWorldTM(GetTransform()->GetWorldTM());
}