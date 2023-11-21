#include "PhysicsSystem.h"

void PhysicsSystem::Initialize()
{
	_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, _allocator, _errorCallback);

	// visual debugger ����, ���ÿ� ����
	_pvd = PxCreatePvd(*_foundation);
	physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	_pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eDEBUG);

	// ����, ����, ���� ���� ������ ������ ���� ���� ����
	_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, physx::PxTolerancesScale(), true, nullptr);
	CreatePhysXScene();

	// ������ ź���� ������ ��Ƽ���� ����
	_material = _physics->createMaterial(0.25f, 0.2f, 0.4f);


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
