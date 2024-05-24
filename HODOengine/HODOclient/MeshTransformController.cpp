#include "MeshTransformController.h"

MeshTransformController::MeshTransformController()
{

}

void MeshTransformController::Start()
{

}

void MeshTransformController::Update()
{
	Vector3 pos = GetGameObject()->GetTransform()->GetLocalPosition();
	float deltaTime = API::GetDeltaTime();
	float angleX = 0.0f;
	float angleY = 0.0f;
	float angleZ = 0.0f;

	if (API::GetKeyPressing(DIK_1))
	{
		GetGameObject()->GetTransform()->SetLocalPosition(pos.x + 50.0f * deltaTime, pos.y, pos.z);
	}
	if (API::GetKeyPressing(DIK_2))
	{
		GetGameObject()->GetTransform()->SetLocalPosition(pos.x - 50.0f * deltaTime, pos.y, pos.z);
	}

	if (API::GetKeyPressing(DIK_3))
	{
		GetGameObject()->GetTransform()->SetLocalPosition(pos.x, pos.y + 50.0f * deltaTime, pos.z);
	}
	if (API::GetKeyPressing(DIK_4))
	{
		GetGameObject()->GetTransform()->SetLocalPosition(pos.x, pos.y - 50.0f * deltaTime, pos.z);
	}

	if (API::GetKeyPressing(DIK_5))
	{
		GetGameObject()->GetTransform()->SetLocalPosition(pos.x, pos.y, pos.z + 50.0f * deltaTime);
	}
	if (API::GetKeyPressing(DIK_6))
	{
		GetGameObject()->GetTransform()->SetLocalPosition(pos.x, pos.y, pos.z - 50.0f * deltaTime);
	}

	if (API::GetKeyPressing(DIK_Z))
	{
		angleX += 10.0f * deltaTime;
	}
	if (API::GetKeyPressing(DIK_X))
	{
		angleX -= 10.0f * deltaTime;
	}

	if (API::GetKeyPressing(DIK_C))
	{
		angleY += 10.0f * deltaTime;
	}
	if (API::GetKeyPressing(DIK_V))
	{
		angleY -= 10.0f * deltaTime;
	}

	if (API::GetKeyPressing(DIK_B))
	{
		angleZ += 10.0f * deltaTime;
	}
	if (API::GetKeyPressing(DIK_N))
	{
		angleZ -= 10.0f * deltaTime;
	}

	GetGameObject()->GetTransform()->Rotate(angleX, angleY, angleZ);
	
}

