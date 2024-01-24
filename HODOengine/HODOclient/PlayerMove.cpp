#include "PlayerMove.h"
#include "../HODOengine/DynamicCollider.h"

PlayerMove::PlayerMove()
	: _moveSpeed(5.0f)
{

}

void PlayerMove::Start()
{
	_playerCollider = this->GetGameObject()->GetComponent<HDData::DynamicBoxCollider>();
	_moveSpeed = 10.0f;


}

void PlayerMove::Update()
{
	// 델타 타임 체크
	_deltaTime = API::GetDeltaTime();

	// check on ground state
	//CheckIsOnGround();

	// 마우스에 따른 플레이어 회전 체크
	CheckLookDirection();

	// 키보드에 따른 플레이어 이동 방향 체크
	CheckMoveDirection();

	CameraControl();

	// 이동, 회전
	Move(_moveDirection);

	API::DrawLineDir({ 0.f,0.f,0.f }, GetTransform()->GetWorldPosition(), 10.0f, { 1.0f,0.0f,0.0f,1.0f });
}

void PlayerMove::SetPlayerCamera(HDData::Camera* camera)
{
	_playerCamera = camera;
}

// 조이스틱 개념
void PlayerMove::CheckMoveDirection()
{
	_moveDirection = 5;

	if (API::GetKeyPressing(DIK_I))
	{
		_moveDirection = 8;
	}
	if (API::GetKeyPressing(DIK_L))
	{
		_moveDirection = 6;
	}
	if (API::GetKeyPressing(DIK_K))
	{
		_moveDirection = 2;
	}
	if (API::GetKeyPressing(DIK_J))
	{
		_moveDirection = 4;
	}
	if (API::GetKeyPressing(DIK_I) && API::GetKeyPressing(DIK_J))
	{
		_moveDirection = 7;
	}
	if (API::GetKeyPressing(DIK_I) && API::GetKeyPressing(DIK_L))
	{
		_moveDirection = 9;
	}
	if (API::GetKeyPressing(DIK_J) && API::GetKeyPressing(DIK_K))
	{
		_moveDirection = 1;
	}
	if (API::GetKeyPressing(DIK_K) && API::GetKeyPressing(DIK_L))
	{
		_moveDirection = 3;
	}
	if (API::GetKeyDown(DIK_SPACE))
	{
		Jump();
	}
}


void PlayerMove::CheckLookDirection()
{

}

bool PlayerMove::CheckIsOnGround()
{
	Vector3 pos = this->GetTransform()->GetWorldPosition();
	const float delta = 0.2f;
	float x[9] = { -delta, -delta,0, delta,delta,delta,0,-delta,0 };
	float z[9] = { 0,delta,delta,delta,0,-delta,-delta,-delta,0 };

	for (int i = 0; i < 9; ++i)
	{
		//RocketEngine::RMFLOAT4 worldPos = RMFloat4MultiplyMatrix(RocketEngine::RMFLOAT4(pos.x + x[i], pos.y, pos.z + z[i], 1.0f), gameObject->transform.GetWorldTM());
		float halfHeight = _playerCollider->GetHeight() / 2.0f;
		Vector3 worldPos = Vector3(pos.x + x[i], pos.y + 0.01f * i - halfHeight, pos.z + z[i]);

		int type = 0;
		HDData::Collider* temp = API::ShootRay({ worldPos.x, worldPos.y, worldPos.z }, { 0.0f, -1.0f,0.0f }, 0.05f, &type);
		//RocketEngine::DrawDebugLine({ worldPos.x,worldPos.y,worldPos.z }, { eachDir.x,eachDir.y,eachDir.z });
		API::DrawLineDir(worldPos, Vector3(0.f, -1.f, 0.f), 0.05f, Vector4(1.f, 0.f, 0.f, 0.f));

		if (temp)
		{
			// type 1이 rigidStatic.
			if (type == 1)
			{
				// 상태 변경 및 착지 Sound.
				if (_isOnGround == false)
				{
					_isOnGround = true;
					_isJumping = false;
					//_playerAudio->Play3DOnce("landing");
					//_jumpCount = 0;
				}
				return true;
			}
		}
	}
	//_isOnGround = false;

	return false;
}

