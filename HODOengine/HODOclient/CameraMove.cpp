#include "CameraMove.h"

CameraMove::CameraMove()
	: moveSpeed(2.0f)
{
}

void CameraMove::Start()
{

}

void CameraMove::Update()
{
	//float deltaTime = rocket.GetDeltaTime();
	float deltaTime = API::GetDeltaTime();
	moveSpeed = 5.0f;

	if (API::GetKeyPressing(VK_SHIFT))
	{
		moveSpeed *= 2.0f;
	}

	if (API::GetKeyPressing('W'))
	{
		Walk(moveSpeed * deltaTime);
	}

	if (API::GetKeyPressing('S'))
	{
		Walk(-moveSpeed * deltaTime);
	}

	if (API::GetKeyPressing('A'))
	{
		Strafe(-moveSpeed * deltaTime);
	}

	if (API::GetKeyPressing('D'))
	{
		Strafe(moveSpeed * deltaTime);
	}

	if (API::GetKeyPressing('Q'))
	{
		WorldUpDown(-moveSpeed * deltaTime);
	}

	if (API::GetKeyPressing('E'))
	{
		WorldUpDown(moveSpeed * deltaTime);
	}

	if (API::GetKeyPressing(VK_LEFT))
	{
		RotateY(-moveSpeed * deltaTime);
	}

	if (API::GetKeyPressing(VK_RIGHT))
	{
		RotateY(moveSpeed * deltaTime);
	}

	if (API::GetKeyPressing(VK_DOWN))
	{
		Pitch(moveSpeed * deltaTime / 4);
	}

	if (API::GetKeyPressing(VK_UP))
	{
		Pitch(-moveSpeed * deltaTime / 4);
	}


	OnMouseMove();
}

void CameraMove::OnMouseMove()
{
	if (!API::GetKeyPressing(VK_RBUTTON))
	{
		return;
	}

// 	RocketEngine::RMFLOAT2 mouseDelta = RocketEngine::GetMouseDelta();
// 	mouseDelta = mouseDelta * RocketEngine::GetDeltaTime();
// 	_camera->Pitch(mouseDelta.y);
// 	_camera->RotateY(mouseDelta.x);
}

void CameraMove::Strafe(float delta)
{
	HDMath::HDFLOAT3 rightVec = GetGameObject()->GetTransform()->GetRight();
	rightVec.x *= delta;
	rightVec.y *= delta;
	rightVec.z *= delta;

	GetGameObject()->GetTransform()->Translate(rightVec);
}

void CameraMove::Walk(float delta)
{
	HDMath::HDFLOAT3 forwardVec = GetGameObject()->GetTransform()->GetForward();
	forwardVec.x *= delta;
	forwardVec.y *= delta;
	forwardVec.z *= delta;

	GetGameObject()->GetTransform()->Translate(forwardVec);
}

void CameraMove::WorldUpDown(float delta)
{
	HDMath::HDFLOAT3 worldUpDelta = { 0.0f,delta,0.0f };
	GetGameObject()->GetTransform()->Translate(worldUpDelta);
}

void CameraMove::Pitch(float radian)
{
	HDMath::HDFLOAT3 r = GetTransform()->GetLocalRotation()* HDMath::HDFLOAT3(1.0f, 0.0f, 0.0f);
	HDMath::HDQuaternion newRot = HDRotateQuaternion(GetGameObject()->GetTransform()->GetLocalRotation(), { r.x,r.y,r.z }, radian);
	GetGameObject()->GetTransform()->SetLocalRotation(newRot);
}

void CameraMove::RotateY(float angle)
{
	HDMath::HDQuaternion newRot = HDRotateQuaternion(GetGameObject()->GetTransform()->GetLocalRotation(), { 0.0f,1.0f,0.0f }, angle);
	GetGameObject()->GetTransform()->SetLocalRotation(newRot);
}