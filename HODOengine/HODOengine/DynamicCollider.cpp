#include "DynamicCollider.h"
#include "Transform.h"

#include "../include/physX/PxPhysics.h"
#include "../include/physX/PxPhysicsAPI.h"

HDData::DynamicCollider::DynamicCollider()
{

}

void HDData::DynamicCollider::SetPhysXRigid(physx::PxRigidDynamic* rigid)
{
	_physXRigid = rigid;
}

void HDData::DynamicCollider::LockPlayerRotation()
{
	_physXRigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, true);
	_physXRigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, true);
	_physXRigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, true);
	for (auto& child : _childColliders)
	{
		dynamic_cast<HDData::DynamicCollider*>(child)->LockPlayerRotation();
	}
}

void HDData::DynamicCollider::SetParentCollider(HDData::DynamicCollider* col)
{
	_parentCollider = col;
	col->SetChildCollider(this);
}

void HDData::DynamicCollider::SetChildCollider(HDData::DynamicCollider* childCol)
{
	_childColliders.push_back(childCol);
}

void HDData::DynamicCollider::Move(Vector3 moveStep, float speed)
{
	//_physXRigid->wakeUp();
	physx::PxTransform playerPos = _physXRigid->getGlobalPose();

	playerPos.p.x += moveStep.x;
	playerPos.p.z += moveStep.z;

	_physXRigid->setGlobalPose(playerPos);
	//_physXRigid->addForce(physx::PxVec3(moveStep.x, moveStep.y, moveStep.z) * speed, physx::PxForceMode::eVELOCITY_CHANGE);
	for (auto& child : _childColliders)
	{
		dynamic_cast<HDData::DynamicCollider*>(child)->LockPlayerRotation();
	}
}

void HDData::DynamicCollider::RotateY(float rotationAmount)
{
	//_physXRigid->addTorque(physx::PxVec3(rotQuat.x, rotQuat.y, rotQuat.z), physx::PxForceMode::eVELOCITY_CHANGE);
	
	//_physXRigid->setAngularVelocity(physx::PxVec3(rotQuat.x, rotQuat.y, rotQuat.z));

	physx::PxTransform currentTransform = _physXRigid->getGlobalPose();
	physx::PxQuat newRot = currentTransform.q * physx::PxQuat(physx::PxPi * rotationAmount, physx::PxVec3(0.0f, 1.0f, 0.0f));
	_physXRigid->setGlobalPose(physx::PxTransform(currentTransform.p, newRot));

	for (auto& child : _childColliders)
	{
		dynamic_cast<HDData::DynamicCollider*>(child)->RotateY(rotationAmount);
	}
}

void HDData::DynamicCollider::RotateOnAxis(float rotationAmount, Vector3 axis)
{
	physx::PxTransform currentTransform = _physXRigid->getGlobalPose();
	physx::PxVec3 axisX(axis.x, axis.y, axis.z);
	physx::PxQuat newRot = currentTransform.q * physx::PxQuat(physx::PxPi * rotationAmount, axisX);

	_physXRigid->setGlobalPose(physx::PxTransform(currentTransform.p, newRot));
}

void HDData::DynamicCollider::SetColliderRotation(Quaternion rot)
{
	physx::PxTransform currentTransform = _physXRigid->getGlobalPose();
	_physXRigid->setGlobalPose(physx::PxTransform(currentTransform.p, physx::PxQuat(rot.x, rot.y, rot.z, rot.w)));

	for (auto& child : _childColliders)
	{
		dynamic_cast<HDData::DynamicCollider*>(child)->SetColliderRotation(rot);
	}
}

void HDData::DynamicCollider::Jump()
{
	_physXRigid->addForce(physx::PxVec3(0.0f, 16.0f, 0.0f), physx::PxForceMode::eIMPULSE);
}

void HDData::DynamicCollider::Sleep()
{
	//_physXRigid->putToSleep();
}

void HDData::DynamicCollider::Stop()
{
	_physXRigid->setLinearVelocity(physx::PxVec3(0.f, _physXRigid->getLinearVelocity().y, 0.f));
}

void HDData::DynamicCollider::AddForce(Vector3 direction, float force)
{
	direction.Normalize();
	_physXRigid->addForce(physx::PxVec3(direction.x, direction.y, direction.z) * force, physx::PxForceMode::eIMPULSE);
}

void HDData::DynamicCollider::AdjustVelocity(float ratio)
{
	_physXRigid->setLinearVelocity(_physXRigid->getLinearVelocity() * ratio);
}

void HDData::DynamicCollider::UpdateToPhysics()
{

}

void HDData::DynamicCollider::UpdateFromPhysics(Vector3 pos, Quaternion quat)
{
	GetTransform()->SetPosition(pos.x, pos.y, pos.z);
	GetTransform()->SetRotation(quat);
}

void HDData::DynamicCollider::DrawDebug()
{

}

void HDData::DynamicCollider::Collide()
{
	_isCollided = true;
}

bool HDData::DynamicCollider::GetIsCollided()
{
	return _isCollided;
}

physx::PxRigidDynamic* HDData::DynamicCollider::GetPhysXRigid() const
{
	return _physXRigid;
}

HDData::Collider* HDData::DynamicCollider::GetParentCollider() const
{
	return _parentCollider;
}

std::vector<HDData::Collider*> HDData::DynamicCollider::GetChildColliderVec() const
{
	return _childColliders;
}
