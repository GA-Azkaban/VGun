#include "PhysicsSystem.h"
#include "SceneSystem.h"
#include "Scene.h"
#include "GameObject.h"

#include "Collider.h"
#include "StaticPlaneCollider.h"
#include "StaticBoxCollider.h"
#include "DynamicBoxCollider.h"
#include "DynamicCapsuleCollider.h"
#include "DynamicSphereCollider.h"

#include <windows.h>

namespace HDEngine
{
	void PhysicsSystem::Initialize()
	{
		_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, _allocator, _errorCallback);

		// visual debugger 세팅, 로컬에 연결
		_pvd = PxCreatePvd(*_foundation);
		physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
		_pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

		// 버전, 세팅, 단위 등의 정보를 지정해 물리 씬을 생성
		_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, physx::PxTolerancesScale(), true, _pvd);

		//PxInitExtensions(*_physics, _pvd);

		CreatePhysXScene();

		// 마찰과 탄성을 지정해 머티리얼 생성
		_material = _physics->createMaterial(0.2f, 0.2f, 0.0f);
		_playerMaterial = _physics->createMaterial(0.2f, 0.2f, 0.0f);

		//physx::PxShape* shape = _physics->createShape(physx::PxBoxGeometry(1.0f, 1.0f, 1.0f), *_material);
		//physx::PxTransform localTm(physx::PxVec3(2.0f, 20.0f, 2.0f));
		//physx::PxRigidDynamic* _dynamic = _physics->createRigidDynamic(localTm);
		//_dynamic->attachShape(*shape);
		//_dynamic->setAngularDamping(0.5f);
		//_dynamic->setLinearDamping(0.5f);
		//_rigidDynamics.push_back(_dynamic);
		//_pxScene->addActor(*_dynamic);
	}

	void PhysicsSystem::PreparePhysics()
	{
		CreateRigidBodies();
	}

	void PhysicsSystem::Update()
	{
		_pxScene->simulate(0.00167f);
		_pxScene->fetchResults(true);

		Vector3 pos;
		Quaternion rot;
		physx::PxTransform temp;

		for (auto& rigid : _rigidDynamics)
		{
			temp = rigid->getGlobalPose();

			pos.x = temp.p.x;
			pos.y = temp.p.y;
			pos.z = temp.p.z;

			rot.x = -temp.q.x;
			rot.y = temp.q.y;
			rot.z = temp.q.z;
			rot.w = temp.q.w;

			static_cast<HDData::DynamicCollider*>(rigid->userData)->UpdateFromPhysics(pos, rot);
		}
	}

	void PhysicsSystem::Finalize()
	{
		// initPhysics에서 초기화해준 전역 변수들을 release
		PX_RELEASE(_pxScene);
		PX_RELEASE(_dispatcher);
		PX_RELEASE(_physics);
		// visual debugger도 release
		if (_pvd)
		{
			physx::PxPvdTransport* transport = _pvd->getTransport();
			_pvd->release();	_pvd = NULL;
			PX_RELEASE(transport);
		}
		PX_RELEASE(_foundation);
	}

	void PhysicsSystem::CreatePhysXScene()
	{
		// 씬에 대한 설정
		physx::PxSceneDesc sceneDesc(_physics->getTolerancesScale());
		sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
		_dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = _dispatcher;
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
		_pxScene = _physics->createScene(sceneDesc);

		// Pvd에 정보 보내기
		physx::PxPvdSceneClient* pvdClient = _pxScene->getScenePvdClient();
		if (pvdClient)
		{
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}
	}

	void PhysicsSystem::CreateRigidBodies()
	{
		// fundamental ground
		physx::PxRigidStatic* groundPlane = physx::PxCreatePlane(*_physics, physx::PxPlane(0.0f, 1.0f, 0.0f, 0.0f), *_material);
		_pxScene->addActor(*groundPlane);

		const auto& sceneIter = SceneSystem::Instance().GetCurrentScene();

		for (auto& object : sceneIter->GetGameObjectList())
		{
			CreateStaticPlaneCollider(object);
			CreateStaticBoxCollider(object);
			CreateDynamicBoxCollider(object);
			CreateDynamicCapsuleCollider(object);
			CreateDynamicSphereCollider(object);
		}
	}

	void PhysicsSystem::CreateStaticPlaneCollider(HDData::GameObject* object)
	{
		HDData::Collider* isCorrectType = object->GetComponent<HDData::StaticPlaneCollider>();

		if (isCorrectType)
		{
			auto colliderVector = object->GetComponents<HDData::StaticPlaneCollider>();
			for (auto& collider : colliderVector)
			{
				HDData::StaticPlaneCollider* planeCollider = dynamic_cast<HDData::StaticPlaneCollider*>(collider);
				Vector3 normal = planeCollider->GetNormalVector();
				physx::PxPlane pxPlane(normal.x, normal.y, normal.z, planeCollider->GetDistance());

				physx::PxRigidStatic* planeRigid = physx::PxCreatePlane(*_physics, pxPlane, *_material);
				_pxScene->addActor(*planeRigid);
				//planeCollider->SetPhysXRigid(planeRigid);
				planeRigid->userData = planeCollider;

				// 본체와 물리에서 서로의 rigid, collider를 건드릴 수 있게 해주는 부분. 추가?
			}
		}
	}

	void PhysicsSystem::CreateStaticBoxCollider(HDData::GameObject* object)
	{
		HDData::Collider* isCorrectType = object->GetComponent<HDData::StaticBoxCollider>();

		if (isCorrectType)
		{
			auto colliderVector = object->GetComponents<HDData::StaticBoxCollider>();
			for (auto& collider : colliderVector)
			{
				HDData::StaticBoxCollider* box = dynamic_cast<HDData::StaticBoxCollider*>(collider);

				physx::PxShape* shape = _physics->createShape(physx::PxBoxGeometry(box->GetWidth() / 2, box->GetHeight() / 2, box->GetDepth() / 2), *_material);
				 
				// TODO : 여기 작업하고 있었음.
				Vector3 position = Vector3::Transform(collider->GetPositionOffset(), object->GetTransform()->GetWorldTM());
				physx::PxTransform localTransform(physx::PxVec3(position.x, position.y, position.z));
				physx::PxRigidStatic* boxRigid = _physics->createRigidStatic(localTransform);
				boxRigid->attachShape(*shape);

				_pxScene->addActor(*boxRigid);
				shape->release();
				// 본체와 물리에서 서로의 rigid, collider를 건드릴 수 있게 해주는 부분. 추가?
			}
		}
	}

	void PhysicsSystem::CreateStaticBoxCollider(float width, float height, float depth)
	{
		HDData::StaticBoxCollider* box = new HDData::StaticBoxCollider(width, height, depth);

		physx::PxShape* shape = _physics->createShape(physx::PxBoxGeometry(box->GetWidth() / 2, box->GetHeight() / 2, box->GetDepth() / 2), *_material);

		physx::PxTransform localTransform(physx::PxVec3(0.0f, box->GetHeight() / 2, 0.0f));
		physx::PxRigidStatic* boxRigid = _physics->createRigidStatic(localTransform);
		boxRigid->attachShape(*shape);

		_pxScene->addActor(*boxRigid);
		shape->release();
	}

	void PhysicsSystem::CreateDynamicBoxCollider(HDData::GameObject* object)
	{
		HDData::Collider* isCorrectType = object->GetComponent<HDData::DynamicBoxCollider>();

		if (isCorrectType)
		{
			auto colliderVector = object->GetComponents<HDData::DynamicBoxCollider>();
			for (auto& collider : colliderVector)
			{
				HDData::DynamicBoxCollider* box = dynamic_cast<HDData::DynamicBoxCollider*>(collider);

				// switch material if player
				physx::PxShape* shape = _physics->createShape(physx::PxBoxGeometry(box->GetWidth() / 2, box->GetHeight() / 2, box->GetDepth() / 2), *_playerMaterial);

				Vector3 position = Vector3::Transform(collider->GetPositionOffset(), object->GetTransform()->GetWorldTM());
				physx::PxTransform localTransform(physx::PxVec3(position.x, position.y, position.z));
				physx::PxRigidDynamic* boxRigid = _physics->createRigidDynamic(localTransform);
				boxRigid->setLinearDamping(0.5f);
				boxRigid->setAngularDamping(0.2f);
				boxRigid->attachShape(*shape);

				// add only if player
				if (object != nullptr)
				{
					boxRigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, true);
					//boxRigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, true);
					boxRigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, true);
				}

				_pxScene->addActor(*boxRigid);
				_rigidDynamics.push_back(boxRigid);
				box->SetPhysXRigid(boxRigid);
				boxRigid->userData = box;
				shape->release();
				// 본체와 물리에서 서로의 rigid, collider를 건드릴 수 있게 해주는 부분. 추가?
			}
		}
	}

	void PhysicsSystem::CreateDynamicCapsuleCollider(HDData::GameObject* object)
	{
		HDData::Collider* isCorrectType = object->GetComponent<HDData::DynamicCapsuleCollider>();

		if (isCorrectType)
		{
			auto colliderVector = object->GetComponents<HDData::DynamicCapsuleCollider>();
			for (auto& collider : colliderVector)
			{
				HDData::DynamicCapsuleCollider* capsule = dynamic_cast<HDData::DynamicCapsuleCollider*>(collider);

				physx::PxShape* shape = _physics->createShape(physx::PxCapsuleGeometry(capsule->GetRadius(), capsule->GetHalfHeight()), *_material);

				Vector3 position = Vector3::Transform(collider->GetPositionOffset(), object->GetTransform()->GetWorldTM());
				physx::PxTransform localTransform(physx::PxVec3(position.x, position.y, position.z));
				physx::PxRigidDynamic* capsuleRigid = _physics->createRigidDynamic(localTransform);
				capsuleRigid->attachShape(*shape);

				_pxScene->addActor(*capsuleRigid);
				_rigidDynamics.push_back(capsuleRigid);
				capsuleRigid->userData = capsule;
				shape->release();
				// 본체와 물리에서 서로의 rigid, collider를 건드릴 수 있게 해주는 부분. 추가?
			}
		}
	}

	void PhysicsSystem::CreateDynamicSphereCollider(HDData::GameObject* object)
	{
		HDData::Collider* isCorrectType = object->GetComponent<HDData::DynamicSphereCollider>();

		if (isCorrectType)
		{
			auto colliderVector = object->GetComponents<HDData::DynamicSphereCollider>();
			for (auto& collider : colliderVector)
			{
				HDData::DynamicSphereCollider* sphere = dynamic_cast<HDData::DynamicSphereCollider*>(collider);

				physx::PxShape* shape = _physics->createShape(physx::PxSphereGeometry(sphere->GetRadius()), *_material);

				Vector3 position = Vector3::Transform(collider->GetPositionOffset(), object->GetTransform()->GetWorldTM());
				physx::PxTransform localTransform(physx::PxVec3(position.x, position.y, position.z));
				physx::PxRigidDynamic* sphereRigid = _physics->createRigidDynamic(localTransform);
				sphereRigid->attachShape(*shape);

				_pxScene->addActor(*sphereRigid);
				_rigidDynamics.push_back(sphereRigid);
				sphere->SetPhysXRigid(sphereRigid);
				sphereRigid->userData = sphere;
				shape->release();
				// 본체와 물리에서 서로의 rigid, collider를 건드릴 수 있게 해주는 부분. 추가?
			}
		}
	}

	HDData::Collider* PhysicsSystem::RayCast(float originX, float originY, float originZ, float directionX, float directionY, float directionZ, float length, int* type)
	{
		physx::PxVec3 rayOrigin;
		rayOrigin.x = originX;
		rayOrigin.y = originY;
		rayOrigin.z = originZ;

		physx::PxVec3 rayDirection;
		rayDirection.x = directionX;
		rayDirection.y = directionY;
		rayDirection.z = directionZ;

		HDData::Collider* hitCol = nullptr;

		// determine if hit or not
		physx::PxRaycastBuffer hitBuffer;
		bool isHit = _pxScene->raycast(rayOrigin, rayDirection, length, hitBuffer);

		// process when hit. target's pointer, hit location, etc.
		if (isHit)
		{
			// collided actor's pointer
			physx::PxRigidActor* hitActor = hitBuffer.block.actor;

			// find actor's type out
			if (hitActor->getType() == physx::PxActorType::eRIGID_STATIC)
			{
				hitActor = static_cast<physx::PxRigidStatic*>(hitActor);
				if (type != nullptr)
				{
					*type = 1;
				}
			}
			else if (hitActor->getType() == physx::PxActorType::eRIGID_DYNAMIC)
			{
				hitActor = static_cast<physx::PxRigidDynamic*>(hitActor);
				if (type != nullptr)
				{
					*type = 2;
				}
			}

			// save collision info in userdata
			hitCol = static_cast<HDData::Collider*>(hitActor->userData);

			// save hitpoint(for particle effect or sth)
			physx::PxVec3 hitPoint = hitBuffer.block.position;
		}
		
		return hitCol;
	}


	HDData::Collider* PhysicsSystem::RayCastHitPoint(float originX, float originY, float originZ, float directionX, float directionY, float directionZ, Vector3& hitPos, float length, int* type)
	{
		physx::PxVec3 rayOrigin;
		rayOrigin.x = originX;
		rayOrigin.y = originY;
		rayOrigin.z = originZ;

		physx::PxVec3 rayDirection;
		rayDirection.x = directionX;
		rayDirection.y = directionY;
		rayDirection.z = directionZ;

		HDData::Collider* hitCol = nullptr;

		// determine if hit or not
		physx::PxRaycastBuffer hitBuffer;
		bool isHit = _pxScene->raycast(rayOrigin, rayDirection, length, hitBuffer);

		// process when hit. target's pointer, hit location, etc.
		if (isHit)
		{
			// collided actor's pointer
			physx::PxRigidActor* hitActor = hitBuffer.block.actor;

			// find actor's type out
			if (hitActor->getType() == physx::PxActorType::eRIGID_STATIC)
			{
				hitActor = static_cast<physx::PxRigidStatic*>(hitActor);
				if (type != nullptr)
				{
					*type = 1;
				}
			}
			else if (hitActor->getType() == physx::PxActorType::eRIGID_DYNAMIC)
			{
				hitActor = static_cast<physx::PxRigidDynamic*>(hitActor);
				if (type != nullptr)
				{
					*type = 2;
				}
			}

			// save collision info in userdata
			hitCol = static_cast<HDData::Collider*>(hitActor->userData);

			// save hitpoint(for particle effect or sth)
			physx::PxVec3 hitPoint = hitBuffer.block.position;
			hitPos = Vector3(hitPoint.x, hitPoint.y, hitPoint.z);
		}

		return hitCol;

	}

	physx::PxScene* PhysicsSystem::GetScene() const
	{
		return _pxScene;
	}
}
