#include "Component/CapsuleCollision.hpp"
#include "GameObject.hpp"
#include "PxRigidBody.h"
#include "PxRigidStatic.h"
#include "foundation/PxTransform.h"
#include "PxMaterial.h"
#include "System/PhysicSystem.hpp"
#include "PxScene.h"
#include "extensions/PxSimpleFactory.h"
#include "System/PhysicUtils.hpp"

using namespace physx;

CapsuleCollision::CapsuleCollision(GameObject* _gameObject, float _radius, float _halfHeight)
	: ShapeCollision(_gameObject), radius(_radius), halfHeight(_halfHeight)
{
	material = PhysicSystem::physics->createMaterial(0.5f, 0.5f, 0.1f);

	shape = PhysicSystem::physics->createShape(PxCapsuleGeometry(radius, halfHeight), *material);

	AttachToRigidComponent();
}

CapsuleCollision::~CapsuleCollision()
{
}