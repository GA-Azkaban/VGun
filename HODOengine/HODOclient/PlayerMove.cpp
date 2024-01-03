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

	// 마우스에 따른 플레이어 회전 체크
	CheckLookDirection();

	// 키보드에 따른 플레이어 이동 방향 체크
	CheckMoveDirection();

	_playerCollider->CheckIsOnGround();

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
		_playerCollider->Move(DecideMovement(_moveDirection));
	}

	_prevDirection = _moveDirection;
}

void PlayerMove::Jump()
{
	if ((!_isJumping) && (_isOnGround))
	{
		// 점프
		_playerCollider->Jump();
		_isJumping = true;
	}
}


HDMath::HDFLOAT3 PlayerMove::DecideMovement(int direction)
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
