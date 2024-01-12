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

	// check ~~ series
private:
	void CheckMoveDirection();
	void CheckLookDirection();
	bool CheckIsOnGround();
	HDMath::HDFLOAT3 DecideMoveDirection(int direction);

private:
	// 마우스에 따른 카메라 회전 체크
	void CameraMove();
	void Pitch(float radian);
	void Yaw(float radian);

	// player's move
private:
	void Jump();
	void Move(int direction);

private:
	bool _isJumping;
	bool _isOnGround;
	float _moveSpeed;
	int _moveDirection;
	int _prevDirection;	// useless on immediate stop mechanism
	
private:
	HDData::DynamicBoxCollider* _playerCollider;
	HDData::Camera* _playerCamera;
};
