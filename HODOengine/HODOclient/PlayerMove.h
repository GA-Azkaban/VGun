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

public:
	void SetPlayerCamera(HDData::Camera* camera);

private:
	bool _isMoveActive;
	float _deltaTime;

	// check ~~ series
private:
	void CheckMoveDirection();
	void CheckLookDirection();
	bool CheckIsOnGround();
	Vector3 DecideMoveDirection(int direction);

private:
	// 마우스 회전에 따른 시야 변경 체크
	void CameraControl();
	void CameraMove();
	void Pitch(float rotationValue);
	void Yaw(float radian);
	void ToggleCameraView();

	// player's move
	Vector3 _nextPosition;
	Quaternion _nextRotation;

private:
	// 점프 관련
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
	Vector3 _prevCameraPos;
	Quaternion _prevCameraRot;
	bool _isCameraConnected;
};
