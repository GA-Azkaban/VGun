#include "StaticPlaneCollider.h"
#include "GameObject.h"

HDData::StaticPlaneCollider::StaticPlaneCollider()
{

}

void HDData::StaticPlaneCollider::Update()
{

}

HDMath::HDFLOAT3 HDData::StaticPlaneCollider::GetNormalVector() const
{
	HDMath::HDFLOAT3 axis = GetGameObject()->GetTransform()->GetRight();
	return axis.Normalize(axis);
}

float HDData::StaticPlaneCollider::GetDistance() const
{
	return HDMath::HDFLOAT3Length(GetGameObject()->GetTransform()->GetLocalPosition());
}
