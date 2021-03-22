#include "Component/CapsuleCollision.hpp"
#include "GameObject.hpp"
#include "PxRigidBody.h"
#include "PxRigidDynamic.h"
#include "PxShape.h"
#include "foundation/PxTransform.h"
#include "PxMaterial.h"
#include "System/PhysicSystem.hpp"
#include "PxScene.h"
#include "extensions/PxSimpleFactory.h"
#include "System/PhysicUtils.hpp"

using namespace physx;

CapsuleCollision::CapsuleCollision(GameObject* _gameObject, float _radius, float _halfHeight, float _density)
	: ShapeCollision(_gameObject, _density), radius(_radius), halfHeight(_halfHeight)
{
	PxTransform t(PxVec3FromVec3(gameObject->WorldPosition()), PxQuatFromQuaternion(gameObject->WorldRotation()));

	material = PhysicSystem::physics->createMaterial(0.5f, 0.5f, 0.1f);

	actor = PxCreateDynamic(*PhysicSystem::physics, t, PxCapsuleGeometry(radius, halfHeight), *material, density);

	actor->userData = gameObject;

	PhysicSystem::scene->addActor(*actor);
}

CapsuleCollision::~CapsuleCollision()
{
}