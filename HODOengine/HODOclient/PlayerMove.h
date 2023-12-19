#pragma once
#include "../HODOengine/HODO_API.h"

/// <summary>
/// �÷��̾� �����Ӱ� ���õ� ��ũ��Ʈ
/// </summary>

class PlayerMove : public HDData::Script
{
public:
	PlayerMove();

public:
	void Start() override;
	void Update() override;

private:
	bool _isMoveActive;
	float _deltaTime;

private:
	void CheckMoveDirection();
	void CheckLookDirection();
	void Move(int direction);

private:
	// ���콺�� ���� ī�޶� ȸ�� üũ
	void Pitch(float radian);
	void Yaw(float radian);

	float _moveSpeed;
	int _moveDirection;

private:
	void Jump();

private:
	bool _isJump;
	float _jumpCoolTime;
	

};
