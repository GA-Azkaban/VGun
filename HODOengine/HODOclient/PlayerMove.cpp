#include "PlayerMove.h"
#include "../HODOengine/DynamicCollider.h"

PlayerMove::PlayerMove()
{

}

void PlayerMove::Start()
{
	_playerCollider = this->GetGameObject()->GetComponent<HDData::DynamicBoxCollider>();
	_moveSpeed = 3.0f;

	pitchAngle = 0.0f;

	_isOnGround = false;
	_isJumping = true;
}

void PlayerMove::Update()
{
	// 델타 타임 체크
	_deltaTime = API::GetDeltaTime();

	// check on_ground state
	if (_isJumping)
	{
		CheckIsOnGround();
	}

	if (API::GetMouseDown(MOUSE_LEFT))
	{
		ShootGun();
	}

	// 마우스에 따른 플레이어 회전 체크
	CheckLookDirection();

	// 키보드에 따른 플레이어 이동 방향 체크
	CheckMoveInfo();

	CameraControl();

	// 이동, 회전
	Move(_moveDirection);

	//API::DrawLineDir({ 0.f,0.f,0.f }, GetTransform()->GetPosition(), 10.0f, { 1.0f,0.0f,0.0f,1.0f });
	
	API::DrawLineDir(_headCam->GetTransform()->GetPosition(), _headCam->GetTransform()->GetForward(), 10.0f, { 1.0f, 0.0f, 1.0f, 1.0f });

	UpdatePlayerPositionDebug();
}

void PlayerMove::SetPlayerCamera(HDData::Camera* camera)
{
	_playerCamera = camera;
}

void PlayerMove::SetPlayerText(HDData::TextUI* pos, HDData::TextUI* aim, HDData::TextUI* hit)
{
	_playerInfoText = pos;
	_aimText = aim;
	_hitText = hit;
}

// 조이스틱 개념
void PlayerMove::CheckMoveInfo()
{
	_moveDirection = 5;

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
	if (API::GetKeyDown(DIK_SPACE))
	{
		Jump();
	}
	if (API::GetKeyDown(DIK_LSHIFT))
	{
		_playerCollider->AdjustVelocity(2.0f);
		_moveSpeed = 6.0f;
	}
	if (API::GetKeyUp(DIK_LSHIFT))
	{
		_playerCollider->AdjustVelocity(0.5f); // 0.5f -> can be replaced with certain ratio or variable
		_moveSpeed = 3.0f;
	}
}


void PlayerMove::CheckLookDirection()
{

}

bool PlayerMove::CheckIsOnGround()
{
	Vector3 pos = this->GetTransform()->GetPosition();
	const float delta = 0.2f;
	float x[9] = { -delta, -delta,0, delta,delta,delta,0,-delta,0 };
	float z[9] = { 0,delta,delta,delta,0,-delta,-delta,-delta,0 };

	for (int i = 0; i < 9; ++i)
	{
		//RocketEngine::RMFLOAT4 worldPos = RMFloat4MultiplyMatrix(RocketEngine::RMFLOAT4(pos.x + x[i], pos.y, pos.z + z[i], 1.0f), gameObject->transform.GetWorldTM());
		float halfHeight = _playerCollider->GetHeight() / 2.0f;
		Vector3 worldPos = Vector3(pos.x + x[i], pos.y - halfHeight - 0.001f * (i - 1), pos.z + z[i]);

		int type = 0;
		HDData::Collider* temp = API::ShootRay({ worldPos.x, worldPos.y, worldPos.z }, { 0.0f, -1.0f,0.0f }, 0.05f, &type);
		//RocketEngine::DrawDebugLine({ worldPos.x,worldPos.y,worldPos.z }, { eachDir.x,eachDir.y,eachDir.z });
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
		_playerCollider->Move(DecideMoveDirection(_moveDirection), _moveSpeed);
	}

	_prevDirection = _moveDirection;
}

void PlayerMove::ShootGun()
{
	HDData::Collider* hitCollider = nullptr;

	Vector3 rayOrigin = GetTransform()->GetPosition() + GetTransform()->GetForward() * 2.0f;
	Vector3 hitPoint = {1.0f, 1.0f, 1.0f};

	hitCollider = API::ShootRayHitPoint(rayOrigin, GetTransform()->GetForward(), hitPoint);
	
	HDData::DynamicCollider* hitDynamic = dynamic_cast<HDData::DynamicCollider*>(hitCollider);
	
	if (hitDynamic != nullptr)
	{
		Vector3 forceDirection = hitCollider->GetTransform()->GetPosition() - hitPoint;
		hitDynamic->AddForce(forceDirection, 5.0f);
		//_hitText->GetTransform()->SetPosition(hitPoint); // must setPos in screenSpace
	}
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
	HDData::Transform* headCamTransform = _headCam->GetTransform();
	headCamTransform->SetLocalPosition(headCamTransform->GetLocalPosition() + headCamTransform->GetForward() * 0.5f);
}

