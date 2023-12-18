#include "StaticPlaneCollider.h"

HDData::StaticPlaneCollider::StaticPlaneCollider()
{

}

void HDData::StaticPlaneCollider::Update()
{

}

HDMaths::HDFLOAT3 hodoData::StaticPlaneCollider::GetNormalVector() const
{
	//HDMaths::HDFLOAT3 axis = GetGameObject().GetTransform()->GetRight();
	return {};
}

float hodoData::StaticPlaneCollider::GetDistance() const
{
	return {};
}
