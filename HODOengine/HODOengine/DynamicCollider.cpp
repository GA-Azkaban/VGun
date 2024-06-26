﻿#include "DynamicCollider.h"
#include "Transform.h"

#include "../include/physX/PxPhysics.h"
#include "../include/physX/PxPhysicsAPI.h"

HDData::DynamicCollider::DynamicCollider()
	: _physXRigid(nullptr), _freezeRotation(false)
{

}

void HDData::DynamicCollider::SetFreezeRotation(bool freezeRotation)
{
	if (_freezeRotation == freezeRotation)
	{
		return;
	}

	_freezeRotation = freezeRotation;
	if (_physXRigid != nullptr)
	{		
		LockPlayerRotation(freezeRotation);		
	}

	for (auto& child : _childColliders)
	{
		dynamic_cast<HDData::DynamicCollider*>(child)->LockPlayerRotation(freezeRotation);
	}
}

void HDData::DynamicCollider::SetPhysXRigid(physx::PxRigidDynamic* rigid)
{
	_physXRigid = rigid;
}

void HDData::DynamicCollider::LockPlayerRotation(bool isLock)
{
	_physXRigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, isLock);
	_physXRigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, isLock);
	_physXRigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, isLock);
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

void HDData::DynamicCollider::SetPlayerShapes(physx::PxShape* stand, physx::PxShape* sit)
{
	_standingShape = stand;
	_sittingShape = sit;
}

bool HDData::DynamicCollider::GetFreezeRotation()
{
	return _freezeRotation;
}

void HDData::DynamicCollider::Move(Vector3 moveStep, float speed, float deltaTime)
{
	//_physXRigid->wakeUp();
	physx::PxTransform playerPos = _physXRigid->getGlobalPose();

	playerPos.p.x += moveStep.x * speed * deltaTime;
	playerPos.p.z += moveStep.z * speed * deltaTime;

	_physXRigid->setGlobalPose(playerPos);
	//_physXRigid->addForce(physx::PxVec3(moveStep.x, moveStep.y, moveStep.z) * speed, physx::PxForceMode::eVELOCITY_CHANGE);
	for (auto& child : _childColliders)
	{
		dynamic_cast<HDData::DynamicCollider*>(child)->Move(moveStep, speed, deltaTime);
	}
}

void HDData::DynamicCollider::Rotate(Quaternion rot)
{
	physx::PxTransform currentTransform = _physXRigid->getGlobalPose();
	Quaternion result = Quaternion::Concatenate(rot, { currentTransform.q.x, currentTransform.q.y, currentTransform.q.z, currentTransform.q.w });
	_physXRigid->setGlobalPose(physx::PxTransform(currentTransform.p, physx::PxQuat(result.x, result.y, result.z, result.w)));

	for (auto& child : _childColliders)
	{
		dynamic_cast<HDData::DynamicCollider*>(child)->Rotate(rot);
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

void HDData::DynamicCollider::SetColliderPosition(Vector3 pos)
{
	physx::PxTransform currentTransform = _physXRigid->getGlobalPose();
	
	Vector3 posDif = {pos.x - currentTransform.p.x, pos.y - currentTransform.p.y, pos.z - currentTransform.p.z };

	_physXRigid->setGlobalPose(physx::PxTransform(physx::PxVec3(pos.x, pos.y, pos.z), currentTransform.q));

	for (auto& child : _childColliders)
	{
		dynamic_cast<HDData::DynamicCollider*>(child)->Move(posDif, 1.0f, 1.0f);
	}
}

void HDData::DynamicCollider::Jump(Vector3 direction)
{
	_physXRigid->addForce(physx::PxVec3(direction.x * 0.16f, 1.2f, direction.z * 0.16f) * 100.0f, physx::PxForceMode::eIMPULSE);
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

void HDData::DynamicCollider::ClearVeloY()
{
	physx::PxVec3 velo = _physXRigid->getLinearVelocity();
	if (velo.y > 0.0f)
	{
		velo.y = 0.0f;
	}
	_physXRigid->clearForce();
	_physXRigid->setLinearVelocity(velo);
	//_physXRigid->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
}

void HDData::DynamicCollider::ClearForceXZ()
{
	if (_physXRigid == nullptr)
	{
		return;
	}
	physx::PxVec3 velo = _physXRigid->getLinearVelocity();
	velo.x = 0.0f;
	velo.z = 0.0f;
	_physXRigid->clearForce();
	_physXRigid->clearTorque();
	_physXRigid->setLinearVelocity(velo);
	_physXRigid->setAngularVelocity(physx::PxVec3(0.0f));

	for (auto& child : _childColliders)
	{
		dynamic_cast<HDData::DynamicCollider*>(child)->ClearForceXZ();
	}
}

void HDData::DynamicCollider::ResetCollider(eColliderType type, Vector3 widthDepthHeight)
{

}

void HDData::DynamicCollider::EnableCollider()
{
	_physXRigid->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, false);
	_isColliderActive = true;
}

void HDData::DynamicCollider::DisableCollider()
{
	_physXRigid->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, true);
	_isColliderActive = false;
}

void HDData::DynamicCollider::EnableStanding(bool isStand)
{
	if (isStand)
	{
		//_physXRigid->attachShape(*_standingShape);
		_standingShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
		_sittingShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
		_sittingShape->setFlag(physx::PxShapeFlag::eVISUALIZATION, false);
		//_physXRigid->detachShape(*_sittingShape);
	}
	else
	{
		//_physXRigid->attachShape(*_sittingShape);
		_standingShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
		_sittingShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
		_sittingShape->setFlag(physx::PxShapeFlag::eVISUALIZATION, true);
		//_physXRigid->detachShape(*_standingShape);
	}
}

void HDData::DynamicCollider::UpdateToPhysics()
{

}

void HDData::DynamicCollider::UpdateFromPhysics(Vector3 pos, Quaternion quat)
{
	GetTransform()->SetPosition(pos.x, pos.y, pos.z);
	GetTransform()->SetRotation(quat);
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

void HDData::DynamicCollider::OnEnable()
{
	_physXRigid->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, false);

	for (auto& child : _childColliders)
	{
		if (dynamic_cast<HDData::DynamicCollider*>(child) != nullptr)
		{
			dynamic_cast<HDData::DynamicCollider*>(child)->OnEnable();
		}
	}
}

void HDData::DynamicCollider::OnDisable()
{
	_physXRigid->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, true);
	//_physXRigid->getScene()->removeActor(*_physXRigid);
	for (auto& child : _childColliders)
	{
		if (dynamic_cast<HDData::DynamicCollider*>(child) != nullptr)
		{
			dynamic_cast<HDData::DynamicCollider*>(child)->OnDisable();
		}
	}
}
