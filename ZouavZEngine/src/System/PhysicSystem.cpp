#include "System/Debug.hpp"
#include "PxPhysicsAPI.h"
#include "pvd/PxPvd.h"
#include "System/PhysicSystem.hpp"

using namespace physx;

PxDefaultAllocator PhysicSystem::physxAllocator;
PxDefaultErrorCallback PhysicSystem::physxErrorCallback;

PhysicSystem::PhysicSystem()
{
	Init();
}

PhysicSystem::~PhysicSystem()
{
	Destroy();
}

void PhysicSystem::Init()
{
	foundation = PxCreateFoundation(PX_PHYSICS_VERSION, physxAllocator, physxErrorCallback);
	ZASSERT(foundation != nullptr, "PxCreateFoundation failed !");

	PxTolerancesScale scale = PxTolerancesScale();

	physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation,
		scale, true, nullptr);
	ZASSERT(physics != nullptr, "PxCreatePhysics failed !");

	cooking = PxCreateCooking(PX_PHYSICS_VERSION, *foundation, PxCookingParams(scale));
	ZASSERT(cooking != nullptr, "PxCreateCooking failed !");

	PxSceneDesc sceneDesc(scale);
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	if (!sceneDesc.cpuDispatcher)
	{
		PxDefaultCpuDispatcher* mCpuDispatcher = PxDefaultCpuDispatcherCreate(1);
		if (!mCpuDispatcher)
			std::cerr << "PxDefaultCpuDispatcherCreate failed!";
		sceneDesc.cpuDispatcher = mCpuDispatcher;
		mCpuDispatcher->release();
	}
	if (!sceneDesc.filterShader)
		sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	scene = physics->createScene(sceneDesc);
	ZASSERT(scene != nullptr, "CreateScene failed !");
}

void PhysicSystem::Destroy()
{
	scene->release();
	cooking->release();
	physics->release();
	foundation->release();
}