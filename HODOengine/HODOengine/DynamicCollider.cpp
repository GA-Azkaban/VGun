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

void HDData::DynamicCollider::Stop()
{
	_physXRigid->setLinearVelocity(physx::PxVec3(0.f, _physXRigid->getLinearVelocity().y, 0.f));
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

bool HDData::DynamicCollider::CheckIsOnGround()
{
	HDMath::HDFLOAT3 pos = this->GetTransform()->GetWorldPosition();
	const float delta = 0.2f;
	float x[9] = { -delta, -delta,0, delta,delta,delta,0,-delta,0 };
	float z[9] = { 0,delta,delta,delta,0,-delta,-delta,-delta,0 };

	for (int i = 0; i < 9; ++i)
	{
		HDMath::HDFLOAT4 worldPos = HDMath::HDFLOAT4(pos.x + x[i], pos.y + 0.01f * i, pos.z + z[i], 1.0f);
		HDMath::HDFLOAT4 eachDir = worldPos;
		eachDir.y -= 0.05f;

		physx::PxRaycastBuffer hitTarget;
		bool isHit = _physXRigid->getScene()->raycast({ worldPos.x, worldPos.y, worldPos.z }, { 0.0f,-1.0f,0.0f }, 0.05f, hitTarget);
		//RocketEngine::Collider* temp = RocketEngine::ShootRay({ worldPos.x, worldPos.y, worldPos.z }, { 0.0f,-1.0f,0.0f }, 0.05f, &type);
		//RocketEngine::DrawDebugLine({ worldPos.x,worldPos.y,worldPos.z }, { eachDir.x,eachDir.y,eachDir.z });

		auto playerCollider = static_cast<HDData::Collider*>(hitTarget.block.actor->userData);

		if (isHit && (playerCollider->GetFlag() == 1))
		{

			// 상태 변경 및 착지 Sound.
			if (_isOnGround == false)
			{
				Landing();
				//_playerAudio->Play3DOnce("landing");
			}
			return true;
		}
	}
	return false;

}

void HDData::DynamicCollider::Landing()
{
	_isOnGround = true;
	_isJumping = false;


}
