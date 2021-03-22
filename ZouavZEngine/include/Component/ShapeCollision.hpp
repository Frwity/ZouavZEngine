#pragma once
#include "Component.hpp"
#include "PxSimulationEventCallback.h"
#include "System/Debug.hpp"

namespace physx
{
	class PxActor;
	class PxShape;
	class PxMaterial;
}

class ShapeCollision: public Component
{
public:
	ShapeCollision(GameObject* _gameObject);
	~ShapeCollision();

	void releasePhysXComponent();
	physx::PxActor* actor;

private:
	

protected:
	physx::PxShape* shape;
	physx::PxMaterial* material;
};