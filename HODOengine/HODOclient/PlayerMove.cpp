#include "PlayerMove.h"
#include "../HODOengine/DynamicCollider.h"

PlayerMove::PlayerMove()
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
	CheckIsOnGround();

	// 마우스에 따른 플레이어 회전 체크
	CheckLookDirection();

	// 키보드에 따른 플레이어 이동 방향 체크
	CheckMoveDirection();

	// camera move
	CameraMove();

	// 이동
	Move(_moveDirection);
}

// 조이스틱 개념
void PlayerMove::CheckMoveDirection()
{
	_moveDirection = 5;

	if (API::GetKeyPressing('I'))
	{
		_moveDirection = 8;
	}
	if (API::GetKeyPressing('L'))
	{
		_moveDirection = 6;
	}
	if (API::GetKeyPressing('K'))
	{
		_moveDirection = 2;
	}
	if (API::GetKeyPressing('J'))
	{
		_moveDirection = 4;
	}
	if (API::GetKeyPressing('I') && API::GetKeyPressing('J'))
	{
		_moveDirection = 7;
	}
	if (API::GetKeyPressing('I') && API::GetKeyPressing('L'))
	{
		_moveDirection = 9;
	}
	if (API::GetKeyPressing('J') && API::GetKeyPressing('K'))
	{
		_moveDirection = 1;
	}
	if (API::GetKeyPressing('K') && API::GetKeyPressing('L'))
	{
		_moveDirection = 3;
	}
	if (API::GetKeyDown(VK_SPACE))
	{
		Jump();
	}
}


void PlayerMove::CheckLookDirection()
{

}

bool PlayerMove::CheckIsOnGround()
{
	HDMath::HDFLOAT3 pos = this->GetTransform()->GetWorldPosition();
	const float delta = 0.2f;
	float x[9] = { -delta, -delta,0, delta,delta,delta,0,-delta,0 };
	float z[9] = { 0,delta,delta,delta,0,-delta,-delta,-delta,0 };

	for (int i = 0; i < 9; ++i)
	{
		//RocketEngine::RMFLOAT4 worldPos = RMFloat4MultiplyMatrix(RocketEngine::RMFLOAT4(pos.x + x[i], pos.y, pos.z + z[i], 1.0f), gameObject->transform.GetWorldTM());
		float halfHeight = _playerCollider->GetHeight();
		HDMath::HDFLOAT4 worldPos = HDMath::HDFLOAT4(pos.x + x[i], pos.y + 0.01f * i - halfHeight / 2.0f, pos.z + z[i], 1.0f);
		HDMath::HDFLOAT4 eachDir = worldPos;
		eachDir.y -= 0.05f;

		int type = 0;
		HDData::Collider* temp = API::ShootRay({ worldPos.x, worldPos.y, worldPos.z }, { 0.0f,-1.0f,0.0f }, 0.05f, &type);
		//RocketEngine::DrawDebugLine({ worldPos.x,worldPos.y,worldPos.z }, { eachDir.x,eachDir.y,eachDir.z });

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
	//if ((!_isJumping) && (_isOnGround))
	if (!_isJumping)
	{
		// 점프
		_playerCollider->Jump();
		_isJumping = true;
	}
}


HDMath::HDFLOAT3 PlayerMove::DecideMoveDirection(int direction)
{
	HDMath::HDFLOAT3 moveStep;

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
			moveStep = GetGameObject()->GetTransform()->GetForward() * _deltaTime * -_moveSpeed;
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
			moveStep = GetGameObject()->GetTransform()->GetRight() * _deltaTime * -_moveSpeed;
		}
		break;
		case 5:
		{
			moveStep = 0;
		}
		break;
		case 6:
		{
			moveStep = GetGameObject()->GetTransform()->GetRight() * _deltaTime * _moveSpeed;
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
			moveStep = GetGameObject()->GetTransform()->GetForward() * _deltaTime * _moveSpeed;
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

// 마우스 이동에 따른 시야 변경을 위한 함수
void PlayerMove::Pitch(float radian)
{

}

void PlayerMove::Yaw(float radian)
{

}

void PlayerMove::CameraMove()
{
	//Pitch();
	//Yaw();
}
