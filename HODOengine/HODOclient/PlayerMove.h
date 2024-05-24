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
	void SetMovable(bool movable);
	void SetPlayerCamera(HDData::Camera* camera);
	void SetPlayerText(HDData::TextUI* pos, HDData::TextUI* aim);
	void SetHitParticle(std::vector<HDData::ParticleSphereCollider*> particleVec);
	void SetHeadCam(HDData::Camera* cam);
	HDData::Camera* GetHeadCam() const;
	void PresetSprayPattern();
	void StartRoundCam();

	// check ~~ series
private:
	void CheckMoveInfo();
	void CheckLookDirection();
	bool CheckIsOnGround();
	Vector3 DecideDisplacement(int direction);
	void UpdatePlayerPositionDebug();

	// 마우스 회전에 따른 시야 변경 체크
private:
	void CameraControl();
	void CameraMove();
	void SwitchCamera();
	void ToggleCam();

	// 이동 관련
private:
	void Jump();
	void Move(int direction);
	void Pitch(float rotationValue);

	// 사격 관련
private:
	void ShootGun();
	void ShootGunDdabal();
	void Reload();
	void SpawnParticle(Vector3 position);
	void ApplyRecoil();

private:
	bool _isMovable;
	bool _isJumping;
	bool _isOnGround;
	bool _isFirstPersonPerspective;
	bool _isHeadCam;

private:
	float _deltaTime;
	float _moveSpeed;
	int _moveDirection;
	int _prevDirection;

private:
	//HDData::DynamicBoxCollider* _playerCollider;
	HDData::DynamicCapsuleCollider* _playerCollider;
	HDData::Camera* _playerCamera;
	HDData::Camera* _headCam;
	HDData::Camera* _prevCam;
	std::vector<HDData::ParticleSphereCollider*> _hitParticles;

private:
	HDData::AudioSource* _playerAudio = nullptr;

private:
	HDData::TextUI* _playerInfoText;
	HDData::TextUI* _aimText;
	HDData::TextUI* _hitText;

	Vector3 _prevCameraPos;
	Quaternion _prevCameraRot;
	float _rotAngleX;
	float _rotAngleY;

	int _particleIndex;
	float _shootCooldown;
	float _jumpCooldown;
	int _shootCount;
	int _bulletCount;
	float _reloadTimer;
	bool _isReloading;
	bool _isRunning;
	bool _tempFlag = 0;
	std::pair<float, float> _sprayPattern[30];
	std::pair<HDData::DynamicBoxCollider*, HDData::DynamicBoxCollider*> _footColliders;
};
