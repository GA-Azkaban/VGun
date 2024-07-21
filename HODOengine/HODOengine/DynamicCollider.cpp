#include "DynamicCollider.h"
#include "Transform.h"



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

void HDData::DynamicCollider::SetPhysScene(physx::PxScene* scene)
{
	_physScene = scene;
}

void HDData::DynamicCollider::LockPlayerRotation(bool isLock)
{
	_physXRigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, isLock);
	_physXRigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, isLock);
	_physXRigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, isLock);
}

//void HDData::DynamicCollider::SetParentCollider(HDData::DynamicCollider* col)
//{
//	_parentCollider = col;
//	col->SetChildCollider(this);
//}
//
//void HDData::DynamicCollider::SetChildCollider(HDData::DynamicCollider* childCol)
//{
//	_childColliders.push_back(childCol);
//}

void HDData::DynamicCollider::SetPlayerShapes(physx::PxShape* stand, physx::PxShape* sit)
{
	_standingShape = stand;
	_sittingShape = sit;
}

bool HDData::DynamicCollider::GetFreezeRotation()
{
	return _freezeRotation;
}

void HDData::DynamicCollider::SetVelocity(Vector3 moveStep, float speed)
{
	physx::PxVec3 velo = _physXRigid->getLinearVelocity();
	_physXRigid->setLinearVelocity(physx::PxVec3(moveStep.x * speed * 20.0f, velo.y, moveStep.z * speed * 20.0f));
}

void HDData::DynamicCollider::SetLinearVelocity(Vector3 velocity)
{
	_physXRigid->setLinearVelocity(physx::PxVec3(velocity.x, velocity.y, velocity.z));
}

void HDData::DynamicCollider::Move(Vector3 moveStep, float speed, float deltaTime)
{
	//physx::PxTransform playerPos = _physXRigid->getGlobalPose();
	//playerPos.p.x += moveStep.x * speed * deltaTime;
	//playerPos.p.z += moveStep.z * speed * deltaTime;
	//_physXRigid->setGlobalPose(playerPos);

	physx::PxVec3 velo = _physXRigid->getLinearVelocity();
#ifdef _DEBUG
	velo.x = moveStep.x * speed * 2;
	velo.z = moveStep.z * speed * 2;
#else
	velo.x = moveStep.x * speed;
	velo.z = moveStep.z * speed;
#endif

	_physXRigid->setLinearVelocity(velo);
	//_physXRigid->setForceAndTorque(physx::PxVec3(moveStep.x * speed * 300.0f, 0.0f, moveStep.z * speed * 300.0f), physx::PxVec3());

	for (auto& child : _childColliders)
	{
		//dynamic_cast<HDData::DynamicCollider*>(child)->Move(moveStep, speed, deltaTime);
		//physx::PxVec3 velo = _physXRigid->getLinearVelocity();
		//dynamic_cast<HDData::DynamicCollider*>(child)->SetLinearVelocity(Vector3(velo.x, velo.y, velo.z));
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
		auto dynamicChild = dynamic_cast<HDData::DynamicCollider*>(child);
		if (dynamicChild != nullptr)
		{
			dynamicChild->SetColliderRotation(rot);
		}
	}
}

void HDData::DynamicCollider::SetColliderPosition(Vector3 pos)
{
	physx::PxTransform currentTransform = _physXRigid->getGlobalPose();
	
	//Vector3 posDif = {pos.x - currentTransform.p.x, pos.y - currentTransform.p.y, pos.z - currentTransform.p.z };

	_physXRigid->setGlobalPose(physx::PxTransform(physx::PxVec3(pos.x, pos.y, pos.z), currentTransform.q));

	for (auto& child : _childColliders)
	{
		auto dynamicChild = dynamic_cast<HDData::DynamicCollider*>(child);
		if (dynamicChild != nullptr)
		{
			//dynamicChild->Move(posDif, 1.0f, 1.0f);
			// 손자뻘이 없다고 가정하고 작성
			Vector3 localPos = dynamicChild->GetTransform()->GetLocalPosition();
			dynamicChild->SetColliderPosition(Vector3(pos.x + localPos.x, pos.y + localPos.y, pos.z + localPos.z));
		}
	}
}

