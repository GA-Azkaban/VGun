#include "PhysicsSystem.h"

namespace hodoEngine
{

	void PhysicsSystem::Initialize()
	{
		_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, _allocator, _errorCallback);

		// visual debugger ����, ���ÿ� ����
		_pvd = PxCreatePvd(*_foundation);
		physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
		_pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

		// ����, ����, ���� ���� ������ ������ ���� ���� ����
		_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, physx::PxTolerancesScale(), true, _pvd);

		//PxInitExtensions(*_physics, _pvd);

		CreatePhysXScene();

		// ������ ź���� ������ ��Ƽ���� ����
		_material = _physics->createMaterial(0.25f, 0.2f, 0.4f);

		physx::PxRigidStatic* groundPlane = physx::PxCreatePlane(*_physics, physx::PxPlane(0.0f, 1.0f, 0.0f, 0.0f), *_material);
		_pxScene->addActor(*groundPlane);

		physx::PxTransform localTm(physx::PxVec3(2.0f, 2.0f, 2.0f));
		physx::PxRigidDynamic* dynamic = _physics->createRigidDynamic(localTm);
		dynamic->setAngularDamping(0.5f);
		dynamic->setLinearDamping(0.5f);
		_pxScene->addActor(*dynamic);

	}

	void PhysicsSystem::Update()
	{
		_pxScene->simulate(0.0167f);
		_pxScene->fetchResults(true);
	}

	void PhysicsSystem::Finalize()
	{
		// initPhysics���� �ʱ�ȭ���� ���� �������� release
		PX_RELEASE(_pxScene);
		PX_RELEASE(_dispatcher);
		PX_RELEASE(_physics);
		// visual debugger�� release
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
		// ���� ���� ����
		physx::PxSceneDesc sceneDesc(_physics->getTolerancesScale());
		sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
		_dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = _dispatcher;
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
		_pxScene = _physics->createScene(sceneDesc);

		// Pvd�� ���� ������
		physx::PxPvdSceneClient* pvdClient = _pxScene->getScenePvdClient();
		if (pvdClient)
		{
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}
	}
}