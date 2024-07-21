#include "PlayerMove.h"
#include "../HODOengine/DynamicCollider.h"
#include "PlayerInfo.h"
#include "GameManager.h"
#include "RoundManager.h"
#include "NetworkManager.h"
#include "CooldownAlpha.h"
#include "CooldownText.h"

PlayerMove::PlayerMove()
	: _isMovable(true), _isMoveableOnJump(true),
	_particleIndex(0),
	_shootCooldown(0.0f),
	_jumpCooldown(0.0f),
	_tumbleCooldown(0.0f),
	_shootCount(0),
	_bulletCount(GameManager::Instance()->GetMyInfo()->GetCurrentBulletCount()),
	_reloadTimer(0.0f),
	_isReloading(false),
	_isRunning(false),
	_isDie(false),
	_rotAngleX(0.0f), _rotAngleY(0.0f),
	_isFirstPersonPerspective(true),
	_isJumping(true), _isOnGround(false),
	_isShootHead(false), _isShootBody(false),
	_prevPlayerState(ePlayerMoveState::NONE, ePlayerMoveState::NONE),
	_playerState(ePlayerMoveState::NONE, ePlayerMoveState::NONE)
{

}

void PlayerMove::Start()
{
	// asdf
	_playerColliderStanding = GetGameObject()->GetComponent<HDData::DynamicCapsuleCollider>();
	_fpMeshObj = GetGameObject()->GetGameObjectByNameInChildren("meshShell");
	_fpmesh = _fpMeshObj->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
	_weapon = _fpMeshObj->GetGameObjectByNameInChildren("Thumb_01.001")->GetGameObjectByNameInChildren("weapon")->GetComponent<HDData::MeshRenderer>();
	_moveSpeed = 3.0f;
	_playerAudio = GetGameObject()->GetComponent<HDData::AudioSource>();

	PresetSprayPattern(2);
	StartRoundCam();

	_prevPlayerState.first = ePlayerMoveState::IDLE;
	_playerState.first = ePlayerMoveState::IDLE;

	_fpanimator = GetGameObject()->GetGameObjectByNameInChildren("FPMesh")->GetComponent<HDData::Animator>();
	_tpanimator = GetGameObject()->GetComponent<HDData::Animator>();

	//_playerAudio->PlayRepeat("bgm");
}

void PlayerMove::Update()
{
	if (GameManager::Instance()->GetMyInfo()->GetIsDie() != _isDie)
	{
		if (_isDie)
		{
			_isDie = false;
			Respawn();
		}
		else
		{
			_isDie = true;
			Die();
		}
	}

	if (!_isMovable || _isDie)
	{
		return;
	}
	// 델타 타임 체크
	_deltaTime = API::GetDeltaTime();

	_playerPos = GetTransform()->GetPosition();

	_isShootHead = false;
	_isShootBody = false;

	CameraControl();
	CheckMoveInfo();
	CoolTime();
	DecidePlayerState();
	Behavior();

	UpdateStateText();

	// sound 관련
	PlayPlayerSound();

	API::DrawLineDir(_headCam->GetTransform()->GetPosition(), _headCam->GetTransform()->GetForward(), 10.0f, { 1.0f, 0.0f, 1.0f, 1.0f });
}

void PlayerMove::SetMovable(bool movable)
{
	_isMovable = movable;
}

void PlayerMove::SetPlayerCamera(HDData::Camera* camera)
{
	_playerCamera = camera;
	camera->SetCamActive(false);
}

void PlayerMove::SetPlayerText(HDData::TextUI* pos, HDData::TextUI* aim)
{
	_playerInfoText = pos;
	_aimText = aim;
}

void PlayerMove::SetHitParticle(std::vector<HDData::ParticleSphereCollider*> particleVec)
{
	_hitParticles = particleVec;
}

// 조이스틱 개념
void PlayerMove::CheckMoveInfo()
{
	_prevDirection = _moveDirection;
	_moveDirection = 5;

	if (!_isHeadCam)
	{
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
	}
	else
	{
		if (API::GetKeyPressing(DIK_W))
		{
			_moveDirection = 8;
		}
		if (API::GetKeyPressing(DIK_A))
		{
			_moveDirection = 4;
		}
		if (API::GetKeyPressing(DIK_S))
		{
			_moveDirection = 2;
		}
		if (API::GetKeyPressing(DIK_D))
		{
			_moveDirection = 6;
		}
		if (API::GetKeyPressing(DIK_W) && API::GetKeyPressing(DIK_A))
		{
			_moveDirection = 7;
		}
		if (API::GetKeyPressing(DIK_W) && API::GetKeyPressing(DIK_D))
		{
			_moveDirection = 9;
		}
		if (API::GetKeyPressing(DIK_A) && API::GetKeyPressing(DIK_S))
		{
			_moveDirection = 1;
		}
		if (API::GetKeyPressing(DIK_S) && API::GetKeyPressing(DIK_D))
		{
			_moveDirection = 3;
		}
	}
	/*
	if (_moveDirection != 5)
	{
		//if (!_playerAudio->IsSoundPlaying("walk"))
		//{
		//	_playerAudio->PlayOnce("walk");
		//}
	}
	if (API::GetKeyDown(DIK_SPACE))
	{
		//CheckIsOnGround();
		Jump(Vector3::Zero);
	}
	if (API::GetKeyDown(DIK_LSHIFT))
	{
		//_playerCollider->AdjustVelocity(2.0f);
		_isRunning = true;
		_moveSpeed = 6.0f;
	}
	if (API::GetKeyUp(DIK_LSHIFT))
	{
		//_playerCollider->AdjustVelocity(0.5f); // 0.5f -> can be replaced with certain ratio or variable
		_isRunning = false;
		_moveSpeed = 3.0f;
	}
	*/
}


void PlayerMove::CheckLookDirection()
{

}

bool PlayerMove::CheckIsOnGround()
{
	/*
	Vector3 pos = this->GetTransform()->GetPosition();
	const float delta = 0.2f;
	float x[9] = { -delta, -delta,0, delta,delta,delta,0,-delta,0 };
	float z[9] = { 0,delta,delta,delta,0,-delta,-delta,-delta,0 };

	for (int i = 0; i < 9; ++i)
	{
		float halfHeight = _playerCollider->GetHeight() / 2.0f;
		Vector3 worldPos = Vector3(pos.x + x[i], pos.y - halfHeight - 0.001f * (i - 1), pos.z + z[i]);

		int type = 0;
		HDData::Collider* temp = API::ShootRay({ worldPos.x, worldPos.y, worldPos.z }, { 0.0f, -1.0f,0.0f }, 0.05f, &type);
		API::DrawLineDir(worldPos, Vector3(0.f, -1.f, 0.f), 0.05f, Vector4(1.f, 0.f, 0.f, 0.f));

		if (temp)
		{
			// type 1이 rigidStatic.
			if (type == 1)
			{
				// 상태 변경 및 착지 Sound.
				if (_isOnGround == false)
				{
					_isOnGround = true;
					_isJumping = false;
					//_playerAudio->PlayOnce("landing");
					//_jumpCount = 0;
				}
				return true;
			}
		}
	}
	_isOnGround = false;
	*/
	//return false;

	Vector3 pos = this->GetTransform()->GetPosition();

	float halfHeight = _playerColliderStanding->GetHeight() / 2.0f;
	Vector3 rayOrigin = Vector3(pos.x, pos.y, pos.z);

	int colliderType = 0;
	HDData::Collider* opponentCollider = API::ShootRay({ rayOrigin.x, rayOrigin.y, rayOrigin.z }, { 0.0f, 1.0f,0.0f }, 0.08f, &colliderType);
	API::DrawLineDir(rayOrigin, Vector3(0.f, 1.f, 0.f), 0.08f, Vector4(1.f, 0.f, 0.f, 0.f));

	if (opponentCollider)
	{
		// type 1이 rigidStatic.
		if (colliderType == 1)
		{
			// 상태 변경 및 착지 Sound.
			//if (_isOnGround == false)
			//{
			_isOnGround = true;
			_isJumping = false;
			_playerColliderStanding->ClearVeloY();
			//_playerAudio->PlayOnce("landing");
		//}
			return true;
		}
	}

	_isOnGround = false;
	return false;
}