void HDData::DynamicCollider::Jump(Vector3 direction)
{
	//_physXRigid->addForce(physx::PxVec3(direction.x * 0.16f, 1.2f, direction.z * 0.16f) * 100.0f, physx::PxForceMode::eIMPULSE);
	//_physXRigid->addForce(physx::PxVec3(0.0f, 1.2f, 0.0f) * 120.0f, physx::PxForceMode::eIMPULSE);
#ifdef _DEBUG
	_physXRigid->addForce(physx::PxVec3(0.0f, 4800.0f, 0.0f), physx::PxForceMode::eFORCE);
#else
	_physXRigid->addForce(physx::PxVec3(0.0f, 3600.0f, 0.0f), physx::PxForceMode::eFORCE);
#endif
}

void HDData::DynamicCollider::Sleep()
{
	//_physXRigid->putToSleep();
}

void HDData::DynamicCollider::Stop()
{
	_physXRigid->setLinearVelocity(physx::PxVec3(0.f, _physXRigid->getLinearVelocity().y, 0.f));
	_physXRigid->clearForce();
	_physXRigid->clearTorque();
}

void HDData::DynamicCollider::AddForce(Vector3 direction, float force /*= 1.0f*/, int forceType /*= 1*/)
{
	direction.Normalize();
	
	if (forceType == 0)
	{
		_physXRigid->addForce(physx::PxVec3(direction.x, direction.y, direction.z) * force, physx::PxForceMode::eFORCE);
	}
	else if (forceType == 1)
	{
		_physXRigid->addForce(physx::PxVec3(direction.x, direction.y, direction.z) * force, physx::PxForceMode::eIMPULSE);
	}
}

void HDData::DynamicCollider::AdjustVelocity(float ratio)
{
	_physXRigid->setLinearVelocity(_physXRigid->getLinearVelocity() * ratio);
}

void HDData::DynamicCollider::ClearVeloY()
{
	physx::PxVec3 velo = _physXRigid->getLinearVelocity();

	velo.y = 0.0f;

	_physXRigid->clearForce();
	_physXRigid->clearTorque();
	//_physXRigid->setLinearVelocity(velo);
	//_physXRigid->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
}

void HDData::DynamicCollider::ClearForceXYZ()
{
	if (_physXRigid == nullptr)
	{
		return;
	}

	_physXRigid->clearForce();
	_physXRigid->clearTorque();
	_physXRigid->setLinearVelocity(physx::PxVec3(0.0f));
	_physXRigid->setAngularVelocity(physx::PxVec3(0.0f));

	for (auto& child : _childColliders)
	{
		auto dynamicChild = dynamic_cast<HDData::DynamicCollider*>(child);
		if (dynamicChild != nullptr)
		{
			dynamicChild->ClearForceXYZ();
		}
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
	_physXRigid->getScene()->removeActor(*_physXRigid);
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
	GetTransform()->SetPositionFromPhysics(pos.x, pos.y, pos.z);
	GetTransform()->SetRotationFromPhysics(quat);
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

std::vector<HDData::Collider*> HDData::DynamicCollider::GetChildColliderVec() const
{
	return _childColliders;
}

void HDData::DynamicCollider::OnEnable()
{
	//_physXRigid->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, false);
	_physScene->addActor(*_physXRigid);

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
	//_physXRigid->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, true);
	_physScene->removeActor(*_physXRigid);

	for (auto& child : _childColliders)
	{
		if (dynamic_cast<HDData::DynamicCollider*>(child) != nullptr)
		{
			dynamic_cast<HDData::DynamicCollider*>(child)->OnDisable();
		}
	}
}

TransformInfo HDData::DynamicCollider::GetPrevTransform() const
{
	return _prevTransform;
}

TransformInfo HDData::DynamicCollider::GetCurTransform() const
{
	return _currentTransform;
}

void HDData::DynamicCollider::SetPrevTransform(TransformInfo info)
{
	_prevTransform.pos = info.pos;
	_prevTransform.rot = info.rot;
}

void HDData::DynamicCollider::SetCurTransform(TransformInfo info)
{
	_currentTransform.pos = info.pos;
	_currentTransform.rot = info.rot;
}
