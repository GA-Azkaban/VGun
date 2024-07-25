#include "PlayerMove.h"
#include "../HODOengine/DynamicCollider.h"
#include "PlayerInfo.h"
#include "GameManager.h"
#include "RoundManager.h"
#include "NetworkManager.h"
#include "CooldownAlpha.h"
#include "CooldownText.h"
#include "DecalPool.h"

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
	_moveSpeed = 6.4f;

	StartRoundCam();

	_prevPlayerState.first = ePlayerMoveState::IDLE;
	_playerState.first = ePlayerMoveState::IDLE;

	_fpanimator = GetGameObject()->GetGameObjectByNameInChildren("FPMesh")->GetComponent<HDData::Animator>();
	_tpanimator = GetGameObject()->GetComponent<HDData::Animator>();
}

void PlayerMove::Update()
{
	// 델타 타임 체크
	_deltaTime = API::GetDeltaTime();

	CoolTime();

	DecidePlayerState();

	_isShootHead = false;
	_isShootBody = false;

	if (!_isDie && _isMovable)
	{
		CameraControl();
		CheckMoveInfo();
	}
	Behavior();

	//_playerPos = GetTransform()->GetPosition();

	//UpdateStateText();

	API::DrawLineDir(_headCam->GetTransform()->GetPosition(), _headCam->GetTransform()->GetForward(), 10.0f, { 1.0f, 0.0f, 1.0f, 1.0f });
}

void PlayerMove::SetMovable(bool movable)
{
	_isMovable = movable;
	if (movable == true)
	{
		_playerColliderStanding->Stop();
	}
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
}


void PlayerMove::CheckLookDirection()
{

}

bool PlayerMove::CheckIsOnGround()
{
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
			//_playerColliderStanding->ClearVeloY();
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
			_playerColliderStanding->Stop();
		}
	}
	else
	{
		_playerColliderStanding->Move(DecideDisplacement(_moveDirection), _moveSpeed, _deltaTime);
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
	_headCam->ToggleCameraShake(true);

	NetworkManager::Instance().SendPlayShoot(GetGameObject()->GetTransform());

	// 총 쏴서
	HDData::Collider* hitCollider = nullptr;

	Vector3 rayOrigin = _headCam->GetTransform()->GetPosition() + _headCam->GetTransform()->GetForward() * 1.0f;
	Vector3 hitPoint = { 314.0f, 314.0f, 314.0f };
	
	hitCollider = API::ShootRayHitPoint(rayOrigin, _headCam->GetTransform()->GetForward(), hitPoint);

	// 맞은 데에 빨간 점 나오게 하기
	if (hitCollider != nullptr)
	{
		_hitPoint = hitPoint;
	}

	// 적군의 머리를 맞췄을 때
	HDData::DynamicSphereCollider* hitDynamicSphere = dynamic_cast<HDData::DynamicSphereCollider*>(hitCollider);
	if (hitDynamicSphere != nullptr)
	{
		if (RoundManager::Instance()->CheckHeadColliderOwner(hitDynamicSphere))
		{
			_isShootHead = true;
			GameManager::Instance()->GetMyInfo()->PlayHeadShotEffect();
			PlayParticle(hitPoint);
		}
		else
		{
			//Vector3 direction = hitDynamicSphere->GetTransform()->GetPosition() - hitPoint;
			//hitDynamicSphere->AddForce(direction, 4.0f, 1);
			//Vector3 axis = GetTransform()->GetRight();
			//hitDynamicSphere->AddTorque(axis, 4.0f, 1);
			//hitDynamicSphere->AddForceAtPoint(hitPoint, direction, 2.0f, 1);
			Vector3 forceDirection = hitDynamicSphere->GetTransform()->GetPosition() - hitPoint;
			hitDynamicSphere->AddForce(forceDirection, 1.5f, 1);
			Vector3 shootDirection = _headCam->GetTransform()->GetForward() - rayOrigin;
			Vector3 hitToCenter = hitDynamicSphere->GetTransform()->GetPosition() - hitPoint;
			Vector3 axis = {shootDirection.y * hitToCenter.z - shootDirection.z * hitToCenter.y,
							shootDirection.z * hitToCenter.x - shootDirection.x * hitToCenter.z,
							shootDirection.x * hitToCenter.y - shootDirection.y * hitToCenter.x};
			hitDynamicSphere->AddTorque(axis, 10.0f, 1);
		}
	}

	// 적군의 몸을 맞췄을 때
	HDData::DynamicCapsuleCollider* hitDynamicCapsule = dynamic_cast<HDData::DynamicCapsuleCollider*>(hitCollider);
	if (hitDynamicCapsule != nullptr)
	{
		RoundManager::Instance()->CheckBodyColliderOwner(hitDynamicCapsule);
		_isShootBody = true;
		PlayParticle(hitPoint);
	}

	// 건물을 맞췄을 때
	HDData::StaticBoxCollider* hitStaticBox = dynamic_cast<HDData::StaticBoxCollider*>(hitCollider);
	if (hitStaticBox != nullptr)
	{
		DecalPool::Instance().SummonDecal(hitPoint);
	}

	++_shootCount;
	--_bulletCount;
	_shootCooldown = 0.5f;
}

