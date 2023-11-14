#include "Camera.h"
using namespace hodoEngine;



Camera* hodoEngine::Camera::_mainCamera = nullptr;

hodoEngine::Camera::Camera()
{
	//_camera = 
}

hodoEngine::Camera* Camera::GetMainCamera()
{
	return _mainCamera;
}

void Camera::SetMainCamera()
{
	_mainCamera = this;
}

void Camera::Update()
{

}