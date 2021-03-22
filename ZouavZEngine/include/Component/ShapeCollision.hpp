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
	ShapeCollision(GameObject* _gameObject, float _density = 1.0f);
	~ShapeCollision();

	void releasePhysXComponent();
	physx::PxActor* actor;
	float density;

private:
	

protected:
	physx::PxMaterial* material;
};