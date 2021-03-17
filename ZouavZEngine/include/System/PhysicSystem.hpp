#pragma once

#include "extensions/PxDefaultAllocator.h"
#include "extensions/PxDefaultErrorCallback.h"
#include "PxFoundation.h"
#include "PxPhysics.h"
#include "cooking/PxCooking.h"

namespace physx
{
	class PxFoundation;
	class PxPhysics;
	class PxCooking;
	class PxScene;
}

class PhysicSystem
{
private:
	static physx::PxDefaultAllocator physxAllocator;
	static physx::PxDefaultErrorCallback physxErrorCallback;

public:
	static physx::PxFoundation* foundation;
	static physx::PxPhysics* physics;
	static physx::PxCooking* cooking;
	static physx::PxScene* scene;

	PhysicSystem() = delete;
	~PhysicSystem() = delete;

	static void Init();
	static void Destroy();
	static void DestroyCollisionComponent();
};