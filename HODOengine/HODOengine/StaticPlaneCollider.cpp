#include "StaticPlaneCollider.h"

HDData::StaticPlaneCollider::StaticPlaneCollider()
{

}

void HDData::StaticPlaneCollider::Update()
{

}

HDMath::HDFLOAT3 HDData::StaticPlaneCollider::GetNormalVector() const
{
	//HDMaths::HDFLOAT3 axis = GetGameObject().GetTransform()->GetRight();
	return {};
}

float HDData::StaticPlaneCollider::GetDistance() const
{
	return {};
}
