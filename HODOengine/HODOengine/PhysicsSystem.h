#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#pragma once

#include <vector>

#include "Singleton.h"
#include "HODO_API.h"

#include "../Include/physx/PxPhysics.h"
#include "../Include/physx/PxPhysicsAPI.h"

namespace HDData
{
	class GameObject;
	class Collider;
}

namespace HDEngine
{
	class PhysicsSystem : public Singleton<PhysicsSystem>
	{
		friend Singleton;

	private:
		PhysicsSystem() = default;

	public:
		void Initialize();
		void PreparePhysics();
		void Update();
		void Finalize();

	public:
		void CreatePhysXScene();

	public:
		physx::PxScene* GetScene() const;

	public:
		void CreateRigidBodies();
		void CreateStaticPlaneCollider(HDData::GameObject* object);
		void CreateStaticBoxCollider(HDData::GameObject* object);
		void CreateStaticBoxCollider(float width, float height, float depth);
		void CreateDynamicBoxCollider(HDData::GameObject* object);
		void CreateDynamicCapsuleCollider(HDData::GameObject* object);
		void CreateDynamicSphereCollider(HDData::GameObject* object);

	public:
		//HDData::Collider* RayCast(HDMath::HDFLOAT3 origin, HDMath::HDFLOAT3 direction, float length, int* type);
		HDData::Collider* RayCast(float originX, float originY, float originZ, float directionX, float directionY, float directionZ, float length, int* type);

	private:
		physx::PxDefaultAllocator		_allocator;
		physx::PxDefaultErrorCallback	_errorCallback;
		physx::PxFoundation* _foundation;
		physx::PxPhysics* _physics;
		physx::PxDefaultCpuDispatcher* _dispatcher;
		physx::PxScene* _pxScene;
		physx::PxMaterial* _material;
		physx::PxPvd* _pvd;

	private:
		std::vector<physx::PxRigidDynamic*> _rigidDynamics;
		int _directionX;
		int _directionZ;
	};
}
