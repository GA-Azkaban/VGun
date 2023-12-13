#include "Camera.h"
#include "Transform.h"
#include "GameObject.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\HODOmath.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\HODOmath.lib")
#endif

namespace HDData
{
	Camera::Camera()
		: _nearZ(0.01f),
		_farZ(1000.0f),
		_aspect(16.0f / 9.0f),
		_fovY(70.0f),
		_nearWindowHeight(),
		_farWindowHeight()
	{

	}

	void Camera::Start()
	{

	}

	HDEngine::CameraData& Camera::GetCameraData()
	{
		// 		float screenWidth = (float)RocketCore::RenderSystem::Instance().GetScreenWidth();
		// 		float screenHeight = (float)RocketCore::RenderSystem::Instance().GetScreenHeight();
		// 		_aspect = screenWidth / screenHeight;


		_cameraData.nearZ = _nearZ;
		_cameraData.farZ = _farZ;
		_cameraData.aspect = _aspect;
		_cameraData.fovY = _fovY;
		_cameraData.nearWindowHeight = _nearWindowHeight;
		_cameraData.farWindowHeight = _farWindowHeight;

		_cameraData.position = GetGameObject()->GetTransform()->GetWorldPosition();
		_cameraData.rotation = GetGameObject()->GetTransform()->GetWorldRotation();

		return _cameraData;
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
		return HDMath::ToDegree(GetRadianFovX());
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
		return HDMath::ToRadian(_fovY);
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

	HDMath::HDFLOAT4X4 Camera::GetViewMatrix() const
	{
		return _viewMatrix;
	}

	HDMath::HDFLOAT4X4 Camera::GetProjMatrix() const
	{
		return _projMatrix;
	}

	void Camera::SetNearZ(float nearZ)
	{
		_nearZ = nearZ;
		_cameraData.nearZ = nearZ;
	}

	void Camera::SetFarZ(float farZ)
	{
		_cameraData.farZ = farZ;
	}

	void Camera::SetAspect(float aspect)
	{
		_cameraData.aspect = aspect;
	}

	void Camera::SetFovY(float fovY)
	{
		_cameraData.fovY = fovY;
	}

	void Camera::Strafe(float delta)
	{
		HDMath::HDFLOAT3 rightVec = GetGameObject()->GetTransform()->GetRight();
		rightVec.x *= delta;
		rightVec.y *= delta;
		rightVec.z *= delta;

		GetGameObject()->GetTransform()->Translate(rightVec);
	}

	void Camera::Walk(float delta)
	{
		HDMath::HDFLOAT3 forwardVec = GetGameObject()->GetTransform()->GetForward();
		forwardVec.x *= delta;
		forwardVec.y *= delta;
		forwardVec.z *= delta;

		GetGameObject()->GetTransform()->Translate(forwardVec);
	}

	void Camera::WorldUpDown(float delta)
	{
		HDMath::HDFLOAT3 worldUpDelta = { 0.0f,delta,0.0f };
		GetGameObject()->GetTransform()->Translate(worldUpDelta);
	}

	void Camera::Pitch(float radian)
	{
		//RMQuaternion newRot = RMRotateQuaternion(gameObject->transform.GetLocalRotation(), gameObject->transform.GetRight(), angle);
		HDMath::HDFLOAT4 r{ 1.0f,0.0f,0.0f,1.0f };
		r = HDFloat4MultiplyMatrix(r, GetGameObject()->GetTransform()->GetLocalTM());
		HDMath::HDQuaternion newRot = HDRotateQuaternion(GetGameObject()->GetTransform()->GetLocalRotation(), { r.x,r.y,r.z }, radian);
		GetGameObject()->GetTransform()->SetLocalRotation(newRot);
	}

	void Camera::RotateY(float angle)
	{
		HDMath::HDQuaternion newRot = HDRotateQuaternion(GetGameObject()->GetTransform()->GetLocalRotation(), { 0.0f,1.0f,0.0f }, angle);
		GetGameObject()->GetTransform()->SetLocalRotation(newRot);
	}


}