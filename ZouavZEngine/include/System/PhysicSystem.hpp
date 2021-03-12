#pragma once

#include "extensions/PxDefaultAllocator.h"
#include "extensions/PxDefaultErrorCallback.h"
#include "PxFoundation.h"
#include "PxPhysics.h"
#include "cooking/PxCooking.h"

class PhysicSystem
{
private:
	static physx::PxDefaultAllocator physxAllocator;
	static physx::PxDefaultErrorCallback physxErrorCallback;

public:
	physx::PxFoundation* foundation;
	physx::PxPhysics* physics;
	physx::PxCooking* cooking;
	physx::PxScene* scene;

	PhysicSystem();
	~PhysicSystem();

	void Init();
	void Destroy();
};