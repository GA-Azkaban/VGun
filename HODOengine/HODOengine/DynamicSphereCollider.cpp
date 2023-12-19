#include "DynamicSphereCollider.h"
#include "GameObject.h"

HDData::DynamicSphereCollider::DynamicSphereCollider()
{

}

float HDData::DynamicSphereCollider::GetWidth() const
{
	return _radius * 2 * _scaleOffset.x * GetGameObject()->GetTransform()->GetWorldScale().x;
}

float HDData::DynamicSphereCollider::GetHeight() const
{
	return _radius * 2 * _scaleOffset.y * GetGameObject()->GetTransform()->GetWorldScale().y;
}

float HDData::DynamicSphereCollider::GetDepth() const
{
	return _radius * 2 * _scaleOffset.z * GetGameObject()->GetTransform()->GetWorldScale().z;
}

float HDData::DynamicSphereCollider::GetRadius() const
{
	return _radius;
}

void HDData::DynamicSphereCollider::SetRadius(float radius)
{
	_radius = radius;
}
