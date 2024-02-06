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
		_shakeDuration(0.1f),
		_distYOnShoot(0.0f),
		_pitchAngle(0.0f)
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

	void Camera::ShakeCamera(float deltaTime)
	{
		if (!_isShakingCamera)
		{
			return;
		}

		float shakeIntensity = 0.03f;
		float shakeFrequency = 90.0f;

		if (_shakeDuration > 0.0f)
		{
			GetGameObject()->GetTransform()->Translate(0.0f, -_distYOnShoot, 0.0f);
			_distYOnShoot = shakeIntensity * cos(shakeFrequency * _shakeDuration);
			GetGameObject()->GetTransform()->Translate(0.0f, _distYOnShoot, 0.0f);

			_shakeDuration -= deltaTime;
		}
		else
		{
			GetGameObject()->GetTransform()->Translate(0.0f, -_distYOnShoot, 0.0f);
			_shakeDuration = 0.1f;
			_isShakingCamera = false;
			_distYOnShoot = 0.0f;
		}
	}

	void Camera::EnableCameraShake()
	{
		_isShakingCamera = true;
	}

	void Camera::Pitch(float radianAngle)
	{
		/*
		//Vector4 rotAxis = XMVector4Transform(Vector4(1.0f, 0.0f, 0.0f, 1.0f), Matrix::CreateFromQuaternion(GetTransform()->GetRotation()));

		Vector3 rotAngleEuler = GetTransform()->GetLocalRotationEuler();

		Quaternion quatRotateY = DirectX::XMQuaternionRotationAxis(Vector3(0.0f,1.0f,0.0f), rotAngleEuler.y);
		
		Vector4 rotAxis = XMVector4Transform(Vector4(1.0f, 0.0f, 0.0f, 1.0f), Matrix::CreateFromQuaternion(quatRotateY));
		
		Vector3 normalizedAxis = DirectX::XMVector3Normalize(Vector3(rotAxis.x, rotAxis.y, rotAxis.z));

		float halfAngle = radianAngle * 0.5f;

		Quaternion rotQuat = DirectX::XMQuaternionRotationAxis(DirectX::XMLoadFloat3(&normalizedAxis), halfAngle);

		Vector4 originalRotQuat = GetTransform()->GetLocalRotation();

		Vector4 rotatedResult = DirectX::XMQuaternionMultiply(rotQuat, originalRotQuat);

		GetTransform()->SetLocalRotation(rotatedResult);

		//GetTransform()->Rotate(rotQuat);
		*/
		
		/*
		Vector3 rotAxis = DirectX::XMVector3Normalize(GetTransform()->GetRight());
		
		//Quaternion rotQuat = DirectX::XMQuaternionRotationRollPitchYaw(radianAngle, 0.0f, 0.0f);
		Quaternion rotQuat = DirectX::XMQuaternionRotationAxis(rotAxis, radianAngle);

		Quaternion nowRot = GetTransform()->GetLocalRotation();

		GetTransform()->SetLocalRotation(Quaternion::Concatenate(rotQuat,nowRot));
		*/

		/*
		Vector3 axis = DirectX::XMVector3Normalize(GetTransform()->GetRight());
		Quaternion newRot = DirectX::XMQuaternionRotationAxis(axis, radianAngle);
		GetTransform()->Rotate(newRot);
		*/

		/// 왜 복원력이
		/*
		//Quaternion newRot = Quaternion::Concatenate(DirectX::XMQuaternionRotationAxis(Vector3(1.0f, 0.0f, 0.0f), radianAngle), GetTransform()->GetLocalRotation());
		float newPitchAngle = DirectX::XMConvertToRadians(GetTransform()->GetLocalRotation().ToEuler().x) + radianAngle;

		float rotAngleY = GetTransform()->GetRotation().ToEuler().y;
		Vector3 newAxisX = DirectX::XMVector3TransformCoord(Vector3(1.0f, 0.0f, 0.0f), DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rotAngleY)));

		Quaternion newRot = DirectX::XMQuaternionRotationAxis(newAxisX, newPitchAngle);

		GetTransform()->SetLocalRotation(newRot);
		*/




		return;

		HDData::Transform* cameraTransform = GetTransform();

		Quaternion rot = cameraTransform->GetLocalRotation();

		Vector3 rotAngle = rot.ToEuler();

		Vector4 rotationAxis{ 1.f, 0.f, 0.f, 0.f };
		rotationAxis = XMVector4Transform(rotationAxis, Matrix::CreateFromQuaternion(cameraTransform->GetRotation()));


		_pitchAngle += radianAngle;
		Quaternion rotToX = Quaternion::CreateFromAxisAngle(Vector3(rotationAxis.x, rotationAxis.y, rotationAxis.z), _pitchAngle);

		cameraTransform->SetLocalRotation(rotToX);
	}

}
