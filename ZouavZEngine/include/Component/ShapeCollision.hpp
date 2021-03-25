#pragma once
#include "Component.hpp"
#include "PxSimulationEventCallback.h"
#include "System/Debug.hpp"

namespace physx
{
	class PxActor;
	class PxMaterial;
}

class ShapeCollision: public Component
{
public:
	physx::PxShape* shape = nullptr;
	bool isAttach = false;

	ShapeCollision(GameObject* _gameObject);
	~ShapeCollision();

	void releasePhysXComponent();

protected:
	physx::PxMaterial* material = nullptr;
	void AttachToRigidComponent();
};