#include "Transform.h"
#include "GameObject.h"

namespace HDData
{
	Transform::Transform()
		:_position(0.0f,0.0f,0.0f), _rotation(1.0f,0.0f,0.0f,0.0f), _scale(HDMath::HDFLOAT3::one)
	{

	}

	HDMath::HDFLOAT3 Transform::GetWorldPosition() const
	{
		/// 이득우의 게임수학 p.619 참조
		// 내 로컬 포지션에 부모의 월드 트랜스폼을 반영해 내 월드 포지션을 구한다.
		if (GetGameObject()->GetParentGameObject() == nullptr)
		{
			return _position;
		}

		return GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldPosition() * _position;

// 		HDMath::HDFLOAT3 parentPosition = GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldPosition();
// 		HDMath::HDQuaternion parentRotation = GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldRotation();
// 		HDMath::HDFLOAT3 parentScale = GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldScale();
// 
// 		return parentPosition + parentRotation * (parentScale * _position);
	}

	HDMath::HDQuaternion Transform::GetWorldRotation() const
	{
		if (GetGameObject()->GetParentGameObject() == nullptr)
		{
			return _rotation;
		}

		return GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldRotation() * _rotation;
	}

	HDMath::HDFLOAT3 Transform::GetWorldScale() const
	{
		if (GetGameObject()->GetParentGameObject() == nullptr)
		{
			return _scale;
		}

		return GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldScale() * _scale;
	}

	HDMath::HDFLOAT3 Transform::GetLocalPosition() const
	{
		return _position;
	}

	HDMath::HDQuaternion Transform::GetLocalRotation() const
	{
		return _rotation;
	}

	HDMath::HDFLOAT3 Transform::GetLocalScale() const
	{
		return _scale;
	}

	HDMath::HDFLOAT4X4 Transform::GetWorldTM() const
	{
		if (GetGameObject()->GetParentGameObject() == nullptr)
		{
			return GetLocalTM();
		}

		return GetLocalTM() * GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldTM();
	}

	HDMath::HDFLOAT4X4 Transform::GetLocalTM() const
	{
		return GetTransformMatrix(_position, _rotation, _scale);
	}

	HDMath::HDFLOAT3 Transform::GetForward() const
	{
		return GetWorldRotation() * HDMath::HDFLOAT3(0.0f, 0.0f, 1.0f);
	}

	HDMath::HDFLOAT3 Transform::GetUp() const
	{
		return GetWorldRotation() * HDMath::HDFLOAT3(0.0f, 1.0f, 0.0f);
	}

	HDMath::HDFLOAT3 Transform::GetRight() const
	{
		return GetWorldRotation() * HDMath::HDFLOAT3(1.0f, 0.0f, 0.0f);
	}

	void Transform::SetWorldPosition(const HDMath::HDFLOAT3& position)
	{
		if (GetGameObject()->GetParentGameObject() == nullptr)
		{
			_position = position;
		}
		else
		{
			// world position = position;
			// 월드 포지션을 통해 로컬 포지션 갱신
			// 부모 월드 역행렬에 내 월드를 곱하면 로컬
			HDMath::HDFLOAT4X4 worldTM = GetTransformMatrix(position, _rotation, _scale);
			HDMath::HDFLOAT4X4 invParent = HDMath::HDFLOAT4X4::Identity;
			invParent = GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldTM().Inverse();
			_position = GetLocalPositionFromLocalTM(invParent * worldTM);
		}
		GetGameObject()->OnUpdateTransform();
	}

	void Transform::SetWorldPosition(float x, float y, float z)
	{
		SetWorldPosition(HDMath::HDFLOAT3(x, y, z));
		GetGameObject()->OnUpdateTransform();
	}

	void Transform::SetWorldRotation(const HDMath::HDQuaternion& rotation)
	{
		if (GetGameObject()->GetParentGameObject() == nullptr)
		{
			_rotation = rotation;
		}
		else
		{
			HDMath::HDFLOAT4X4 worldTM = GetTransformMatrix(_position, rotation, _scale);
			HDMath::HDFLOAT4X4 invParent = HDMath::HDFLOAT4X4::Identity;
			invParent = GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldTM().Inverse();
			_rotation = GetLocalRotationFromLocalTM(invParent * worldTM);
		}
		GetGameObject()->OnUpdateTransform();
	}