void PlayerMove::Move(int direction)
{
	// 조이스틱
	// 7 8 9
	// 4 5 6
	// 1 2 3

	// PhysX로 오브젝트 옮겨주기
	if (_moveDirection == 5)
	{
		if (_prevDirection != 0)
		{
			//_playerCollider->Move(DecideMovement(_prevDirection) * -30.0f);
			_playerColliderStanding->Stop();
		}
	}
	else
	{
		_playerColliderStanding->Move(DecideDisplacement(_moveDirection), _moveSpeed, _deltaTime);

		if (!(_playerAudio->IsSoundPlaying("walk") || _playerAudio->IsSoundPlaying("run") || _isJumping))
		{
			if (_isRunning)
			{
				_playerAudio->PlayOnce("run");
			}
			else
			{
				_playerAudio->PlayOnce("walk");
			}
		}
	}

	_prevDirection = _moveDirection;
}

void PlayerMove::ShootGun()
{
	if (_isReloading)
	{
		return;
	}

	if (_bulletCount <= 0)	// 장탄수를 임시로 30발로 제한
	{
		_headCam->ToggleCameraShake(false);
		_headCam->ResetCameraPos();
		return;
	}

	// 총기 반동
	//ApplyRecoil();
	_headCam->ToggleCameraShake(true);

	NetworkManager::Instance().SendPlayShoot(GetGameObject()->GetTransform());

	// 총구 화염 이펙트

	// 총 쏴서
	HDData::Collider* hitCollider = nullptr;

	Vector3 rayOrigin = _headCam->GetTransform()->GetPosition() + _headCam->GetTransform()->GetForward() * 1.0f;
	Vector3 hitPoint = { 314.0f, 314.0f, 314.0f };
	
	Vector3 recoilDirection = _headCam->GetTransform()->GetForward();
	//Vector3 recoilDirection = _headCam->GetTransform()->GetForward() +
	//	_headCam->GetTransform()->GetRight() * _sprayPattern[_shootCount].first * -4.0f +
	//	_headCam->GetTransform()->GetUp() * _sprayPattern[_shootCount].second * -0.02f;
	//recoilDirection.Normalize(recoilDirection);
	hitCollider = API::ShootRayHitPoint(rayOrigin, recoilDirection, hitPoint);
	//hitCollider = API::ShootRayHitPoint(rayOrigin, _headCam->GetTransform()->GetForward(), hitPoint);
	_playerAudio->PlayOnce("shoot");
	_playerAudio->PlayOnce("shoot2");

	// 맞은 데에 빨간 점 나오게 하기
	if (hitCollider != nullptr)
	{
		//_playerAudio->PlayOnce("hit");
		SpawnParticle(hitPoint);
		_hitPoint = hitPoint;
	}

	// 적군의 머리를 맞췄을 때
	HDData::DynamicSphereCollider* hitDynamicSphere = dynamic_cast<HDData::DynamicSphereCollider*>(hitCollider);
	if (hitDynamicSphere != nullptr)
	{
		RoundManager::Instance()->CheckHeadColliderOwner(hitDynamicSphere);
		GameManager::Instance()->GetMyInfo()->PlayHeadShotEffect();
		_isShootHead = true;
		//_playerAudio->PlayOnce("hitBody");
		//_playerAudio->PlayOnce("hitHead");
	}

	// 적군의 몸을 맞췄을 때
	HDData::DynamicCapsuleCollider* hitDynamicCapsule = dynamic_cast<HDData::DynamicCapsuleCollider*>(hitCollider);
	if (hitDynamicCapsule != nullptr)
	{
		RoundManager::Instance()->CheckBodyColliderOwner(hitDynamicCapsule);
		_isShootBody = true;
		//_playerAudio->PlayOnce("hitBody");
	}

	++_shootCount;
	--_bulletCount;
	_shootCooldown = 0.5f;
}

void PlayerMove::Reload()
{
	_shootCount = 0;
	_playerState.second = ePlayerMoveState::IDLE;
	_bulletCount = GameManager::Instance()->GetMyInfo()->GetMaxBulletCount();
}

void PlayerMove::SpawnParticle(Vector3 position)
{
	_hitParticles[_particleIndex]->SetGlobalPosition(position);
	_hitParticles[_particleIndex]->SetTimerActive();

	if (_particleIndex < 29)
	{
		++_particleIndex;
	}
	else
	{
		_particleIndex = 0;
	}
}

void PlayerMove::ApplyRecoil()
{
	//// 고정된 값으로 spray 해주는 버전
	//static_cast<HDData::DynamicCollider*>(_playerCollider->GetChildColliderVec()[0])->RotateY(_sprayPattern[_shootCount].first);
	//Pitch(_sprayPattern[_shootCount].second);

	//_rotAngleY += _sprayPattern[_shootCount].first;
	//_rotAngleX += _sprayPattern[_shootCount].second * 0.01f;

	_rotAngleY += _sprayCamera[_shootCount].first;
	_rotAngleX += _sprayCamera[_shootCount].second;
}

void PlayerMove::ShootTrail(Vector3 endPoint)
{
	Vector3 forward = _headCam->GetTransform()->GetForward();
	Vector3 up = _headCam->GetTransform()->GetUp();
	Vector3 right = _headCam->GetTransform()->GetRight();

	Vector3 startPoint = _headCam->GetTransform()->GetPosition() + forward * 1.75f - up * 0.15f + right * 0.25f;
	Vector3 endPoint2 = _headCam->GetTransform()->GetPosition() + _headCam->GetTransform()->GetForward() * 50.0f;
	API::DrawLine(startPoint, endPoint2, Vector4(1.0f, 0.0f, 0.0f, 0.0f));
}

void PlayerMove::Tumble(Vector3 direction)
{
	// 데굴
	_playerColliderStanding->Move(direction, 16.0f, _deltaTime);
}

void PlayerMove::PlayPlayerSound()
{

}

void PlayerMove::OnEnable()
{
	//_playerAudio->PlayRepeat("bgm");
}

