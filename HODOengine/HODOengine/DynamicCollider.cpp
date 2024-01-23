﻿#include "DynamicCollider.h"
#include "Transform.h"

#include "../include/physX/PxPhysics.h"
#include "../include/physX/PxPhysicsAPI.h"

namespace HDData
{
	DynamicCollider::DynamicCollider()
	{

	}

	void DynamicCollider::SetPhysXRigid(physx::PxRigidDynamic* rigid)
	{
		_physXRigid = rigid;
	}

	void DynamicCollider::Move(Vector3 moveStep)
	{
		//_physXRigid->wakeUp();
		_physXRigid->addForce(physx::PxVec3(moveStep.x, moveStep.y, moveStep.z), physx::PxForceMode::eVELOCITY_CHANGE);
	}

	void DynamicCollider::Rotate(float rotationAmount)
	{
		//_physXRigid->addTorque(physx::PxVec3(rotQuat.x, rotQuat.y, rotQuat.z), physx::PxForceMode::eVELOCITY_CHANGE);
	
		//_physXRigid->setAngularVelocity(physx::PxVec3(rotQuat.x, rotQuat.y, rotQuat.z));

		physx::PxTransform currentTransform = _physXRigid->getGlobalPose();
		physx::PxQuat newRot = currentTransform.q * physx::PxQuat(physx::PxPi * rotationAmount, physx::PxVec3(0.0f, 1.0f, 0.0f));
		_physXRigid->setGlobalPose(physx::PxTransform(currentTransform.p, newRot));
	}

	void DynamicCollider::Jump()
	{
		_physXRigid->addForce(physx::PxVec3(0.0f, 5.0f, 0.0f), physx::PxForceMode::eIMPULSE);
	}

	void DynamicCollider::Sleep()
	{
		//_physXRigid->putToSleep();
	}

	void DynamicCollider::Stop()
	{
		_physXRigid->setLinearVelocity(physx::PxVec3(0.f, _physXRigid->getLinearVelocity().y, 0.f));
	}

	void DynamicCollider::UpdateToPhysics()
	{

	}

	void DynamicCollider::UpdateFromPhysics(Vector3 pos, Quaternion quat)
	{
		this->GetTransform()->SetPosition(pos.x, pos.y, pos.z);
		this->GetTransform()->SetRotation(quat);

	}

	void DynamicCollider::DrawDebug()
	{

	}

	void DynamicCollider::Collide()
	{
		_isCollided = true;
	}

	bool DynamicCollider::GetIsCollided()
	{
		return _isCollided;
	}

}
