#include "DynamicSphereCollider.h"
#include "GameObject.h"

HDData::DynamicSphereCollider::DynamicSphereCollider()
	: _radius(1.0f)
{

}

float HDData::DynamicSphereCollider::GetWidth() const
{
	return _radius * 2 * _scaleOffset.x * GetGameObject()->GetTransform()->GetScale().x;
}

float HDData::DynamicSphereCollider::GetHeight() const
{
	return _radius * 2 * _scaleOffset.y * GetGameObject()->GetTransform()->GetScale().y;
}

float HDData::DynamicSphereCollider::GetDepth() const
{
	return _radius * 2 * _scaleOffset.z * GetGameObject()->GetTransform()->GetScale().z;
}

float HDData::DynamicSphereCollider::GetRadius() const
{
	return _radius;
}

void HDData::DynamicSphereCollider::SetRadius(float radius)
{
	_radius = radius;
}
