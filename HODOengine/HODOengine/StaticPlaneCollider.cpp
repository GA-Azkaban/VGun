#include "StaticPlaneCollider.h"

HDData::StaticPlaneCollider::StaticPlaneCollider()
	: _normalVector(0.f, 1.f, 0.f), _distance(0.f)
{

}

HDMath::HDFLOAT3 HDData::StaticPlaneCollider::GetNormalVector() const
{
	//HDMaths::HDFLOAT3 axis = GetGameObject().GetTransform()->GetRight();
	return _normalVector;
}

float HDData::StaticPlaneCollider::GetDistance() const
{
	return _distance;
}