void PlayerMove::Move(int direction)
{
	// 조이스틱
	// 7 8 9
	// 4 5 6
	// 1 2 3

	//switch (direction)
	//{
	//	case 1:
	//	{
	//		moveStep =
	//			GetGameObject()->GetTransform()->GetForward() * _deltaTime * -_moveSpeed
	//			+ GetGameObject()->GetTransform()->GetRight() * _deltaTime * -_moveSpeed;
	//	}
	//	break;
	//	case 2:
	//	{
	//		moveStep = GetGameObject()->GetTransform()->GetForward() * _deltaTime * -_moveSpeed;
	//	}
	//	break;
	//	case 3:
	//	{
	//		moveStep =
	//			GetGameObject()->GetTransform()->GetForward() * _deltaTime * -_moveSpeed
	//			+ GetGameObject()->GetTransform()->GetRight() * _deltaTime * _moveSpeed;
	//	}
	//	break;
	//	case 4:
	//	{
	//		moveStep = GetGameObject()->GetTransform()->GetRight() * _deltaTime * -_moveSpeed;
	//	}
	//	break;
	//	case 5:
	//	{
	//		// 정지 상태
	//	}
	//	break;
	//	case 6:
	//	{
	//		moveStep = GetGameObject()->GetTransform()->GetRight() * _deltaTime * _moveSpeed;
	//	}
	//	break;
	//	case 7:
	//	{
	//		moveStep =
	//			GetGameObject()->GetTransform()->GetForward() * _deltaTime * _moveSpeed
	//			+ GetGameObject()->GetTransform()->GetRight() * _deltaTime * -_moveSpeed;
	//	}
	//	break;
	//	case 8:
	//	{
	//		moveStep = GetGameObject()->GetTransform()->GetForward() * _deltaTime * _moveSpeed;
	//	}
	//	break;
	//	case 9:
	//	{
	//		moveStep =
	//			GetGameObject()->GetTransform()->GetForward() * _deltaTime * _moveSpeed
	//			+ GetGameObject()->GetTransform()->GetRight() * _deltaTime * _moveSpeed;
	//	}
	//	break;
	//	default:
	//	{

	//	}
	//	break;
	//}

	// PhysX로 오브젝트 옮겨주기
	if (_moveDirection == 5)
	{
		if (_prevDirection != 0)
		{
			//_playerCollider->Move(DecideMovement(_prevDirection) * -30.0f);
			_playerCollider->Stop();
		}
	}
	else
	{
		_playerCollider->Move(DecideMoveDirection(_moveDirection));
	}

	_prevDirection = _moveDirection;
}

void PlayerMove::Jump()
{
	CheckIsOnGround();

	if ((!_isJumping) && (_isOnGround))
		//if (!_isJumping)
	{
		// 점프
		_playerCollider->Jump();
		_isJumping = true;
		_isOnGround = false;
	}
}


Vector3 PlayerMove::DecideMoveDirection(int direction)
{
	Vector3 moveStep;

	switch (direction)
	{
		case 1:
		{
			moveStep =
				GetGameObject()->GetTransform()->GetForward() * _deltaTime * -_moveSpeed * 0.7f
				+ GetGameObject()->GetTransform()->GetRight() * _deltaTime * -_moveSpeed * 0.7f;
		}
		break;
		case 2:
		{
			moveStep = GetTransform()->GetForward() * _deltaTime * -_moveSpeed;
		}
		break;
		case 3:
		{
			moveStep =
				GetGameObject()->GetTransform()->GetForward() * _deltaTime * -_moveSpeed * 0.7f
				+ GetGameObject()->GetTransform()->GetRight() * _deltaTime * _moveSpeed * 0.7f;
		}
		break;
		case 4:
		{
			moveStep = GetTransform()->GetRight() * _deltaTime * -_moveSpeed;
		}
		break;
		case 5:
		{
			moveStep = 0;
		}
		break;
		case 6:
		{
			moveStep = GetTransform()->GetRight() * _deltaTime * _moveSpeed;
		}
		break;
		case 7:
		{
			moveStep =
				GetGameObject()->GetTransform()->GetForward() * _deltaTime * _moveSpeed * 0.7f
				+ GetGameObject()->GetTransform()->GetRight() * _deltaTime * -_moveSpeed * 0.7f;
		}
		break;
		case 8:
		{
			moveStep = GetTransform()->GetForward() * _deltaTime * _moveSpeed;
		}
		break;
		case 9:
		{
			moveStep =
				GetGameObject()->GetTransform()->GetForward() * _deltaTime * _moveSpeed * 0.7f
				+ GetGameObject()->GetTransform()->GetRight() * _deltaTime * _moveSpeed * 0.7f;
		}
		break;
		default:
		{

		}
		break;
	}

	return moveStep;
}

