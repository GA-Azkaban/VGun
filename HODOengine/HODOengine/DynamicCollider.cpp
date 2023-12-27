#include "DynamicCollider.h"
#include "Transform.h"

#include "../Include/physx/PxPhysics.h"
#include "../Include/physx/PxPhysicsAPI.h"

HDData::DynamicCollider::DynamicCollider()
{

}

void HDData::DynamicCollider::SetPhysXRigid(physx::PxRigidDynamic* rigid)
{
	_physXRigid = rigid;
}

void HDData::DynamicCollider::Move(HDMath::HDFLOAT3 moveStep)
{
	//_physXRigid->wakeUp();
	_physXRigid->addForce(physx::PxVec3(moveStep.x, moveStep.y, moveStep.z), physx::PxForceMode::eVELOCITY_CHANGE);
}

void HDData::DynamicCollider::Jump()
{
	_physXRigid->addForce(physx::PxVec3(0.0f, 5.0f, 0.0f), physx::PxForceMode::eIMPULSE);
}

void HDData::DynamicCollider::Sleep()
{
	//_physXRigid->putToSleep();
}

void HDData::DynamicCollider::UpdateToPhysics()
{

}

void HDData::DynamicCollider::UpdateFromPhysics(HDMath::HDFLOAT3 pos, HDMath::HDQuaternion quat)
{
	this->GetTransform()->SetWorldPosition(pos.x, pos.y, pos.z);
	this->GetTransform()->SetWorldRotation(quat);

}

void HDData::DynamicCollider::Collide()
{
	_isCollided = true;
}

bool HDData::DynamicCollider::GetIsCollided()
{
	return _isCollided;
}