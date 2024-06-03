#include "StaticCollider.h"
#include "Transform.h"

#include "../include/physX/PxPhysics.h"
#include "../include/physX/PxPhysicsAPI.h"

HDData::StaticCollider::StaticCollider()
	: _isCollided(false)
{

}

void HDData::StaticCollider::UpdateToPhysics()
{
	// physics 씬을 가져와서 콜라이더의 transform 값을 보낸다
}

void HDData::StaticCollider::UpdateFromPhysics(Vector3 pos, Quaternion quat)
{
	GetTransform()->SetPosition(pos.x, pos.y, pos.z);
	GetTransform()->SetRotation(quat);
}

void HDData::StaticCollider::Collided()
{
	_isCollided = true;
}

bool HDData::StaticCollider::GetIsCollided()
{
	return _isCollided;
}

void HDData::StaticCollider::SetGlobalPosition(Vector3 pos)
{
	physx::PxTransform position = { pos.x, pos.y, pos.z };
	_physXStatic->setGlobalPose(position);
}

void HDData::StaticCollider::Update()
{
}

void HDData::StaticCollider::SetPhysXRigid(physx::PxRigidStatic* rigid)
{
	_physXStatic = rigid;
}
