#include "DynamicCollider.h"

HDData::DynamicCollider::DynamicCollider()
{

}

void HDData::DynamicCollider::UpdateToPhysics()
{

}

void HDData::DynamicCollider::UpdateFromPhysics()
{

}

void HDData::DynamicCollider::Collide()
{
	_isCollided = true;
}

bool HDData::DynamicCollider::GetIsCollided()
{
	return _isCollided;
}