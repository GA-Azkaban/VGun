#include "Transform.h"
#include "GameObject.h"

namespace HDData
{
	Transform::Transform()
		:_position(), _rotation(), _scale(HDMath::HDFLOAT3::one)
	{

	}

	HDMath::HDFLOAT3 Transform::GetWorldPosition() const
	{
		/// �̵���� ���Ӽ��� p.619 ����
		// �� ���� �����ǿ� �θ��� ���� Ʈ�������� �ݿ��� �� ���� �������� ���Ѵ�.
		if (GetGameObject()->GetParentGameObject() == nullptr)
		{
			return _position;
		}

		HDMath::HDFLOAT3 parentPosition = GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldPosition();
		HDMath::HDQuaternion parentRotation = GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldRotation();
		HDMath::HDFLOAT3 parentScale = GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldScale();

		return parentPosition + parentRotation * (parentScale * _position);
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
		return GetTransformMatrix(GetWorldPosition(), GetWorldRotation(), GetWorldScale());
	}

	HDMath::HDFLOAT4X4 Transform::GetLocalTM() const
	{
		return GetTransformMatrix(_position, _rotation, _scale);
	}

	HDMath::HDFLOAT3 Transform::GetForward() const
	{
		HDMath::HDFLOAT4 temp = HDFloat4MultiplyMatrix(HDMath::HDFLOAT4(0.0f, 0.0f, 1.0f, 0.0f), GetWorldTM());
		return HDMath::HDFLOAT3(temp.x, temp.y, temp.z);
	}

	HDMath::HDFLOAT3 Transform::GetUp() const
	{
		HDMath::HDFLOAT4 temp = HDFloat4MultiplyMatrix(HDMath::HDFLOAT4(0.0f, 1.0f, 0.0f, 0.0f), GetWorldTM());
		return HDMath::HDFLOAT3(temp.x, temp.y, temp.z);
	}

	HDMath::HDFLOAT3 Transform::GetRight() const
	{
		HDMath::HDFLOAT4 temp = HDFloat4MultiplyMatrix(HDMath::HDFLOAT4(1.0f, 0.0f, 0.0f, 0.0f), GetWorldTM());
		return HDMath::HDFLOAT3(temp.x, temp.y, temp.z);
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
			// ���� �������� ���� ���� ������ ����
			// �θ� ���� ����Ŀ� �� ���带 ���ϸ� ����
			HDMath::HDFLOAT4X4 worldTM = GetTransformMatrix(position, _rotation, _scale);
			HDMath::HDFLOAT4X4 invParent = HDMath::HDFLOAT4X4::Identity;
			invParent = GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldTM().Inverse();
			_position = GetLocalPositionFromLocalTM(invParent * worldTM);
		}
	}

	void Transform::SetWorldPosition(float x, float y, float z)
	{
		SetWorldPosition(HDMath::HDFLOAT3(x, y, z));
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
	}

	void Transform::SetWorldRotation(float w, float x, float y, float z)
	{
		SetWorldRotation(HDMath::HDQuaternion(w, x, y, z));
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
	}

	void Transform::SetLocalPosition(const HDMath::HDFLOAT3& position)
	{
		_position = position;
	}

	void Transform::SetLocalRotation(const HDMath::HDQuaternion& rotation)
	{
		_rotation = rotation;
	}

	void Transform::SetLocalScale(const HDMath::HDFLOAT3& scale)
	{
		_scale = scale;
	}

	void Transform::Translate(const HDMath::HDFLOAT3& position)
	{
		_position.x += position.x;
		_position.y += position.y;
		_position.z += position.z;
	}

	void Transform::Translate(float x, float y, float z)
	{
		_position.x += x;
		_position.y += y;
		_position.z += z;
	}

	void Transform::Rotate(float angleX, float angleY, float angleZ)
	{
		float radianX = HDMath::ToRadian(angleX);
		float radianY = HDMath::ToRadian(angleY);
		float radianZ = HDMath::ToRadian(angleZ);

		// ȸ�� ������ ������ ������ �غ��մϴ�.
		float half_radianX = radianX * 0.5f;
		float half_radianY = radianY * 0.5f;
		float half_radianZ = radianZ * 0.5f;

		// �� ���� ���ʹϾ� ��Ҹ� ����մϴ�.
		float sin_half_radianX = sin(half_radianX);
		float sin_half_radianY = sin(half_radianY);
		float sin_half_radianZ = sin(half_radianZ);
		float cos_half_radianX = cos(half_radianX);
		float cos_half_radianY = cos(half_radianY);
		float cos_half_radianZ = cos(half_radianZ);

		// ���ʹϾ� ��Ҹ� ����մϴ�.
		HDMath::HDFLOAT4 rotQuat;
		rotQuat.x = sin_half_radianX * cos_half_radianY * cos_half_radianZ + cos_half_radianX * sin_half_radianY * sin_half_radianZ;
		rotQuat.y = cos_half_radianX * sin_half_radianY * cos_half_radianZ - sin_half_radianX * cos_half_radianY * sin_half_radianZ;
		rotQuat.z = cos_half_radianX * cos_half_radianY * sin_half_radianZ + sin_half_radianX * sin_half_radianY * cos_half_radianZ;
		rotQuat.w = cos_half_radianX * cos_half_radianY * cos_half_radianZ - sin_half_radianX * sin_half_radianY * sin_half_radianZ;

		// ���� ���ʹϾ�� ȸ�� ���ʹϾ��� ������ ��� ���ʹϾ��� ����մϴ�.
		HDMath::HDQuaternion result;
		result.x = _rotation.w * rotQuat.x + _rotation.x * rotQuat.w + _rotation.y * rotQuat.z - _rotation.z * rotQuat.y;
		result.y = _rotation.w * rotQuat.y - _rotation.x * rotQuat.z + _rotation.y * rotQuat.w + _rotation.z * rotQuat.x;
		result.z = _rotation.w * rotQuat.z + _rotation.x * rotQuat.y - _rotation.y * rotQuat.x + _rotation.z * rotQuat.w;
		result.w = _rotation.w * rotQuat.w - _rotation.x * rotQuat.x - _rotation.y * rotQuat.y - _rotation.z * rotQuat.z;

		// ����� �����մϴ�.
		_rotation = result;
	}

	void Transform::Rotate(const HDMath::HDQuaternion& quaternion)
	{
		_rotation = HDMath::HDQuaternionMultiply(_rotation, quaternion);
	}

}
