#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#pragma once

#include "Singleton.h"

#include "../Include/physx/PxPhysics.h"
#include "../Include/physx/PxPhysicsAPI.h"

namespace HDData
{
	class GameObject;
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
		void Update();
		void Finalize();

	public:
		void CreatePhysXScene();

		void TempMove();

	public:
		physx::PxScene* GetScene() const;

	public:
		void CreateRigidBodies();
		void CreateStaticPlaneCollider(HDData::GameObject* object);
		void CreateStaticBoxCollider(HDData::GameObject* object);

	private:
		physx::PxDefaultAllocator		_allocator;
		physx::PxDefaultErrorCallback	_errorCallback;
		physx::PxFoundation* _foundation;
		physx::PxPhysics* _physics;
		physx::PxDefaultCpuDispatcher* _dispatcher;
		physx::PxScene* _pxScene;
		physx::PxMaterial* _material;
		physx::PxPvd* _pvd;

		physx::PxRigidDynamic* _dynamic;
	};
}
