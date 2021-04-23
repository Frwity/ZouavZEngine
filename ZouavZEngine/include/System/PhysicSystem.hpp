#pragma once

#include "extensions/PxDefaultAllocator.h"
#include "extensions/PxDefaultErrorCallback.h"
#include "PxFoundation.h"
#include "PxPhysics.h"
#include "cooking/PxCooking.h"
#include "PxSimulationEventCallback.h"
#include "PxRigidActor.h"
#include "System/Debug.hpp"

#include "GameObject.hpp"
#include "Component/RigidBody.hpp"
#include "Component/ScriptComponent.hpp"

namespace physx
{
	class PxFoundation;
	class PxPhysics;
	class PxCooking;
	class PxScene;
	class PxPvdSceneClient;
	class PxRigidActor;
	class PxPvdTransport;
}

class PhysicEventCallback : public physx::PxSimulationEventCallback
{
public:
	void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override { PX_UNUSED(constraints); PX_UNUSED(count); }
	void onWake(physx::PxActor** actors, physx::PxU32 count) override { Debug::Log("AWAKE !"); PX_UNUSED(actors); PX_UNUSED(count); }
	void onSleep(physx::PxActor** actors, physx::PxU32 count) override { Debug::Log("SLEEP !");  PX_UNUSED(actors); PX_UNUSED(count); }
	void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override { Debug::Log("Trigger");  PX_UNUSED(pairs); PX_UNUSED(count); }
	void onAdvance(const physx::PxRigidBody* const*, const physx::PxTransform*, const physx::PxU32) override { Debug::Log("Avance !"); }
	void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override
	{
		/*RigidBody* rb1 = static_cast<RigidBody*>(pairHeader.actors[0]->userData);
		RigidBody* rb2 = static_cast<RigidBody*>(pairHeader.actors[1]->userData);

		if (rb1 && rb2)
		{
			rb1->OnContact(rb2->gameObject);
			rb2->OnContact(rb1->gameObject);
		}

		PX_UNUSED((pairs));
		PX_UNUSED((nbPairs));*/
	}

public:
	PhysicEventCallback() = default;
	~PhysicEventCallback() = default;
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
	static physx::PxPvdTransport* transport;
	//static PhysicEventCallback* physicEventCallback;

	PhysicSystem() = delete;
	~PhysicSystem() = delete;

	static void Init();
	static void InitScene();
	static void Destroy();
	static void ResetScene();
};