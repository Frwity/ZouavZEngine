#include "System/Debug.hpp"
#include "PxPhysicsAPI.h"
#include "pvd/PxPvd.h"
#include "pvd/PxPvdSceneClient.h"
#include "System/PhysicSystem.hpp"
#include "System/TimeManager.hpp"
#include "GameObject.hpp"
#include "Component/ShapeCollision.hpp"

using namespace physx;

PxDefaultAllocator PhysicSystem::physxAllocator;
PxDefaultErrorCallback PhysicSystem::physxErrorCallback;
PxFoundation* PhysicSystem::foundation = nullptr;
PxPhysics* PhysicSystem::physics = nullptr;
PxCooking* PhysicSystem::cooking = nullptr;
PxPvd* PhysicSystem::pvd = nullptr;
PxScene* PhysicSystem::scene = nullptr;
PxPvdSceneClient* PhysicSystem::pvdClient = nullptr;
PhysicEventCallback* PhysicSystem::physicEventCallback = new PhysicEventCallback();

static PxFilterFlags filterShader(
    PxFilterObjectAttributes attributes0,
    PxFilterData filterData0,
    PxFilterObjectAttributes attributes1,
    PxFilterData filterData1,
    PxPairFlags& pairFlags,
    const void* constantBlock,
    PxU32 constantBlockSize)
{
	pairFlags = PxPairFlag::eSOLVE_CONTACT | PxPairFlag::eDETECT_DISCRETE_CONTACT
		| PxPairFlag::eNOTIFY_TOUCH_FOUND
		| PxPairFlag::eNOTIFY_TOUCH_PERSISTS
		| PxPairFlag::eNOTIFY_CONTACT_POINTS
		| PxPairFlag::eTRIGGER_DEFAULT;
    return PxFilterFlag::eDEFAULT;
}

void PhysicSystem::Init()
{
	foundation = PxCreateFoundation(PX_PHYSICS_VERSION, physxAllocator, physxErrorCallback);
	ZASSERT(foundation != nullptr, "PxCreateFoundation failed !");

	PxTolerancesScale scale = PxTolerancesScale();

	pvd = PxCreatePvd(*foundation);
	ZASSERT(pvd != nullptr, "PxCreatePvd failed !");
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation,
		scale, true, pvd);
	ZASSERT(physics != nullptr, "PxCreatePhysics failed !");
	PxInitExtensions(*physics, pvd);

	cooking = PxCreateCooking(PX_PHYSICS_VERSION, *foundation, PxCookingParams(scale));
	ZASSERT(cooking != nullptr, "PxCreateCooking failed !");
	InitScene();

	PxPvdSceneClient* pvdClient = scene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
	}

}

void PhysicSystem::InitScene()
{
	PxTolerancesScale scale = PxTolerancesScale();
	PxSceneDesc sceneDesc(scale);
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);

	if (!sceneDesc.cpuDispatcher)
	{
		PxDefaultCpuDispatcher* mCpuDispatcher = PxDefaultCpuDispatcherCreate(1);
		if (!mCpuDispatcher)
			std::cerr << "PxDefaultCpuDispatcherCreate failed!";
		sceneDesc.cpuDispatcher = mCpuDispatcher;
	}

	sceneDesc.filterShader = filterShader;

	sceneDesc.simulationEventCallback = physicEventCallback;
	sceneDesc.flags = PxSceneFlag::eENABLE_ACTIVE_ACTORS | PxSceneFlag::eENABLE_CCD;

	scene = physics->createScene(sceneDesc);
	ZASSERT(scene != nullptr, "CreateScene failed !");

	pvdClient = scene->getScenePvdClient();
	ZASSERT(pvdClient != nullptr, "getScenePvdClient failed !");
}

void PhysicSystem::Destroy()
{
	//TODO destroy chunks
	scene->release();
	cooking->release();
	physics->release();
	pvd->release();
	foundation->release();
	delete physicEventCallback;
}