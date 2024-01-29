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


	public:
		void SetPhysXRigid(physx::PxRigidDynamic* rigid);
		void Move(Vector3 moveStep, float speed);
		void Rotate(float rotationAmount);
		void Jump();
		void Sleep();
		void Stop();
		void AddForce(Vector3 direction, float force = 1.0f);
		void AdjustVelocity(float ratio);

	public:
		// PhysX 씬과 데이터를 주고받는 함수
		void UpdateToPhysics() override;
		void UpdateFromPhysics(Vector3 pos, Quaternion quat);

	public:
		virtual void DrawDebug() override;

	public:
		void Collide();
		bool GetIsCollided();

	private:
		bool _isJumping;
		bool _isCollided;
		bool _isOnGround;

	private:
		physx::PxRigidDynamic* _physXRigid;
	};
}

