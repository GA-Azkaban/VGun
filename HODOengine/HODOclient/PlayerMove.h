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
	// 마우스에 따른 카메라 회전 체크
	void Pitch(float radian);
	void Yaw(float radian);

	float _moveSpeed;
	int _moveDirection;
	int _prevDirection;

private:
	void Jump();
	HDMath::HDFLOAT3 DecideMovement(int direction);

private:
	bool _isJumping;
	bool _isOnGround;
	
private:
	HDData::DynamicBoxCollider* _playerCollider;

};
