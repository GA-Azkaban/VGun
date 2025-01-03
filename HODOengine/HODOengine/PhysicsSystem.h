﻿#define _SILENCE_CXX20_CISO646_REMOVED_WARNING
#pragma once

#include <vector>
#include <memory>

#include "Singleton.h"
#include "HODO_API.h"
#include "CollisionCallback.h"

#include "../HODOclient/PlayerMove.h"
#include "../include/physX/PxPhysics.h"
#include "../include/physX/PxPhysicsAPI.h"

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
		void UpdateTransform();
		void Finalize();

	public:
		void CreatePhysXScene();
		void CreateRigidBodies();
		void CreateSphericalJoint();
		void AddActorsToScene();
		void Flush();

	public:
		physx::PxScene* GetScene() const;

	public:
		void CreateStaticPlaneCollider(HDData::GameObject* object);
		void CreateStaticBoxCollider(HDData::GameObject* object);
		void CreateDynamicBoxCollider(HDData::GameObject* object);
		void CreateDynamicCapsuleCollider(HDData::GameObject* object);
		void CreateDynamicSphereCollider(HDData::GameObject* object);
		void CreateTriggerBoxCollider(HDData::GameObject* object);
		void CreateParticleSphereCollider(HDData::GameObject* object);

	private:
		void ResizeCollider();
		physx::PxTransform InterpolateTransform(const TransformInfo& prev, const TransformInfo& cur, float alpha);
		physx::PxQuat Slerp(const physx::PxQuat& qa, const physx::PxQuat& qb, float time);

	public:
		//HDData::Collider* RayCast(Vector3 origin, Vector3 direction, float length, int* type);
		HDData::Collider* RayCast(float originX, float originY, float originZ, float directionX, float directionY, float directionZ, float length, int* type);
		HDData::Collider* RayCastHitPoint(float originX, float originY, float originZ, float directionX, float directionY, float directionZ, Vector3& hitPos, float length, int* type);

	private:
		physx::PxDefaultAllocator		_allocator;
		physx::PxDefaultErrorCallback	_errorCallback;
		physx::PxFoundation* _foundation;
		physx::PxPhysics* _physics;
		physx::PxDefaultCpuDispatcher* _dispatcher;
		physx::PxScene* _pxScene;
		physx::PxMaterial* _material;
		physx::PxMaterial* _playerMaterial;
		physx::PxMaterial* _planeMaterial;
		physx::PxPvd* _pvd;

	private:
		std::vector<physx::PxRigidDynamic*> _rigidDynamics;
		std::vector<physx::PxRigidStatic*> _rigidStatics;
		std::vector<physx::PxRigidStatic*> _movableStatics;
		std::vector<physx::PxJoint*> _joints;

		std::unique_ptr<CollisionCallback> _collisionCallback;

	private:
		// 맘에 들진 않지만 플레이어를 여기에 저장해두자
		physx::PxRigidDynamic* _playerRigid;
		physx::PxShape* _playerShape;
		PlayerMove* _plMove;

	private:
		float _accumulateTime = 0.0f;
		physx::PxQuat _prevPlayerRot;
		physx::PxQuat _currentPlayerRot;
	};
}