void PlayerMove::CameraControl()
{

	if (API::GetKeyDown(DIK_P))
	{
		ToggleCameraView();
	}

	// camera move
	if (_isCameraConnected)
	{
		CameraMove();
	}
}

// 마우스 이동에 따른 시야 변경을 위한 함수
void PlayerMove::Pitch(float rotationValue)
{
	HDData::Transform* cameraTransform = _playerCamera->GetGameObject()->GetTransform();
	Quaternion rot = cameraTransform->GetLocalRotation();
	float eulerAngleX = std::atan2(2.0f * (rot.w * rot.x + rot.y * rot.z), 1.0f - 2.0f * (rot.x * rot.x + rot.y * rot.y));

	if (89.0f < eulerAngleX)
	{
		constexpr float radX = HDMath::ToRadian(89.0f) * 0.5f;
		Quaternion closedAngle = { std::cos(radX), std::sin(radX), 0.f, 0.f };

		cameraTransform->SetLocalRotation(closedAngle);
	}
	else if (eulerAngleX < -89.0f)
	{
		constexpr float radX = HDMath::ToRadian(-89.0f) * 0.5f;
		Quaternion closedAngle = { std::cos(radX), std::sin(radX), 0.f, 0.f };

		cameraTransform->SetLocalRotation(closedAngle);
	}
	else
	{
		Vector4 rotationAxis{ 1.f, 0.f, 0.f, 1.0f };
		rotationAxis = Vector4MultiplyMatrix(rotationAxis, cameraTransform->GetLocalRotationMatrix());
		Quaternion newRot = HDMath::HDRotateQuaternion(cameraTransform->GetLocalRotation(),
			{ rotationAxis.x, rotationAxis.y, rotationAxis.z }, rotationValue);
		cameraTransform->SetLocalRotation(newRot);
	}
}

void PlayerMove::Yaw(float radian)
{
	// rotation along Z-direction. necessary?
}

void PlayerMove::ToggleCameraView()
{
	HDData::Transform* playerTransform = this->GetGameObject()->GetTransform();
	HDData::Transform* cameraTransform = _playerCamera->GetGameObject()->GetTransform();

	if (!_isCameraConnected)
	{
		HDData::Transform* playerTransform = this->GetGameObject()->GetTransform();
		HDData::Transform* cameraTransform = _playerCamera->GetGameObject()->GetTransform();

		_prevCameraPos = cameraTransform->GetWorldPosition();
		_prevCameraRot = cameraTransform->GetWorldRotation();

		cameraTransform->SetWorldPosition(playerTransform->GetWorldPosition() + Vector3(0.f, 4.f, 0.f));
		cameraTransform->SetWorldRotation(playerTransform->GetWorldRotation());
		_playerCamera->GetGameObject()->SetParentObject(this->GetGameObject());
		_isCameraConnected = true;
	}
	else
	{
		_playerCamera->GetGameObject()->SetParentObject(nullptr);
		cameraTransform->SetWorldPosition(_prevCameraPos);
		cameraTransform->SetWorldRotation(_prevCameraRot);
		_isCameraConnected = false;
	}
}

void PlayerMove::CameraMove()
{
	Vector2 mouseDelta = API::GetMouseDelta();

	// RotateY
	_playerCollider->Rotate(mouseDelta.x * 0.002f);	// adjust sensitivity later (0.002f -> variable)

	// Pitch in closed angle
	Pitch(mouseDelta.y * 0.002f);
}