	void Transform::SetWorldRotation(float w, float x, float y, float z)
	{
		SetWorldRotation(HDMath::HDQuaternion(w, x, y, z));
		GetGameObject()->OnUpdateTransform();
	}

	void Transform::SetWorldScale(const HDMath::HDFLOAT3& scale)
	{
		if (GetGameObject()->GetParentGameObject() == nullptr)
		{
			_scale = scale;
		}
		else
		{
			HDMath::HDFLOAT4X4 worldTM = GetTransformMatrix(_position, _rotation, scale);
			HDMath::HDFLOAT4X4 invParent = HDMath::HDFLOAT4X4::Identity;
			invParent = GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldTM().Inverse();
			_scale = GetLocalScaleFromLocalTM(invParent * worldTM);
		}
		GetGameObject()->OnUpdateTransform();
	}

	void Transform::SetLocalPosition(const HDMath::HDFLOAT3& position)
	{
		_position = position;
		GetGameObject()->OnUpdateTransform();
	}

	void Transform::SetLocalRotation(const HDMath::HDQuaternion& rotation)
	{
		_rotation = rotation;
		GetGameObject()->OnUpdateTransform();
	}

	void Transform::SetLocalScale(const HDMath::HDFLOAT3& scale)
	{
		_scale = scale;
		GetGameObject()->OnUpdateTransform();
	}

	void Transform::Translate(const HDMath::HDFLOAT3& position)
	{
		_position.x += position.x;
		_position.y += position.y;
		_position.z += position.z;
		GetGameObject()->OnUpdateTransform();
	}

	void Transform::Translate(float x, float y, float z)
	{
		_position.x += x;
		_position.y += y;
		_position.z += z;
		GetGameObject()->OnUpdateTransform();
	}

	void Transform::Rotate(float angleX, float angleY, float angleZ)
	{
		float radianX = HDMath::ToRadian(angleX);
		float radianY = HDMath::ToRadian(angleY);
		float radianZ = HDMath::ToRadian(angleZ);

		// 회전 각도를 반으로 나누어 준비합니다.
		float half_radianX = radianX * 0.5f;
		float half_radianY = radianY * 0.5f;
		float half_radianZ = radianZ * 0.5f;

		// 각 축의 쿼터니언 요소를 계산합니다.
		float sin_half_radianX = sin(half_radianX);
		float sin_half_radianY = sin(half_radianY);
		float sin_half_radianZ = sin(half_radianZ);
		float cos_half_radianX = cos(half_radianX);
		float cos_half_radianY = cos(half_radianY);
		float cos_half_radianZ = cos(half_radianZ);

		// 쿼터니언 요소를 계산합니다.
		HDMath::HDFLOAT4 rotQuat;
		rotQuat.x = sin_half_radianX * cos_half_radianY * cos_half_radianZ + cos_half_radianX * sin_half_radianY * sin_half_radianZ;
		rotQuat.y = cos_half_radianX * sin_half_radianY * cos_half_radianZ - sin_half_radianX * cos_half_radianY * sin_half_radianZ;
		rotQuat.z = cos_half_radianX * cos_half_radianY * sin_half_radianZ + sin_half_radianX * sin_half_radianY * cos_half_radianZ;
		rotQuat.w = cos_half_radianX * cos_half_radianY * cos_half_radianZ - sin_half_radianX * sin_half_radianY * sin_half_radianZ;

		// 원본 쿼터니언과 회전 쿼터니언의 곱으로 결과 쿼터니언을 계산합니다.
		HDMath::HDQuaternion result;
		result.x = _rotation.w * rotQuat.x + _rotation.x * rotQuat.w + _rotation.y * rotQuat.z - _rotation.z * rotQuat.y;
		result.y = _rotation.w * rotQuat.y - _rotation.x * rotQuat.z + _rotation.y * rotQuat.w + _rotation.z * rotQuat.x;
		result.z = _rotation.w * rotQuat.z + _rotation.x * rotQuat.y - _rotation.y * rotQuat.x + _rotation.z * rotQuat.w;
		result.w = _rotation.w * rotQuat.w - _rotation.x * rotQuat.x - _rotation.y * rotQuat.y - _rotation.z * rotQuat.z;

		// 결과를 저장합니다.
		_rotation = result;
		GetGameObject()->OnUpdateTransform();
	}

	void Transform::Rotate(const HDMath::HDQuaternion& quaternion)
	{
		_rotation = HDMath::HDQuaternionMultiply(_rotation, quaternion);
		GetGameObject()->OnUpdateTransform();
	}

}
