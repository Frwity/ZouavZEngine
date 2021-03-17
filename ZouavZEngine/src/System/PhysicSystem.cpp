#include "System/Debug.hpp"
#include "PxPhysicsAPI.h"
#include "pvd/PxPvd.h"
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
PxScene* PhysicSystem::scene = nullptr;

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
	}

	if (!sceneDesc.filterShader)
		sceneDesc.filterShader = PxDefaultSimulationFilterShader;

	sceneDesc.flags = PxSceneFlag::eENABLE_ACTIVE_ACTORS;

	scene = physics->createScene(sceneDesc);
	ZASSERT(scene != nullptr, "CreateScene failed !");
}

void PhysicSystem::DestroyCollisionComponent()
{
	PhysicSystem::scene->simulate(1.0f);
	PhysicSystem::scene->fetchResults(true);

	physx::PxU32 nbActiveActor;

	physx::PxActor** activeActors = PhysicSystem::scene->getActiveActors(nbActiveActor);

	for (int i = 0; i < nbActiveActor; i++)
	{
		GameObject* go = static_cast<GameObject*>(activeActors[i]->userData);

		ShapeCollision* collision = go->GetComponent<ShapeCollision>();
		collision->releasePhysXComponent();
	}
}

void PhysicSystem::Destroy()
{
	DestroyCollisionComponent();
	scene->release();
	cooking->release();
	physics->release();
	foundation->release();
}