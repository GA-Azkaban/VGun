#pragma once
#include "dllExporter.h"
#include "Collider.h"

namespace physx
{
	class PxRigidStatic;
}

namespace HDData
{
	class HODO_API StaticCollider : public Collider
	{
	public:
		StaticCollider();

	public:
		void UpdateToPhysics() override;
		void UpdateFromPhysics(Vector3 pos, Quaternion quat);
		virtual void Update() override;

		void SetPhysXRigid(physx::PxRigidStatic* rigid);

	public:
		void Collided();
		bool GetIsCollided();

	public:
		void SetGlobalPosition(Vector3 pos);
		void SetColliderRotation(Quaternion rot);
		void Move(Vector3 moveStep, float speed);

	private:
		bool _isCollided;
		physx::PxRigidStatic* _physXStatic;
	};
}