void PlayerMove::OnStateEnter(ePlayerMoveState state)
{
	switch (state)
	{
		case ePlayerMoveState::IDLE:
		{
			_fpanimator->GetAllAC()->SetBool("isIdle", true);
			_playerColliderStanding->Stop();

			break;
		}
		case ePlayerMoveState::WALK:
		{
			_moveSpeed = 3.0f;
			//_playerColliderStanding->SetVelocity(DecideDisplacement(_moveDirection), _moveSpeed);

			break;
		}
		case ePlayerMoveState::RUN:
		{
			_moveSpeed = 6.0f;
			//_playerColliderStanding->SetVelocity(DecideDisplacement(_moveDirection), _moveSpeed);

			break;
		}
		case ePlayerMoveState::JUMP:
		{
			_moveSpeed = 5.0f;
			_playerColliderStanding->Jump(Vector3::Zero);
			_playerAudio->PlayOnce("jump");

			break;
		}
		case ePlayerMoveState::TUMBLE:
		{
			_tumbleTimer = 0.3f;
			_tumbleCooldown = 6.0f;
			recoilCooldown->SetTimerOn();
			cooldownCountText->SetTimerOn();

			_headCam->ToggleCameraShake(true);
			_fpmesh->SetMeshActive(false, 0);
			_weapon->SetMeshActive(false, 0);
			_weapon->SetMeshActive(false, 1);
			_weapon->SetMeshActive(false, 2);
			_weapon->SetMeshActive(false, 3);

			if (_moveDirection == 5)
			{
				_tumbleDirection = GetTransform()->GetForward();
			}
			else
			{
				_tumbleDirection = DecideDisplacement(_moveDirection);
			}

			_headCam->TumbleCamera(_deltaTime);
			_playerAudio->PlayOnce("tumble");
			_playerAudio->PlayOnce("tumblingMan");

			break;
		}
		case ePlayerMoveState::AIM:
		{

			break;
		}
		case ePlayerMoveState::FIRE:
		{
			ShootGun();
			_fpanimator->GetAllAC()->SetTrigger("isFire");
			_headCam->ToggleCameraShake(true);
			_playerAudio->PlayOnce("shoot");

			break;
		}
		case ePlayerMoveState::EMPTY:
		{
			_shootCooldown = 0.8f;
			_playerAudio->PlayOnce("empty");

			break;
		}
		case ePlayerMoveState::RELOAD:
		{
			_fpmesh->SetMeshActive(false, 0);
			_fpanimator->GetAllAC()->SetBool("isIdle", false);
			_fpanimator->GetAllAC()->SetTrigger("isReload");
			_playerAudio->PlayOnce("reload");
			_reloadTimer = 2.6f;

			break;
		}
		case ePlayerMoveState::DIE:
		{
			Die();

			break;
		}

		default:
		{

		}
	}
}

void PlayerMove::OnStateStay(ePlayerMoveState state)
{
	switch (state)
	{
		case ePlayerMoveState::IDLE:
		{
			_playerColliderStanding->ClearVeloY();
			_playerColliderStanding->Stop();

			break;	
		}
		case ePlayerMoveState::WALK:
		{
			//_headCam->ShakeCamera(_deltaTime, _rotAngleX);
			//if (_moveDirection != _prevDirection)
			//{
			//	_playerColliderStanding->SetVelocity(DecideDisplacement(_moveDirection), _moveSpeed);
			//}
			_playerColliderStanding->Move(DecideDisplacement(_moveDirection), _moveSpeed, _deltaTime);
			_playerAudio->PlayOnceIfNotPlaying2("walk", "run");

			break;
		}
		case ePlayerMoveState::RUN:
		{
			//_headCam->ShakeCamera(_deltaTime, _rotAngleX);
			//if (_moveDirection != _prevDirection)
			//{
			//	_playerColliderStanding->SetVelocity(DecideDisplacement(_moveDirection), _moveSpeed);
			//}
			_playerColliderStanding->Move(DecideDisplacement(_moveDirection), _moveSpeed, _deltaTime);
			_playerAudio->PlayOnceIfNotPlaying2("run", "walk");

			break;
		}
		case ePlayerMoveState::JUMP:
		{
			if (_isMoveableOnJump)
			{
				_playerColliderStanding->Move(DecideDisplacement(_moveDirection), _moveSpeed, _deltaTime);
			}

			break;
		}
		case ePlayerMoveState::TUMBLE:
		{
			Tumble(_tumbleDirection);
			_headCam->TumbleCamera(_deltaTime);

			break;
		}
		case ePlayerMoveState::AIM:
		{

			break;
		}
		case ePlayerMoveState::FIRE:
		{
			_headCam->ShakeCamera(_deltaTime, _rotAngleX);
			ShootTrail(_hitPoint);

			break;
		}
		case ePlayerMoveState::EMPTY:
		{

			break;
		}
		case ePlayerMoveState::RELOAD:
		{

			break;
		}
		case ePlayerMoveState::DIE:
		{

			break;
		}

		default:
		{

		}
	}
}

void PlayerMove::OnStateExit(ePlayerMoveState state)
{
	switch (state)
	{
		case ePlayerMoveState::IDLE:
		{

			break;
		}
		case ePlayerMoveState::WALK:
		{
			_playerColliderStanding->Stop();

			break;
		}
		case ePlayerMoveState::RUN:
		{
			_playerColliderStanding->Stop();

			break;
		}
		case ePlayerMoveState::JUMP:
		{
			Landing();

			break;
		}
		case ePlayerMoveState::TUMBLE:
		{
			Reload();
			_headCam->TumbleCamera(_deltaTime);
			//_headCam->ResetCameraPos();
			_headCam->ToggleCameraShake(false);
			_fpmesh->SetMeshActive(true, 0);
			_weapon->SetMeshActive(true, 0);
			_weapon->SetMeshActive(true, 1);
			_weapon->SetMeshActive(true, 2);
			_weapon->SetMeshActive(true, 3);

			_playerState.second = ePlayerMoveState::AIM;

			break;
		}
		case ePlayerMoveState::AIM:
		{

			break;
		}
		case ePlayerMoveState::FIRE:
		{
			// 반동 초기화
			_shootCount = 0;
			_headCam->ToggleCameraShake(false);
			_headCam->ResetCameraPos();

			break;
		}
		case ePlayerMoveState::EMPTY:
		{

			break;
		}
		case ePlayerMoveState::RELOAD:
		{
			Reload();
			_fpmesh->SetMeshActive(true, 0);

			_playerAudio->Stop("reload");
			_reloadTimer = 0.0f;

			break;
		}
		case ePlayerMoveState::DIE:
		{
			Respawn();

			break;
		}

		default:
		{

		}
	}
}

void PlayerMove::UpdateStateText()
{
	std::string first = "NONE";
	std::string second = "NONE";

	switch (_playerState.first)
	{
		case ePlayerMoveState::DIE:
		{
			first = "DIE";
			break;
		}
		case ePlayerMoveState::WALK:
		{
			first = "WALK";
			break;
		}
		case ePlayerMoveState::RUN:
		{
			first = "RUN";
			break;
		}
		case ePlayerMoveState::IDLE:
		{
			first = "IDLE";
			break;
		}
		case ePlayerMoveState::JUMP:
		{
			first = "JUMP";
			break;
		}
		case ePlayerMoveState::TUMBLE:
		{
			first = "TUMBLE";
			break;
		}
		default:
		{
			first = "NONE";
		}
	}

	switch (_playerState.second)
	{
		case ePlayerMoveState::FIRE:
		{
			second = "FIRE";
			break;
		}
		case ePlayerMoveState::RELOAD:
		{
			second = "RELOAD";
			break;
		}
		case ePlayerMoveState::AIM:
		{
			second = "AIM";
			break;
		}
		case ePlayerMoveState::EMPTY:
		{
			second = "EMPTY";
			break;
		}
		default:
		{
			second = "NONE";
		}
	}

	//_plStateText->SetText(first + "/" + second);

	//_anyText->SetText(std::to_string(_rotAngleX) + "/" + std::to_string(_rotAngleY));

	//_tumbleText->SetText(std::to_string(_tumbleCooldown));

	//std::string posText = std::to_string((int)(_playerPos.x)) + "/" + std::to_string((int)(_playerPos.y)) + "/" + std::to_string((int)(_playerPos.z));
	//_plPosText->SetText(posText);
}


int& PlayerMove::GetBulletCount()
{
	return _bulletCount;
}

ePlayerMoveState PlayerMove::GetPlayerMoveEnum(int index)
{
	if (index == 1)
	{
		return _playerState.first;
	}
	else
	{
		return _playerState.second;
	}
}