void PlayerMove::ToggleCam()
{
	HDData::Camera* nowCam = API::GetMainCamera();

	if (nowCam == _headCam)
	{
		API::SetMainCamera(_prevCam);
		_prevCam = nullptr;
		_isHeadCam = false;
		_aimText->SetText("");
	}
	else
	{
		_prevCam = API::SetMainCamera(_headCam);
		_isHeadCam = true;
		_aimText->SetText("O");
	}
}

void PlayerMove::Jump()
{
	//CheckIsOnGround();

	if ((!_isJumping) && (_isOnGround))
		//if (!_isJumping)
	{
		// 점프
		_playerCollider->Jump();
		_isJumping = true;
		//_isOnGround = false;
	}
}


Vector3 PlayerMove::DecideMoveDirection(int direction)
{
	Vector3 moveStep;

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
	HDData::Transform* cameraTransform = _headCam->GetGameObject()->GetTransform();
	Quaternion rot = cameraTransform->GetLocalRotation();
	float eulerAngleX = std::atan2(2.0f * (rot.w * rot.x + rot.y * rot.z), 1.0f - 2.0f * (rot.x * rot.x + rot.y * rot.y));

	if (eulerAngleX > 89.0f)
	{
		//constexpr float radX = HDMath::ToRadian(89.0f) * 0.5f;
		constexpr float radX = DirectX::XMConvertToRadians(89.0f) * 0.5f;
		Quaternion closedAngle = { std::cos(radX), std::sin(radX), 0.f, 0.f };

		cameraTransform->SetLocalRotation(closedAngle);
	}
	else if (eulerAngleX < -89.0f)
	{
		//constexpr float radX = HDMath::ToRadian(-89.0f) * 0.5f;
		constexpr float radX = DirectX::XMConvertToRadians(-89.0f) * 0.5f;
		Quaternion closedAngle = { std::cos(radX), std::sin(radX), 0.f, 0.f };

		cameraTransform->SetLocalRotation(closedAngle);
	}
	else
	{
		/*
		Vector4 rotationAxis{ 1.f, 0.f, 0.f, 1.0f };
		//rotationAxis = Vector4MultiplyMatrix(rotationAxis, cameraTransform->GetLocalRotationMatrix());
		//Quaternion newRot = HDMath::HDRotateQuaternion(cameraTransform->GetLocalRotation(),
		//	{ rotationAxis.x, rotationAxis.y, rotationAxis.z }, rotationValue);
		Matrix localRotMat = Matrix::CreateFromQuaternion(cameraTransform->GetLocalRotation());
		rotationAxis = XMVector4Transform(rotationAxis, localRotMat);
		
		Quaternion newRot = XMQuaternionMultiply(cameraTransform->GetLocalRotation(), DirectX::XMQuaternionRotationAxis(rotationAxis, rotationValue));

		//newRot.RotateTowards(rotationAxis, rotationValue);
		cameraTransform->SetLocalRotation(newRot);

		//cameraTransform->Rotate(rotationValue, 0.f, 0.f);
		*/

		/*
		Vector4 rotationAxis{ 1.f, 0.f, 0.f, 1.f };
		rotationAxis = XMVector4Transform(rotationAxis, Matrix::CreateFromQuaternion(cameraTransform->GetLocalRotation()));
		Quaternion newRot = XMQuaternionMultiply(cameraTransform->GetLocalRotation(), DirectX::XMQuaternionRotationAxis(rotationAxis, rotationValue));
		
		cameraTransform->SetLocalRotation(newRot);
		*/

		/*
		Vector3 rotationAxis = cameraTransform->GetRight();

		Quaternion newRot = XMQuaternionMultiply(cameraTransform->GetLocalRotation(), Quaternion::CreateFromAxisAngle(rotationAxis, rotationValue));

		cameraTransform->SetLocalRotation(newRot);
		*/

		/*
		Quaternion curRot = cameraTransform->GetRotation();
		Quaternion curRotInverse;
		curRot.Inverse(curRotInverse);

		cameraTransform->Rotate(curRotInverse);
		cameraTransform->Rotate(rotationValue, 0.f, 0.f);
		cameraTransform->Rotate(curRot);
		*/

		Vector4 rotationAxis{ 1.f, 0.f, 0.f, 0.f };
		rotationAxis = XMVector4Transform(rotationAxis, Matrix::CreateFromQuaternion(cameraTransform->GetRotation()));

		pitchAngle += rotationValue;
		Quaternion rotToX = Quaternion::CreateFromAxisAngle(Vector3(rotationAxis.x, rotationAxis.y, rotationAxis.z), pitchAngle);

		cameraTransform->SetLocalRotation(rotToX);
	}
}

void PlayerMove::Yaw(float radian)
{
	// rotation along Z-direction. necessary?
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

	// RotateY
	_playerCollider->Rotate(mouseDelta.x * 0.002f);	// adjust sensitivity later (0.002f -> variable)

	// Pitch in closed angle
	Pitch(mouseDelta.y * 0.002f);
}
