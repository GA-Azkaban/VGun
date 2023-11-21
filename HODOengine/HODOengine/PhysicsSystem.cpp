#include "PhysicsSystem.h"

void PhysicsSystem::Initialize()
{
	_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, _allocator, _errorCallback);

	// visual debugger 세팅, 로컬에 연결
	_pvd = PxCreatePvd(*_foundation);
	physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	_pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eDEBUG);

	// 버전, 세팅, 단위 등의 정보를 지정해 물리 씬을 생성
	_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, physx::PxTolerancesScale(), true, nullptr);
	CreatePhysXScene();

	// 마찰과 탄성을 지정해 머티리얼 생성
	_material = _physics->createMaterial(0.25f, 0.2f, 0.4f);


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
