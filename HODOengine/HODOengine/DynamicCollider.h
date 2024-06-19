#pragma once
#include "dllExporter.h"
#include "Collider.h"

namespace physx
{
	class PxRigidDynamic;
	class PxShape;
	//class PxVec3;
}

namespace HDData
{
	class HODO_API DynamicCollider : public Collider
	{
	public:
		DynamicCollider();

	public:
		virtual float GetWidth() const abstract;
		virtual float GetHeight() const abstract;
		virtual float GetDepth() const abstract;


	// setting
	public:
		virtual void SetFreezeRotation(bool freezeRotation);
		void SetPhysXRigid(physx::PxRigidDynamic* rigid);
		void LockPlayerRotation(bool isLock);
		void SetParentCollider(HDData::DynamicCollider* col);
		void SetChildCollider(HDData::DynamicCollider* childCol);
		void SetPlayerShapes(physx::PxShape* stand, physx::PxShape* sit);

	// getter
	public:
		virtual bool GetFreezeRotation();

	// Move and Control
	public:
		void Move(Vector3 moveStep, float speed);
		void Rotate(Quaternion rot);
		void RotateY(float rotationAmount);
		void RotateOnAxis(float rotationAmount, Vector3 axis);
		void SetColliderRotation(Quaternion rot);
		void SetColliderPosition(Vector3 pos);
		void Jump(Vector3 direction);
		void Sleep();
		void Stop();
		void AddForce(Vector3 direction, float force = 1.0f);
		void AdjustVelocity(float ratio);
		void ClearVeloY();
		void ClearForceXZ();
		void ResetCollider(eColliderType type, Vector3 widthDepthHeight);
		void EnableCollider();
		void DisableCollider();
		void EnableStanding(bool isStand);

	public:
		// PhysX 씬과 데이터를 주고받는 함수
		void UpdateToPhysics() override;
		void UpdateFromPhysics(Vector3 pos, Quaternion quat);

	public:
		void Collide();
		bool GetIsCollided();
		physx::PxRigidDynamic* GetPhysXRigid() const;
		HDData::Collider* GetParentCollider() const;
		std::vector<HDData::Collider*> GetChildColliderVec()const;

	protected:
		bool _freezeRotation;

	private:
		bool _isCollided;
		bool _isColliderActive;

	private:
		physx::PxRigidDynamic* _physXRigid;
		physx::PxShape* _standingShape;
		physx::PxShape* _sittingShape;
	};
}

