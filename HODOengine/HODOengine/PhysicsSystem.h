#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#pragma once

#include "Singleton.h"

#include "../Include/physx/PxPhysics.h"
#include "../Include/physx/PxPhysicsAPI.h"

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

	private:
		physx::PxDefaultAllocator		_allocator;
		physx::PxDefaultErrorCallback	_errorCallback;
		physx::PxFoundation* _foundation;
		physx::PxPhysics* _physics;
		physx::PxDefaultCpuDispatcher* _dispatcher;
		physx::PxScene* _pxScene;
		physx::PxMaterial* _material;
		physx::PxPvd* _pvd;
	};
}