void PlayerMove::OnCollisionEnter(HDData::PhysicsCollision** colArr, unsigned int count)
{
	//++_enterCount;
	//if (_enterCount == 1)
	//{
	//	return;
	//}

	/*
	if (_playerState.first == ePlayerMoveState::TUMBLE)
	{
		return;
	}

	auto& opponentCollider = (*colArr)->_otherActor;

	// 지형인 경우
	if (opponentCollider->GetColType() == eColliderRole::TERRAIN)
	{
		// 착지 판정
		_playerState.first = ePlayerMoveState::IDLE;
		// 일단 
		OnStateExit(ePlayerMoveState::JUMP);
	}
	*/
	auto& opponentCollider = (*colArr)->_otherActor;

	if (opponentCollider->GetColType() == eColliderRole::TERRAIN && _playerState.first == ePlayerMoveState::JUMP)
	{
		//_isMoveableOnJump = false;
	}
}

void PlayerMove::OnCollisionExit(HDData::PhysicsCollision** colArr, unsigned int count)
{
	auto& opponentCollider = (*colArr)->_otherActor;

	//// plane인 경우
	//if (opponentCollider == nullptr)
	//{
	//	_isOnGround = false;
	//}
	//// 지형인 경우
	//else if (opponentCollider->GetColType() == eColliderType::TERRAIN)
	//{
	//	_isOnGround = false;
	//}

	//if (opponentCollider->GetColType() == eColliderRole::TERRAIN)
	//{
	//	_playerColliderStanding->ClearForceXYZ();
	//}
}

void PlayerMove::OnTriggerEnter(HDData::Collider** colArr, unsigned int count)
{
	//if (_playerState.first == ePlayerMoveState::TUMBLE)
	//{
	//	return;
	//}


	// 지형인 경우
	if ((*colArr)->GetColType() == eColliderRole::TERRAIN && _playerState.first == ePlayerMoveState::JUMP)
	{
		// 착지 판정
		_playerState.first = ePlayerMoveState::IDLE;
		//_isMoveableOnJump = true;

		// 일단 
		//OnStateExit(ePlayerMoveState::JUMP);
	}

	return;
}

void PlayerMove::OnTriggerExit(HDData::Collider** colArr, unsigned int count)
{
	if (_playerState.first != ePlayerMoveState::JUMP)
	{

	}
	return;
}

void PlayerMove::UpdatePlayerPositionDebug()
{
	Vector3 pos = GetTransform()->GetPosition();
	std::string posText = "x : " + std::to_string(pos.x) + "\ny : " + std::to_string(pos.y) + "\nz : " + std::to_string(pos.z);
	_playerInfoText->SetText(posText);
}

void PlayerMove::SetHeadCam(HDData::Camera* cam)
{
	_headCam = cam;
}

void PlayerMove::SetPlayerColliders(HDData::DynamicCapsuleCollider* standing, HDData::DynamicCapsuleCollider* sitting)
{
	_playerColliderStanding = standing;
	_playerColliderSitting = sitting;
}

