#include "StaticCollider.h"

HDData::StaticCollider::StaticCollider()
	: _isCollided(false)
{

}

void HDData::StaticCollider::UpdateToPhysics()
{
	// physics ���� �����ͼ� �ݶ��̴��� transform ���� ������
}

void HDData::StaticCollider::Collided()
{
	_isCollided = true;
}

bool HDData::StaticCollider::GetIsCollided()
{
	return _isCollided;
}
