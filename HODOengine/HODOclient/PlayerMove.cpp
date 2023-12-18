#include "PlayerMove.h"

PlayerMove::PlayerMove()
{

}

void PlayerMove::Start()
{

}

void PlayerMove::Update()
{
	// 플레이어 시야를 체크
	
	// 플레이어가 누른 방향을 체크
	CheckLookDirection();
	// 이동
	Move(_moveDirection);
}

// 조이스틱 개념
void PlayerMove::CheckLookDirection()
{
	if (API::GetKeyPressing('W'))
	{
		_moveDirection = 8;
	}
	if (API::GetKeyPressing('D'))
	{
		_moveDirection = 6;
	}
	if (API::GetKeyPressing('S'))
	{
		_moveDirection = 2;
	}
	if (API::GetKeyPressing('A'))
	{
		_moveDirection = 4;
	}
	if (API::GetKeyPressing('W') && API::GetKeyPressing('A'))
	{
		_moveDirection = 7;
	}
	if (API::GetKeyPressing('W') && API::GetKeyPressing('D'))
	{
		_moveDirection = 9;
	}
	if (API::GetKeyPressing('A') && API::GetKeyPressing('S'))
	{
		_moveDirection = 1;
	}
	if (API::GetKeyPressing('S') && API::GetKeyPressing('D'))
	{
		_moveDirection = 3;
	}
}


void PlayerMove::Move(int direction)
{
	switch (direction)
	{
		default:
			break;
	}
}

void PlayerMove::Pitch()
{

}

void PlayerMove::Roll()
{

}

void PlayerMove::Yaw()
{

}