void PlayerMove::PresetSprayPattern(int gunType)
{
	switch (gunType)
	{
		case 1:
		{
			float scale = 0.3f;

			// 조준점을 흐트리는 방식의 경우
			_sprayPattern[0] = std::make_pair(0.0f * scale, 0.0f * scale);
			_sprayPattern[1] = std::make_pair(0.002f * scale, -0.3f * scale);
			_sprayPattern[2] = std::make_pair(0.000f * scale, -1.3f * scale);
			_sprayPattern[3] = std::make_pair(0.002f * scale, -2.8f * scale);
			_sprayPattern[4] = std::make_pair(0.004f * scale, -4.3f * scale);
			_sprayPattern[5] = std::make_pair(-0.01f * scale, -6.0f * scale);
			_sprayPattern[6] = std::make_pair(-0.014f * scale, -7.5f * scale);
			_sprayPattern[7] = std::make_pair(-0.02f * scale, -8.5f * scale);
			_sprayPattern[8] = std::make_pair(-0.012f * scale, -9.5f * scale);
			_sprayPattern[9] = std::make_pair(0.008f * scale, -9.0f * scale);
			_sprayPattern[10] = std::make_pair(0.018f * scale, -9.3f * scale);
			_sprayPattern[11] = std::make_pair(0.012f * scale, -9.9f * scale);
			_sprayPattern[12] = std::make_pair(0.02f * scale, -10.5f * scale);
			_sprayPattern[13] = std::make_pair(0.036f * scale, -9.8f * scale);
			_sprayPattern[14] = std::make_pair(0.038f * scale, -10.0f * scale);
			_sprayPattern[15] = std::make_pair(0.016f * scale, -10.1f * scale);
			_sprayPattern[16] = std::make_pair(0.01f * scale, -10.5f * scale);
			_sprayPattern[17] = std::make_pair(0.004f * scale, -11.1f * scale);
			_sprayPattern[18] = std::make_pair(-0.008f * scale, -10.8f * scale);
			_sprayPattern[19] = std::make_pair(-0.014f * scale, -10.1f * scale);
			_sprayPattern[20] = std::make_pair(-0.024f * scale, -10.2f * scale);
			_sprayPattern[21] = std::make_pair(-0.016f * scale, -10.2f * scale);
			_sprayPattern[22] = std::make_pair(-0.014f * scale, -11.2f * scale);
			_sprayPattern[23] = std::make_pair(-0.012f * scale, -11.6f * scale);
			_sprayPattern[24] = std::make_pair(-0.022f * scale, -11.5f * scale);
			_sprayPattern[25] = std::make_pair(-0.014f * scale, -11.5f * scale);
			_sprayPattern[26] = std::make_pair(0.002f * scale, -10.9f * scale);
			_sprayPattern[27] = std::make_pair(0.022f * scale, -9.6f * scale);
			_sprayPattern[28] = std::make_pair(0.01f * scale, -10.0f * scale);
			_sprayPattern[29] = std::make_pair(0.016f * scale, -10.3f * scale);

			float scale2 = 0.01f;
			_sprayCamera[0] = std::make_pair(0.002f * scale2, -0.3f * scale2);
			_sprayCamera[1] = std::make_pair(-0.002f * scale2, -1.0f * scale2);
			_sprayCamera[2] = std::make_pair(0.002f * scale2, -1.5f * scale2);
			_sprayCamera[3] = std::make_pair(0.002f * scale2, -1.5f * scale2);
			_sprayCamera[4] = std::make_pair(-0.006f * scale2, -1.7f * scale2);
			_sprayCamera[5] = std::make_pair(-0.004f * scale2, -1.5f * scale2);
			_sprayCamera[6] = std::make_pair(-0.006f * scale2, -1.0f * scale2);
			_sprayCamera[7] = std::make_pair(0.008f * scale2, -1.0f * scale2);
			_sprayCamera[8] = std::make_pair(0.02f * scale2, 0.5f * scale2);
			_sprayCamera[9] = std::make_pair(0.01f * scale2, -0.3f * scale2);
			_sprayCamera[10] = std::make_pair(-0.006f * scale2, -0.6f * scale2);
			_sprayCamera[11] = std::make_pair(0.008f * scale2, -0.6f * scale2);
			_sprayCamera[12] = std::make_pair(0.016f * scale2, 0.7f * scale2);
			_sprayCamera[13] = std::make_pair(0.002f * scale2, -0.2f * scale2);
			_sprayCamera[14] = std::make_pair(-0.022f * scale2, -0.1f * scale2);
			_sprayCamera[15] = std::make_pair(-0.006f * scale2, -0.4f * scale2);
			_sprayCamera[16] = std::make_pair(-0.006f * scale2, -0.6f * scale2);
			_sprayCamera[17] = std::make_pair(-0.012f * scale2, 0.3f * scale2);
			_sprayCamera[18] = std::make_pair(-0.006f * scale2, 0.7f * scale2);
			_sprayCamera[19] = std::make_pair(-0.01f * scale2, -0.1f * scale2);
			_sprayCamera[20] = std::make_pair(0.008f * scale2, 0.0f * scale2);
			_sprayCamera[21] = std::make_pair(0.002f * scale2, -1.0f * scale2);
			_sprayCamera[22] = std::make_pair(0.002f * scale2, -0.4f * scale2);
			_sprayCamera[23] = std::make_pair(-0.01f * scale2, 0.1f * scale2);
			_sprayCamera[24] = std::make_pair(0.008f * scale2, 0.0f * scale2);
			_sprayCamera[25] = std::make_pair(0.016f * scale2, 0.6f * scale2);
			_sprayCamera[26] = std::make_pair(0.02f * scale2, 1.3f * scale2);
			_sprayCamera[27] = std::make_pair(0.008f * scale2, -0.4f * scale2);
			_sprayCamera[28] = std::make_pair(0.004f * scale2, -0.2f * scale2);
			_sprayCamera[29] = std::make_pair(0.002f * scale2, -0.1f * scale2);

			break;
		}

		case 2:
		{
			_sprayPattern[0] = std::make_pair(0.0f, 0.0f);
			_sprayPattern[1] = std::make_pair(0.0f, 0.0f);
			_sprayPattern[2] = std::make_pair(0.0f, 0.0f);
			_sprayPattern[3] = std::make_pair(0.0f, 0.0f);
			_sprayPattern[4] = std::make_pair(0.0f, 0.0f);
			_sprayPattern[5] = std::make_pair(0.0f, 0.0f);
			_sprayPattern[6] = std::make_pair(0.0f, 0.0f);
			_sprayPattern[7] = std::make_pair(0.0f, 0.0f);
			_sprayPattern[8] = std::make_pair(0.0f, 0.0f);
			_sprayPattern[9] = std::make_pair(0.0f, 0.0f);
			_sprayPattern[10] = std::make_pair(0.0f, 0.0f);
			_sprayPattern[11] = std::make_pair(0.0f, 0.0f);
			_sprayPattern[12] = std::make_pair(0.0f, 0.0f);
			_sprayPattern[13] = std::make_pair(0.0f, 0.0f);
			_sprayPattern[14] = std::make_pair(0.0f, 0.0f);
			_sprayPattern[15] = std::make_pair(0.0f, 0.0f);
			_sprayPattern[16] = std::make_pair(0.0f, 0.0f);
			_sprayPattern[17] = std::make_pair(0.0f, 0.0f);
			_sprayPattern[18] = std::make_pair(0.0f, 0.0f);
			_sprayPattern[19] = std::make_pair(0.0f, 0.0f);
			_sprayPattern[20] = std::make_pair(0.0f, 0.0f);
			_sprayPattern[21] = std::make_pair(0.0f, 0.0f);
			_sprayPattern[22] = std::make_pair(0.0f, 0.0f);
			_sprayPattern[23] = std::make_pair(0.0f, 0.0f);
			_sprayPattern[24] = std::make_pair(0.0f, 0.0f);
			_sprayPattern[25] = std::make_pair(0.0f, 0.0f);
			_sprayPattern[26] = std::make_pair(0.0f, 0.0f);
			_sprayPattern[27] = std::make_pair(0.0f, 0.0f);
			_sprayPattern[28] = std::make_pair(0.0f, 0.0f);
			_sprayPattern[29] = std::make_pair(0.0f, 0.0f);

			_sprayCamera[0] = std::make_pair(0.0f, 0.0f);
			_sprayCamera[1] = std::make_pair(0.0f, 0.0f);
			_sprayCamera[2] = std::make_pair(0.0f, 0.0f);
			_sprayCamera[3] = std::make_pair(0.0f, 0.0f);
			_sprayCamera[4] = std::make_pair(0.0f, 0.0f);
			_sprayCamera[5] = std::make_pair(0.0f, 0.0f);
			_sprayCamera[6] = std::make_pair(0.0f, 0.0f);
			_sprayCamera[7] = std::make_pair(0.0f, 0.0f);
			_sprayCamera[8] = std::make_pair(0.0f, 0.0f);
			_sprayCamera[9] = std::make_pair(0.0f, 0.0f);
			_sprayCamera[10] = std::make_pair(0.0f, 0.0f);
			_sprayCamera[11] = std::make_pair(0.0f, 0.0f);
			_sprayCamera[12] = std::make_pair(0.0f, 0.0f);
			_sprayCamera[13] = std::make_pair(0.0f, 0.0f);
			_sprayCamera[14] = std::make_pair(0.0f, 0.0f);
			_sprayCamera[15] = std::make_pair(0.0f, 0.0f);
			_sprayCamera[16] = std::make_pair(0.0f, 0.0f);
			_sprayCamera[17] = std::make_pair(0.0f, 0.0f);
			_sprayCamera[18] = std::make_pair(0.0f, 0.0f);
			_sprayCamera[19] = std::make_pair(0.0f, 0.0f);
			_sprayCamera[20] = std::make_pair(0.0f, 0.0f);
			_sprayCamera[21] = std::make_pair(0.0f, 0.0f);
			_sprayCamera[22] = std::make_pair(0.0f, 0.0f);
			_sprayCamera[23] = std::make_pair(0.0f, 0.0f);
			_sprayCamera[24] = std::make_pair(0.0f, 0.0f);
			_sprayCamera[25] = std::make_pair(0.0f, 0.0f);
			_sprayCamera[26] = std::make_pair(0.0f, 0.0f);
			_sprayCamera[27] = std::make_pair(0.0f, 0.0f);
			_sprayCamera[28] = std::make_pair(0.0f, 0.0f);
			_sprayCamera[29] = std::make_pair(0.0f, 0.0f);

			break;
		}

		default:
		{
			// 카메라를 돌려주는 방식의 경우
			//_sprayPattern[0] = std::make_pair(0.001f * scale, -0.3f * scale);
			//_sprayPattern[1] = std::make_pair(-0.001f * scale, -1.0f * scale);
			//_sprayPattern[2] = std::make_pair(0.001f * scale, -1.5f * scale);
			//_sprayPattern[3] = std::make_pair(0.001f * scale, -1.5f * scale);
			//_sprayPattern[4] = std::make_pair(-0.003f * scale, -1.7f * scale);
			//_sprayPattern[5] = std::make_pair(-0.002f * scale, -1.5f * scale);
			//_sprayPattern[6] = std::make_pair(-0.003f * scale, -1.0f * scale);
			//_sprayPattern[7] = std::make_pair(0.004f * scale, -1.0f * scale);
			//_sprayPattern[8] = std::make_pair(0.01f * scale, 0.5f * scale);
			//_sprayPattern[9] = std::make_pair(0.005f * scale, -0.3f * scale);
			//_sprayPattern[10] = std::make_pair(-0.003f * scale, -0.6f * scale);
			//_sprayPattern[11] = std::make_pair(0.004f * scale, -0.6f * scale);
			//_sprayPattern[12] = std::make_pair(0.008f * scale, 0.7f * scale);
			//_sprayPattern[13] = std::make_pair(0.001f * scale, -0.2f * scale);
			//_sprayPattern[14] = std::make_pair(-0.011f * scale, -0.1f * scale);
			//_sprayPattern[15] = std::make_pair(-0.003f * scale, -0.4f * scale);
			//_sprayPattern[16] = std::make_pair(-0.003f * scale, -0.6f * scale);
			//_sprayPattern[17] = std::make_pair(-0.006f * scale, 0.3f * scale);
			//_sprayPattern[18] = std::make_pair(-0.003f * scale, 0.7f * scale);
			//_sprayPattern[19] = std::make_pair(-0.005f * scale, -0.1f * scale);
			//_sprayPattern[20] = std::make_pair(0.004f * scale, 0.0f * scale);
			//_sprayPattern[21] = std::make_pair(0.001f * scale, -1.0f * scale);
			//_sprayPattern[22] = std::make_pair(0.001f * scale, -0.4f * scale);
			//_sprayPattern[23] = std::make_pair(-0.005f * scale, 0.1f * scale);
			//_sprayPattern[24] = std::make_pair(0.004f * scale, 0.0f * scale);
			//_sprayPattern[25] = std::make_pair(0.008f * scale, 0.6f * scale);
			//_sprayPattern[26] = std::make_pair(0.01f * scale, 1.3f * scale);
			//_sprayPattern[27] = std::make_pair(0.004f * scale, -0.4f * scale);
			//_sprayPattern[28] = std::make_pair(0.002f * scale, -0.2f * scale);
			//_sprayPattern[29] = std::make_pair(0.001f * scale, -0.1f * scale);
			break;
		}
	}
}

