#include "MeshTransformController.h"

MeshTransformController::MeshTransformController()
	: angleX(0.0f), angleY(0.0f), angleZ(0.0f)
{

}

void MeshTransformController::Start()
{

}

void MeshTransformController::Update()
{
	Vector3 pos = GetGameObject()->GetTransform()->GetLocalPosition();
	float deltaTime = API::GetDeltaTime();
	if (API::GetKeyPressing(DIK_1))
	{
		GetGameObject()->GetTransform()->SetLocalPosition(pos.x + 1000.0f * deltaTime, 0.0f, 0.0f);
	}
	if (API::GetKeyPressing(DIK_2))
	{
		GetGameObject()->GetTransform()->SetLocalPosition(pos.x - 1000.0f * deltaTime, 0.0f, 0.0f);
	}

	if (API::GetKeyPressing(DIK_3))
	{
		GetGameObject()->GetTransform()->SetLocalPosition(0.0f, pos.y + 1000.0f * deltaTime, 0.0f);
	}
	if (API::GetKeyPressing(DIK_4))
	{
		GetGameObject()->GetTransform()->SetLocalPosition(0.0f, pos.y - 1000.0f * deltaTime, 0.0f);
	}

	if (API::GetKeyPressing(DIK_5))
	{
		GetGameObject()->GetTransform()->SetLocalPosition(0.0f, 0.0f, pos.z + 1000.0f * deltaTime);
	}
	if (API::GetKeyPressing(DIK_6))
	{
		GetGameObject()->GetTransform()->SetLocalPosition(0.0f, 0.0f, pos.z - 1000.0f * deltaTime);
	}

	if (API::GetKeyPressing(DIK_Z))
	{
		angleX += 1.0f * deltaTime;
	}
	if (API::GetKeyPressing(DIK_X))
	{
		angleX -= 1.0f * deltaTime;
	}

	if (API::GetKeyPressing(DIK_C))
	{
		angleY += 1.0f * deltaTime;
	}
	if (API::GetKeyPressing(DIK_V))
	{
		angleY -= 1.0f * deltaTime;
	}

	if (API::GetKeyPressing(DIK_B))
	{
		angleZ += 1.0f * deltaTime;
	}
	if (API::GetKeyPressing(DIK_N))
	{
		angleZ -= 1.0f * deltaTime;
	}

	GetGameObject()->GetTransform()->Rotate(angleX, angleY, angleZ);
}

