#include "Camera.h"
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
