#pragma once
#include "PlayerMoveState.h"
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
	void SetPlayerColliders(HDData::DynamicCapsuleCollider* standing, HDData::DynamicCapsuleCollider* sitting);
	void PresetSprayPattern(int gunType);
	void StartRoundCam();

public:
	bool IsShootHead();
	bool IsShootBody();
	bool GetIsIngamePlaying();
	void SetIsIngamePlaying(bool isPlaying);

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
	void Jump(Vector3 direction);
	void Move(int direction);
	void Pitch(float rotationValue);
	void ToggleSit(bool isSit);
	void Die();
	void Respawn();

	// 사격 관련
private:
	void DecidePlayerState();
	void DecidePlayerStateSecond();
	void Behavior();
	void CoolTime();
	void Landing();
	void ShootGun();
	void Reload();
	void SpawnParticle(Vector3 position);
	void ApplyRecoil();
	void ShootTrail(Vector3 endPoint);
	void Tumble(Vector3 direction);
	void PlayPlayerSound();
	virtual void OnEnable() override;
	void OnStateEnter(ePlayerMoveState state);
	void OnStateStay(ePlayerMoveState state);
	void OnStateExit(ePlayerMoveState state);
	void UpdateStateText();
	//Quaternion Slerp(const Quaternion& qa, const Quaternion& qb, float time);

public:
	int& GetBulletCount();
	ePlayerMoveState GetPlayerMoveEnum(int index);
public:
	virtual void OnCollisionEnter(HDData::PhysicsCollision** colArr, unsigned int count) override;
	virtual void OnCollisionExit(HDData::PhysicsCollision** colArr, unsigned int count) override;
	virtual void OnTriggerEnter(HDData::Collider** colArr, unsigned int count) override;
	virtual void OnTriggerExit(HDData::Collider** colArr, unsigned int count) override;

private:
	bool _isMovable;
	bool _isJumping;
	bool _isOnGround;
	bool _isFirstPersonPerspective;
	bool _isHeadCam;
	bool _isMoveableOnJump;

private:
	float _deltaTime;
	float _moveSpeed;
	int _moveDirection;
	int _prevDirection;
	bool _isShootHead;
	bool _isShootBody;

public:
	// 디버그용 state 표시
	HDData::TextUI* _plStateText;
	HDData::TextUI* _tumbleText;
	HDData::TextUI* _plPosText;
	HDData::TextUI* _anyText;
	int melon;

private:
	//HDData::DynamicBoxCollider* _playerCollider;
	HDData::DynamicCapsuleCollider* _playerColliderStanding;
	HDData::DynamicCapsuleCollider* _playerColliderSitting;
	HDData::Camera* _playerCamera;
	HDData::Camera* _headCam;
	HDData::Camera* _prevCam;
	HDData::GameObject* _fpMeshObj;
	HDData::SkinnedMeshRenderer* _fpmesh;
	HDData::MeshRenderer* _weapon;
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
	float _prevRotAngleX;
	float _prevRotAngleY;

	int _particleIndex;
	float _shootCooldown;
	float _jumpCooldown;
	float _tumbleCooldown;
	int _shootCount;
	int& _bulletCount;
	float _reloadTimer;
	float _tumbleTimer;
	bool _isReloading;
	bool _isRunning;
	bool _tempFlag = 0;
	bool _isDie;

	Vector3 _tumbleDirection;
	Vector3 _playerPos;
	Vector3 _hitPoint;

	int _enterCount = 0;
	// 상태 중첩을 표현하기 위해. 2번째 요소에는 shoot, reload만.
	std::pair<ePlayerMoveState, ePlayerMoveState> _prevPlayerState;
	std::pair<ePlayerMoveState, ePlayerMoveState> _playerState;

	std::pair<float, float> _sprayPattern[30];
	std::pair<float, float> _sprayCamera[30];
	std::pair<HDData::DynamicBoxCollider*, HDData::DynamicBoxCollider*> _footColliders;

public:
	bool _isIngamePlaying;
};
