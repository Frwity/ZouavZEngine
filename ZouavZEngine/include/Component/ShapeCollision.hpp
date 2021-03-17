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

	void releasePhysXComponent();
	physx::PxActor* actor;
protected:
	physx::PxShape* shape;
	physx::PxMaterial* material;
};