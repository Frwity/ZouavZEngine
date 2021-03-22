#pragma once

#include "extensions/PxDefaultAllocator.h"
#include "extensions/PxDefaultErrorCallback.h"
#include "PxFoundation.h"
#include "PxPhysics.h"
#include "cooking/PxCooking.h"
#include "PxSimulationEventCallback.h"
#include "System/Debug.hpp"

namespace physx
{
	class PxFoundation;
	class PxPhysics;
	class PxCooking;
	class PxScene;
	class PxPvdSceneClient;
}

class PhysicEventCallback : public physx::PxSimulationEventCallback
{
	void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) { PX_UNUSED(constraints); PX_UNUSED(count); }
	void onWake(physx::PxActor** actors, physx::PxU32 count) { Debug::Log("AWAKE !"); PX_UNUSED(actors); PX_UNUSED(count); }
	void onSleep(physx::PxActor** actors, physx::PxU32 count) { Debug::Log("SLEEP !");  PX_UNUSED(actors); PX_UNUSED(count); }
	void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) { Debug::Log("Trigger");  PX_UNUSED(pairs); PX_UNUSED(count); }
	void onAdvance(const physx::PxRigidBody* const*, const physx::PxTransform*, const physx::PxU32) { Debug::Log("Avance !"); }
	void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
	{
		Debug::Log("Contact !");

		PX_UNUSED((pairHeader));
		std::vector<physx::PxContactPairPoint> contactPoints;

		for (physx::PxU32 i = 0; i < nbPairs; i++)
		{
			physx::PxU32 contactCount = pairs[i].contactCount;
			if (contactCount)
			{
				contactPoints.resize(contactCount);
				pairs[i].extractContacts(&contactPoints[0], contactCount);

				for (physx::PxU32 j = 0; j < contactCount; j++)
				{
					gContactPositions.push_back(contactPoints[j].position);
					gContactImpulses.push_back(contactPoints[j].impulse);
				}
			}
		}
	}

public:
	PhysicEventCallback() = default;
	~PhysicEventCallback() = default;
	std::vector<physx::PxVec3> gContactPositions;
	std::vector<physx::PxVec3> gContactImpulses;

};

class PhysicSystem
{
private:
	static physx::PxDefaultAllocator physxAllocator;
	static physx::PxDefaultErrorCallback physxErrorCallback;

public:
	static physx::PxFoundation* foundation;
	static physx::PxPvd* pvd;
	static physx::PxPhysics* physics;
	static physx::PxCooking* cooking;
	static physx::PxScene* scene;
	static physx::PxPvdSceneClient* pvdClient;
	static PhysicEventCallback* physicEventCallback;

	PhysicSystem() = delete;
	~PhysicSystem() = delete;

	static void Init();
	static void Destroy();
	static void DestroyCollisionComponent();
};