#include "StaticCollider.h"

HDData::StaticCollider::StaticCollider()
	: _isCollided(false)
{

}

void HDData::StaticCollider::UpdateToPhysics()
{
	// physics 씬을 가져와서 콜라이더의 transform 값을 보낸다
}

void HDData::StaticCollider::Collided()
{
	_isCollided = true;
}

bool HDData::StaticCollider::GetIsCollided()
{
	return _isCollided;
}

void HDData::StaticCollider::DrawDebug()
{
}
