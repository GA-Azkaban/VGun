#include "CameraMove.h"

CameraMove::CameraMove()
	: moveSpeed(5.0f),
	rotateSpeed(5.0f)
{
	
}

void CameraMove::Start()
{
	_camera = GetGameObject()->GetComponent<HDData::Camera>();
}

void CameraMove::Update()
{
	if (!_camera->GetCamActive())
	{
		return;
	}

	float deltaTime = API::GetDeltaTime();
	moveSpeed = 5.0f;

	if (API::GetKeyPressing(DIK_LALT))
	{
		moveSpeed *= 5.0f;
	}

	if (API::GetKeyPressing(DIK_W))
	{
		Walk(moveSpeed * deltaTime);
	}

	if (API::GetKeyPressing(DIK_S))
	{
		Walk(-moveSpeed * deltaTime);
	}

	if (API::GetKeyPressing(DIK_A))
	{
		Strafe(-moveSpeed * deltaTime);
	}

	if (API::GetKeyPressing(DIK_D))
	{
		Strafe(moveSpeed * deltaTime);
	}

	if (API::GetKeyPressing(DIK_Q))
	{
		WorldUpDown(-moveSpeed * deltaTime);
	}

	if (API::GetKeyPressing(DIK_E))
	{
		WorldUpDown(moveSpeed * deltaTime);
	}

	if (API::GetKeyPressing(DIK_LEFTARROW))
	{
		Yaw(-rotateSpeed * deltaTime / 4);
	}

	if (API::GetKeyPressing(DIK_RIGHTARROW))
	{
		Yaw(rotateSpeed * deltaTime / 4);
	}

	if (API::GetKeyPressing(DIK_DOWNARROW))
	{
		Pitch(rotateSpeed * deltaTime * 10);
	}

	if (API::GetKeyPressing(DIK_UPARROW))
	{
		Pitch(-rotateSpeed * deltaTime * 10);
	}
}

void CameraMove::Strafe(float delta)
{
	Vector3 rightVec = GetGameObject()->GetTransform()->GetRight();
	rightVec.x *= delta;
	rightVec.y *= delta;
	rightVec.z *= delta;

	GetGameObject()->GetTransform()->Translate(rightVec);
}

void CameraMove::Walk(float delta)
{
	Vector3 forwardVec = GetGameObject()->GetTransform()->GetForward();
	forwardVec.x *= delta;
	forwardVec.y *= delta;
	forwardVec.z *= delta;

	GetGameObject()->GetTransform()->Translate(forwardVec);
}

void CameraMove::WorldUpDown(float delta)
{
	Vector3 worldUpDelta = { 0.0f,delta,0.0f };
	GetGameObject()->GetTransform()->Translate(worldUpDelta);
}

void CameraMove::Yaw(float angle)
{
	Quaternion rotQuat = Quaternion::CreateFromAxisAngle({ 0.0f,1.0f,0.0f }, angle);
	Quaternion result = Quaternion::Concatenate(rotQuat, GetGameObject()->GetTransform()->GetLocalRotation());
	GetGameObject()->GetTransform()->SetLocalRotation(result);
}

void CameraMove::Pitch(float angle)
{
	GetTransform()->Rotate(angle, 0.0f, 0.0f);
}

void CameraMove::Roll(float angle)
{
	GetTransform()->Rotate(0.0f, 0.0f, angle);
}