void PlayerMove::Reload()
{
	_shootCount = 0;
	//_playerState.second = ePlayerMoveState::IDLE;
	_bulletCount = GameManager::Instance()->GetMyInfo()->GetMaxBulletCount();
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

void PlayerMove::Tumble(Vector3 direction)
{
	// 데굴
	_playerColliderStanding->Move(direction, 30.0f, _deltaTime);
}

void PlayerMove::OnStateEnter(ePlayerMoveState state)
{
	switch (state)
	{
		case ePlayerMoveState::IDLE:
		{
			
			break;
		}
		case ePlayerMoveState::RUN:
		{
			_playerColliderStanding->AdjustFriction(0.02f, 0.01f);
			break;
		}
		case ePlayerMoveState::JUMP:
		{
			_playerColliderStanding->AdjustFriction(0.02f, 0.01f);
			_playerColliderStanding->Jump(Vector3::Zero);
			GameManager::Instance()->GetMyInfo()->audio->PlayOnce("2d_jump");
			//NetworkManager::Instance().SendPlayJump();
			_tpanimator->GetAllAC()->SetBool("isRunFront", false);
			_tpanimator->GetAllAC()->SetBool("isRunBack", false);
			_tpanimator->GetAllAC()->SetBool("isRunRight", false);
			_tpanimator->GetAllAC()->SetBool("isRunLeft", false);
			_tpanimator->GetAllAC()->SetTrigger("isJump");

			break;
		}
		case ePlayerMoveState::TUMBLE:
		{
			_tumbleTimer = 0.3f;
			_tumbleCooldown = 6.0f;
			recoilCooldown->SetTimerOn();
			cooldownCountText->SetTimerOn();
			GameManager::Instance()->GetMyInfo()->audio->PlayOnce("2d_roll");

			_headCam->ToggleCameraShake(true);
			_fpmesh->SetMeshActive(false, 0);
			_weapon->SetMeshActive(false, 0);
			_weapon->SetMeshActive(false, 1);
			_weapon->SetMeshActive(false, 2);
			_weapon->SetMeshActive(false, 3);

			if (_moveDirection == 5)
			{
				_tpanimator->GetAllAC()->SetTrigger("isRollFront");
				_tumbleDirection = GetTransform()->GetForward();
			}
			else
			{
				if (_moveDirection == 8 || _moveDirection == 7 || _moveDirection == 9)
				{
					_tpanimator->GetAllAC()->SetTrigger("isRollFront");
				}
				else if (_moveDirection == 4)
				{
					_tpanimator->GetAllAC()->SetTrigger("isRollLeft");
				}
				else if (_moveDirection == 6)
				{
					_tpanimator->GetAllAC()->SetTrigger("isRollRight");
				}
				else if (_moveDirection == 1 || _moveDirection == 3 || _moveDirection == 2)
				{
					_tpanimator->GetAllAC()->SetTrigger("isRollBack");
				}

				_tumbleDirection = DecideDisplacement(_moveDirection);
			}

			_headCam->TumbleCamera(_deltaTime);

			break;
		}
		case ePlayerMoveState::AIM:
		{
			_fpanimator->GetAllAC()->SetBool("isIdle", true);

			break;
		}
		case ePlayerMoveState::FIRE:
		{
			ShootGun();
			_fpanimator->GetAllAC()->SetTrigger("isFire");
			GameManager::Instance()->GetMyInfo()->audio->PlayOnce("2d_fire");
			_headCam->ToggleCameraShake(true);

			break;
		}
		case ePlayerMoveState::EMPTY:
		{
			_shootCooldown = 0.8f;

			break;
		}
		case ePlayerMoveState::RELOAD:
		{
			_fpanimator->GetAllAC()->SetTrigger("isReload");
			_tpanimator->GetAllAC()->SetTrigger("isReload");
			GameManager::Instance()->GetMyInfo()->audio->PlayOnce("2d_reload");
			_reloadTimer = 2.6f;

			break;
		}
		case ePlayerMoveState::DIE:
		{
			GameManager::Instance()->GetMyInfo()->audio->PlayOnce("2d_die");
			_tpanimator->GetAllAC()->SetBool("isDie", true);
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
			//_playerColliderStanding->ClearVeloY();
			_playerColliderStanding->Stop();

			break;	
		}

		case ePlayerMoveState::RUN:
		{
			if (_prevDirection != _moveDirection)
			{
				_playerColliderStanding->ClearForce();
			}
			_playerColliderStanding->Move(DecideDisplacement(_moveDirection), _moveSpeed, _deltaTime);

			if (_moveDirection == 8 || _moveDirection == 7 || _moveDirection == 9)
			{
				_tpanimator->GetAllAC()->SetBool("isRunFront", true);
				_tpanimator->GetAllAC()->SetBool("isRunBack", false);
				_tpanimator->GetAllAC()->SetBool("isRunRight", false);
				_tpanimator->GetAllAC()->SetBool("isRunLeft", false);
			}
			else if (_moveDirection == 4)
			{
				_tpanimator->GetAllAC()->SetBool("isRunLeft", true);
				_tpanimator->GetAllAC()->SetBool("isRunFront", false);
				_tpanimator->GetAllAC()->SetBool("isRunBack", false);
				_tpanimator->GetAllAC()->SetBool("isRunRight", false);
			}
			else if (_moveDirection == 6)
			{
				_tpanimator->GetAllAC()->SetBool("isRunRight", true);
				_tpanimator->GetAllAC()->SetBool("isRunFront", false);
				_tpanimator->GetAllAC()->SetBool("isRunBack", false);
				_tpanimator->GetAllAC()->SetBool("isRunLeft", false);
			}
			else if (_moveDirection == 1 || _moveDirection == 3 || _moveDirection == 2)
			{
				_tpanimator->GetAllAC()->SetBool("isRunBack", true);
				_tpanimator->GetAllAC()->SetBool("isRunFront", false);
				_tpanimator->GetAllAC()->SetBool("isRunRight", false);
				_tpanimator->GetAllAC()->SetBool("isRunLeft", false);
			}

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
			break;
		}
		case ePlayerMoveState::EMPTY:
		{

			break;
		}
		case ePlayerMoveState::RELOAD:
		{
			//_fpmesh->SetMeshActive(false, 0);
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
		case ePlayerMoveState::RUN:
		{
			//_playerColliderStanding->Stop();
			_playerColliderStanding->AdjustFriction(0.7f, 0.63f);
			_tpanimator->GetAllAC()->SetBool("isRunFront", false);
			_tpanimator->GetAllAC()->SetBool("isRunBack", false);
			_tpanimator->GetAllAC()->SetBool("isRunRight", false);
			_tpanimator->GetAllAC()->SetBool("isRunLeft", false);

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
			_fpanimator->GetAllAC()->SetBool("isIdle", false);

			break;
		}
		case ePlayerMoveState::FIRE:
		{
			// 반동 초기화
			_shootCount = 0;
			_headCam->ToggleCameraShake(false);
			_headCam->ResetCameraPos();
			_fpanimator->GetAllAC()->SetTrigger("isFire");

			break;
		}
		case ePlayerMoveState::EMPTY:
		{

			break;
		}
		case ePlayerMoveState::RELOAD:
		{
			//_fpmesh->SetMeshActive(true, 0);
			_fpanimator->GetAllAC()->SetTrigger("isReload");
			GameManager::Instance()->GetMyInfo()->audio->Stop("2d_reload");

			Reload();
			_reloadTimer = 0.0f;
			_shootCooldown = 0.2f;

			break;
		}
		case ePlayerMoveState::DIE:
		{
			_tpanimator->GetAllAC()->SetBool("isDie", false);
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

void PlayerMove::PlayParticle(Vector3 position)
{
	bloodParticle->GetTransform()->SetPosition(position);
	//Vector4 cameraRotation = _headCam->GetTransform()->GetRotation();
	//bloodParticle->GetTransform()->SetRotation(cameraRotation);
	//bloodParticle->GetTransform()->Rotate(0.0f, 90.0f, 0.0);
	bloodParticle->Play();
}

void PlayerMove::ResetState()
{
	_playerState.first = ePlayerMoveState::IDLE;
	_playerState.second = ePlayerMoveState::AIM;
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
	else if(index == 2)
	{
		return _playerState.second;
	}
	else
	{
		assert(false);
	}
}

std::unordered_map<int, HDData::DynamicCollider*>& PlayerMove::GetOtherPlayerCols()
{
	return _otherPlayers;
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
	if ((*colArr)->GetColType() != eColliderRole::PLAYER && _playerState.first == ePlayerMoveState::JUMP)
	{
		// 착지 판정
		_playerState.first = ePlayerMoveState::IDLE;
		_playerColliderStanding->AdjustFriction(0.7f, 0.63f);
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

			break;
		}
	}
}

void PlayerMove::StartRoundCam()
{
	_isHeadCam = true;
	_isFirstPersonPerspective = true;
}

void PlayerMove::InsertOtherPlayerInfo(int uid, HDData::DynamicCapsuleCollider* collider)
{
	_otherPlayers.insert({uid, collider});
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

void PlayerMove::StopMoving()
{
	_playerColliderStanding->Stop();
	_moveDirection = 5;
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

	static_cast<HDData::DynamicCollider*>(_playerColliderStanding->GetChildColliderVec()[0])->RotateOnAxis(rotationValue * 0.1f, rotAxis);
}

void PlayerMove::ToggleSit(bool isSit)
{
	if (isSit)
	{
		_playerColliderStanding->SetSitStand(1);
	}
	else
	{
		_playerColliderStanding->SetSitStand(2);
	}
}

void PlayerMove::Die()
{
	auto origin = _headCam->GetTransform()->GetPosition();
	_headCam->GetTransform()->Rotate(0, 90, 0);
	_playerColliderStanding->OnDisable();
	_playerState.second = ePlayerMoveState::AIM;
}

void PlayerMove::Respawn()
{
	_playerColliderStanding->OnEnable();
	_tpanimator->GetAllAC()->SetBool("isDie", false);
	Reload();
	_playerState.first = ePlayerMoveState::IDLE;
	_headCam->ResetCameraPos();
}

void PlayerMove::DecidePlayerState()
{
	// 이전 스테이트 정보로 저장해주고 넘어간다.
	_prevPlayerState.first = _playerState.first;
	_prevPlayerState.second = _playerState.second;

	//_isDie = GameManager::Instance()->GetMyInfo()->GetIsDie();
	//if (_isDie)
	//{
	//	_playerState.first = ePlayerMoveState::DIE;
	//}
	//else
	//{
	//	_playerState.first = ePlayerMoveState::IDLE;
	//}

	if (GameManager::Instance()->GetMyInfo()->GetIsDie() != _isDie)
	{
		if (_isDie)
		{
			_isDie = false;
			_playerState.first = ePlayerMoveState::IDLE;
		}
		else
		{
			_isDie = true;
			_playerState.first = ePlayerMoveState::DIE;
		}
	}
	if (_isDie)
	{
		return;
	}

	// 구르기 타이머 체크해서 상태 종료
	if (_tumbleTimer > 0.0f)
	{
		return;
	}

	if (!_isMovable)
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
		_playerState.first = ePlayerMoveState::RUN;
	}
}

void PlayerMove::DecidePlayerStateSecond()
{
	// 재장전, 사격 (BULLET_MAX 6)
	if (_reloadTimer > 0.0f)
	{
		return;
	}
	else if ((API::GetKeyDown(DIK_R) && _bulletCount < 6) || (_bulletCount == 0 && _shootCooldown <= 0.0f && _playerState.first != ePlayerMoveState::TUMBLE && _playerState.second != ePlayerMoveState::RELOAD))
	{
		_playerState.second = ePlayerMoveState::RELOAD;
		return;
	}

	if (_shootCooldown <= 0.0f)
	{
		if (API::GetMouseDown(MOUSE_LEFT))
		{
			if (_bulletCount != 0)
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
		//_playerColliderStanding->ClearVeloY();
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
	
	//float finalRotX = _rotAngleX * (1.0f - ratio) + _prevRotAngleX * ratio;
	//float finalRotY = _rotAngleY * (1.0f - ratio) + _prevRotAngleY * ratio;

	Quaternion rot = rot.CreateFromYawPitchRoll(_rotAngleY, 0.0f, 0.0f);
	_playerColliderStanding->SetColliderRotation(rot);

	// 통짜 콜라이더일 때 위아래 카메라 움직이는 부분
	Quaternion pitchRotQuat = Quaternion::CreateFromYawPitchRoll(_rotAngleY, _rotAngleX, 0.0f);
	_headCam->GetTransform()->SetRotation(pitchRotQuat);

	// 메쉬 회전
	Quaternion rotX = Quaternion::CreateFromAxisAngle({ 1.0f, 0.0f, 0.0f }, _rotAngleX);
	_fpMeshObj->GetTransform()->SetLocalRotation(rotX);
}
