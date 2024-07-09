#include <cmath>
#include "Camera.h"
#include "MathHeader.h"
#include "ResourceManager.h"
#include "ObjectManager.h"
#include "StaticMeshObject.h"
#include "SkinningMeshObject.h"
#include "Light.h"

using namespace DirectX;

namespace RocketCore::Graphics
{
	Camera* Camera::_mainCamera;

	Camera::Camera()
		: _position(0.0f, 2.0f, -10.0f),
		_rotation(0.0f, 0.0f, 0.0f, 1.0f),
		_nearZ(0.01f), _farZ(1000.0f), _aspect(16.0f / 9.0f), _fovY(90.0f),
		_nearWindowHeight(), _farWindowHeight(), _nearWindowWidth(), _farWindowWidth(),
		_viewMatrix(), _projectionMatrix()
	{
		_nearWindowHeight = 2.0f * _nearZ * std::tanf(XMConvertToRadians(_fovY / 2));
		_farWindowHeight = 2.0f * _farZ * std::tanf(XMConvertToRadians(_fovY / 2));
		_nearWindowWidth = _nearWindowHeight * _aspect;
		_farWindowWidth = _farWindowHeight * _aspect;
		UpdateProjectionMatrix();
	}

	Camera::~Camera()
	{

	}

	DirectX::XMFLOAT3 Camera::GetPosition() const
	{
		return _position;
	}

	void Camera::SetPosition(float x, float y, float z)
	{
		_position = { x,y,z };
	}

	void Camera::SetRotation(float x, float y, float z, float w)
	{
		_rotation = { x,y,z,w };
	}

	void Camera::UpdateViewMatrix()
	{
		XMFLOAT3 right = GetRight();
		XMFLOAT3 up = GetUp();
		XMFLOAT3 forward = GetForward();
		XMVECTOR R = XMLoadFloat3(&right);
		XMVECTOR U = XMLoadFloat3(&up);
		XMVECTOR L = XMLoadFloat3(&forward);
		XMVECTOR P = XMLoadFloat3(&_position);

		// Keep camera's axes orthogonal to each other and of unit length.
		L = XMVector3Normalize(L);
		U = XMVector3Normalize(XMVector3Cross(L, R));

		// U, L already ortho-normal, so no need to normalize cross product.
		R = XMVector3Cross(U, L);

		// Fill in the view matrix entries.
		float x = -XMVectorGetX(XMVector3Dot(P, R));
		float y = -XMVectorGetX(XMVector3Dot(P, U));
		float z = -XMVectorGetX(XMVector3Dot(P, L));

		DirectX::XMFLOAT3 _right;
		DirectX::XMFLOAT3 _up;
		DirectX::XMFLOAT3 _look;

		XMStoreFloat3(&_right, R);
		XMStoreFloat3(&_up, U);
		XMStoreFloat3(&_look, L);

		_viewMatrix.r[0].m128_f32[0] = _right.x;
		_viewMatrix.r[1].m128_f32[0] = _right.y;
		_viewMatrix.r[2].m128_f32[0] = _right.z;
		_viewMatrix.r[3].m128_f32[0] = x;

		//_viewMatrix(0, 0) = _right.x;
		//_viewMatrix(1, 0) = _right.y;
		//_viewMatrix(2, 0) = _right.z;
		//_viewMatrix(3, 0) = x;

		_viewMatrix.r[0].m128_f32[1] = _up.x;
		_viewMatrix.r[1].m128_f32[1] = _up.y;
		_viewMatrix.r[2].m128_f32[1] = _up.z;
		_viewMatrix.r[3].m128_f32[1] = y;

		//_viewMatrix(0, 1) = _up.x;
		//_viewMatrix(1, 1) = _up.y;
		//_viewMatrix(2, 1) = _up.z;
		//_viewMatrix(3, 1) = y;

		_viewMatrix.r[0].m128_f32[2] = _look.x;
		_viewMatrix.r[1].m128_f32[2] = _look.y;
		_viewMatrix.r[2].m128_f32[2] = _look.z;
		_viewMatrix.r[3].m128_f32[2] = z;

		//_viewMatrix(0, 2) = _look.x;
		//_viewMatrix(1, 2) = _look.y;
		//_viewMatrix(2, 2) = _look.z;
		//_viewMatrix(3, 2) = z;

		_viewMatrix.r[0].m128_f32[3] = 0.0f;
		_viewMatrix.r[1].m128_f32[3] = 0.0f;
		_viewMatrix.r[2].m128_f32[3] = 0.0f;
		_viewMatrix.r[3].m128_f32[3] = 1.0f;

		//_viewMatrix(0, 3) = 0.0f;
		//_viewMatrix(1, 3) = 0.0f;
		//_viewMatrix(2, 3) = 0.0f;
		//_viewMatrix(3, 3) = 1.0f;

		// 
		// 	viewMatrix_ =
		// 	{
		// 		right_.x,		up_.x,			look_.x,		0.0f,
		// 		right_.y,		up_.y,			look_.y,		0.0f,
		// 		right_.z,		up_.z,			look_.z,		0.0f,
		// 		-position_.x,	-position_.y,	-position_.z,	1.0f
		// 	};
	}

