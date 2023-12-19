#include "DynamicCapsuleCollider.h"
#include "GameObject.h"

HDData::DynamicCapsuleCollider::DynamicCapsuleCollider()
{

}

float HDData::DynamicCapsuleCollider::GetWidth() const
{
	return (_radius + _halfHeight) * 2 * _scaleOffset.y * GetGameObject()->GetTransform()->GetWorldScale().y;
}

float HDData::DynamicCapsuleCollider::GetHeight() const
{
	return (_radius + _halfHeight) * 2 * _scaleOffset.x * GetGameObject()->GetTransform()->GetWorldScale().x;
}

float HDData::DynamicCapsuleCollider::GetDepth() const
{
	return (_radius + _halfHeight) * 2 * _scaleOffset.z * GetGameObject()->GetTransform()->GetWorldScale().z;
}

float HDData::DynamicCapsuleCollider::GetRadius() const
{
	return _radius;
}

float HDData::DynamicCapsuleCollider::GetHalfHeight() const
{
	return _halfHeight;
}

void HDData::DynamicCapsuleCollider::SetCapsuleInfo(float radius, float halfHeight)
{
	_radius = radius;
	_halfHeight = halfHeight;
}

void HDData::DynamicCapsuleCollider::SetRadius(float rad)
{
	_radius = rad;
}

void HDData::DynamicCapsuleCollider::SetHalfHeight(float val)
{
	_halfHeight = val;
}
