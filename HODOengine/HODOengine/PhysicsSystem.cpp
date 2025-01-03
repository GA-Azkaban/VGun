﻿#include "PhysicsSystem.h"
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
		_material = _physics->createMaterial(0.9f, 0.8f, 0.2f);
		_playerMaterial = _physics->createMaterial(0.7f, 0.63f, 0.0f);
		_planeMaterial = _physics->createMaterial(0.2f, 0.1f, 0.0f);

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
		if (!_playerRigid)
		{
			return;
		}

		if (static_cast<HDData::DynamicCollider*>(_playerRigid->userData)->GetGameObject()->GetComponent<PlayerMove>()->_isIngamePlaying == false)
		{
			return;
		}

		_accumulateTime += API::GetDeltaTime();
		while (_accumulateTime >= 0.007f)
		{
			_accumulateTime -= 0.007f;
			for (auto& rigid : _rigidDynamics)
			{
				HDData::DynamicCollider* dynamicCol = static_cast<HDData::DynamicCollider*>(rigid->userData);
				if (dynamicCol->GetGameObject()->GetObjectName() == "playerSelf")
				{
					//physx::PxTransform temp = rigid->getGlobalPose();
					//TransformInfo tempInfo;
					//tempInfo.pos = Vector3(temp.p.x, temp.p.y, temp.p.z);
					//tempInfo.rot = Quaternion(temp.q.x, temp.q.y, temp.q.z, temp.q.w);
					//dynamicCol->SetPrevTransform(tempInfo);
					_prevPlayerRot = rigid->getGlobalPose().q;
				}
			}
			_collisionCallback->Clear();

			_pxScene->simulate(0.007f);
			_pxScene->fetchResults(true);

			_collisionCallback->CollectResults();
			_collisionCallback->SendTriggerEvents();
			_collisionCallback->SendCollisionEvents();

			for (auto& rigid : _rigidDynamics)
			{
				// Collider On/Off
				HDData::DynamicCollider* dynamicCol = static_cast<HDData::DynamicCollider*>(rigid->userData);
				if (!dynamicCol->GetIsStarted()) continue;

				if (dynamicCol->GetGameObject()->GetObjectName() == "playerSelf")
				{
					//physx::PxTransform temp = rigid->getGlobalPose();
					//TransformInfo tempInfo;
					//tempInfo.pos = Vector3(temp.p.x, temp.p.y, temp.p.z);
					//tempInfo.rot = Quaternion(temp.q.x, temp.q.y, temp.q.z, temp.q.w);
					//dynamicCol->SetCurTransform(tempInfo);
					_currentPlayerRot = rigid->getGlobalPose().q;
				}

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

		UpdateTransform();
		}
	}

	void PhysicsSystem::UpdateTransform()
	{
		// Transform Update

		for (auto& rigid : _rigidDynamics)
		{
			HDData::DynamicCollider* col = static_cast<HDData::DynamicCollider*>(rigid->userData);

			if (col->GetGameObject()->GetObjectName() == "playerSelf")
			{
				//float alpha = _accumulateTime / 0.008333f;

				physx::PxTransform nowTr = rigid->getGlobalPose();
				//sphysx::PxQuat newRot = Slerp(_prevPlayerRot, _currentPlayerRot, alpha);

				Vector3 pos = Vector3(nowTr.p.x, nowTr.p.y, nowTr.p.z);
				Quaternion rot = Quaternion(nowTr.q.x, nowTr.q.y, nowTr.q.z, nowTr.q.w);
				//Quaternion rot = Quaternion(newRot.x, newRot.y, newRot.z, newRot.w);

				col->UpdateFromPhysics(pos, rot);

				// Child Velocity
				for (auto& child : col->GetChildColliderVec())
				{
					// 손자뻘이 없음을 가정하고 만듦
					physx::PxRigidDynamic* childRigid = static_cast<HDData::DynamicCollider*>(child)->GetPhysXRigid();
					//childRigid->setLinearVelocity(rigid->getLinearVelocity());
					physx::PxVec3 childPos = nowTr.p;
					Vector3 localPos = child->GetTransform()->GetLocalPosition();
					childPos.x += (child->GetTransform()->GetForward() * localPos.z).x;
					childPos.y += localPos.y;
					childPos.z += (child->GetTransform()->GetForward() * localPos.z).z;
					childRigid->setGlobalPose(physx::PxTransform(childPos, nowTr.q));
					dynamic_cast<HDData::DynamicCollider*>(child)->UpdateFromPhysics(Vector3(childPos.x, childPos.y, childPos.z), rot);
				}
			}
			else
			{
				physx::PxTransform nowTr = rigid->getGlobalPose();
				Vector3 pos = Vector3(nowTr.p.x, nowTr.p.y, nowTr.p.z);
				Quaternion rot = Quaternion(nowTr.q.x, nowTr.q.y, nowTr.q.z, nowTr.q.w);
				col->UpdateFromPhysics(pos, rot);
			}
			//auto headCam = col->GetGameObject()->GetComponentInChildren<HDData::Camera>();
			//if (headCam != nullptr)
			//{
			//	Vector3 deltaZ = 0.175f * col->GetTransform()->GetForward();
			//	headCam->GetTransform()->SetPosition(pos + Vector3(deltaZ.x, 1.65f, deltaZ.z));
			//}
			
			//auto fpMeshShell = col->GetGameObject()->GetGameObjectByNameInChildren("meshShell");
			//if (fpMeshShell != nullptr)
			//{
			//	Vector3 localppp = fpMeshShell->GetTransform()->GetLocalPosition();
			//	Vector3 ppp;
			//	Vector3 px = localppp.x * col->GetTransform()->GetRight();
			//	Vector3 pz = localppp.z * col->GetTransform()->GetForward();
			//	ppp.x = pos.x + px.x + pz.x;
			//	ppp.y = pos.y + localppp.y;
			//	ppp.z = pos.z + pz.z + px.z;
			//	fpMeshShell->GetTransform()->SetPosition(ppp);
			//}

			//auto fpMeshObj = col->GetGameObject()->GetGameObjectByNameInChildren("FPMesh");
			//if (fpMeshObj != nullptr)
			//{
			//	Vector3 localppp = fpMeshObj->GetTransform()->GetLocalPosition() + fpMeshShell->GetTransform()->GetLocalPosition();
			//	Vector3 ppp;
			//	ppp.x = pos.x + (localppp.x * col->GetTransform()->GetRight()).x;
			//	ppp.y = pos.y + localppp.y;
			//	ppp.z = pos.z + (localppp.z * col->GetTransform()->GetForward()).z;
			//	fpMeshObj->GetTransform()->SetPosition(ppp);
			//}
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
//#ifdef _DEBUG
//		sceneDesc.gravity = physx::PxVec3(0.0f, -9.80665f * 12, 0.0f);
//#else
		sceneDesc.gravity = physx::PxVec3(0.0f, -9.80665f * 3, 0.0f);
//#endif
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
				capsuleRigid->setMass(0.1f);
				capsuleRigid->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, !collider->GetIsActive());

				standingShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
				//sittingShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);

				capsuleRigid->attachShape(*standingShape);
				//capsuleRigid->attachShape(*sittingShape);

				// 앉을 때 switch 해줄 두 shape 를 넣어준다.
				//collider->SetPlayerShapes(standingShape, sittingShape);
				if (capsule->GetGameObject()->GetObjectName() == "playerSelf")
				{
					_playerRigid = capsuleRigid;
					_playerShape = standingShape;
					collider->SetPlayerMaterial(_playerMaterial);
				}

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

				physx::PxShape* shape;
				if (sphere->GetParentCollider() == nullptr)
				{
					shape = _physics->createShape(physx::PxSphereGeometry(sphere->GetRadius()), *_material);
				}
				else
				{
					shape = _physics->createShape(physx::PxSphereGeometry(sphere->GetRadius()), *_playerMaterial);
				}
				shape->userData = sphere;

				physx::PxFilterData headFilterData;
				headFilterData.word0 = sphere->GetColFilterNum();
				shape->setSimulationFilterData(headFilterData);
				shape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, true);

				Vector3 position = Vector3::Transform(collider->GetPositionOffset(), object->GetTransform()->GetWorldTM());
				physx::PxTransform localTransform(physx::PxVec3(position.x, position.y, position.z));
				physx::PxRigidDynamic* sphereRigid = _physics->createRigidDynamic(localTransform);
				//physx::PxRigidBodyExt::updateMassAndInertia(*sphereRigid, 0.0f);
				if (sphere->GetParentCollider() == nullptr)
				{
					sphereRigid->setMass(0.08f);
				}
				else
				{
					sphereRigid->setMass(0.2f);
				}
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

	physx::PxTransform PhysicsSystem::InterpolateTransform(const TransformInfo& prev, const TransformInfo& cur, float alpha)
	{
		physx::PxVec3 interpolatedPos = physx::PxVec3(prev.pos.x, prev.pos.y, prev.pos.z) * (1.0 - alpha) + physx::PxVec3(cur.pos.x, cur.pos.y, cur.pos.z) * alpha;
		physx::PxQuat interpolatedRot = Slerp(physx::PxQuat(prev.rot.x, prev.rot.y, prev.rot.z, prev.rot.w), physx::PxQuat(cur.rot.x, cur.rot.y, cur.rot.z, cur.rot.w), alpha);

		return physx::PxTransform(interpolatedPos, interpolatedRot);
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

	physx::PxQuat PhysicsSystem::Slerp(const physx::PxQuat& qa, const physx::PxQuat& qb, float time)
	{
		float cosHalfTheta = qa.w * qb.w + qa.x * qb.x + qa.y * qb.y + qa.z * qb.z;

		if (std::abs(cosHalfTheta) >= 1.0)
		{
			return qa;
		}

		if (cosHalfTheta < 0.0f)
		{
			return Slerp(qa, -qb, time);
		}

		float halfTheta = std::acos(cosHalfTheta);
		float sinHalfTheta = std::sqrt(1.0f - cosHalfTheta * cosHalfTheta);

		if (std::abs(sinHalfTheta) < 0.001f)
		{
			return physx::PxQuat(
				(qa.x * 0.5f + qb.x * 0.5f),
				(qa.y * 0.5f + qb.y * 0.5f),
				(qa.z * 0.5f + qb.z * 0.5f),
				(qa.w * 0.5f + qb.w * 0.5f)
			);
		}

		float ratioA = std::sin((1 - time) * halfTheta) / sinHalfTheta;
		float ratioB = std::sin(time * halfTheta) / sinHalfTheta;

		physx::PxQuat result(
			(qa.x * ratioA + qb.x * ratioB),
			(qa.y * ratioA + qb.y * ratioB),
			(qa.z * ratioA + qb.z * ratioB),
			(qa.w * ratioA + qb.w * ratioB)
		);
		return result;
	}

	physx::PxScene* PhysicsSystem::GetScene() const
	{
		return _pxScene;
	}
}