void PlayerMove::StartRoundCam()
{
	_isHeadCam = true;
	_isFirstPersonPerspective = true;
}

bool PlayerMove::IsShootHead()
{
	return _isShootHead;
}

bool PlayerMove::IsShootBody()
{
	return _isShootBody;
}

bool PlayerMove::GetIsIngamePlaying()
{
	return _isIngamePlaying;
}

void PlayerMove::SetIsIngamePlaying(bool isPlaying)
{
	_isIngamePlaying = isPlaying;
}

void PlayerMove::ToggleCam()
{
	if (_isHeadCam)
	{
		API::SetCurrentSceneMainCamera(_playerCamera);
		_playerCamera->SetAsMainCamera();
		_isHeadCam = false;
		//_aimText->SetText("");
		_isFirstPersonPerspective = false;
		_playerCamera->SetCamActive(true);
	}
	else
	{
		API::SetCurrentSceneMainCamera(_headCam);
		_headCam->SetAsMainCamera();
		_isHeadCam = true;
		//_aimText->SetText("O");
		_isFirstPersonPerspective = true;
		_playerCamera->SetCamActive(false);
	}
}

void PlayerMove::Jump(Vector3 direction)
{
	if ((!_isJumping) && (_isOnGround))
	{
		// 점프
		_playerColliderStanding->Jump(direction);
		_playerAudio->PlayOnce("jump");
		_isJumping = true;
		_isOnGround = false;

		if (RoundManager::Instance()->GetIsRoundStart())
		{
			RoundManager::Instance()->SendJump(GetGameObject()->GetComponent<PlayerInfo>()->GetPlayerUID());
		}
	}
}


Vector3 PlayerMove::DecideDisplacement(int direction)
{
	Vector3 moveStep;

	if (_isFirstPersonPerspective)
	{
		switch (direction)
		{
			case 1:
			{
				moveStep =
					GetGameObject()->GetTransform()->GetForward() * -0.7f
					+ GetGameObject()->GetTransform()->GetRight() * -0.7f;
			}
			break;
			case 2:
			{
				moveStep = GetTransform()->GetForward() * -1;
			}
			break;
			case 3:
			{
				moveStep =
					GetGameObject()->GetTransform()->GetForward() * -0.7f
					+ GetGameObject()->GetTransform()->GetRight() * 0.7f;
			}
			break;
			case 4:
			{
				moveStep = GetTransform()->GetRight() * -1;
			}
			break;
			case 5:
			{
				moveStep = Vector3::Zero;
			}
			break;
			case 6:
			{
				moveStep = GetTransform()->GetRight();
			}
			break;
			case 7:
			{
				moveStep =
					GetGameObject()->GetTransform()->GetForward() * 0.7f
					+ GetGameObject()->GetTransform()->GetRight() * -0.7f;
			}
			break;
			case 8:
			{
				moveStep = GetTransform()->GetForward();
			}
			break;
			case 9:
			{
				moveStep =
					GetGameObject()->GetTransform()->GetForward() * 0.7f
					+ GetGameObject()->GetTransform()->GetRight() * 0.7f;
			}
			break;
			default:
			{

			}
			break;
		}
	}
	else
	{
		switch (direction)
		{
			case 1:
			{
				moveStep = Vector3(-0.7f, 0.0f, -0.7f);
			}
			break;
			case 2:
			{
				moveStep = Vector3(0.0f, 0.0f, -1.0f);
			}
			break;
			case 3:
			{
				moveStep = Vector3(0.7f, 0.0f, -0.7f);
			}
			break;
			case 4:
			{
				moveStep = Vector3(-1.0f, 0.0f, 0.0f);
			}
			break;
			case 5:
			{
				moveStep = Vector3::Zero;
			}
			break;
			case 6:
			{
				moveStep = Vector3(1.0f, 0.0f, 0.0f);
			}
			break;
			case 7:
			{
				moveStep = Vector3(-0.7f, 0.0f, 0.7f);
			}
			break;
			case 8:
			{
				moveStep = Vector3(0.0f, 0.0f, 1.0f);
			}
			break;
			case 9:
			{
				moveStep = Vector3(0.7f, 0.0f, 0.7f);
			}
			break;
			default:
			{

			}
			break;
		}
	}

	return moveStep;
}

void PlayerMove::CameraControl()
{
#ifdef _DEBUG
	if (API::GetKeyDown(DIK_P))
	{
		ToggleCam();
	}
#endif

	// camera move
	if (_isHeadCam)
	{
		CameraMove();
	}
}

