#include "StaticCollider.h"

hodoData::StaticCollider::StaticCollider()
	: _isCollided(false)
{

}

void hodoData::StaticCollider::UpdateToPhysics()
{
	// physics ���� �����ͼ� �ݶ��̴��� transform ���� ������
}

void hodoData::StaticCollider::Collided()
{
	_isCollided = true;
}

bool hodoData::StaticCollider::GetIsCollided()
{
	return _isCollided;
}
