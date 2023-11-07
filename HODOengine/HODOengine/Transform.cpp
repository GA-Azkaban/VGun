#include "Transform.h"
#include "GameObject.h"


hodoEngine::Transform::Transform()
	:_position(), _rotation(), _scale(HDMaths::HDFLOAT3::one)
{

}

HDMaths::HDFLOAT3 hodoEngine::Transform::GetWorldPosition() const
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

HDMaths::HDQuaternion hodoEngine::Transform::GetWorldRotation() const
{
	if (GetGameObject()->GetParentGameObject() == nullptr)
	{
		return _rotation;
	}

	return GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldRotation() * _rotation;
}

HDMaths::HDFLOAT3 hodoEngine::Transform::GetWorldScale() const
{
	if (GetGameObject()->GetParentGameObject() == nullptr)
	{
		return _scale;
	}

	return GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldScale() * _scale;
}

HDMaths::HDFLOAT3 hodoEngine::Transform::GetLocalPosition() const
{
	return _position;
}

HDMaths::HDQuaternion hodoEngine::Transform::GetLocalRotation() const
{
	return _rotation;
}

HDMaths::HDFLOAT3 hodoEngine::Transform::GetLocalScale() const
{
	return _scale;
}

HDMaths::HDFLOAT4X4 hodoEngine::Transform::GetWorldTM() const
{
	if (GetGameObject()->GetParentGameObject() == nullptr)
	{
		return GetLocalTM();
	}
	return GetTransformMatrix(GetWorldPosition(), GetWorldRotation(), GetWorldScale());
}

HDMaths::HDFLOAT4X4 hodoEngine::Transform::GetLocalTM() const
{
	return GetTransformMatrix(_position, _rotation, _scale);
}

void hodoEngine::Transform::SetWorldPosition(const HDMaths::HDFLOAT3& position)
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

void hodoEngine::Transform::SetWorldRotation(const HDMaths::HDQuaternion& rotation)
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
		_rotation = HDMaths::GetLocalRotationFromLocalTM(invParent * worldTM);
	}
}

void hodoEngine::Transform::SetWorldScale(const HDMaths::HDFLOAT3& scale)
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
		_scale = HDMaths::GetLocalScaleFromLocalTM(invParent * worldTM);
	}
}

void hodoEngine::Transform::SetLocalPosition(const HDMaths::HDFLOAT3& position)
{
	_position = position;
}

void hodoEngine::Transform::SetLocalRotation(const HDMaths::HDQuaternion& rotation)
{
	_rotation = rotation;
}

void hodoEngine::Transform::SetLocalScale(const HDMaths::HDFLOAT3& scale)
{
	_scale = scale;
}