// 마우스 이동에 따른 시야 변경을 위한 함수
void PlayerMove::Pitch(float rotationValue)
{
	/*
	HDData::Transform* cameraTransform = _headCam->GetGameObject()->GetTransform();
	Quaternion rot = cameraTransform->GetLocalRotation();
	float eulerAngleX = std::atan2(2.0f * (rot.w * rot.x + rot.y * rot.z), 1.0f - 2.0f * (rot.x * rot.x + rot.y * rot.y));

	Vector3 rotAngle = rot.ToEuler() * 60.0f;

	Vector4 rotationAxis{ 1.f, 0.f, 0.f, 0.f };
	rotationAxis = XMVector4Transform(rotationAxis, Matrix::CreateFromQuaternion(cameraTransform->GetRotation()));


	if (rotAngle.x > 60.0f)
	{
		//constexpr float radX = HDMath::ToRadian(89.0f) * 0.5f;
		//constexpr float radX = DirectX::XMConvertToRadians(89.0f);
		//Quaternion closedAngle = { std::cos(radX), std::sin(radX), 0.f, 0.f };

		//cameraTransform->SetLocalRotation(closedAngle);
		//cameraTransform->SetLocalRotation(Quaternion(rot.x, DirectX::XMConvertToRadians(89.0f), rot.z, rot.w));

		Quaternion temp = Quaternion::CreateFromAxisAngle(Vector3(rotationAxis.x, rotationAxis.y, rotationAxis.z), DirectX::XMConvertToRadians(360.0f));
		cameraTransform->SetLocalRotation(temp);
	}
	else if (rotAngle.x < -60.0f)
	{
		////constexpr float radX = HDMath::ToRadian(-89.0f) * 0.5f;
		//constexpr float radX = DirectX::XMConvertToRadians(-89.0f) * 0.5f;
		//Quaternion closedAngle = { std::cos(radX), std::sin(radX), 0.f, 0.f };

		//cameraTransform->SetLocalRotation(closedAngle);

		Quaternion temp = Quaternion::CreateFromAxisAngle(Vector3(rotationAxis.x, rotationAxis.y, rotationAxis.z), DirectX::XMConvertToRadians(-360.0f));
		cameraTransform->SetLocalRotation(temp);
	}
	else
	{
		Vector4 rotationAxis{ 1.f, 0.f, 0.f, 0.f };
		rotationAxis = XMVector4Transform(rotationAxis, Matrix::CreateFromQuaternion(cameraTransform->GetRotation()));

		_pitchAngle += rotationValue;
		Quaternion rotToX = Quaternion::CreateFromAxisAngle(Vector3(rotationAxis.x, rotationAxis.y, rotationAxis.z), _pitchAngle);

		cameraTransform->SetLocalRotation(rotToX);
	}*/

	/*
	HDData::Transform* cameraTransform = _headCam->GetTransform();

	float rotAngleX = DirectX::XMConvertToDegrees(cameraTransform->GetLocalRotation().ToEuler().x);
	if (rotAngleX >= 79.0f)
	{
		if (rotationValue >= -0.0f)
		{
			return;
		}
	}
	else if (rotAngleX <= -79.0f)
	{
		if (rotationValue <= 0.0f)
		{
			return;
		}
	}

	HDData::Transform copy = *cameraTransform;
	copy.Rotate(rotationValue, 0.0f, 0.0f);

	float rotAnglePrediction = DirectX::XMConvertToDegrees(copy.GetLocalRotation().ToEuler().x);

	if (rotAnglePrediction >= 79.0f)
	{
		cameraTransform->SetLocalRotationEuler(Vector3(80.0f, 0.0f, 0.0f));
	}
	else if (rotAnglePrediction <= -79.0f)
	{
		cameraTransform->SetLocalRotationEuler(Vector3(-80.0f, 0.0f, 0.0f));
	}
	else
	{
		cameraTransform->Rotate(rotationValue, 0.0f, 0.0f);
	}
	*/

	HDData::Transform* headTrasnform = _headCam->GetGameObject()->GetParentGameObject()->GetTransform();

	float rotAngleX = DirectX::XMConvertToDegrees(headTrasnform->GetLocalRotation().ToEuler().x);
	if (rotAngleX >= 79.0f)
	{
		if (rotationValue >= -0.0f)
		{
			return;
		}
	}
	else if (rotAngleX <= -79.0f)
	{
		if (rotationValue <= 0.0f)
		{
			return;
		}
	}

	Vector3 rotAxis = headTrasnform->GetRight();
	rotAxis.y = 0.0f;
	//Quaternion rotVal = XMQuaternionRotationAxis(rotAxis, rotationValue);
	//Quaternion newRot = XMQuaternionMultiply(headTrasnform->GetRotation(), rotVal);
	//headTrasnform->Rotate(rotVal);
	static_cast<HDData::DynamicCollider*>(_playerColliderStanding->GetChildColliderVec()[0])->RotateOnAxis(rotationValue * 0.1f, rotAxis);
}

void PlayerMove::ToggleSit(bool isSit)
{
	if (isSit)
	{
		//_playerColliderStanding->DisableCollider();
		//_playerColliderSitting->EnableCollider();

		//_playerColliderStanding->EnableStanding(false);
		_playerColliderStanding->SetSitStand(1);
	}
	else
	{
		//_playerColliderSitting->DisableCollider();
		//_playerColliderStanding->EnableCollider();

		//_playerColliderStanding->EnableStanding(true);
		_playerColliderStanding->SetSitStand(2);
	}
}

void PlayerMove::Die()
{
	auto origin = _headCam->GetTransform()->GetPosition();
	_headCam->GetTransform()->Rotate(0, 90, 0);
	_playerColliderStanding->OnDisable();
}

void PlayerMove::Respawn()
{
	
	_playerColliderStanding->OnEnable();
}

void PlayerMove::DecidePlayerState()
{
	// 이전 스테이트 정보로 저장해주고 넘어간다.
	_prevPlayerState.first = _playerState.first;
	_prevPlayerState.second = _playerState.second;

	// 구르기 타이머 체크해서 상태 종료
	if (_tumbleTimer > 0.0f)
	{
		return;
	}

	if (API::GetKeyDown(DIK_LSHIFT) && _tumbleCooldown <= 0.0f && _playerState.first != ePlayerMoveState::JUMP)
	{
		_playerState.first = ePlayerMoveState::TUMBLE;
		_playerState.second = ePlayerMoveState::AIM;
		_headCam->ResetCameraPos();

		return;
	}

	DecidePlayerStateSecond();

	// 점프, 걷기, 뛰기
	if (_playerState.first == ePlayerMoveState::JUMP)
	{
		return;
	}
	else if (API::GetKeyDown(DIK_SPACE))
	{
		_playerState.first = ePlayerMoveState::JUMP;
	}
	else if (_moveDirection == 5)
	{
		_playerState.first = ePlayerMoveState::IDLE;
	}
	else
	{
		if (API::GetKeyPressing(DIK_LCONTROL))
		{
			_playerState.first = ePlayerMoveState::WALK;
		}
		else
		{
			_playerState.first = ePlayerMoveState::RUN;
		}
	}
}

void PlayerMove::DecidePlayerStateSecond()
{
	// 재장전, 사격 (BULLET_MAX 6)
	if (_reloadTimer > 0.0f)
	{
		return;
	}
	else if (API::GetKeyDown(DIK_R) && _bulletCount < 6)
	{
		_playerState.second = ePlayerMoveState::RELOAD;
		return;
	}

	if (_shootCooldown <= 0.0f)
	{
		if (API::GetMouseDown(MOUSE_LEFT))
		{
			if (_bulletCount == 0)
			{
				//_playerState.second = ePlayerMoveState::EMPTY;
				_playerState.second = ePlayerMoveState::RELOAD;
			}
			else
			{
				_playerState.second = ePlayerMoveState::FIRE;
			}
		}
		else  if (API::GetMouseHold(MOUSE_LEFT) && _prevPlayerState.second == ePlayerMoveState::AIM)
		{
			if (_bulletCount != 0)
			{
				_playerState.second = ePlayerMoveState::FIRE;
			}
		}
		else
		{
			_playerState.second = ePlayerMoveState::AIM;
		}
	}
}

