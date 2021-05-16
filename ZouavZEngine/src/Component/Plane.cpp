#include "Component/Plane.hpp"
#include "GameObject.hpp"
#include "extensions/PxSimpleFactory.h"
#include "System/PhysicSystem.hpp"
#include "imgui.h"

using namespace physx;

Plane::Plane(GameObject* _gameObject)
	: ShapeCollision(_gameObject)
{
	shape = PhysicSystem::physics->createShape(PxPlaneGeometry(), *material);

	AttachToRigidComponent();
}

Plane::Plane(const Plane& _other)
	: ShapeCollision(_other)
{
	shape = PhysicSystem::physics->createShape(PxPlaneGeometry(), *material);

	AttachToRigidComponent();
}

Plane::~Plane()
{

}

void Plane::Editor()
{
}