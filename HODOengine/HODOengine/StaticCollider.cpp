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

void HDData::StaticCollider::SetColliderRotation(Quaternion rot)
{
	physx::PxTransform currentTransform = _physXStatic->getGlobalPose();
	_physXStatic->setGlobalPose(physx::PxTransform(currentTransform.p, physx::PxQuat(rot.x, rot.y, rot.z, rot.w)));

	for (auto& child : _childColliders)
	{
		dynamic_cast<HDData::StaticCollider*>(child)->SetColliderRotation(rot);
	}

}

void HDData::StaticCollider::Move(Vector3 moveStep, float speed)
{
	physx::PxTransform playerPos = _physXStatic->getGlobalPose();

	playerPos.p.x += moveStep.x;
	playerPos.p.z += moveStep.z;

	_physXStatic->setGlobalPose(playerPos);
	//_physXRigid->addForce(physx::PxVec3(moveStep.x, moveStep.y, moveStep.z) * speed, physx::PxForceMode::eVELOCITY_CHANGE);
	for (auto& child : _childColliders)
	{
		dynamic_cast<HDData::StaticCollider*>(child)->Move(moveStep, speed);
	}
}

void HDData::StaticCollider::Update()
{
}

void HDData::StaticCollider::SetPhysXRigid(physx::PxRigidStatic* rigid)
{
	_physXStatic = rigid;
}
