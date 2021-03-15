#pragma once
#include "Component.hpp"

namespace physx
{
	class PxActor;
}

class ShapeCollision: public Component
{
public:
	ShapeCollision(GameObject* _gameObject);
	~ShapeCollision();
protected:
	physx::PxActor* actor;
};