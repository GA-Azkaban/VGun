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
		void Move(HDMath::HDFLOAT3 moveStep);
		void Jump();
		void Sleep();

	public:
		// PhysX 씬과 데이터를 주고받는 함수
		void UpdateToPhysics() override;
		void UpdateFromPhysics(HDMath::HDFLOAT3 pos, HDMath::HDQuaternion quat);

	public:
		void Collide();
		bool GetIsCollided();

	private:
		bool _isJumping;
		bool _isCollided;

	private:
		physx::PxRigidDynamic* _physXRigid;
	};
}

