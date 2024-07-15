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
#include "TriggerBoxCollider.h"
#include "ParticleSphereCollider.h"

#include <windows.h>

namespace HDEngine
{
	physx::PxFilterFlags CustomFilterShader(
		physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
		physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
		physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
	{
		pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;
		pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
		//pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_LOST;
		return physx::PxFilterFlag::eDEFAULT;
	}

	void PhysicsSystem::Initialize()
	{
		_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, _allocator, _errorCallback);

		// visual debugger 세팅, 로컬에 연결
		_pvd = PxCreatePvd(*_foundation);
		physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
		_pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

		// 버전, 세팅, 단위 등의 정보를 지정해 물리 씬을 생성
		_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, physx::PxTolerancesScale(), true, _pvd);

		PxInitExtensions(*_physics, _pvd);

		CreatePhysXScene();

		// 마찰과 탄성을 지정해 머티리얼 생성
		_material = _physics->createMaterial(0.2f, 0.2f, 0.0f);
		_playerMaterial = _physics->createMaterial(0.8f, 0.6f, 0.0f);
		_planeMaterial = _physics->createMaterial(0.6f, 0.4f, 0.0f);

		_collisionCallback = std::make_unique<CollisionCallback>();
		_pxScene->setSimulationEventCallback(_collisionCallback.get());

