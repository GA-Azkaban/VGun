#pragma once
#include "../HODOengine/HODO_API.h"

/// <summary>
/// 플레이어 움직임과 관련된 스크립트
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
	// 마우스 회전에 따른 시야 변경 체크
	void Pitch(float radian);
	void Yaw(float radian);

	float _moveSpeed;
	int _moveDirection;

	HDMath::HDFLOAT3 _nextPosition;
	HDMath::HDQuaternion _nextRotation;

private:
	// 점프 관련
	void Jump();

private:
	bool _isJump;
	float _jumpCoolTime;

};