	void Camera::FrustumCulling()
	{
		BoundingFrustum frustum = _boundingFrustum;
		frustum.Transform(frustum, XMMatrixInverse(nullptr, _viewMatrix));

		XMMATRIX lightView = LightManager::Instance().GetLightView();
		XMMATRIX lightProj = LightManager::Instance().GetLightProj();

		BoundingFrustum lightFrustum(lightProj);
		lightFrustum.Transform(lightFrustum, XMMatrixInverse(nullptr, lightView));

		for (auto staticMeshObj : ObjectManager::Instance().GetStaticMeshObjList())
		{
			if (!staticMeshObj->IsActive())
			{
				staticMeshObj->SetCameraVisible(false);
				staticMeshObj->SetLightVisible(false);
				continue;
			}
			bool isInCameraFrustum = frustum.Intersects(staticMeshObj->GetBoundingBox());
			staticMeshObj->SetCameraVisible(isInCameraFrustum);
			bool isInLightFrustum = lightFrustum.Intersects(staticMeshObj->GetBoundingBox());
			staticMeshObj->SetLightVisible(isInLightFrustum);
		}

		for (auto skinningMeshObj : ObjectManager::Instance().GetSkinningMeshObjList())
		{
			if (!skinningMeshObj->IsActive())
			{
				skinningMeshObj->SetCameraVisible(false);
				skinningMeshObj->SetLightVisible(false);
				continue;
			}
			//bool isInCameraFrustum = frustum.Intersects(skinningMeshObj->GetBoundingBox());
			//skinningMeshObj->SetCameraVisible(isInCameraFrustum);
			//bool isInLightFrustum = lightFrustum.Intersects(skinningMeshObj->GetBoundingBox());
			//skinningMeshObj->SetLightVisible(isInLightFrustum);
		}
	}

	DirectX::XMMATRIX Camera::GetViewMatrix() const
	{
		return _viewMatrix;
	}

	DirectX::XMMATRIX Camera::GetProjectionMatrix() const
	{
		return _projectionMatrix;
	}

	DirectX::XMMATRIX Camera::GetViewProjectionMatrix() const
	{
		return XMMatrixMultiply(GetViewMatrix(), GetProjectionMatrix());
	}

	DirectX::XMFLOAT3 Camera::GetForward() const
	{
		XMFLOAT3 ret;
		XMVECTOR forward = { 0.0f,0.0f,1.0f };
		auto rotationMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&_rotation));
		auto result = DirectX::XMVector3Transform(forward, rotationMatrix);
		XMStoreFloat3(&ret, result);
		return ret;
	}

	DirectX::XMFLOAT3 Camera::GetUp() const
	{
		XMFLOAT3 ret;
		XMVECTOR up = { 0.0f,1.0f,0.0f };
		auto rotationMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&_rotation));
		auto result = DirectX::XMVector3Transform(up, rotationMatrix);
		XMStoreFloat3(&ret, result);
		return ret;
	}

	DirectX::XMFLOAT3 Camera::GetRight() const
	{
		XMFLOAT3 ret;
		XMVECTOR right = { 1.0f,0.0f,0.0f };
		auto rotationMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&_rotation));
		auto result = DirectX::XMVector3Transform(right, rotationMatrix);
		XMStoreFloat3(&ret, result);
		return ret;
	}

	float Camera::GetFOVY() const
	{
		return _fovY;
	}

	float Camera::GetFOVZ() const
	{
		return _fovY * _aspect;
	}

	float Camera::GetAspect() const
	{
		return _aspect;
	}

	float Camera::GetNearZ() const
	{
		return _nearZ;
	}

	float Camera::GetFarZ() const
	{
		return _farZ;
	}

	void Camera::SetWorldTM(const Matrix& matrix)
	{
		_worldMatrix = matrix;
// 		for (int i = 0; i < 4; i++)
// 		{
// 			for (int j = 0; j < 4; j++)
// 			{
// 				_worldMatrix.m[i][j] = matrix.element[i][j];
// 			}
// 		}

		//XMMATRIX worldTM = XMLoadFloat4x4(&_worldMatrix);
		/*XMVECTOR scale;
		XMVECTOR rotation;
		XMVECTOR translate;
		XMMatrixDecompose(&scale, &rotation, &translate, worldTM);
		XMStoreFloat3(&_position, translate);
		XMStoreFloat4(&_rotation, rotation);

		UpdateViewMatrix();*/
	}

	void Camera::SetNearZ(float nearZ)
	{
		_nearZ = nearZ;
		UpdateProjectionMatrix();
	}

	void Camera::SetFarZ(float farZ)
	{
		_farZ = farZ;
		UpdateProjectionMatrix();
	}

	void Camera::SetAspect(float aspect)
	{
		_aspect = aspect;
		UpdateProjectionMatrix();
	}

	void Camera::SetFOVY(float fov)
	{
		_fovY = fov;
		UpdateProjectionMatrix();
	}

	void Camera::SetNearHeight(float height)
	{
		_nearWindowHeight = height;
		float nearZ = _nearWindowHeight * 0.5f / tan(0.5f * _fovY);
		SetNearZ(nearZ);
	}

	void Camera::SetFarHeight(float height)
	{
		_farWindowHeight = height;
		float farZ = _farWindowHeight * 0.5f / tan(0.5f * _fovY);
		SetFarZ(farZ);
	}

	void Camera::UpdateProjectionMatrix()
	{
		_projectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(_fovY / 2), _aspect, _nearZ, _farZ);
		_boundingFrustum = DirectX::BoundingFrustum(_projectionMatrix);
	}

	void Camera::SetAsMainCamera()
	{
		_mainCamera = this;
	}

	Camera* Camera::GetMainCamera()
	{
		return _mainCamera;
	}

	void Camera::SetPositionAndRotation(const Vector3& pos, const Quaternion& rot)
	{
		SetPosition(pos.x, pos.y, pos.z);
		SetRotation(rot.x, rot.y, rot.z, rot.w);
	}

	void Camera::SetViewMatrix(const DirectX::XMMATRIX& tm)
	{
		_viewMatrix = tm;
	}

}
