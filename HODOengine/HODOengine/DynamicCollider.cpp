#include "DynamicCollider.h"
#include "Transform.h"

HDData::DynamicCollider::DynamicCollider()
{

}

void HDData::DynamicCollider::UpdateToPhysics()
{

}

void HDData::DynamicCollider::UpdateFromPhysics(HDMath::HDFLOAT3 pos, HDMath::HDQuaternion quat)
{
	this->GetTransform()->SetWorldPosition(pos.x, pos.y, pos.z);
	this->GetTransform()->SetWorldRotation(quat);

}

void HDData::DynamicCollider::Collide()
{
	_isCollided = true;
}

bool HDData::DynamicCollider::GetIsCollided()
{
	return _isCollided;
}