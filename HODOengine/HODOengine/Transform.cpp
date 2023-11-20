#include "Transform.h"
#include "GameObject.h"


hodoData::Transform::Transform()
	:_position(), _rotation(), _scale(HDMaths::HDFLOAT3::one)
{

}

HDMaths::HDFLOAT3 hodoData::Transform::GetWorldPosition() const
{
	/// �̵���� ���Ӽ��� p.619 ����
	// �� ���� �����ǿ� �θ��� ���� Ʈ�������� �ݿ��� �� ���� �������� ���Ѵ�.
	if (GetGameObject()->GetParentGameObject() == nullptr)
	{
		return _position;
	}

	HDMaths::HDFLOAT3 parentPosition = GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldPosition();
	HDMaths::HDQuaternion parentRotation = GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldRotation();
	HDMaths::HDFLOAT3 parentScale = GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldScale();
	
	return parentPosition + parentRotation * (parentScale * _position);
}

HDMaths::HDQuaternion hodoData::Transform::GetWorldRotation() const
{
	if (GetGameObject()->GetParentGameObject() == nullptr)
	{
		return _rotation;
	}

	return GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldRotation() * _rotation;
}

HDMaths::HDFLOAT3 hodoData::Transform::GetWorldScale() const
{
	if (GetGameObject()->GetParentGameObject() == nullptr)
	{
		return _scale;
	}

	return GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldScale() * _scale;
}

HDMaths::HDFLOAT3 hodoData::Transform::GetLocalPosition() const
{
	return _position;
}

HDMaths::HDQuaternion hodoData::Transform::GetLocalRotation() const
{
	return _rotation;
}

HDMaths::HDFLOAT3 hodoData::Transform::GetLocalScale() const
{
	return _scale;
}

HDMaths::HDFLOAT4X4 hodoData::Transform::GetWorldTM() const
{
	if (GetGameObject()->GetParentGameObject() == nullptr)
	{
		return GetLocalTM();
	}
	return GetTransformMatrix(GetWorldPosition(), GetWorldRotation(), GetWorldScale());
}

HDMaths::HDFLOAT4X4 hodoData::Transform::GetLocalTM() const
{
	return GetTransformMatrix(_position, _rotation, _scale);
}

void hodoData::Transform::SetWorldPosition(const HDMaths::HDFLOAT3& position)
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
		HDMaths::HDFLOAT4X4 worldTM = GetTransformMatrix(position, _rotation, _scale);
		HDMaths::HDFLOAT4X4 invParent = HDMaths::HDFLOAT4X4::Identity;
		invParent = GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldTM().Inverse();
		_position = GetLocalPositionFromLocalTM(invParent * worldTM);
	}
}

void hodoData::Transform::SetWorldRotation(const HDMaths::HDQuaternion& rotation)
{
	if (GetGameObject()->GetParentGameObject() == nullptr)
	{
		_rotation = rotation;
	}
	else
	{
		HDMaths::HDFLOAT4X4 worldTM = GetTransformMatrix(_position, rotation, _scale);
		HDMaths::HDFLOAT4X4 invParent = HDMaths::HDFLOAT4X4::Identity;
		invParent = GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldTM().Inverse();
		_rotation = GetLocalRotationFromLocalTM(invParent * worldTM);
	}
}

void hodoData::Transform::SetWorldScale(const HDMaths::HDFLOAT3& scale)
{
	if (GetGameObject()->GetParentGameObject() == nullptr)
	{
		_scale = scale;
	}
	else
	{
		HDMaths::HDFLOAT4X4 worldTM = GetTransformMatrix(_position, _rotation, scale);
		HDMaths::HDFLOAT4X4 invParent = HDMaths::HDFLOAT4X4::Identity;
		invParent = GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldTM().Inverse();
		_scale = GetLocalScaleFromLocalTM(invParent * worldTM);
	}
}

void hodoData::Transform::SetLocalPosition(const HDMaths::HDFLOAT3& position)
{
	_position = position;
}

void hodoData::Transform::SetLocalRotation(const HDMaths::HDQuaternion& rotation)
{
	_rotation = rotation;
}

void hodoData::Transform::SetLocalScale(const HDMaths::HDFLOAT3& scale)
{
	_scale = scale;
}