		_pxScene->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LOCAL_FRAMES, 1.0f);
		_pxScene->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LIMITS, 1.0f);
	}

	void PhysicsSystem::PreparePhysics()
	{
		CreateRigidBodies();
		CreateSphericalJoint();
		AddActorsToScene();
	}

	void PhysicsSystem::Update()
	{
		const auto& sceneIter = SceneSystem::Instance().GetCurrentScene();

		_collisionCallback->Clear();

		//ResizeCollider();

#ifdef _DEBUG
		_pxScene->simulate(0.00167f);
#else
		_pxScene->simulate(0.0005f);
#endif
		_pxScene->fetchResults(true);

		_collisionCallback->CollectResults();
		_collisionCallback->SendTriggerEvents();
		_collisionCallback->SendCollisionEvents();

		for (auto& rigid : _rigidDynamics)
		{
			// Collider On/Off
			HDData::DynamicCollider* dynamicCol = static_cast<HDData::DynamicCollider*>(rigid->userData);
			if (!dynamicCol->GetIsStarted()) continue;

			// 트리거가 아닌 경우 onCollision 함수들 실행
			if (dynamicCol->GetIsTriggerType() == false)
			{
				if (!dynamicCol->GetPrevIsCollide() && dynamicCol->GetIsCollide())
				{
					dynamicCol->GetGameObject()->OnCollisionEnter(dynamicCol->GetCollisionStorage().data(), dynamicCol->GetCollisionStorage().size());
				}
				// Stay는 잠시 보류해뒀다. PhysX 내부에서 지원해주지 않음.
				else if (dynamicCol->GetPrevIsCollide() && dynamicCol->GetIsCollide())
				{
					dynamicCol->GetGameObject()->OnCollisionStay(dynamicCol->GetCollisionStorage().data(), dynamicCol->GetCollisionStorage().size());
				}
				else if (dynamicCol->GetPrevIsCollide() && !dynamicCol->GetIsCollide())
				{
					dynamicCol->GetGameObject()->OnCollisionExit(dynamicCol->GetCollisionStorage().data(), dynamicCol->GetCollisionStorage().size());
				}
			}
			else
			{
				if (!dynamicCol->GetPrevIsTriggerCollide() && dynamicCol->GetIsTriggerCollide())
				{
 					dynamicCol->GetGameObject()->OnTriggerEnter(dynamicCol->GetTriggerStorage().data(), dynamicCol->GetTriggerStorage().size());
				}
				// Stay는 잠시 보류해뒀다. PhysX 내부에서 지원해주지 않음.
				else if (dynamicCol->GetPrevIsTriggerCollide() && dynamicCol->GetIsTriggerCollide())
				{
					//dynamicCol->GetGameObject()->OnTriggerStay(dynamicCol->GetCollisionStorage().data(), dynamicCol->GetCollisionStorage().size());
				}
				else if (dynamicCol->GetPrevIsTriggerCollide() && !dynamicCol->GetIsTriggerCollide())
				{
					dynamicCol->GetGameObject()->OnTriggerExit(dynamicCol->GetTriggerStorage().data(), dynamicCol->GetTriggerStorage().size());
				}
			}
		}

		for (auto& rigid : _rigidDynamics)
		{
			// Transform Update
			physx::PxTransform nowTransform = rigid->getGlobalPose();
			Vector3 pos;
			Quaternion rot;

			pos.x = nowTransform.p.x;
			pos.y = nowTransform.p.y;
			pos.z = nowTransform.p.z;

			rot.x = nowTransform.q.x;
			rot.y = nowTransform.q.y;
			rot.z = nowTransform.q.z;
			rot.w = nowTransform.q.w;

			static_cast<HDData::DynamicCollider*>(rigid->userData)->UpdateFromPhysics(pos, rot);

		}

		for (auto& rigid : _movableStatics)
		{


			// Transform Update
			physx::PxTransform nowTransform = rigid->getGlobalPose();
			Vector3 pos;
			Quaternion rot;

			pos.x = nowTransform.p.x;
			pos.y = nowTransform.p.y;
			pos.z = nowTransform.p.z;

			rot.x = nowTransform.q.x;
			rot.y = nowTransform.q.y;
			rot.z = nowTransform.q.z;
			rot.w = nowTransform.q.w;

			static_cast<HDData::StaticCollider*>(rigid->userData)->UpdateFromPhysics(pos, rot);
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
		sceneDesc.gravity = physx::PxVec3(0.0f, -9.80665f * 150.0f, 0.0f);
		_dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = _dispatcher;
		//sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
		sceneDesc.filterShader = CustomFilterShader;
		sceneDesc.simulationEventCallback = _collisionCallback.get();

		// 1번을 몸톰, 2번을 다리, 팔, 머리
		//physx::PxSetGroupCollisionFlag(1, 2, false);

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
		//physx::PxRigidStatic* groundPlane = physx::PxCreatePlane(*_physics, physx::PxPlane(0.0f, 1.0f, 0.0f, 0.0f), *_planeMaterial);
		//_pxScene->addActor(*groundPlane);

		//const auto& sceneIter = SceneSystem::Instance().GetCurrentScene();

		auto& allScenes = SceneSystem::Instance().GetAllScenes();
		auto sceneIter = allScenes.find("InGame");
		//auto sceneIter = allScenes.find("Training");

		if (sceneIter != allScenes.end())
		{
			for (auto& object : sceneIter->second->GetGameObjectList())
			{
				//CreateStaticPlaneCollider(object);
				CreateStaticBoxCollider(object);
				CreateDynamicBoxCollider(object);
				CreateDynamicCapsuleCollider(object);
				CreateDynamicSphereCollider(object);
				CreateTriggerBoxCollider(object);
				CreateParticleSphereCollider(object);
			}
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

				physx::PxRigidStatic* planeRigid = physx::PxCreatePlane(*_physics, pxPlane, *_planeMaterial);
				//_pxScene->addActor(*planeRigid);
				_rigidStatics.push_back(planeRigid);
				//planeCollider->SetPhysXRigid(planeRigid);
				planeCollider->SetPhysXRigid(planeRigid);
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

				physx::PxShape* shape = _physics->createShape(physx::PxBoxGeometry(box->GetWidth() / 2, box->GetHeight() / 2, box->GetDepth() / 2), *_planeMaterial);
				//physx::PxShape* shape = _physics->createShape(physx::PxBoxGeometry(box->GetWidth() / 2 * scale.x * box->GetScaleOffset().x, box->GetHeight() / 2 * scale.y * box->GetScaleOffset().y, box->GetDepth() / 2 * scale.z * box->GetScaleOffset().z), *_planeMaterial);
				shape->userData = box;
				// TODO : 여기 작업하고 있었음.
				Vector3 position = object->GetTransform()->GetPosition();

				Vector3 posOffset = collider->GetPositionOffset();
				if (posOffset != Vector3::Zero)
				{
					//posOffset.x *= -1.0f;
					//posOffset.z *= -1.0f;
					position = Vector3::Transform(posOffset, object->GetTransform()->GetWorldTM());
				}

				Quaternion rot = object->GetTransform()->GetRotation();
				physx::PxTransform localTransform(physx::PxVec3(position.x, position.y, position.z));
				localTransform.q.x = rot.x;
				localTransform.q.y = rot.y;
				localTransform.q.z = rot.z;
				localTransform.q.w = rot.w;

				physx::PxRigidStatic* boxRigid = _physics->createRigidStatic(localTransform);
				boxRigid->attachShape(*shape);
				boxRigid->userData = box;
				box->SetPhysXRigid(boxRigid);
				//_pxScene->addActor(*boxRigid);
				_rigidStatics.push_back(boxRigid);
				shape->release();
				// 본체와 물리에서 서로의 rigid, collider를 건드릴 수 있게 해주는 부분. 추가?
			}
		}
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
				//shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
				shape->userData = box;

				physx::PxFilterData playerFilterData;
				playerFilterData.word0 = collider->GetColGroup();
				shape->setSimulationFilterData(playerFilterData);

				Vector3 position = Vector3::Transform(collider->GetPositionOffset(), object->GetTransform()->GetWorldTM());
				physx::PxTransform localTransform(physx::PxVec3(position.x, position.y, position.z));
				physx::PxRigidDynamic* boxRigid = _physics->createRigidDynamic(localTransform);
				boxRigid->setLinearDamping(0.5f);
				boxRigid->setAngularDamping(0.5f);
				physx::PxRigidBodyExt::updateMassAndInertia(*boxRigid, 0.0f);
				//boxRigid->setMass(10.0f);
				if (box->GetColGroup() != 0)
				{
					boxRigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, true);
					boxRigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, true);
					boxRigid->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, true);
				}
				boxRigid->attachShape(*shape);

				//_pxScene->addActor(*boxRigid);
				_rigidDynamics.push_back(boxRigid);
				box->SetPhysXRigid(boxRigid);
				box->SetPhysScene(_pxScene);
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

				physx::PxShape* standingShape = _physics->createShape(physx::PxCapsuleGeometry(capsule->GetRadius(), capsule->GetHalfHeight()), *_playerMaterial);
				standingShape->userData = capsule;
				//physx::PxShape* sittingShape = _physics->createShape(physx::PxCapsuleGeometry(capsule->GetRadius(), capsule->GetHalfHeight() * 0.5f), *_playerMaterial);
				//sittingShape->userData = capsule;
				
				physx::PxFilterData filterData;
				filterData.word0 = capsule->GetColFilterNum();
				standingShape->setSimulationFilterData(filterData);
				standingShape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, true);
				//sittingShape->setSimulationFilterData(filterData);
				//sittingShape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, true);

				physx::PxQuat rotation = physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0, 0, 1));
				Vector3 posOffset = collider->GetPositionOffset();
				physx::PxTransform localTransform(physx::PxVec3(posOffset.x, posOffset.y + capsule->GetRadius(), posOffset.z), rotation);
				standingShape->setLocalPose(localTransform);
				//sittingShape->setLocalPose(localTransform);

				Vector3 position = Vector3::Transform(collider->GetPositionOffset(), object->GetTransform()->GetWorldTM());
				physx::PxRigidDynamic* capsuleRigid = _physics->createRigidDynamic(physx::PxTransform(physx::PxVec3(position.x, position.y, position.z)));
				capsuleRigid->setMass(1.5f);
				capsuleRigid->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, !collider->GetIsActive());

				standingShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
				//sittingShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);

				capsuleRigid->attachShape(*standingShape);
				//capsuleRigid->attachShape(*sittingShape);

				// 앉을 때 switch 해줄 두 shape 를 넣어준다.
				//collider->SetPlayerShapes(standingShape, sittingShape);
				_playerRigid = capsuleRigid;
				_playerShape = standingShape;

				//_pxScene->addActor(*capsuleRigid);
				_rigidDynamics.push_back(capsuleRigid);
				capsule->SetPhysXRigid(capsuleRigid);
				capsule->SetPhysScene(_pxScene);
				capsuleRigid->userData = capsule;
				standingShape->release();

				if (capsule->GetFreezeRotation())
				{
					capsule->LockPlayerRotation(true);
				}

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

				physx::PxShape* shape = _physics->createShape(physx::PxSphereGeometry(sphere->GetRadius()), *_playerMaterial);
				shape->userData = sphere;

				physx::PxFilterData headFilterData;
				headFilterData.word0 = sphere->GetColFilterNum();
				shape->setSimulationFilterData(headFilterData);
				shape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, true);

				Vector3 position = Vector3::Transform(collider->GetPositionOffset(), object->GetTransform()->GetWorldTM());
				physx::PxTransform localTransform(physx::PxVec3(position.x, position.y, position.z));
				physx::PxRigidDynamic* sphereRigid = _physics->createRigidDynamic(localTransform);
				physx::PxRigidBodyExt::updateMassAndInertia(*sphereRigid, 0.0f);
				sphereRigid->attachShape(*shape);

				//_pxScene->addActor(*sphereRigid);
				_rigidDynamics.push_back(sphereRigid);
				sphere->SetPhysXRigid(sphereRigid);
				sphere->SetPhysScene(_pxScene);
				sphereRigid->userData = sphere;
				shape->release();
				// 본체와 물리에서 서로의 rigid, collider를 건드릴 수 있게 해주는 부분. 추가?
			}
		}
	}

	void PhysicsSystem::CreateTriggerBoxCollider(HDData::GameObject* object)
	{
		HDData::Collider* isCorrectType = object->GetComponent<HDData::TriggerBoxCollider>();

		if (isCorrectType)
		{
			auto colliderVector = object->GetComponents<HDData::TriggerBoxCollider>();
			for (auto& collider : colliderVector)
			{
				HDData::TriggerBoxCollider* box = dynamic_cast<HDData::TriggerBoxCollider*>(collider);
				Vector3 scale = object->GetTransform()->GetScale();

				physx::PxShape* shape = _physics->createShape(physx::PxBoxGeometry(box->GetWidth() / 2 * scale.x, box->GetHeight() / 2 * scale.y, box->GetDepth() / 2 * scale.z), *_material);
				shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
				shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
				shape->userData = box;

				Vector3 position = Vector3::Transform(collider->GetPositionOffset(), object->GetTransform()->GetWorldTM());

				Quaternion rot = object->GetTransform()->GetRotation();
				physx::PxTransform localTransform(physx::PxVec3(position.x, position.y, position.z));
				localTransform.q.x = rot.x;
				localTransform.q.y = rot.y;
				localTransform.q.z = rot.z;
				localTransform.q.w = rot.w;

				//physx::PxRigidStatic* boxRigid = _physics->createRigidStatic(localTransform);
				physx::PxRigidDynamic* boxRigid = _physics->createRigidDynamic(localTransform);
				boxRigid->attachShape(*shape);
				//_rigidStatics.push_back(boxRigid);
				_rigidDynamics.push_back(boxRigid);
				boxRigid->userData = box;
				box->SetPhysXRigid(boxRigid);
				box->SetPhysScene(_pxScene);
				shape->release();
			}
		}
	}

	void PhysicsSystem::CreateParticleSphereCollider(HDData::GameObject* object)
	{
		HDData::Collider* isCorrectType = object->GetComponent<HDData::ParticleSphereCollider>();

		if (isCorrectType)
		{
			auto colliderVector = object->GetComponents<HDData::ParticleSphereCollider>();
			for (auto& collider : colliderVector)
			{
				HDData::ParticleSphereCollider* particle = dynamic_cast<HDData::ParticleSphereCollider*>(collider);

				physx::PxShape* shape = _physics->createShape(physx::PxSphereGeometry(particle->GetRadius()), *_material);
				shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
				shape->userData = particle;

				Vector3 position = Vector3::Transform(collider->GetPositionOffset(), object->GetTransform()->GetWorldTM());
				physx::PxTransform localTransform(physx::PxVec3(position.x, position.y, position.z));
				physx::PxRigidStatic* sphereRigid = _physics->createRigidStatic(localTransform);
				sphereRigid->attachShape(*shape);

				//_pxScene->addActor(*sphereRigid);
				_movableStatics.push_back(sphereRigid);
				particle->SetPhysXRigid(sphereRigid);
				sphereRigid->userData = particle;
				shape->release();
			}
		}
	}

	void PhysicsSystem::ResizeCollider()
	{
		if (_playerRigid == nullptr)
		{
			return;
		}

		HDData::DynamicCapsuleCollider* capsule = static_cast<HDData::DynamicCapsuleCollider*>(_playerRigid->userData);

		if (capsule->GetSitStand() == 1)
		{
			// shape만 갈아끼는 버전.
			/*
			_playerRigid->detachShape(*_playerShape);

			capsule->SetHalfHeight(capsule->GetHalfHeight() * 0.5f);
			physx::PxShape* newShape = _physics->createShape(physx::PxCapsuleGeometry(capsule->GetRadius(), capsule->GetHalfHeight()), *_playerMaterial);
			newShape->userData = _playerShape->userData;

			physx::PxShapeFlags shapeFlags = newShape->getFlags();
			newShape->setFlags(shapeFlags | physx::PxShapeFlag::eSIMULATION_SHAPE);
			newShape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, true);
			newShape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, true);

			newShape->setSimulationFilterData(_playerShape->getSimulationFilterData());

			_playerRigid->attachShape(*newShape);
			_playerShape = newShape;
			capsule->SetSitStand(0);
			*/

			// rigid를 재생성하는 버전.
			capsule->SetHalfHeight(capsule->GetHalfHeight() * 0.5f);
			physx::PxShape* sittingShape = _physics->createShape(physx::PxCapsuleGeometry(capsule->GetRadius(), capsule->GetHalfHeight()), *_playerMaterial);
			sittingShape->userData = capsule;
			HDData::Transform* headTransform = capsule->GetGameObject()->GetChildGameObjects()[0]->GetTransform();
			//headTransform->SetLocalPosition(headTransform->GetLocalPosition() - Vector3(0.0f, capsule->GetHalfHeight(), 0.0f));

			physx::PxFilterData filterData;
			filterData.word0 = capsule->GetColFilterNum();
			sittingShape->setSimulationFilterData(filterData);
			sittingShape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, true);

			sittingShape->setLocalPose(_playerShape->getLocalPose());

			physx::PxTransform pose = _playerRigid->getGlobalPose();
			pose.p.y -= capsule->GetHalfHeight();
			physx::PxRigidDynamic* capsuleRigid = _physics->createRigidDynamic(pose);
			capsuleRigid->setMass(1.5f);

			sittingShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);

			capsuleRigid->attachShape(*sittingShape);

			_playerShape->release();
			_pxScene->removeActor(*_playerRigid);
			std::erase_if(_rigidDynamics, [this](physx::PxRigidDynamic* rigid) {return (rigid == _playerRigid); });
			_pxScene->addActor(*capsuleRigid);
			_playerRigid->release();
			
			_playerRigid = capsuleRigid;
			_playerShape = sittingShape;

			_rigidDynamics.push_back(capsuleRigid);
			capsule->SetPhysXRigid(capsuleRigid);
			capsuleRigid->userData = capsule;

			if (capsule->GetFreezeRotation())
			{
				capsule->LockPlayerRotation(true);
			}

			_joints.clear();
			CreateSphericalJoint();
			capsule->SetSitStand(0);
		}
		else if (capsule->GetSitStand() == 2)
		{
			// shape만 갈아끼는 버전.
			/*
			_playerRigid->detachShape(*_playerShape);

			capsule->SetHalfHeight(capsule->GetHalfHeight() * 2.0f);
			physx::PxShape* newShape = _physics->createShape(physx::PxCapsuleGeometry(capsule->GetRadius(), capsule->GetHalfHeight()), *_playerMaterial);
			newShape->userData = _playerShape->userData;

			physx::PxShapeFlags shapeFlags = newShape->getFlags();
			newShape->setFlags(shapeFlags | physx::PxShapeFlag::eSIMULATION_SHAPE);
			newShape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, true);

			newShape->setSimulationFilterData(_playerShape->getSimulationFilterData());

			_playerRigid->attachShape(*newShape);
			_playerShape = newShape;
			capsule->SetSitStand(0);
			*/

			// rigid 재생성하는 버전
			capsule->SetHalfHeight(capsule->GetHalfHeight() * 2.0f);
			physx::PxShape* standingShape = _physics->createShape(physx::PxCapsuleGeometry(capsule->GetRadius(), capsule->GetHalfHeight()), *_playerMaterial);
			standingShape->userData = capsule;
			HDData::Transform* headTransform = capsule->GetGameObject()->GetChildGameObjects()[0]->GetTransform();
			headTransform->SetLocalPosition(headTransform->GetLocalPosition() + Vector3(0.0f, capsule->GetHalfHeight(), 0.0f));

			physx::PxFilterData filterData;
			filterData.word0 = capsule->GetColFilterNum();
			standingShape->setSimulationFilterData(filterData);
			standingShape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, true);

			standingShape->setLocalPose(_playerShape->getLocalPose());

			physx::PxTransform pose = _playerRigid->getGlobalPose();
			pose.p.y += capsule->GetHalfHeight();
			physx::PxRigidDynamic* capsuleRigid = _physics->createRigidDynamic(pose);
			capsuleRigid->setMass(1.5f);

			standingShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);

			capsuleRigid->attachShape(*standingShape);

			_playerShape->release();
			_pxScene->removeActor(*_playerRigid);
			std::erase_if(_rigidDynamics, [this](physx::PxRigidDynamic* rigid) {return (rigid == _playerRigid); });
			_pxScene->addActor(*capsuleRigid);
			_playerRigid->release();

			_playerRigid = capsuleRigid;
			_playerShape = standingShape;

			_rigidDynamics.push_back(capsuleRigid);
			capsule->SetPhysXRigid(capsuleRigid);
			capsuleRigid->userData = capsule;

			if (capsule->GetFreezeRotation())
			{
				capsule->LockPlayerRotation(true);
			}

			_joints.clear();
			CreateSphericalJoint();
			capsule->SetSitStand(0);
		}
	}

	void PhysicsSystem::CreateSphericalJoint()
	{
		for (auto& dynamics : _rigidDynamics)
		{
			HDData::DynamicCollider* thisCol = static_cast<HDData::DynamicCollider*>(dynamics->userData);
			HDData::DynamicCollider* parentCol = static_cast<HDData::DynamicCollider*>(thisCol->GetParentCollider());
			if (parentCol != nullptr)
				//if (thisCol->GetGameObject()->GetObjectName() == "playerHead")
			{
				physx::PxTransform localTransform(physx::PxIdentity);
				Vector3 localPose = thisCol->GetTransform()->GetLocalPosition();
				localTransform.p = { localPose.x, localPose.y, localPose.z };

				//if (thisCol->GetGameObject()->GetObjectName() == "head")
				//{
				//	physx::PxSphericalJoint* resultJoint = physx::PxSphericalJointCreate(*_physics, dynamics, physx::PxTransform(physx::PxIdentity),
				//		parentCol->GetPhysXRigid(), localTransform);
				//	_joints.push_back(resultJoint);
				//}
				//else
				//{
					physx::PxFixedJoint* resultJoint = physx::PxFixedJointCreate(*_physics, dynamics, physx::PxTransform(physx::PxIdentity),
						parentCol->GetPhysXRigid(), localTransform);
					_joints.push_back(resultJoint);
				//}
			}
		}
	}

	void PhysicsSystem::AddActorsToScene()
	{
		for (auto& actors : _rigidDynamics)
		{
			_pxScene->addActor(*actors);
		}

		for (auto& actors : _rigidStatics)
		{
			_pxScene->addActor(*actors);
		}

		for (auto& actors : _movableStatics)
		{
			_pxScene->addActor(*actors);
		}
	}

	void PhysicsSystem::Flush()
	{
		for (auto& rigid : _rigidDynamics)
		{
			static_cast<HDData::DynamicCollider*>(rigid->userData)->Flush();
		}

		for (auto& rigid : _rigidStatics)
		{
			static_cast<HDData::StaticCollider*>(rigid->userData)->Flush();
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
