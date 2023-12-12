#include "Transform.h"
#include "GameObject.h"


HDData::Transform::Transform()
	:_position(), _rotation(), _scale(HDMath::HDFLOAT3::one)
{

}

HDMath::HDFLOAT3 HDData::Transform::GetWorldPosition() const
{
	/// 이득우의 게임수학 p.619 참조
	// 내 로컬 포지션에 부모의 월드 트랜스폼을 반영해 내 월드 포지션을 구한다.
	if (GetGameObject()->GetParentGameObject() == nullptr)
	{
		return _position;
	}

	HDMath::HDFLOAT3 parentPosition = GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldPosition();
	HDMath::HDQuaternion parentRotation = GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldRotation();
	HDMath::HDFLOAT3 parentScale = GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldScale();
	
	return parentPosition + parentRotation * (parentScale * _position);
}

HDMath::HDQuaternion HDData::Transform::GetWorldRotation() const
{
	if (GetGameObject()->GetParentGameObject() == nullptr)
	{
		return _rotation;
	}

	return GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldRotation() * _rotation;
}

HDMath::HDFLOAT3 HDData::Transform::GetWorldScale() const
{
	if (GetGameObject()->GetParentGameObject() == nullptr)
	{
		return _scale;
	}

	return GetGameObject()->GetParentGameObject()->GetTransform()->GetWorldScale() * _scale;
}

HDMath::HDFLOAT3 HDData::Transform::GetLocalPosition() const
{
	return _position;
}

HDMath::HDQuaternion HDData::Transform::GetLocalRotation() const
{
	return _rotation;
}

HDMath::HDFLOAT3 HDData::Transform::GetLocalScale() const
{
	return _scale;
}

HDMath::HDFLOAT4X4 HDData::Transform::GetWorldTM() const
{
	if (GetGameObject()->GetParentGameObject() == nullptr)
	{
		return GetLocalTM();
	}
	return GetTransformMatrix(GetWorldPosition(), GetWorldRotation(), GetWorldScale());
}

HDMath::HDFLOAT4X4 HDData::Transform::GetLocalTM() const
{
	return GetTransformMatrix(_position, _rotation, _scale);
}

void HDData::Transform::SetWorldPosition(const HDMath::HDFLOAT3& position)
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
}

void HDData::Transform::SetWorldRotation(const HDMath::HDQuaternion& rotation)
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

void HDData::Transform::SetWorldScale(const HDMath::HDFLOAT3& scale)
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

void HDData::Transform::SetLocalPosition(const HDMath::HDFLOAT3& position)
{
	_position = position;
}

void HDData::Transform::SetLocalRotation(const HDMath::HDQuaternion& rotation)
{
	_rotation = rotation;
}

void HDData::Transform::SetLocalScale(const HDMath::HDFLOAT3& scale)
{
	_scale = scale;
}