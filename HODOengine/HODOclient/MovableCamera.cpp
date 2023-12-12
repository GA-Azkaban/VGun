#include "MovableCamera.h"
#include "../HODOengine/HODO_API.h"
using namespace API;

MovableCamera::MovableCamera()
	: _moveSpeed(3.0f)
{

}

void MovableCamera::Update()
{
	_moveSpeed = 3.0f;
	if (GetKeyPressing(VK_LSHIFT))
	{
		_moveSpeed = 10.0f;
	}

	HDMaths::HDFLOAT3 position = GetTransform()->GetWorldPosition();
	HDMaths::HDQuaternion rotation = GetTransform()->GetWorldRotation();
	static HDMaths::HDFLOAT2 lastMousePos = GetMousePosition();
	HDMaths::HDFLOAT2 currentMousePos = GetMousePosition();
	HDMaths::HDFLOAT2 deltaMousePos = currentMousePos - lastMousePos;
	lastMousePos = currentMousePos;

	if (GetKeyPressing('W'))
	{
		position -= HDMaths::HDFLOAT3(0.0f, 0.0f, 1.0f) * _moveSpeed * GetDeltaTime();
	}
	if (GetKeyPressing('A'))
	{
		position += HDMaths::HDFLOAT3(1.0f, 0.0f, 0.0f) * _moveSpeed * GetDeltaTime();
	}
	if (GetKeyPressing('S'))
	{
		position += HDMaths::HDFLOAT3(0.0f, 0.0f, 1.0f) * _moveSpeed * GetDeltaTime();
	}
	if (GetKeyPressing('D'))
	{
		position -= HDMaths::HDFLOAT3(1.0f, 0.0f, 0.0f) * _moveSpeed * GetDeltaTime();
	}
	if (GetKeyPressing('Q'))
	{
		position += HDMaths::HDFLOAT3(0.0f, 1.0f, 0.0f) * _moveSpeed * GetDeltaTime();
	}
	if (GetKeyPressing('E'))
	{
		position -= HDMaths::HDFLOAT3(0.0f, 1.0f, 0.0f) * _moveSpeed * GetDeltaTime();
	}
	if (GetKeyPressing(VK_RBUTTON))
	{
		HDMaths::HDFLOAT3 newForward = rotation.Forward() - 
			rotation.Forward() * deltaMousePos.x * _moveSpeed * GetDeltaTime() * 0.05f +
			rotation.Right() * deltaMousePos.x * _moveSpeed * GetDeltaTime() * 0.05f;
		HDMaths::HDFLOAT3 newUp = HDMaths::HDFLOAT3::up;
		HDMaths::HDFLOAT3 newRight = HDMaths::HDFLOAT3::Cross(newUp, newForward);

		HDMaths::HDQuaternion newRot = HDMaths::HDQuaternion::FromLocalAxes(newRight, newUp, newForward);
		GetTransform()->SetWorldRotation(newRot);
	}
	GetTransform()->SetWorldPosition(position);
	hodoData::Camera::Update();
}