void PlayerMove::Behavior()
{
	if (_playerState.first != ePlayerMoveState::JUMP)
	{
		_playerColliderStanding->ClearVeloY();
	}

	if (_prevPlayerState.second == _playerState.second)
	{
		OnStateStay(_playerState.second);
	}
	else
	{
		OnStateExit(_prevPlayerState.second);
		OnStateEnter(_playerState.second);
	}

	if (_prevPlayerState.first == _playerState.first)
	{
		OnStateStay(_playerState.first);
	}
	else
	{
		OnStateExit(_prevPlayerState.first);
		OnStateEnter(_playerState.first);
	}

	/*
	// 카메라 셰이크는 매 프레임 들어가긴 해야한다.
	if (_playerState.first != ePlayerMoveState::TUMBLE)
	{
		_headCam->ShakeCamera(_deltaTime, _rotAngleX);
	}

	// 움직임
	switch (_playerState.first)
	{
		case ePlayerMoveState::IDLE:
		{
			_playerColliderStanding->Stop();
			break;
		}
		case ePlayerMoveState::WALK:
		{
			if (!_playerAudio->IsSoundPlaying("walk"))
			{
				_playerAudio->PlayOnce("walk");
			}
			_moveSpeed = 3.0f;
			_playerColliderStanding->Move(DecideDisplacement(_moveDirection), _moveSpeed, _deltaTime);
			break;
		}
		case ePlayerMoveState::RUN:
		{
			if (!_playerAudio->IsSoundPlaying("run"))
			{
				_playerAudio->PlayOnce("run");
			}
			_moveSpeed = 6.0f;
			_playerColliderStanding->Move(DecideDisplacement(_moveDirection), _moveSpeed, _deltaTime);
			break;
		}
		case ePlayerMoveState::JUMP:
		{
			if (_prevPlayerState.first != ePlayerMoveState::JUMP)
			{
				Jump(DecideDisplacement(_moveDirection) * _moveSpeed);
			}

			_moveSpeed = 5.0f;
			_playerColliderStanding->Move(DecideDisplacement(_moveDirection), _moveSpeed, _deltaTime);

			break;
		}
		case ePlayerMoveState::TUMBLE:
		{
			if (_prevPlayerState.first != ePlayerMoveState::TUMBLE)
			{
				_playerAudio->PlayOnce("tumblingMan");
				_playerAudio->PlayOnce("tumble");

				_tumbleTimer = 0.3f;
				_headCam->ToggleCameraShake(true);

				if (_moveDirection == 5)
				{
					_tumbleDirection = GetTransform()->GetForward();
				}
				else
				{
					_tumbleDirection = DecideDisplacement(_moveDirection);
				}
			}

			Tumble(_tumbleDirection);
			_headCam->TumbleCamera(_deltaTime);

			break;
		}
		default:
		{

		}
	}

	// 총 쏘거나 재장전
	if (_playerState.second == ePlayerMoveState::FIRE)
	{
		if (_shootCooldown <= 0.0f)
		{
			if (_bulletCount == 0)
			{
				if (_prevPlayerState.second != ePlayerMoveState::FIRE)
				{
					_playerAudio->PlayOnce("empty");
				}
			}
			else
			{
				if (_prevPlayerState.second != ePlayerMoveState::FIRE)
				{
					_headCam->ToggleCameraShake(true);
				}
				ShootGun();
			}
		}
	}
	else if (_playerState.second == ePlayerMoveState::RELOAD)
	{
		if (_prevPlayerState.second != ePlayerMoveState::RELOAD)
		{
			_fpmesh->SetMeshActive(false, 0);
			_playerAudio->PlayOnce("reload");
			_reloadTimer = 3.0f;
		}
		else
		{
			if (_reloadTimer > 0.0f)
			{
				_reloadTimer -= _deltaTime;
			}
			else
			{
				_fpmesh->SetMeshActive(true, 0);
				Reload();
			}
		}
	}

	if (_prevPlayerState.first != _playerState.first)
	{
		if (_prevPlayerState.first == ePlayerMoveState::JUMP)
		{
			Landing();
		}
		if (_prevPlayerState.first == ePlayerMoveState::TUMBLE)
		{
			Reload();
			_shootCooldown = 0.0f;
			if (API::GetMouseHold(MOUSE_LEFT))
			{
				_playerState.second = ePlayerMoveState::FIRE;
			}
			else
			{
				_playerState.second = ePlayerMoveState::IDLE;
			}
		}
		if (_prevPlayerState.second == ePlayerMoveState::FIRE)
		{
			// 반동 초기화
			_shootCount = 0;
			_headCam->ToggleCameraShake(false);
			_headCam->ResetCameraPos();
		}
	}
	*/
}

void PlayerMove::CoolTime()
{
	if (_shootCooldown > 0.0f)
	{
		_shootCooldown -= _deltaTime;
	}

	if (_tumbleCooldown > 0.0f)
	{
		_tumbleCooldown -= _deltaTime;
	}
	else
	{
		_tumbleCooldown = 0.0f;
	}

	if (_reloadTimer > 0.0f)
	{
		_reloadTimer -= _deltaTime;
	}

	if (_tumbleTimer > 0.0f)
	{
		_tumbleTimer -= _deltaTime;
	}
}

void PlayerMove::Landing()
{
	_isOnGround = true;
	_isJumping = false;
	_playerAudio->PlayOnce("land");
	_playerColliderStanding->ClearForceXYZ();
}

void PlayerMove::SwitchCamera()
{
	HDData::Transform* playerTransform = this->GetGameObject()->GetTransform();
	HDData::Transform* cameraTransform = _playerCamera->GetGameObject()->GetTransform();

	if (!_isHeadCam)
	{
		HDData::Transform* playerTransform = this->GetGameObject()->GetTransform();
		HDData::Transform* cameraTransform = _playerCamera->GetGameObject()->GetTransform();

		_prevCameraPos = cameraTransform->GetPosition();
		_prevCameraRot = cameraTransform->GetRotation();

		cameraTransform->SetPosition(playerTransform->GetPosition() + Vector3(0.f, 4.f, 0.f));
		cameraTransform->SetRotation(playerTransform->GetRotation());
		_playerCamera->GetGameObject()->SetParentObject(this->GetGameObject());
		_isHeadCam = true;
	}
	else
	{
		_playerCamera->GetGameObject()->SetParentObject(nullptr);
		cameraTransform->SetPosition(_prevCameraPos);
		cameraTransform->SetRotation(_prevCameraRot);
		_isHeadCam = false;
	}
}

void PlayerMove::CameraMove()
{
	Vector2 mouseDelta = API::GetMouseDelta();

	if (mouseDelta.x > 400.0f)
	{
		mouseDelta.x = 400.0f;
	}
	if (mouseDelta.x < -400.0f)
	{
		mouseDelta.x = -400.0f;
	}

	if (mouseDelta.y > 400.0f)
	{
		mouseDelta.y = 400.0f;
	}
	if (mouseDelta.y < -400.0f)
	{
		mouseDelta.y = -400.0f;
	}

	//Quaternion prevRot = Quaternion::CreateFromYawPitchRoll(_rotAngleY, 0.0f, 0.0f);
	//Quaternion prevPitchRot = Quaternion::CreateFromYawPitchRoll(_rotAngleY, _rotAngleX, 0.0f);
	//Quaternion prevMeshRot = Quaternion::CreateFromAxisAngle({ 1.0f, 0.0f, 0.0f }, _rotAngleX);

	_prevRotAngleX = _rotAngleX;
	_prevRotAngleY = _rotAngleY;

	_rotAngleY = (_rotAngleY + mouseDelta.x * 0.0003f);
	_rotAngleX = (_rotAngleX + mouseDelta.y * 0.0003f);

	if (_rotAngleX >= 1.57f)
	{
		_rotAngleX = 1.57f;
	}
	if (_rotAngleX <= -1.57f)
	{
		_rotAngleX = -1.57f;
	}

	if (_rotAngleY > 6.2832f)
	{
		_rotAngleY -= 6.2832f;
	}
	if (_rotAngleY < -6.2832f)
	{
		_rotAngleY += 6.2832f;
	}

	float ratio = _deltaTime * 10;
	
	float finalRotX = _rotAngleX * (1.0f - ratio) + _prevRotAngleX * ratio;
	float finalRotY = _rotAngleY * (1.0f - ratio) + _prevRotAngleY * ratio;

	Quaternion rot = rot.CreateFromYawPitchRoll(finalRotY, 0.0f, 0.0f);
	_playerColliderStanding->SetColliderRotation(rot);

	// 통짜 콜라이더일 때 위아래 카메라 움직이는 부분
	Quaternion pitchRotQuat = Quaternion::CreateFromYawPitchRoll(finalRotY, finalRotX, 0.0f);
	_headCam->GetTransform()->SetRotation(pitchRotQuat);

	// 메쉬 회전
	Quaternion rotX = Quaternion::CreateFromAxisAngle({ 1.0f, 0.0f, 0.0f }, finalRotX);
	_fpMeshObj->GetTransform()->SetLocalRotation(rotX);

}
