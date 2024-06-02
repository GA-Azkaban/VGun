#pragma once
#include "dllExporter.h"
#include "Collider.h"

namespace physx
{
	class PxRigidDynamic;
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
		void SetPhysXRigid(physx::PxRigidDynamic* rigid);
		void LockPlayerRotation();
		void SetParentCollider(HDData::DynamicCollider* col);
		void SetChildCollider(HDData::DynamicCollider* childCol);

	// Move and Control
	public:
		void Move(Vector3 moveStep, float speed);
		void Rotate(Quaternion rot);
		void RotateY(float rotationAmount);
		void RotateOnAxis(float rotationAmount, Vector3 axis);
		void SetColliderRotation(Quaternion rot);
		void Jump();
		void Sleep();
		void Stop();
		void AddForce(Vector3 direction, float force = 1.0f);
		void AdjustVelocity(float ratio);
		void ClearVeloY();

	public:
		// PhysX 씬과 데이터를 주고받는 함수
		void UpdateToPhysics() override;
		void UpdateFromPhysics(Vector3 pos, Quaternion quat);

	public:
		virtual void Update() override;

	public:
		void Collide();
		bool GetIsCollided();
		physx::PxRigidDynamic* GetPhysXRigid() const;
		HDData::Collider* GetParentCollider() const;
		std::vector<HDData::Collider*> GetChildColliderVec()const;

	private:
		bool _isJumping;
		bool _isCollided;
		bool _isOnGround;

	private:
		physx::PxRigidDynamic* _physXRigid;
	};
}

