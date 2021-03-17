#pragma once
#include "Component.hpp"

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
protected:
	physx::PxActor* actor;
	physx::PxShape* shape;
	physx::PxMaterial* material;
};