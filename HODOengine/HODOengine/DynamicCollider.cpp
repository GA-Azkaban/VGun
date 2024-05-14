#include "DynamicCollider.h"
#include "Transform.h"

#include "../include/physX/PxPhysics.h"
#include "../include/physX/PxPhysicsAPI.h"

HDData::DynamicCollider::DynamicCollider()
	: _prevPos(Vector3::Zero), _prevRot()
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

void HDData::DynamicCollider::GetReady()
{
	_prevPos = GetTransform()->GetNodePosition();
}

void HDData::DynamicCollider::Move(Vector3 moveStep, float speed /*= 1.0f*/)
{
	//_physXRigid->wakeUp();
	physx::PxTransform playerPos = _physXRigid->getGlobalPose();

	playerPos.p.x += moveStep.x;
	playerPos.p.z += moveStep.z;

	_physXRigid->setGlobalPose(playerPos);
	//_physXRigid->addForce(physx::PxVec3(moveStep.x, moveStep.y, moveStep.z) * speed, physx::PxForceMode::eVELOCITY_CHANGE);


	for (auto& child : _childColliders)
	{
		dynamic_cast<HDData::DynamicCollider*>(child)->Move(moveStep, speed);
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
	_physXRigid->addForce(physx::PxVec3(0.0f, 320.0f, 0.0f), physx::PxForceMode::eIMPULSE);
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

void HDData::DynamicCollider::SetPose(Vector3 pos)
{
	_physXRigid->setGlobalPose(physx::PxTransform(pos.x, pos.y, pos.z));
}

void HDData::DynamicCollider::ApplyNodeInfo()
{
	Transform* transform = GetTransform();

	Vector3 pos = transform->GetNodePosition();
	Quaternion rot = transform->GetNodeRotation();

	physx::PxTransform playerPos = _physXRigid->getGlobalPose();

	Vector3 posOffset = GetPositionOffset();

	//playerPos.p.x += (pos.x - _prevPos.x);
	playerPos.p.x = pos.x;
	playerPos.p.y = pos.y;
	playerPos.p.z = pos.z;
	playerPos.q.x = rot.x;
	playerPos.q.y = rot.y;
	playerPos.q.z = rot.z;
	playerPos.q.w = rot.w;
	//transform->SetLocalPosition(pos);
	//transform->SetLocalRotation(rot);

	_prevPos = pos;

	//_physXRigid->setGlobalPose(playerPos);
	_physXRigid->setGlobalPose(physx::PxTransform(pos.x, pos.y, pos.z, physx::PxQuat(rot.x, rot.y, rot.z, rot.w)));
	//_physXRigid->setCMassLocalPose(physx::PxTransform(pos.x, pos.y, pos.z, physx::PxQuat(rot.x, rot.y, rot.z, rot.w)));

	for (auto& child : _childColliders)
	{
		dynamic_cast<HDData::DynamicCollider*>(child)->ApplyNodeInfo();
	}
}

void HDData::DynamicCollider::EveryMove(Vector3 moveStep, Matrix parentTM = Matrix::Identity)
{
	//physx::PxTransform playerPos = _physXRigid->getGlobalPose();
	//playerPos.p.x = playerPos.p.x + moveStep.x + GetPositionOffset().x;
	//playerPos.p.y = playerPos.p.y + moveStep.y + GetPositionOffset().y;
	//playerPos.p.z = playerPos.p.x + moveStep.z + GetPositionOffset().z;

	//Transform* transform = GetTransform();
	//Vector3 nodePos = transform->GetNodePosition();
	//Quaternion nodeRot = transform->GetNodeRotation();
	//Vector3 nodeScale = transform->GetNodeScale();

	//physx::PxQuat pq = playerPos.q;
	//Quaternion plRot = Quaternion::Concatenate(Quaternion(pq.x, pq.y, pq.z, pq.w), nodeRot);
	//pq.x = plRot.x;
	//pq.y = plRot.y;
	//pq.z = plRot.z;
	//pq.w = plRot.w;


	//Matrix nodeTM = Matrix::Identity;
	//nodeTM *= Matrix::CreateScale(nodeScale);
	//nodeTM *= Matrix::CreateFromQuaternion(nodeRot);
	//nodeTM *= Matrix::CreateTranslation(nodePos);

	//Matrix playerTM = Matrix::Identity;
	//playerTM *= Matrix::CreateFromQuaternion(plRot);
	//playerTM *= Matrix::CreateTranslation(Vector3(playerPos.p.x, playerPos.p.y, playerPos.p.z));

	//Matrix result = DirectX::XMMatrixMultiply(nodeTM, playerTM);
	//result = DirectX::XMMatrixMultiply(result, parentTM);

	//playerPos.p.x = result._41;
	//playerPos.p.y = result._42;
	//playerPos.p.z = result._43;

	//DirectX::XMVECTOR quatVec = XMQuaternionRotationMatrix(result);
	//Vector4 quat;
	//XMStoreFloat4(&quat, quatVec);

	//playerPos.q.x = quat.x;
	//playerPos.q.y = quat.y;
	//playerPos.q.z = quat.z;
	//playerPos.q.w = quat.w;

	//_physXRigid->setGlobalPose(playerPos);

	//for (auto& child : _childColliders)
	//{
	//	dynamic_cast<HDData::DynamicCollider*>(child)->EveryMove(moveStep, result);
	//}

	/*
	Matrix result;

	if (_parentCollider == nullptr)
	{
		Vector3 nowPos = _prevPos + moveStep;
		_prevPos = nowPos;
		nowPos += GetPositionOffset();

		Transform* transform = GetTransform();
		Vector3 nodePos = transform->GetNodePosition();
		Quaternion nodeRot = transform->GetNodeRotation();
		Vector3 nodeScale = transform->GetNodeScale();


		Matrix nodeTM = Matrix::Identity;
		//nodeTM *= Matrix::CreateScale(nodeScale);
		nodeTM *= Matrix::CreateFromQuaternion(nodeRot);
		nodeTM *= Matrix::CreateTranslation(nodePos);

		Matrix playerTM = Matrix::Identity;
		playerTM *= Matrix::CreateFromQuaternion(_prevRot);
		playerTM *= Matrix::CreateTranslation(nowPos);

		result = DirectX::XMMatrixMultiply(playerTM, nodeTM);
		result = DirectX::XMMatrixMultiply(parentTM, result);
	}
	else
	{
		Vector3 nowPos = GetPositionOffset();

		Transform* transform = GetTransform();
		Vector3 nodePos = transform->GetNodePosition();
		Quaternion nodeRot = transform->GetNodeRotation();
		Vector3 nodeScale = transform->GetNodeScale();


		Matrix nodeTM = Matrix::Identity;
		//nodeTM *= Matrix::CreateScale(nodeScale);
		nodeTM *= Matrix::CreateFromQuaternion(nodeRot);
		nodeTM *= Matrix::CreateTranslation(nodePos);

		Matrix playerTM = Matrix::CreateTranslation(nowPos);

		result = DirectX::XMMatrixMultiply(playerTM, nodeTM);
		result = DirectX::XMMatrixMultiply(parentTM, result);
	}


	physx::PxTransform playerPos;

	playerPos.p.x = result._41;
	playerPos.p.y = result._42;
	playerPos.p.z = result._43;

	Matrix rotMat = Matrix(	result.m[0][0], result.m[0][1], result.m[0][2], 0.0f,
							result.m[1][0], result.m[1][1], result.m[1][2], 0.0f,
							result.m[2][0], result.m[2][1], result.m[2][2], 0.0f,
							0.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMVECTOR quatVec = XMQuaternionRotationMatrix(rotMat);
	Vector4 quat;
	XMStoreFloat4(&quat, quatVec);

	playerPos.q.x = quat.x;
	playerPos.q.y = quat.y;
	playerPos.q.z = quat.z;
	playerPos.q.w = quat.w;

	_physXRigid->setGlobalPose(playerPos);

	for (auto& child : _childColliders)
	{
		dynamic_cast<HDData::DynamicCollider*>(child)->EveryMove(moveStep, result);
	}
	*/


	Vector3 nowPos = _prevPos + moveStep;
	_prevPos = nowPos;
	nowPos += GetPositionOffsetTotal();
	physx::PxTransform playerPos = _physXRigid->getGlobalPose();
	playerPos.p.x = nowPos.x;
	playerPos.p.y = nowPos.y;
	playerPos.p.z = nowPos.z;

	Vector3 nodePos = GetTransform()->GetNodePosition();
	Quaternion nodeRot = GetTransform()->GetNodeRotation();
	Matrix nodeTM = Matrix::Identity;
	nodeTM *= Matrix::CreateFromQuaternion(nodeRot);
	nodeTM *= Matrix::CreateTranslation(nodePos);
	nodeTM *= parentTM;

	Matrix rotMat = Matrix(nodeTM.m[0][0], nodeTM.m[0][1], nodeTM.m[0][2], 0.0f,
		nodeTM.m[1][0], nodeTM.m[1][1], nodeTM.m[1][2], 0.0f,
		nodeTM.m[2][0], nodeTM.m[2][1], nodeTM.m[2][2], 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMVECTOR quatVec = XMQuaternionRotationMatrix(rotMat);
	Vector4 quat;
	XMStoreFloat4(&quat, quatVec);

	playerPos.p.x += nodeTM._41;
	playerPos.p.y += nodeTM._42;
	playerPos.p.z += nodeTM._43;
	playerPos.q.x = quat.x;
	playerPos.q.y = quat.y;
	playerPos.q.z = quat.z;
	playerPos.q.w = quat.w;

	_physXRigid->setGlobalPose(playerPos);

	for (auto& child : _childColliders)
	{
		dynamic_cast<HDData::DynamicCollider*>(child)->EveryMove(moveStep, nodeTM);
	}

}

void HDData::DynamicCollider::UpdateToPhysics()
{
	Transform* transform = GetTransform();
	Vector3 pos = transform->GetPosition();
	Quaternion rot = transform->GetRotation();

	_physXRigid->setGlobalPose(physx::PxTransform(pos.x, pos.y, pos.z, physx::PxQuat(rot.x, rot.y, rot.z, rot.w)));
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
