#include "PlayerMove.h"
#include "../HODOengine/DynamicCollider.h"
#include "FPAniScript.h"

PlayerMove::PlayerMove()
	: _isMovable(true),
	_particleIndex(0),
	_shootCooldown(0.0f),
	_jumpCooldown(0.0f),
	_shootCount(0),
	_bulletCount(30),
	_reloadTimer(0.0f),
	_isReloading(false),
	_isRunning(false),
	_rotAngleX(0.0f), _rotAngleY(0.0f),
	_isFirstPersonPerspective(true),
	_isJumping(true), _isOnGround(false)
{
	
}

void PlayerMove::Start()
{
	_animator = GetGameObject()->AddComponent<HDData::Animator>();
	API::LoadFPAnimationFromData(GetGameObject(), "FP_animation.json");
	GetGameObject()->AddComponent<FPAniScript>();

	_playerCollider = GetGameObject()->GetComponent<HDData::DynamicCapsuleCollider>();
	_fpMeshObj = GetGameObject()->GetGameObjectByNameInChildren("meshShell");	
	_moveSpeed = 3.0f;
	_playerAudio = GetGameObject()->GetComponent<HDData::AudioSource>();

	PresetSprayPattern();
	StartRoundCam();
}

void PlayerMove::Update()
{
	if (!_isMovable)
	{
		return;
	}
	// 델타 타임 체크
	_deltaTime = API::GetDeltaTime();

	// check on_ground state
	if (_isJumping)
	{
		CheckIsOnGround();
	}
	//else
	//{
	//	_playerCollider->ClearVeloY();
	//}

	// 쿨타임 방식의 점프 관리
	//if (_jumpCooldown >= 0.0f)
	//{
	//	_jumpCooldown -= _deltaTime;
	//}
	//else
	//{
	//	if (_isJumping)
	//	{
	//		_isJumping = false;
	//		_playerCollider->ClearVeloY();
	//	}
	//}

	//if (API::GetMouseDown(MOUSE_LEFT))
	//{
	//	ShootGun();
	//}


	// 발사 쿨타임 및 파티클 수명관리
	if (_shootCooldown >= 0.0f)
	{
		_shootCooldown -= _deltaTime;
	}

	for (int i = 0; i < 30; ++i)
	{
		//_hitParticles[i]->CheckTimer(_deltaTime);
	}

	// 탄창 비었는데 쏘면 딸깍소리
	if (API::GetMouseDown(MOUSE_LEFT))
	{
		//_headCam->GetTransform()->SetLocalPosition(Vector3(0.0f, 0.10f, 0.18f));
		if (_bulletCount <= 0)
		{
			_playerAudio->PlayOnce("empty");
		}
	}

	if (API::GetMouseHold(MOUSE_LEFT) && _shootCooldown <= 0.0f)
	{
		ShootGunDdabal();
	}

	if (API::GetMouseUp(MOUSE_LEFT))
	{
		// 반동 리셋
		_shootCount = 0;
		//_headCam->GetTransform()->SetLocalPosition(Vector3(0.0f, 0.12f, 0.2f));
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

	// 이동, 회전
	Move(_moveDirection);

	//API::DrawLineDir({ 0.f,0.f,0.f }, GetTransform()->GetPosition(), 10.0f, { 1.0f,0.0f,0.0f,1.0f });
	
	API::DrawLineDir(_headCam->GetTransform()->GetPosition(), _headCam->GetTransform()->GetForward(), 10.0f, { 1.0f, 0.0f, 1.0f, 1.0f });

	////UpdatePlayerPositionDebug();
	//if (_tempFlag == 0)
	//{
	//	API::SetCurrentSceneMainCamera(_headCam);
	//	_headCam->SetAsMainCamera();
	//	_isHeadCam = true;
	//	//_aimText->SetText("O");
	//	_isFirstPersonPerspective = true;
	//	_headCam->GetTransform()->SetLocalPosition(Vector3(0.0f, 1.0f, 0.3f));

	//	_tempFlag = 1;
	//}
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

	if (_moveDirection != 5)
	{
		//if (!_playerAudio->IsSoundPlaying("walk"))
		//{
		//	_playerAudio->PlayOnce("walk");
		//}
	}
	if (API::GetKeyDown(DIK_SPACE))
	{
		CheckIsOnGround();
		Jump();
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
					//_playerAudio->Play3DOnce("landing");
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

		float halfHeight = _playerCollider->GetHeight() / 2.0f;
		Vector3 rayOrigin = Vector3(pos.x, pos.y - 0.04f, pos.z);

		int colliderType = 0;
		HDData::Collider* opponentCollider = API::ShootRay({ rayOrigin.x, rayOrigin.y, rayOrigin.z }, { 0.0f, -1.0f,0.0f }, 0.08f, &colliderType);
		API::DrawLineDir(rayOrigin, Vector3(0.f, 1.f, 0.f), 0.08f, Vector4(1.f, 0.f, 0.f, 0.f));

		if (opponentCollider)
		{
			// type 1이 rigidStatic.
			if (colliderType == 1)
			{
				// 상태 변경 및 착지 Sound.
				if (_isOnGround == false)
				{
					_isOnGround = true;
					_isJumping = false;
					_playerCollider->ClearVeloY();
					//_playerAudio->Play3DOnce("landing");
				}
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
			_playerCollider->Stop();
		}
	}
	else
	{
		_playerCollider->Move(DecideDisplacement(_moveDirection), _moveSpeed);

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
	_headCam->EnableCameraShake();

	HDData::Collider* hitCollider = nullptr;

	Vector3 rayOrigin = GetTransform()->GetPosition() + GetTransform()->GetForward() * 2.0f;
	Vector3 hitPoint = {1.0f, 1.0f, 1.0f};

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
	if (_bulletCount <= 0)	// 장탄수를 임시로 30발로 제한
	{
		return;
	}

	// 총기 반동
	ApplyRecoil();
	_headCam->EnableCameraShake();

	++_shootCount;
	--_bulletCount;

	// 총 쏴서
	HDData::Collider* hitCollider = nullptr;

	Vector3 rayOrigin = _headCam->GetTransform()->GetPosition() + _headCam->GetTransform()->GetForward() * 1.0f;
	Vector3 hitPoint = { 314.0f, 314.0f, 314.0f };

	hitCollider = API::ShootRayHitPoint(rayOrigin, _headCam->GetTransform()->GetForward(), hitPoint);
	_playerAudio->PlayOnce("shoot");

	// 맞은 데에 빨간 점 나오게 하기
	if (hitCollider != nullptr)
	{
		_playerAudio->PlayOnce("hit");
		SpawnParticle(hitPoint);
	}

	// 맞은 애가 dynamic이면 힘 가해주기
	HDData::DynamicCollider* hitDynamic = dynamic_cast<HDData::DynamicCollider*>(hitCollider);

	if (hitDynamic != nullptr)
	{
		Vector3 forceDirection = hitCollider->GetTransform()->GetPosition() - hitPoint;
		hitDynamic->AddForce(forceDirection, 5.0f);
		//_hitText->GetTransform()->SetPosition(hitPoint); // must setPos in screenSpace
	}

	_shootCooldown = 0.1f;
}

void PlayerMove::Reload()
{
	// 2초간 장전
	if (_isReloading == true)
	{
		_reloadTimer += _deltaTime;
		if (_reloadTimer >= 2.0f)
		{
			_bulletCount = 30;
			_isReloading = false;
			_reloadTimer = 0.0f;
		}
	}
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
	_rotAngleY += _sprayPattern[_shootCount].first;
	_rotAngleX += _sprayPattern[_shootCount].second * 0.01f;
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

HDData::Camera* PlayerMove::GetHeadCam() const
{
	return _headCam;
}

void PlayerMove::PresetSprayPattern()
{
	float scale = 1.0f;
	_sprayPattern[0] = std::make_pair(0.0f * scale, 0.0f * scale);
	_sprayPattern[1] = std::make_pair(0.001f * scale, -0.3f * scale);
	_sprayPattern[2] = std::make_pair(-0.001f * scale, -1.0f * scale);
	_sprayPattern[3] = std::make_pair(0.001f * scale, -1.5f * scale);
	_sprayPattern[4] = std::make_pair(0.001f * scale, -1.5f * scale);
	_sprayPattern[5] = std::make_pair(-0.003f * scale, -1.7f * scale);
	_sprayPattern[6] = std::make_pair(-0.002f * scale, -1.5f * scale);
	_sprayPattern[7] = std::make_pair(-0.003f * scale, -1.0f * scale);
	_sprayPattern[8] = std::make_pair(0.004f * scale, -1.0f * scale);
	_sprayPattern[9] = std::make_pair(0.01f * scale, 0.5f * scale);
	_sprayPattern[10] = std::make_pair(0.005f * scale, -0.3f * scale);
	_sprayPattern[11] = std::make_pair(-0.003f * scale, -0.6f * scale);
	_sprayPattern[12] = std::make_pair(0.004f * scale, -0.6f * scale);
	_sprayPattern[13] = std::make_pair(0.008f * scale, 0.7f * scale);
	_sprayPattern[14] = std::make_pair(0.001f * scale, -0.2f * scale);
	_sprayPattern[15] = std::make_pair(-0.011f * scale, -0.1f * scale);
	_sprayPattern[16] = std::make_pair(-0.003f * scale, -0.4f * scale);
	_sprayPattern[17] = std::make_pair(-0.003f * scale, -0.6f * scale);
	_sprayPattern[18] = std::make_pair(-0.006f * scale, 0.3f * scale);
	_sprayPattern[19] = std::make_pair(-0.003f * scale, 0.7f * scale);
	_sprayPattern[20] = std::make_pair(-0.005f * scale, -0.1f * scale);
	_sprayPattern[21] = std::make_pair(0.004f * scale, 0.0f * scale);
	_sprayPattern[22] = std::make_pair(0.001f * scale, -1.0f * scale);
	_sprayPattern[23] = std::make_pair(0.001f * scale, -0.4f * scale);
	_sprayPattern[24] = std::make_pair(-0.005f * scale, 0.1f * scale);
	_sprayPattern[25] = std::make_pair(0.004f * scale, 0.0f * scale);
	_sprayPattern[26] = std::make_pair(0.008f * scale, 0.6f * scale);
	_sprayPattern[27] = std::make_pair(0.01f * scale, 1.3f * scale);
	_sprayPattern[28] = std::make_pair(0.004f * scale, -0.4f * scale);
	_sprayPattern[29] = std::make_pair(0.002f * scale, -0.2f * scale);
}

void PlayerMove::StartRoundCam()
{
	_isHeadCam = true;
	_isFirstPersonPerspective = true;
}

void PlayerMove::ToggleCam()
{
	if(_isHeadCam)
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

void PlayerMove::Jump()
{
	//CheckIsOnGround();

	//if ((!_isJumping) && (_isOnGround))
		//if (!_isJumping)
	if(!_isJumping)
	{
		// 점프
		_playerCollider->Jump();
		_playerAudio->PlayOnce("jump");
		_isJumping = true;

		_jumpCooldown = 0.8f;
		//_isOnGround = false;
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
					GetGameObject()->GetTransform()->GetForward() * _deltaTime * -_moveSpeed * 0.7f
					+ GetGameObject()->GetTransform()->GetRight() * _deltaTime * -_moveSpeed * 0.7f;
			}
			break;
			case 2:
			{
				moveStep = GetTransform()->GetForward() * _deltaTime * -_moveSpeed;
			}
			break;
			case 3:
			{
				moveStep =
					GetGameObject()->GetTransform()->GetForward() * _deltaTime * -_moveSpeed * 0.7f
					+ GetGameObject()->GetTransform()->GetRight() * _deltaTime * _moveSpeed * 0.7f;
			}
			break;
			case 4:
			{
				moveStep = GetTransform()->GetRight() * _deltaTime * -_moveSpeed;
			}
			break;
			case 5:
			{
				moveStep = Vector3::Zero;
			}
			break;
			case 6:
			{
				moveStep = GetTransform()->GetRight() * _deltaTime * _moveSpeed;
			}
			break;
			case 7:
			{
				moveStep =
					GetGameObject()->GetTransform()->GetForward() * _deltaTime * _moveSpeed * 0.7f
					+ GetGameObject()->GetTransform()->GetRight() * _deltaTime * -_moveSpeed * 0.7f;
			}
			break;
			case 8:
			{
				moveStep = GetTransform()->GetForward() * _deltaTime * _moveSpeed;
			}
			break;
			case 9:
			{
				moveStep =
					GetGameObject()->GetTransform()->GetForward() * _deltaTime * _moveSpeed * 0.7f
					+ GetGameObject()->GetTransform()->GetRight() * _deltaTime * _moveSpeed * 0.7f;
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
				moveStep = Vector3(_deltaTime * -_moveSpeed * 0.7f, 0.0f, _deltaTime * -_moveSpeed * 0.7f);
			}
			break;
			case 2:
			{
				moveStep = Vector3(0.0f, 0.0f, _deltaTime * -_moveSpeed);
			}
			break;
			case 3:
			{
				moveStep = Vector3(_deltaTime * _moveSpeed * 0.7f, 0.0f, _deltaTime * -_moveSpeed * 0.7f);
			}
			break;
			case 4:
			{
				moveStep = Vector3(_deltaTime * -_moveSpeed, 0.0f, 0.0f);
			}
			break;
			case 5:
			{
				moveStep = Vector3::Zero;
			}
			break;
			case 6:
			{
				moveStep = Vector3(_deltaTime * _moveSpeed, 0.0f, 0.0f);
			}
			break;
			case 7:
			{
				moveStep = Vector3(_deltaTime * -_moveSpeed * 0.7f, 0.0f, _deltaTime * _moveSpeed * 0.7f);
			}
			break;
			case 8:
			{
				moveStep = Vector3(0.0f, 0.0f, _deltaTime * _moveSpeed);
			}
			break;
			case 9:
			{
				moveStep = Vector3(_deltaTime * _moveSpeed * 0.7f, 0.0f, _deltaTime * _moveSpeed * 0.7f);
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

	if (API::GetKeyDown(DIK_P))
	{
		//ToggleCameraView();
		ToggleCam();
	}

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
	static_cast<HDData::DynamicCollider*>(_playerCollider->GetChildColliderVec()[0])->RotateOnAxis(rotationValue * 0.1f, rotAxis);
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

	float angleX = 0.0f;
	float angleY = 0.0f;
	
	angleX += mouseDelta.y * 0.001f;
	angleY += mouseDelta.x * 0.001f;

	_rotAngleY = (_rotAngleY + mouseDelta.x * 0.001f);
	_rotAngleX = (_rotAngleX + mouseDelta.y * 0.001f);
	
	if (_rotAngleX >= 1.5f)
	{
		_rotAngleX = 1.5f;
	}
	if (_rotAngleX <= -1.5f)
	{
		_rotAngleX = -1.5f;
	}

	Quaternion rot = rot.CreateFromYawPitchRoll(_rotAngleY, 0.0f, 0.0f);
	_playerCollider->SetColliderRotation(rot);

	// 통짜 콜라이더일 때 위아래 카메라 움직이는 부분
	Quaternion pitchRotQuat = Quaternion::CreateFromYawPitchRoll(_rotAngleY, _rotAngleX, 0.0f);
	_headCam->GetTransform()->SetRotation(pitchRotQuat);

	// 메쉬 회전
	Quaternion rotX = Quaternion::CreateFromAxisAngle({ 1.0f, 0.0f, 0.0f }, _rotAngleX);
	_fpMeshObj->GetTransform()->SetLocalRotation(rotX);
	
}
