#include "Camera.h"
#include "Transform.h"
#include "GameObject.h"
#include "..\\HODO3DGraphicsInterface\\ICamera.h"
#include "GraphicsObjFactory.h"

namespace HDData
{
	Camera::Camera()
		: _nearZ(0.01f),
		_farZ(1000.0f),
		_aspect(16.0f / 9.0f),
		_fovY(70.0f),
		_nearWindowHeight(),
		_farWindowHeight(),
		_graphicsCamera(HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateCamera()),
		_isShakingCamera(false),
		_shakeTime(0.1f),
		_distYOnShoot(0.0f),
		_camActive(true)
	{

	}

	float Camera::GetNearZ() const
	{
		return _nearZ;
	}

	float Camera::GetFarZ() const
	{
		return _farZ;
	}

	float Camera::GetAspect() const
	{
		return _aspect;
	}

	float Camera::GetFovX() const
	{
		return DirectX::XMConvertToDegrees(GetRadianFovX());
		//return HDMath::ToDegree(GetRadianFovX());
	}

	float Camera::GetRadianFovX() const
	{
		float halfWidth = GetNearWindowWidth() / 2;
		return 2.0f * atan(halfWidth / _nearZ);
	}

	float Camera::GetFovY() const
	{
		return _fovY;
	}

	float Camera::GetRadianFovY() const
	{
		return DirectX::XMConvertToRadians(_fovY);
		// return HDMath::ToRadian(_fovY);
	}

	float Camera::GetNearWindowWidth() const
	{
		return _aspect * _nearWindowHeight;
	}

	float Camera::GetNearWindowHeight() const
	{
		return _nearWindowHeight;
	}

	float Camera::GetFarWindowWidth() const
	{
		return _aspect * _farWindowHeight;
	}

	float Camera::GetFarWindowHeight() const
	{
		return _farWindowHeight;
	}

	Matrix Camera::GetViewMatrix() const
	{
		return _viewMatrix;
	}

	Matrix Camera::GetProjMatrix() const
	{
		return _projMatrix;
	}

	void Camera::SetNearZ(float nearZ)
	{
		_nearZ = nearZ;
		_graphicsCamera->SetNearZ(nearZ);
	}

	void Camera::SetFarZ(float farZ)
	{
		_farZ = farZ;
		_graphicsCamera->SetFarZ(farZ);
	}

	void Camera::SetAspect(float aspect)
	{
		_aspect = aspect;
		_graphicsCamera->SetAspect(aspect);
	}

	void Camera::SetFovY(float fovY)
	{
		_fovY = fovY;
		_graphicsCamera->SetFOVY(fovY);
	}

	void Camera::UpdateRenderData()
	{
		_graphicsCamera->SetPositionAndRotation(GetTransform()->GetPosition(), GetTransform()->GetRotation());
	}

	/*void Camera::Update()
	{
		_graphicsCamera->SetWorldTM(GetTransform()->GetWorldTM());
	}*/

	void Camera::SetAsMainCamera()
	{
		_graphicsCamera->SetAsMainCamera();
	}

	void Camera::ShakeCamera(float deltaTime, float& angleX)
	{
		if (!_isShakingCamera)
		{
			return;
		}

		float shakeIntensity = 0.008f;
		float shakeFrequency = 31.4f;

		if (_shakeTime < 0.1f)
		{
			//_distYOnShoot = shakeIntensity * sin(shakeFrequency * _shakeTime);
			//GetGameObject()->GetTransform()->SetLocalPosition(Vector3(0.0f, 1.65f + _distYOnShoot, 0.175f));

			// 카메라를 돌려주는 방식 시도
			//Quaternion nowRot = GetTransform()->GetLocalRotation();
			//Quaternion newRot = Quaternion::Concatenate(nowRot, Quaternion::CreateFromYawPitchRoll(0.0f, -_distYOnShoot, 0.0f));
			//GetGameObject()->GetTransform()->SetLocalRotation(newRot);

			_distYOnShoot = shakeIntensity * sin(shakeFrequency * _shakeTime);
			angleX -= _distYOnShoot;
			GetGameObject()->GetTransform()->SetLocalPosition(Vector3(0.0f, 1.65f, 0.175f - _distYOnShoot * 20.0f));

			_shakeTime += deltaTime;
		}
		else if (_shakeTime < 0.4f)
		{
			shakeFrequency = 7.86f;
			shakeIntensity = 0.002f;

			_distYOnShoot = shakeIntensity * sin(shakeFrequency * _shakeTime);
			angleX += _distYOnShoot;
			GetGameObject()->GetTransform()->SetLocalPosition(Vector3(0.0f, 1.65f, 0.175f - _distYOnShoot * 20.0f));

			_shakeTime += deltaTime;
		}
		else
		{
			//GetGameObject()->GetTransform()->Translate(0.0f, _distYOnShootEnd, 0.0f);

			ResetCameraPos();
		}
	}

	void Camera::TumbleCamera(float deltaTime)
	{
		if (!_isShakingCamera)
		{
			return;
		}

		if (_shakeTime < 0.3f)
		{
			float distY = sin(_shakeTime * 10.472f);

			GetGameObject()->GetTransform()->SetLocalPosition(Vector3(0.0f, 1.65f - distY, 0.175f));

			//GetGameObject()->GetTransform()->Translate(0.0f, _distYOnShoot, 0.0f);
			//_distYOnShootEnd -= _distYOnShoot;
			//GetGameObject()->GetTransform()->Translate(0.0f, _distYOnShoot, 0.0f);
			//_distYOnShootEnd -= _distYOnShoot;

			_shakeTime += deltaTime;
		}
		else
		{
			//GetGameObject()->GetTransform()->Translate(0.0f, _distYOnShootEnd, 0.0f);
			ResetCameraPos();
		}
	}

	void Camera::ToggleCameraShake(bool isShake)
{
		_isShakingCamera = isShake;
	}

	void Camera::ResetCameraPos()
	{
		_shakeTime = 0.0f;
		_isShakingCamera = false;
		_distYOnShoot = 0.0f;
		GetGameObject()->GetTransform()->SetLocalPosition(Vector3{ 0.0f, 1.65f, 0.175f });
	}

	void Camera::SetCamActive(bool active)
{
		_camActive = active;
	}

	bool Camera::GetCamActive() const
{
		return _camActive;
	}

}
