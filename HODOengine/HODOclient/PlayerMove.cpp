#include "PlayerMove.h"
#include "../HODOengine/DynamicCollider.h"
#include "FPAniScript.h"
#include "PlayerInfo.h"
#include "GameManager.h"
#include "RoundManager.h"

PlayerMove::PlayerMove()
	: _isMovable(true),
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
	_animator = GetGameObject()->AddComponent<HDData::Animator>();
	API::LoadFPAnimationFromData(GetGameObject(), "FP_animation.json");
	GetGameObject()->AddComponent<FPAniScript>();

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

	_playerAudio->PlayRepeat("bgm");
}

void PlayerMove::Update()
{
	if (GameManager::Instance()->GetMyInfo()->GetIsDie())
	{
		Die();
	}
	else
	{
		Respawn();
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
	DecidePlayerState();
	CoolTime();
	Behavior();

	// sound 관련
	_playerAudio->UpdateSoundPos(_playerPos);
	PlayPlayerSound();

	/*
	// 탄창 비었는데 쏘면 딸깍소리
	if (API::GetMouseDown(MOUSE_LEFT))
	{
		if (_bulletCount <= 0 && !_isReloading)
		{
			_playerAudio->PlayOnce("empty");
		}
	}

	if(API::GetKeyDown(DIK_LCONTROL))
	{
		ToggleSit(true);
	}
	else if(API::GetKeyUp(DIK_LCONTROL))
	{
		ToggleSit(false);
	}

	_isShootHead = false;
	_isShootBody = false;

	if (API::GetMouseHold(MOUSE_LEFT) && _shootCooldown <= 0.0f)
	{
		ShootGunDdabal();
	}

	if (API::GetMouseUp(MOUSE_LEFT))
	{
		// 반동 리셋
		_shootCount = 0;
		_headCam->ToggleCameraShake(false);
		_headCam->ResetCameraPos();
	}

	if (API::GetKeyDown(DIK_R))
	{
		if (_isReloading == false && _bulletCount < 30)
		{
			// 여기에 재장전 애니메이션과 소리 넣기
			_playerAudio->PlayOnce("reload");
			_isReloading = true;
		}
	}
	Reload();

	// 마우스에 따른 플레이어 회전 체크
	CheckLookDirection();

	// 키보드에 따른 플레이어 이동 방향 체크
	CheckMoveInfo();

	CameraControl();
	_headCam->ShakeCamera(_deltaTime);

	// 이동, 회전
	Move(_moveDirection);
	*/

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
	Vector3 rayOrigin = Vector3(pos.x, pos.y - 0.04f, pos.z);

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
	_headCam->ToggleCameraShake(true);

	HDData::Collider* hitCollider = nullptr;

	Vector3 rayOrigin = GetTransform()->GetPosition() + GetTransform()->GetForward() * 2.0f;
	Vector3 hitPoint = { 1.0f, 1.0f, 1.0f };

	hitCollider = API::ShootRayHitPoint(rayOrigin, GetTransform()->GetForward(), hitPoint);

	HDData::DynamicCollider* hitDynamic = dynamic_cast<HDData::DynamicCollider*>(hitCollider);

	if (hitDynamic != nullptr)
	{
		Vector3 forceDirection = hitCollider->GetTransform()->GetPosition() - hitPoint;
		hitDynamic->AddForce(forceDirection, 50.0f);
		//_hitText->GetTransform()->SetPosition(hitPoint); // must setPos in screenSpace
	}
}

void PlayerMove::ShootGunDdabal()
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


	// 총 쏴서
	HDData::Collider* hitCollider = nullptr;

	Vector3 rayOrigin = _headCam->GetTransform()->GetPosition() + _headCam->GetTransform()->GetForward() * 1.0f;
	Vector3 hitPoint = { 314.0f, 314.0f, 314.0f };

	Vector3 recoilDirection = _headCam->GetTransform()->GetForward() +
		_headCam->GetTransform()->GetRight() * _sprayPattern[_shootCount].first * -4.0f +
		_headCam->GetTransform()->GetUp() * _sprayPattern[_shootCount].second * -0.02f;
	recoilDirection.Normalize(recoilDirection);
	hitCollider = API::ShootRayHitPoint(rayOrigin, recoilDirection, hitPoint);
	//hitCollider = API::ShootRayHitPoint(rayOrigin, _headCam->GetTransform()->GetForward(), hitPoint);
	_playerAudio->PlayOnce("shoot");
	_playerAudio->PlayOnce("shoot2");

	// 맞은 데에 빨간 점 나오게 하기
	if (hitCollider != nullptr)
	{
		_playerAudio->PlayOnce("hit");
		SpawnParticle(hitPoint);
	}

	// 적군의 머리를 맞췄을 때
	HDData::DynamicSphereCollider* hitDynamicSphere = dynamic_cast<HDData::DynamicSphereCollider*>(hitCollider);
	if (hitDynamicSphere != nullptr)
	{
		RoundManager::Instance()->CheckHeadColliderOwner(hitDynamicSphere);
		_isShootHead = true;
		_playerAudio->PlayOnce("hitBody");
		_playerAudio->PlayOnce("hitHead");
	}

	// 적군의 몸을 맞췄을 때
	HDData::DynamicCapsuleCollider* hitDynamicCapsule = dynamic_cast<HDData::DynamicCapsuleCollider*>(hitCollider);
	if (hitDynamicCapsule != nullptr)
	{
		RoundManager::Instance()->CheckBodyColliderOwner(hitDynamicCapsule);
		_isShootBody = true;
		_playerAudio->PlayOnce("hitBody");
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

void PlayerMove::Tumble(Vector3 direction)
{
	// 데굴
	_fpmesh->SetMeshActive(false, 0);
	_weapon->SetMeshActive(false, 0);
	_weapon->SetMeshActive(false, 1);
	_weapon->SetMeshActive(false, 2);
	_weapon->SetMeshActive(false, 3);
	_playerColliderStanding->Move(direction, 8.0f, _deltaTime);
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
		case ePlayerMoveState::IDLE :
		{

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

		default:
		{

		}
	}
}

int& PlayerMove::GetBulletCount()
{
	return _bulletCount;
}

void PlayerMove::OnCollisionEnter(HDData::PhysicsCollision** colArr, unsigned int count)
{
	//++_enterCount;
	//if (_enterCount == 1)
	//{
	//	return;
	//}

	if (_playerState.first == ePlayerMoveState::TUMBLE)
	{
		return;
	}

	auto& opponentCollider = (*colArr)->_otherActor;

	// 지형인 경우
	if (opponentCollider->GetColType() == eColliderRole::TERRAIN)
	{
		// 착지 판정
		_isOnGround = true;
		_isJumping = false;
		_playerState.first = ePlayerMoveState::IDLE;
		_playerAudio->PlayOnce("land");
		_playerColliderStanding->ClearForceXYZ();
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
	_playerColliderStanding->OnDisable();
}

void PlayerMove::Respawn()
{
	_playerColliderStanding->OnEnable();
}

void PlayerMove::DecidePlayerState()
{
	_prevPlayerState.first = _playerState.first;
	_prevPlayerState.second = _playerState.second;

	if (_playerState.first == ePlayerMoveState::TUMBLE)
	{
		if (_tumbleTimer > 0.0f)
		{
			_tumbleTimer -= _deltaTime;
			return;
		}
		else
		{
			_weapon->SetMeshActive(true, 0);
			_weapon->SetMeshActive(true, 1);
			_weapon->SetMeshActive(true, 2);
			_weapon->SetMeshActive(true, 3);
			_fpmesh->SetMeshActive(true, 0);
		}
	}

	// jump가 아닐 때만 walk나 run이 될 수 있다.
	if (_playerState.first != ePlayerMoveState::JUMP)
	{
		if (_moveDirection == 5)
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

		// jump, tumble 들어오면 덮어씌우고
		if (API::GetKeyDown(DIK_SPACE))
		{
			_playerState.first = ePlayerMoveState::JUMP;
		}
		else if (API::GetKeyDown(DIK_LSHIFT))
		{
			_playerState.first = ePlayerMoveState::TUMBLE;

			// 재장전 중에 구르는 경우
			if (_prevPlayerState.second == ePlayerMoveState::RELOAD)
			{
				_playerAudio->Stop("reload");
				_reloadTimer = 0.0f;
			}
		}
	// tumble, jump 들어오면 덮어씌우고
	if (API::GetKeyDown(DIK_LSHIFT))
	{
		if (_tumbleCooldown <= 0.0f)
		{
			_playerState.first = ePlayerMoveState::TUMBLE;
			// 구르기 쿨타임 5초로 설정
			_tumbleCooldown = 5.0f;
		}
	}



	// shoot, reload 는 second에 넣어주고
	//if (_playerState.second == ePlayerMoveState::RELOAD)
	//{
	//	if (_reloadTimer > 0.0f)
	//	{
	//		_reloadTimer -= _deltaTime;
	//		return;
	//	}
	//}

	if (_playerState.second != ePlayerMoveState::RELOAD)
	{
		if (API::GetMouseDown(MOUSE_LEFT))
		{
			_playerState.second = ePlayerMoveState::FIRE;
		}
		else if (API::GetMouseUp(MOUSE_LEFT))
		{
			_playerState.second = ePlayerMoveState::IDLE;
			_shootCount = 0;
		}
		if (API::GetKeyDown(DIK_R) && _bulletCount < GameManager::Instance()->GetMyInfo()->GetMaxBulletCount())
		{
			_playerState.second = ePlayerMoveState::RELOAD;
		}
	}
}

void PlayerMove::Behavior()
{
	/*
	if (_prevPlayerState.first == _playerState.first)
	{
		OnStateStay(_playerState.first);
	}
	else
	{
		OnStateExit(_prevPlayerState.first);
		OnStateEnter(_playerState.first);
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
	*/

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

				_tumbleTimer = 0.4f;
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
				ShootGunDdabal();
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
		}
		if (_prevPlayerState.second == ePlayerMoveState::FIRE)
		{
			// 반동 초기화
			_shootCount = 0;
			_headCam->ToggleCameraShake(false);
			_headCam->ResetCameraPos();
		}
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
}

void PlayerMove::Landing()
{

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

	if (mouseDelta.x > 500.0f)
	{
		mouseDelta.x = 500.0f;
	}
	if (mouseDelta.x < -500.0f)
	{
		mouseDelta.x = -500.0f;
	}

	if (mouseDelta.y > 500.0f)
	{
		mouseDelta.y = 500.0f;
	}
	if (mouseDelta.y < -500.0f)
	{
		mouseDelta.y = -500.0f;
	}

	_rotAngleY = (_rotAngleY + mouseDelta.x * 0.0005f);
	_rotAngleX = (_rotAngleX + mouseDelta.y * 0.0005f);

	if (_rotAngleX >= 1.5f)
	{
		_rotAngleX = 1.5f;
	}
	if (_rotAngleX <= -1.5f)
	{
		_rotAngleX = -1.5f;
	}

	Quaternion rot = rot.CreateFromYawPitchRoll(_rotAngleY, 0.0f, 0.0f);
	_playerColliderStanding->SetColliderRotation(rot);

	// 통짜 콜라이더일 때 위아래 카메라 움직이는 부분
	Quaternion pitchRotQuat = Quaternion::CreateFromYawPitchRoll(_rotAngleY, _rotAngleX, 0.0f);
	_headCam->GetTransform()->SetRotation(pitchRotQuat);

	// 메쉬 회전
	Quaternion rotX = Quaternion::CreateFromAxisAngle({ 1.0f, 0.0f, 0.0f }, _rotAngleX);
	_fpMeshObj->GetTransform()->SetLocalRotation(rotX);

}
