#include "Component/Plane.hpp"
#include "GameObject.hpp"
#include "extensions/PxSimpleFactory.h"
#include "System/PhysicSystem.hpp"
#include "imgui.h"

using namespace physx;

Plane::Plane(GameObject* _gameObject)
	: ShapeCollision(_gameObject)
{
	material = PhysicSystem::physics->createMaterial(0.5f, 0.5f, 0.1f);
	
	shape = PhysicSystem::physics->createShape(PxPlaneGeometry(), *material);

	AttachToRigidComponent();
}

Plane::~Plane()
{

}

void Plane::Editor()
{
}