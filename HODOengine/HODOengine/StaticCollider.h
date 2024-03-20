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
		virtual void DrawDebug() override;

		void SetPhysXRigid(physx::PxRigidStatic* rigid);

	public:
		void Collided();
		bool GetIsCollided();

	private:
		bool _isCollided;

	private:
		physx::PxRigidStatic* _physXStatic;
	};
}

