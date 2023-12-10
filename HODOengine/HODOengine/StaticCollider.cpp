#include "StaticCollider.h"

hodoData::StaticCollider::StaticCollider()
	: _isCollided(false)
{

}

void hodoData::StaticCollider::UpdateToPhysics()
{
	// physics 씬을 가져와서 콜라이더의 transform 값을 보낸다
}

void hodoData::StaticCollider::Collided()
{
	_isCollided = true;
}

bool hodoData::StaticCollider::GetIsCollided()
{
	return _isCollided;
}
