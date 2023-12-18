#include "PlayerMove.h"

PlayerMove::PlayerMove()
{

}

void PlayerMove::Start()
{

}

void PlayerMove::Update()
{
	// �÷��̾� �þ߸� üũ
	
	// �÷��̾ ���� ������ üũ
	CheckLookDirection();
	// �̵�
	Move(_moveDirection);
}

// ���̽�ƽ ����
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
