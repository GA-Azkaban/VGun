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
		_graphicsCamera(HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateCamera())
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
}
