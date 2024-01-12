#include "PlayerMove.h"

PlayerMove::PlayerMove()
	: _moveSpeed(5.0f)
{

}

void PlayerMove::Start()
{

}

void PlayerMove::Update()
{
	// 델타 타임 체크
	_deltaTime = API::GetDeltaTime();

	// 마우스에 따른 플레이어 회전 체크
	CheckLookDirection();

	// 키보드에 따른 플레이어 이동 방향 체크
	CheckMoveDirection();

	// 이동
	Move(_moveDirection);

	API::DrawLineDir({ 0.f,0.f,0.f }, GetTransform()->GetWorldPosition(), 10.0f, { 1.0f,0.0f,0.0f,1.0f });
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

void PlayerMove::Move(int direction)
{
	// 조이스틱
	// 7 8 9
	// 4 5 6
	// 1 2 3

	HDMath::HDFLOAT3 moveStep;

	switch (direction)
	{
		case 1:
		{
			moveStep =
				GetTransform()->GetForward() * _deltaTime * -_moveSpeed
				+ GetTransform()->GetRight() * _deltaTime * -_moveSpeed;
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
				GetTransform()->GetForward() * _deltaTime * -_moveSpeed
				+ GetTransform()->GetRight() * _deltaTime * _moveSpeed;
		}
		break;
		case 4:
		{
			moveStep = GetTransform()->GetRight() * _deltaTime * -_moveSpeed;
		}
		break;
		case 5:
		{
			// 정지 상태
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
				GetTransform()->GetForward() * _deltaTime * _moveSpeed
				+ GetTransform()->GetRight() * _deltaTime * -_moveSpeed;
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
				GetTransform()->GetForward() * _deltaTime * _moveSpeed
				+ GetTransform()->GetRight() * _deltaTime * _moveSpeed;
		}
		break;
		default:
		{

		}
		break;
	}

	GetTransform()->Translate(moveStep);
}

void PlayerMove::Jump()
{
	if (_isJump == false)
	{
		// 점프
		_isJump = true;
	}
}


// 마우스 이동에 따른 시야 변경을 위한 함수
void PlayerMove::Pitch(float radian)
{
	
}

void PlayerMove::Yaw(float radian)
{

}
