#pragma once

#include "extensions/PxDefaultAllocator.h"
#include "extensions/PxDefaultErrorCallback.h"
#include "PxFoundation.h"
#include "PxPhysics.h"
#include "cooking/PxCooking.h"
#include "PxSimulationEventCallback.h"
#include "PxRigidActor.h"
#include "PxRigidDynamic.h"
#include "PxRigidStatic.h"
#include "System/Debug.hpp"
#include "Terrain.hpp"

#include "GameObject.hpp"
#include "Component/Rigid.hpp"
#include "Component/RigidStatic.hpp"
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
	class PxRigidDynamic;
	class PxRigidStatic;
}

class PhysicEventCallback : public physx::PxSimulationEventCallback
{
public:
	void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override { PX_UNUSED(constraints); PX_UNUSED(count); }
	void onWake(physx::PxActor** actors, physx::PxU32 count) override { Debug::Log("AWAKE !"); PX_UNUSED(actors); PX_UNUSED(count); }
	void onSleep(physx::PxActor** actors, physx::PxU32 count) override { std::cout << "SLEEP !" << std::endl;  PX_UNUSED(actors); PX_UNUSED(count); }
	void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override { Debug::Log("Trigger");  PX_UNUSED(pairs); PX_UNUSED(count); }
	void onAdvance(const physx::PxRigidBody* const*, const physx::PxTransform*, const physx::PxU32) override { Debug::Log("Avance !"); }
	void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override
	{
		physx::PxRigidActor* rd = pairHeader.actors[0]->is<physx::PxRigidActor>();
		physx::PxRigidActor* rd2 = pairHeader.actors[1]->is<physx::PxRigidActor>();

		if (rd && rd2)
		{
			Rigid* rb1 = static_cast<Rigid*>(rd->userData);
			Rigid* rb2 = static_cast<Rigid*>(rd2->userData);

			rb1->OnContact(&rb2->GetGameObject());
			rb2->OnContact(&rb1->GetGameObject());
		}

		PX_UNUSED((pairs));
		PX_UNUSED((nbPairs));
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
};