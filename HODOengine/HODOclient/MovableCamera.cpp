#include "MovableCamera.h"
#include "../HODOengine/HODO_API.h"
using namespace HDEngine;

MovableCamera::MovableCamera()
	: _moveSpeed(3.0f)
{

}

void MovableCamera::Update()
{
	_moveSpeed = 3.0f;
	if (API::GetKeyPressing(VK_LSHIFT))
	{
		_moveSpeed = 10.0f;
	}

	HDMath::HDFLOAT3 position = GetTransform()->GetWorldPosition();
	HDMath::HDQuaternion rotation = GetTransform()->GetWorldRotation();
	static HDMath::HDFLOAT2 lastMousePos = API::GetMousePosition();
	HDMath::HDFLOAT2 currentMousePos = API::GetMousePosition();
	HDMath::HDFLOAT2 deltaMousePos = currentMousePos - lastMousePos;
	lastMousePos = currentMousePos;

	if (API::GetKeyPressing('W'))
	{
		position += HDMath::HDFLOAT3(0.0f, 0.0f, 1.0f) * _moveSpeed * API::GetDeltaTime();
	}
	if (API::GetKeyPressing('A'))
	{
		position -= HDMath::HDFLOAT3(1.0f, 0.0f, 0.0f) * _moveSpeed * API::GetDeltaTime();
	}
	if (API::GetKeyPressing('S'))
	{
		position -= HDMath::HDFLOAT3(0.0f, 0.0f, 1.0f) * _moveSpeed * API::GetDeltaTime();
	}
	if (API::GetKeyPressing('D'))
	{
		position += HDMath::HDFLOAT3(1.0f, 0.0f, 0.0f) * _moveSpeed * API::GetDeltaTime();
	}
	if (API::GetKeyPressing('Q'))
	{
		position -= HDMath::HDFLOAT3(0.0f, 1.0f, 0.0f) * _moveSpeed * API::GetDeltaTime();
	}
	if (API::GetKeyPressing('E'))
	{
		position += HDMath::HDFLOAT3(0.0f, 1.0f, 0.0f) * _moveSpeed * API::GetDeltaTime();
	}
	if (API::GetKeyPressing(VK_RBUTTON))
	{
		HDMath::HDFLOAT3 newForward = rotation.Forward() - 
			rotation.Forward() * deltaMousePos.x * _moveSpeed * API::GetDeltaTime() * 0.025f +
			rotation.Right() * deltaMousePos.x * _moveSpeed * API::GetDeltaTime() * 0.025f;
		HDMath::HDFLOAT3 newUp = HDMath::HDFLOAT3::up;
		HDMath::HDFLOAT3 newRight = HDMath::HDFLOAT3::Cross(newUp, newForward);

		HDMath::HDQuaternion newRot = HDMath::HDQuaternion::FromLocalAxes(newRight, newUp, newForward);
		GetTransform()->SetWorldRotation(newRot);
	}
	GetTransform()->SetWorldPosition(position);
	HDData::Camera::Update();
}

